#include "basic_form.h"
#pragma comment(lib, "Urlmon.lib") 
#include	<Shlobj.h>
#include	"global_.h"

const std::wstring BasicForm::kClassName = L"Basic";
BasicForm::BasicForm()
{
	loop_getgameuipos_thread_.reset(new MiscThread(kThreadLoopGetGameUiPosThread, "循环获取LOL的界面位置线程"));
	loop_getgameuipos_thread_->Start();
	//尝试初始化config_配置文件
	OutputDebugStringA("config_Settings_enable\n");
	if(!config_settings.load_config_file())
		config_settings.save_current_config();
	load_configs();
}

BasicForm::~BasicForm()
{
	loop_getgameuipos_thread_.reset(nullptr);
	loop_getgameuipos_thread_->Stop();
}

std::wstring BasicForm::GetSkinFolder()
{
	return L"basic";
}

std::wstring BasicForm::GetSkinFile()
{
	return L"basic.xml";
}

std::wstring BasicForm::GetWindowClassName() const
{
	return kClassName;
}

void BasicForm::InitWindow()
{
	init_all_controls();
	init_set_listen_controls();

	StdClosure	wait_game_start = [this]() {
		do
		{
			std::string result;
			while (result.find("token") == std::string::npos) {
				result = GetProcessCommandLine("wmic process where caption='LeagueClientUx.exe' get commandline");
				if (result.find("token") == std::string::npos) {
					Sleep(2000);
					continue;
				}
			}
			raw_lcu_api::getInstance().set_game_commandline(result);
			Sleep(5000);
			if (raw_lcu_api::getInstance().init() == false) {
				Sleep(5000);
				continue;
			}
			nbase::ThreadManager::PostDelayedTask(kThreadNetwork, std::bind(&BasicForm::Receive_Datas2, this, raw_lcu_api::getInstance().get_summoner_datas()), nbase::TimeDelta::FromMilliseconds(1000));
			nbase::ThreadManager::PostDelayedTask(kThreadNetwork, std::bind(&BasicForm::Receive_Datas3, this, raw_lcu_api::getInstance().puuid_get_rank_datas()), nbase::TimeDelta::FromMilliseconds(1000));
			nbase::ThreadManager::PostDelayedTask(kThreadNetwork, std::bind(&BasicForm::Receive_Datas4, this, raw_lcu_api::getInstance().get_owner_champions()), nbase::TimeDelta::FromMilliseconds(1000));
			GAME_STATUS		forg_game_status=GAME_STATUS::Error;
			GAME_STATUS		game_Status = raw_lcu_api::getInstance().get_game_status();
			while (game_Status != GAME_STATUS::Error) {
				game_Status = raw_lcu_api::getInstance().get_game_status();
				if (game_Status!=forg_game_status)
				{
					nbase::ThreadManager::PostTask(kThreadNetwork, nbase::Bind(&BasicForm::Receive_Datas1, this, game_Status));
					forg_game_status = game_Status;
				}
				Sleep(500);
			}
		} while (true);
	};
	nbase::ThreadManager::PostTask(kThreadLoopGetGameStatusThread, ToWeakCallback(wait_game_start));
}

LRESULT BasicForm::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PostQuitMessage(0L);
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

LRESULT BasicForm::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (uMsg == WM_USER && wParam == TRAY_ICON_ID) {
		return OnTrayIcon(uMsg, wParam, lParam, false);
	}
	if (uMsg == myMessage) {
		nbase::ThreadManager::PostDelayedTask(kThreadMain, [this]() {
			this->ActiveWindow();
			::SetForegroundWindow(m_hWnd);
			::BringWindowToTop(m_hWnd);
			}, nbase::TimeDelta::FromSeconds(0));
		//删除托盘图标
		Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
		//显示主窗口
		ShowWindow(SW_SHOWNORMAL);
	}
	return __super::HandleMessage(uMsg, wParam, lParam);
}

bool	BasicForm::OnSelected(ui::EventArgs* args) {
	if (args->Type == ui::kEventSelect || args->Type == ui::kEventUnSelect)
	{
		update_select_status();	//更新选择状态
		ui::CheckBox* c = static_cast<ui::CheckBox*>((args->pSender));
		auto	str = c->GetText();
		add_str_status(str, c->IsSelected());
		c->SetText(str);
	}
	return	true;
}

