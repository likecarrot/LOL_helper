#pragma	once
#include	"../utils.h"
#include	"json.hpp"
#include <iostream>
#include <fstream>
#include	<list>
using   json = nlohmann::json;

struct CONFIG_
{
	bool	_auto_accept = true;
	bool	_auto_next = true;
	bool	_search_queue = false;
	bool	_lock_champion = false;
	int		_lock_champion_id = 0;

	bool	_matching_helper = true;
	bool	_aram_helper = true;
	struct {
		int	Use_Reroller = 0;	//配置方面,0-代表不使用 1-代表没有时使用 2-代表有就使用
		std::vector<std::pair<int, int>> love_champs;//喜欢的英雄  int-好感值 英雄id
		std::vector<std::pair<int, int>> hate_champs;//不喜欢的英雄 int-讨厌值 英雄id
		std::vector<int> normal_champs;//无感的英雄 英雄id
	}_aram_config;
public:
	std::string output_config() {
		json json_data = {
			{"auto_accept_switch", _auto_accept},
			{"auto_next_again_switch", _auto_next},
			{"auto_search_queue_switch", _search_queue},
			{"lock_champion_switch", _lock_champion},
			{"lock_champion_id", _lock_champion_id},
			{"matching_helper_switch", _matching_helper},
			{"aram_helper_switch", _aram_helper}
		};

		json_data["love_champs"] = _aram_config.love_champs;
		json_data["hate_champs"] = _aram_config.hate_champs;
		json_data["normal_champs"] = _aram_config.normal_champs;
		return  json_data.dump(4);
	};
	void load_configs(json j, CONFIG_& config) {
		// Load config values from JSON
		config._auto_accept = j["auto_accept_switch"];
		config._auto_next = j["auto_next_again_switch"];
		config._search_queue = j["auto_search_queue_switch"];
		config._lock_champion = j["lock_champion_switch"];
		config._lock_champion_id = j["lock_champion_id"];
		config._matching_helper = j["matching_helper_switch"];
		config._aram_helper = j["aram_helper_switch"];

		if (j.contains("love_champs") && !j["love_champs"].empty()) {
			std::vector<std::vector<nlohmann::json>> loveChamps = j["love_champs"];
			for (const auto& champ : loveChamps) {
				int score = champ[0];
				int champId = champ[1];
				config._aram_config.love_champs.push_back(std::make_pair(score, champId));
			}
		}

		if (j.contains("hate_champs") && !j["hate_champs"].empty()) {
			std::vector<std::vector<nlohmann::json>> hateChamps = j["hate_champs"];
			for (const auto& champ : hateChamps) {
				int score = champ[0];
				int champId = champ[1];
				config._aram_config.hate_champs.push_back(std::make_pair(score, champId));
			}
		}

		if (j.contains("normal_champs") && !j["normal_champs"].empty()) {
			for (const auto& champ : j["normal_champs"]) {
				int champId = champ;
				config._aram_config.normal_champs.push_back(champId);
			}
		}
	};
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

