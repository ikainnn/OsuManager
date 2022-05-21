#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace kaede::api::core
{
    template <class T>
    auto read(std::ifstream& _stream, const std::streamsize _size) -> T
    {
        T result { };
        _stream.read(reinterpret_cast<char*>(&result), _size);
        return result;
    }

    template <class T>
    auto read(std::ifstream& _stream) -> T
    {
        return read<T>(_stream, sizeof(T));
    }

    template <>
    auto read(std::ifstream& _stream, std::streamsize _size) -> std::string;
}