void	BasicForm::update_select_status() {
	ui_datas._ui_accept_status = _ui_accept->IsSelected();
	ui_datas._ui_nextgame_status = _ui_nextgame->IsSelected();
	ui_datas._ui_searchqueue_status = _ui_searchqueue->IsSelected();
	ui_datas._ui_lockchampion_status = _ui_lockchampion->IsSelected();
	ui_datas._ui_matching_helper = _ui_player_helper->IsSelected();
	ui_datas._ui_aram_helper = _ui_aram_helper->IsSelected();
}

void	BasicForm::init_all_controls() {
	tools_area = static_cast<ui::VBox*>(FindControl(L"tools_area"));
	_ui_accept = static_cast<ui::CheckBox*>(FindControl(L"_ui_accept"));
	_ui_nextgame = static_cast<ui::CheckBox*>(FindControl(L"_ui_nextgame"));
	_ui_searchqueue = static_cast<ui::CheckBox*>(FindControl(L"_ui_searchqueue"));
	_ui_lockchampion = static_cast<ui::CheckBox*>(FindControl(L"_ui_lockchampion"));
	_ui_selectchampion = static_cast<ui::Label*>(FindControl(L"_ui_champion"));
	_champion_list = dynamic_cast<ui::ListBox*>(FindControl(L"list"));
	_ui_search_champion = dynamic_cast<ui::RichEdit*>(FindControl(L"edit_total"));
	summoner_icon = dynamic_cast<ui::Control*>(FindControl(L"summoner-icon"));

	_ui_player_name = static_cast<ui::Label*>(FindControl(L"player_name"));
	_ui_player_level = static_cast<ui::Label*>(FindControl(L"player_level"));
	_ui_RANKED_SOLO_5x5 = static_cast<ui::Label*>(FindControl(L"RANKED_SOLO_5x5"));
	_ui_RANKED_FLEX_SR = static_cast<ui::Label*>(FindControl(L"RANKED_FLEX_SR"));
	_ui_RANKED_TFT = static_cast<ui::Label*>(FindControl(L"RANKED_TFT"));
	_ui_RANKED_TFT_TURBO = static_cast<ui::Label*>(FindControl(L"RANKED_TFT_TURBO"));
	_ui_RANKED_TFT_DOUBLE_UP = static_cast<ui::Label*>(FindControl(L"RANKED_TFT_DOUBLE_UP"));

	display_game_status = static_cast<ui::Label*>(FindControl(L"display_game_status"));
	_ui_close = dynamic_cast<ui::Button*>(FindControl(L"closebtn1"));

	_ui_player_helper = dynamic_cast<ui::CheckBox*>(FindControl(L"matching_helper"));
	_ui_aram_helper = dynamic_cast<ui::CheckBox*>(FindControl(L"aram_helper"));

#ifdef DYNAMIC_SKIN
	_download_R3nzdll = dynamic_cast<ui::Label*>(FindControl(L"_download_progress"));
	_dynamic_skin = dynamic_cast<ui::CheckBox*>(FindControl(L"_dynamic_skin"));
	nbase::ThreadManager::PostDelayedTask(kThreadNetwork, nbase::Bind(&BasicForm::Receive_check, this, dynamic_skin_host_my::check_Allow()), nbase::TimeDelta::FromSeconds(1));
#endif // DYNAMIC_SKIN
}

