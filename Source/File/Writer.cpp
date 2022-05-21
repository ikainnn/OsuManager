#include "../../Include/KaedeManager/File/Writer.hpp"

namespace kaede::api::core
{
    template<>
    auto write<>(std::ofstream& _stream, const std::string& _value) -> void
    {
        for (auto byte : _value) 
            write<char>(_stream, byte);
    }
}