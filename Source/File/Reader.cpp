#include "../../Include/OsuManager/File/Reader.hpp"

#include <ranges>
#include <algorithm>

namespace osu_manager::api::core
{
    template <>
    auto read<>(std::ifstream& _stream, const std::streamsize _size) -> std::string
    {
        std::string result ( _size, ' ' );

        for (auto& chr : result)
            chr = read<char>(_stream);
            
        return result;
    }
}