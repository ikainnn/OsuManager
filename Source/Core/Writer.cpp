#include "Writer.hpp"

namespace kaede::api::core
{
    template <>
    auto write<std::string>(std::ofstream& stream, const std::string& value) -> void
    {
        for ( auto _char : value ) write<char>(stream, _char);
    }
}