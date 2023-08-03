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
	std::string accountId;	//这个值应该是等于 accountid的
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

struct TEAM_SUMMONER_INFO
{
	std::string	summonerId;
	std::string	puuid;
	std::wstring	displayName;
	int	participantId;	//本次对局中的序号
};



struct PARTICIPANTS
{
	int	participantId;//所处的序号
	int	championId;	//选择的英雄
	int	assists;	//助攻
	int	champLevel;	//等级
	int	deaths;		//死亡次数
	int	kills;		//击杀次数
	int	visionScore;//视野得分
	int	goldEarned;	//本局获得金币
	int	totalDamageDealtToChampions;//本局对英雄造成的伤害总和
	bool	win;	//胜利?
};
struct PLAYER_HISTORY_MATCHDATA
{
	unsigned	long long	gameCreation;//1690566965655,
	unsigned	long long	gameDuration;//2067
	std::string	gameId;
	std::string	gameMode;
	std::string gameType;
	PARTICIPANTS participants;
	bool	isRank = false;	//是排位赛吗
	std::string summoner_id;	//用户id
};
struct BANS
{
	int	champid;
	int	pickTurn;
};
struct MATCH_DETAILED_DATA//队伍的对局信息
{
	unsigned	long long	gameCreation;// 1690566965655,
	unsigned	long long	gameDuration;//2067
	std::string	gameId;
	std::string	gameMode;	//如果是斗魂竞技场 1-2 一队 3-4 一队
	bool	isRank = false;	//是排位赛吗
	std::vector<TEAM_SUMMONER_INFO>	player_info;	//玩家信息
	std::vector<PARTICIPANTS>	player_datas;
	std::vector<BANS>	bans;
};

struct CHAMPION_TOP
{
	int	championId;
	int	championLevel;
	int	championPoints;
	std::string	lastPlayTime;
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