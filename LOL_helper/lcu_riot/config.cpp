#include	"config.h"

using   json = nlohmann::json;

bool exist_configfile() {
	FILE* file;
	if (fopen_s(&file, config_file_path.c_str(), "r") == 0) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

void	save_default_config() {
	json json_data = {
	  {"自动接收对局", true},
	  {"自动再来一局", true},
	  {"?什么是自动再来一局", "游戏结束后再来一局,但是并不会自动开始匹配"},
	  {"自动匹配下一把", false},
	  {"?什么自动匹配下一把", "与上面不同的是只有开启了自动再来一局才可以使用自动匹配下一把功能"},
	  {"安静输出", true},
		{"自动锁定英雄",false},
		{"lock_champion_id",0}
	};
	std::ofstream file(config_file_path);
	if (!file.is_open()) {
		std::cerr << "Failed to create file: " << config_file_path << std::endl;
		return;
	}
	try
	{
		file << std::setw(4) << json_data << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "e.what   " << e.what() << std::endl;
		file.close();
	}
	std::cout << "输出配置文件在运行目录下:文件名 " << config_file_path << std::endl;
}
void	load_config(CONFIG_FILES& configs) {
	CONFIG_FILES    config;
	if (exist_configfile() == false)
	{
		save_default_config();
	}
	if (exist_configfile() == true)
	{
		std::ifstream file(config_file_path);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << config_file_path << std::endl;
			return;
		}

		json jsonData;
		try {
			file >> jsonData;
		}
		catch (const json::parse_error& e) {
			std::cerr << "Failed to parse JSON from file: " << config_file_path << std::endl;
			std::cerr << "Error message: " << e.what() << std::endl;
			return;
		}
		if (!jsonData.contains("自动接收对局") || !jsonData.contains("自动再来一局") || !jsonData.contains("自动匹配下一把") || !jsonData.contains("安静输出") || !jsonData.contains("自动锁定英雄") || !jsonData.contains("lock_champion_id"))
			return;
		configs._auto_accept = jsonData["自动接收对局"].get<bool>();
		configs._auto_next = jsonData["自动再来一局"].get<bool>();
		configs._search_queue = jsonData["自动匹配下一把"].get<bool>();
		configs._quiet_output = jsonData["安静输出"].get<bool>();
		configs._lock_champion = jsonData["自动锁定英雄"].get<bool>();
		configs._lock_champion_id = jsonData["lock_champion_id"].get<int>();
	}
}