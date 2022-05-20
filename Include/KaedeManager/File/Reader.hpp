#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace kaede::api::core
{
    template <class T>
    auto read(std::ifstream& stream, const std::streamsize readSize) -> T
    {
        T result { };
        stream.read(reinterpret_cast<char*>(&result), readSize);
        return result;
    }

    template <class T>
    auto read(std::ifstream& stream) -> T
    {
        return read<T>(stream, sizeof(T));
    }

    template <>
    auto read(std::ifstream& stream, std::streamsize readSize) -> std::string;
}
