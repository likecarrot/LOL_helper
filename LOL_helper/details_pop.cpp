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
void	Details_Pop::init_info(std::vector<TEAM_SUMMONER_INFO> info) {
	nbase::ThreadManager::PostTask(kThreadMain, [this, info] {
		info_ = info;
		});
}
void	Details_Pop::reset_info(int key) {
	nbase::ThreadManager::PostTask(kThreadMain, [this, key] {
		if (key >=0&&info_.size()!=0)
		{
			set_info_to_ui(info_.at(key%info_.size()));
		}
		});
}

void	Details_Pop::set_info_to_ui(TEAM_SUMMONER_INFO player) {
	wind_title->SetText(player.displayName);
	if (all_datas.find(player.participantId) != all_datas.cend())
		nbase::ThreadManager::PostTask(kThreadMain, [this, player] {
		Recv_datas1(all_datas.at(player.participantId));
			});
	else
		nbase::ThreadManager::PostTask(kThreadNetwork, nbase::Bind(&Details_Pop::Recv_datas1, this, helper::getInstance().getHistoryMatchDatas(player.puuid, player.summonerId)));
}

void	Details_Pop::Recv_datas1(std::vector<PLAYER_HISTORY_MATCHDATA>	datas) {
	nbase::ThreadManager::PostTask(kThreadMain, [this, datas] {
		if (datas.size() == 0)
		{
			return;
		}
		Vlist->RemoveAll();
		for (const auto& a : info_) {
			if (a.summonerId._Equal(datas.at(0).summoner_id))
			{
				if (all_datas.find(a.participantId) == all_datas.cend())
				{
					all_datas.insert(std::make_pair(a.participantId, datas));
				}
				add_items(a.participantId);
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
	
	for (const auto& i : all_datas.at(participantId))
	{
		Details_Item* item = new Details_Item;
		ui::GlobalManager::FillBoxWithCache(item, L"basic/detailsitem.xml");
		item->InitSubControls(i);
		Vlist->Add(item);
	}
}
bool	Details_Pop::details_isinvalid() {
	return	info_.size() > 0 ? true : false;
}