#include "details_pop.h"


const std::wstring Details_Pop::kClassName = L"basic";


Details_Pop::Details_Pop() {

};
Details_Pop::~Details_Pop() {};
std::wstring Details_Pop::GetSkinFolder()
{
	return L"basic";
}

std::wstring Details_Pop::GetSkinFile()
{
	return L"detailspop.xml";
}

std::wstring Details_Pop::GetWindowClassName() const
{
	return kClassName;
}

void Details_Pop::InitWindow()
{
	wind_title = dynamic_cast<ui::Label*>(FindControl(L"window_title"));
	Vlist = dynamic_cast<ui::ListBox*>(FindControl(L"list"));
	main_box= dynamic_cast<ui::VBox*>(FindControl(L"main_box"));
	main_box->AttachBubbledEvent(ui::kEventAll, nbase::Bind(&Details_Pop::init_set_listen_controls, this, std::placeholders::_1));
}
bool	Details_Pop::init_set_listen_controls(ui::EventArgs* args) {
	if (args->chKey == VK_TAB)
	{
		int n = 3;
	}
	return	true;
}
LRESULT Details_Pop::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}


LRESULT Details_Pop::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg== WM_KEYDOWN)
	{
		UINT virtualKeyCode = static_cast<UINT>(wParam);
		if (virtualKeyCode==VK_TAB)
		{
			reset_info(now_show_player_participantId);
		}
		return	false;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}
void	Details_Pop::init_info(LCU_JSON_RESPONSE::LolChampSelect info) {
	nbase::ThreadManager::PostTask(kThreadMain, [this, info] {
		info_ = info;
		});
}
void	Details_Pop::reset_info(int key) {
	nbase::ThreadManager::PostTask(kThreadMain, [this, key] {
		if (key >=0&&info_.my_team.size()!=0)
		{
			set_info_to_ui(info_.my_team.at(key%info_.my_team.size()));
		}
		});
}

void	Details_Pop::set_info_to_ui(LCU_JSON_RESPONSE::MyTeam player) {
	wind_title->SetText(string2wstring(player.display_name));
	if (all_datas.find(player.cell_id) != all_datas.cend())
		nbase::ThreadManager::PostTask(kThreadMain, [this, player] {
		Recv_datas1(all_datas.at(player.cell_id));
			});
	else
		nbase::ThreadManager::PostTask(kThreadNetwork, nbase::Bind(&Details_Pop::Recv_datas1, this, raw_lcu_api::getInstance().getHistoryMatchDatas(player.puuid, std::to_string(player.summoner_id))));
}

void	Details_Pop::Recv_datas1(LCU_JSON_RESPONSE::LolMatchHistory	datas) {
	nbase::ThreadManager::PostTask(kThreadMain, [this, datas] {
		if (datas.games->games.size() == 0)
		{
			return;
		}
		Vlist->RemoveAll();
		for (const auto& a : info_.my_team) {
			if (a.summoner_id==datas.account_id)
			{
				if (all_datas.find(a.cell_id) == all_datas.cend())
				{
					all_datas.insert(std::make_pair(a.cell_id, datas));
				}
				add_items(a.cell_id);
				break;
			}
		}
		});
}

void	Details_Pop::add_items(int participantId) {
	if (now_show_player_participantId !=participantId)
	{
		now_show_player_participantId = participantId;
	}
	
	for (const auto& i : all_datas.at(participantId).games->games)
	{
		Details_Item* item = new Details_Item;
		ui::GlobalManager::FillBoxWithCache(item, L"basic/detailsitem.xml");
		item->InitSubControls(i);
		Vlist->Add(item);
	}
}
bool	Details_Pop::details_isinvalid() {
	return	info_.my_team.size() > 0 ? true : false;
}