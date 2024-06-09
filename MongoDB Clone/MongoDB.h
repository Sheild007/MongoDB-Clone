
#ifndef MONGODB_H
#define MONGODB_H
#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<unordered_map>
#include <variant>
#include <cstdint>
#include <cstring>
#include"unordered_Map.h"
#include"Btree.h"
#include"tries.h"
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include"ObjectId.h"
#include<conio.h>
#include<stack>
#include<queue>
#include"indexTree.h"


using namespace std;

#define END 0
#define DOUBLE 1
#define STRING 2
#define OBJECT 3
#define ARRAY 4
#define BINDATA 5
#define UNDEFINED 6
#define OBJECTID 7
#define BOOL 8
#define DATE 9
#define NULLTYPE 10
#define REGEX 11
#define DBPOINTER 12
#define JAVASCRIPT 13
#define SYMBOL 14
#define JAVASCRIPTWITHSCOPE 15
#define INT32 16
#define TIMESTAMP 17
#define INT64 18
#define DECIMAL 19
#define MINKEY -1
#define MAXKEY 127


extern enum types { s_type, i_type, b_type, d_type, l_type, a_type, o_type };
extern  int littleEndianToInt(const char* bytes) {
    return (int)((unsigned char)bytes[0]) | ((int)((unsigned char)bytes[1]) << 8) |
        ((int)((unsigned char)bytes[2]) << 16) | ((int)((unsigned char)bytes[3]) << 24);
}

class JsonValue
{
private:
    variant<string, int, bool, double, long, vector<JsonValue>, unordered_map<string, JsonValue>> m_value;
    int m_type;

public:
    JsonValue() = default;
    JsonValue(const string& value) : m_value(value), m_type(0) {}
    JsonValue(const int& value) : m_value(value), m_type(1) {}
    JsonValue(const bool& value) : m_value(value), m_type(2) {}
    JsonValue(const double& value) : m_value(value), m_type(3) {}
    JsonValue(const long& value) : m_value(value), m_type(4) {}
    JsonValue(const vector<JsonValue>& value) : m_value(value), m_type(5) {}
    JsonValue(const unordered_map<string, JsonValue>& value) : m_value(value), m_type(6) {}
    JsonValue(const JsonValue& value) : m_value(value.m_value), m_type(value.m_type) {}
    JsonValue& operator=(const JsonValue& value) { m_value = value.m_value; m_type = value.m_type; return *this; }
    JsonValue(JsonValue&& value) : m_value(move(value.m_value)), m_type(value.m_type) {}
    JsonValue& operator=(JsonValue&& value) { m_value = move(value.m_value); m_type = value.m_type; return *this; }
    ~JsonValue() = default;

    //template<typename T>
    //T get(const variant<string, int, bool, double, long,vector<JsonValue>, unordered_map<string, vector<JsonValue>>>& value) const { return get<T>(value); }

    string asString() const { return get<string>(m_value); }
    int asInt() const { return get<int>(m_value); }
    bool asBool() const { return get<bool>(m_value); }
    double asDouble() const { return get<double>(m_value); }
    long asLong() const { return get<long>(m_value); }
    unordered_map<string, JsonValue> asObj() const { return get<unordered_map<string, JsonValue>>(m_value); }
    vector<JsonValue> asVector() const { return get<vector<JsonValue>>(m_value); }
    int getType() const { return m_type; }

};

class Filter
{
private:
    variant<string, pair<string ,string>, pair<string,vector<string>>, vector<pair<string,string>>, pair<string,vector<JsonValue>>, pair<string,pair<vector<string>,string>>, pair<string, pair<vector<string>, vector<string>>>, pair<string, pair<vector<string>, vector<JsonValue>>> , vector<string>> value;
    int type;

public:

    Filter() = default;
    Filter(const string& value) : value(value), type(0) {}
    Filter(const pair<string, string>& value) : value(value), type(1) {}
    Filter(const pair<string, vector<string>>& value) : value(value), type(2) {}
    Filter(const vector<pair<string, string>>& value) : value(value), type(3) {}
    Filter(const pair<string, vector<JsonValue>>& value) : value(value), type(4) {}
    Filter(const pair<string, pair<vector<string>, string>>& value) : value(value), type(5) {}
    Filter(const pair<string, pair<vector<string>, vector<string>>>& value) : value(value), type(6) {}
    Filter(const pair<string, pair<vector<string>, vector<JsonValue>>> value) : value(value), type(7) {}
    Filter(const vector<string>& value) : value(value), type(8) {}
    Filter(const Filter& value) : value(value.value), type(value.type) {}
    Filter& operator=(const Filter& value) { this->value = value.value; this->type = value.type; return *this; }
	Filter(Filter&& value) : value(move(value.value)), type(value.type) {}
    Filter& operator=(Filter&& value) { this->value = move(value.value); this->type = value.type; return *this; }
	~Filter() = default;

	
    
	string asString() const { return get<string>(value); }
	pair<string, string> asKey_value() const { return get<pair<string, string>>(value); }
	pair<string, vector<string>> askey_values() const { return get<pair<string, vector<string>>>(value); }
	vector<pair<string, string>> askeys_values() const { return get<vector<pair<string, string>>>(value); }
	pair<string, vector<JsonValue>> askey_vector() const { return get<pair<string, vector<JsonValue>>>(value); }
	pair<string, pair<vector<string>, string>> askey_nestedKey() const { return get<pair<string, pair<vector<string>, string>>>(value); }
	pair<string, pair<vector<string>, vector<string>>> asKey_nested_values() const { return get<pair<string, pair<vector<string>, vector<string>>>>(value); }
    pair<string, pair<vector<string>, vector<JsonValue>>> askey_nested_array() const { return get<pair<string, pair<vector<string>, vector<JsonValue>>>>(value); }
    vector<string> askeys() const { return get<vector<string>>(value); }
    int get_type() const { return type; }


};

class uniqueID
{

 private:
	static size_t ID;
 public:
    static size_t getID()
     {
		return ID++;
	}
};

size_t uniqueID::ID = 7246;
using object = unordered_map<string, JsonValue>;
using ID=size_t;

extern pair<string, string> separateBracketsAndSymbols(const string& inputString) {
    string brackets;
    string symbols;



    for (char ch : inputString)
    {
        if (ch == '{' || ch == '}' || ch == '[' || ch == ']')
        {
            brackets.push_back(ch);
        }
        else if (ch == ':' || ch == ',' || ch == '\"')
        {
            symbols.push_back(ch);
        }



    }

    return make_pair(brackets, symbols);
}
extern bool isBalanced(string str)
{
    stack<char> container;
    for (int i = 0; i < str.size(); i++)
    {
        char bracket = str[i];
        if (bracket == '[' || bracket == '{' || bracket == '(')
            container.push(bracket);
        else
        {
            if (container.empty()) {
                return false;
            }
            else
            {
                char top = container.top();
                container.pop();

                if ((bracket == ')' && top != '(') ||
                    (bracket == ']' && top != '[') ||
                    (bracket == '}' && top != '{'))
                    return false;

            }
        }
    }

    return container.empty();
}
extern  pair<string, string> filter(string input)
{
    pair<string, string>result;
    int si = 0;
    int ei = 0;

    for (int i = 0; i < input.size(); i++)
    {
        char ch = input[i];
        if (ch == ':')
            si = i;

        result.first.push_back(ch);
        if (ch == ',')
        {
            ei = i;
            for (int j = si; j < ei; j++)
            {
                result.first.pop_back();
                result.second.push_back(input[j]);
            }

        }
    }
    while (result.first[result.first.size() - 1] != ':')
    {
        result.first.pop_back();
        result.second.push_back(input[input.size() - 1]);
    }
    string filter;
    for (int i = 0; i < result.second.size(); i++)
    {
        char ch = result.second[i];
        if (ch == '\"')
            filter.push_back(ch);
    }
    result.second = filter;
    return result;
}
extern  string findSymbols(const string& inputString) {
    vector<string> foundSymbols;
    size_t pos = 0;
    string result;

    // Find all occurrences of symbols in the input string
    while ((pos = inputString.find_first_of("{}[]\":,", pos)) != string::npos) {
        foundSymbols.push_back(inputString.substr(pos, 1));
        ++pos;
    }
    for (int i = 0; i < foundSymbols.size(); i++)
    {
        result += foundSymbols[i];

    }
    return result;

}
extern  bool isValid_json(const string& inputString)
{

    string foundSymbols = findSymbols(inputString);
    pair<string, string> result = separateBracketsAndSymbols(foundSymbols);
    pair<string, string> filtered = filter(result.second);

    bool balanced = isBalanced(result.first);
    if (!balanced)
        return false;

    if (filtered.second.size() % 2 != 0)
        return false;
    int count1 = 0, count2 = 0;
    for (char ch : filtered.first)
    {
        if (ch == ':')
            count1++;
        else if (ch == '\"')
            count2++;
    }
    bool colon = (count1 == count2 / 2);
    if (!colon)
        return false;

    for (int i = 0; i < filtered.first.size(); i++)
    {
        char ch = filtered.first[i];
        int count = 0;
        while (ch != ':')
        {
            count++;
            i++;
            ch = filtered.first[i];
        }
        if (count != 2)
            return false;


    }


    return true;


}

class Document
{

private:
	object* document;
	//ID m_id;
    ObjectId _id;
    bool isFirst;
    //string filename;
    //fstream* file;
    bool is_strerilized = false;
    void print_JSON(unordered_map<string, JsonValue> json)
    {

        cout << "{\n";
        if (isFirst)
        {
            //printing id
           cout<< "  _id: " << _id.get_ID() << ",\n";
            isFirst = false;
        }
        
        int size = json.size();
        int count = 0;

        for (auto& [key, value] : json)
        {
            if (value.getType() == 0)
            {
                cout<<" " << key << " : " << "\"" << value.asString() << "\"";
                count++;
            }
            else if (value.getType() == 1)
            {
                cout << " " << key << " : " << value.asInt();
                count++;
            }
            else if (value.getType() == 2)
            {
                cout << " " << key << " : " << (value.asBool() ? "true" : "false");
                count++;
            }
            else if (value.getType() == 3)
            {
                cout << " " << key << ": " << value.asDouble();
                count++;
            }
            else if (value.getType() == 4)
            {
                cout << " " << key << " : " << value.asLong();
                count++;
            }
            else if (value.getType() == 5)
            {
                cout << " " << key << " : [ ";
                vector<JsonValue> array = value.asVector();
                for (int i = 0; i < array.size(); i++)
                {

                    switch (array[i].getType())
                    {
                    case 0:
                    {
                        cout << "\"" << array[i].asString() << "\"";
                        break;
                    }
                    case 1:
                    {
                        cout << array[i].asInt();
                        break;
                    }
                    case 2:
                    {
                        cout << (array[i].asBool() ? "true" : "false");
                        break;
                    }
                    case 3:
                    {
                        cout << array[i].asDouble();
                        break;
                    }
                    case 4:
                    {
                        cout << array[i].asLong();
                        break;
                    }
                    case 6:
                    {

                        unordered_map<string, JsonValue> obj1 = array[i].asObj();
                        print_JSON(obj1);

                    }
                    }
                    if (i != array.size() - 1)
                    {
                        cout << ", ";
                    }
                }
                count++;
                cout << "]";



            }
            else if (value.getType() == 6)
            {
                cout << " " << key << " : ";
                unordered_map<string, JsonValue> obj = value.asObj();
                cout << "{ ";
                print_JSON(obj);
                cout << " }";
                count++;

            }


            if (count != size)
            {
                cout << ",\n  ";
            }
        }
        cout<<"\n}";
        
    }
    
