#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/struct.h"
#include	"game_resource.h"
#include	"GameItemsConstants.h"
#include	"lcu_structs.hpp"

// 从 ui::ListContainerElement 中继承所有可用功能
namespace MATCH_ITEMS {
	struct NEED_DATAS
	{
		std::wstring player_name;
		int	player_level;
		LCU_JSON_RESPONSE::LolRankedData	rank_datas;
		int	top1 = 0;	//英雄id
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
	// 提供外部调用来初始化 item 数据
	void InitSubControls(MATCH_ITEMS::NEED_DATAS player_info) {
		_info = player_info;
		team_player_name = dynamic_cast<ui::Label*>(FindSubControl(L"team_player_name"));
		team_player_level = dynamic_cast<ui::Label*>(FindSubControl(L"team_player_level"));
		team_player_rank_SOLO = dynamic_cast<ui::Label*>(FindSubControl(L"team_player_rank_SOLO"));
		team_player_rank_FLEX = dynamic_cast<ui::Label*>(FindSubControl(L"team_player_rank_FLEX"));
		team_player_rank_SOLO_last = dynamic_cast<ui::Label*>(FindSubControl(L"team_player_rank_SOLO_last"));
		team_player_rank_FLEX_last = dynamic_cast<ui::Label*>(FindSubControl(L"team_player_rank_FLEX_last"));
		top1 = dynamic_cast<ui::Control*>(FindSubControl(L"top1"));
		top2 = dynamic_cast<ui::Control*>(FindSubControl(L"top2"));
		top3 = dynamic_cast<ui::Control*>(FindSubControl(L"top3"));
		top4 = dynamic_cast<ui::Control*>(FindSubControl(L"top4"));
		top5 = dynamic_cast<ui::Control*>(FindSubControl(L"top5"));
		top6 = dynamic_cast<ui::Control*>(FindSubControl(L"top6"));

		team_player_name->SetText(_info.player_name);
		team_player_level->SetText(L"level:"+std::to_wstring(_info.player_level));
		team_player_rank_SOLO->SetText(StringToWString(F_ClassicRank_tiers.GetSecond(_info.rank_datas.queue_map.ranked_solo_5_x5.tier)  + _info.rank_datas.queue_map.ranked_solo_5_x5.division));
		team_player_rank_SOLO_last->SetText(StringToWString(F_ClassicRank_tiers.GetSecond(_info.rank_datas.queue_map.ranked_solo_5_x5.previous_season_end_tier)  + _info.rank_datas.queue_map.ranked_solo_5_x5.previous_season_end_division));
		team_player_rank_FLEX->SetText(StringToWString(F_ClassicRank_tiers.GetSecond(_info.rank_datas.queue_map.ranked_flex_sr.tier)  + _info.rank_datas.queue_map.ranked_flex_sr.division));
		team_player_rank_FLEX_last->SetText(StringToWString(F_ClassicRank_tiers.GetSecond(_info.rank_datas.queue_map.ranked_flex_sr.previous_season_end_tier)  + _info.rank_datas.queue_map.ranked_flex_sr.previous_season_end_division));
		top1->SetBkImage(string2wstring(GAME_RESOURCES::GAME_RES::getInstance().getIconsPath(GAME_RESOURCES::CHAMPION_ICONS, _info.top1)));
		top2->SetBkImage(string2wstring(GAME_RESOURCES::GAME_RES::getInstance().getIconsPath(GAME_RESOURCES::CHAMPION_ICONS, _info.top2)));
		top3->SetBkImage(string2wstring(GAME_RESOURCES::GAME_RES::getInstance().getIconsPath(GAME_RESOURCES::CHAMPION_ICONS, _info.top3)));
		top4->SetBkImage(string2wstring(GAME_RESOURCES::GAME_RES::getInstance().getIconsPath(GAME_RESOURCES::CHAMPION_ICONS, _info.top4)));
		top5->SetBkImage(string2wstring(GAME_RESOURCES::GAME_RES::getInstance().getIconsPath(GAME_RESOURCES::CHAMPION_ICONS, _info.top5)));
		top6->SetBkImage(string2wstring(GAME_RESOURCES::GAME_RES::getInstance().getIconsPath(GAME_RESOURCES::CHAMPION_ICONS, _info.top6)));
	};

private:
	bool OnRemove(ui::EventArgs* args);
private:
	MATCH_ITEMS::NEED_DATAS _info;


	ui::Label* team_player_name;		//玩家名
	ui::Label* team_player_level;		//玩家等级

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
