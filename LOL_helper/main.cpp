// basic.cpp : 定义应用程序的入口点。
//

#include "main.h"
#include "basic_form.h"
#include	"resource.h"
//#define	USE_RESOURCE_FILE	0


std::wstring	getAppPath() {
	return	L"C:\\Users\\Administrator\\source\\repos\\LOL_helper\\";
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MainThread().RunOnCurrentThreadWithLoop(nbase::MessageLoop::kUIMessageLoop);

	return 0;
}

void MiscThread::Init()
{
	nbase::ThreadManager::RegisterThread(thread_id_);
}

void MiscThread::Cleanup()
{
	nbase::ThreadManager::UnregisterThread();
}

void MainThread::Init()
{

	check_only();

	nbase::ThreadManager::RegisterThread(kThreadMain);

	// 启动杂事处理线程
	misc_thread_.reset(new MiscThread(kThreadGlobalMisc, "Global Misc Thread"));
	misc_thread_->Start();

#ifdef USE_RESOURCE_FILE
	// 获取资源路径，初始化全局参数
	std::wstring theme_dir = getAppPath();
	ui::GlobalManager::Startup(theme_dir + L"resources\\", ui::CreateControlCallback(), false);
#else
	// Release 模式下使用资源中的压缩包作为资源
	// 资源被导入到资源列表分类为 THEME，资源名称为 IDR_THEME
	// 如果资源使用的是本地的 zip 文件而非资源中的 zip 压缩包
	// 可以使用 OpenResZip 另一个重载函数打开本地的资源压缩包
	ui::GlobalManager::OpenResZip(MAKEINTRESOURCE(IDR_RESOUCRS2), L"RESOUCRS", "");
	//ui::GlobalManager::OpenResZip(L"resources.zip", "");
	ui::GlobalManager::Startup(L"resources\\", ui::CreateControlCallback(), false);
#endif

	// 创建一个默认带有阴影的居中窗口
	BasicForm* window = new BasicForm();
	window->Create(NULL, L"自动接收助手", WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX, 0);
	window->CenterWindow();
	window->ShowWindow();
}

void MainThread::Cleanup()
{
	ui::GlobalManager::Shutdown();

	misc_thread_->Stop();
	misc_thread_.reset(nullptr);

	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}
void	MainThread::check_only() {
	// 创建一个命名的互斥体
	HANDLE hMutex = CreateMutex(NULL, TRUE, L"874678DC-29FB-4CA1-8E8E-A3D478F03CAF");

	// 检查互斥体是否已存在
	if (GetLastError() == ERROR_ALREADY_EXISTS) {
		// 互斥体已存在，表示已经有一个相同的进程在运行
		HWND hWnd = FindWindow(NULL, L"自动接收助手");
		UINT myMessage = RegisterWindowMessage(L"ONLY_ONE");
		SendMessage(hWnd, myMessage, 0, 0);
		exit(1);
		return;
	}
}