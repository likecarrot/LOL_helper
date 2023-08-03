#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/struct.h"
#include	"lcu_riot/lol_helper.h"
#include	"match_items.h"
#include	"MiscThread.h"
#include	"item.h"
#include	<Windows.h>
#include	"details_pop.h"

class Pop_form : public ui::WindowImplBase
{
public:

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	virtual void InitWindow() override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static const std::wstring kClassName;

	void	Recv_info(std::vector<TEAM_SUMMONER_INFO> info);
	void	Recv_PosInfo(ui::UiRect	pos);
	//¿Ø¼þÇø
	void	init_all_controls();
	bool	init_set_listen_controls(ui::EventArgs* args);
private:
	int		now_select_item_index = 0;
	ui::ListBox* _list;
	
	std::vector<TEAM_SUMMONER_INFO> player_info;
	Details_Pop* details_wind;
	std::mutex	details_wind_mtx;

};
