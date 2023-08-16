#include "helper_setting.h"


const std::wstring Helper_Setting::kClassName = L"basic";
Helper_Setting::Helper_Setting() : all_champs()
{

}

std::wstring Helper_Setting::GetSkinFolder()
{
	return L"basic";
}

std::wstring Helper_Setting::GetSkinFile()
{
	return L"settingsui.xml";
}

std::wstring Helper_Setting::GetWindowClassName() const
{
	return kClassName;
}

void Helper_Setting::InitWindow()
{
	initAllSubControls();
	init_ui_for_config();
}
void Helper_Setting::AddSettingsConfig(Settings_Config Config)
{
	_config = Config;
}
LRESULT Helper_Setting::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}

void Helper_Setting::initAllSubControls()
{
	_ui_lockChamp = static_cast<ui::Label*>(FindControl(L"_ui_lockchamp"));
	_ui_lockAotherChamp = static_cast<ui::Button*>(FindControl(L"_ui_lockaotherchamp"));
	_ui_autoLock = static_cast<ui::CheckBox*>(FindControl(L"_ui_autolock"));
	_ui_autoLockTimeout = static_cast<ui::RichEdit*>(FindControl(L"_ui_autolocktimeout"));
	_ui_acceptTimeout = dynamic_cast<ui::RichEdit*>(FindControl(L"_ui_accepttimeout"));
	_ui_arammaxtimeout= dynamic_cast<ui::RichEdit*>(FindControl(L"_ui_arammaxtimeout"));


	_ui_reroller1 = static_cast<ui::Option*>(FindControl(L"_reroller1"));
	_ui_reroller2 = static_cast<ui::Option*>(FindControl(L"_reroller2"));
	_ui_reroller3 = static_cast<ui::Option*>(FindControl(L"_reroller3"));
	_ui_lovaChampList = static_cast<ui::Combo*>(FindControl(L"_ui_lovachamplist"));
	_ui_lovaAotherChamp = static_cast<ui::Button*>(FindControl(L"_ui_lovaaotherchamp"));

	_ui_loadConfig = static_cast<ui::Button*>(FindControl(L"_ui_loadconfig"));
	_ui_outputConfig = static_cast<ui::Button*>(FindControl(L"_ui_outputconfig"));
	_ui_saveConfig = static_cast<ui::Button*>(FindControl(L"_ui_saveconfig"));
	_ui_calcelConfig = dynamic_cast<ui::Button*>(FindControl(L"_ui_cancal"));

	_ui_autoLock->AttachSelect(std::bind(&Helper_Setting::OnControlSelected, this, std::placeholders::_1));
	_ui_autoLock->AttachUnSelect(std::bind(&Helper_Setting::OnControlSelected, this, std::placeholders::_1));

	_ui_autoLockTimeout->AttachTextChange(std::bind(&Helper_Setting::OnControlSelected, this, std::placeholders::_1));
	_ui_acceptTimeout->AttachTextChange(std::bind(&Helper_Setting::OnControlSelected, this, std::placeholders::_1));
	_ui_arammaxtimeout->AttachTextChange(std::bind(&Helper_Setting::OnControlSelected, this, std::placeholders::_1));

	_ui_reroller1->AttachSelect(std::bind(&Helper_Setting::isWhichReroller, this, std::placeholders::_1));
	_ui_reroller2->AttachSelect(std::bind(&Helper_Setting::isWhichReroller, this, std::placeholders::_1));
	_ui_reroller3->AttachSelect(std::bind(&Helper_Setting::isWhichReroller, this, std::placeholders::_1));

	_ui_lockAotherChamp->AttachClick(std::bind(&Helper_Setting::OnBtnClicked, this, std::placeholders::_1));
	_ui_lovaAotherChamp->AttachClick(std::bind(&Helper_Setting::OnBtnClicked, this, std::placeholders::_1));
	_ui_loadConfig->AttachClick(std::bind(&Helper_Setting::OnBtnClicked, this, std::placeholders::_1));
	_ui_outputConfig->AttachClick(std::bind(&Helper_Setting::OnBtnClicked, this, std::placeholders::_1));
	_ui_saveConfig->AttachClick(std::bind(&Helper_Setting::OnBtnClicked, this, std::placeholders::_1));
	_ui_calcelConfig->AttachClick(std::bind(&Helper_Setting::OnBtnClicked, this, std::placeholders::_1));

	if (all_champs.empty())
	{
		auto c = raw_lcu_api::getInstance().get_all_champions();
		for (auto i : c) {
			CHAMP_INFO	champ;
			champ.champ_id = i.id;
			champ.champ_name = i.name;
			all_champs.push_back(champ);
		}
	}
	foreg_get_configfiledatas();
}

