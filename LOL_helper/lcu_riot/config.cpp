#include	"config.h"

using   json = nlohmann::json;


void to_json(json& j, const CONFIG_& config)
{
	j = json{
		{"accept_status", config.accept_status},
		{"nextgame_status", config.nextgame_status},
		{"searchqueue_status", config.searchqueue_status},
		{"matching_helper", config.matching_helper},
		{"aram_helper", config.aram_helper},
		{"classic_helper", config.classic_helper},
		{"aram_config", {{"Use_Reroller", config._aram_config.Use_Reroller}, {"wait_max_sec",config._aram_config.wait_lova_champs_max_sec},{"love_champs", config._aram_config.love_champs}}},
		{"classic_config", {{"auto_lock", config._classic_config.auto_lock}, {"champ_id", config._classic_config.champ_id},{"lock_timeout",config._classic_config.lockchamp_timeout}}},
		{"accept_timeout",config.accept_timeout}
	};
}

void from_json(const json& j, CONFIG_& config)
{
	config.accept_status = j["accept_status"];
	config.nextgame_status = j["nextgame_status"];
	config.searchqueue_status = j["searchqueue_status"];
	config.matching_helper = j["matching_helper"];
	config.aram_helper = j["aram_helper"];
	config.classic_helper = j["classic_helper"];

	config._aram_config.Use_Reroller = j["aram_config"]["Use_Reroller"];
	config._aram_config.wait_lova_champs_max_sec= j["aram_config"]["wait_max_sec"];
	config._aram_config.love_champs = j["aram_config"]["love_champs"].get<std::vector<std::pair<int, int>>>();

	config._classic_config.auto_lock = j["classic_config"]["auto_lock"];
	config._classic_config.champ_id = j["classic_config"]["champ_id"];
	config._classic_config.lockchamp_timeout = j["classic_config"]["lock_timeout"];

	config.accept_timeout= j["accept_timeout"];
}

CONFIG_FILES::CONFIG_FILES()
{
}

CONFIG_FILES::~CONFIG_FILES()
{
}

bool	CONFIG_FILES::check_exist_config_file(std::string	input_path) {
	FILE* file;
	if (fopen_s(&file, input_path.c_str(), "r") == 0) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

void	CONFIG_FILES::save_default_config() {
	CONFIG_	new_config;
	json output = new_config;
	std::ofstream file(default_config_file_path);
	if (!file.is_open()) {
		std::cerr << "Failed to create file: " << default_config_file_path << std::endl;
		return;
	}
	try
	{
		file << std::setw(4) << output.dump(4) << std::endl;
	}
	catch (const std::exception& e)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringA("\n");
	}
}
bool	CONFIG_FILES::load_config_file(std::string	input_path) {
	if (input_path.empty())
	{
		input_path = default_config_file_path;
	}
	if (check_exist_config_file(input_path) == true)
	{
		try
		{
			std::ifstream infile(input_path);
			json input;
			infile >> input;
			infile.close();
			this->configs = input;
			return	true;
		}
		catch (const std::exception&)
		{
			OutputDebugStringW(__FUNCTIONW__);
			OutputDebugStringA("\n");
		}
		return	false;
	}
	return	false;
}
bool	CONFIG_FILES::save_current_config(std::string	output_path) {
	if (output_path.empty())
	{
		output_path = default_config_file_path;
	}
	std::ofstream file(output_path);
	if (!file.is_open()) {
		OutputDebugStringW(L"Failed to create file: ");
		OutputDebugStringA("\n");
		return	false;
	}
	try
	{
		json output = configs;
		file << output.dump(4);
	}
	catch (const std::exception& e)
	{
		OutputDebugStringW(__FUNCTIONW__);
		OutputDebugStringA("\n");
		file.close();
		return	false;
	}
	file.close();
	return	true;
}
