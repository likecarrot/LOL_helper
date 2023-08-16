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
	void	startMiscThread();
	void	StopMiscThread();
	/**
	* 虚函数，主线程退出时，做一些清理工作
	* @return void	无返回值
	*/
	virtual void Cleanup() override;
	void	check_only();
};