    string get_JSON(unordered_map<string, JsonValue> json)
    {

        stringstream ss;
        ss << "{\n";
        if (isFirst)
        {
            //printing id
            ss << "  _id: " << _id.get_ID() << ",\n";
            isFirst = false;
        }

        int size = json.size();
        int count = 0;

        for (auto& [key, value] : json)
        {
            if (value.getType() == 0)
            {
                ss << " " << key << " : " << "\"" << value.asString() << "\"";
                count++;
            }
            else if (value.getType() == 1)
            {
                ss << " " << key << " : " << value.asInt();
                count++;
            }
            else if (value.getType() == 2)
            {
                ss << " " << key << " : " << (value.asBool() ? "true" : "false");
                count++;
            }
            else if (value.getType() == 3)
            {
                ss << " " << key << ": " << value.asDouble();
                count++;
            }
            else if (value.getType() == 4)
            {
                ss << " " << key << " : " << value.asLong();
                count++;
            }
            else if (value.getType() == 5)
            {
                ss << " " << key << " : [ ";
                vector<JsonValue> array = value.asVector();
                for (int i = 0; i < array.size(); i++)
                {

                    switch (array[i].getType())
                    {
                    case 0:
                    {
                        ss << "\"" << array[i].asString() << "\"";
                        break;
                    }
                    case 1:
                    {
                        ss << array[i].asInt();
                        break;
                    }
                    case 2:
                    {
                        ss << (array[i].asBool() ? "true" : "false");
                        break;
                    }
                    case 3:
                    {
                        ss << array[i].asDouble();
                        break;
                    }
                    case 4:
                    {
                        ss << array[i].asLong();
                        break;
                    }
                    case 6:
                    {

                        unordered_map<string, JsonValue> obj1 = array[i].asObj();
                        print_JSON(obj1);

                    }
                    }
                    if (i != array.size() - 1)
                    {
                        ss << ", ";
                    }
                }
                count++;
                ss << "]";



            }
            else if (value.getType() == 6)
            {
                ss << " " << key << " : ";
                unordered_map<string, JsonValue> obj = value.asObj();
                ss << "{ ";
                ss<<get_JSON(obj);
                ss << " }";
                count++;

            }


            if (count != size)
            {
                ss << ",\n  ";
            }
        }
        ss << "\n}";
        return ss.str();

    }
    JsonValue& insert(string key,JsonValue original, JsonValue chnage)
    {
        if (original.getType() == s_type || original.getType() == i_type ||original.getType() == d_type ||original.getType() == l_type || original.getType() == b_type)
        {
			original = chnage;
            return original;
		}
      
        else if (original.getType() == a_type)
        {
			vector<JsonValue> array = original.asVector();
			vector<JsonValue> array2 = chnage.asVector();
            for (int i = 0; i < array2.size(); i++)
            {
				array.push_back(array2[i]);
			}
			original = array;
		}
        else if (original.getType() == 6)
        {
			unordered_map<string, JsonValue> obj = original.asObj();
			unordered_map<string, JsonValue> obj2 = chnage.asObj();
            for (auto& [key, value] : obj2)
            {
				obj[key] = value;
			}
			original = obj;
		}
		return original;

    }
public:
    Document():_id()
    {
        document = new object();
		
		isFirst = true;

		//filename = "object" + to_string(m_id) + ".bson";
		//file = new fstream(filename, ios::binary | ios::out);



    }
    Document(const object& doc) : document(new object(doc))
    {
       // filename = "object"+to_string(m_id) + ".bson";
        //file=new fstream(filename, ios::binary | ios::out);
    }
    Document(const object& doc, const ObjectId id)
    {
        document = new object(doc);
		_id = id;
		
		isFirst = true;

    }
    Document(const Document& doc) : document(doc.document)
    {
        _id=doc._id;
      //  this->filename=doc.filename;
       // this->file=doc.file;
    }
    Document(Document&& doc) : document(move(doc.document))
    {
      //  this->filename=doc.filename;
		//this->file=doc.file;
    }
    Document& operator=(const Document& doc) 
    { 
        document = doc.document;
       
        _id = doc._id;
      //  filename=doc.filename;
       // file=doc.file;
    return *this; 
    }
    Document& operator=(Document&& doc) 
    {
        document = move(doc.document); 
        
       // filename=doc.filename;
       // file=doc.file;
        return *this;
    }
    ~Document() = default;
    Document(string key, JsonValue value)
    {
        document = new object();
		document->insert({ key, value });
	
        isFirst = true;
	   
    }
    Document(const vector<pair<string, JsonValue>>& values)
    {
		document = new object();
        for (auto& [key, value] : values)
        {
			document->insert({ key, value });
		}
		
		isFirst = true;
	}

    void insert_one(string key, JsonValue value)
    {
		document->insert({ key, value });
	}
    void remove_one(const string& key)
    {
        if(document->find(key)!=document->end())
		document->erase(key);
	}
    void update_one(const string& key, const JsonValue& value)
    {
        if(document->find(key)!=document->end())
		document->at(key) = value;
	}
	bool is_empty() const 
    { 
        return document->empty();
    }
    void insert_many(const vector<pair<string, JsonValue>>& values)
    {
        for (auto& [key, value] : values)
        {
           document->insert({ key, value });
        }

    }

	
	object getDocument() const { return *document; }
	void setDocument(const object& doc) { document = new object(doc); }
	
	void addField(const string& field, const JsonValue& value) { document->insert({ field, value }); }
	void removeField(const string& field) { document->erase(field); }   
	void updateField(const string& field, const JsonValue& value) 
    { 
        if (document->find(field) != document->end())
        {
            JsonValue prev_val = document->at(field);
            document->at(field) = value;
        }
    }
    void clear() { document->clear(); } 
    void view()
    {
        if (!is_strerilized)
        {
            isFirst = true;
            print_JSON(*document);
        }
        else
        cout<<"Document is not in memory"<<endl;    
       

    }
    string get_doc_as_json()
    {
        if (!is_strerilized)
        {
            isFirst = true;
            return get_JSON(*document);
        }
        else
            return "Document is not in memory";


    }
    

    void sterilize()
    {
        
        /*filename = "object" + to_string(m_id) + ".bson";
        string path="mongoDB\\" + filename;
        file = new fstream(path, ios::binary | ios::out);

        if (!file->is_open())
        {
            cout << "Unable to open file for writing!";
            return;
        }
        write_obj(*document, *file);

        file->close();*/
        clear();
        document = nullptr;
        is_strerilized = true;
	}
    
    
    
    friend class Collection;
	
};

class Collection
{
private:
    vector<Document> current;
   
    vector<BTree> documents;
    
    string m_name;
    string filename;
    fstream* File;
    vector<pair<string,IndexTree>> indexes;
    vector<string> index_names;
    string index_key;
 
    
    bool is_strerilized = false;
    string index_name="";
    bool index_created = false;
   
    bool is_doc = true;
    bool is_first = true;
    bool updateMany = false;
   
