#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"MiscThread.h"


/** @class MainThread
* @brief 主线程（UI线程）类，继承 nbase::FrameworkThread
* @copyright (c) 2015, NetEase Inc. All rights reserved
* @date 2015/1/1
*/
class MainThread : public nbase::FrameworkThread
{
public:
	MainThread() : nbase::FrameworkThread("MainThread") {}
	virtual ~MainThread() {}

private:
	/**
	* 虚函数，初始化主线程
	* @return void	无返回值
	*/
	virtual void Init() override;

	/**
	* 虚函数，主线程退出时，做一些清理工作
	* @return void	无返回值
	*/
	virtual void Cleanup() override;
	void	check_only();

private:
	std::unique_ptr<MiscThread>	loop_getgamestatus_thread_;	//循环获取游戏状态的线程
	std::unique_ptr<MiscThread>	network_thread_;	// 负责网络请求的线程
};
