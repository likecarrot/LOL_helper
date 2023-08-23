#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/struct.h"
#include	"lcu_riot/lcu_api.h"
#include	"utils.h"
#include	"item.h"
#include	<Windows.h>
#include	<shellapi.h>
#include	"resource.h"
#include	"pop_form.h"
#include	"game_resource.h"
#include	"MiscThread.h"
#include	"lcu_riot/config.h"
#include	"game_helper.h"
#include	"helper_setting.h"
#include	"timeout_ui.h"

#define	DYNAMIC_SKIN		//加入动态换肤
#ifdef DYNAMIC_SKIN
#include	"dynamic_skin.h"
#endif // DYNAMIC_SKIN


// 托盘图标的唯一标识符
#define TRAY_ICON_ID 1001
#define	MENUBAR_ONCLOSE	1002
#define	MENUBAR_COPYQQ	1003

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

	
private:	//变量区
	
	ui::CheckBox* _ui_accept;		//复选框 
	ui::CheckBox* _ui_nextgame;
	ui::CheckBox* _ui_searchqueue;
	ui::CheckBox* _ui_player_helper;//战绩助手
	ui::CheckBox* _ui_aram_helper;//大乱斗助手
	ui::CheckBox* _ui_classic_helper;//匹配助手

	ui::VBox* tools_area;		//功能区域的容器
	ui::Control* summoner_icon;	//用户头像
	ui::CheckBox* _dynamic_skin;	//动态换肤 这个控件默认是隐藏的

	ui::Label* _ui_player_name;
	ui::Label* _ui_player_level;
	ui::Label* _ui_RANKED_SOLO_5x5;
	ui::Label* _ui_RANKED_FLEX_SR;
	ui::Label* _ui_RANKED_TFT;
	ui::Label* _ui_RANKED_TFT_TURBO;
	ui::Label* _ui_RANKED_TFT_DOUBLE_UP;
	ui::Label* display_game_status;

	ui::Button* _ui_close;
	ui::Button* _helper_settings;//助手设置 按钮

	GAME_HELPER::Aram_helper	aram_helper;	//两个助手相关
	GAME_HELPER::Classic_helper classic_helper;	//两个助手相关

	// 自定义消息的ID
	UINT myMessage = RegisterWindowMessage(L"ONLY_ONE");
	Pop_form* tools_windows;//lol助手窗口
	//托盘图标
	NOTIFYICONDATA	m_trayIcon;

	//使用的线程
	std::mutex	ui_still_alive_lock;
	bool	still_alive = false;

private://函数区
	bool	OnSelected(ui::EventArgs* args);	//复选框被单击的处理函数
	void	update_select_status();	//当ui'上复选框被选择时,更新复选框的显示文本
	bool	OnUiMyClose(ui::EventArgs* args);	//这个是当ui上 关闭按钮 被单击时 设置一个托盘图标的函数
	bool	OnHelperSettings(ui::EventArgs* args);//当ui上 助手设置被单击时的处理函数

	void	open_player_helper_tools();	//当开启战绩助手之后,调用这个函数来弹出战绩框
	//控件初始化函数
	void	init_all_controls();
	void	init_set_listen_controls();
	
	//添加托盘图标(初始化)
	void	AddTrayIcon();
	//处理托盘图标上的事件
	LRESULT OnTrayIcon(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL bHandled);

	//辅助函数 弃用
	//static std::wstring	add_str_status(const std::wstring& content,bool status);

	//网络请求结束后的回调函数
	void	Receive_Datas1(GAME_STATUS gamestatus);
	void	Receive_Datas2(LCU_JSON_RESPONSE::LolSummonerInfo info);
	void	Receive_Datas3(LCU_JSON_RESPONSE::LolRankedData rank_Datas);

	void	set_current_player_icon(std::string icon_path);
	void	config_file_settings_to_ui();
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


