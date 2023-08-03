#include	"lol_helper.h"
#include <ShlObj.h>


void	helper::set_game_commandline(std::string commandline) {

		// 截取auth_token
		size_t auth_token_pos = commandline.find("--remoting-auth-token=");
		if (auth_token_pos != std::string::npos) {
			auth_token_pos += strlen("--remoting-auth-token=");
			size_t auth_token_end = commandline.find("\"", auth_token_pos);
			if (auth_token_end != std::string::npos) {
				domain.auth_token = commandline.substr(auth_token_pos, auth_token_end - auth_token_pos);
			}
		}

		// 截取port
		size_t port_pos = commandline.find("--app-port=");
		if (port_pos != std::string::npos) {
			port_pos += strlen("--app-port=");
			size_t port_end = commandline.find("\"", port_pos);
			if (port_end != std::string::npos) {
				domain.port = commandline.substr(port_pos, port_end - port_pos);
			}
		}

#ifdef DEBUG
		std::cout << "-----------" << std::endl;
		std::cout << domain.port << std::endl;
		std::cout << domain.auth_token << std::endl;
		std::cout << "-----------" << std::endl;
#endif // DEBUG
}


bool	helper::init() {
	LCU_REQUEST::setInstance(domain.auth_token, domain.port);

	OutputDebugStringA(domain.auth_token.c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA(domain.port.c_str());
	OutputDebugStringA("\n");

	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, current_client_player_summoner_info_api);
	if (response.length()<1)
	{
		return	false;
	}
	try {
		my_summoner = parse_json<SUMMONER_INFO>(response);
	}
	catch (const std::exception& e) {
		return	false;
	}
	return	true;
}
SUMMONER_INFO helper::get_self_summoner_datas() {
	SUMMONER_INFO	summoner;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, current_client_player_summoner_info_api);

	try {
		summoner = parse_json<SUMMONER_INFO>(response);
	}
	catch (const std::exception& e) {
	}
	return	summoner;
}
RANK_LEVEL helper::get_self_Rank_level() {	//获取排位分数
	RANK_LEVEL	rank_level;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, puuid_get_player_rank_data_api, this->my_summoner.puuid);
	try {
		rank_level = parse_json<RANK_LEVEL>(response);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	return	rank_level;
}


GAME_STATUS		helper::get_game_status() {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, client_get_player_status_api);
	auto	status = get_enum_gamestatus(response);
#ifdef DEBUG
	std::cout << "游戏状态: " << game_status_dict[static_cast<int>(status)][0] << std::endl;
#endif // DEBUG
	return	status;
}
void	helper::accept_game() {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, client_accept_matching_api, "", "");
#ifdef DEBUG
	std::cout << response << std::endl;
#endif // DEBUG
}

void	helper::auto_next_game() {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, "/lol-lobby/v2/play-again", "", "");
#ifdef DEBUG
	std::cout << response << std::endl;
#endif // DEBUG
}

void	helper::search_queue() {
	// /lol-lobby/v2/lobby/matchmaking/search 开始搜索队伍
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, "/lol-lobby/v2/lobby/matchmaking/search", "", "");
#ifdef DEBUG
	std::cout << response << std::endl;
#endif // DEBUG
}

std::vector<CHAMPION> helper::get_owner_champions() {
	std::vector<CHAMPION> ret_datas;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, "/lol-champions/v1/owned-champions-minimal");
	try
	{
		json	js = json::parse(response);
		for (const auto& role : js) {
			CHAMPION	ch = { 0 };
			ch.name = wstring2string(string2wstring(role["name"].get<std::string>()));
			ch.title = wstring2string(string2wstring(role["title"].get<std::string>()));
			ch.freePlay = role["freeToPlay"].get<bool>();
			ch.selectid = role["id"].get<int>();
			ret_datas.push_back(ch);
		}
	}
	catch (const std::exception&)
	{
	}
	return	ret_datas;
}	//获取已经拥有的英雄


std::vector<CHAMPION> helper::get_all_champions() {
	std::vector<CHAMPION> ret_datas;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, "/lol-game-data/assets/v1/champion-summary.json");
	try
	{
		json	js = json::parse(response);
		for (const auto& role : js) {
			CHAMPION	ch;
			ch.selectid = role["id"].get<int>();
			ch.name = wstring2string(string2wstring(role["name"].get<std::string>()));
			if (ch.selectid > 0)
			{
				ret_datas.push_back(ch);
			}
		}
	}
	catch (const std::exception&)
	{
	}
	return	ret_datas;
}	//获取所有英雄
void	helper::lock_champions(int	champion_id) {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, matching_get_myteam_summonerinfo_api);
	int	localPlayerCellId = 0;
	try
	{
		json	js = json::parse(response);
		json	data;
		data["championId"] = champion_id;
		data["completed"] = true;
		if (js.contains("localPlayerCellId"))
		{
			for (const auto& ch : owner_champions) {
				if (ch.selectid == champion_id)
				{
					localPlayerCellId = js["localPlayerCellId"].get<int>();
					response = request.request(LCU_REQUEST::RequestMethod::PATCH_METHOD, "/lol-champ-select/v1/session/actions/", std::to_string(localPlayerCellId), data.dump());
					break;
				}
			}
		}
	}
	catch (const std::exception&)
	{
	}
}

