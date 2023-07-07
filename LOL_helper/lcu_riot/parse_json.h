#pragma	once
#include	"json.hpp"
#include    "struct.h"
#include    "utils.h"



using json = nlohmann::json;


template<typename T>
T parse_json(const std::string& s) {
	json j = json::parse(s);
	T result;
	from_json(j, result);
	return result;
}

std::string convert_rank_tocn(std::string en);

void from_json(const json& j, SUMMONER_INFO& a);
void from_json(const json& j, RANK_LEVEL& a);


