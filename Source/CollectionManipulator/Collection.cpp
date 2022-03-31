#include "Collection.hpp"

#include <fstream>
#include <iostream>

#include "Core/Reader.hpp"

namespace ws::cm
{
	#define SIGNATURE_CHECK(value)												   \
		if ((value) != 0x0B)													   \
		{																		   \
			std::cerr << "Invalid collection signature. Is it corrupted?" << '\n'; \
			return { };															   \
		}

	constexpr auto RELEASE_DATE = 0x1324204;
	constexpr auto MAXIMUM_DATE = 0x5F5BEBF;

	auto read_byte_pair(std::ifstream& collectionStream) -> std::pair<std::int8_t, std::int8_t>;
	auto read_beatmap_hashs(std::ifstream& collectionStream, std::size_t hashCount) -> std::vector<std::string>;

	std::vector<Collection> read_collection(std::ifstream& collectionStream)
	{
		using namespace ws::cm;

		const auto gameVersion = core::read<std::int32_t>(collectionStream);
		if (!(gameVersion >= RELEASE_DATE && 
			  gameVersion <= MAXIMUM_DATE))
		{
			std::cerr << "Invalid collection date time. Is it corrupted?" << '\n'; return { };
		}

		std::vector<Collection> collections { };

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

	void write_collection(const std::vector<Collection>& collectionList)
	{
		throw std::runtime_error("Not yet implemented! :^)");
	}

	std::pair<std::int8_t, std::int8_t> read_byte_pair(std::ifstream& collectionStream)
	{
		const auto value = core::read<std::int16_t>(collectionStream);
		return { (value >> 8), ((value << 12) >> 12) % ((value >> 8) << 8) };
	}

	std::vector<std::string> read_beatmap_hashs(std::ifstream& collectionStream, const std::size_t hashCount)
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
}