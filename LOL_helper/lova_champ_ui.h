#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	<Windows.h>
#include	"lcu_structs.hpp"
#include	"lcu_riot/struct.h"
#include	"lcu_riot/config.h"
#include	"champ_ui.h"
#include	"MiscThread.h"
#include "lcu_riot/lcu_api.h"
#include	<algorithm>

class Lova_Champ_Ui : public ui::WindowImplBase
{
public:

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	virtual void InitWindow() override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static const std::wstring kClassName;
	void	init_datas(std::vector<CHAMP_INFO> all_champs, Settings_Config* config);
private:
	bool	onBtnClick(ui::EventArgs* args);
	bool	Onbtn_checkchamps(ui::EventArgs* args);
	bool	onTextChanged(ui::EventArgs* args);

	bool	onTextButtonUp(ui::EventArgs* args);
	void	_list1Remove_list2Add(int champid, int score);
	void	_list2Remove_list1Add(int champid);
	int	GetChampidByChampname(std::string champname);
	std::string		GetChampnameBychampid(int champid);
	ui::RichEdit* _check_champ;
	ui::RichEdit* _display_champ;
	ui::RichEdit* _champ_score;
	ui::RichEdit* _output_lova_champs;

	ui::Button* _btncheck_champ;
	ui::Button* _lova_add;
	ui::Button* _save;
	ui::Button* _calcel;
	std::vector<CHAMP_INFO>	_all_champs;
	std::vector<CHAMP_INFO>	_champs1;	//这个用来保存所有英雄 当然除了已经标记为喜欢的英雄了
	std::vector<CHAMP_INFO>	_champs2;	//这个用来保存所有喜欢英雄
	Settings_Config* _config;

	std::vector<CHAMP_INFO> have_champions_copy_sort;
	std::wstring	_last_content;	//用来标识是否输入内容没变
	std::int16_t	_last_index;
};