#include "../../Include/KaedeManager/File/Reader.hpp"

#include <ranges>
#include <algorithm>

namespace kaede::api::core
{
    template <>
    auto read<>(std::ifstream& stream, const std::streamsize readSize) -> std::string
    {
        std::string result ( readSize, ' ' );

        for (auto& chr : result)
            chr = read<char>(stream);
            
        return result;
    }
}