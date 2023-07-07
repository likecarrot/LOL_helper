#include	"parse_json.h"


std::string convert_rank_tocn(std::string en) {
	std::string	ret;
	if (rank_tiers_dict.count(en)>0)
	{
		return	rank_tiers_dict.at(en);
	}
	//if (tft_rank_tiers.count(en)>0)
	//{
	//	return	tft_rank_tiers.at(en);
	//}
	return	"";
}
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
		for (auto it = rank_class_dict.begin(); it != rank_class_dict.end(); ++it,++index) {
			std::string value = it->first;
			RANK_LEVEL_ITEM	items;

			j.at("queueMap").at(value).at("division").get_to(items.division);
			items.tier = convert_rank_tocn(j.at("queueMap").at((value)).at("tier"));
			j.at("queueMap").at(value).at("leaguePoints").get_to(items.leaguePoints);
			items.previousEndTier = convert_rank_tocn(j.at("queueMap").at(value).at("previousSeasonEndTier"));
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

