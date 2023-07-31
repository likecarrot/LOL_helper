#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/struct.h"
#include	"lcu_riot/lol_helper.h"
#include	"lcu_riot/utils.h"
#include	"item.h"
#include	<Windows.h>
#include	<shellapi.h>
#include	"resource.h"
#include	"utils.hpp"
#include	"pop_form.h"
#include	"game_resource.h"
#include	"MiscThread.h"

#define	DYNAMIC_SKIN		//加入动态换肤
#ifdef DYNAMIC_SKIN
#include	"dynamic_skin.h"
#endif // DYNAMIC_SKIN


// 托盘图标的唯一标识符
#define TRAY_ICON_ID 1001
#define	MENUBAR_ONCLOSE	1002
#define	MENUBAR_COPYQQ	1003

struct _UI_STATUS
{
	bool	_ui_accept_status = true;
	bool	_ui_nextgame_status = false;
	bool	_ui_searchqueue_status = false;
	bool	_ui_lockchampion_status = false;
	int		_ui_champion_id = 0;
	bool	_ui_player_help = true;
};

class BasicForm : public ui::WindowImplBase
{
public:
	BasicForm();
	~BasicForm();

	/**
	 * 一下三个接口是必须要覆写的接口，父类会调用这三个接口来构建窗口
	 * GetSkinFolder		接口设置你要绘制的窗口皮肤资源路径
	 * GetSkinFile			接口设置你要绘制的窗口的 xml 描述文件
	 * GetWindowClassName	接口设置窗口唯一的类名称
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	 * 收到 WM_CREATE 消息时该函数会被调用，通常做一些控件初始化的操作
	 */
	virtual void InitWindow() override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)override;


	/**
	 * 收到 WM_CLOSE 消息时该函数会被调用
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static const std::wstring kClassName;
	
	

	//控件区
	void	init_all_controls();
	void	init_set_listen_controls();

	ui::CheckBox* _ui_accept;		//复选框
	ui::CheckBox* _ui_nextgame;
	ui::CheckBox* _ui_searchqueue;
	ui::CheckBox* _ui_lockchampion;
	ui::CheckBox* _ui_player_helper;//对局助手
	ui::Label* _ui_selectchampion;	//用户双击后提示目前所需要拿的英雄,仅作显示用
	ui::VBox* tools_area;		//功能区域的容器
	ui::Control* summoner_icon;	//用户头像
	ui::CheckBox* _dynamic_skin;	//动态换肤 这个控件默认是隐藏的
	ui::Label* _download_R3nzdll;//下载进度

	bool	OnSelected(ui::EventArgs* args);
	void	update_select_status();
	bool	OnSelectedChampion(ui::EventArgs* args);
	bool	OnUiMyClose(ui::EventArgs* args);

	void	open_player_helper_tools();

	ui::Label* _ui_player_name;
	ui::Label* _ui_player_level;
	ui::Label* _ui_RANKED_SOLO_5x5;
	ui::Label* _ui_RANKED_FLEX_SR;
	ui::Label* _ui_RANKED_TFT;
	ui::Label* _ui_RANKED_TFT_TURBO;
	ui::Label* _ui_RANKED_TFT_DOUBLE_UP;
	ui::Label* display_game_status;
	ui::ListBox* _champion_list;
	ui::RichEdit* _ui_search_champion;

	ui::Button* _ui_close;
private:

	// 自定义消息的ID
	UINT myMessage = RegisterWindowMessage(L"ONLY_ONE");


	//我所需要的结构和数据
	_UI_STATUS	ui_datas;

	Pop_form* tools_windows;//lol助手窗口

	//托盘图标
	NOTIFYICONDATA	m_trayIcon;
	//添加托盘图标(初始化)
	void AddTrayIcon();
	//处理托盘图标上的事件
	LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled);

	std::vector<CHAMPION>	have_champions_copy;	
	std::vector<CHAMPION> have_champions_copy_sort;	
	//副本数据,只作为查找选择id使用
	//副本数据 这个结构是为了搜索框而设计的,如果搜索框里有内容就从这里面拿英雄id,如果没有 就从上面那个拿
	// 
	// 
	//辅助函数
	static std::wstring	add_str_status(std::wstring& content,bool status);
	RANK_LEVEL_ITEM get(const RANK_LEVEL& vec,std::string en)const ;

	bool	sort_champions_datas(ui::EventArgs* args);

	void	Receive_Datas1(GAME_STATUS gamestatus);
	void	Receive_Datas2(SUMMONER_INFO info);
	void	Receive_Datas3(RANK_LEVEL rank_Datas);
	void	Receive_Datas4(std::vector<CHAMPION> owner_datas);
	void	set_current_player_icon(std::string icon_path);

#ifdef DYNAMIC_SKIN
	//对动态皮肤新加入的处理
	//2023-7-16
	std::string	origin_dll_path;	//下载好的 皮肤 dll 的路径
	std::string	lol_game_path;	//lol\\game路径
	void	Receive_check(bool	check);
	void	Receive_download_dll_path(std::string	save_path);
	bool	Open_dynamic_skin(ui::EventArgs* args);
#endif // DYNAMIC_SKIN
};


