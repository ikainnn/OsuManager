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

	auto read_collection(std::ifstream& collectionStream) -> std::vector<Collection>;
	auto write_collection(const std::vector<Collection>& collectionList) -> void;
}

#endif