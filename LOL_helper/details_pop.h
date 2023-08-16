#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/struct.h"
#include	"lcu_riot/lcu_api.h"
#include	"details_items_ui.h"
#include	"lcu_structs.hpp"
#include	"MiscThread.h"
class Details_Pop : public ui::WindowImplBase
{
public:
	Details_Pop();
	~Details_Pop();

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
	bool	details_isinvalid();
	void	reset_info();
	void	init_info(LCU_JSON_RESPONSE::LolChampSelect info);
private:
	void	set_info_to_ui(LCU_JSON_RESPONSE::MyTeam player);
	void	Recv_datas1(LCU_JSON_RESPONSE::LolMatchHistory	datas);
	void	add_items(int participantId);

	int64_t		now_player_cellid = 0;	//这里0是默认初始化 还有一次初始化 把他赋值成第一个元素的cellid


	LCU_JSON_RESPONSE::LolChampSelect	info_;
	std::map<int64_t,const LCU_JSON_RESPONSE::MyTeam>	info_my_team;
	std::map<int, LCU_JSON_RESPONSE::LolMatchHistory>	all_datas;//CellId --> 战绩

	ui::Label* wind_title;
	ui::ListBox* Vlist;
	ui::VBox* main_box;

	std::mutex	_data_mtx;
};
