#include	"MiscThread.h"


void MiscThread::Init()
{
	nbase::ThreadManager::RegisterThread(thread_id_);
}

void MiscThread::Cleanup()
{
	nbase::ThreadManager::UnregisterThread();
}

void Post2UI(const StdClosure& closure)
{
	nbase::ThreadManager::PostTask(kThreadUi, closure);
}