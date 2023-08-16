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
}

LRESULT Details_Pop::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	info_my_team.clear();
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}


LRESULT Details_Pop::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg== WM_KEYDOWN)
	{
		UINT virtualKeyCode = static_cast<UINT>(wParam);
		if (virtualKeyCode==VK_TAB)
		{
			reset_info();
		}
		return	false;
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}
void	Details_Pop::init_info(LCU_JSON_RESPONSE::LolChampSelect info) {
	nbase::ThreadManager::PostTask(KThreadTestUiThread, [this, info] {
		std::lock_guard<std::mutex> lock(_data_mtx);
		info_ = info;
		for (const auto a : info.my_team) {
			info_my_team.insert(std::make_pair(a.cell_id,a));
		}
		now_player_cellid = info_my_team.cbegin()->first;
		});
}
void	Details_Pop::reset_info() {
	nbase::ThreadManager::PostTask(KThreadTestUiThread, [this] {
		std::lock_guard<std::mutex> lock(_data_mtx);
		set_info_to_ui(info_my_team.at(now_player_cellid));
		if (now_player_cellid+1>info_my_team.crbegin()->first)
			now_player_cellid = info_my_team.cbegin()->first;
		else
			now_player_cellid += 1;
		});
}

void	Details_Pop::set_info_to_ui(LCU_JSON_RESPONSE::MyTeam player) {
	wind_title->SetText(string2wstring(player.display_name));
	wind_title->SetStateTextColor(ui::ControlStateType::kControlStateNormal, L"blue");

	if (all_datas.find(player.cell_id) != all_datas.cend())
		Post2UI( [this, player] {
		Recv_datas1(all_datas.at(player.cell_id));
			});
	else
		nbase::ThreadManager::PostTask(kThreadNetwork, nbase::Bind(&Details_Pop::Recv_datas1, this, raw_lcu_api::getInstance().getHistoryMatchDatas(player.puuid, std::to_string(player.summoner_id))));
}

void	Details_Pop::Recv_datas1(LCU_JSON_RESPONSE::LolMatchHistory	datas) {
	Post2UI([this, datas] {
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
	for (const auto& i : all_datas.at(participantId).games->games)
	{
		Details_Item* item = new Details_Item;
		ui::GlobalManager::FillBoxWithCache(item, L"basic/detailsitem.xml");
		item->InitSubControls(i);
		Vlist->Add(item);
	}
}
bool	Details_Pop::details_isinvalid() {
	std::lock_guard<std::mutex> lock(_data_mtx);
	return	info_my_team.size() > 0&&info_my_team.size()<100 ? true : false;
}