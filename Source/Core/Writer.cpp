#include "../../Include/KaedeManager/File/Writer.hpp"

namespace kaede::api::core
{
    auto write(std::ofstream& stream, const std::string& value) -> void
    {
        for (auto byte : value) write<char>(stream, byte);
    }
}