#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	<Windows.h>
#include	"lcu_structs.hpp"
#include	"lcu_riot/struct.h"
#include	"lcu_riot/config.h"
#include	"champ_ui.h"
#include	"VirtualTileBox.h"
#include	"provider.h"
#include	"MiscThread.h"

class Lock_Champs_Select : public ui::WindowImplBase
{
public:

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	virtual void InitWindow() override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static const std::wstring kClassName;
	void	init_datas(std::vector<CHAMP_INFO> champs, Settings_Config* config);
private:
	bool	onBtnClick(ui::EventArgs* args);
	bool	OnSelected(ui::EventArgs* args);

	VirtualTileBox* _list;
	Provider* m_DataProvider;

	ui::Button* _save;
	ui::Button* _calcel;
	ui::Label* _ui_select_champname;
	std::vector<CHAMP_INFO>	_champs;
	Settings_Config* _config;

	std::mutex	_data_mtx;
	int	select_champ_id = 0;
};