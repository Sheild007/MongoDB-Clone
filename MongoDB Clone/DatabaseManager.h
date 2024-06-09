
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <WinSock2.h>
#include<mutex>
#pragma once


#include"MongoDB.h"
#include<vector>
#include<string>
#include<iostream>
#include<fstream>
#include<conio.h>
using namespace std;




class DatabaseManager
{
	unordered_map<string, MongoDB> databases;
	string safefile = "safe.txt";
	
	void create_database(string name)
	{
		databases[name] = MongoDB(name);
		safedatabases();
			
	}
	void load_database()
	{
		ifstream  rdr(safefile);
		if (!rdr)
		{
			cout << "Loading database failed" << endl;
			return;
		}
	     //this->databases["pharma"] = MongoDB(rdr);
		string metadata;
		while (getline(rdr, metadata))
		{
			if (metadata == "")
				break;
			stringstream ss(metadata);
			ss >> metadata;
			ifstream rdr(metadata);
			if (!rdr)
			{
				cout << "Loading database failed" << endl;
				return;
			}
			string name = metadata.substr(0, metadata.find("_metadata.txt"));
			this->databases[name] = MongoDB(metadata.c_str());
			
		}
		rdr.close();


	}
	void show_databases()
	{
		for (auto it = databases.begin(); it != databases.end(); it++)
		{
			cout << it->first << endl;
		}
		
	}
	void drop_database(string name)
	{
		databases.erase(name);
		
	}
	bool get_database(MongoDB& db,string name)
	{
		if (databases.find(name) != databases.end())
		{
			db = databases[name];
			return true;
		}
		else
		{
			cout << "database not found" << endl;
			return false;
		}
		
		
	}
	
	void safedatabases()
	{
		ofstream wrt(safefile);
		if (!wrt)
		{
		cout << "saving databases failed" << endl;
			return;
		}
		string metadata;
		wrt.clear();
	for (auto it = databases.begin(); it != databases.end(); it++)
	{
		metadata = "";
		metadata += it->second.get_name() + "_metadata.txt" + "\n";
		wrt << metadata;
	}
		wrt.close();
	}
	void use_database(string name)
	{
		
	     
		if (databases.find(name) != databases.end())
		{
			MongoDB& db = databases[name];
			string command;
			string query;
			while (true)
			{
				cout << db.get_name() << ">>";
				getline(cin, query);
				if(query.find(".")!=string::npos)
				command = query.substr(0, query.find("."));
				else
					command = query;
				/*
				db.createCollection("students");
				db.showCollections();
				db.dropCollection("students");
				db.importCollection("C:\\Users\\HP\\Desktop\\students.json");
				
				
				*/
				
				if (command == "db")
				{
					int start= query.find(".");
					int end = query.find("(");
					command = query.substr(start + 1, end - start - 1);
					
					
					if (command == "createCollection")
					{
						start = end+1;
						end=query.find(")")-1;
						command = query.substr(start + 1, end - start - 1);
						db.create_collection(command);
						
					}
					else if (command == "importCollection")
					{
						

							start = end+1;
							end = query.find(")")-1;
							command = query.substr(start + 1, end - start - 1);
							const char* path = command.c_str();
							db.import_collection(path);
							cout << "imported" << endl;

						
					}
					else if (command == "dropCollection")
					{

						
						start = end+1;
						end = query.find(")")-1;
						command = query.substr(start + 1, end - start - 1);

							db.drop_collection(command);
							cout << "dropped" << endl;
						
					}
					else if (command == "showCollections")
					{
						
							db.show_collections();
						
					}
					else if (command.find("insertOne") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						if(db.check_collection(collection))
						{
							
							start = end+1;
							end = query.find(")")-1;
							command = query.substr(start + 1, end - start - 1);
							db.insert_one(collection, command);
							
							
					
						}
						else
						{
							cout << "collection not found" << endl;
						}
						
						
					}
					else if (command.find("insertMany") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						
							start = end + 1;
							end = query.find(")") - 1;
							command = query.substr(start + 1, end - start - 1);
							db.insert_many(collection, command);



						
						
					}
					else if (command.find("findOne") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.find_one(collection, command);

					}
					else if (command.find("find") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.find(collection,command);
						
					}
					
					else if (command.find("updateOne") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.update_one(collection, command);
					
					}
					else if (command.find("updateMany") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.update_many(collection, command);
					}
					else if (command.find("deleteOne") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.delete_one(collection, command);
					}
					else if (command.find("deleteMany") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.delete_many(collection, command);
					}
					else if (command.find("dropIndex") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.dropIndex(collection, command);
					}
					else if (command.find("createIndex") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.createIndex(collection, command);
					}
					else if (command.find("count") != string::npos)
					{
						start = end;
						end = query.find(")");
						command = query.substr(start + 1, end - start - 1);
						//db.count(command);
					}
					else if (command.find("sort") != string::npos)
					{

					}

				}
				else if (command == "cls")
				{
					cout << "\x1B[2J\x1B[H";
				}
				else if (command == "exit")
				{
					exit(1);
				}
				else if (command == "back")
				{
					break;
				}
				
					
				
			}
		}
		else
			return;

	}

