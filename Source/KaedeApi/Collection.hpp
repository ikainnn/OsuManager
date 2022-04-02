#ifndef COLLECTION_HPP
#define COLLECTION_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <fstream>

namespace kaede::api
{
	struct Collection
	{
		using BeatmapHashs = std::vector<std::string>;

		std::int8_t  nameLength;
		std::string	 name;
		std::int32_t hashCount;

		BeatmapHashs hashs;
	};

	using Collections = std::pair<std::int32_t, std::vector<Collection>>;

	auto read_collection(std::ifstream& collectionStream) -> Collections;
	auto write_collection(std::ofstream& collectionStream, const Collections& collections) -> void;
}

#endif