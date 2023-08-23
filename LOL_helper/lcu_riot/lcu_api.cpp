#include	"lcu_api.h"
#include <ShlObj.h>


void	raw_lcu_api::set_game_commandline(std::string commandline) {

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
}


bool	raw_lcu_api::init() {
	LCU_REQUEST::setInstance(domain.auth_token, domain.port);

	OutputDebugStringA(domain.auth_token.c_str());
	OutputDebugStringA("\n");
	OutputDebugStringA(domain.port.c_str());
	OutputDebugStringA("\n");

	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, current_client_player_summoner_info_api);

	if (response.length() < 1)
	{
		return	false;
	}
	try
	{
		LCU_JSON_RESPONSE::LolSummonerInfo data = nlohmann::json::parse(response);
	}
	catch (const std::exception&)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
		return	false;
	}
	return	true;
}

GAME_STATUS		raw_lcu_api::get_game_status() {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, client_get_player_status_api);
	auto	status = get_enum_gamestatus(response);
	return	status;
}

void	raw_lcu_api::accept_game() {
	OutputDebugStringA("接受对局\n");
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, client_accept_matching_api, "", "");
}

void raw_lcu_api::decline_game()
{
	OutputDebugStringA("拒绝对局\n");
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, client_decline_matching_api, "", "");
}

void	raw_lcu_api::auto_next_game() {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, "/lol-lobby/v2/play-again", "", "");
}

void	raw_lcu_api::search_queue() {
	// /lol-lobby/v2/lobby/matchmaking/search 开始搜索队伍
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, "/lol-lobby/v2/lobby/matchmaking/search", "", "");
#ifdef DEBUG
	std::cout << response << std::endl;
#endif // DEBUG
}

std::string		raw_lcu_api::lock_champions(int	champion_id,bool completed) {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, matching_get_myteam_summonerinfo_api);
	int	localPlayerCellId = 0;
	try
	{
		json	js = json::parse(response);
		json	data;
		data["championId"] = champion_id;
		data["completed"] = completed;
		if (js.contains("localPlayerCellId"))
		{
			localPlayerCellId = js["localPlayerCellId"].get<int>();
			response = request.request(LCU_REQUEST::RequestMethod::PATCH_METHOD, lock_champ_completed_api, std::to_string(localPlayerCellId), data.dump());
		}
	}
	catch (const std::exception&)
	{
		return	std::string(__FUNCTION__);
	}
	return	response;
}

bool raw_lcu_api::bench_champiosn(int champion_id)
{
	LCU_REQUEST& request = LCU_REQUEST::getInstance();
	std::string	url = aram_swap_champ_api + std::to_string(champion_id);
	std::string response=request.request(LCU_REQUEST::RequestMethod::POST_METHOD, url);
	if (response.empty())
	{
		return	true;
	}
	return	false;
}

//2023-8-16重构

LCU_JSON_RESPONSE::LolSummonerInfo raw_lcu_api::get_summoner_datas(std::string summoner_id) {
	LCU_JSON_RESPONSE::LolSummonerInfo	summoner;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response; 
	response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, summonerid_get_summonerinfo_api + summoner_id);

	try {
		summoner = nlohmann::json::parse(response);
	}
	catch (const std::exception& e) {
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
	}
	return	summoner;
}
LCU_JSON_RESPONSE::LolSummonerInfo raw_lcu_api::get_summoner_datas() {
	LCU_JSON_RESPONSE::LolSummonerInfo	summoner;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response;
	response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, current_client_player_summoner_info_api);

	try {
		summoner = nlohmann::json::parse(response);
		this->my_summoner = summoner;
	}
	catch (const std::exception& e) {
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
	}
	return	summoner;
}


LCU_JSON_RESPONSE::LolChampSelect raw_lcu_api::get_lol_champ_select_v1_session()
{
	LCU_JSON_RESPONSE::LolChampSelect	lobby;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, matching_get_myteam_summonerinfo_api);
	
	try
	{
		lobby = nlohmann::json::parse(response);
	}
	catch (const std::exception&)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
	}
	return	lobby;
}

LCU_JSON_RESPONSE::LolRankedData raw_lcu_api::puuid_get_rank_datas(std::string puuid)
{
	LCU_JSON_RESPONSE::LolRankedData	ret_datas;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response; 
	if (puuid.empty())
		response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, puuid_get_player_rank_data_api, this->my_summoner.puuid);
	else
		response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, puuid_get_player_rank_data_api, puuid);
	
	try
	{
		ret_datas = nlohmann::json::parse(response);
	}
	catch (const std::exception&)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
	}
	return ret_datas;
}

LCU_JSON_RESPONSE::LolMatchHistory	raw_lcu_api::getHistoryMatchDatas(std::string puuid, std::string accountid, int min, int max) {
	std::string puuid_get_player_history_data_api1 = "/lol-match-history/v1/products/lol/" + puuid + "/matches?begIndex=" + std::to_string(min) + "&endIndex=" + std::to_string(max);
	LCU_JSON_RESPONSE::LolMatchHistory	ret_datas;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, puuid_get_player_history_data_api1);
	try
	{
		ret_datas = nlohmann::json::parse(response);
	}
	catch (const std::exception&)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
	}
	return	ret_datas;
}

LCU_JSON_RESPONSE::LolCollections	raw_lcu_api::get_top_champions(std::string	accountid, int limit) {
	// /lol-collections/v1/inventories/4007627523/champion-mastery/top?limit=6
	std::string	url = summonerid_get_top_champion_api + accountid + "/champion-mastery/top?limit=" + std::to_string(limit);
	LCU_JSON_RESPONSE::LolCollections	ret_data;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, url);
	try
	{
		ret_data = nlohmann::json::parse(response);
	}
	catch (const std::exception&)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
	}
	return ret_data;
}

LCU_JSON_RESPONSE::LolOwnerChampions raw_lcu_api::get_owner_champions()
{
	LCU_JSON_RESPONSE::LolOwnerChampions	owner;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, get_owner_champions_api);
	try
	{
		owner = nlohmann::json::parse(response);
	}
	catch (const std::exception&)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
	}
	return	owner;
}

LCU_JSON_RESPONSE::LolGameDataChampSummary raw_lcu_api::get_all_champions()
{
	LCU_JSON_RESPONSE::LolGameDataChampSummary	champs;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, get_all_champions_api);
	try
	{
		champs = nlohmann::json::parse(response);
	}
	catch (const std::exception&)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
	}
	return	champs;
}


LCU_JSON_RESPONSE::LolGameflow raw_lcu_api::get_current_game_mode()
{
	LCU_JSON_RESPONSE::LolGameflow ret_datas;
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, get_current_game_mode_api);
	try
	{
		ret_datas = nlohmann::json::parse(response);
	}
	catch (const std::exception&)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringW(L"\n");
	}
	return	 ret_datas;
}

void raw_lcu_api::ReRoll()
{
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, use_reroller_api);
}