	string use_database_with_query(string name,string query)
	{
		
		if (databases.find(name) != databases.end())
		{
			MongoDB& db = databases[name];
			string command;
			
			
				cout << db.get_name() << ">>";
				
				if (query.find(".") != string::npos)
					command = query.substr(0, query.find("."));
				else
					command = query;
				/*
				db.createCollection("students");
				db.showCollections();
				db.dropCollection("students");
				db.importCollection("C:\\Users\\HP\\Desktop\\students.json");


				*/

				if (command == "db")
				{
					int start = query.find(".");
					int end = query.find("(");
					command = query.substr(start + 1, end - start - 1);


					if (command == "createCollection")
					{
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.create_collection(command);
						return "1";

					}
					else if (command == "importCollection")
					{


						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						const char* path = command.c_str();
						db.import_collection(path);
						cout << "imported" << endl;
						return "1";


					}
					else if (command == "dropCollection")
					{


						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);

						db.drop_collection(command);
						cout << "dropped" << endl;
						return "1";

					}
					else if (command == "showCollections")
					{

						string result = db.getCollectionsName();
						return result;

					}
					else if (command.find("insertOne") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						if (db.check_collection(collection))
						{

							start = end + 1;
							end = query.find(")") - 1;
							command = query.substr(start + 1, end - start - 1);
							db.insert_one(collection, command);
							return "1";

						}
						else
						{
							return "collection not found";
						}


					}
					else if (command.find("insertMany") != string::npos)
					{
						string collection = command.substr(0, command.find("."));

						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.insert_many(collection, command);
						return "1";




					}
					else if (command.find("findOne") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.find_one(collection, command);

					}
					else if (command.find("find") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
					    return db.get_filter_docs(collection, command);

					}

					else if (command.find("updateOne") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.update_one(collection, command);
						return "1";

					}
					else if (command.find("updateMany") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.update_many(collection, command);
						return "1";
					}
					else if (command.find("deleteOne") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.delete_one(collection, command);
						return "1";
					}
					else if (command.find("deleteMany") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.delete_many(collection, command);
						return "1";
					}
					else if (command.find("dropIndex") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.dropIndex(collection, command);
						return "1";
					}
					else if (command.find("createIndex") != string::npos)
					{
						string collection = command.substr(0, command.find("."));
						start = end + 1;
						end = query.find(")") - 1;
						command = query.substr(start + 1, end - start - 1);
						db.createIndex(collection, command);
						return "1";
					}
					else if (command.find("count") != string::npos)
					{
						start = end;
						end = query.find(")");
						command = query.substr(start + 1, end - start - 1);
						//db.count(command);
					}
					else if (command.find("sort") != string::npos)
					{

					}

				}
				else if (command == "cls")
				{
					cout << "\x1B[2J\x1B[H";
				}
				else if (command == "exit")
				{
					exit(1);
				}
			



			
		}
		else
		{
			use = false;
			return "database not found";
		}

	}
	bool use = false;
	string use_database_name="";

