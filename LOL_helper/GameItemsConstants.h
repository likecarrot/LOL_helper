#pragma	once

#include	<iostream>
#include    <map>
#include	"factory.h"


//包含游戏的常量定义,和来源


//mapid https://static.developer.riotgames.com/docs/lol/maps.json
//gamemode https://static.developer.riotgames.com/docs/lol/gameModes.json
//gametype https://static.developer.riotgames.com/docs/lol/gameTypes.json
static const std::map<std::string, std::string>   rank_class_dict = {
	{"RANKED_SOLO_5x5","单排/双排"},
	{"RANKED_FLEX_SR","灵活组排 5v5"},
	{"RANKED_TFT","云顶之弈"},
	{"RANKED_TFT_TURBO","云顶之弈狂暴模式"},
	{"RANKED_TFT_DOUBLE_UP","云顶之弈双人作战"}
}; //排位等级的模式

static const std::map<std::string, std::string>   rank_tiers_dict = {
	{"CHALLENGER","最强王者"},
	{"GRANDMASTER","傲世宗师"},
	{"MASTER","超凡大师"},
	{"IRON","坚韧黑铁"},
	{"DIAMOND","璀璨钻石"},
	{"EMERALD","流光翡翠"},
	{"PLATINUM","华贵铂金"},
	{"GOLD","荣耀黄金"},
	{"SILVER","不屈白银"},
	{"BRONZE","英勇黄铜"},
	{"UNRANKED","没有段位"}
};


static const std::map<std::string, std::string>   GAME_MODE = {
	{"CLASSIC","经典模式"},
	{"ARAM","极地大乱斗"},
	{"CHERRY","斗魂竞技场"},
	{"TUTORIAL_MODULE_1","新手教程1"},
	{"TUTORIAL_MODULE_2","新手教程2"},
	{"TUTORIAL_MODULE_3","新手教程3"},
	{"PRACTICETOOL","训练模式"}
};
static const std::map<std::string, std::string >GAME_TYPE = {
	{"MATCHED_GAME","匹配模式"},
	{"CUSTOM_GAME","自定义"},
	{"TUTORIAL_GAME","游戏教程"}
};


static Factory<std::string>	F_rank_class(rank_class_dict);
static Factory<std::string>	F_rank_tiers(rank_tiers_dict);
static Factory<std::string>	F_game_mode(GAME_MODE);
static Factory<std::string>	F_game_type(GAME_TYPE);
