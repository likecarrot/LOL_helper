#pragma	once
#include	"json.hpp"
#include    "struct.h"
#include    "utils.h"



using json = nlohmann::json;


template<typename T>
T parse_json(const std::string& s) {
	T result;
	try
	{
		json j = json::parse(s);
		from_json(j, result);
	}
	catch (const std::exception&)
	{
	}
	return result;
}
template<typename T>
T parse_json(const json& j) {
	T result;
	from_json(j, result);
	return result;
}

std::string convert_rank_tocn(std::string en);

void from_json(const json& j, SUMMONER_INFO& a);
void from_json(const json& j, RANK_LEVEL& a);
void from_json(const json& j, std::vector<TEAM_SUMMONER_INFO>& a);
void from_json(const json& j, PARTICIPANTS &data);
void from_json(const json&j, PLAYER_HISTORY_MATCHDATA &data);
void from_json(const json& j, MATCH_DETAILED_DATA& data);
void from_json(const json& j, TEAM_SUMMONER_INFO& data);
void from_json(const json& j, BANS& ban);
void from_json(const json& j, CHAMPION_TOP& top);