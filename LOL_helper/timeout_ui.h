#pragma once
#ifndef MY_HEADER_FILE_H
#define MY_HEADER_FILE_H
#include	<functional>
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/lcu_api.h"
#include <chrono>
#include <thread>
#include "MiscThread.h"



class Notify_Ui_Base : public ui::WindowImplBase
{
public:
	static const std::wstring kClassName;

	Notify_Ui_Base(int all_tick);
	~Notify_Ui_Base();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;


	virtual void InitWindow() override;
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	bool	Clicked(ui::EventArgs* args);
private:
	void startTimer();
	bool _thread_status = true;
	std::thread _thread;


	ui::Button* btn;
	int	all_tick;
	bool	_run = true;
};
#endif
