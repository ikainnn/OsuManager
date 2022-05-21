
#include "../Include/KaedeManager/Collection.hpp"

#include "../Include/KaedeManager/File/Reader.hpp"
#include "../Include/KaedeManager/File/Writer.hpp"
#include "../Include/Logging.hpp"

#define KAEDE_ASSERT(condition, msg) if (!(condition)) kaede::logging::error(msg)

namespace kaede::api
{
    auto read_beatmap_hashes(std::ifstream& stream, std::size_t count) -> std::vector<std::string>;
    void write_beatmap_hashes(std::ofstream& stream, const kaede::api::Collection::BeatmapHashes& hashes);

    auto read_collection_from_stream(std::ifstream& stream) -> Collections
    {
        constexpr auto RELEASE_DATE = 0x1324204, MAXIMUM_DATE = 0x5F5BEBF;
        
        const auto gameVersion = core::read<std::int32_t>(stream);
        KAEDE_ASSERT((gameVersion >= RELEASE_DATE && gameVersion <= MAXIMUM_DATE), "Invalid collection date time. Is it corrupted?");

        const auto funcReadBytes = [&stream] (auto&& value) -> std::pair<std::int8_t, std::int8_t>
            { return { (value >> 8), ((value << 12) >> 12) % ((value >> 8) << 8) }; };
        std::vector<Collection> collections ( core::read<std::int32_t>(stream) );

        for (auto& collection : collections)
        {
            const auto [nameLength, flagValue] = funcReadBytes(core::read<std::int16_t>(stream));
            KAEDE_ASSERT(flagValue == 0x0B, "Invalid collection signature. Is it corrupted?");

            collection =
            { 
                .name       = core::read<std::string>(stream, nameLength),
                .nameLength = nameLength,
                .hashCount  = core::read<std::int32_t>(stream),
                .hashes     = read_beatmap_hashes(stream, collection.hashCount)
            };
        }

        return { gameVersion, collections };
    }

    void write_collection_to_stream(std::ofstream& stream, const Collections& collections)
    {
        const auto& [gameVersion, _collections] = collections;

        core::write<std::int32_t>(stream, gameVersion);
        core::write<std::int32_t>(stream, static_cast<std::int32_t>(_collections.size()));

        const auto funcWriteByte = [&stream](auto&& value)
            { core::write<std::int16_t>(stream, static_cast<std::int16_t>((value << 8) | 0x0B)); };

        for (const auto& collection : _collections)
        {
            funcWriteByte(collection.nameLength);
            core::write<std::string>(stream, collection.name);
            core::write<std::int32_t>(stream, collection.hashCount);

            write_beatmap_hashes(stream, collection.hashes);
        }
    }

    auto read_beatmap_hashes(std::ifstream& stream, const std::size_t count) -> Collection::BeatmapHashes
    {
        Collection::BeatmapHashes beatmapHashes ( count );

        const auto funcReadBytes = [&stream] (auto&& value) -> std::pair<std::int8_t, std::int8_t>
            { return { (value >> 8), ((value << 12) >> 12) % ((value >> 8) << 8) }; };

        for (auto& beatmapHash : beatmapHashes)
        {
            const auto [nameLength, flagValue] = funcReadBytes(core::read<std::int16_t>(stream));
            KAEDE_ASSERT(flagValue == 0x0B, "Invalid collection signature. Is it corrupted?");

            beatmapHash = core::read<std::string>(stream, nameLength);
        }

        return beatmapHashes;
    }

    void write_beatmap_hashes(std::ofstream& stream, const Collection::BeatmapHashes& hashes)
    {
        const auto funcWriteByte = [&stream](auto&& value)
            { core::write<std::int16_t>(stream, static_cast<std::int16_t>((value << 8) | 0x0B)); };

        for (const auto& hash : hashes)
        {
            funcWriteByte(static_cast<std::int8_t>(hash.size()));
            core::write<std::string>(stream, hash);
        }
    }
}