void	BasicForm::init_set_listen_controls() {
	_ui_accept->AttachSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	_ui_accept->AttachUnSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	auto	str = _ui_accept->GetText();
	add_str_status(str, _ui_accept->IsSelected());
	_ui_accept->SetText(str);
	//自动下一把
	_ui_nextgame->AttachSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	_ui_nextgame->AttachUnSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	str = _ui_nextgame->GetText();
	add_str_status(str, _ui_nextgame->IsSelected());
	_ui_nextgame->SetText(str);

	//自动搜索组队
	_ui_searchqueue->AttachSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	_ui_searchqueue->AttachUnSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	str = _ui_searchqueue->GetText();
	add_str_status(str, _ui_searchqueue->IsSelected());
	_ui_searchqueue->SetText(str);

	//锁定英雄
	_ui_lockchampion->AttachSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	_ui_lockchampion->AttachUnSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	str = _ui_lockchampion->GetText();
	add_str_status(str, _ui_lockchampion->IsSelected());
	_ui_lockchampion->SetText(str);

	//自动接收
	_ui_accept->AttachSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	_ui_accept->AttachUnSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	str = _ui_accept->GetText();
	add_str_status(str, _ui_accept->IsSelected());
	_ui_accept->SetText(str);

	//战绩助手
	_ui_player_helper->AttachSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	_ui_player_helper->AttachUnSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	str = _ui_player_helper->GetText();
	add_str_status(str, _ui_player_helper->IsSelected());
	_ui_player_helper->SetText(str);
	//大乱斗助手
	_ui_aram_helper->AttachSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	_ui_aram_helper->AttachUnSelect(std::bind(&BasicForm::OnSelected, this, std::placeholders::_1));
	str = _ui_aram_helper->GetText();
	add_str_status(str, _ui_aram_helper->IsSelected());
	_ui_aram_helper->SetText(str);

	_ui_close->AttachClick(std::bind(&BasicForm::OnUiMyClose, this, std::placeholders::_1));

#ifdef DYNAMIC_SKIN
	str = L"风险功能:动态换肤";
	add_str_status(str, _dynamic_skin->IsSelected());
	_dynamic_skin->SetText(str);
#endif // DYNAMIC_SKIN
}


std::wstring	BasicForm::add_str_status(std::wstring& content, bool status) {
	if (content.find(L"-") == std::wstring::npos) {
		content += L"-";
	}
	else {
		content = content.substr(0, content.find(L"-") + 1);
	}

	if (status)
	{
		content += L"开启";
	}
	else
		content += L"关闭";
	return	content;
}


void	BasicForm::Receive_Datas1(GAME_STATUS gamestatus) {
	StdClosure	closure = [this, gamestatus]() {
		/*if (gamestatus != GAME_STATUS::ChampSelect && ui_datas._ui_matching_helper && tools_windows != NULL)
		{
			std::lock_guard<std::mutex> lock(ui_still_alive_lock);
			still_alive = false;
			tools_windows->Close();
			tools_windows = NULL;
		}*/

		switch (gamestatus)
		{
		case GAME_STATUS::Error:
			display_game_status->SetText(L"等待游戏启动中");
			break;
		case GAME_STATUS::None:
			display_game_status->SetText(L"游戏大厅");
			break;
		case GAME_STATUS::Lobby:
			display_game_status->SetText(L"房间内");
			if (ui_datas._ui_searchqueue_status == true && tools_windows == NULL)
				raw_lcu_api::getInstance().search_queue();
			break;
		case GAME_STATUS::Matchmaking:
			display_game_status->SetText(L"匹配中");
			break;
		case GAME_STATUS::ReadyCheck:
			display_game_status->SetText(L"找到对局");
			if (ui_datas._ui_accept_status == true)
				raw_lcu_api::getInstance().accept_game();
			break;
		case GAME_STATUS::ChampSelect:
			display_game_status->SetText(L"选英雄中");
			if (ui_datas._ui_lockchampion_status && ui_datas._ui_champion_id != 0)
				raw_lcu_api::getInstance().lock_champions(ui_datas._ui_champion_id);
			if (ui_datas._ui_matching_helper)
			{//战绩助手
				open_player_helper_tools();
			}
			if (ui_datas._ui_aram_helper)
			{//大乱斗助手

			}
			break;
		case GAME_STATUS::InProgress:
			display_game_status->SetText(L"游戏中");
			break;
		case GAME_STATUS::PreEndOfGame:
			display_game_status->SetText(L"游戏即将结束");
			break;
		case GAME_STATUS::WaitingForStats:
			display_game_status->SetText(L"等待结算页面");
			break;
		case GAME_STATUS::EndOfGame:
			display_game_status->SetText(L"游戏结束");
			if (ui_datas._ui_nextgame_status == true)
				raw_lcu_api::getInstance().auto_next_game();
			break;
		case GAME_STATUS::Reconnect:
			display_game_status->SetText(L"等待重新连接");
			break;
		default:
			break;
		}
	};
	nbase::ThreadManager::PostTask(kThreadMain, ToWeakCallback(closure));//Global network request Thread
}

