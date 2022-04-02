#include "Collection.hpp"

#include <fstream>
#include <iostream>

#include "Core/Reader.hpp"
#include "Core/Writer.hpp"
#include "Logging.hpp"

namespace kaede::api
{
	#define SIGNATURE_CHECK(value)												      \
		if ((value) != 0x0B)													      \
		{																			  \
			KAEDE_ERRO("Invalid collection signature. Is it corrupted?"); return { }; \
		}

	#define PROTECTED_BYTE(value) std::pair{ (value), 0x0B }  

	auto read_byte_pair(std::ifstream& collectionStream) -> std::pair<std::int8_t, std::int8_t>;
	auto read_beatmap_hashs(std::ifstream& collectionStream, std::size_t hashCount) -> std::vector<std::string>;
	auto write_byte_pair(std::ofstream& collectionStream, const std::pair<std::int8_t, std::int8_t> value) -> void;
	auto write_beatmap_hashs(std::ofstream& collectionStream, const kaede::api::Collection::BeatmapHashs& hashs) -> void;

	constexpr auto RELEASE_DATE	   = 0x1324204;
	constexpr auto MAXIMUM_DATE	   = 0x5F5BEBF;
	constexpr auto MD5_HASH_LENGTH = 0x20;

	auto read_collection(std::ifstream& collectionStream) -> Collections
	{
		const auto gameVersion = core::read<std::int32_t>(collectionStream);
		if (!(gameVersion >= RELEASE_DATE && 
			  gameVersion <= MAXIMUM_DATE))
		{
			KAEDE_ERRO("Invalid collection date time. Is it corrupted?"); return { };
		}

		Collections collections { };

		collections.reserve(core::read<std::int32_t>(collectionStream));

		for (auto dummyPos = 0; dummyPos < collections.capacity(); ++dummyPos)
		{
			const auto [nameLength, flagValue] = read_byte_pair(collectionStream);

			SIGNATURE_CHECK(flagValue);

			Collection collection { };
			collection.gameVersion = gameVersion;
			collection.nameLength  = nameLength;
			collection.name		   = core::read<std::string>(collectionStream, collection.nameLength);
			collection.hashCount   = core::read<std::int32_t>(collectionStream);

			collection.hashs = read_beatmap_hashs(collectionStream, collection.hashCount);

			collections.emplace_back(collection);
		}

		return collections;
	}
	
	auto write_collection(const Collections& collections, std::ofstream& collectionStream) -> void
	{
		core::write<std::int32_t>(collectionStream, collections.front().gameVersion);
		core::write<std::int32_t>(collectionStream, static_cast<std::int32_t>(collections.size()));

		for (const auto& collection : collections)
		{
			write_byte_pair(collectionStream, PROTECTED_BYTE(collection.nameLength));
			core::write<std::string>(collectionStream, collection.name);

			core::write<std::int32_t>(collectionStream, collection.hashCount);

			write_beatmap_hashs(collectionStream, collection.hashs);
		}
	}
	
	auto read_byte_pair(std::ifstream& collectionStream) -> std::pair<std::int8_t, std::int8_t>
	{
		const auto value = core::read<std::int16_t>(collectionStream);
		return { (value >> 8), ((value << 12) >> 12) % ((value >> 8) << 8) };
	}
	
	auto write_byte_pair(std::ofstream& collectionStream, const std::pair<std::int8_t, std::int8_t> value) -> void
	{
		core::write<std::int16_t>(collectionStream, static_cast<std::int16_t>((value.first << 8) | value.second));
	}

	auto read_beatmap_hashs(std::ifstream& collectionStream, const std::size_t hashCount) -> std::vector<std::string>
	{
		std::vector<std::string> beatmapHashs { };

		for (auto dummyPos = 0; dummyPos < hashCount; ++dummyPos)
		{
			const auto [nameLength, flagValue] = read_byte_pair(collectionStream);
			SIGNATURE_CHECK(flagValue);
			beatmapHashs.emplace_back(core::read<std::string>(collectionStream, nameLength));
		}

		return beatmapHashs;
	}

	auto write_beatmap_hashs(std::ofstream& collectionStream, const Collection::BeatmapHashs& hashs) -> void
	{
		for (const auto& hash : hashs)
		{
			write_byte_pair(collectionStream, PROTECTED_BYTE(MD5_HASH_LENGTH));
			core::write<std::string>(collectionStream, hash);
		}
	}
}