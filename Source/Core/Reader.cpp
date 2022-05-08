#include "../../Include/KaedeManager/File/Reader.hpp"

#include <ranges>
#include <algorithm>

namespace kaede::api::core
{
    auto read(std::ifstream& stream, const std::streamsize readSize) -> std::string
    {
        std::string result { }; result.reserve(readSize);

        for (auto pos = 0; pos < readSize; ++pos)
        {
            result.push_back(read<char>(stream));
        }

        return result;
    }
}