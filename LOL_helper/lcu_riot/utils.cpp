#include "utils.h"
#include <stdexcept>
#include <string>
#include <vector>
#include <Windows.h>


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