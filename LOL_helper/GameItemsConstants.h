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
	{"UNRANKED","没有段位"},

    {"Grey","灰"},
    {"Green","绿"},
    {"Blue","蓝"},
    {"Purple","紫"},
    {"Hyper","金"}
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


static const std::map<int, std::string> GAME_CLASS = {
    {0, "自定义"},
    {72, "冰雪节1v1"},
    {73, "冰雪节2v2"},
    {75, "双倍人数"},
    {76, "极速模式"},
    {78, "镜像模式"},
    {83, "合作对战极速模式"},
    {98, "双倍人数"},
    {100, "狂乱ARAM"},
    {310, "复仇者"},
    {313, "黑市交易"},
    {317, "不是支配"},
    {325, "全随机"},
    {400, "选秀"},
    {420, "单双"},
    {430,"匹配"},
    {440, "灵活"},
    {450, "大乱斗"},
    {600, "猎杀刺客"},
    {610, "宇宙星系"},
    {700, "对决"},
    {720, "ARAM对决"},
    {820, "初级人机"},
    {830, "入门人机"},
    {840, "初级人机"},
    {850, "一般人机"},
    {900, "乱斗模式"},
    {910, "晋级"},
    {920, "波比王传奇"},
    {940, "攻城战"},
    {950, "末日投票"},
    {960, "末日"},
    {980, "星光守护者"},
    {990, "星光守护者"},
    {1000, "PROJECT: Hunters"},
    {1010, "雪地乱斗"},
    {1020, "全员英雄"},
    {1030, "奥德赛入门"},
    {1040, "奥德赛新兵"},
    {1050, "奥德赛船员"},
    {1060, "奥德赛船长"},
    {1070, "奥德赛强袭"},
    {1090, "云顶匹配"},
    {1100, "云顶排位"},
    {1110, "云顶教学"},
    {1111, "云顶测试"},
    {1130,"云顶狂暴"},
    {1160,"云顶双人作战"},
    {1300, "奇迹模式"},
    {1400, "终极技能书"},
    {1700, "斗魂竞技场"},
    {1900, "URF"},
    {2000, "教学 1"},
    {2010, "教学 2"},
    {2020, "教学 3"},
};//不完全统计,并且只是暂时作为判断游戏类型的选项
//来源:https://github.com/RiotGames/developer-relations/issues/574

static Factory<std::string>	F_rank_class(rank_class_dict);
static Factory<std::string>	F_ClassicRank_tiers(rank_tiers_dict);
static Factory<std::string>	F_game_mode(GAME_MODE);
static Factory<std::string>	F_game_type(GAME_TYPE);
static Factory<int> F_game_class(GAME_CLASS);