#include <iostream>
#include <fstream>
#include <vector>
#include <string>


using namespace std;
int littleEndianToInt1(const char* bytes) 
{
    int res=(int)((unsigned char)bytes[0]) | ((int)((unsigned char)bytes[1]) << 8) |
        ((int)((unsigned char)bytes[2]) << 16) | ((int)((unsigned char)bytes[3]) << 24);
    return res;
}

void convertBSONtoJSON(const char* filename)
{
    ifstream file(filename, ios::binary);

    if (!file.is_open()) {
        cout << "Unable to open file!";
        return;
    }

 
    char fileSizeBytes[4];
    file.read(fileSizeBytes, 4);
    int fileSize = littleEndianToInt1(fileSizeBytes);

    vector<char> bsonData(fileSize);
    file.read(bsonData.data(), fileSize);

    file.close();

    
    int currentPosition = 0;
    cout << "{" << endl;
    
    while (currentPosition < fileSize)
    {
        
        char dataType = bsonData[currentPosition];
        currentPosition++;

        if (dataType == 0x00) 
            break; 

        
        string fieldName;
        while (bsonData[currentPosition] != 0x00) 
        {
            fieldName += bsonData[currentPosition];
            currentPosition++;
        }
        currentPosition++; 

        
        switch (dataType)
        {
        case 0x01: 
        { // Double
            double doubleValue;
            memcpy(&doubleValue, &bsonData[currentPosition], sizeof(double));
            currentPosition += sizeof(double);
            cout << "\"" << fieldName << "\": " << doubleValue << ", ";
            break;
        }
        case 0x02: 
        { // String
            int stringSize = littleEndianToInt1(&bsonData[currentPosition]);
            currentPosition += 4;
            string stringValue(&bsonData[currentPosition], stringSize - 1); 
            currentPosition += stringSize;
            cout << "\"" << fieldName << "\": \"" << stringValue << "\", ";
            break;
        }
        case 0x04: { // Array
            cout << "\"" << fieldName << "\": [ ";

            
            int arraySize = littleEndianToInt1(&bsonData[currentPosition]);
            currentPosition += 4;

            int arrayEndPosition = currentPosition + arraySize - 5; 

          
            while (currentPosition < arrayEndPosition) 
            {
                
                char elementType = bsonData[currentPosition];
                currentPosition++;

                switch (elementType) {
                case 0x01:
                { //
                    currentPosition += 2;
                    double doubleValue;
                    memcpy(&doubleValue, &bsonData[currentPosition], sizeof(double));
                    currentPosition += sizeof(double);
                    cout << doubleValue << ", ";
                    break;
                }
                case 0x02: { // String
                    currentPosition += 2; // Skip the null byte
                    int stringSize = littleEndianToInt1(&bsonData[currentPosition]);
                    currentPosition += 4;
                    string stringValue(&bsonData[currentPosition], stringSize - 1); // Exclude null terminator
                    currentPosition += stringSize;
                    cout << "\"" << stringValue << "\", ";
                    break;
                }
                case 0x10: { // 32-bit integer
                    currentPosition += 2;
                    int intValue = littleEndianToInt1(&bsonData[currentPosition]);
                    currentPosition += 4;
                    cout << intValue << ", ";
                    break;
                }
                        
                default:
                    cout << "Unhandled array element type: " << static_cast<int>(elementType) << endl;
                    break;
                }
            }

            cout << "], ";
            break;
        }
                 // Add cases for other BSON types
        case 0x05: { // Binary data
            int binarySize = littleEndianToInt1(&bsonData[currentPosition]);
            currentPosition += 4;

            // Skipping binary data handling for simplicity in this example
            currentPosition += binarySize;
            break;
        }
        case 0x08: { // Boolean
            bool boolValue = bsonData[currentPosition] != 0x00;
            currentPosition++;
            cout << "\"" << fieldName << "\": " << (boolValue ? "true" : "false") << ", ";
            break;
        }
        case 0x10: // 32-bit integer
        {
            // 32-bit integer
            int intValue = littleEndianToInt1(&bsonData[currentPosition]);
            currentPosition += 4;
            cout << "\"" << fieldName << "\": " << intValue << ", ";
            break;
        }
        case 0x12: // 64-bit integer
        {
            int intValue = littleEndianToInt1(&bsonData[currentPosition]);
            currentPosition += 4;
            cout << "\"" << fieldName << "\": " << intValue << ", ";
            break;
        }
        case 0x13: // UTC datetime
        {
            int intValue = littleEndianToInt1(&bsonData[currentPosition]);
			currentPosition += 4;
			cout << "\"" << fieldName << "\": " << intValue << ", ";
			break;

        }
        case 0x0A: // Null value
        {
            cout << "\"" << fieldName << "\": null, ";
			break;

        }
        case 0x0D: // JavaScript code
        {
        int intValue = littleEndianToInt1(&bsonData[currentPosition]);
        currentPosition += 4;
        cout << "\"" << fieldName << "\": " << intValue << ", ";
        break;

        }
        
        case 0x11: // 32-bit timestamp
        {
            int intValue = littleEndianToInt1(&bsonData[currentPosition]);
			currentPosition += 4;
			cout << "\"" << fieldName << "\": " << intValue << ", ";
			break;

        }


        default:
            cout << "Unhandled data type: " << static_cast<int>(dataType) << endl;
            break;
        }
    }
    cout << "\n}";
    cout << endl;
}

int main()
{
	convertBSONtoJSON("example.bson");
	return 0;
}
