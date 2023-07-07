#pragma	once
#include	<curl/curl.h>
#include	<string>
#include "shlobj.h" 
#include	"lcu_riot/lol_helper.h"

static	std::string	my_host_check = "http://116.63.172.78:5000/api/check";
static	std::string	my_host_dll = "http://116.63.172.78:5000/api/dll";

class dynamic_skin_host_my
{
public:
	static bool	check_Allow();
	static std::string	download_dll();	//返回下载的地址
	static	std::string	get_lol_game_path();	//返回lol游戏目录
	static	bool	open_dynamic_skin(std::string	dll_path);
	static	bool	close_dynamic_skin();
};

