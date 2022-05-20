
#include "../Include/KaedeManager/Collection.hpp"

#include "../Include/KaedeManager/File/Reader.hpp"
#include "../Include/KaedeManager/File/Writer.hpp"
#include "../Include/Logging.hpp"

namespace kaede::api
{
    #define PROTECTION_CHECK(value)                                                              \
        if ((value) != 0x0B)                                                                     \
        {                                                                                        \
            kaede::logging::error("Invalid collection signature. Is it corrupted?"); return { }; \
        }

    auto read_protected_byte(std::ifstream& stream) -> std::pair<std::int8_t, std::int8_t>;
    auto read_beatmap_hashes(std::ifstream& stream, std::size_t count) -> std::vector<std::string>;
    void write_protected_byte(std::ofstream& stream, std::int8_t value);
    void write_beatmap_hashes(std::ofstream& stream, const kaede::api::Collection::BeatmapHashes& hashs);

    auto read_collection(std::ifstream& stream) -> Collections
    {
        constexpr auto RELEASE_DATE = 0x1324204;
        constexpr auto MAXIMUM_DATE = 0x5F5BEBF;
        
        const auto gameVersion = core::read<std::int32_t>(stream);
        if (!(gameVersion >= RELEASE_DATE && gameVersion <= MAXIMUM_DATE))
        {
            kaede::logging::error("Invalid collection date time. Is it corrupted?"); return { };
        }

        std::vector<Collection> collections ( core::read<std::int32_t>(stream) );

        for (auto& collection : collections)
        {
            const auto [nameLength, protectionValue] = read_protected_byte(stream);

            PROTECTION_CHECK(protectionValue);

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

    void write_collection(std::ofstream& stream, const Collections& collections)
    {
        const auto& [gameVersion, _collections] = collections;

        core::write<std::int32_t>(stream, gameVersion);
        core::write<std::int32_t>(stream, static_cast<std::int32_t>(_collections.size()));

        for (const auto& collection : _collections)
        {
            write_protected_byte(stream, collection.nameLength);
            core::write<std::string>(stream, collection.name);
            core::write<std::int32_t>(stream, collection.hashCount);

            write_beatmap_hashes(stream, collection.hashes);
        }
    }

    auto read_protected_byte(std::ifstream& stream) -> std::pair<std::int8_t, std::int8_t>
    {
        const auto value = core::read<std::int16_t>(stream);
        return { (value >> 8), ((value << 12) >> 12) % ((value >> 8) << 8) };
    }

    auto read_beatmap_hashes(std::ifstream& stream, const std::size_t count) -> Collection::BeatmapHashes
    {
        Collection::BeatmapHashes beatmapHashes ( count );

        for (auto& beatmapHash : beatmapHashes)
        {
            const auto [nameLength, flagValue] = read_protected_byte(stream);
            PROTECTION_CHECK(flagValue);
            beatmapHash = core::read<std::string>(stream, nameLength);
        }

        return beatmapHashes;
    }

    void write_beatmap_hashes(std::ofstream& stream, const Collection::BeatmapHashes& hashs)
    {
        for (const auto& hash : hashs)
        {
            write_protected_byte(stream, static_cast<std::int8_t>(hash.size()));
            core::write<std::string>(stream, hash);
        }
    }

    void write_protected_byte(std::ofstream& stream, const std::int8_t value)
    {
        // FIXME: why do i need to cast this thing? cant exactly remember 
        // but maybe there's no need for this.
        core::write<std::int16_t>(stream, static_cast<std::int16_t>((value << 8) | 0x0B));
    }
}