public:
	DatabaseManager()
	{
	  load_database();

		

	}
	
	~DatabaseManager()
	{
		databases.clear();
	}
	
	void manage()
	{
		string query;
		while (true)
		{
		cout << ">>";
		getline(cin, query);
		string command = query.substr(0, query.find(" "));
			if (command == "create")
			{
				command = query.substr(query.find(" ") + 1, query.find(" ", query.find(" ") + 1) - query.find(" ") - 1);
				if (command == "db")
				{
					command = query.substr(query.find(" ", query.find(" ") + 1) + 1, query.length() - query.find(" ", query.find(" ") + 1) - 1);
					create_database(command);
				}
			}
			else if(command=="load")
			{
				command = query.substr(query.find(" ") + 1, query.length() - query.find(" ") - 1);
				if (command == "dbs")
				{
					load_database();
				}
			}
			else if (command == "show")
			{
				command = query.substr(query.find(" ") + 1, query.length() - query.find(" ") - 1);
				if (command == "dbs")
				{
					show_databases();
				}
			}
			else if (command == "drop")
			{
				command = query.substr(query.find(" ") + 1, query.find(" ", query.find(" ") + 1) - query.find(" ") - 1);
				if (command == "database")
				{
					
					command = query.substr(query.find(" ", query.find(" ") + 1) + 1, query.length() - query.find(" ", query.find(" ") + 1) - 1);
					drop_database(command);
				}
			}
			else if (command == "use")
			{
				command = query.substr(query.find(" ") + 1, query.length() - query.find(" ") - 1);
				
				use_database(command);
			}
			else if (command == "cls")
			{
				cout << "\x1B[2J\x1B[H";
			}
			else if (command == "help")
			{
				cout << "create db name" << endl;
				cout << "show dbs" << endl;
				cout << "drop database" << endl;
				cout << "use database" << endl;
				cout << "cls" << endl;
				cout << "exit" << endl;
			}
			else if (command == "exit")
			{
				exit(0);
			}
	
		}
	}
	
	string queryProcessor(string query)
	{
		string command = query.substr(0, query.find(" "));
		if (command == "create")
		{
			command = query.substr(query.find(" ") + 1, query.find(" ", query.find(" ") + 1) - query.find(" ") - 1);
			if (command == "db")
			{
				command = query.substr(query.find(" ", query.find(" ") + 1) + 1, query.length() - query.find(" ", query.find(" ") + 1) - 1);
				create_database(command);
				return "1";
			}
		}
		else if (command == "load")
		{
			command = query.substr(query.find(" ") + 1, query.length() - query.find(" ") - 1);
			if (command == "dbs")
			{
				load_database();
				cout << "loaded" << endl;
				return "1";
			}
		}
		else if (command == "show")
		{
			command = query.substr(query.find(" ") + 1, query.length() - query.find(" ") - 1);
			if (command == "dbs")
			{
				show_databases();
				return "1";
			}
		}
		else if (command == "drop")
		{
			command = query.substr(query.find(" ") + 1, query.find(" ", query.find(" ") + 1) - query.find(" ") - 1);
			if (command == "database")
			{

				command = query.substr(query.find(" ", query.find(" ") + 1) + 1, query.length() - query.find(" ", query.find(" ") + 1) - 1);
				drop_database(command);
				return "1";
			}
		}
		else if (command == "use")
		{
			command = query.substr(query.find(" ") + 1, query.length() - query.find(" ") - 1);
			use_database_name = command;
			use = true;
			return "1";

		}
		else if (command == "cls")
		{
			cout << "\x1B[2J\x1B[H";

		}
		else if (command == "help")
		{
			cout << "create db name" << endl;
			cout << "show dbs" << endl;
			cout << "drop database" << endl;
			cout << "use database" << endl;
			cout << "cls" << endl;
			cout << "exit" << endl;
		}
		else if (command == "exit")
		{
			exit(0);
		}


		if (use)
		{
			return use_database_with_query(use_database_name, query);
		}
									
	
	}



		/*
		* CRUD
		* Create
		* Read
		* Update
		* Delete 
		
		*/




};

//int main()
//{
//	DatabaseManager db;
//	db.manage();
//
//	return 0;
//}


#endif // !DATABASEMANAGER_H
