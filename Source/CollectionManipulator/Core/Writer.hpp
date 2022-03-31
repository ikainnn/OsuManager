#ifndef WRITER_HPP
#define WRITER_HPP

#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace ws::cm::core
{
	template <class UnknownType>
	void write(std::ofstream& stream, const std::streamsize writeSize, const UnknownType& value)
	{
		stream.write(reinterpret_cast<const char*>(&value), writeSize);
	}

	template <class UnknownType>
	void write(std::ofstream& stream, const UnknownType& value)
	{
		write<UnknownType>(stream, sizeof(UnknownType), value);
	}
}

#endif