#include "Reader.hpp"

namespace ws::cm::core
{
	template <>
	std::string read<std::string>(std::ifstream& stream, const std::streamsize readSize)
	{
		std::string result { };

		result.reserve(readSize);
		for (auto pos = 0; pos < readSize; ++pos)
		{
			result.push_back(read<char>(stream));
		}

		return result;
	}
}