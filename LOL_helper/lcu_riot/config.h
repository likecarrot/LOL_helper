#pragma	once
#include	"utils.h"
#include	"json.hpp"
#include <iostream>
#include <fstream>
struct CONFIG_FILES
{
	bool	_auto_accept=true;
	bool	_auto_next=true;
	bool	_search_queue=false;
	bool	_quiet_output=true;
	bool	_lock_champion=false;
	int		_lock_champion_id = 0;
};
static	const	std::string config_file_path = "config.json";
bool	exist_configfile();
void	load_config(CONFIG_FILES& configs);
void	save_default_config();