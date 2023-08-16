#include "pop_form.h"

const std::wstring Pop_form::kClassName = L"basic";

std::wstring Pop_form::GetSkinFolder()
{
	return L"basic";
}

std::wstring Pop_form::GetSkinFile()
{
	return L"secondui.xml";
}

std::wstring Pop_form::GetWindowClassName() const
{
	return kClassName;
}

void Pop_form::InitWindow()
{
	_list = dynamic_cast<ui::ListBox*>(FindControl(L"list"));
	//_list->AttachBubbledEvent(ui::kEventKeyUp, nbase::Bind(&Pop_form::init_set_listen_controls, this, std::placeholders::_1));
	nbase::ThreadManager::PostTask(kThreadNetwork, std::bind(&Pop_form::Recv_info, this, raw_lcu_api::getInstance().get_lol_champ_select_v1_session()));
}

LRESULT Pop_form::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void Pop_form::Recv_info(LCU_JSON_RESPONSE::LolChampSelect info)
{

	nbase::ThreadManager::PostTask(KThreadTestUiThread, [this, info] {
		HWND hwnd = FindWindowA(NULL, "League of Legends");
		RECT rect;
		GetWindowRect(hwnd, &rect);
		ui::UiRect	pos;
		pos.top = rect.top;
		pos.left = rect.right;
		pos.right = 290 + pos.left;
		pos.bottom = 100 * info.my_team.size() + pos.top;
		this->SetPos(pos, true, SWP_SHOWWINDOW, HWND_TOP, false);
		});


	for (auto& i : info.my_team)
	{
		LCU_JSON_RESPONSE::LolSummonerInfo	info;
		MATCH_ITEMS::NEED_DATAS	need;

		info = raw_lcu_api::getInstance().get_summoner_datas(std::to_string(i.summoner_id));
		i.puuid = info.puuid;
		i.display_name = info.display_name;
		need.player_name = string2wstring(i.display_name);
		need.player_level = info.summoner_level;
		need.rank_datas = raw_lcu_api::getInstance().puuid_get_rank_datas(info.puuid);

		std::string out = i.puuid + "    " + std::to_string(i.summoner_id) + "    " + std::to_string(i.cell_id) + "  \n";
		OutputDebugStringA(out.c_str());

		auto top_ = raw_lcu_api::getInstance().get_top_champions(std::to_string(i.summoner_id), 6);
		if (top_.masteries.size() < 6)
		{
			for (int i = top_.masteries.size(); i < 6; i++) {
				LCU_JSON_RESPONSE::Mastery	t;
				t.champion_id = 0;
				top_.masteries.push_back(t);
			}
		}

		{
			need.top1 = top_.masteries.at(0).champion_id;
			need.top2 = top_.masteries.at(1).champion_id;
			need.top3 = top_.masteries.at(2).champion_id;
			need.top4 = top_.masteries.at(3).champion_id;
			need.top5 = top_.masteries.at(4).champion_id;
			need.top6 = top_.masteries.at(5).champion_id;
		}

		Post2UI([this, need] {
			Match_Items* item = new Match_Items;
			ui::GlobalManager::FillBoxWithCache(item, L"basic/matchitem.xml");
			item->InitSubControls(need);
			_list->Add(item);
			});
	}
	_list->AttachBubbledEvent(ui::kEventAll, nbase::Bind(&Pop_form::init_set_listen_controls, this, std::placeholders::_1));
	player_info = info;
}

void Pop_form::Recv_PosInfo(ui::UiRect pos)
{
	Post2UI([this,pos]() {
		this->SetPos(pos, false, SWP_NOSIZE, HWND_TOP, false);
		});
}

LRESULT Pop_form::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return __super::HandleMessage(uMsg, wParam, lParam);
}

bool	Pop_form::init_set_listen_controls(ui::EventArgs* args) {
	if (args->chKey == VK_TAB)
	{
		std::lock_guard<std::mutex> lock(details_wind_mtx);
		if (details_wind == NULL)
		{
			details_wind = new Details_Pop();
			details_wind->Create(this->GetHWND(), NULL, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
			details_wind->init_info(player_info);
		}
		else if (details_wind->details_isinvalid() == false)
		{	
			details_wind = new Details_Pop();
			details_wind->Create(this->GetHWND(), NULL, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
			details_wind->init_info(player_info);
		}
		details_wind->ShowWindow();
		details_wind->reset_info();
		details_wind->CenterWindow();
	}
	return	true;
}