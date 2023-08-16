#include "utils.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <Windows.h>
#pragma comment(lib, "Urlmon.lib") 
#include	<Shlobj.h>
#include <tchar.h>

std::string to_string(long long num)
{
	std::string result;
	if (num == 0)
	{
		result = "0";
		return result;
	}
	bool neg = num < 0;
	if (neg)
	{
		num = -num;
	}
	while (num > 0)
	{
		result = static_cast<char>('0' + num % 10) + result;
		num /= 10;
	}
	if (neg)
	{
		result = "-" + result;
	}
	return result;
}

std::string get_commandline_exec(const char* cmd) {
	char buffer[128];
	std::string result = "";
	FILE* pipe = _popen(cmd, "r");
	if (!pipe) throw std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
			result += buffer;
		}
	}
	catch (...) {
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);
	return result;
}


std::string wstring2string(std::wstring wstr)
{
	std::string result;
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	if (len <= 0)return result;
	char* buffer = new char[len + 1];
	if (buffer == NULL)return result;
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

std::wstring string2wstring(const std::string& str)
{
	std::wstring wstr;
	int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	if (len > 0) {
		std::vector<wchar_t> buffer(len + 1);
		int result = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, &buffer[0], len);
		if (result > 0) {
			wstr.assign(buffer.begin(), buffer.end() - 1);
		}
	}
	return wstr;
}

std::wstring StringToWString(const std::string& str)
{
	LPCSTR pszSrc = str.c_str();
	int nLen = ::MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, NULL, 0);
	if (nLen == 0)
		return std::wstring(L"");

	wchar_t* pwszDst = new wchar_t[nLen];
	if (!pwszDst)
		return std::wstring(L"");

	::MultiByteToWideChar(CP_ACP, 0, pszSrc, -1, pwszDst, nLen);
	std::wstring wstr(pwszDst);
	delete[] pwszDst;
	pwszDst = NULL;

	return wstr;
}


std::string wstring_to_utf8(const std::wstring& wstr) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}


std::string timestamp2string(unsigned long long timestamp) {
	// 将时间戳从毫秒转换为秒
	auto timepoint = std::chrono::milliseconds(timestamp);
	auto seconds = std::chrono::duration_cast<std::chrono::seconds>(timepoint);

	// 将时间戳转换为日期时间
	auto datetime = std::chrono::system_clock::time_point(std::chrono::seconds(seconds.count()));
	std::time_t time = std::chrono::system_clock::to_time_t(datetime);

	// 使用 localtime_s 函数替代 localtime
	std::tm timeStruct;
	localtime_s(&timeStruct, &time);

	// 格式化日期时间为字符串
	std::stringstream ss;
	ss << std::put_time(&timeStruct, "%Y-%m-%d %H:%M:%S");

	return ss.str();
}

std::string GetProcessCommandLine(const std::string& cmdLine) {
	/* 创建匿名管道 */
	SECURITY_ATTRIBUTES _security = { 0 };
	_security.bInheritHandle = TRUE;
	_security.nLength = sizeof(_security);
	_security.lpSecurityDescriptor = NULL;
	HANDLE hRead = NULL, hWrite = NULL;
	if (!CreatePipe(&hRead, &hWrite, &_security, 0)) {
		printf("创建管道失败,error code=%d \n", GetLastError());
	}
	/* cmd命令行转换为Unicode编码 */
	int convLength = MultiByteToWideChar(CP_UTF8, 0, cmdLine.c_str(), (int)strlen(cmdLine.c_str()), NULL, 0);
	if (convLength <= 0) {
		printf("字符串转换长度计算出错\n");
	}
	std::wstring wCmdLine;
	wCmdLine.resize(convLength + 10);
	convLength = MultiByteToWideChar(CP_UTF8, 0, cmdLine.c_str(), (int)strlen(cmdLine.c_str()), &wCmdLine[0], (int)wCmdLine.size());
	if (convLength <= 0) {
		printf("字符串转换出错\n");
	}
	/* 创建新进程执行cmd命令并将结果写入到管道 */
	PROCESS_INFORMATION pi = { 0 };
	STARTUPINFO si = { 0 };
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.wShowWindow = SW_HIDE; // 隐藏cmd执行的窗口
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	if (!CreateProcess(NULL,
		&wCmdLine[0],
		NULL,
		NULL,
		TRUE,
		0,
		NULL,
		NULL,
		&si,
		&pi)) {
		printf("创建子进程失败,error code=%d \n", GetLastError());
	}
	/* 等待进程执行命令结束 */
	::WaitForSingleObject(pi.hThread, INFINITE);
	::WaitForSingleObject(pi.hProcess, INFINITE);
	/* 从管道中读取数据 */
	DWORD bufferLen = 10240;
	char* buffer = (char*)malloc(10240);
	memset(buffer, '\0', bufferLen);
	DWORD recLen = 0;
	if (!ReadFile(hRead, buffer, bufferLen, &recLen, NULL)) {
		printf("读取管道内容失败, error code=%d\n", GetLastError());
	}
	std::string ret(buffer);
	/* 关闭句柄 */
	CloseHandle(hRead);
	CloseHandle(hWrite);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	free(buffer);
	return ret;
}


//把文本复制到剪切板上
void CopyTextToClipboard(const std::wstring& text) {
	if (OpenClipboard(nullptr)) {
		EmptyClipboard();

		HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, (text.length() + 1) * sizeof(wchar_t));
		if (hMem) {
			wchar_t* pMem = (wchar_t*)GlobalLock(hMem);
			if (pMem) {
				wcscpy_s(pMem, text.length() + 1, text.c_str());
				GlobalUnlock(hMem);
				SetClipboardData(CF_UNICODETEXT, hMem);
			}
		}
		CloseClipboard();
	}
	const TCHAR szOperation[] = _T("open");
	const TCHAR szAddress[] = _T("https://qm.qq.com/cgi-bin/qm/qr?k=41ynsb-IkVHs-H7f6ElttSeUOopoFAqm&jump_from=webapi&authKey=snh/EwHV9krS2juORMHNkmiOLD6Jrump+wRzMYm74toEIDlw23ibfRV1AfQmTeMu");
	HINSTANCE hRslt = ShellExecute(NULL, szOperation,
		szAddress, NULL, NULL, SW_SHOWNORMAL);
}


std::string	get_app_path() {//返回appdata目录下 lol_helper路径,如果不存在,就创建
	CHAR szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPathA(NULL, szPath, CSIDL_APPDATA, FALSE);
	std::string AppdataFolder = szPath;
	std::string MyFolder = AppdataFolder + "\\lol_helper";

	if (!folderExists(MyFolder)) {
		::CreateDirectoryA(MyFolder.c_str(), NULL);
	}
	return	MyFolder;
}
std::string get_app_path(std::string child_folder) {	//如果调用这个函数,返回这个child_folder的目录,如果不存在,就创建
	std::string path=get_app_path();
	path = path + "\\" + child_folder;
	if (!folderExists(path)) {
		::CreateDirectoryA(path.c_str(), NULL);
	}
	return	path;
}

bool folderExists(const std::string& folderPath) {
	DWORD fileAttributes = ::GetFileAttributesA(folderPath.c_str());
	return (fileAttributes != INVALID_FILE_ATTRIBUTES && (fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}