    int updated = 0;
    int get_sizeof_array(vector<JsonValue> array)
    {
        int byte_size = 0;
        byte_size += sizeof(int);
        for (int i = 0; i < array.size(); i++)
        {

            switch (array[i].getType())
            {
            case 0:
            {
                const string& strValue = array[i].asString();
                int strSize = strValue.size() + 1;
                byte_size += sizeof(char) + sizeof(int) + strSize;
                byte_size += sizeof(char);
                byte_size += sizeof(char);
                break;
            }
            case 1:
            {
                byte_size += sizeof(char) + sizeof(int);
                byte_size += sizeof(char);
                byte_size += sizeof(char);
                break;
            }
            case 2:
            {
                byte_size += sizeof(char) + sizeof(char);
                byte_size += sizeof(char);
                byte_size += sizeof(char);
                break;
            }
            case 3:
            {
                byte_size += sizeof(char) + sizeof(double);
                byte_size += sizeof(char);
                byte_size += sizeof(char);
                break;
            }
            case 4:
            {
                byte_size += sizeof(char) + sizeof(long);
                byte_size += sizeof(char);
                byte_size += sizeof(char);
                break;
            }
            case 6:
            {
                unordered_map<string, JsonValue> obj1 = array[i].asObj();
                byte_size += sizeof(char);
                byte_size += get_byteSize(obj1);
                byte_size += sizeof(char);
                byte_size += sizeof(char);
                break;
            }
            }
        }
        byte_size += sizeof(char);
        return byte_size;
    }
    int get_byteSize(unordered_map<string, JsonValue> json)
    {
        int byte_size = sizeof(int);
        for (const auto& [key, value] : json)
        {
            switch (value.getType())
            {
            case s_type:// string
            {
                const string& strValue = value.asString();
                int strSize = strValue.size() + 1; // Include null terminator
                // adding size of type + size of key + size of null terminator + size of string + size of string value
                byte_size += sizeof(char) + key.size() + 1 + sizeof(int) + strSize;
                break;
            }
            case i_type:
            {
                // adding size of type + size of key + size of null terminator + size of int 
                byte_size += sizeof(char) + key.size() + 1 + sizeof(int);
                break;
            }
            case b_type:
            {
                // adding size of type + size of key + size of null terminator + size of bool
                byte_size += sizeof(char) + key.size() + 1 + sizeof(char);
                break;
            }
            case d_type:
            {
                // adding size of type + size of key + size of null terminator + size of double
                byte_size += sizeof(char) + key.size() + 1 + sizeof(double);
                break;
            }
            case l_type:
            {
                //adding size of type + size of key + size of null terminator + size of long
                byte_size += sizeof(char) + key.size() + 1 + sizeof(long);
                break;
            }
            case a_type:
            {
                vector<JsonValue> array = value.asVector();
                // adding size of type + size of key + size of null terminator + size of array
                byte_size += sizeof(char) + key.size() + 1;
                byte_size += get_sizeof_array(array);

                break;
            }
            case o_type:
            {
                // adding size of type + size of key + size of null terminator + size of object
                unordered_map<string, JsonValue> obj1 = value.asObj();
                byte_size += sizeof(char) + key.size() + 1;
                byte_size += get_byteSize(obj1);
                break;
            }
            default:
                // Handle unsupported data types
                break;
            }
        }
        byte_size += sizeof(char);
        return byte_size;

    }
    void write_obj(const object documentData, fstream& file)
    {
        if (!is_doc&&!is_first)
        {
            int byte_size = get_byteSize(documentData);
            file.write(reinterpret_cast<const char*>(&byte_size), sizeof(int)); // Write document size

        }
        is_first = false;
        for (const auto& [key, value] : documentData)
        {

            switch (value.getType())
            {
            case s_type:
            {
                const string& strValue = value.asString();
                int strSize = strValue.size() + 1; // Include null terminator

                int dataType = STRING; // Define the BSON data type
                file.write(reinterpret_cast<const char*>(&dataType), sizeof(char)); // Write the BSON data type

                file.write(key.c_str(), key.size() + 1); // Include null terminator for field name
                file.write(reinterpret_cast<const char*>(&strSize), sizeof(int)); // Write string size
                file.write(strValue.c_str(), strSize); // Write string value

                break;
            }
            case i_type:
            {
                int intValue = value.asInt();
                int dataType = INT32;
                file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                file.write(key.c_str(), key.size() + 1);
                file.write(reinterpret_cast<const char*>(&intValue), sizeof(int));
                break;
            }
            case b_type:
            {
                bool boolValue = value.asBool();
                int dataType = BOOL;
                file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                file.write(key.c_str(), key.size() + 1);
                file.write(reinterpret_cast<const char*>(&boolValue), sizeof(char));
                break;
            }
            case d_type:
            {
                double doubleValue = value.asDouble();
                int dataType = DOUBLE;
                file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                file.write(key.c_str(), key.size() + 1);
                file.write(reinterpret_cast<const char*>(&doubleValue), sizeof(double));
                break;
            }
            case l_type:
            {
                long longValue = value.asLong();
                int dataType = INT64;
                file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                file.write(key.c_str(), key.size() + 1);
                file.write(reinterpret_cast<const char*>(&longValue), sizeof(long));
                break;
            }
            case a_type:
            {
                vector<JsonValue> array = value.asVector();
                int arraySize = get_sizeof_array(array);
                int dataType = ARRAY;
                file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                file.write(key.c_str(), key.size() + 1);
                file.write(reinterpret_cast<const char*>(&arraySize), sizeof(int));
                int null = END;
                for (int i = 0; i < array.size(); i++)
                {
                    int index = i;


                    switch (array[i].getType())
                    {
                    case 0:
                    {
                        const string& strValue = array[i].asString();
                        int strSize = strValue.size() + 1; // Include null terminator

                        int dataType = STRING; // Define the BSON data type
                        file.write(reinterpret_cast<const char*>(&dataType), sizeof(char)); // Write the BSON data type

                        file.write(reinterpret_cast<const char*>(&index), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&null), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&strSize), sizeof(int)); // Write string size
                        file.write(strValue.c_str(), strSize); // Write string value
                        break;
                    }
                    case 1:
                    {
                        int intValue = array[i].asInt();
                        int dataType = INT32;
                        file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));

                        file.write(reinterpret_cast<const char*>(&index), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&null), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&intValue), sizeof(int));
                        break;
                    }
                    case 2:
                    {
                        bool boolValue = array[i].asBool();
                        int dataType = BOOL;
                        file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&index), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&null), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&boolValue), sizeof(char));
                        break;
                    }
                    case 3:
                    {
                        double doubleValue = array[i].asDouble();
                        int dataType = DOUBLE;
                        file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&index), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&null), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&doubleValue), sizeof(double));
                        break;
                    }
                    case 4:
                    {
                        long longValue = array[i].asLong();
                        int dataType = INT64;
                        file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&index), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&null), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&longValue), sizeof(long));
                        break;
                    }
                    case 6:
                    {
                        unordered_map<string, JsonValue> obj1 = array[i].asObj();
                        int obj_size = get_byteSize(obj1);
                        int dataType = OBJECT;
                        file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&index), sizeof(char));
                        file.write(reinterpret_cast<const char*>(&null), sizeof(char));
                        write_obj(obj1, file);
                    }

                    }

                }
                file.write(reinterpret_cast<const char*>(&null), sizeof(char));
                break;
            }
            case o_type:
            {
                unordered_map<string, JsonValue> obj1 = value.asObj();
                int obj_size = get_byteSize(obj1);
                int dataType = OBJECT;
                file.write(reinterpret_cast<const char*>(&dataType), sizeof(char));
                file.write(key.c_str(), key.size() + 1);
                write_obj(obj1, file);
                break;
            }
            default:
                // Handle unsupported data types
                break;
            }
        }

        // Write document end marker
        int end = END;
        file.write(reinterpret_cast<const char*>(&end), sizeof(char));
       

    }
    int write_doc (const Document doc, fstream& file)
    {
        is_doc = true;
        is_first = true;
        int offset = 0;
        int byte_size = get_byteSize(doc.getDocument());
        byte_size += sizeof(int)+sizeof(char)+12; //size of _id field
        file.write(reinterpret_cast<const char*>(&byte_size), sizeof(int));
        is_doc = false;
        int datatype = 7;
        file.write(reinterpret_cast<const char*>(&datatype), sizeof(char));
        file.write("_id", 4);
        uint32_t first = doc._id.get_timestamp();
        uint32_t second = doc._id.get_randomValue();
        uint32_t third = doc._id.get_increment();
        file.write(reinterpret_cast<const char*>(&first), sizeof(first));
        file.write(reinterpret_cast<const char*>(&second), sizeof(second));
        file.write(reinterpret_cast<const char*>(&third), sizeof(third));
        write_obj(doc.getDocument(), file);
        offset = file.tellp();
        offset -= byte_size;
        return offset;
      

    }
    object JSON_obj(int& index, vector<char>& bsonData, int obj_size, ObjectId& ID)
    {

        unordered_map<string, JsonValue> object;
        int currentPosition = index;
        while (currentPosition < obj_size + index)
        {
            int dataType = bsonData[currentPosition];
            currentPosition++;

            if (dataType == END)
                // currentPosition++;
                break;

            string fieldname;
            while (bsonData[currentPosition] != 0x00)
            {
                fieldname += bsonData[currentPosition];
                currentPosition++;
            }
            currentPosition++;

            switch (dataType)
            {
            case DOUBLE:
            { // Double
                double doubleValue;
                memcpy(&doubleValue, &bsonData[currentPosition], sizeof(double));
                currentPosition += sizeof(double);
                JsonValue value = doubleValue;

                object[fieldname] = doubleValue;
                break;
            }
            case STRING:
            { // String
                int stringSize = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                string stringValue(&bsonData[currentPosition], stringSize - 1); // Exclude null terminator
                currentPosition += stringSize;

                object[fieldname] = stringValue;
                break;
            }
            case ARRAY:
            { // Array
                vector<JsonValue> array;
                // Read array size
                int arraySize = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                int arrayEndPosition = currentPosition + arraySize - 5; // Deducting array size and type bytes

                // Process array elements
                while (currentPosition < arrayEndPosition)
                {
                    // Read the data type of array element
                    int elementType = bsonData[currentPosition];
                    char type = bsonData[currentPosition];
                    currentPosition++;
                    switch (elementType)
                    {
                    case DOUBLE:
                    { // Double
                        currentPosition += 2;
                        double doubleValue;
                        memcpy(&doubleValue, &bsonData[currentPosition], sizeof(double));
                        currentPosition += sizeof(double);
                        array.push_back(doubleValue);
                        break;
                    }
                    case STRING:
                    { // String

                        currentPosition += 2; // Skip the null byte
                        int stringSize = littleEndianToInt(&bsonData[currentPosition]);
                        currentPosition += 4;
                        string stringValue(&bsonData[currentPosition], stringSize - 1); // Exclude null terminator
                        currentPosition += stringSize;
                        array.push_back(stringValue);
                        break;
                    }
                    case INT32:
                    { // 32-bit integer
                        currentPosition += 2;
                        int intValue = littleEndianToInt(&bsonData[currentPosition]);
                        currentPosition += 4;
                        array.push_back(intValue);
                        break;
                    }
                    case INT64:
                    {
                        currentPosition += 2;
                        long longValue = littleEndianToInt(&bsonData[currentPosition]);
                        currentPosition += 4;
                        array.push_back(longValue);
                        break;
                    }
                    case BOOL:
                    {
                        currentPosition += 2;
                        bool boolValue = bsonData[currentPosition] != 0x00;
                        currentPosition++;
                        array.push_back(boolValue);
                        break;

                    }
                    case OBJECT:
                    {
                        currentPosition += 2;
                        int obj_size = littleEndianToInt(&bsonData[currentPosition]);
                        currentPosition += 4;
                        ObjectId dummy;
                        unordered_map<string, JsonValue> obj = JSON_obj(currentPosition, bsonData, obj_size, dummy);
                        array.push_back(obj);
                        break;
                    }
                    case BINDATA:
                    {
                        currentPosition += 2;
                        int binarySize = littleEndianToInt(&bsonData[currentPosition]);
                        currentPosition += 4;
                        currentPosition += binarySize;
                        break;
                    }
                    case DATE:
                    {
                        currentPosition += 2;
                        currentPosition += 8;
                        break;
                    }
                    case NULLTYPE:
                    {
                        currentPosition += 2;
                        break;
                    }
                    case REGEX:
                    {
                        currentPosition += 2;
                        int regexSize = 0;
                        while (bsonData[currentPosition + regexSize] != 0x00)
                        {
                            regexSize++;
                        }
                        currentPosition += regexSize + 1;
                        while (bsonData[currentPosition + regexSize] != 0x00)
                        {
                            regexSize++;
                        }
                        currentPosition += regexSize + 1;
                        break;
                    }
                    case DBPOINTER:
                    {
                        currentPosition += 2;
                        currentPosition += 12;
                        break;
                    }
                    case JAVASCRIPT:
                    {
                        currentPosition += 2;
                        int stringSize = littleEndianToInt(&bsonData[currentPosition]);
                        currentPosition += 4;
                        string stringValue(&bsonData[currentPosition], stringSize - 1); // Exclude null terminator
                        currentPosition += stringSize;
                        break;
                    }
                    case SYMBOL:
                    {
                        currentPosition += 2;
                        int stringSize = littleEndianToInt(&bsonData[currentPosition]);
                        currentPosition += 4;
                        string stringValue(&bsonData[currentPosition], stringSize - 1); // Exclude null terminator
                        currentPosition += stringSize;
                        break;
                    }
                    case JAVASCRIPTWITHSCOPE:
                    {
                        currentPosition += 2;
                        int stringSize = littleEndianToInt(&bsonData[currentPosition]);
                        currentPosition += 4;
                        string stringValue(&bsonData[currentPosition], stringSize - 1); // Exclude null terminator
                        currentPosition += stringSize;
                        break;
                    }
                    case TIMESTAMP:
                    {
                        currentPosition += 2;
                        currentPosition += 8;
                        break;
                    }
                    case DECIMAL:
                    {
                        currentPosition += 2;
                        currentPosition += 16;
                        break;
                    }
                    case MINKEY:
                    {
                        currentPosition += 2;
                        break;
                    }
                    case MAXKEY:
                    {
                        currentPosition += 2;
                        break;
                    }
                    case UNDEFINED:
                    {
                        currentPosition += 2;
                        break;
                    }



                    default:
                        cout << "Unhandled array element type: " << static_cast<int>(elementType) << endl;
                        break;
                    }
                }

                object[fieldname] = array;
                currentPosition = arrayEndPosition + 1; // Skip the null terminator
                break;
            }

            case BINDATA:
            {
                int binarySize = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                currentPosition += binarySize;
                break;

            }
            case BOOL:
            { // Boolean
                bool boolValue = bsonData[currentPosition] != 0x00;
                currentPosition++;
                object[fieldname] = boolValue;
                break;
            }
            case INT32:
            {
                int intValue = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                object[fieldname] = intValue;
                break;
            }
            case INT64:
            {
                long longValue = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                object[fieldname] = longValue;
                break;
            }
            case NULLTYPE:
            {
                object[fieldname] = NULL;

            }
            case OBJECTID:
            {
                uint32_t first = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                uint32_t second = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                uint32_t third = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                ID.set_timestamp(first);
                ID.set_randomValue(second);
                ID.set_increment(third);
                
                break;
            }
            case DATE:
            {
                currentPosition += 8;
                break;
            }
            case REGEX:
            {
                int regexSize = 0;
                while (bsonData[currentPosition + regexSize] != 0x00)
                {
                    regexSize++;
                }
                currentPosition += regexSize + 1;
                while (bsonData[currentPosition + regexSize] != 0x00)
                {
                    regexSize++;
                }
                currentPosition += regexSize + 1;
                break;
            }
            case DBPOINTER:
            {

                currentPosition += 12;
                break;
            }
            case JAVASCRIPT:
            {
                int stringSize = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                string stringValue(&bsonData[currentPosition], stringSize - 1); // Exclude null terminator
                currentPosition += stringSize;
                break;
            }
            case  SYMBOL:
            {

                int stringSize = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                string stringValue(&bsonData[currentPosition], stringSize - 1); // Exclude null terminator
                currentPosition += stringSize;
                break;
            }
            case JAVASCRIPTWITHSCOPE:
            {

                int stringSize = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                string stringValue(&bsonData[currentPosition], stringSize - 1); // Exclude null terminator
                currentPosition += stringSize;
            }
            case OBJECT:
            {
                int obj_size = littleEndianToInt(&bsonData[currentPosition]);
                currentPosition += 4;
                ObjectId Dummy;
                unordered_map<string, JsonValue> obj = JSON_obj(currentPosition, bsonData, obj_size, Dummy);
                object[fieldname] = obj;
                break;


            }
            case TIMESTAMP:
            {
                currentPosition += 8;
                break;
            }
            case DECIMAL:
            {
                currentPosition += 16;
                break;
            }
            case MINKEY:
            {
                break;
            }
            case MAXKEY:
            {
                break;
            }
            case UNDEFINED:
            {
                break;
            }

            default:
                break;
            }




        }
        index = currentPosition;
        return object;
    }


  

    object JSON_to_memory(string json)
    {
        if (isValid_json(json))
        {
            object obj;
            int index = 0;

            if (json[0] == '{')
                json = json.substr(1, json.size() - 2);
            if (json[json.size() - 1] == '}')
                json = json.substr(0, json.size() - 1);
            int size = json.size();
            while (index < size)
            {
                while ((json[index] != '\"' ) && index < size)
                    index++;
                if (index >= size)
                    break;

                string key = json.substr(index + 1, json.find('\"', index + 1) - index - 1);
                index = json.find(':', index);
                index++;
                while ((json[index] == ' '|| json[index]=='\n' || json[index]==',' || json[index] == '\t') && index<size)
                    index++;

                if (json[index] == '\"')
                {
                   
                    string value = json.substr(index + 1, json.find('\"', index + 1) - index - 1);
                    obj[key] = value;
                    index+=value.size()+2;
                   
                }
                else if (isalnum(json[index]))
                {
					string value = json.substr(index, json.find(',', index) - index);
					if (value == "true")
						obj[key] = true;
					else if (value == "false")
						obj[key] = false;
					else if (value == "null")
						obj[key] = NULL;
                    else
                    {
                        if (value.find('.') != string::npos)
                        {
							double d = stod(value);
							obj[key] = d;
						}
                        else if (value.size() > 9)
                        {
							long l = stol(value);
							obj[key] = l;

						}
                        else if(value.size()>9)
                        {
                            long l = stol(value);
                            obj[key] = l;

                        }
                        else
                        {
							int i = stoi(value);
							obj[key] = i;
						}
					}
                    index += value.size()+2;
					
				}
              
                else if (json[index] == ' ')
                {
					index++;
				}
                else if (json[index] == 't')
                {
					obj[key] = true;
					index = json.find(',', index);
					index++;
				}
                else if (json[index] == 'f')
                {
					obj[key] = false;
					index = json.find(',', index);
					index++;
				}
                else if (json[index] == 'n')
                {
					obj[key] = NULL;
					index = json.find(',', index);
					index++;
				}
                else if (json[index] == '{')
                {
                    unordered_map<string, JsonValue> obj1 = JSON_to_memory(json.substr(index, json.find('}', index) - index + 1));
                    obj[key] = obj1;
                    index = json.find('}', index);
                    index++;
                }
                


                else if (json[index] == '[')
                {
                    index++;
                  
                    vector<JsonValue> Array;
                    while (json[index] != ']')
                    {
                        while ((json[index] == ' ' || json[index] == '\n' || json[index] == ',' || json[index] == '\t') && index < size)
                            index++;
                        if (json[index] == '\"')
                        {
                            
                            string value = json.substr(index + 1, json.find('\"', index + 1) - index - 1);
                            Array.push_back(value);
                          
                            if (index == string::npos)
                                break;
                            index+=value.size()+2;
                        }
                        else if (isalnum(json[index]))
                        {
                            string value = json.substr(index, json.find(',', index) - index);
                            if (value == "true")
                                Array.push_back(true);
                            else if (value == "false")
                                Array.push_back(false);
                            
                            else
                            {
                                if (value.find('.') != string::npos)
                                {
                                    double d = stod(value);
                                    Array.push_back(d);
                                    
                                }
                                else if (value.size() > 9)
                                {
                                    long l = stol(value);
                                    obj[key] = l;

                                }
                                else
                                {
                                    int i = stoi(value);
                                    Array.push_back(i);
                                }
                            }
                            index += value.size() + 2;

                        }
                        else if (json[index] == '{')
                        {
                            
                            unordered_map<string, JsonValue> obj1 = JSON_to_memory(json.substr(index, json.find('}', index) - index + 1));
                            Array.push_back(obj1);
                            index = json.find('}', index);
                       
                            index++;
                        }
                        else if (json[index] == ' ')
                        {
                            index++;
                        }
                        else if (json[index] == 't')
                        {
                            Array.push_back(true);
                            index = json.find(',', index);
                            if (index == string::npos)
                                break;
                            index++;
                        }
                        else if (json[index] == 'f')
                        {
                            Array.push_back(false);
                            index = json.find(',', index);
                            if (index == string::npos)
                                break;
                            index++;
                        }
                        else if (json[index] == 'n')
                        {
                            Array.push_back(NULL);
                            index = json.find(',', index);
                            if (index == string::npos)
                                break;
                            index++;
                        }
                       




                    }
                    obj[key] = Array;
                    index++;
                    
                }
                
            }
            return obj;
        }
        else
        {
            cout << "Invalid JSON" << endl;
            return object();
        }
    }

    int Hash(const string& str)
    {
        unsigned int hash = 5381;
        for (char c : str) {
            hash = ((hash << 5) + hash) + static_cast<unsigned int>(c);
        }
        return static_cast<int>(hash %INT_MAX);

    }
    void deSterilize_one(KEY info)
    {
        fstream file(info.second.first, ios::binary | ios::in);
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        file.seekg(info.second.second, ios::beg);
        char fileSizeBytes[4];
        file.read(fileSizeBytes, 4);
        int docSize = littleEndianToInt(fileSizeBytes);
        ObjectId obj_id;
        object json;
        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        int currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize, obj_id);
        Document doc(json, obj_id);
        current.push_back(doc);
        file.close();
        is_strerilized = false;
    }
    void create_index_one(string key,KEY info)
    {
        fstream file(info.second.first, ios::binary | ios::in);
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        file.seekg(info.second.second, ios::beg);
        char fileSizeBytes[4];
        file.read(fileSizeBytes, 4);
        int docSize = littleEndianToInt(fileSizeBytes);
        ObjectId obj_id;
        object json;
        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        int currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize, obj_id);
        
        KEY new_info;
        JsonValue value = json[key];
        new_info.first = Hash(val_to_string(value));
        
        new_info.second = info.second;
        int index = 0;
        for (int i = 0; i < indexes.size(); i++)
        {
            if (indexes[i].first == key)
            {
				index = i;
				break;
			}
		}
        indexes[index].second.insert(new_info);
        file.close();
       
    }
    
    void find(string key, string value, KEY info)
    {
        fstream file(info.second.first, ios::binary | ios::in);
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        file.seekg(info.second.second, ios::beg);
        char fileSizeBytes[4];
        file.read(fileSizeBytes, 4);
        int docSize = littleEndianToInt(fileSizeBytes);
        ObjectId obj_id;
        object json;
        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        int currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize, obj_id);
        
        // json have that key in it
        if (json.find(key) != json.end())
        {
            JsonValue val = json[key];
            if(val_to_string(val,value)==value)
			{
                if(updateMany)
                    filename=info.second.first;
				Document doc(json, obj_id);
				current.push_back(doc);
			}

            

        }
    
		
        
        file.close();

    }

    void array_find_helper(object&json , string key, vector<string> values, KEY info, ObjectId obj_id,bool all=false, object* nested= nullptr, bool nest=false)
    {
        if (all)
        {
            vector <JsonValue> Array;
            bool forward = false;
            if (nest)
            {
                Array = (*nested)[key].asVector();
                if (nested->find(key) != nested->end())
					forward = true;
            }
            else
            {
                Array = json[key].asVector();
                if (json.find(key) != json.end())
                    forward = true;
            }
            if (forward)
            {
                
               
              
                int count = 0;
                for (int i = 0; i < Array.size(); i++)
                {
                    for (int j = 0; j < values.size(); j++)
                    {
                        if (val_to_string(Array[i]) == values[j])
                        {
                            count++;
                        }
                    }
                }
                if (count == values.size())
                {
                    if (updateMany)
                        filename = info.second.first;
                    Document doc(json, obj_id);
                    current.push_back(doc);
                }



            }
        }
        else
        {
            JsonValue val;
            bool forward = false;
            if (nest)
            {
                val = (*nested)[key];
                if (nested->find(key) != nested->end())
                    forward = true;
            }
            else
            {
                val = json[key];
                if (json.find(key) != json.end())
                    forward = true;
            }
            if (forward)
            {


                for (int i = 0; i < values.size(); i++)
                    if (val_to_string(val, values[i]) == values[i])
                    {
                        if (updateMany)
                            filename = info.second.first;
                        Document doc(json, obj_id);
                        current.push_back(doc);
                        break;
                    }



            }
        }

    }
    void array_find_helper(object& json, string key, vector<JsonValue> Array, KEY info, ObjectId obj_id, object* nested = nullptr, bool nest = false)
    {


        bool forward = false;
        vector<JsonValue> Arr;
        if (nest)
        {
            Arr = (*nested)[key].asVector();
                if (nested->find(key) != nested->end())
				forward = true;
        }
        else
        {
            Arr = json[key].asVector();
            if(json.find(key)!=json.end())
				forward = true;
        }
        
        
        if (forward)
        {
            
           
            int equal = false;
            if (Arr.size() == Array.size())
            {
                equal = true;
                for (int i = 0; i < Array.size(); i++)
                {
                    if (val_to_string(Arr[i]) != val_to_string(Array[i]))
                    {
                        equal = false;
                        break;
                    }

                }

            }
            if (equal)
            {
                if (updateMany)
                    filename = info.second.first;
                Document doc(json, obj_id);
                current.push_back(doc);
            }



        }
    }
    void find(string key, vector<string> values, KEY info, bool all = false)
    {
        fstream file(info.second.first, ios::binary | ios::in);
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        file.seekg(info.second.second, ios::beg);
        char fileSizeBytes[4];
        file.read(fileSizeBytes, 4);
        int docSize = littleEndianToInt(fileSizeBytes);
        ObjectId obj_id;
        object json;
        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        int currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize, obj_id);

        // json have that key in it
       
         array_find_helper(json, key, values, info, obj_id, all);

            file.close();

    }
    void find(vector<pair<string, string>> key_values, KEY info)
    {
        fstream file(info.second.first, ios::binary | ios::in);
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        file.seekg(info.second.second, ios::beg);
        char fileSizeBytes[4];
        file.read(fileSizeBytes, 4);
        int docSize = littleEndianToInt(fileSizeBytes);
        ObjectId obj_id;
        object json;
        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        int currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize, obj_id);

        // json have that key in it
        bool insert = true;
        for (int i = 0; i < key_values.size(); i++)
        {
            if (json.find(key_values[i].first) != json.end())
            {
                JsonValue val = json[key_values[i].first];
                if (val_to_string(val) != key_values[i].second)
                {
                    insert = false;
                    break;
                }

            }
        }
        if (insert)
        {
            Document doc(json, obj_id);
            current.push_back(doc);

        }

        file.close();

    }
    void find(string key, vector<JsonValue> Array, KEY info)
    {
        fstream file(info.second.first, ios::binary | ios::in);
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        file.seekg(info.second.second, ios::beg);
        char fileSizeBytes[4];
        file.read(fileSizeBytes, 4);
        int docSize = littleEndianToInt(fileSizeBytes);
        ObjectId obj_id;
        object json;
        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        int currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize, obj_id);

        // json have that key in it
      
        array_find_helper(json, key, Array, info, obj_id);


        file.close();

    }

    void find(string key,vector<string> nested_keys, string value, KEY info,bool is_array=false)
    {
        fstream file(info.second.first, ios::binary | ios::in);
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        file.seekg(info.second.second, ios::beg);
        char fileSizeBytes[4];
        file.read(fileSizeBytes, 4);
        int docSize = littleEndianToInt(fileSizeBytes);
        ObjectId obj_id;
        object json;
        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        int currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize, obj_id);

        // json have that key in it
        if (is_array)
        {
            if (json.find(key) != json.end())
            {
                vector<JsonValue> Array = json[key].asVector();
                for(int i=0;i<Array.size();i++)
                {
                    object val = Array[i].asObj();
                    for (int i = 1; i < nested_keys.size() - 1; i++)
                    {
                        if (val.find(nested_keys[i]) != val.end())
                        {
                            val = val[nested_keys[i]].asObj();
                        }
                        else
                        {
                            return;
                        }
                    }
                    if (val.find(nested_keys[nested_keys.size() - 1]) != val.end())
                    {
                        if (val_to_string(val[nested_keys[nested_keys.size() - 1]], value) == value)
                        {
                            if (updateMany)
                                filename = info.second.first;
                            Document doc(json, obj_id);
                            current.push_back(doc);
                            break;
                        }
                    }
                    else
                    {
                        return;
                    }
                }




            }
        }
        else
        {
            if (json.find(key) != json.end())
            {
                object val = json[key].asObj();
                for (int i = 0; i < nested_keys.size() - 1; i++)
                {
                    if (val.find(nested_keys[i]) != val.end())
                    {
                        val = val[nested_keys[i]].asObj();
                    }
                    else
                    {
                        return;
                    }
                }
                if (val.find(nested_keys[nested_keys.size() - 1]) != val.end())
                {
                    if (val_to_string(val[nested_keys[nested_keys.size() - 1]], value) == value)
                    {
                        if (updateMany)
                            filename = info.second.first;
                        Document doc(json, obj_id);
                        current.push_back(doc);
                    }
                }
                else
                {
                    return;
                }




            }
        }


        file.close();

    }
    void find(string key, vector<string> nested_keys, vector<string> values, KEY info, bool all = false, bool is_array = false)
    {
        fstream file(info.second.first, ios::binary | ios::in);
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        file.seekg(info.second.second, ios::beg);
        char fileSizeBytes[4];
        file.read(fileSizeBytes, 4);
        int docSize = littleEndianToInt(fileSizeBytes);
        ObjectId obj_id;
        object json;
        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        int currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize, obj_id);

        // json have that key in it
        if (is_array)
        {
            if (json.find(key) != json.end())
            {
                   vector<JsonValue> Array = json[key].asVector();
                   for(int i=0;i<Array.size();i++)
                   
                   {
                    object val = Array[i].asObj();

                    for (int i = 1; i < nested_keys.size() - 1; i++)
                    {
                        if (val.find(nested_keys[i]) != val.end())
                        {
                            val = val[nested_keys[i]].asObj();
                        }
                        else
                        {
                            return;
                        }
                    }
                    if (val.find(nested_keys[nested_keys.size() - 1]) != val.end())
                    {
                        array_find_helper(json, nested_keys[nested_keys.size() - 1], values, info, obj_id, all, &val, true);
                    }
                    else
                    {
                        return;
                    }
                   }  



            }
        }
        else
        {
            if (json.find(key) != json.end())
            {
                object val = json[key].asObj();

                for (int i = 0; i < nested_keys.size() - 1; i++)
                {
                    if (val.find(nested_keys[i]) != val.end())
                    {
                        val = val[nested_keys[i]].asObj();
                    }
                    else
                    {
                        return;
                    }
                }
                if (val.find(nested_keys[nested_keys.size() - 1]) != val.end())
                {
                    array_find_helper(json, nested_keys[nested_keys.size() - 1], values, info, obj_id, all, &val, true);
                }
                else
                {
                    return;
                }




            }
        }


        file.close();

    }
    void find(string key, vector<string> nested_keys, vector<JsonValue> Array, KEY info, bool is_array = false)
    {
        fstream file(info.second.first, ios::binary | ios::in);
        if (!file.is_open())
        {
            cout << "File not found" << endl;
            return;
        }
        file.seekg(info.second.second, ios::beg);
        char fileSizeBytes[4];
        file.read(fileSizeBytes, 4);
        int docSize = littleEndianToInt(fileSizeBytes);
        ObjectId obj_id;
        object json;
        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        int currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize, obj_id);

        // json have that key in it
        if (is_array)
        {
            if (json.find(key) != json.end())
            {
                vector<JsonValue> Array1 = json[key].asVector();
                for (int i = 0; i < Array1.size(); i++)
                {
                    object val = Array1[i].asObj();

                    for (int i = 1; i < nested_keys.size() - 1; i++)
                    {
                        if (val.find(nested_keys[i]) != val.end())
                        {
                            val = val[nested_keys[i]].asObj();
                        }
                        else
                        {
                            return;
                        }
                    }
                    if (val.find(nested_keys[nested_keys.size() - 1]) != val.end())
                    {
                        array_find_helper(json, nested_keys[nested_keys.size() - 1], Array, info, obj_id, &val, true);
                    }
                    else
                    {
                        return;
                    }
                }



            }
        }
        else
        {
            if (json.find(key) != json.end())
            {
                object val = json[key].asObj();

                for (int i = 0; i < nested_keys.size() - 1; i++)
                {
                    if (val.find(nested_keys[i]) != val.end())
                    {
                        val = val[nested_keys[i]].asObj();
                    }
                    else
                    {
                        return;
                    }
                }
                if (val.find(nested_keys[nested_keys.size() - 1]) != val.end())
                {
                    array_find_helper(json, nested_keys[nested_keys.size() - 1], Array, info, obj_id, &val, true);
                }
                else
                {
                    return;
                }




            }
        }



        file.close();

    }
    
    JsonValue string_to_val( string value)
    {
		if (value == "true")
			return true;
		else if (value == "false")
			return false;
		else if (value == "null")
			return NULL;
        else if(isdigit(value[0]))
        {
            if (value.find('.') != string::npos)
            {
				double d = stod(value);
				return d;
			}
            else if (value.size() > 9)
            {
                long l = stol(value);
                return l;

            }
            else
            {
				int i = stoi(value);
				return i;
			}
		}
       
            return value;
        

    }
    string val_to_string(JsonValue value,string check="")
    {
        switch (value.getType())
        {
        case 0:
        {
            const string& strValue = value.asString();
            return strValue;
            break;
        }
        case 1:
        {
            int intValue = value.asInt();
            return to_string(intValue);
            break;
        }
        case 2:
        {
            bool boolValue = value.asBool();
            if (boolValue)
                return "true";
            else
                return "false";
            break;
        }
        case 3:
        {
            double doubleValue = value.asDouble();
            return to_string(doubleValue);
            break;
        }
        case 4:
        {
            long longValue = value.asLong();
            return to_string(longValue);
            break;
        }
        case 5:
        {
            vector<JsonValue> array = value.asVector();
            for (int i = 0; i < array.size(); i++)
            {
                if(val_to_string(array[i])==check)
					return check;

            }
            return "";
        }

        }
    }
    void apend_in_file( string path)
    {
       
        

        File = new fstream(path, ios::binary | ios::out | ios::app);
        KEY info;
        if (!File->is_open())
        {
            cout << "Unable to open file for writing!";
            return;
        }
        //write_obj(*document, *file); 
     
        if(!current.empty())
        for (auto& doc : current)
        {

            info.first = Hash(doc._id.get_ID());
            // info.first=doc._id.;
            info.second.first = path;
            info.second.second = this->write_doc(doc, *File);
            doc.sterilize();
           
            documents[updated].insert(info);
        }

        File->close();
        is_strerilized = true;
        current.clear();
    }

    Filter filter_processor(string query, bool& is_array, bool& all)
    {

        if (query[0] == '{')
            query = query.substr(1, query.size() - 2);

        string key;
        Filter filter;
        bool nested = false;
        vector<string> nested_keys;
        key = query.substr(0, query.find(":"));
        int shrink = key.find(" ");
        string value;
        if (shrink != string::npos)
            key = key.substr(0, shrink);

        if (key == "_id")
        {
            int start = query.find("\"");
            int end = query.find("\"", start + 1);
            string value = query.substr(start + 1, end - start - 1);
            filter = value;
            return filter;


        }
        else
        {
            if (key.find(".") != string::npos)
            {
                nested = true;
                int si = key.find(".");
                int ei = key.find("\"", si);
                key = key.substr(1, key.find(".") - 1);

                while (si < ei)
                {
                    int start = si;
                    int end = query.find(".", start + 1);
                    if (end == string::npos)
                        end = ei;
                    value = query.substr(start + 1, end - start - 1);
                    nested_keys.push_back(value);
                    si = end + 1;
                    ei = query.find(".", si);
                }


            }
            query = query.substr(query.find(":") + 1, query.size() - query.find(":") - 1);
            if (query.find(",") == string::npos)
            {


                int start = query.find("\"");
                int end = query.find("\"", start + 1);
                value = query.substr(start + 1, end - start - 1);
                if (!nested)
                {
                    pair<string, string> key_val(make_pair(key, value));

                    filter = key_val;
                }
                else
                {
                    pair<string, pair <vector<string>, string>> key_nested_val;
                    key_nested_val.first = key;
                    key_nested_val.second.first = nested_keys;
                    key_nested_val.second.second = value;
                    filter = key_nested_val;
                }

                return filter;
            }
            else if (query.find("[") != string::npos)
            {
                if (query.find("$all") == string::npos)
                {
                    vector<JsonValue> values;

                    int si = query.find("[");
                    int ei = query.find("]");
                    while (si < ei)
                    {
                        int start = query.find("\"", si + 1);
                        int end = query.find("\"", start + 1);
                        value = query.substr(start + 1, end - start - 1);
                        values.push_back(value);
                        si = end + 1;
                        ei = query.find("]", si);
                    }
                    if (!nested)
                    {
                        pair<string, vector<JsonValue>> key_val;
                        key_val.first = key;
                        key_val.second = values;
                        filter = key_val;
                        is_array = true;
                    }
                    else
                    {
                        pair<string, pair<vector<string>, vector<JsonValue>>>key_val;
                        key_val.first = key;
                        key_val.second.first = nested_keys;
                        key_val.second.second = values;

                        filter = key_val;
                    }
                    return filter;
                }
                else
                {
                    all = true;
                    vector<string> values;
                    int si = query.find("[");
                    int ei = query.find("]");
                    while (si < ei)
                    {
                        int start = query.find("\"", si + 1);
                        int end = query.find("\"", start + 1);
                        value = query.substr(start + 1, end - start - 1);
                        values.push_back(value);
                        si = end + 1;
                        ei = query.find("]", si);
                    }
                    if (!nested)
                    {
                        pair<string, vector<string>> key_val;
                        key_val.first = key;
                        key_val.second = values;
                        filter = key_val;
                        is_array = true;
                    }
                    else
                    {
                        pair<string, pair<vector<string>, vector<string>>>key_val;
                        key_val.first = key;
                        key_val.second.first = nested_keys;
                        key_val.second.second = values;
                        filter = key_val;

                    }

                    return filter;

                }

            }
            else if (query.find("{") != string::npos)
            {
                vector<string> values;
                int si = query.find("{");
                int ei = query.find("}");
                while (si < ei)
                {
                    int start = query.find("\"", si + 1);
                    int end = query.find("\"", start + 1);
                    value = query.substr(start + 1, end - start - 1);
                    values.push_back(value);
                    si = end + 1;
                    ei = query.find("}", si);
                }
                if (!nested)
                {
                    pair<string, vector<string>> key_val;
                    key_val.first = key;
                    key_val.second = values;
                    filter = key_val;
                    is_array = true;
                }
                else
                {
                    pair<string, pair<vector<string>, vector<string>>>key_val;
                    key_val.first = key;
                    key_val.second.first = nested_keys;
                    key_val.second.first = values;
                    filter = key_val;
                }
                return filter;

            }

            else
            {
                int start = query.find("\"");
                int end = query.find("\"", start + 1);
                value = query.substr(start + 1, end - start - 1);
                vector < pair<string, string>> key_values;
                key_values.push_back(make_pair(key, value));
                end = query.find(",", end + 1) + 1;
                while (end < query.size())
                {
                    while (query[end] == ' ' && end < query.size())
                        end++;
                    if (query[end] == '}' || end >= query.size())
                        break;
                    key = query.substr(end, query.find(":", end + 1) - end - 1);
                    end = query.find("\"", end + 1);
                    value = query.substr(end + 1, query.find("\"", end + 1) - end - 1);
                    end = query.find("\"", end + 1) + 1;
                    key_values.push_back(make_pair(key, value));

                }
                if (!nested)
                {
                    filter = key_values;
                }
                else
                {
                    pair<string, pair<vector<string>, vector<pair<string, string>>>>key_val;
                    key_val.first = key;
                    key_val.second.first = nested_keys;
                    key_val.second.second = key_values;
                    filter = key_values;
                }
                return filter;
            }
        }



    }

    Filter updates_processor(string query, bool& set, bool& unset, bool& push, bool& pull, bool& all, bool& is_array)
    {
        if (query[0] == '{')
            query = query.substr(1, query.size() - 2);
        string type;
        type = query.substr(query.find("$") + 1, query.find(":") - query.find("$") - 1);
        size_t found = type.find_last_not_of(" \t\n\r");
        if (found != std::string::npos)
            type = type.substr(0, found + 1);
       
    
        if (type == "set")
        {
            set = true;
            query = query.substr(query.find("{") , query.size() - query.find(":") - 1);
            return filter_processor(query, is_array, all);


        }
        else if (type == "unset")
        {
            unset = true;
            int si = query.find(":");
            int ei = query.size();
            query = query.substr(si + 1, ei - si - 1);
            vector<string> keys;
            istringstream iss(query);


            string temp;
            getline(iss, temp, '{');
            getline(iss, temp, '}');


            istringstream keyStream(temp);
            string key;
            while (getline(keyStream, key, ','))
            {

                key.erase(0, key.find_first_not_of(" \t\n\r\f\v"));
                key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);
                keys.push_back(key);
            }

            Filter res = keys;
            return res;



        }
        else if (type == "pull")
        {

        }
        else if (type == "push")
        {

        }
    }
    Filter key_processor(string query)
    {

        if (query[0] == '{')
            query = query.substr(1, query.size() - 2);

        string key;
        Filter filter;

        vector<string> nested_keys;
        key = query.substr(0, query.find(":"));
        int shrink = key.find(" ");
        string value;
        if (shrink != string::npos)
            key = key.substr(0, shrink);
        nested_keys.push_back(key);
        if (key.find(".") != string::npos)
        {

            int si = key.find(".");
            int ei = key.find("\"", si);
            key = key.substr(1, key.find(".") - 1);


            while (si < ei)
            {
                int start = si;
                int end = query.find(".", start + 1);
                if (end == string::npos)
                    end = ei;
                value = query.substr(start + 1, end - start - 1);
                nested_keys.push_back(value);
                si = end + 1;
                ei = query.find(".", si);
            }


        }
        filter = nested_keys;
        return filter;





    }

