#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace kaede::api
{
    struct Collection
    {
        using BeatmapHashes = std::vector<std::string>;

        std::string  name;
        std::int8_t  nameLength;
        std::int32_t hashCount;

        BeatmapHashes hashes;
    };

    using Collections = std::pair<std::int32_t, std::vector<Collection>>;

    auto read_collection(std::ifstream& stream) -> Collections;
    void write_collection(std::ofstream& stream, const Collections& collections);
}
