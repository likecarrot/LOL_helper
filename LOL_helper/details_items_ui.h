#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/struct.h"
#include "lcu_riot/utils.h"
#include	"game_resource.h"


// 从 ui::ListContainerElement 中继承所有可用功能
class Details_Item : public ui::ListContainerElement
{
public:
	Details_Item();
	~Details_Item();

	// 提供外部调用来初始化 item 数据
	void InitSubControls(const PLAYER_HISTORY_MATCHDATA &data);

private:
	bool OnRemove(ui::EventArgs* args);
	void	SetBkImg(std::string path);
private:
	ui::Label* game_mode;	//游戏模式
	ui::Label* create_date;	//创建游戏时间
	ui::Label* game_date;	//游戏持续时间

	ui::Label* game_win;	//赢了还是输了
	ui::Label* player_kda;	//战绩
	ui::Label* player_dealtandvision_detailinfo;	//伤害和视野得分
	ui::Control* use_champ;	//英雄头像
};

