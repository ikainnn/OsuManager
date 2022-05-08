#include "../../Include/KaedeManager/File/Writer.hpp"

namespace kaede::api::core
{
    template <>
    auto write<std::string>(std::ofstream& stream, const std::string& value) -> void
    {
        for (auto byte : value) write<char>(stream, byte);
    }
}