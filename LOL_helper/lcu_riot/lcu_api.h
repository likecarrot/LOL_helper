#pragma	once
#include	<Windows.h>
#include	<string>
#include	"request_utils.h"
#include	<regex>
#include	"../utils.h"
#include	<vector>
#include	<TlHelp32.h>
#include	<mutex>
#include	"..\game_resource.h"
#include	"../lcu_structs.hpp"
#include	"struct.h"

class raw_lcu_api
{
public:
	raw_lcu_api(const raw_lcu_api&) = delete;
	raw_lcu_api& operator=(const raw_lcu_api&) = delete;

	static raw_lcu_api& getInstance() {
		static raw_lcu_api instance;
		return instance;
	}
private:
	raw_lcu_api() {};
	~raw_lcu_api() {};

public:

	GAME_STATUS	get_game_status();
	void	accept_game();
	void	decline_game();	
	void	auto_next_game();	//这个函数是游戏结束后,重新开始进入模式,但是并不进入匹配队列
	void	search_queue();		//这个函数是在游戏房间内,进入匹配队列
	std::string	lock_champions(int	champion_id,bool completed=true);		//锁定选择英雄
	bool	bench_champiosn(int champion_id);	//从备战席上交换英雄 极地大乱斗模式	如果返回true代表成功


	void	set_game_commandline(std::string commandline);
	bool	init();	//获取当前客户端账号的信息,用这个接口来测试是否成功连接

	//2023-8-16重构
	//发现很多请求类,都做的操作太少,返回了一个json数据,只用其中一部分
	//在此之前都是面对功能的网络请求,之后开始面对网络请求
	
	LCU_JSON_RESPONSE::LolChampSelect	get_lol_champ_select_v1_session();
	LCU_JSON_RESPONSE::LolOwnerChampions get_owner_champions();	//获取已经拥有的英雄
	LCU_JSON_RESPONSE::LolGameDataChampSummary get_all_champions();	//获取所有英雄
	LCU_JSON_RESPONSE::LolSummonerInfo get_summoner_datas();
	LCU_JSON_RESPONSE::LolSummonerInfo get_summoner_datas(std::string summoner_id);
	LCU_JSON_RESPONSE::LolRankedData	puuid_get_rank_datas(std::string puuid="");//通过summoner_id获取个人信息

	LCU_JSON_RESPONSE::LolMatchHistory	getHistoryMatchDatas(std::string puuid, std::string accountid, int min = 0, int max = 19);//通过summoner_info获取历史战绩
	LCU_JSON_RESPONSE::LolCollections	get_top_champions(std::string	accountid, int limit);
	LCU_JSON_RESPONSE::LolGameflow	get_current_game_mode();	//是大乱斗模式吗? 判断标准是 "allowRerolling": false, 如果是true就是大乱斗 
	void	ReRoll();		//使用骰子


private:
	DOMAIN_INFO	domain;
	LCU_JSON_RESPONSE::LolSummonerInfo	my_summoner;

	const	std::string	current_client_player_summoner_info_api = "/lol-summoner/v1/current-summoner";		//获取现在客户端上登录的玩家的信息
	const	std::string	puuid_get_player_rank_data_api = "/lol-ranked/v1/ranked-stats/";			//通过puuid获取段位信息
	const	std::string	client_get_player_status_api = "/lol-gameflow/v1/gameflow-phase";			//获取客户端状态,比如 游戏中,大厅中
	const	std::string	client_accept_matching_api = "/lol-matchmaking/v1/ready-check/accept";		//接收对局
	const	std::string	client_decline_matching_api = "/lol-matchmaking/v1/ready-check/decline";	//拒绝对局
	const	std::string	matching_get_myteam_summonerinfo_api = "/lol-champ-select/v1/session";		//获取己方队伍的信息,比如 summoner accoutid
	const   std::string	get_current_game_mode_api = "/lol-gameflow/v1/session";					//430 匹配 420 单双排位 440灵活 450 大乱斗 云顶匹配 1090  云顶排位 1100 云顶狂暴 1130 云顶双人作战 1160
	const	std::string use_reroller_api = "/lol-champ-select/v1/session/my-selection/reroll";	//使用骰子
	const	std::string	get_owner_champions_api = "/lol-champions/v1/owned-champions-minimal";	//获取已经拥有的英雄
	const	std::string	get_all_champions_api = "/lol-game-data/assets/v1/champion-summary.json";	//获取所有英雄
	const	std::string	summonerid_get_summonerinfo_api = "/lol-summoner/v1/summoners/";	//根据summonerid获取信息 + ${summonerId}
	const	std::string	gameid_get_detailed_data_api = "/lol-match-history/v1/games/";		//根据gameid获取详细数据,+gameid
	const	std::string	summonerid_get_top_champion_api = "/lol-collections/v1/inventories/";	//+ ${summonerId} + /champion-mastery/top?limit=6
	const	std::string	get_select_champion_chatroomid_api = "/lol-chat/v1/conversations";	//获取选人界面时,游戏房间id
	const	std::string	gamename_get_summonerinfo_api = "/lol-summoner/v1/summoners?name=";	//玩家游戏名 "/lol-summoner/v1/summoners?name=" + name
	const	std::string	aram_swap_champ_api = "/lol-champ-select/v1/session/bench/swap/";	//+英雄id  大乱斗从备战席拿英雄
	const	std::string	lock_champ_completed_api = "/lol-champ-select/v1/session/actions/";	//+localPlayerCellId
	const	std::string	lock_now_select_champ_api = "/lol-champ-select/v1/session/actions/";	//+localPlayerCellId +/complete
};




