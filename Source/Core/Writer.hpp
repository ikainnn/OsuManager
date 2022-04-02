#ifndef WRITER_HPP
#define WRITER_HPP

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace kaede::api::core
{
	template <class UnknownType>
	auto write(std::ofstream& stream, const std::streamsize writeSize, const UnknownType& value) -> void
	{
		stream.write(reinterpret_cast<const char*>(&value), writeSize);
	}

	template <class UnknownType>
	auto write(std::ofstream& stream, const UnknownType& value) -> void
	{
		write<UnknownType>(stream, sizeof(UnknownType), value);
	}

	template <>
	auto write<std::string>(std::ofstream& stream, const std::string& value) -> void;
}

#endif