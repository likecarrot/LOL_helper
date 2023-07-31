#pragma	once
#include	<Windows.h>
#include	<string>
#include	"struct.h"
#include	"request_utils.h"
#include	<regex>
#include	"utils.h"
#include	"parse_json.h"
#include	<vector>
#include	<TlHelp32.h>
#include	<mutex>
#include	"..\game_resource.h"

//#define	DEBUG


class helper
{
public:
	helper(const helper&) = delete;
	helper& operator=(const helper&) = delete;

	static helper& getInstance() {
		static helper instance;
		return instance;
	}
private:
	helper() {};
	~helper() {};

public:
	void	wait_game_start();
	bool	init();	//获取当前客户端账号的信息,用这个接口来测试是否成功连接



	GAME_STATUS	get_game_status();
	void	accept_game();
	void	auto_next_game();	//这个函数是游戏结束后,重新开始进入模式,但是并不进入匹配队列
	void	search_queue();		//这个函数是在游戏房间内,进入匹配队列
	void	lock_champions(int	champion_id);		//锁定选择英雄


	std::vector<CHAMPION> get_owner_champions();	//获取已经拥有的英雄
	std::vector<CHAMPION> get_all_champions();	//获取所有英雄
	SUMMONER_INFO get_self_summoner_datas();
	RANK_LEVEL get_self_Rank_level();

	std::vector<CHAMPION_TOP>	get_top_champions(std::string	accountid, int limit);


	//对局助手相关
	//bool	getEnvironment();//获取当前客户端所在大区
	std::vector<TEAM_SUMMONER_INFO>	getChatRoomPlayerIdList();	//获取选人界面我队所有人的信息
	std::string	getChatRoomId();	//获取选人界面的游戏房间id

	std::wstring	getDisplayName(std::string accountid);
	RANK_LEVEL	puuid_get_rank_datas(std::string puuid);//通过summoner_info获取段位信息
	std::vector<PLAYER_HISTORY_MATCHDATA>	getHistoryMatchDatas(std::string puuid, std::string accountid, int min = 0, int max = 19);//通过summoner_info获取历史战绩
	MATCH_DETAILED_DATA	getGameNoticeInfo(std::string	gameid);//通过gameid获取具体详细数据




	static std::string GetProcessCommandLine(const std::string& cmdLine);
private:
	DOMAIN_INFO	domain;
	GAME_STATUS	game_status;	//现在游戏的状态
	SUMMONER_INFO	my_summoner;

	std::vector<CHAMPION>	owner_champions;		//id , 英雄结构
	std::vector<CHAMPION>	all_champions;


	const	std::string	current_client_player_summoner_info_api = "/lol-summoner/v1/current-summoner";		//获取现在客户端上登录的玩家的信息
	const	std::string	puuid_get_player_rank_data_api = "/lol-ranked/v1/ranked-stats/";			//通过puuid获取段位信息
	const	std::string	client_get_player_status_api = "/lol-gameflow/v1/gameflow-phase";			//获取客户端状态,比如 游戏中,大厅中
	const	std::string	client_accept_matching_api = "/lol-matchmaking/v1/ready-check/accept";		//接收对局
	const	std::string	matching_get_myteam_summonerinfo_api = "/lol-champ-select/v1/session";		//获取己方队伍的信息,比如 summoner accoutid
	const	std::string	summonerid_get_summonerinfo_api = "/lol-summoner/v1/summoners/";	//根据summonerid获取信息 + ${summonerId}
	const	std::string	gameid_get_detailed_data_api = "/lol-match-history/v1/games/";	//根据gameid获取详细数据,+gameid
	const	std::string	summonerid_get_top_champion_api = "/lol-collections/v1/inventories/";//+ ${summonerId} + /champion-mastery/top?limit=6
	const   std::string	get_current_game_mode = "/lol-gameflow/v1/session";		//430 匹配 420 单双排位 440灵活 450 大乱斗 云顶匹配 1090  云顶排位 1100 云顶狂暴 1130 云顶双人作战 1160
	const	std::string	get_select_champion_chatroomid_api = "/lol-chat/v1/conversations";//获取选人界面时,游戏房间id
};