public:
    Collection() = default;
    Collection(const string& name) : m_name(name) 
   {
       filename = m_name + ".bson";
       File = new fstream(filename, ios::binary | ios::out);
       documents.push_back(BTree(100));
       
       
   
   }
    Collection(const Collection& collection) : m_name(collection.m_name), current(collection.current) 
   {
       filename = collection.filename;
	   File = collection.File;
       this->documents=collection.documents;
       this->filename=collection.filename;
       this->is_strerilized=collection.is_strerilized;
       this->m_name=collection.m_name;
       this->updated=collection.updated;
       this->updateMany=collection.updateMany;
       this->indexes=collection.indexes;
       this->is_first=collection.is_first;
       this->index_names = collection.index_names;



   }
    Collection(const char* path)
    {
        documents.push_back(BTree(100));
        filename = path;
        if (filename.find_last_of("\\") != string::npos)
        {
            filename = filename.substr(filename.find_last_of("\\") + 1);
        }
        else if (filename.find_last_of("/") != string::npos)
        {
			filename = filename.substr(filename.find_last_of("/") + 1);
		}   

       
       
       
        m_name = filename;
        m_name = m_name.substr(0, m_name.find("."));
        
        this->File = nullptr;
        ifstream file(path, ios::binary | ios::in);
        File= new fstream(path, ios::binary | ios::in);
        Document doc;
        if (!file.is_open())
        {
			cout<<"File not found"<<endl;
			return;
		}
        int currentPosition = 0;
        int cursor = 0;
      
        while (!file.eof())
        {

            char fileSizeBytes[4];
            int pos = currentPosition;
            pos += cursor;
            file.seekg(pos, ios::beg);

            file.read(fileSizeBytes, 4);
            cursor = file.tellg();
            if (cursor < 0)
                break;

            int docSize = littleEndianToInt(fileSizeBytes);

            vector<char> bsonData(docSize);
            file.read(bsonData.data(), docSize);
            currentPosition = 0;
            ObjectId _id;
            doc.document =new object( JSON_obj(currentPosition, bsonData, docSize,_id));
           
            doc._id = _id;
            current.push_back(doc);
          //  this->updates_record[doc._id.get_ID()] = false;



        }
        file.close();
        sterilize();

    }
    Collection& operator=(const Collection& collection)
    {
        if (this != &collection)
        {
            m_name = collection.m_name;
            current = collection.current;
            filename = collection.filename;
            File = collection.File;
            this->indexes = collection.indexes;
            this->documents = collection.documents;
            this->filename = collection.filename;
            this->is_strerilized = collection.is_strerilized;
            this->m_name = collection.m_name;
            this->updated = collection.updated;
            this->updateMany = collection.updateMany;
            this->indexes = collection.indexes;
            this->is_first = collection.is_first;
            this->index_names = collection.index_names;
        }
		return *this; 
	}

    Collection(ifstream& rdr)
    {
        load(rdr);
    }

    Document& findDocument(const ID& id)
    { 
        for (auto& doc : current)
        {
			//if(doc.getID() == id)
				return doc;
		}
    }
    void insert_one(const Document& document)
    {
		current.push_back(document);
	}
    void insert_one(const object& obj)
    {
        Document doc(obj);
        current.push_back(doc);
        
    }
    void remove_one(const ID& id)
    {
        for (int i = 0; i < current.size(); i++)
        {
            //if (current[i].getID() == id)
            {
				current.erase(current.begin() + i);
				return;
			}
		}

    }
    
    string get_name() 
	{
		return m_name;
	}
    
    void view()
    {
        cout << "Collection: " << m_name << endl;
        if(!current.empty())
        for (auto& doc : current)
        {
			doc.view();
			cout << endl;
		}
	
     
    }
    void view(int start, int end)
    {
        if (start >= 0 && start < end && end <= current.size())
        {
            cout << "Collection: " << m_name << endl;
            for (int i = start; i < end; i++)
            {
                current[i].view();
                cout << endl;
            }
        }
        else
            return;
		
    }

    void sterilize()
    {
        filename = this->m_name+ ".bson";
       
       string path="mongoDB\\" + filename;
       
       File = new fstream(path, ios::binary | ios::out);
       KEY info;
       if (!File->is_open())
       {
           cout << "Unable to open file for writing!";
           return;
       }
       //write_obj(*document, *file); 
       File->clear();
       for (auto& doc : current)
       {
           
		  info.first = Hash(doc._id.get_ID());
          // info.first=doc._id.;
          info.second.first = path;
		  info.second.second=this->write_doc(doc, *File);
          doc.sterilize();
         documents[updated].insert(info);
	   }

       File->close();
       is_strerilized = true;
       current.clear();
    }
    void view_collection()
    {
        
        queue<node*> q=documents[updated].getnodes();
        if(q.empty())
		{
			cout<<"Collection is empty"<<endl;
			return;
		}
        node* temp=q.front();
        q.pop();
        for (int i = 0; i < temp->numKeys; i++)
        {
            deSterilize_one(temp->keys[i]);


        }
        int strat = 0;
        int end=temp->numKeys;
        view(strat, end);
        bool again = true;
        string next;
        while (again)
        {
            again=false;
            cout << "press \"it\" to view more or any to not ";

            cin >> next;
            if (next=="it")
            {
                
                    if (!q.empty())
                    {
                        system("cls");
                        
                        temp = q.front();
						q.pop();
						strat = end;
						end += temp->numKeys;
                        for (int i = 0; i < temp->numKeys; i++)
                        {
							deSterilize_one(temp->keys[i]);
                        }
                        view(strat, end);
                        again = true;
                    }
                
            
            }
        }
        
        current.clear();

    }


    void deSterilize_one(string id)
    {
        int key=Hash(id);
        KEY info=documents[updated].getkey(key);
        if (info.second.first == "")
		{
			cout << "Document not found" << endl;
			return;
		}
        deSterilize_one(info);
       

    }

    void deSterilize()
    {
        
        string path="mongoDB/" + filename;
        fstream file(path, ios::binary | ios::in);
        if (!file.is_open())
        {
			cout<<"File not found"<<endl;
			return;
          }
        int currentPosition = 0;
        int cursor = 0;
        object json;
        int index = 0;
        ObjectId id;
        while (!file.eof())
        {

            char fileSizeBytes[4];
            int pos = currentPosition;
            pos += cursor;
            file.seekg(pos, ios::beg);

            file.read(fileSizeBytes, 4);
            cursor = file.tellg();
            if (cursor < 0)
                break;

            int docSize = littleEndianToInt(fileSizeBytes);

            vector<char> bsonData(docSize);
            file.read(bsonData.data(), docSize);
            currentPosition = 0;
            json = JSON_obj(currentPosition, bsonData, docSize,id);
            this->current[index].setDocument(json);
            this->current[index]._id = id;
            this->current[index].is_strerilized = false;
            index++;


        }
        file.close();
        is_strerilized = false;

    }

    int count()
    {
        return current.size();
    }

    int get_byteSize()
    {
        File->open(filename, ios::binary | ios::in);
        int currentPosition = 0;
        int cursor = 0;
        int byte_size = 0;
        if (!File->is_open())
        {
            
            while (!File->eof())
            {
                char fileSizeBytes[4];
                int pos = currentPosition;
                pos += cursor;
                File->seekg(pos, ios::beg);
                File->read(fileSizeBytes, 4);
                cursor = File->tellg();
                if (cursor < 0)
                    break;
                int docSize = littleEndianToInt(fileSizeBytes);

                currentPosition = 0;
                byte_size += docSize;
            }
            
        }
        return byte_size;
		
	}

    //CRUD

    //Create
    void insert_one(string Json)
    {
        object temp = this->JSON_to_memory(Json);
          Document doc(temp);
        current.push_back(doc);
       // this->updates_record[doc._id.get_ID()] = false;
        filename = this->m_name + ".bson";

        string path = "mongoDB\\" + filename;
       this->apend_in_file(path);

        

    }
    void insert_many(string Json)
    {
        vector<string>jsons;
        string sub;
        int find=0;
        for (size_t i = 0; i < Json.size(); i = find+1 ) 
        {
            find = Json.find('+', i);
            if (find == string::npos) 
            {
                find = Json.size();
                
            }
            
            sub = Json.substr(i, find - i);
            
            jsons.push_back(sub);
        }
        for (auto& json : jsons)
        {
            //cout<<json<<endl;
			object temp = this->JSON_to_memory(json);
			Document doc(temp);
			current.push_back(doc);
          //  this->updates_record[doc._id.get_ID()] = false;
		}
        filename = this->m_name + ".bson";

        string path = "mongoDB\\" + filename;
        this->apend_in_file(path);
        
    }
    
    
    //read
    void find() // method for finding all documents in a collection
    {
        view_collection();
	}
    vector<Document> find_all()
    {
        queue<node*> q = documents[updated].getnodes();
        
        while (!q.empty())
        {
			node* temp = q.front();
			q.pop();
            for (int i = 0; i < temp->numKeys; i++)
            {
				deSterilize_one(temp->keys[i]);


			}
		}
        vector<Document> res = current;
		current.clear();
		return res;
        
    }
    vector<Document> find(string key, string value) 
    {
        int  index = -1;
        for (int i = 0; i < indexes.size(); i++)
        {
            if (indexes[i].first == key)
            {
				index=i;
				break;
			}
		}
        if (index>-1)
        {
            KEY info;
            info.first = Hash(value);
			vector<pair<string,int>> keys = indexes[index].second.getValues(info.first);
            if (updateMany)
            {
                filename = keys[0].first;
            }
			for (int i = 0; i < keys.size(); i++)
			{ 
                info.second=keys[i];
                deSterilize_one(info);
            }
            index_created = true;
            index_key=key;
            index_names.clear();
            index_names.push_back(key);
            vector<Document> res = current;
            current.clear();
            return res;
		
        }
        else
        {
			queue<node*> q = documents[updated].getnodes();
			node* temp;
            while (!q.empty())
            {
                temp=q.front();
                q.pop();
                for (int i = 0; i < temp->numKeys; i++)
                {
				  find(key, value,temp->keys[i]);
					
                }
            }
            vector<Document> res= current;
            current.clear();
            return res;
        }

    }
    vector<Document> find(vector<pair<string, string>> key_values) // method for finding all documents in a collection with specific key-value pairs
    {
        int  index = -1;
        for (int i = 0; i < indexes.size(); i++)
		{
           
			if (indexes[i].first == key_values[0].first)
			{
				index = i;
				break;
			}
		}
        if (index > -1)
        {
            KEY info;
            info.first = Hash(key_values[0].second);
            vector<pair<string, int>> keys = indexes[index].second.getValues(info.first);
            if (updateMany)
            {
                filename = keys[0].first;
            }
            for (int i = 0; i < keys.size(); i++)
            {
                info.second = keys[i];
                deSterilize_one(info);
            }
            index_key= key_values[0].first;
            index_created = true;
            index_names.clear();
            index_names.push_back(key_values[0].second);
            vector<Document> res = current;
            current.clear();
            return res;

        }
        else
        {
            queue<node*> q = documents[updated].getnodes();
            node* temp;
            while (!q.empty())
            {
                temp = q.front();
                q.pop();
                for (int i = 0; i < temp->numKeys; i++)
                {
                    find(key_values, temp->keys[i]);

                }
            }
            vector<Document> res = current;
            current.clear();
            return res;
        }

	}
    vector<Document> find_one(string id) // method for finding a single document in a collection with a specific id
    {
		deSterilize_one(id);
        vector<Document> res = current;
        current.clear();
        return res;


    }
    vector<Document> find(string key, vector<string> values, bool all = false) // method for finding all documents in a collection with a specific key-value pair
    {
        
            queue<node*> q = documents[updated].getnodes();
            node* temp;
            while (!q.empty())
            {
                temp = q.front();
                q.pop();
                for (int i = 0; i < temp->numKeys; i++)
                {
                   find(key, values, temp->keys[i],all);

                }
            }
            vector<Document> res = current;
            current.clear();
            return res;
        

    }
    vector<Document> find(string key, vector<JsonValue> Array) 
    {

        queue<node*> q = documents[updated].getnodes();
        node* temp;
        while (!q.empty())
        {
            temp = q.front();
            q.pop();
            for (int i = 0; i < temp->numKeys; i++)
            {
               find(key, Array, temp->keys[i]);

            }
        }
        vector<Document> res = current;
        current.clear();
        return res;


    }
    vector<Document> find(string primary_key, vector<string> nested_keys, string value, bool is_array=false) 
    {
        int  index = -1;
        for (int i = 0; i < indexes.size(); i++)
        {
            if (indexes[i].first == nested_keys.back())
            {
                index = i;
                break;
            }
        }
        if (index > -1)
        {
            KEY info;
            info.first = Hash(value);
            vector<pair<string, int>> keys = indexes[index].second.getValues(info.first);
            if (updateMany)
            {
                filename = keys[0].first;
            }
            for (int i = 0; i < keys.size(); i++)
            {
                info.second = keys[i];
                deSterilize_one(info);
            }

            vector<Document> res = current;
            current.clear();
            return res;

        }
        else
        {
            queue<node*> q = documents[updated].getnodes();
            node* temp;
            while (!q.empty())
            {
                temp = q.front();
                q.pop();
                for (int i = 0; i < temp->numKeys; i++)
                {
                    find(primary_key, nested_keys, value, temp->keys[i],is_array);

                }
            }
            vector<Document> res = current;
            current.clear();
            return res;
        }

    }

    vector<Document> find(string primary_key, vector<string> nested_keys, vector<string> values, bool all = false, bool is_array=false)
    {
        queue<node*> q = documents[updated].getnodes();
        node* temp;
        while (!q.empty())
        {
            temp = q.front();
            q.pop();
            for (int i = 0; i < temp->numKeys; i++)
            {
                find(primary_key, nested_keys, values, temp->keys[i], all,is_array);

            }
        }
        vector<Document> res = current;
        current.clear();
        return res;

    }
    vector<Document> find(string primary_key, vector<string> nested_keys, vector<JsonValue> Array,  bool is_array = false)
    
    {
        queue<node*> q = documents[updated].getnodes();
        node* temp;
        while (!q.empty())
        {
            temp = q.front();
            q.pop();
            for (int i = 0; i < temp->numKeys; i++)
            {
               find(primary_key, nested_keys, Array, temp->keys[i],is_array);

            }
        }
        vector<Document> res = current;
        current.clear();
        return res;

    }
    
    vector<Document> find( string query)
    {
        if (query == "")
        {
            return find_all();


        }
        else
        {
            Filter obj;
            bool is_array = false;
            bool all = false;
            obj = filter_processor(query, is_array, all);
            int type = obj.get_type();

            if (type == 0)
                return  find_one(obj.asString());

            else    if (type == 1)
            {
                pair<string, string> key_val = obj.asKey_value();
                return find(key_val.first, key_val.second);
            }

            else  if (type == 2)
            {
                pair<string, vector<string>> key_values = obj.askey_values();
                return find(key_values.first, key_values.second, all);
            }

            else if (type == 3)
            {
                vector<pair<string, string>> key_values = obj.askeys_values();


                return find(key_values);
            }

            else if (type == 4)
            {
                pair<string, vector<JsonValue>> key_values = obj.askey_vector();
                return find(key_values.first, key_values.second);
            }

            else if (type == 5)
            {
                pair<string, pair<vector<string>, string>> key_nested_val = obj.askey_nestedKey();
                return find(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second);
            }

            else  if (type == 6)
            {
                pair<string, pair<vector<string>, vector<string>>> key_nested_val = obj.asKey_nested_values();
                return find(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second, all, is_array);
            }
            else  if (type == 7)
            {
                pair<string, pair<vector<string>, vector<JsonValue>>> key_nested_val = obj.askey_nested_array();
                return find(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second, is_array);

            }

        }
            
               


            
       

    }
    
    string get_documnets(string query)
    {
        
        vector<Document> res = find(query);
		string result = "";
        for (int i = 0; i < res.size(); i++)
        {
			result += res[i].get_doc_as_json();
			result += "+";
		}
		
		return result;

    }
    
    
    void view(string key, string value) // method for finding all documents in a collection with a specific key-value pair
    {
       current=find(key, value);
      view();
      current.clear();

    }
    void view(vector<pair<string, string>> key_values) // method for finding all documents in a collection with specific key-value pairs
    {
        current=find(key_values);
        view();
		current.clear();

    }
    void view(string id) // method for finding a single document in a collection with a specific id
    {
        deSterilize_one(id);
        current.back().view();
        current.clear();


    }
    void view(string key, vector<string> values , bool all=false) 
    {
        current = find(key, values, all);
        view();
        current.clear();
    }
    void view(string key, vector<JsonValue> Array)
	{
		current = find(key, Array);
		view();
		current.clear();
	}
    void view(string primary_key, vector<string> nested_keys, string value,bool is_array=false) 
	{
		current = find(primary_key, nested_keys, value,is_array);
		view();
		current.clear();

	}
    void view(string primary_key, vector<string> nested_keys, vector<string> values, bool all = false ,bool is_array = false)
	{
		current = find(primary_key, nested_keys, values, all,is_array);
		view();
		current.clear();

	}
    void view(string primary_key, vector<string> nested_keys, vector<JsonValue> Array, bool is_array = false)
    {
        current = find(primary_key, nested_keys, Array,is_array);
		view();
		current.clear();

    }



   


    //Update
    void update_one(string id, string key, string value)
    {
		int hash = Hash(id);
		KEY info = documents[updated].getkey(hash);
        if (info.second.first == "")
        {
			cout << "Document not found" << endl;
			return;
		}
		deSterilize_one(info);
		Document& doc = current.back();
		doc.update_one(key, string_to_val(value));
        doc.view();
       
       
        string path = "";
        if (info.second.first.find("updated") != string::npos)
        {
            documents.push_back(BTree(documents[updated]));
            updated++;
			path = info.second.first.substr(0, info.second.first.find("updated") - 1);
			path += " updated-" + to_string(updated) + ".bson";
		}
        else
        {
         
            if (updated == 0)
            {
                BTree temp= documents[updated];
                documents.push_back(temp);
                updated++;

            }
			path = info.second.first.substr(0, info.second.first.find(".bson"));
            path += " updated-1.bson";
            
		}
        documents[updated].remove(info);
        this->apend_in_file(path);
		
	}
    void update_one(string id, vector<pair<string, string>> key_values)
    {
        int hash = Hash(id);
        KEY info = documents[updated].getkey(hash);
        if (info.second.first == "")
        {
            cout << "Document not found" << endl;
            return;
        }
        deSterilize_one(info);
        Document& doc = current.back();
        for (int i = 0; i < key_values.size(); i++)
        {
            doc.update_one(key_values[i].first, string_to_val(key_values[i].second));
        }
        doc.view();


        string path = "";
        if (info.second.first.find("updated") != string::npos)
        {
            documents.push_back(BTree(documents[updated]));
            updated++;
            path = info.second.first.substr(0, info.second.first.find("updated") - 1);
            path += " updated-" + to_string(updated) + ".bson";
        }
        else
        {

            if (updated == 0)
            {
                BTree temp = documents[updated];
                documents.push_back(temp);
                updated++;

            }
            path = info.second.first.substr(0, info.second.first.find(".bson"));
            path += " updated-1.bson";

        }
        documents[updated].remove(info);
        this->apend_in_file(path);

    }
    void update_one_unset(string id, vector<string> keys)
    {
        int hash = Hash(id);
        KEY info = documents[updated].getkey(hash);
        if (info.second.first == "")
        {
            cout << "Document not found" << endl;
            return;
        }
        deSterilize_one(info);
        Document& doc = current.back();
        for (int i = 0; i < keys.size(); i++)
        {
            doc.remove_one(keys[i]);
        }
        doc.view();


        string path = "";
        if (info.second.first.find("updated") != string::npos)
        {
            documents.push_back(BTree(documents[updated]));
            updated++;
            path = info.second.first.substr(0, info.second.first.find("updated") - 1);
            path += " updated-" + to_string(updated) + ".bson";
        }
        else
        {

            if (updated == 0)
            {
                BTree temp = documents[updated];
                documents.push_back(temp);
                updated++;

            }
            path = info.second.first.substr(0, info.second.first.find(".bson"));
            path += " updated-1.bson";

        }
        documents[updated].remove(info);
        this->apend_in_file(path);

    }
   
    void update_many(string key, string value, vector<pair<string,string>> key_values)
	{
        updateMany=true;
		current=find(key, value);
        for(Document doc:current)
		{
			for (int i = 0; i < key_values.size(); i++)
			{
				doc.update_one(key_values[i].first, string_to_val(key_values[i].second));
			}
			doc.view();
			
		}
        this->updateMany = true;
        string path = filename;
        updateMany= false;
      
        if (filename.find("updated") != string::npos)
        {
            documents.push_back(BTree(documents[updated]));
            updated++;
            path = filename.substr(0, filename.find("updated") - 1);
            path += " updated-" + to_string(updated) + ".bson";
        }
        else
        {

            if (updated == 0)
            {
                BTree temp = documents[updated];
                documents.push_back(temp);
                updated++;

            }
            path = filename.substr(0, filename.find(".bson"));
            path += " updated-1.bson";

        }
        for (int i = 0; i < current.size(); i++)
        {
            KEY info;
			info.first = Hash(current[i]._id.get_ID());
			documents[updated].remove(info);

        }
        this->apend_in_file(path);
       /* for (int i = 0; i < indexes.size(); i++)
        {
            if (indexes[i].first == key)
            {
				drop_index(key);
                create_index(key);
				break;
			}
		}*/

	}
    void update_many(vector<pair<string, string>> fkey_values, vector<pair<string, string>> key_values)
    {
        updateMany = true;
        current = find(fkey_values);
        for (Document doc : current)
        {
            for (int i = 0; i < key_values.size(); i++)
            {
                doc.update_one(key_values[i].first, string_to_val(key_values[i].second));
            }
            doc.view();

        }
        this->updateMany = true;
        string path = filename;
        updateMany = false;

        if (filename.find("updated") != string::npos)
        {
            documents.push_back(BTree(documents[updated]));
            updated++;
            path = filename.substr(0, filename.find("updated") - 1);
            path += " updated-" + to_string(updated) + ".bson";
        }
        else
        {

            if (updated == 0)
            {
                BTree temp = documents[updated];
                documents.push_back(temp);
                updated++;

            }
            path = filename.substr(0, filename.find(".bson"));
            path += " updated-1.bson";

        }
        for (int i = 0; i < current.size(); i++)
        {
            KEY info;
            info.first = Hash(current[i]._id.get_ID());
            documents[updated].remove(info);

        }
        this->apend_in_file(path);

        /*for (int j = 0; j < fkey_values.size(); j++)
        {
            for (int i = 0; i < indexes.size(); i++)
            {
                if (indexes[i].first == fkey_values[j].first)
                {
                    drop_index(fkey_values[j].first);
                    create_index(fkey_values[j].first);
                    break;
                }
            }
        }*/

    }


    void update_many(vector<pair<string, string>> key_values)
    {
        updateMany = true;

        for (Document doc : current)
        {
            for (int i = 0; i < key_values.size(); i++)
            {
                doc.update_one(key_values[i].first, string_to_val(key_values[i].second));
            }
            doc.view();

        }
        this->updateMany = true;
        string path = filename;
        updateMany = false;

        if (filename.find("updated") != string::npos)
        {
            documents.push_back(BTree(documents[updated]));
            updated++;
            path = filename.substr(0, filename.find("updated") - 1);
            path += " updated-" + to_string(updated) + ".bson";
        }
        else
        {

            if (updated == 0)
            {
                BTree temp = documents[updated];
                documents.push_back(temp);
                updated++;

            }
            path = filename.substr(0, filename.find(".bson"));
            path += " updated-1.bson";

        }
        for (int i = 0; i < current.size(); i++)
        {
            KEY info;
            info.first = Hash(current[i]._id.get_ID());
            documents[updated].remove(info);

        }
        this->apend_in_file(path);
        if (!indexes.empty()&&this->index_created)
        {
            this->index_created = false;
            string name = this->index_names.back();
            drop_index(name);
            create_index(name,index_key);

        }

    }
    
    void update_many_unset(vector< string> keys)
    {
        updateMany = true;

        for (Document doc : current)
        {
            for (int i = 0; i < keys.size(); i++)
            {
                doc.remove_one(keys[i]);
            }
            doc.view();

        }
        this->updateMany = true;
        string path = filename;
        updateMany = false;

        if (filename.find("updated") != string::npos)
        {
            documents.push_back(BTree(documents[updated]));
            updated++;
            path = filename.substr(0, filename.find("updated") - 1);
            path += " updated-" + to_string(updated) + ".bson";
        }
        else
        {

            if (updated == 0)
            {
                BTree temp = documents[updated];
                documents.push_back(temp);
                updated++;

            }
            path = filename.substr(0, filename.find(".bson"));
            path += " updated-1.bson";

        }
        for (int i = 0; i < current.size(); i++)
        {
            KEY info;
            info.first = Hash(current[i]._id.get_ID());
            documents[updated].remove(info);

        }
        this->apend_in_file(path);
        /*if (!indexes.empty())
            for (int j = 0; j < fkey_values.size(); j++)
            {
                for (int i = 0; i < indexes.size(); i++)
                {
                    if (indexes[i].first == fkey_values[j].first)
                    {
                        drop_index(fkey_values[j].first);
                        create_index(fkey_values[j].first);
                        break;
                    }
                }
            }*/

    }
    
    void update_one(string query)
    {
        string filter = query.substr(0, query.find("$") - 1);
        filter.erase(0, filter.find_first_not_of(" \t\n\r\f\v"));
        filter.erase(filter.find_last_not_of(" \t\n\r\f\v,") + 1);
        string updates = query.substr(query.find("$") - 1, query.size() - query.find("$") + 1);
        
        bool set = false;
        bool unset = false;
        bool push = false;
        bool pull = false;
        bool all = false;
        bool is_array = false;
        Filter obj = updates_processor(updates, set, unset, push, pull, all, is_array);
        Filter id = filter_processor(filter, is_array,all);
        int type = obj.get_type();


        if (id.get_type() == 0)
        {
            if (type == 1)
            {
                pair<string, string> key_val = obj.asKey_value();
               
               this->update_one(id.asString(), key_val.first, key_val.second);

            }

            else  if (type == 2)
            {
                pair<string, vector<string>> key_values = obj.askey_values();
                // return find(key_values.first, key_values.second, all);
            }

            else if (type == 3)
            {
                vector<pair<string, string>> key_values = obj.askeys_values();
                this->update_one(id.asString(), key_values);


                
            }

            else if (type == 4)
            {
                pair<string, vector<JsonValue>> key_values = obj.askey_vector();
                // return find(key_values.first, key_values.second);
            }

            else if (type == 5)
            {
                pair<string, pair<vector<string>, string>> key_nested_val = obj.askey_nestedKey();
                // return find(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second);
            }

            else  if (type == 6)
            {
                pair<string, pair<vector<string>, vector<string>>> key_nested_val = obj.asKey_nested_values();
                // return find(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second, all, is_array);
            }
            else  if (type == 7)
            {
                pair<string, pair<vector<string>, vector<JsonValue>>> key_nested_val = obj.askey_nested_array();
                // return find(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second, is_array);

            }
            else if (type == 8 && unset)
            {
                vector<string> keys = obj.askeys();
                this->update_one_unset(id.asString(), keys);
            }
        }
    }
    void update_many(string query)
    {
        updateMany = true;
        string filter = query.substr(0, query.find("$") - 1);
        filter.erase(0, filter.find_first_not_of(" \t\n\r\f\v"));
        filter.erase(filter.find_last_not_of(" \t\n\r\f\v,") + 1);
        string updates = query.substr(query.find("$") - 1, query.size() - query.find("$") + 1);
        current=find(filter);
        bool set = false;
        bool unset = false;
        bool push = false;
        bool pull = false;
        bool all = false;
       bool is_array = false;
       Filter obj = updates_processor(updates,set,unset,push,pull,all,is_array);
       int type = obj.get_type();

    

       if (type == 1)
       {
           pair<string, string> key_val = obj.asKey_value();
           vector<pair<string, string>> key_values;
           key_values.push_back(key_val);
           update_many(key_values);
           
       }

       else  if (type == 2)
       {
           pair<string, vector<string>> key_values = obj.askey_values();
          // return find(key_values.first, key_values.second, all);
       }

       else if (type == 3)
       {
           vector<pair<string, string>> key_values = obj.askeys_values();


           update_many(key_values);
       }

       else if (type == 4)
       {
           pair<string, vector<JsonValue>> key_values = obj.askey_vector();
          // return find(key_values.first, key_values.second);
       }

       else if (type == 5)
       {
           pair<string, pair<vector<string>, string>> key_nested_val = obj.askey_nestedKey();
          // return find(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second);
       }

       else  if (type == 6)
       {
           pair<string, pair<vector<string>, vector<string>>> key_nested_val = obj.asKey_nested_values();
          // return find(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second, all, is_array);
       }
       else  if (type == 7)
       {
           pair<string, pair<vector<string>, vector<JsonValue>>> key_nested_val = obj.askey_nested_array();
          // return find(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second, is_array);

       }
       else if(type== 8 && unset)
	   {
		   vector<string> keys = obj.askeys();
		   update_many_unset(keys);
	   }

    }
    //Delete
  
    void delete_one(string id)
    {
        int hash = Hash(id);
		KEY info = documents[updated].getkey(hash);
		if (info.second.first == "")
		{
			cout << "Document not found" << endl;
			return;
		}
        documents[updated].remove(info);
        cout << "{" << endl;
        cout << "   _id: " << id << endl;
        cout<<"   Document deleted"<<endl;
        cout << "}" << endl;
	

    }
    void delete_many(string key, string value)
	{
		vector<Document> docs = find(key, value);
		for (int i = 0; i < docs.size(); i++)
		{
			KEY info;
			info.first = Hash(docs[i]._id.get_ID());
			documents[updated].remove(info);
		}
		cout << "{" << endl;
		cout << "   " << docs.size() << " documents deleted" << endl;
		cout << "}" << endl;
	}
	void delete_many(vector<pair<string, string>> key_values)
	{
		vector<Document> docs = find(key_values);
		for (int i = 0; i < docs.size(); i++)
		{
			KEY info;
			info.first = Hash(docs[i]._id.get_ID());
			documents[updated].remove(info);
		}
		cout << "{" << endl;
		cout << "   " << docs.size() << " documents deleted" << endl;
		cout << "}" << endl;
	}


    void deleteOne(string query)
    {
        bool d;
        Filter id=filter_processor(query,d,d);
        if(id.get_type()==0)
		{
			delete_one(id.asString());
		}

    }
    void delete_many(string query)
	{
        vector<Document> docs = find(query);
        for (int i = 0; i < docs.size(); i++)
        {
            KEY info;
            info.first = Hash(docs[i]._id.get_ID());
            documents[updated].remove(info);
        }
        cout << "{" << endl;
        cout << "   " << docs.size() << " documents deleted" << endl;
        cout << "}" << endl;

	}
    
    // indexing
    void create_index(string name,string key)
    {
        index_name = name;
        indexes.push_back(make_pair(name, IndexTree(100)));
        queue<node*> q = documents[updated].getnodes();
        node* temp;
        
        while (!q.empty())
        {
			temp = q.front();
			q.pop();
            for (int i = 0; i < temp->numKeys; i++)
            {
				create_index_one(key, temp->keys[i]);
            }
        }
        
		
        current.clear();
    }
    void drop_index(string name)
    {
        int index = -1;
        for (int i = 0; i < indexes.size(); i++)
        {
            if (indexes[i].first == name)
            {
                index = i;
                break;
            }
        }
        if(index>-1)
        indexes.erase(indexes.begin() + index);
    }
	
    void createIndex(string query)
    {
      Filter keys=key_processor(query);
      if (keys.get_type() == 8)
      {
          vector<string> key = keys.askeys();
          string name=key[0];
          for (int i = 1; i < key.size(); i++)
          {
              name=name+"_"+key[i];

          }
          create_index(name,key[key.size()-1]);

      }

    }
    void dropIndex(string query)
    {
        Filter keys = key_processor(query);
        if (keys.get_type() == 8)
        {
            vector<string> key = keys.askeys();
            string name = key[0];
            for (int i = 1; i < key.size(); i++)
            {
                name = name + "_" + key[i];

            }
            drop_index(name);
           

        }

    }


    void safe(fstream& file)
    {
        file<<this->m_name<<endl;
        queue<node*> q = documents[updated].getnodes();
        node* temp;
        while (!q.empty())
		{
			temp = q.front();
			q.pop();
			for (int i = 0; i < temp->numKeys; i++)
			{
				file<<temp->keys[i].first<<" "<<temp->keys[i].second.first<<" "<<temp->keys[i].second.second<<endl;

			}
            
		}
        file<<"end"<<endl;
        if(!indexes.empty())
		{
            for (int i = 0; i < indexes.size(); i++)
            {
				file<<indexes[i].first<<endl;
				
            }
            file << "end" << endl;
		}
       
        file<<"end"<<endl;
        

    }
    void load(ifstream& rdr)
    {
        rdr >> this->m_name;
        filename = m_name + ".bson";
        File = new fstream(filename, ios::binary | ios::out);
        documents.push_back(BTree(100));
        string line;
        KEY info;
        int first, last;
        string mid;
        int firstNumber;
        do 
        {
            
            rdr >> line;
            if (line == "end")
                break;
            
                
                firstNumber = stoi(line);
                string filename;
                int secondNumber;
                rdr >> filename >> secondNumber;
                info=make_pair(firstNumber, make_pair(filename, secondNumber));
                documents[updated].insert(info);
                   
               
               

            
        } while (line != "end");
        
        rdr >> line;
        if (line != "end")
        {
            do
            {
               
                if (line != "end")
                {
                    create_index(line, line);
                }
                rdr >> line;
            } while (line != "end");
        }
        
       
       
    }

    friend class MongoDB;


};


