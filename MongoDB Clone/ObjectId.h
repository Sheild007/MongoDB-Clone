#pragma once

#ifndef OBJECTID_H
#define OBJECTID_H


#include <ctime>
#include <cstdlib>
#include <iomanip>
#include <sstream>
using namespace std;


class ObjectId
{
private:
    static uint32_t counter;
    static uint32_t processIdentifier;
    static uint32_t getRandomValue();

public:
    uint32_t timestamp;
    uint32_t randomValue;
    uint32_t increment;

    ObjectId()
    {
        timestamp = static_cast<uint32_t>(time(nullptr));
        randomValue = processIdentifier;
        increment = (++counter) % 0xFFFFFFu; // 3-byte counter, max value 0xFFFFFF
    }
    ObjectId(string id)
	{
		set_ID(id);
	}
    ObjectId(const ObjectId& other)
	{
		timestamp = other.timestamp;
		randomValue = other.randomValue;
		increment = other.increment;
	}

    static void initialize()
    {
        srand(static_cast<unsigned int>(time(nullptr)));
        processIdentifier = getRandomValue();
        counter = static_cast<uint32_t>(rand()) % 0xFFFFFFu;
    }
    string get_ID() const
    {
        std::stringstream stream;
        stream << std::hex << std::setfill('0') << std::setw(8) << timestamp
            << std::setw(8) << randomValue << std::setw(6) << increment;
        return stream.str();
    }
    void set_ID(string id)
	{
		std::stringstream stream;
		stream << std::hex << id.substr(0, 8);
		stream >> timestamp;
		stream.clear();
		stream << std::hex << id.substr(8, 8);
		stream >> randomValue;
		stream.clear();
		stream << std::hex << id.substr(16, 6);
		stream >> increment;
	}
    uint32_t get_timestamp() const
	{
		return timestamp;
	}
    uint32_t get_randomValue() const
    {
        return randomValue;
    }
    uint32_t get_increment() const
    {
        return increment;
    }

    void set_timestamp(uint32_t timestamp)
	{
		this->timestamp = timestamp;
	}
    void set_randomValue(uint32_t randomValue)
	{
		this->randomValue = randomValue;
	}
	void set_increment(uint32_t increment)
	{
		this->increment = increment;
	}

};

uint32_t ObjectId::counter = 0;
uint32_t ObjectId::processIdentifier = 0;

uint32_t ObjectId::getRandomValue()
{
        srand(static_cast<unsigned int>(time(nullptr)));
        return static_cast<uint32_t>(rand());
   
}
#endif