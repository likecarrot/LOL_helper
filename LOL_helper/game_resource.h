#pragma	once
#include	<curl/curl.h>
#include	<string>
#include <fstream>
#include	"lcu_riot/request_utils.h"
#include	"lcu_riot/json.hpp"
#include	"utils.h"

//这个类用来保存和下载游戏文件
//比如 头像,英雄预览
//首先初始化,用来获取一个文件夹appdata下,然后新建目录
//有一些枚举值,用来标识 头像 | 英雄 
//好处是如果本地已经存在了,那么就返回路径,如果不存在就下载
using json = nlohmann::json;

namespace GAME_RESOURCES
{
	enum DOWN_TYPE
	{
		ICONS, CHAMPION_ICONS
	};
	class GAME_RES
	{
	public:
		GAME_RES(const GAME_RES&) = delete;
		GAME_RES& operator=(const GAME_RES&) = delete;

		static GAME_RES& getInstance() {
			static GAME_RES instance;
			return instance;
		}
		std::string	getIconsPath(DOWN_TYPE type, int id);

	private:
		GAME_RES();
		~GAME_RES();
		bool	download(DOWN_TYPE type, std::string child_url, std::string save_name);
	private:
		std::string	IconsFolder;
	};
}