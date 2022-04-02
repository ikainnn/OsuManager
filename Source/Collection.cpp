#include "Collection.hpp"

#include <fstream>
#include <iostream>

#include "Core/Reader.hpp"
#include "Core/Writer.hpp"
#include "Common/Logging.hpp"

namespace kaede::api
{
	#define PROTECTION_CHECK(value)												      \
		if ((value) != 0x0B)													      \
		{																			  \
			KAEDE_ERRO("Invalid collection signature. Is it corrupted?"); return { }; \
		}

	auto read_protected_byte(std::ifstream& collectionStream) -> std::pair<std::int8_t, std::int8_t>;
	auto read_beatmap_hashs(std::ifstream& collectionStream, std::size_t hashCount) -> std::vector<std::string>;
	auto write_protected_byte(std::ofstream& collectionStream, std::int8_t value) -> void;
	auto write_beatmap_hashs(std::ofstream& collectionStream, const kaede::api::Collection::BeatmapHashs& hashs) -> void;

	constexpr auto RELEASE_DATE	= 0x1324204;
	constexpr auto MAXIMUM_DATE	= 0x5F5BEBF;

	auto read_collection(std::ifstream& collectionStream) -> Collections
	{
		const auto gameVersion = core::read<std::int32_t>(collectionStream);
		if (!(gameVersion >= RELEASE_DATE && 
			  gameVersion <= MAXIMUM_DATE))
		{
			KAEDE_ERRO("Invalid collection date time. Is it corrupted?"); return { };
		}

		std::vector<Collection> collections { core::read<std::int32_t>(collectionStream) };

		for (auto dummyPos = 0; dummyPos < collections.capacity(); ++dummyPos)
		{
			const auto [nameLength, protectionValue] = read_protected_byte(collectionStream);

			PROTECTION_CHECK(protectionValue);

			Collection collection { };
			collection.nameLength  = nameLength;
			collection.name		   = core::read<std::string>(collectionStream, collection.nameLength);
			collection.hashCount   = core::read<std::int32_t>(collectionStream);

			collection.hashs = read_beatmap_hashs(collectionStream, collection.hashCount);

			collections.emplace_back(collection);
		}

		return { gameVersion, collections };
	}
	
	auto write_collection(std::ofstream& collectionStream, const Collections& collections) -> void
	{
		const auto& [_gameVersion, _collections] = collections;

		core::write<std::int32_t>(collectionStream, _gameVersion);
		core::write<std::int32_t>(collectionStream, static_cast<std::int32_t>(_collections.size()));

		for (const auto& collection : _collections)
		{
			write_protected_byte(collectionStream, collection.nameLength);
			core::write<std::string>(collectionStream, collection.name);

			core::write<std::int32_t>(collectionStream, collection.hashCount);

			write_beatmap_hashs(collectionStream, collection.hashs);
		}
	}
	
	auto read_protected_byte(std::ifstream& collectionStream) -> std::pair<std::int8_t, std::int8_t>
	{
		const auto value = core::read<std::int16_t>(collectionStream);
		return { (value >> 8), ((value << 12) >> 12) % ((value >> 8) << 8) };
	}
	
	auto write_protected_byte(std::ofstream& collectionStream, std::int8_t value) -> void
	{
		core::write<std::int16_t>(collectionStream, static_cast<std::int16_t>((value << 8) | 0x0B));
	}
	
	auto read_beatmap_hashs(std::ifstream& collectionStream, const std::size_t hashCount) -> Collection::BeatmapHashs
	{
		Collection::BeatmapHashs beatmapHashs { };

		for (auto dummyPos = 0; dummyPos < hashCount; ++dummyPos)
		{
			const auto [nameLength, flagValue] = read_protected_byte(collectionStream);

			PROTECTION_CHECK(flagValue);

			beatmapHashs.emplace_back(core::read<std::string>(collectionStream, nameLength));
		}

		return beatmapHashs;
	}
	
	auto write_beatmap_hashs(std::ofstream& collectionStream, const Collection::BeatmapHashs& hashs) -> void
	{
		for (const auto& hash : hashs)
		{
			write_protected_byte(collectionStream, static_cast<std::int8_t>(hash.size()));
			core::write<std::string>(collectionStream, hash);
		}
	}
}