class MongoDB
{
private:
    unordered_map<string, Collection> database;
    string name;
    int Hash(const string& str)
    {
        int hash = 5381;
        for (int i = 0; i < str.length(); i++)
        {
            hash = ((hash << 5) + hash) + str[i];
        }
        return hash;

    }
  //  trie collectionNames;
    string saveFile;
    Filter filter_processor(string query, bool& is_array, bool& all)
    {

        if (query[0] == '{')
            query = query.substr(1, query.size() - 2);

        string key;
        Filter filter;
        bool nested = false;
        vector<string> nested_keys;
        key = query.substr(0, query.find(":"));
        int shrink = key.find(" ");
        string value;
        if (shrink != string::npos)
            key = key.substr(0, shrink);

        if (key == "_id")
        {
            int start = query.find("\"");
            int end = query.find("\"", start + 1);
            string value = query.substr(start + 1, end - start - 1);
            filter = value;
            return filter;


        }
        else
        {
            if (key.find(".") != string::npos)
            {
                nested = true;
                int si = key.find(".");
                int ei = key.find("\"", si);
                key = key.substr(1, key.find(".") - 1);

                while (si < ei)
                {
                    int start = si;
                    int end = query.find(".", start + 1);
                    if (end == string::npos)
                        end = ei;
                    value = query.substr(start + 1, end - start - 1);
                    nested_keys.push_back(value);
                    si = end + 1;
                    ei = query.find(".", si);
                }


            }
            query = query.substr(query.find(":") + 1, query.size() - query.find(":") - 1);
            if (query.find(",") == string::npos)
            {


                int start = query.find("\"");
                int end = query.find("\"", start + 1);
                value = query.substr(start + 1, end - start - 1);
                if (!nested)
                {
                    pair<string, string> key_val(make_pair(key, value));

                    filter = key_val;
                }
                else
                {
                    pair<string, pair <vector<string>, string>> key_nested_val;
                    key_nested_val.first = key;
                    key_nested_val.second.first = nested_keys;
                    key_nested_val.second.second = value;
                    filter = key_nested_val;
                }

                return filter;
            }
            else if (query.find("[") != string::npos)
            {
                if (query.find("$all") == string::npos)
                {
                    vector<JsonValue> values;

                    int si = query.find("[");
                    int ei = query.find("]");
                    while (si < ei)
                    {
                        int start = query.find("\"", si + 1);
                        int end = query.find("\"", start + 1);
                        value = query.substr(start + 1, end - start - 1);
                        values.push_back(value);
                        si = end + 1;
                        ei = query.find("]", si);
                    }
                    if (!nested)
                    {
                        pair<string, vector<JsonValue>> key_val;
                        key_val.first = key;
                        key_val.second = values;
                        filter = key_val;
                        is_array = true;
                    }
                    else
                    {
                        pair<string, pair<vector<string>, vector<JsonValue>>>key_val;
                        key_val.first = key;
                        key_val.second.first = nested_keys;
                        key_val.second.second = values;

                        filter = key_val;
                    }
                    return filter;
                }
                else
                {
                    all = true;
                    vector<string> values;
                    int si = query.find("[");
                    int ei = query.find("]");
                    while (si < ei)
                    {
                        int start = query.find("\"", si + 1);
                        int end = query.find("\"", start + 1);
                        value = query.substr(start + 1, end - start - 1);
                        values.push_back(value);
                        si = end + 1;
                        ei = query.find("]", si);
                    }
                    if (!nested)
                    {
                        pair<string, vector<string>> key_val;
                        key_val.first = key;
                        key_val.second = values;
                        filter = key_val;
                        is_array = true;
                    }
                    else
                    {
                        pair<string, pair<vector<string>, vector<string>>>key_val;
                        key_val.first = key;
                        key_val.second.first = nested_keys;
                        key_val.second.second = values;
                        filter = key_val;

                    }

                    return filter;

                }

            }
            else if (query.find("{") != string::npos)
            {
                vector<string> values;
                int si = query.find("{");
                int ei = query.find("}");
                while (si < ei)
                {
                    int start = query.find("\"", si + 1);
                    int end = query.find("\"", start + 1);
                    value = query.substr(start + 1, end - start - 1);
                    values.push_back(value);
                    si = end + 1;
                    ei = query.find("}", si);
                }
                if (!nested)
                {
                    pair<string, vector<string>> key_val;
                    key_val.first = key;
                    key_val.second = values;
                    filter = key_val;
                    is_array = true;
                }
                else
                {
                    pair<string, pair<vector<string>, vector<string>>>key_val;
                    key_val.first = key;
                    key_val.second.first = nested_keys;
                    key_val.second.first = values;
                    filter = key_val;
                }
                return filter;

            }

            else
            {
                int start = query.find("\"");
                int end = query.find("\"", start + 1);
                value = query.substr(start + 1, end - start - 1);
                vector < pair<string, string>> key_values;
                key_values.push_back(make_pair(key, value));
                end = query.find(",", end + 1) + 1;
                while (end < query.size())
                {
                    while (query[end] == ' ' && end < query.size())
                        end++;
                    if (query[end] == '}' || end >= query.size())
                        break;
                    key = query.substr(end, query.find(":", end + 1) - end - 1);
                    end = query.find("\"", end + 1);
                    value = query.substr(end + 1, query.find("\"", end + 1) - end - 1);
                    end = query.find("\"", end + 1) + 1;
                    key_values.push_back(make_pair(key, value));

                }
                if (!nested)
                {
                    filter = key_values;
                }
                else
                {
                    pair<string, pair<vector<string>, vector<pair<string, string>>>>key_val;
                    key_val.first = key;
                    key_val.second.first = nested_keys;
                    key_val.second.second = key_values;
                    filter = key_values;
                }
                return filter;
            }
        }



    }
    void safe()
    {
        fstream File(saveFile, ios::out | ios::beg);

        if (!File.is_open())
        {
            cout << "Error in saving\n";
            return;
        }
        File.clear();
        File << name << endl;
        for(auto it = database.begin(); it != database.end(); it++)
		{
			
			 it->second.safe(File);
		}
        File << "dbEnd" << endl;
		File.close();
      
        


    }

