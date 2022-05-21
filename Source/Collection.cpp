
#include "../Include/OsuManager/Collection.hpp"

#include "../Include/OsuManager/File/Reader.hpp"
#include "../Include/OsuManager/File/Writer.hpp"
#include "../Include/Logging.hpp"

#define OSU_MAN_ASSERT(condition, msg) if (!(condition)) osu_manager::logging::error(msg)

namespace osu_manager::api
{
    auto _read_beatmap_hashes(std::ifstream& _stream, std::size_t _count) -> std::vector<std::string>;
    void _write_beatmap_hashes(std::ofstream& _stream, const Collection::BeatmapHashes& _hashes);

    auto read_collection_from_stream(std::ifstream& _stream) -> Collections
    {
        constexpr auto RELEASE_DATE = 0x1324204, MAXIMUM_DATE = 0x5F5BEBF;

        const auto gameVersion = core::read<std::int32_t>(_stream);
        OSU_MAN_ASSERT((gameVersion >= RELEASE_DATE && gameVersion <= MAXIMUM_DATE), "Invalid collection date time. Is it corrupted?");

        const auto funcReadBytes = [] (auto&& value) -> std::pair<std::int8_t, std::uint8_t>
            { return { (value >> 8), ((value << 12) >> 12) }; };
        std::vector<Collection> collections ( core::read<std::int32_t>(_stream) );

        for (auto& collection : collections)
        {
            const auto [nameLength, sentinel] = funcReadBytes(core::read<std::int16_t>(_stream));
            OSU_MAN_ASSERT(sentinel == 0x0B, "Invalid collection signature. Is it corrupted?");

            collection =
            {
                .name       = core::read<std::string>(_stream, nameLength),
                .nameLength = nameLength,
                .hashCount  = core::read<std::int32_t>(_stream),
                .hashes     = _read_beatmap_hashes(_stream, collection.hashCount)
            };
        }

        return { gameVersion, collections };
    }

    void write_collection_to_stream(std::ofstream& _stream, const Collections& _collections)
    {
        const auto& [gameVersion, collections] = _collections;

        core::write<std::int32_t>(_stream, gameVersion);
        core::write<std::int32_t>(_stream, static_cast<std::int32_t>(collections.size()));

        const auto funcWriteByte = [&_stream](auto&& value)
            { core::write<std::int16_t>(_stream, static_cast<std::int16_t>((value << 8) | 0x0B)); };

        for (const auto& collection : collections)
        {
            funcWriteByte(collection.nameLength);
            core::write<std::string>(_stream, collection.name);
            core::write<std::int32_t>(_stream, collection.hashCount);

            _write_beatmap_hashes(_stream, collection.hashes);
        }
    }

    auto _read_beatmap_hashes(std::ifstream& _stream, const std::size_t _count) -> Collection::BeatmapHashes
    {
        Collection::BeatmapHashes beatmapHashes ( _count );

        const auto funcReadBytes = [] (auto&& value) -> std::pair<std::int8_t, std::uint8_t>
            { return { (value >> 8), ((value << 12) >> 12) }; };

        for (auto& beatmapHash : beatmapHashes)
        {
            const auto [nameLength, sentinel] = funcReadBytes(core::read<std::int16_t>(_stream));
            OSU_MAN_ASSERT(sentinel == 0x0B, "Invalid collection signature. Is it corrupted?");

            beatmapHash = core::read<std::string>(_stream, nameLength);
        }

        return beatmapHashes;
    }

    void _write_beatmap_hashes(std::ofstream& _stream, const Collection::BeatmapHashes& _hashes)
    {
        const auto funcWriteByte = [&_stream](auto&& value)
            { core::write<std::int16_t>(_stream, static_cast<std::int16_t>((value << 8) | 0x0B)); };

        for (const auto& hash : _hashes)
        {
            funcWriteByte(static_cast<std::int8_t>(hash.size()));
            core::write<std::string>(_stream, hash);
        }
    }
}