bool Helper_Setting::isWhichReroller(ui::EventArgs* Args)
{
	if (_ui_reroller2->IsSelected())
		_config.reroll = 2;
	else if (_ui_reroller3->IsSelected())
		_config.reroll = 3;
	else
		_config.reroll = 1;
	return	true;
}
void Helper_Setting::saveToConfigJson()
{
	config_settings.configs._classic_config.auto_lock = _config.autolock;
	config_settings.configs._classic_config.champ_id = getIdByChampName(_config.lock_champname);
	config_settings.configs.accept_timeout = _config.accept_timeout;
	config_settings.configs._classic_config.lockchamp_timeout = _config.autolock_timeout;
	config_settings.configs._aram_config.Use_Reroller = _config.reroll;
	config_settings.configs._aram_config.love_champs = _config.lova_champlist;
}
bool Helper_Setting::OnControlSelected(ui::EventArgs* Args)
{
	int accept_timeout = 0;
	if (!_ui_acceptTimeout->GetText().empty())
	{
		accept_timeout= _wtoi(_ui_acceptTimeout->GetText().c_str());
		if (accept_timeout>=10)
		{
			accept_timeout = 10;
		}
		if (accept_timeout<=0)
		{
			accept_timeout = 0;
		}
	}
	_config.accept_timeout = accept_timeout;
	
	int lockchamp_timeout = 0;
	if (!_ui_autoLockTimeout->GetText().empty())
	{
		lockchamp_timeout = _wtoi(_ui_autoLockTimeout->GetText().c_str());
		if (lockchamp_timeout >= 30)
		{
			lockchamp_timeout = 30;
		}
		if (lockchamp_timeout <= 0)
		{
			lockchamp_timeout = 0;
		}
	}
	_config.autolock_timeout = lockchamp_timeout;

	int	max_timeout = 0;
	if (!_ui_arammaxtimeout->GetText().empty())
	{
		max_timeout = _wtoi(_ui_arammaxtimeout->GetText().c_str());
		if (max_timeout >= 30)
		{
			max_timeout = 30;
		}
		if (max_timeout <= 0)
		{
			max_timeout = 0;
		}
	}
	_config.aram_maxtimeout = max_timeout;


	_config.autolock = _ui_autoLock->IsSelected();
	return true;
}
bool Helper_Setting::child_close_notify(ui::EventArgs* args)
{
	if (all_champs.empty())
	{
		return	true;
	}
	
	_ui_lockChamp->SetText(string2wstring(getNameByChampId(_config.lock_champid)));
	_ui_lovaChampList->RemoveAll();
	for (auto c : _config.lova_champlist)
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(20);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(string2wstring(getNameByChampId(c.first))+L":"+ std::to_wstring(c.second));
		_ui_lovaChampList->Add(element);
	}
	if (_ui_lovaChampList->GetCount() != 0)
		_ui_lovaChampList->SelectItem(0);
	return	true;
}
bool Helper_Setting::OnBtnClicked(ui::EventArgs* args)
{
	ULONG64 c = (ULONG64)args->pSender;
	if (c== (ULONG64)_ui_lockAotherChamp)
	{
		Lock_Champs_Select* lock_ui = new Lock_Champs_Select;
		if (all_champs.empty())
		{
			auto c = raw_lcu_api::getInstance().get_all_champions();
			for (auto i : c) {
				CHAMP_INFO	champ;
				champ.champ_id = i.id;
				champ.champ_name = i.name;
				all_champs.push_back(champ);
			}
		}
		
		lock_ui->init_datas(this->all_champs,&this->_config);
		Post2UI([lock_ui,this]() {
			lock_ui->Create(this->GetHWND(), NULL, WS_EX_OVERLAPPEDWINDOW & ~WS_EX_APPWINDOW, 0);
			lock_ui->ShowModalFake(this->GetHWND(), true);
			lock_ui->ShowWindow();
			lock_ui->AttachWindowClose(std::bind(&Helper_Setting::child_close_notify, this, std::placeholders::_1));
			});
		
	}
	else if (c == (ULONG64)_ui_lovaAotherChamp) {
		Lova_Champ_Ui* lova_ui = new Lova_Champ_Ui;
		if (all_champs.empty())
		{
			auto c = raw_lcu_api::getInstance().get_all_champions();
			for (auto i : c) {
				CHAMP_INFO	champ;
				champ.champ_id = i.id;
				champ.champ_name = i.name;
				all_champs.push_back(champ);
			}
		}

		lova_ui->init_datas(this->all_champs, &this->_config);
		Post2UI([lova_ui, this]() {
			lova_ui->Create(this->GetHWND(), NULL, WS_EX_OVERLAPPEDWINDOW & ~WS_EX_APPWINDOW, 0);
			lova_ui->CenterWindow();
			lova_ui->ShowModalFake(this->GetHWND(), true);
			lova_ui->ShowWindow();
			lova_ui->AttachWindowClose(std::bind(&Helper_Setting::child_close_notify, this, std::placeholders::_1));
			});
	}
	else if (c == (ULONG64)_ui_loadConfig) {
		// 初始化公共对话框结构体
		OPENFILENAMEA ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = new CHAR[MAX_PATH];
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = ("JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0\0");
		ofn.lpstrDefExt = ("json");

		// 显示文件打开对话框
		if (GetOpenFileNameA(&ofn)) {
			std::string	path(ofn.lpstrFile);
			if (!path.empty())
			{
				config_settings.load_config_file(path);
				foreg_get_configfiledatas();
			}
		}
		delete[] ofn.lpstrFile;
	}
	else if (c == (ULONG64)_ui_outputConfig) {
		// 初始化公共对话框结构体
		OPENFILENAMEA ofn = { 0 };
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = new CHAR[MAX_PATH];
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrFilter = ("JSON Files (*.json)\0*.json\0All Files (*.*)\0*.*\0\0");
		ofn.lpstrDefExt = ("json");
		// 显示文件保存对话框
		if (GetSaveFileNameA(&ofn)) {
			std::string	path(ofn.lpstrFile);
			if (!path.empty())
			{
				config_settings.save_current_config(path);
			}
		}
		delete[] ofn.lpstrFile;
	}
	else if (c == (ULONG64)_ui_calcelConfig) {
		this->Close(1);
	}
	else if (c == (ULONG64)_ui_saveConfig) {
		config_settings.configs.accept_timeout = _config.accept_timeout;
		config_settings.configs._classic_config.lockchamp_timeout = _config.autolock_timeout;
		config_settings.configs._aram_config.Use_Reroller = _config.reroll;
		config_settings.configs._classic_config.champ_id = _config.lock_champid;
		config_settings.configs._classic_config.auto_lock = _config.autolock;
		config_settings.configs._aram_config.love_champs = _config.lova_champlist;
		config_settings.configs._aram_config.wait_lova_champs_max_sec = _config.aram_maxtimeout;//int n3
		config_settings.save_current_config();
		this->Close(1);
	}
	return true;
}