    void load(ifstream& rdr)
    {
        string end="dbEnd";
        rdr >> name;
        int  prev;
        while (!rdr.eof())
        {
             prev=rdr.tellg();
            rdr>>end;
           
            if (end == "dbEnd")
				break;
            else
            {
               
                rdr.seekg(prev);
                
                Collection collection(rdr);
                database.insert({ collection.get_name(), collection });
            }
        }
       
    }

public:
    MongoDB() = default;
    
    MongoDB(string _name) //: collectionNames()
    {
        name = _name;
        saveFile = name+"_metadata.txt";



    }
    MongoDB(const char* fileName) //: collectionNames()
    {
        ifstream rdr(fileName);
        this->saveFile = fileName;
        load(rdr);
        
    }
    MongoDB(const MongoDB& db) 
    {
        name = db.name;
        database = db.database;
       // collectionNames = db.collectionNames;
        this->saveFile= db.saveFile;

    }
    MongoDB& operator=(const MongoDB& db)
    {
        if (this != &db)
        {
            name = db.name;
            database = db.database;
            //collectionNames = db.collectionNames;
          this->saveFile = db.saveFile;
        }
        return *this;
    }
    void import_collection(const char* path)
    {
        Collection collection(path);
        database.insert({ collection.get_name(), collection });
      //  collectionNames.insert(collection.get_name());
        safe();

    }
    Collection& get_collection(const string& collectionName)
    {
        if (database.find(collectionName) != database.end())
        {
            return database[collectionName];
        }
        else
        {
            throw "Collection not found";
        }



    }
    void create_collection(const string& collectionName)
    {
        if(database.find(collectionName)!=database.end())
        {
            cout << " A collection of this name already exists" << endl;
            return;
        }
        Collection collection(collectionName);
        database.insert({ collection.get_name(), collection });
       // collectionNames.insert(collection.get_name());


    }
    void drop_collection(const string& collectionName)
    {
        if (database.find(collectionName)!=database.end())
        {
            database.erase(collectionName);
        }

    }
    void show_collections()
    {
        for (auto it = database.begin(); it != database.end(); it++)
        {
            cout << it->first << endl;

        }
    }
    string getCollectionsName()
	{
        string s = "";
		for (auto it = database.begin(); it != database.end(); it++)
		{
			s += it->first + "\n";
		}
		return s;
		
	}
    string get_name() const { return name; }
    void set_name(const string& _name) { name = _name; }
    void print_collectionsNames()
    {
        //collectionNames.printWords();
    }
    bool check_collection(string Collection_name)
    {
        if (database.find(Collection_name) != database.end())
        {
			return true;
		}
        return false;
    }
    void insert_one(string Collection_name, string query)
    {
        if (database.find(Collection_name) != database.end())
        {
            database[Collection_name].insert_one(query);
        }
        else
        {
            cout << "Collection not found" << endl;
        }
        safe();
    }
    void insert_many(string Collection_name, string query)
    {
        if (database.find(Collection_name) != database.end())
        {
            database[Collection_name].insert_many(query);
        }
        else
        {
            cout << "Collection not found" << endl;
        }
        safe();
    }
    void find(string Collection_name, string query)
    {
        if (database.find(Collection_name) != database.end())
        {

            if (query == "")
                database[Collection_name].find();
            else
            {
                Filter obj;
                bool is_array = false;
                bool all = false;
                obj = filter_processor(query, is_array, all);
                int type = obj.get_type();

                if (type == 0)
                    database[Collection_name].find_one(obj.asString());

                else if (type == 1)
                {
                    pair<string, string> key_val = obj.asKey_value();
                    database[Collection_name].view(key_val.first, key_val.second);
                }

                else  if (type == 2)
                {
                    pair<string, vector<string>> key_values = obj.askey_values();
                    database[Collection_name].view(key_values.first, key_values.second, all);
                }

                else if (type == 3)
                {
                    vector<pair<string, string>> key_values = obj.askeys_values();


                    database[Collection_name].view(key_values);
                }

                else if (type == 4)
                {
                    pair<string, vector<JsonValue>> key_values = obj.askey_vector();
                    database[Collection_name].view(key_values.first, key_values.second);
                }

                else if (type == 5)
                {
                    pair<string, pair<vector<string>, string>> key_nested_val = obj.askey_nestedKey();
                    database[Collection_name].view(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second);
                }

                else  if (type == 6)
                {
                    pair<string, pair<vector<string>, vector<string>>> key_nested_val = obj.asKey_nested_values();
                    database[Collection_name].view(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second, all, is_array);
                }
                else  if (type == 7)
                {
                    pair<string, pair<vector<string>, vector<JsonValue>>> key_nested_val = obj.askey_nested_array();
                    database[Collection_name].view(key_nested_val.first, key_nested_val.second.first, key_nested_val.second.second, is_array);

                }


            }
        }

        else
        {
            cout << "Collection not found" << endl;
        }

    }
    string get_filter_docs(string Collection_name, string query)
    {
        if (database.find(Collection_name) != database.end())
        {
           return database[Collection_name].get_documnets(query);
        }
        else
        {
            return "Collection not found";
        }

    }
    void find_one(string Collection_name, string query)
    {
        if (database.find(Collection_name) != database.end())
        {
            bool d = false;
            Filter obj = filter_processor(query, d, d);
            if (obj.get_type() == 0)
            {
                string id = obj.asString();
                database[Collection_name].view(id);
            }
        }
        else
        {
            cout << "Collection not found" << endl;
        }

    }

