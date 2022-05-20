#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace kaede::api::core
{
    template <class T>
    auto write(std::ofstream& stream, const std::streamsize writeSize, const T& value) -> void
    {
        stream.write(reinterpret_cast<const char*>(&value), writeSize);
    }
    
    template <class T>
    auto write(std::ofstream& stream, const T& value) -> void
    {
        write<T>(stream, sizeof(T), value);
    }

    template <>
    auto write<>(std::ofstream& stream, const std::string& value) -> void;

}