int Helper_Setting::getIdByChampName(std::string champ_name)
{
	if (all_champs.empty())
	{
		return 0;
	}
	for (const auto in:all_champs)
	{
		if (in.champ_name._Equal(champ_name))
		{
			return	in.champ_id;
		}
	}
	return 0;
}
std::string Helper_Setting::getNameByChampId(int champ_id)
{
	if (all_champs.empty())
	{
		return std::string();
	}
	for (const auto in : all_champs)
	{
		if (in.champ_id==champ_id)
		{
			return	in.champ_name;
		}
	}
	return	"null";
}

void Helper_Setting::init_ui_for_config()
{
	if (all_champs.empty())
	{
		auto c = raw_lcu_api::getInstance().get_all_champions();
		for (auto i : c) {
			CHAMP_INFO	champ;
			champ.champ_id = i.id;
			champ.champ_name = i.name;
			all_champs.push_back(champ);
		}
	}

	_ui_lockChamp->SetText(string2wstring(getNameByChampId(config_settings.configs._classic_config.champ_id)));
	for (auto c : config_settings.configs._aram_config.love_champs)
	{
		ui::ListContainerElement* element = new ui::ListContainerElement;
		element->SetClass(L"listitem");
		element->SetFixedHeight(20);
		element->SetBkColor(L"white");
		element->SetTextPadding({ 6,0,6,0 });
		element->SetText(string2wstring(getNameByChampId(c.first)) + L":" + std::to_wstring(c.second));
		_ui_lovaChampList->Add(element);
	}
	if (_ui_lovaChampList->GetCount() != 0)
		_ui_lovaChampList->SelectItem(0);
	_ui_arammaxtimeout->SetText(std::to_wstring(config_settings.configs._aram_config.wait_lova_champs_max_sec));

	_ui_autoLock->Selected(config_settings.configs._classic_config.auto_lock);
	_ui_autoLockTimeout->SetText(std::to_wstring(config_settings.configs._classic_config.lockchamp_timeout));
	_ui_acceptTimeout->SetText(std::to_wstring(config_settings.configs.accept_timeout));
	switch (config_settings.configs._aram_config.Use_Reroller)
	{
	case 1:_ui_reroller1->Selected(true); break;
	case 2:_ui_reroller2->Selected(true); break;
	case 3:_ui_reroller3->Selected(true); break;
	default:
		_ui_reroller1->Selected(false);
		_ui_reroller2->Selected(false);
		_ui_reroller3->Selected(false);
		break;
	}
}

void Helper_Setting::foreg_get_configfiledatas()
{
	_config.autolock = config_settings.configs._classic_config.auto_lock;
	_config.lock_champname = getNameByChampId(config_settings.configs._classic_config.champ_id);
	_config.lock_champid = config_settings.configs._classic_config.champ_id;
	_config.accept_timeout = config_settings.configs.accept_timeout;
	_config.autolock_timeout = config_settings.configs._classic_config.lockchamp_timeout;
	_config.reroll = config_settings.configs._aram_config.Use_Reroller;
	_config.aram_maxtimeout= config_settings.configs._aram_config.wait_lova_champs_max_sec;
	_config.lova_champlist = config_settings.configs._aram_config.love_champs;
}
