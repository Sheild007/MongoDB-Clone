#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<unordered_map>
#include <variant>
#include <cstdint>
#include <cstring>

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
    unordered_map<string,JsonValue> asObj() const { return get<unordered_map<string, JsonValue>>(m_value); }
    vector<JsonValue> asVector() const { return get<vector<JsonValue>>(m_value); }
    int getType() const { return m_type; }

};

unordered_map<string, JsonValue> JSON_obj(int& index, vector<char>& bsonData,int obj_size )
{

    unordered_map<string, JsonValue> object;
    int currentPosition = index;
    while(currentPosition < obj_size+index)
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
					currentPosition +=4;
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
            unordered_map<string, JsonValue> obj = JSON_obj(currentPosition, bsonData,obj_size);
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

vector<unordered_map<string, JsonValue>> ConvertBSONtoJSON(const char* filename) 
{
    ifstream file(filename, ios::binary);
    unordered_map<string, JsonValue> json;
   vector< unordered_map<string, JsonValue>> json_vec;
     
    if (!file.is_open()) 
    {
        cout << "Unable to open file!";
        return json_vec;
    } 

    int currentPosition = 0;
    int cursor=0;
   
    while (!file.eof())
    {

        char fileSizeBytes[4];
        int pos=currentPosition;
        pos+=cursor;
        file.seekg(pos,ios::beg);
        file.read(fileSizeBytes, 4);
         cursor = file.tellg();
        int docSize = littleEndianToInt(fileSizeBytes);

        vector<char> bsonData(docSize);
        file.read(bsonData.data(), docSize);
        currentPosition = 0;
        json = JSON_obj(currentPosition, bsonData, docSize);
        json_vec.push_back(json);
        
        
    }
    file.close();

    return json_vec;
}

void print_JSON(unordered_map<string, JsonValue> json)
{
    cout << "{\n  ";
    int size = json.size();
    int count = 0;
    
    for (auto& [key, value] : json)
    {
        if (value.getType() == 0)
        {
            cout << "\"" << key << "\": " << "\"" << value.asString()<<"\"";
            count++;
        }
        else if (value.getType() == 1)
        {
            cout << "\"" << key << "\": " << value.asInt() ;
            count++;
        }
        else if (value.getType() == 2)
        {
            cout << "\"" << key << "\": " << (value.asBool() ? "true" : "false") ;
            count++;
        }
        else if (value.getType() == 3)
        {
            cout << "\"" << key << "\": " << value.asDouble() ;
            count++;
        }
        else if (value.getType() == 4)
        {
            cout << "\"" << key << "\": " << value.asLong() ;
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
						cout << array[i].asDouble() ;
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
			print_JSON(obj);
			count++;
             
        }


        if (count != size)
        {
			cout << ",\n  ";
		}
    }
    
    cout << "\n}";
}

void create_JSON(unordered_map<string, JsonValue> json)
{
    fstream outfile("output.bson", ios::out);
    outfile << "{\n  ";
    int size = json.size();
    int count = 0;

    

    for (auto& [key, value] : json)
    {
        if (value.getType() == 0)
        {
            outfile << "\"" << key << "\": " << "\"" << value.asString() << "\"";
            count++;
        }
        else if (value.getType() == 1)
        {
            outfile << "\"" << key << "\": " << value.asInt();
            count++;
        }
        else if (value.getType() == 2)
        {
            outfile << "\"" << key << "\": " << (value.asBool() ? "true" : "false");
            count++;
        }
        else if (value.getType() == 3)
        {
            outfile << "\"" << key << "\": " << value.asDouble();
            count++;
        }
        else if (value.getType() == 4)
        {
            outfile << "\"" << key << "\": " << value.asLong();
            count++;
        }
        else if (value.getType() == 5)
        {
            outfile << "\"" << key << "\": [ ";
            vector<JsonValue> array = value.asVector();
            for (int i = 0; i < array.size(); i++)
            {

                switch (array[i].getType())
                {
                case 0:
                {
                    outfile << "\"" << array[i].asString() << "\"";
                    break;
                }
                case 1:
                {
                    outfile << array[i].asInt();
                    break;
                }
                case 2:
                {
                    outfile << (array[i].asBool() ? "true" : "false");
                    break;
                }
                case 3:
                {
                    outfile << array[i].asDouble();
                    break;
                }
                case 4:
                {
                    outfile << array[i].asLong();
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
                    outfile << ", ";
                }
            }
            count++;
            outfile << "]";



        }
        else if (value.getType() == 6)
        {
            outfile << "\"" << key << "\": ";
            unordered_map<string, JsonValue> obj = value.asObj();
            create_JSON(obj);
            count++;

        }


        if (count != size)
        {
            outfile << ",\n  ";
        }
    }

    outfile << "\n}";
}


int main1() 
{
    const char* filename = "sales.bson";
    vector<unordered_map<string,JsonValue>>json = ConvertBSONtoJSON(filename);
    for (int i = 0; i < json.size(); i++)
    {
		cout << "Object " << i + 1 << endl;
		print_JSON(json[i]);
		cout << endl;
	}
   


    return 0;
}


