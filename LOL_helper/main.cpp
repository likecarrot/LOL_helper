// basic.cpp : 定义应用程序的入口点。
//

#include "main.h"
#include "basic_form.h"
#include	"resource.h"
//#define	USE_RESOURCE_FILE	0
ui::Control* MyCreateControlCallback(const std::wstring& sName)
{
	if (sName == L"VirtualTileBox")
	{
		return new VirtualTileBox();
	}
	return nullptr;
}

std::wstring	getAppPath() {
	return	L"C:\\Users\\Administrator\\source\\repos\\LOL_helper\\";
}

static MiscThread* _GameStatusThread = NULL;
static MiscThread* _GameUiPosThread = NULL;
static MiscThread* _Network = NULL;
static MiscThread* _GameCommandline = NULL;
static MiscThread* _HelperWorkThread = NULL;
static MiscThread* _TestUiThread = NULL;


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


void MainThread::Init()
{
	check_only();

	nbase::ThreadManager::RegisterThread(kThreadUi);

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
	ui::GlobalManager::Startup(L"resources\\", MyCreateControlCallback, false);
#endif
	startMiscThread();
	// 创建一个默认带有阴影的居中窗口
	BasicForm* window = new BasicForm();
	window->Create(NULL, L"自动接收助手", WS_OVERLAPPEDWINDOW &~WS_MAXIMIZEBOX, 0);
	window->ShowWindow();
	window->CenterWindow();
}

void MainThread::startMiscThread()
{
	if (NULL == _GameStatusThread)
	{
		_GameStatusThread = new MiscThread(kThreadLoopGetGameStatusThread, "一个死循环获取游戏的状态,比如 对局中,房间中");
		_GameStatusThread->Start();
	}
	if (NULL == _GameUiPosThread)
	{
		_GameUiPosThread = new MiscThread(kThreadLoopGetGameUiPosThread, "循环获取LOL的界面位置线程	要做到游戏界面挪了,对战助手的Ui也跟着挪");
		_GameUiPosThread->Start();
	}
	if (NULL == _Network)
	{
		_Network = new MiscThread(kThreadNetwork, "负责网络请求");
		_Network->Start();
	}
	if (NULL == _GameCommandline)
	{
		_GameCommandline = new MiscThread(kThreadLoopGetGameCommandline, "循环获取游戏的命令行,也并不是一直获取,而是如果有了正常的值就返回");
		_GameCommandline->Start();
	}
	if (NULL == _HelperWorkThread)
	{
		_HelperWorkThread = new MiscThread(kThreadHelperWorkThread, "助手工作线程");
		_HelperWorkThread->Start();
	}
	if (NULL == _TestUiThread)
	{
		_TestUiThread = new MiscThread(KThreadTestUiThread, "测试是不是因为线程的原因 listbox才变得很卡");
		_TestUiThread->Start();
	}
}
void MainThread::StopMiscThread()
{
	if (_GameStatusThread)
	{
		_GameStatusThread->Stop();
		delete _GameStatusThread;
		_GameStatusThread = NULL;
	}
	if (_GameUiPosThread)
	{
		_GameUiPosThread->Stop();
		delete _GameUiPosThread;
		_GameUiPosThread = NULL;
	}
	if (_Network)
	{
		_Network->Stop();
		delete _Network;
		_Network = NULL;
	}
	if (_GameCommandline)
	{
		_GameCommandline->Stop();
		delete _GameCommandline;
		_GameCommandline = NULL;
	}
	if (_HelperWorkThread)
	{
		_HelperWorkThread->Stop();
		delete _HelperWorkThread;
		_HelperWorkThread = NULL;
	}
	if (_TestUiThread)
	{
		_TestUiThread->Stop();
		delete _TestUiThread;
		_TestUiThread = NULL;
	}
}

void MainThread::Cleanup()
{
	StopMiscThread();
	ui::GlobalManager::Shutdown();
	SetThreadWasQuitProperly(true);
	nbase::ThreadManager::UnregisterThread();
}
void	MainThread::check_only() {
	HANDLE hPipe = CreateNamedPipe(
		L"\\\\.\\pipe\\{6799FB87-574B-4382-AF2A-19AE027B68CD}",  // 命名管道的名称
		PIPE_ACCESS_DUPLEX,          // 管道的访问模式
		PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,  // 管道的类型和读取模式
		1,                           // 允许的最大实例数
		0,                           // 输出缓冲区的大小
		0,                           // 输入缓冲区的大小
		NMPWAIT_USE_DEFAULT_WAIT,    // 等待客户端连接的超时时间
		NULL                         // 安全属性
	);
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		HWND hWnd = FindWindow(NULL, L"自动接收助手");
		UINT myMessage = RegisterWindowMessage(L"ONLY_ONE");
		SendMessage(hWnd, myMessage, 0, 0);
		exit(1);
		return;
	}
}