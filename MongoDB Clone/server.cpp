#include"DatabaseManager.h"



queue<pair<SOCKET, vector<string>>> processingQueue;
mutex queueMutex;


// Inside the ClientThread function
DWORD WINAPI ClientThread(LPVOID clientSocket)
{
    SOCKET client = (SOCKET)clientSocket;

    // Receive query from client
    char buffer[1024];
    int bytesReceived = recv(client, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0'; // Null-terminate the received data
        string query = string(buffer);

        // Push the received query into the processing queue
        queueMutex.lock();
        processingQueue.push({ client, {query} });
        queueMutex.unlock();
    }

    return 0;
}


// Inside the WorkerThread function
DWORD WINAPI WorkerThread(LPVOID)
{
     DatabaseManager db;
      while (true) {
          // Check if there are queries in the processing queue
          queueMutex.lock();
          if (!processingQueue.empty()) {
              pair<SOCKET, vector<string>> queryData = processingQueue.front();
              processingQueue.pop();
              queueMutex.unlock();

              SOCKET clientSocket = queryData.first;
              string query = queryData.second[0]; // Assuming a single query per request

              // Process the query using your database query processor
              string response = db.queryProcessor(query);
             
            //  string response = " yayyyyy"; // Implement this function

              // Send the query result back to the client
              int responseSize = static_cast<int>(response.size());
              send(clientSocket, reinterpret_cast<char*>(&responseSize), sizeof(responseSize), 0);
              

              // Send the response string in 4KB buffer chunks if size > 4KB
             
              int bytesSent = 0;
              int chunkSize = 4096;
              while (bytesSent < responseSize) 
              {
                  int remainingBytes = responseSize - bytesSent;
                  int currentChunkSize = (remainingBytes < chunkSize) ? remainingBytes : chunkSize;
                  send(clientSocket, response.c_str() + bytesSent, currentChunkSize, 0);
                  bytesSent += currentChunkSize;
              }
          }
          else {
              queueMutex.unlock();
              Sleep(100); // Sleep to avoid high CPU usage when the queue is empty
          }
      }

    return 0;
}


int main1()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0)
    {
        cerr << "WSAStartup failed with error: " << result << endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSocket == INVALID_SOCKET)
    {
        cerr << "Error creating socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    result = ::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

    if (result == SOCKET_ERROR)
    {
        cerr << "Bind failed with error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    result = listen(serverSocket, SOMAXCONN);

    if (result == SOCKET_ERROR)
    {
        cerr << "Listen failed with error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    cout << "Server is listening for connections..." << endl;

    DWORD workerThreadId;
    HANDLE workerThread = CreateThread(NULL, 0, WorkerThread, NULL, 0, &workerThreadId);


    while (true)
    {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);

        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        if (clientSocket == INVALID_SOCKET)
        {
            cerr << "Accept failed with error: " << WSAGetLastError() << endl;
            closesocket(serverSocket);
            WSACleanup();
            return 1;
        }

        DWORD threadId;
        HANDLE clientThread = CreateThread(NULL, 0, ClientThread, (LPVOID)clientSocket, 0, &threadId);
        CloseHandle(clientThread);
    }

    return 0;
}
int main()

{
    	DatabaseManager db;
        db.manage();
        return 0;
	
}