    void update_one(string Collection_name, string query)
    {
        if (database.find(Collection_name) != database.end())
        {
            database[Collection_name].update_one(query);
        }
        else
        {
            cout << "Collection not found" << endl;
        }
        safe();
    }
    void update_many(string Collection_name, string query)
    {
        if (database.find(Collection_name) != database.end())
        {
            database[Collection_name].update_many(query);
        }
        else
        {
            cout << "Collection not found" << endl;
        }
        safe();
    }
    void delete_one(string Collection_name, string query)
    {
        if (database.find(Collection_name) != database.end())
        {
            database[Collection_name].deleteOne(query);
        }
        else
        {
            cout << "Collection not found" << endl;
        }
        safe();
        
    }
    void delete_many(string Collection_name, string query)
    {
        if (database.find(Collection_name) != database.end())
        {
            database[Collection_name].delete_many(query);
        }
        else
        {
            cout << "Collection not found" << endl;

        }



        safe();



    };

    void createIndex(string Collection_name, string query)
    {
		    if (database.find(Collection_name) != database.end())
		    {
			    database[Collection_name].createIndex(query);
		    }
		    else
		    {
			    cout << "Collection not found" << endl;

		    }
            safe();
	}
    void dropIndex(string Collection_name, string query)
   {
       if (database.find(Collection_name) != database.end())
       {
           database[Collection_name].dropIndex(query);
       }
       else
       {
           cout << "Collection not found" << endl;

       }

   }


};


#endif

