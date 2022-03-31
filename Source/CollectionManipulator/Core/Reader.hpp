#ifndef READER_HPP
#define READER_HPP

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace ws::cm::core
{
	template <class UnknownType>
	UnknownType read(std::ifstream& stream, const std::streamsize readSize)
	{
		UnknownType result { };
		stream.read(reinterpret_cast<char*>(&result), readSize);
		return result;
	}

	template <class UnknownType>
	UnknownType read(std::ifstream& stream)
	{
		return read<UnknownType>(stream, sizeof(UnknownType));
	}

	template <>
	auto read<std::string>(std::ifstream& stream, std::streamsize readSize) -> std::string;
}

#endif