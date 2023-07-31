#pragma once
#include <string>
#include    <Windows.h>
#include <codecvt>
#include <locale>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

std::string to_string(long long num);

std::string get_commandline_exec(const char* cmd);


std::string wstring2string(std::wstring wstr);
std::wstring string2wstring(const std::string& str);
std::wstring StringToWString(const std::string& str);


std::string wstring_to_utf8(const std::wstring& wstr);

std::string timestamp2string(unsigned long long timestamp);