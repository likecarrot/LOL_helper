#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>


/** @class �����߳�
* @brief �����߳��࣬�̳� nbase::FrameworkThread
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
	* �麯������ʼ���߳�
	* @return void	�޷���ֵ
	*/
	virtual void Init() override;

	/**
	* �麯�����߳��˳�ʱ����һЩ��������
	* @return void	�޷���ֵ
	*/
	virtual void Cleanup() override;
private:
	enum ThreadIds thread_id_;
};

/** @class MainThread
* @brief ���̣߳�UI�̣߳��࣬�̳� nbase::FrameworkThread
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
	* �麯������ʼ�����߳�
	* @return void	�޷���ֵ
	*/
	virtual void Init() override;

	/**
	* �麯�������߳��˳�ʱ����һЩ��������
	* @return void	�޷���ֵ
	*/
	virtual void Cleanup() override;
	void	check_only();

private:
	std::unique_ptr<MiscThread>	misc_thread_;	// ר�Ŵ������µ��߳�
};