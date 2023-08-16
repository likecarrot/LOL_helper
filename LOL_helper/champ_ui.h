#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/struct.h"
#include	"game_resource.h"

// 从 ui::ListContainerElement 中继承所有可用功能
class Champ_Item_Ui : public ui::ListContainerElement
{
public:
	Champ_Item_Ui();
	~Champ_Item_Ui();

	// 提供外部调用来初始化 item 数据
	void InitSubControls(const CHAMP_INFO data);

private:
	bool OnRemove(ui::EventArgs* args);
	void	SetBkImg(std::string path);
private:
	ui::Label* champ_name;	//游戏模式
	ui::Control* champ_icon;	//英雄头像
};

