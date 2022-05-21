#pragma once

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

namespace osu_manager::api
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

    auto read_collection_from_stream(std::ifstream& _stream) -> Collections;
    void write_collection_to_stream(std::ofstream& _stream, const Collections& _collections);
}
