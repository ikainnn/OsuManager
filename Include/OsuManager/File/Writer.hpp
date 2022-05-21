#pragma once

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace osu_manager::api::core
{
    template <class T>
    auto write(std::ofstream& _stream, const std::streamsize _size, const T& _value) -> void
    {
        _stream.write(reinterpret_cast<const char*>(&_value), _size);
    }
    
    template <class T>
    auto write(std::ofstream& _stream, const T& _value) -> void
    {
        write<T>(_stream, sizeof(T), _value);
    }

    template <>
    auto write<>(std::ofstream& _stream, const std::string& _value) -> void;

}