#pragma once

#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/struct.h"
#include	"game_resource.h"
// 从 ui::ListContainerElement 中继承所有可用功能
namespace MATCH_ITEMS {
	struct NEED_DATAS
	{
		std::wstring player_name;
		RANK_LEVEL	rank_datas;
		int	top1=0;	//英雄id
		int	top2 = 0;	//英雄id
		int	top3 = 0;	//英雄id
		int	top4 = 0;	//英雄id
		int	top5 = 0;	//英雄id
		int	top6 = 0;	//英雄id
	};
}

class Match_Items : public ui::ListContainerElement
{
public:
	Match_Items() {};
	~Match_Items() {};

	// 提供外部调用来初始化 item 数据
	void InitSubControls(MATCH_ITEMS::NEED_DATAS player_info);

private:
	bool OnRemove(ui::EventArgs* args);
private:
	MATCH_ITEMS::NEED_DATAS _info;


	ui::Label* team_player_name;		//玩家名

	ui::Label* team_player_rank_SOLO;	//单双
	ui::Label* team_player_rank_FLEX;	//灵活
	ui::Label* team_player_rank_SOLO_last;	//单双
	ui::Label* team_player_rank_FLEX_last;	//灵活


	ui::Control* top1;
	ui::Control* top2;
	ui::Control* top3;
	ui::Control* top4;
	ui::Control* top5;
	ui::Control* top6;


	ui::Label* label_title_;

};
