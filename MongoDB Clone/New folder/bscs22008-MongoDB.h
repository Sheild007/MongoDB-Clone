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


enum types { s_type, i_type, b_type, d_type, l_type, a_type, o_type };
int littleEndianToInt(const char* bytes) {
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

size_t uniqueID::ID = 0;
using object = unordered_map<string, JsonValue>;
using ID=size_t;

class Document
{

private:
	object* document;
	ID m_id;
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
            cout<< "  \"_id\": " << m_id << ",\n";
            isFirst = false;
        }
        
        int size = json.size();
        int count = 0;

        for (auto& [key, value] : json)
        {
            if (value.getType() == 0)
            {
                cout << "\"" << key << "\": " << "\"" << value.asString() << "\"";
                count++;
            }
            else if (value.getType() == 1)
            {
                cout << "\"" << key << "\": " << value.asInt();
                count++;
            }
            else if (value.getType() == 2)
            {
                cout << "\"" << key << "\": " << (value.asBool() ? "true" : "false");
                count++;
            }
            else if (value.getType() == 3)
            {
                cout << "\"" << key << "\": " << value.asDouble();
                count++;
            }
            else if (value.getType() == 4)
            {
                cout << "\"" << key << "\": " << value.asLong();
                count++;
            }
            else if (value.getType() == 5)
            {
                cout << "\"" << key << "\": [ ";
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
                cout << "\"" << key << "\": ";
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
public:
    Document() = default;
    Document(const object& doc) : document(new object(doc)), m_id(uniqueID::getID())
    {
       // filename = "object"+to_string(m_id) + ".bson";
        //file=new fstream(filename, ios::binary | ios::out);
    }
    Document(const Document& doc) : document(doc.document), m_id(doc.m_id) 
    {
      //  this->filename=doc.filename;
       // this->file=doc.file;
    }
    Document(Document&& doc) : document(move(doc.document)), m_id(doc.m_id) 
    {
      //  this->filename=doc.filename;
		//this->file=doc.file;
    }
    Document& operator=(const Document& doc) 
    { 
        document = doc.document;
        m_id = doc.m_id; 
      //  filename=doc.filename;
       // file=doc.file;
    return *this; 
    }
    Document& operator=(Document&& doc) 
    {
        document = move(doc.document); 
        m_id = doc.m_id;
       // filename=doc.filename;
       // file=doc.file;
        return *this;
    }
    ~Document() = default;

	ID getID() const { return m_id; }
	object getDocument() const { return *document; }
	void setDocument(const object& doc) { document = new object(doc); }
	void setID(const ID& id) { m_id = id; }
	void addField(const string& field, const JsonValue& value) { document->insert({ field, value }); }
	void removeField(const string& field) { document->erase(field); }   
	void updateField(const string& field, const JsonValue& value) { document->at(field) = value; }
    void clear() { document->clear(); } 
    void print()
    {
        if (!is_strerilized)
        {
            isFirst = true;
            print_JSON(*document);
        }
        else
        cout<<"Document is not in memory"<<endl;    
       

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
    vector<Document> documents;
    string m_name;
    string filename;
    fstream* File;
    bool is_strerilized = false;
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
        int byte_size = get_byteSize(documentData);
        file.write(reinterpret_cast<const char*>(&byte_size), sizeof(int)); // Write document size
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
    object JSON_obj(int& index, vector<char>& bsonData, int obj_size)
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
                        unordered_map<string, JsonValue> obj = JSON_obj(currentPosition, bsonData, obj_size);
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
                currentPosition += 12;
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
                unordered_map<string, JsonValue> obj = JSON_obj(currentPosition, bsonData, obj_size);
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
public:
   Collection() = default;
   Collection(const string& name) : m_name(name) 
   {
       filename = m_name + ".bson";
       File = new fstream(filename, ios::binary | ios::out);
   
   }
   Collection(const Collection& collection) : m_name(collection.m_name), documents(collection.documents) 
   {
       filename = collection.filename;
	   File = collection.File;
   }
    Collection(const char* filename)
    {
        m_name = filename;
        m_name = m_name.substr(0, m_name.find("."));
        this->filename = filename;
        this->File = nullptr;
        ifstream file(filename, ios::binary);
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
            doc.document =new object( JSON_obj(currentPosition, bsonData, docSize));
            ID id = uniqueID::getID();
            doc.m_id = id;
            documents.push_back(doc);


        }
        file.close();

    }

    Document& findDocument(const ID& id)
    { 
        for (auto& doc : documents)
        {
			if(doc.getID() == id)
				return doc;
		}
    }

   

    void print()
    {
        cout << "Collection: " << m_name << endl;
        for (auto& doc : documents)
        {
			doc.print();
			cout << endl;
		}
	
     
    }
    void print(int start, int end)
    {
        if (start >= 0 && start < end && end <= documents.size())
        {
            cout << "Collection: " << m_name << endl;
            for (int i = start; i < end; i++)
            {
                documents[i].print();
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

       if (!File->is_open())
       {
           cout << "Unable to open file for writing!";
           return;
       }
       //write_obj(*document, *file);
       File->clear();
       for (auto& doc : documents)
       {
		   
		   write_obj(doc.getDocument(), *File);
           doc.sterilize();
	   }

       File->close();
       is_strerilized = true;
    }

    void deSterilize()
    {
       
        File->open(filename, ios::binary | ios::in);
        int currentPosition = 0;
        int cursor = 0;
        object json;
        int index = 0;
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

            vector<char> bsonData(docSize);
            File->read(bsonData.data(), docSize);
            currentPosition = 0;
            json = JSON_obj(currentPosition, bsonData, docSize);
            this->documents[index].setDocument(json);
            this->documents[index].is_strerilized = false;
            index++;


        }
        File->close();
        is_strerilized = false;

    }
	
    friend class MongoDB;


};


class MongoDB
{
private:
    BTree database;
    int Hash(const string& str)
    {
        int hash = 5381;
        for (int i = 0; i < str.length(); i++)
        {
            hash = ((hash << 5) + hash) + str[i];
        }
        return hash;

    }
    
public:
    MongoDB():database(100) {}
    void addCollection(const char* filename)
    {
		Collection* collection = new Collection(filename);
		int hash = Hash(collection->m_name);
        collection->sterilize();
		database.insert({ hash, collection });
	}
    void viewCollection(string collectionName)
    {
        int hash = Hash(collectionName);
        pair<int, Collection*> collection;
        collection.first = hash;
        try
        {
            collection.second = database.getkey(collection).second;
        }
        catch (const char* msg)
        {
            cout << msg << endl;
            return;
        }

        if (collection.second == nullptr)
        {
            cout << "Collection not found" << endl;
            return;
        }
        collection.second->deSterilize();
        collection.second->print();
        
    }


};







