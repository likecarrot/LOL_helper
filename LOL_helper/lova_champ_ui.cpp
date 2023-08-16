#include "lova_champ_ui.h"


const std::wstring Lova_Champ_Ui::kClassName = L"basic";

std::wstring Lova_Champ_Ui::GetSkinFolder()
{
	return L"basic";
}

std::wstring Lova_Champ_Ui::GetSkinFile()
{
	return L"selectlovachamps.xml";
}

std::wstring Lova_Champ_Ui::GetWindowClassName() const
{
	return kClassName;
}

void Lova_Champ_Ui::InitWindow()
{
	
	_save = static_cast<ui::Button*>(FindControl(L"select_yes"));
	_calcel = static_cast<ui::Button*>(FindControl(L"select_no"));
	_lova_add = static_cast<ui::Button*>(FindControl(L"_lova_add"));
	_check_champ = static_cast<ui::RichEdit*>(FindControl(L"_input_champ_name"));
	_display_champ = static_cast<ui::RichEdit*>(FindControl(L"_lova_champ_name"));
	_champ_score = static_cast<ui::RichEdit*>(FindControl(L"_lova_score"));
	_output_lova_champs = static_cast<ui::RichEdit*>(FindControl(L"_output_lova_champs"));
	_btncheck_champ = static_cast<ui::Button*>(FindControl(L"_check_champ"));

	_save->AttachClick(std::bind(&Lova_Champ_Ui::onBtnClick, this, std::placeholders::_1));
	_calcel->AttachClick(std::bind(&Lova_Champ_Ui::onBtnClick, this, std::placeholders::_1));
	_lova_add->AttachClick(std::bind(&Lova_Champ_Ui::onBtnClick, this, std::placeholders::_1));
	_btncheck_champ->AttachClick(std::bind(&Lova_Champ_Ui::Onbtn_checkchamps, this, std::placeholders::_1));

	_champ_score->SetText(L"0");
}

void Lova_Champ_Ui::_list1Remove_list2Add(int champid, int score)
{
	for (auto it = _champs1.begin(); it != _champs1.end();)
	{
		if (it->champ_id == champid)
		{
			if (it->champ_name.empty())
			{
				it->champ_name = GetChampnameBychampid(it->champ_id);
			}
			_champs2.push_back(*it);
			it = _champs1.erase(it);
		}
		else
		{
			++it;
		}
	}
}

void Lova_Champ_Ui::_list2Remove_list1Add(int champid)
{
	for (auto it = _champs2.begin(); it != _champs2.end();)
	{
		if (it->champ_id == champid)
		{
			if (it->champ_name.empty())
			{
				it->champ_name = GetChampnameBychampid(it->champ_id);
			}
			it->lova_score = 0;
			_champs1.push_back(*it);
			it = _champs2.erase(it);
		}
		else
		{
			++it;
		}
	}
}

int Lova_Champ_Ui::GetChampidByChampname(std::string champname)
{
	if (_all_champs.empty())
	{
		return 0;
	}
	for (const auto in : _all_champs) {
		if ((in.champ_name)._Equal(champname))
		{
			return	in.champ_id;
		}
	}
	return 0;
}

std::string Lova_Champ_Ui::GetChampnameBychampid(int champid)
{
	if (_all_champs.empty())
	{
		return	std::string();
	}
	for (const auto in : _all_champs) {
		if (in.champ_id== champid)
		{
			return	in.champ_name;
		}
	}
	return	std::string();
}

void Lova_Champ_Ui::init_datas(std::vector<CHAMP_INFO> all_champs,Settings_Config* config)
{
	_all_champs = all_champs;
	_config = config;
	if (_all_champs.size() == 0)
	{
		return;
	}
	_all_champs.erase(_all_champs.begin());

	for (const auto in : config->lova_champlist)
	{
		CHAMP_INFO	inf;
		inf.champ_id = in.first;
		inf.lova_score = in.second;
		_champs2.push_back(inf);
	}
}

bool Lova_Champ_Ui::onBtnClick(ui::EventArgs* args)
{
	ULONG64	btn = (ULONG64)args->pSender;
	if (btn == (ULONG64)_calcel)
	{
		this->Close(1);
	}
	else if (btn == (ULONG64)_save) {
		for (const auto in : _champs1) {
			if (in.lova_score==0)
			{
				_config->lova_champlist.erase(std::remove_if(_config->lova_champlist.begin(), _config->lova_champlist.end(), [in](const std::pair<int, int>& p) {
					return p.first == in.champ_id;
					}), _config->lova_champlist.end());
			}else
				_config->lova_champlist.emplace_back(in.champ_id,in.lova_score);
		}
		_config->lova_champlist.erase(std::unique(_config->lova_champlist.begin(), _config->lova_champlist.end(), [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
			return p1.first == p2.first;
			}), _config->lova_champlist.end());//先去一下重
		this->Close(1);
	}
	else if (btn == (ULONG64)_lova_add) {
		int champ_id = GetChampidByChampname(wstring_to_utf8(_display_champ->GetText()));
		int	score= _wtoi(_champ_score->GetText().c_str());
		if (champ_id!=0)
		{
			CHAMP_INFO	info;
			info.champ_id = champ_id;
			info.champ_name = wstring2string(_display_champ->GetText());
			info.lova_score = score;
			_champs1.push_back(info);
			std::wstring content = _output_lova_champs->GetText();
			content.append(_display_champ->GetText() + L":" + std::to_wstring(score) + L"\n");
			_output_lova_champs->SetText(content);
		}
	}
	return	true;
}
bool	Lova_Champ_Ui::Onbtn_checkchamps(ui::EventArgs* args) {
	std::wstring	name = _check_champ->GetText();
	if (name._Equal(_last_content)&& !have_champions_copy_sort.empty())
	{
		_last_index++;
		_display_champ->SetText(string2wstring(have_champions_copy_sort.at(_last_index%have_champions_copy_sort.size()).champ_name));
	}
	else {
		_last_index = 0;
		have_champions_copy_sort.clear();
		for (auto c : _all_champs) {
			std::wstring	cn = string2wstring(c.champ_name);
			if (cn.find(name) != std::string::npos)
				have_champions_copy_sort.push_back(c);
		}
		if (!have_champions_copy_sort.empty())
		{
			_display_champ->SetText(string2wstring(have_champions_copy_sort.cbegin()->champ_name));
		}
		_last_content = name;
	}
	
	
	return	true;
}

bool	Lova_Champ_Ui::onTextChanged(ui::EventArgs* args) {
	if (!have_champions_copy_sort.empty())
	{
		_display_champ->SetText(string2wstring(have_champions_copy_sort.cbegin()->champ_name));
	}
	return	true;
}

bool	Lova_Champ_Ui::onTextButtonUp(ui::EventArgs* args) {
	return	true;
}
LRESULT Lova_Champ_Ui::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::OnClose(uMsg, wParam, lParam, bHandled);
}