void	BasicForm::Receive_Datas2(LCU_JSON_RESPONSE::LolSummonerInfo info) {
	StdClosure	closure = [this, info]() {
		_ui_player_name->SetText(string2wstring(info.display_name));
		_ui_player_level->SetText(std::to_wstring(info.summoner_level));
		nbase::ThreadManager::PostTask(kThreadNetwork, nbase::Bind(&BasicForm::set_current_player_icon, this, GAME_RESOURCES::GAME_RES::getInstance().getIconsPath(GAME_RESOURCES::ICONS, info.profile_icon_id)));;//Global network request Thread
	};
	nbase::ThreadManager::PostTask(kThreadMain, ToWeakCallback(closure));//Global network request Thread
}

void	BasicForm::Receive_Datas3(LCU_JSON_RESPONSE::LolRankedData rank_Datas) {
	StdClosure	closure = [this, rank_Datas]() {
		auto	SOLO_5x5 = rank_Datas.queue_map.ranked_solo_5_x5;
		if (!SOLO_5x5.tier.empty()) {
			std::wstring	n = StringToWString(SOLO_5x5.tier) + L" " + StringToWString(SOLO_5x5.division) + L" " + std::to_wstring(SOLO_5x5.league_points);
			_ui_RANKED_SOLO_5x5->SetText(n);
		}
		auto	FLEX_SR = rank_Datas.queue_map.ranked_flex_sr;
		if (!FLEX_SR.tier.empty()) {
			std::wstring	n = StringToWString(FLEX_SR.tier) + L" " + StringToWString(FLEX_SR.division) + L" " + std::to_wstring(FLEX_SR.league_points);
			_ui_RANKED_FLEX_SR->SetText(n);
		}
		auto	TFT = rank_Datas.queue_map.ranked_tft;
		if (!TFT.tier.empty()) {
			std::wstring	n = StringToWString(TFT.tier) + L" " + StringToWString(TFT.division) + L" " + std::to_wstring(TFT.league_points);
			_ui_RANKED_TFT->SetText(n);
		}
		auto	TFT_TURBO = rank_Datas.queue_map.ranked_tft_turbo;
		if (!TFT_TURBO.tier.empty()) {
			std::wstring	n = StringToWString(TFT_TURBO.tier) + L" " + StringToWString(TFT_TURBO.division) + L" " + std::to_wstring(TFT_TURBO.league_points);
			_ui_RANKED_TFT_TURBO->SetText(n);
		}
		auto	TFT_DOUBLE_UP = rank_Datas.queue_map.ranked_tft_double_up;
		if (!TFT_DOUBLE_UP.tier.empty()) {
			std::wstring	n = StringToWString(TFT_DOUBLE_UP.tier) + L" " + StringToWString(TFT_DOUBLE_UP.division) + L" " + std::to_wstring(TFT_DOUBLE_UP.league_points);
			_ui_RANKED_SOLO_5x5->SetText(n);
		}
	};
	nbase::ThreadManager::PostTask(kThreadMain, ToWeakCallback(closure));//Global network request Thread
}

void	BasicForm::Receive_Datas4(LCU_JSON_RESPONSE::LolOwnerChampions owner_datas) {
	StdClosure	closure = [this, owner_datas]() {
		_champion_list->RemoveAll();
		//if (owner_datas.size() == 0) {
		//	Item* item = new Item;
		//	ui::GlobalManager::FillBoxWithCache(item, L"basic/item.xml");
		//	std::wstring title = L"没有拥有任何英雄";
		//	item->InitSubControls(title);
		//	_champion_list->Add(item);
		//}
		//else {
		//	have_champions_copy = owner_datas;
		//	for (const auto& i : have_champions_copy)
		//	{
		//		Item* item = new Item;
		//		ui::GlobalManager::FillBoxWithCache(item, L"basic/item.xml");
		//		std::wstring title = StringToWString(i.name) + L"-" + StringToWString(i.title);
		//		item->InitSubControls(title);
		//		item->AttachDoubleClick(nbase::Bind(&BasicForm::OnSelectedChampion, this, std::placeholders::_1));
		//		_champion_list->Add(item);
		//	}
		//}
	};
	nbase::ThreadManager::PostTask(kThreadMain, ToWeakCallback(closure));//Global network request Thread
}



