#include	"parse_json.h"


void from_json(const json& j, SUMMONER_INFO& a) {
	try {
		j.at("profileIconId").get_to(a.profileIconId);
		long long account_id;
		j.at("accountId").get_to(account_id);
		a.accountId = to_string(account_id);
		j.at("displayName").get_to(a.displayName);
		j.at("puuid").get_to(a.puuid);
		j.at("summonerLevel").get_to(a.summonerLevel);
	}
	catch (const json::parse_error e) {
		throw e;
	}
}
void from_json(const json& j, RANK_LEVEL& a) {
	try {
		// 使用迭代器遍历整个 std::map
		int	index = 0;
		for (auto it = rank_class_dict.begin(); it != rank_class_dict.end(); ++it, ++index) {
			std::string value = it->first;
			RANK_LEVEL_ITEM	items;

			j.at("queueMap").at(value).at("division").get_to(items.division);
			items.tier = F_rank_tiers.GetSecond(j.at("queueMap").at((value)).at("tier"));

			j.at("queueMap").at(value).at("leaguePoints").get_to(items.leaguePoints);
			items.previousEndTier = F_rank_tiers.GetSecond(j.at("queueMap").at(value).at("previousSeasonEndTier"));

			j.at("queueMap").at(value).at("previousSeasonEndDivision").get_to(items.previousEndDivision);
			items.en = it->first;
			items.cn = it->second;
			a.push_back(items);
		}
	}
	catch (const json::parse_error e) {
		throw e;
	}
}

void	from_json(const json& j, std::vector<TEAM_SUMMONER_INFO>& a) {
	try {
		nlohmann::json myTeamArray = j["myTeam"];

		// 遍历 myTeam 数组中的每个元素
		for (const auto& teamMember : myTeamArray) {
			TEAM_SUMMONER_INFO	info;
			info.summonerId=std::to_string(teamMember["summonerId"].get<LONG64>());
			info.puuid = teamMember["puuid"].get<std::string>();
			info.participantId = a.size()+1;
			a.push_back(info);
		}
	}
	catch (const json::parse_error e) {
		throw e;
	}
}
void from_json(const json& j, PARTICIPANTS& data) {
	data.championId = j.at("championId").get<int>();
	data.participantId = j.at("participantId").get<int>();

	data.assists = j.at("stats").at("assists").get<int>();
	data.champLevel = j.at("stats").at("champLevel").get<int>();
	data.deaths = j.at("stats").at("deaths").get<int>();
	data.totalDamageDealtToChampions = j.at("stats").at("totalDamageDealtToChampions").get<int>();
	data.goldEarned = j.at("stats").at("goldEarned").get<int>();
	data.kills = j.at("stats").at("kills").get<int>();
	data.visionScore = j.at("stats").at("visionScore").get<int>();
	data.win = j.at("stats").at("win").get<bool>();
}

void from_json(const json& j, PLAYER_HISTORY_MATCHDATA& data)
{
	try {
		data.gameCreation = j.at("gameCreation").get<unsigned	long long>();
		data.gameDuration = j.at("gameDuration").get<unsigned	long long>();
		data.gameId = std::to_string(j.at("gameId").get<unsigned	long long>());

		auto gametype = j.at("gameType").get<std::string>();
		auto gamemode = j.at("gameMode").get<std::string>();
		if (GAME_MODE.find(gamemode)!=GAME_MODE.cend())
		{
			data.gameMode = GAME_MODE.at(gamemode);
		}else
			data.gameMode = "未知模式";

		if (GAME_TYPE.find(gametype) != GAME_TYPE.cend())
		{
			if (gametype==GAME_TYPE.at("CUSTOM_GAME"))
			{
				data.gameMode = "自定义";
			}
			if (gametype==GAME_TYPE.at("TUTORIAL_GAME"))
			{
				data.gameMode = "新手教程";
			}
		}
		int queueId = j.at("queueId").get<int>();
		if (queueId == 830)
		{
			data.gameMode = "人机赛";
		}
		

		json arr = j["participants"][0];
		data.summoner_id = std::to_string(j["participantIdentities"][0]["player"].at("summonerId").get<unsigned	long long>());

		data.participants = parse_json<PARTICIPANTS>(arr);
		if (data.gameMode._Equal(GAME_MODE.at("CLASSIC")))
		{
			json	json_team = j["teams"][0];
			if (json_team.contains("bans")) {
				json json_team_ban = json_team["bans"];
				for (const auto c : json_team_ban) {
					if (c.at("championId").get<int>() > 0) {
						data.isRank = true;
						break;
					}
				}
			}
		}
	}
	catch (const json::parse_error e) {
		throw e;
	}
}

void from_json(const json& j, MATCH_DETAILED_DATA& data) {
	try {
		data.gameCreation = j.at("gameCreation").get<unsigned	long long>();
		data.gameDuration = j.at("gameDuration").get<unsigned	long long>();
		data.gameId = std::to_string(j.at("gameId").get<unsigned	long long>());
		auto gamemode = j.at("gameMode").get<std::string>();
		data.gameMode = GAME_MODE.at(gamemode);

		json arr_participant = j["participants"];
		for (int i=0;i< arr_participant.size();i++)
		{
			data.player_datas.push_back(parse_json<PARTICIPANTS>(arr_participant[i]));
		}

		json arr_participantIdentities = j["participantIdentities"];
		for (int i = 0; i < arr_participantIdentities.size(); i++)
		{
			data.player_info.push_back(parse_json<TEAM_SUMMONER_INFO>(arr_participantIdentities[i]));
		}

		json arr_bans = j["teams"]["bans"];

		for (int i = 0; i < arr_bans.size(); i++)
		{
			data.bans.push_back(parse_json<BANS>(arr_bans[i]));
		}

		if (data.gameMode._Equal(GAME_MODE.at("CLASSIC")))
		{
			json	json_team = j["teams"][0];
			if (json_team.contains("bans")) {
				json json_team_ban = json_team["bans"];
				for (const auto c : json_team_ban) {
					if (c.at("championId").get<int>() > 0)
						data.isRank = true;
				}
			}
		}
	}
	catch (const json::parse_error e) {
		throw e;
	}
}

void from_json(const json& j, TEAM_SUMMONER_INFO& data) {
	j.at("participantId").get_to<int>(data.participantId);
	j.at("player").at("puuid").get_to<std::string>(data.puuid);
	j.at("player").at("summonerId").get_to<std::string>(data.summonerId);

	std::string	ss;
	j.at("player").at("summonerName").get_to<std::string>(ss);
	data.displayName = string2wstring(ss);
}

/*
{
	"championId": 245,
	"pickTurn": 1
},
*/
void from_json(const json& j, BANS& ban) {
	j.at("championId").get_to<int>(ban.champid);
	j.at("pickTurn").get_to<int>(ban.pickTurn);
}

void from_json(const json& j, CHAMPION_TOP& top) {
	j.at("championId").get_to<int>(top.championId);
	j.at("championLevel").get_to<int>(top.championLevel);
	j.at("championPoints").get_to<int>(top.championPoints);
	unsigned long long	lasttime;
	j.at("lastPlayTime").get_to<unsigned long long>(lasttime);
	top.lastPlayTime = std::to_string(lasttime);
}