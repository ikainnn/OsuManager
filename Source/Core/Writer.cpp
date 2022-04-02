#include "Writer.hpp"

namespace kaede::api::core
{
	template <>
	auto write<std::string>(std::ofstream& stream, const std::string& value) -> void
	{
		write<std::string>(stream, value.size(), value.data());
	}
}