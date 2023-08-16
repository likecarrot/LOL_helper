#include "details_items_ui.h"


Details_Item::Details_Item()
{
}


Details_Item::~Details_Item()
{
}

void Details_Item::InitSubControls(const LCU_JSON_RESPONSE::LolMatchHistory& data)
{
	game_mode = dynamic_cast<ui::Label*>(FindSubControl(L"game_mode"));
	game_date = dynamic_cast<ui::Label*>(FindSubControl(L"game_date"));
	create_date = dynamic_cast<ui::Label*>(FindSubControl(L"create_date"));
	game_win = dynamic_cast<ui::Label*>(FindSubControl(L"game_win"));
	player_kda = dynamic_cast<ui::Label*>(FindSubControl(L"player_kda"));
	use_champ = dynamic_cast<ui::Control*>(FindSubControl(L"use_champ"));
	player_dealtandvision_detailinfo = dynamic_cast<ui::Label*>(FindSubControl(L"player_dealtandvision_detailinfo"));


	if (!F_game_class.GetSecond(data.queue_id.value()).empty())
	{
		game_mode->SetText(StringToWString(F_game_class.GetSecond(data.queue_id.value())));
	}else
		game_mode->SetText(L"未知模式");
	
	create_date->SetText(StringToWString(timestamp2string(data.game_creation.value())));
	game_date->SetText(L"对局时长:" + std::to_wstring(data.game_duration.value() / 60) + L"分钟");

	
	game_win->SetText( data.participants.value().at(0).stats.win ? L"胜利" : L"失败");
	game_win->SetStateTextColor(ui::ControlStateType::kControlStateNormal, (data.participants.value().at(0).stats.win) ? L"blue" : L"red");


	std::string kda = std::to_string(data.participants.value().at(0).stats.kills) + "/";		//这里瞎填的成员
	kda.append(std::to_string(data.participants.value().at(0).stats.deaths) + "/");
	kda.append(std::to_string(data.participants.value().at(0).stats.assists)+" ");
	
	kda.append("Lv:" + std::to_string(data.participants.value().at(0).stats.champ_level));
	kda.append(" 经济:" + std::to_string(data.participants.value().at(0).stats.gold_earned));
	player_kda->SetText(StringToWString(kda));
	std::string	dealt = "对英雄伤害:" + std::to_string(data.participants.value().at(0).stats.total_damage_dealt_to_champions);
	dealt.append(" 视野:" + std::to_string(data.participants.value().at(0).stats.vision_score));
	player_dealtandvision_detailinfo->SetText(StringToWString(dealt));

	nbase::ThreadManager::PostTask(kThreadNetwork, nbase::Bind(&Details_Item::SetBkImg, this, GAME_RESOURCES::GAME_RES::getInstance().getIconsPath(GAME_RESOURCES::CHAMPION_ICONS, data.participants.value().at(0).champion_id)));
}

bool Details_Item::OnRemove(ui::EventArgs* args)
{
	ui::ListBox* parent = dynamic_cast<ui::ListBox*>(this->GetParent());
	return parent->Remove(this);
}
void	Details_Item::SetBkImg(std::string path) {
	if (!path.empty())
	{
		use_champ->SetBkImage(string2wstring(path));
	}
}