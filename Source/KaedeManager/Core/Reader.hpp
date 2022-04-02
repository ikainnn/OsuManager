#ifndef READER_HPP
#define READER_HPP

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace kaede::api::core
{
	template <class UnknownType>
	auto read(std::ifstream& stream, const std::streamsize readSize) -> UnknownType
	{
		UnknownType result { };
		stream.read(reinterpret_cast<char*>(&result), readSize);
		return result;
	}

	template <class UnknownType>
	auto read(std::ifstream& stream) -> UnknownType
	{
		return read<UnknownType>(stream, sizeof(UnknownType));
	}

	template <>
	auto read<std::string>(std::ifstream& stream, std::streamsize readSize) -> std::string;
}

#endif