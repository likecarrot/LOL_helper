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
	Pop_form() {};
	~Pop_form() {};

	/**
	 * һ�������ӿ��Ǳ���Ҫ��д�Ľӿڣ����������������ӿ�����������
	 * GetSkinFolder		�ӿ�������Ҫ���ƵĴ���Ƥ����Դ·��
	 * GetSkinFile			�ӿ�������Ҫ���ƵĴ��ڵ� xml �����ļ�
	 * GetWindowClassName	�ӿ����ô���Ψһ��������
	 */
	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	/**
	 * �յ� WM_CREATE ��Ϣʱ�ú����ᱻ���ã�ͨ����һЩ�ؼ���ʼ���Ĳ���
	 */
	virtual void InitWindow() override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)override;

	/**
	 * �յ� WM_CLOSE ��Ϣʱ�ú����ᱻ����
	 */
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static const std::wstring kClassName;

	void	Recv_info(std::vector<TEAM_SUMMONER_INFO> info);

	//�ؼ���
	void	init_all_controls();
	bool	init_set_listen_controls(ui::EventArgs* args);
private:
	int		now_select_item_index = 0;
	ui::ListBox* _list;
	
	std::vector<TEAM_SUMMONER_INFO> player_info;
	Details_Pop* details_wind;
	std::mutex	details_wind_mtx;

};