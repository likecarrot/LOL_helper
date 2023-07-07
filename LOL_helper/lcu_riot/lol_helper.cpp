#include	"lol_helper.h"
#include <ShlObj.h>

helper::helper()
{
}

helper::~helper()
{
}

void	helper::wait_game_start() {
	std::string result;
	while (result.find("token") == std::string::npos) {
		result = GetProcessCommandLine("wmic process where caption='LeagueClientUx.exe' get commandline");
		if (result.find("token") == std::string::npos) {
			Sleep(2000);
			continue;
		}
		// 截取auth_token
		size_t auth_token_pos = result.find("--remoting-auth-token=");
		if (auth_token_pos != std::string::npos) {
			auth_token_pos += strlen("--remoting-auth-token=");
			size_t auth_token_end = result.find("\"", auth_token_pos);
			if (auth_token_end != std::string::npos) {
				domain.auth_token = result.substr(auth_token_pos, auth_token_end - auth_token_pos);
			}
		}

		// 截取port
		size_t port_pos = result.find("--app-port=");
		if (port_pos != std::string::npos) {
			port_pos += strlen("--app-port=");
			size_t port_end = result.find("\"", port_pos);
			if (port_end != std::string::npos) {
				domain.port = result.substr(port_pos, port_end - port_pos);
			}
		}

#ifdef DEBUG
		std::cout << "-----------" << std::endl;
		std::cout << domain.port << std::endl;
		std::cout << domain.auth_token << std::endl;
		std::cout << "-----------" << std::endl;
#endif // DEBUG
	}
}

std::string helper::GetProcessCommandLine(const std::string& cmdLine) {
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

bool	helper::init() {
	summoner.clear();
	LCU_REQUEST::setInstance(domain.auth_token, domain.port);
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, "/lol-summoner/v1/current-summoner");

	try {
		summoner = parse_json<SUMMONER_INFO>(response);
		std::wstring c = string2wstring(summoner.displayName);
		std::cout << "游戏昵称:" << wstring_to_utf8(c) << '\n';
		std::cout << "游戏等级:" << summoner.summonerLevel << '\n';
	}
	catch (const std::exception& e) {
		return	false;
	}
	return	true;
}

bool	helper::get_Rank_level() {	//获取排位分数
	rank_level.clear();
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, "/lol-ranked/v1/ranked-stats/", this->summoner.puuid);
	try {
		rank_level = parse_json<RANK_LEVEL>(response);
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return	false;
	}
	return	true;
}


GAME_STATUS		helper::get_game_status() {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, "/lol-gameflow/v1/gameflow-phase");
	auto	status = get_enum_gamestatus(response);
#ifdef DEBUG
	std::cout << "游戏状态: " << game_status_dict[static_cast<int>(status)][0] << std::endl;
#endif // DEBUG
	return	status;
}
void	helper::accept_game() {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, "/lol-matchmaking/v1/ready-check/accept", "", "");
#ifdef DEBUG
	std::cout << response << std::endl;
#endif // DEBUG
}

void	helper::auto_next_game() {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, "/lol-lobby/v2/play-again", "", "");
#ifdef DEBUG
	std::cout << response << std::endl;
#endif // DEBUG
}

void	helper::search_queue() {
	// /lol-lobby/v2/lobby/matchmaking/search 开始搜索队伍
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::POST_METHOD, "/lol-lobby/v2/lobby/matchmaking/search", "", "");
#ifdef DEBUG
	std::cout << response << std::endl;
#endif // DEBUG
}


int	helper::get_owner_champions() {
	std::lock_guard<std::mutex> lock(owner_champions_mutex);
	owner_champions.clear();
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, "/lol-champions/v1/owned-champions-minimal");
	try
	{
		json	js = json::parse(response);
		for (const auto& role : js) {
			CHAMPION	ch = { 0 };
			ch.name = wstring2string(string2wstring(role["name"].get<std::string>()));
			ch.title = wstring2string(string2wstring(role["title"].get<std::string>()));
			ch.freePlay = role["freeToPlay"].get<bool>();
			ch.selectid = role["id"].get<int>();
			owner_champions.push_back(ch);
		}
	}
	catch (const std::exception&)
	{
		OutputDebugStringA("AAAA");
		return 0;
	}
	return	owner_champions.size();
}	//获取已经拥有的英雄

std::string	helper::download_icons(int	icon_id) {
	//"https://127.0.0.1:51932/lol-game-data/assets/v1/profile-icons/1000.jpg";
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable

	CHAR szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPathA(NULL, szPath, CSIDL_APPDATA, FALSE);
	FILE* fp;

	std::string	url2 = ("/lol-game-data/assets/v1/profile-icons/");
	url2.append(std::to_string(icon_id) + ".jpg");

	std::string	save_folder(szPath);
	save_folder.append("\\icons.jpg");
	

	if (fopen_s(&fp, save_folder.c_str(), "wb") == 0)
	{
		bool ret = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, url2,fp);
		fclose(fp);
	}
	return	save_folder;
}
int		helper::get_all_champions() {
	all_champions.clear();
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, "/lol-game-data/assets/v1/champion-summary.json");
	try
	{
		json	js = json::parse(response);
		for (const auto& role : js) {
			CHAMPION	ch;
			ch.selectid = role["id"].get<int>();
			ch.name = wstring2string(string2wstring(role["name"].get<std::string>()));
			if (ch.selectid > 0)
			{
				all_champions.push_back(ch);
			}
		}
	}
	catch (const std::exception&)
	{
		return	0;
	}
	return	all_champions.size();
}	//获取所有英雄
void	helper::lock_champions(int	champion_id) {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	std::string response = request.request(LCU_REQUEST::RequestMethod::GET_METHOD, "/lol-champ-select/v1/session");
	int	localPlayerCellId = 0;
	try
	{
		json	js = json::parse(response);
		json	data;
		data["championId"] = champion_id;
		data["completed"] = true;
		if (js.contains("localPlayerCellId"))
		{
			for (const auto& ch : owner_champions) {
				if (ch.selectid == champion_id)
				{
					localPlayerCellId = js["localPlayerCellId"].get<int>();
					response = request.request(LCU_REQUEST::RequestMethod::PATCH_METHOD, "/lol-champ-select/v1/session/actions/", std::to_string(localPlayerCellId), data.dump());
					break;
				}
			}
		}
	}
	catch (const std::exception&)
	{
	}
}
