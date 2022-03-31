#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

namespace ws::cm
{
	struct Collection
	{
		using BeatmapHashs = std::vector<std::string>;

		std::int32_t gameVersion;
		std::int8_t  nameLength;
		std::string	 name;
		std::int32_t hashCount;

		BeatmapHashs hashs;
	};

	using Collections = std::vector<Collection>;

	auto read_collection(std::ifstream& collectionStream) -> Collections;
	auto write_collection(const Collections& collectionList) -> void;
}

#endif