std::vector<TEAM_SUMMONER_INFO> helper::getChatRoomPlayerIdList()
{
	std::vector<TEAM_SUMMONER_INFO>	ret_datas;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, matching_get_myteam_summonerinfo_api);
	
	
	/*TEAM_SUMMONER_INFO	a,b,c,d,e;
	a.summonerId = "4007627523";
	a.puuid = "445a397a-5f8d-5a2c-b81c-0eeab67bad2f";
	a.participantId = 1;
	b.summonerId = "2952329458";
	b.puuid = "a1bf4773-1eba-53fa-99f2-3c41d559f583";
	b.participantId = 2;*/
	/*c.summonerId = "3288707299378848";
	c.puuid = "4fe6cd8b-6740-5d68-ab48-50c4a2759d26";
	c.participantId = 3;
	d.summonerId = "3293269878400480";
	d.puuid = "516f8db6-4e29-58a1-a12b-451ff20e1d34";
	d.participantId = 4;
	e.summonerId = "4007627523";
	e.puuid = "445a397a-5f8d-5a2c-b81c-0eeab67bad2f";
	e.participantId = 5;*/
	/*ret_datas.push_back(a);
	ret_datas.push_back(b);*/
	//ret_datas.push_back(c);
	//ret_datas.push_back(d);
	//ret_datas.push_back(e);

	try
	{
		ret_datas = parse_json<std::vector<TEAM_SUMMONER_INFO>>(response);
		OutputDebugStringA(("matching_get_myteam_summonerinfo_api"+std::to_string(ret_datas.size())+"\n").c_str());
	}
	catch (const std::exception&)
	{
	}
	return	ret_datas;
}

std::string	helper::getChatRoomId() {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, matching_get_myteam_summonerinfo_api);
		// 查找 id 字段的起始位置
		size_t idStart = response.find("\"id\":\"");
		if (idStart == std::string::npos) {
			std::cout << "id字段未找到" << std::endl;
			return std::string();
		}

		// 截取 id 字段的值
		size_t idValueStart = idStart + 6; // 跳过 "\"id\":\"" 的长度
		size_t idValueEnd = response.find("\"", idValueStart);
		if (idValueEnd == std::string::npos) {
			return std::string();
		}
		std::string idValue = response.substr(idValueStart, idValueEnd - idValueStart);
	return	idValue;
}
std::wstring helper::getDisplayName(std::string accountid)
{
	SUMMONER_INFO	ret_datas;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, summonerid_get_summonerinfo_api, accountid);
	int	localPlayerCellId = 0;
	try
	{
		ret_datas = parse_json<SUMMONER_INFO>(response);
	}
	catch (const std::exception&)
	{
	}
	return	string2wstring(ret_datas.displayName);
}

RANK_LEVEL helper::puuid_get_rank_datas(std::string puuid)
{
	RANK_LEVEL	ret_datas;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, puuid_get_player_rank_data_api, puuid);
	try
	{
		ret_datas = parse_json<RANK_LEVEL>(response);
	}
	catch (const std::exception&)
	{
	}
	return ret_datas;
}
std::vector<PLAYER_HISTORY_MATCHDATA>	helper::getHistoryMatchDatas(std::string puuid, std::string accountid, int min, int max) {

	std::string puuid_get_player_history_data_api1 = "/lol-match-history/v1/products/lol/" + puuid + "/matches?begIndex=" + std::to_string(min) + "&endIndex=" + std::to_string(max);
	std::vector<PLAYER_HISTORY_MATCHDATA>	ret_datas;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, puuid_get_player_history_data_api1);
	try
	{
		json	j = json::parse(response);
		if (j.at("games").at("games").is_array())
		{
			for (const auto& game : j["games"]["games"]) {
				ret_datas.push_back(parse_json<PLAYER_HISTORY_MATCHDATA>(game));
			}
		}
	}
	catch (const std::exception&)
	{
	}
	return	ret_datas;
}
MATCH_DETAILED_DATA helper::getGameNoticeInfo(std::string gameid)
{
	MATCH_DETAILED_DATA	ret_data;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, gameid_get_detailed_data_api, gameid);
	try
	{
		json	j = json::parse(response);
		ret_data = parse_json<MATCH_DETAILED_DATA>(j);
	}
	catch (const std::exception&)
	{
	}
	return ret_data;
}

std::vector<CHAMPION_TOP>	helper::get_top_champions(std::string	accountid, int limit) {
	// /lol-collections/v1/inventories/4007627523/champion-mastery/top?limit=6
	std::string	url = summonerid_get_top_champion_api + accountid + "/champion-mastery/top?limit=" + std::to_string(limit);
	std::vector < CHAMPION_TOP>	ret_data;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, url);
	try
	{
		json	j = json::parse(response);
		if (j.contains("masteries") && j.at("masteries").is_array()) {
			for(int i=0;i<j.at("masteries").size();i++)
				ret_data .push_back(parse_json<CHAMPION_TOP>(j.at("masteries")[i]));
		}
	}
	catch (const std::exception&)
	{
	}
	return ret_data;
}