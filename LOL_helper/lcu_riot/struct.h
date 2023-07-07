#pragma	once
#include	<string>
#include    <map>
#include	<vector>

//访问api时所需要的必备信息
struct DOMAIN_INFO
{
	std::string	domain = "127.0.0.1";
	std::string	port;
	std::string	auth_token;
};

//lol-summoner/v1/current-summoner
struct SUMMONER_INFO
{
	int profileIconId;      //icon id
	std::string accountId;
	std::string displayName;
	std::string puuid;
	int summonerLevel;
	void	clear() {
		this->profileIconId = 0;
		this->accountId = "";
		this->displayName = "";
		this->puuid = "";
		this->summonerLevel = 0;
	}
};

//requestUtil.doGet("/lol-ranked/v1/ranked-stats/" + puuid);
struct RANK_LEVEL_ITEM
{
	std::string tier;       //段位分层
	std::string division;   //段位几 比如 黄金4 这里就是 VI
	int leaguePoints;       //胜点

	std::string previousEndTier;       //上赛季结束段位分层 黄金 
	std::string previousEndDivision;   //上赛季结束段位
	std::string	en;						//排位类型
	std::string	cn;						//排位类型中文
};
enum class RANK_LEVEL_INDEX
{
	RANKED_SOLO_5x5,
	RANKED_FLEX_SR,
	RANKED_TFT,
	RANKED_TFT_TURBO,
	RANKED_TFT_DOUBLE_UP
};
/*RANK_LEVEL_ITEM RANKED_SOLO_5x5;
RANK_LEVEL_ITEM RANKED_FLEX_SR;
RANK_LEVEL_ITEM RANKED_TFT;
RANK_LEVEL_ITEM RANKED_TFT_TURBO;
RANK_LEVEL_ITEM RANKED_TFT_DOUBLE_UP;*/
using RANK_LEVEL = std::vector<RANK_LEVEL_ITEM>;

static const std::map<std::string, std::string>   rank_class_dict = {
	{"RANKED_SOLO_5x5","单排/双排"},
	{"RANKED_FLEX_SR","灵活组排 5v5"},
	{"RANKED_TFT","云顶之弈"},
	{"RANKED_TFT_TURBO","云顶之弈狂暴模式"},
	{"RANKED_TFT_DOUBLE_UP","云顶之弈双人作战"}
}; //排位等级的模式
static const std::map<std::string, std::string>   rank_tiers_dict = {
	{"CHALLENGER","最强王者"},
	{"GRANDMASTER","傲世宗师"},
	{"MASTER","超凡大师"},
	{"IRON","坚韧黑铁"},
	{"DIAMOND","璀璨钻石"},
	{"PLATINUM","华贵铂金"},
	{"GOLD","荣耀黄金"},
	{"SILVER","不屈白银"},
	{"BRONZE","英勇黄铜"},
	{"UNRANKED","没有段位"}
};//排位段位对应中文
//static const std::map<std::string, std::string>   tft_rank_tiers = {
//    {"CHALLENGER","最强王者"},
//    {"GRANDMASTER","傲世宗师"},
//    {"MASTER","超凡大师"},
//    {"IRON","坚韧黑铁"},
//    {"DIAMOND","璀璨钻石"},
//    {"PLATINUM","华贵铂金"},
//    {"GOLD","荣耀黄金"},
//    {"SILVER","不屈白银"},
//    {"BRONZE","英勇黄铜"},
//    {"UNRANKED","没有段位"}
//};//云顶之弈排位(不包括经典云顶之弈排位) 段位对应中文 双人模式的段位好像改回成经典模式一致的命名规则了

// String s = requestUtil.doGet("/lol-gameflow/v1/gameflow-phase");
enum class GAME_STATUS
{
	Error = 0,
	None = 1,	//游戏大厅:None
	Lobby = 2,	//房间内:Lobby
	Matchmaking = 3,	//匹配中:Matchmaking
	ReadyCheck = 4,		//找到对局:ReadyCheck
	ChampSelect = 5,	//选英雄中:ChampSelect
	InProgress = 6,		//游戏中:InProgress
	PreEndOfGame = 7,	//游戏即将结束:PreEndOfGame
	WaitingForStats = 8,	//等待结算页面:WaitingForStats
	EndOfGame = 9,		//游戏结束:EndOfGame
	Reconnect = 10		//等待重新连接:Reconnect
};
static const std::string game_status_dict[11][2] = {
	{"获取错误", "Error"},
	{"游戏大厅", "None"},
	{"房间内", "Lobby"},
	{"匹配中", "Matchmaking"},
	{"找到对局", "ReadyCheck"},
	{"选英雄中", "ChampSelect"},
	{"游戏中", "InProgress"},
	{"游戏即将结束", "PreEndOfGame"},
	{"等待结算页面", "WaitingForStats"},
	{"游戏结束", "EndOfGame"},
	{"等待重新连接", "Reconnect"}
};
inline GAME_STATUS	get_enum_gamestatus(std::string status) {
	status.erase(std::remove(status.begin(), status.end(), '"'), status.end());
	int index = 0;
	for (index = static_cast<int>(GAME_STATUS::Error); index <= static_cast<int>(GAME_STATUS::Reconnect); index++) {
		if (status._Equal(game_status_dict[index][1]))
		{
			return	static_cast<GAME_STATUS>(index);
		}
	}
	return	GAME_STATUS::Error;
}



struct CHAMPION
{
	int	selectid;
	std::string	name;	//黑暗之女
	std::string	title;	//安妮
	bool	freePlay;	//周免?
};