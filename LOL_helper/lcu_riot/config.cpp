#include	"config.h"

using   json = nlohmann::json;

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
	std::ofstream file(default_config_file_path);
	if (!file.is_open()) {
		std::cerr << "Failed to create file: " << default_config_file_path << std::endl;
		return;
	}
	try
	{
		file << std::setw(4) << new_config.output_config() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cout << "e.what   " << e.what() << std::endl;
		file.close();
	}
}
bool	CONFIG_FILES::load_config_file(std::string	input_path) {
	if (input_path.empty())
	{
		input_path = default_config_file_path;
	}
	if (check_exist_config_file(input_path) == true)
	{
		std::ifstream file(input_path);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << input_path << std::endl;
			return	false;
		}

		json jsonData;
		try {
			jsonData = json::parse(file);
			configs.load_configs(jsonData, configs);
		}
		catch (const json::parse_error& e) {
			std::cerr << "Failed to parse JSON from file: " << input_path << std::endl;
			std::cerr << "Error message: " << e.what() << std::endl;
			return	false;
		}
		return	true;
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
		std::cerr << "Failed to create file: " << output_path << std::endl;
		return	false;
	}
	try
	{
		file << configs.output_config() << std::endl;
	}
	catch (const std::exception& e)
	{
		file.close();
		return	false;
	}
	file.close();
	return	true;
}

