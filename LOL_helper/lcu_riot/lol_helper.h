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
//#define	DEBUG


class helper
{
public:
	helper();
	~helper();
	void	wait_game_start();
	bool	init();	//获取当前客户端账号的信息,用这个接口来测试是否成功连接

	bool	get_Rank_level();
	GAME_STATUS	get_game_status();
	void	accept_game();
	void	auto_next_game();	//这个函数是游戏结束后,重新开始进入模式,但是并不进入匹配队列
	void	search_queue();		//这个函数是在游戏房间内,进入匹配队列
	int		get_owner_champions();	//获取已经拥有的英雄
	int		get_all_champions();	//获取所有英雄
	std::string	download_icons(int	icon_id);	//下载头像,返回值是保存的地址
	void	lock_champions(int	champion_id);		//锁定选择英雄
	static std::string GetProcessCommandLine(const std::string& cmdLine);

	std::vector<CHAMPION>&	get_owner_champions_datas() { std::lock_guard<std::mutex> lock(owner_champions_mutex); return owner_champions; };
	std::vector<CHAMPION>&	get_all_champions_datas() { return all_champions; };
	SUMMONER_INFO&	get_summoner_datas() { return	summoner; };
	RANK_LEVEL	&get_rank_level_datas() { return rank_level; };
	
	std::mutex owner_champions_mutex; // 添加互斥锁

private:
	DOMAIN_INFO	domain;
	SUMMONER_INFO	summoner;
	RANK_LEVEL	rank_level;
	GAME_STATUS	game_status;	//现在游戏的状态
	std::vector<CHAMPION>	owner_champions;		//id , 英雄结构
	std::vector<CHAMPION>	all_champions;
};

