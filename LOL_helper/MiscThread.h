#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>


/** @class 杂事线程
* @brief 杂事线程类，继承 nbase::FrameworkThread
* @copyright (c) 2015, NetEase Inc. All rights reserved
* @date 2015/1/1
*/
class MiscThread : public nbase::FrameworkThread
{
public:
	MiscThread(enum ThreadIds thread_id, const char* name)
		: FrameworkThread(name)
		, thread_id_(thread_id) {}

	~MiscThread(void) {}

private:
	/**
	* 虚函数，初始化线程
	* @return void	无返回值
	*/
	virtual void Init() override;

	/**
	* 虚函数，线程退出时，做一些清理工作
	* @return void	无返回值
	*/
	virtual void Cleanup() override;
private:
	enum ThreadIds thread_id_;
};


//投递任务到UI线程
void Post2UI(const StdClosure& closure);
