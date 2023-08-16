#include "lock_champs_ui.h"


const std::wstring Lock_Champs_Select::kClassName = L"basic";

std::wstring Lock_Champs_Select::GetSkinFolder()
{
	return L"basic";
}

std::wstring Lock_Champs_Select::GetSkinFile()
{
	return L"selectlockchamp.xml";
}

std::wstring Lock_Champs_Select::GetWindowClassName() const
{
	return kClassName;
}

void Lock_Champs_Select::InitWindow()
{
	_list = dynamic_cast<VirtualTileBox*>(FindControl(L"list"));
	_save = static_cast<ui::Button*>(FindControl(L"select_yes"));
	_calcel = static_cast<ui::Button*>(FindControl(L"select_no"));
	_ui_select_champname = static_cast<ui::Label * > (FindControl(L"_ui_select_champname"));
	// 设置提供者
	m_DataProvider = new Provider;
	_list->SetDataProvider(m_DataProvider);
	std::lock_guard<std::mutex> lock(_data_mtx);

	m_DataProvider->SetDatas(_champs);

	_save->AttachClick(std::bind(&Lock_Champs_Select::onBtnClick, this, std::placeholders::_1));
	_calcel->AttachClick(std::bind(&Lock_Champs_Select::onBtnClick, this, std::placeholders::_1));
	_list->AttachSelect(std::bind(&Lock_Champs_Select::OnSelected, this, std::placeholders::_1));
	_list->SetForceArrange(true);
}
bool Lock_Champs_Select::OnSelected(ui::EventArgs* args) {
	std::vector<int> all;
	_list->GetDisplayCollection(all);
	if (all.size()<args->wParam)
	{
		return	true;
	}
	select_champ_id = _champs.at(all.at(args->wParam)).champ_id;
	std::string champ_name;
	for (const auto c : _champs) {
		if (c.champ_id==select_champ_id)
		{
			champ_name = c.champ_name;
		}
	}
	_ui_select_champname->SetText(string2wstring(champ_name));
	return true;
}

void Lock_Champs_Select::init_datas(std::vector<CHAMP_INFO> champs, Settings_Config* config)
{
	std::lock_guard<std::mutex> lock(_data_mtx);
	_config = config;

	if (champs.size()==0)
	{
		return;
	}
	_champs = champs;
	_champs.erase(_champs.begin());
}

bool Lock_Champs_Select::onBtnClick(ui::EventArgs* args)
{
	ULONG64	btn = (ULONG64)args->pSender;
	if (btn==(ULONG64)_calcel)
	{
		this->Close(1);
	}
	else if (btn == (ULONG64)_save) {
		_config->lock_champid= select_champ_id;
		this->Close(1);
	}
	return	true;
}

LRESULT Lock_Champs_Select::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}