void	BasicForm::set_current_player_icon(std::string icon_path) {
	StdClosure	closure = [this, icon_path]() {
		summoner_icon->SetBkImage(string2wstring(icon_path));
	};
	nbase::ThreadManager::PostTask(kThreadMain, ToWeakCallback(closure));//Global network request Thread
}

#ifdef DYNAMIC_SKIN
void	BasicForm::Receive_check(bool	check) {
	if (check == false)
	{
		_dynamic_skin->SetText(L"O No 隐藏功能暂时不开启");
	}
	else
	{
		std::wstring str = L"风险功能:动态换肤";
		add_str_status(str, _dynamic_skin->IsSelected());
		_dynamic_skin->SetText(str);
		_dynamic_skin->AttachSelect(std::bind(&BasicForm::Open_dynamic_skin, this, std::placeholders::_1));
		_dynamic_skin->AttachUnSelect(std::bind(&BasicForm::Open_dynamic_skin, this, std::placeholders::_1));
		nbase::ThreadManager::PostDelayedTask(kThreadNetwork, nbase::Bind(&BasicForm::Receive_download_dll_path, this, dynamic_skin_host_my::download_dll()), nbase::TimeDelta::FromSeconds(1));
	}//风险功能:动态换肤
}

bool	BasicForm::Open_dynamic_skin(ui::EventArgs* args) {
	if (args->Type == ui::kEventSelect || args->Type == ui::kEventUnSelect)
	{
		if (!origin_dll_path.empty())
		{
			if (args->Type == ui::kEventSelect) {
				if (!dynamic_skin_host_my::open_dynamic_skin(origin_dll_path)) {
					_dynamic_skin->Selected(false, false);
				}
				else
				{
					const TCHAR szOperation[] = _T("open");
					const TCHAR szAddress[] = _T("https://note.youdao.com/s/P8BuWoMJ");
					HINSTANCE hRslt = ShellExecute(NULL, szOperation,
						szAddress, NULL, NULL, SW_SHOWNORMAL);
				}
			}

			if (args->Type == ui::kEventUnSelect) {
				if (!dynamic_skin_host_my::close_dynamic_skin()) {
					_dynamic_skin->Selected(true, false);
				}
			}
		}
		else {
			_dynamic_skin->Selected(false, false);
		}
		ui::CheckBox* c = static_cast<ui::CheckBox*>((args->pSender));
		auto	str = c->GetText();
		add_str_status(str, c->IsSelected());
		c->SetText(str);
	}
	return	true;
}
void	BasicForm::Receive_download_dll_path(std::string	save_path) {
	if (!save_path.empty())
	{
		origin_dll_path = save_path;
		_download_R3nzdll->SetText(L"插件下载完毕");
	}
	else
		_download_R3nzdll->SetText(L"插件下载失败");
}
#endif // DYNAMIC_SKIN


//RANK_LEVEL_ITEM BasicForm::get(const RANK_LEVEL& vec, std::string en)const
//{
//	RANK_LEVEL_ITEM	b;
//	for (const auto it : vec) {
//		if (it.en._Equal(en))
//			return	it;
//	}
//	return	b;
//}


bool	BasicForm::OnUiMyClose(ui::EventArgs* args) {
	//SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	::ShowWindow(m_hWnd, SW_HIDE);
	AddTrayIcon();
	return true;
}


void BasicForm::AddTrayIcon() {
	// 加载托盘图标
	ZeroMemory(&m_trayIcon, sizeof(NOTIFYICONDATA));
	m_trayIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_trayIcon.hWnd = m_hWnd;
	m_trayIcon.uID = TRAY_ICON_ID;
	m_trayIcon.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
	m_trayIcon.uCallbackMessage = WM_USER;
	m_trayIcon.hIcon = ::LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON1));
	lstrcpy(m_trayIcon.szTip, L"LOL对局助手");
	Shell_NotifyIcon(NIM_ADD, &m_trayIcon);
	ShowWindow(SW_HIDE);
}

