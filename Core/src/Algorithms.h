#pragma once

#include <iomanip>
#include <random>
#include <string>
#include <sstream>

namespace Algorithm
{
    inline uint32_t HexToUInt32(const std::string& str)
    {
        uint32_t value;
        std::stringstream ss;

        ss << std::hex << str;
        ss >> value;

        return value;
    }

    inline std::string UInt32ToHex(const uint32_t value)
    {
        std::stringstream ss;

        ss << std::hex << std::setfill('0') << std::setw(8) << value;

        return ss.str();
    }

    inline uint8_t HexToUInt8(const std::string& str)
    {
        uint32_t value;
        std::stringstream ss;

        ss << std::hex << str;
        ss >> value;

        return (uint8_t)value;
    }

    inline std::string UInt8ToHex(const uint8_t value)
    {
        std::stringstream ss;

        ss << std::hex << std::setfill('0') << std::setw(2) << (int)value;

        return ss.str();
    }

    inline uint32_t RandUInt32()
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_int_distribution<uint32_t> dis(0, 0xFFFFFFFF);

        return dis(gen);
    }

    inline float Clamp(float val, float minimum, float maximum)
    {
        return (val < minimum) ? minimum : ((val > maximum) ? maximum : val);
    }
}