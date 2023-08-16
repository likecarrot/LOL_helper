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

enum class RANK_LEVEL_INDEX
{
	RANKED_SOLO_5x5,
	RANKED_FLEX_SR,
	RANKED_TFT,
	RANKED_TFT_TURBO,
	RANKED_TFT_DOUBLE_UP
};



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


struct CHAMP_INFO
{
	int		champ_id;
	std::string	champ_name;
	int		lova_score;	//只有aram助手用得到 并且会在使用的地方进行自主初始化
public:
	CHAMP_INFO():champ_id(0),champ_name(),lova_score(0){}
};