void	BasicForm::load_configs() {
	ui_datas._ui_accept_status = config_settings.configs._auto_accept;
	ui_datas._ui_nextgame_status = config_settings.configs._auto_next;
	ui_datas._ui_searchqueue_status = config_settings.configs._search_queue;
	ui_datas._ui_lockchampion_status = config_settings.configs._lock_champion;
	ui_datas._ui_lockchampion_status = config_settings.configs._matching_helper;
	ui_datas._ui_aram_helper = config_settings.configs._aram_helper;
}	//从全局变量中加载或保存配置
void	BasicForm::save_configs() {
	config_settings.configs._auto_accept = ui_datas._ui_accept_status;
	config_settings.configs._auto_next = ui_datas._ui_nextgame_status;
	config_settings.configs._search_queue = ui_datas._ui_searchqueue_status;
	config_settings.configs._lock_champion = ui_datas._ui_lockchampion_status;
	config_settings.configs._matching_helper = ui_datas._ui_matching_helper;
	config_settings.configs._aram_helper = ui_datas._ui_aram_helper;
}


LRESULT BasicForm::OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled)
{
	//如果在图标中双击左键则还原
	if (lParam == WM_LBUTTONDBLCLK)		//WM_LBUTTONDBLCLK	WM_LBUTTONDOWN
	{
		nbase::ThreadManager::PostDelayedTask(kThreadMain, [this]() {//MainThread
			this->ActiveWindow();
			::SetForegroundWindow(m_hWnd);
			::BringWindowToTop(m_hWnd);
			}, nbase::TimeDelta::FromSeconds(0));
		//删除托盘图标
		Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
		//显示主窗口
		ShowWindow(SW_SHOWNORMAL);
	}
	//如果在图标中单击右键则弹出声明式菜单
	if (lParam == WM_RBUTTONDOWN)
	{
		//获取鼠标坐标
		POINT pt; GetCursorPos(&pt);
		//右击后点别地可以清除“右击出来的菜单”
		SetForegroundWindow(m_hWnd);
		//托盘菜单    win32程序使用的是HMENU，如果是MFC程序可以使用CMenu
		HMENU hMenu;
		//生成托盘菜单
		hMenu = CreatePopupMenu();
		//添加菜单,关键在于设置的一个标识符  WM_ONCLOSE 点击后会用到
		AppendMenu(hMenu, MF_STRING, 0, _T("q群:758843151-复制到剪切板"));
		AppendMenu(hMenu, MF_SEPARATOR, 0, 0);
		AppendMenu(hMenu, MF_STRING, MENUBAR_ONCLOSE, _T("Exit"));
		//弹出菜单,并把用户所选菜单项的标识符返回
		int cmd = TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, NULL, m_hWnd, NULL);
		//如果标识符是WM_ONCLOSE则关闭
		if (cmd == MENUBAR_ONCLOSE)
		{
			m_trayIcon.hIcon = NULL;
			Shell_NotifyIcon(NIM_DELETE, &m_trayIcon);
#ifdef DYNAMIC_SKIN
			dynamic_skin_host_my::close_dynamic_skin();
#endif // DYNAMIC_SKIN
			//同时更改配置
			save_configs();
			exit(0);
		}
		else if (cmd == MENUBAR_COPYQQ) {
			CopyTextToClipboard(L"758843151");
		}
	}
	bHandled = true;
	return 0;
}

void	BasicForm::open_player_helper_tools() {
	if (tools_windows == NULL)
	{
		std::lock_guard<std::mutex> lock(ui_still_alive_lock);
		still_alive = true;
		tools_windows = new Pop_form();
		tools_windows->Create(this->GetHWND(), NULL, WS_EX_OVERLAPPEDWINDOW & ~WS_EX_APPWINDOW, 0);
		tools_windows->ShowModalFake(this->GetHWND(), true);
		tools_windows->ShowWindow();
		StdClosure	setUiPos = [this]() {
			while (still_alive) {
				// 获取窗口位置信息
				HWND hwnd = FindWindowA(NULL, "League of Legends");
				RECT rect;
				GetWindowRect(hwnd, &rect);
				ui::UiRect	pos;
				pos.top = rect.top;
				pos.left = rect.right;
				tools_windows->Recv_PosInfo(pos);
				Sleep(100);
			}
		};
		nbase::ThreadManager::PostTask(kThreadLoopGetGameUiPosThread, setUiPos);
	}
}