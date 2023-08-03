#include	"dynamic_skin.h"

bool dynamic_skin_host_my::check_Allow()
{
	CURL* curl;
	CURLcode res;
	std::string file_path;
	bool	ret = false;
	curl = curl_easy_init();

	if (curl) {
		// 设置API的URL
		curl_easy_setopt(curl, CURLOPT_URL, my_host_check.c_str());
		CHAR path[260];
		SHGetSpecialFolderPathA(0, path, CSIDL_DESKTOPDIRECTORY, 0);
		file_path = path;
		file_path += "\\hallo.jpg";
		// 设置回调函数来将API返回的数据写入文件
		FILE* file;
		if (fopen_s(&file, file_path.c_str(), "wb") == 0) {
			// 设置回调函数来将API返回的数据写入文件
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

			// 发送请求
			res = curl_easy_perform(curl);
			long httpcode;
			res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpcode);

			// 检查请求是否成功
			if (httpcode != 200) {
				ret = false;
			}
			else {
				ret = true;
			}

			// 关闭文件
			fclose(file);
			curl_easy_cleanup(curl);
		}
		else {
			ret = false;
		}
	}
	return	ret;
}
std::string	dynamic_skin_host_my::download_dll() {
	CHAR szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPathA(NULL, szPath, CSIDL_APPDATA, FALSE);
	std::string	dll_path = szPath;
	dll_path += "\\hid.dll";

	CURL* curl;
	CURLcode res;
	curl = curl_easy_init();

	if (curl) {
		// 设置API的URL
		curl_easy_setopt(curl, CURLOPT_URL, my_host_dll.c_str());
		CHAR path[260];
		SHGetSpecialFolderPathA(0, path, CSIDL_DESKTOPDIRECTORY, 0);
		// 设置回调函数来将API返回的数据写入文件
		FILE* file;
		if (fopen_s(&file, dll_path.c_str(), "wb") == 0) {
			// 设置回调函数来将API返回的数据写入文件
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NULL);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);

			// 发送请求
			res = curl_easy_perform(curl);
			long httpcode;
			res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &httpcode);

			// 检查请求是否成功
			if (httpcode != 200) {
				dll_path.clear();
			}
			// 关闭文件
			fclose(file);
			curl_easy_cleanup(curl);
		}
		else {
			dll_path.clear();
		}
	}
	return	dll_path;
}	//返回下载的地址


std::string	dynamic_skin_host_my::get_lol_game_path() {
	std::string result = GetProcessCommandLine("wmic process where caption='LeagueClientUx.exe' get commandline");
	if (result.empty())
	{
		return	"";
	}
	std::string	game_path;
	size_t _pos = result.find("--app-log-file-path=");
	if (_pos != std::string::npos) {
		_pos += strlen("--app-log-file-path=");
		size_t _end = result.find("\"", _pos);
		if (_end != std::string::npos) {
			game_path = result.substr(_pos, _end - _pos);
		}
	}
	_pos = game_path.find("/LeagueClient");
	game_path = game_path.substr(0, _pos);
	game_path.append("/game/hid.dll");
	return	game_path;
}

bool	dynamic_skin_host_my::open_dynamic_skin(std::string	dll_path) {
	auto	game_path = get_lol_game_path();
	if (game_path.empty()||dll_path.empty())
	{
		return	false;
	}
	return CopyFileA(dll_path.c_str(), game_path.c_str(), false);
}

bool	dynamic_skin_host_my::close_dynamic_skin() {
	auto	game_path = get_lol_game_path();
	return	DeleteFileA(game_path.c_str());
}