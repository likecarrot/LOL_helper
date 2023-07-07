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


// 托盘图标的唯一标识符
#define TRAY_ICON_ID 1001
#define	WM_ONCLOSE	1002

struct _UI_STATUS
{
	bool	_ui_accept_status = true;
	bool	_ui_nextgame_status = false;
	bool	_ui_searchqueue_status = false;
	bool	_ui_lockchampion_status = false;
	int		_ui_champion_id = 0;
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
	ui::Label* _ui_selectchampion;	//用户双击后提示目前所需要拿的英雄,仅作显示用
	ui::VBox* tools_area;		//功能区域的容器
	bool	OnSelected(ui::EventArgs* args);
	void	update_select_status();
	bool	OnSelectedChampion(ui::EventArgs* args);
	bool	OnUiMyClose(ui::EventArgs* args);


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
	//我所需要的结构和数据
	_UI_STATUS	ui_datas;
	helper		client;

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
	RANK_LEVEL_ITEM get(RANK_LEVEL& vec,std::string en)const ;

	bool	sort_champions_datas(ui::EventArgs* args);

	void	Receive_Game_status(GAME_STATUS gamestatus);
	void	Receive_Summoner_info(SUMMONER_INFO& info);
	void	Receive_Rank_level(RANK_LEVEL& rank_Datas);
	void	Receive_Owner_champions(std::vector<CHAMPION>& owner_datas);
};


