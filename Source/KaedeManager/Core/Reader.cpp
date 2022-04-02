#include "Reader.hpp"

namespace kaede::api::core
{
	template <>
	auto read<std::string>(std::ifstream& stream, const std::streamsize readSize) -> std::string
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