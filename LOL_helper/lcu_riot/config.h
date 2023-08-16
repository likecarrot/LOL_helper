#pragma	once
#include	"../utils.h"
#include	"json.hpp"
#include <iostream>
#include <fstream>
#include	<list>
using   json = nlohmann::json;


struct AramConfig {
	int Use_Reroller;
	int	wait_lova_champs_max_sec;
	std::vector<std::pair<int, int>> love_champs;
	AramConfig() : Use_Reroller(1), love_champs(), wait_lova_champs_max_sec(30){
		// 在构造函数中将 love_champs 初始化为空的 std::vector
	}
};
struct CONFIG_
{
	bool	accept_status = true;
	bool	nextgame_status = false;
	bool	searchqueue_status = false;
	bool	matching_helper = true;
	bool	aram_helper = false;
	bool	classic_helper = false;
	int		accept_timeout = 5;	
	AramConfig	_aram_config;
	struct
	{
		bool	auto_lock = false;
		int		champ_id = 0;
		int	lockchamp_timeout = 30;
	}_classic_config;
	// 赋值运算符重载
	CONFIG_& operator=(const CONFIG_& other)
	{
		if (this != &other) {
			// 复制所有成员变量
			accept_status = other.accept_status;
			nextgame_status = other.nextgame_status;
			searchqueue_status = other.searchqueue_status;
			matching_helper = other.matching_helper;
			aram_helper = other.aram_helper;
			classic_helper = other.classic_helper;
			_aram_config = other._aram_config;
			_classic_config = other._classic_config;
			accept_timeout = other.accept_timeout;
		}
		return *this;
	}
};

class CONFIG_FILES
{
public:
	CONFIG_FILES();
	~CONFIG_FILES();
	bool	check_exist_config_file(std::string	input_path);	//检查是否存在配置文件
	bool	load_config_file(std::string	input_path = "");		//加载配置文件
	bool	save_current_config(std::string	output_path = "");	//保存现在配置到
	CONFIG_	configs;

private:
	void	save_default_config();	//保存默认配置
	const	std::string config_file_path = "config.json";
	std::string	default_config_file_path = get_app_path() + "\\" + config_file_path;	//   %APPDATA%/lol_helper/config.json
};

struct Settings_Config
{
	std::string	lock_champname;
	int		lock_champid;
	bool	autolock = false;
	int	autolock_timeout = 0;
	int	accept_timeout = 0;
	int aram_maxtimeout = 0;
	int	reroll = 1;
	std::vector <std::pair<int, int>>	lova_champlist;
};
