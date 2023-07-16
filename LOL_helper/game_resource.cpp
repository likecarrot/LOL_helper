#include	"game_resource.h"


const std::string pre_icons = "icons_";
const std::string pre_champion_icons = "champion_icons_";

GAME_RESOURCES::GAME_RES::GAME_RES()
{
	CHAR szPath[MAX_PATH] = { 0 };
	SHGetSpecialFolderPathA(NULL, szPath, CSIDL_APPDATA, FALSE);
	AppdataFolder = szPath;
	MyFolder = AppdataFolder + "\\lol_helper";

	if (!folderExists(MyFolder)) {
		::CreateDirectoryA(MyFolder.c_str(), NULL);
	}
}

GAME_RESOURCES::GAME_RES::~GAME_RES()
{
}

std::string	GAME_RESOURCES::GAME_RES::getIconsPath(DOWN_TYPE type, int id) {

	if (type == DOWN_TYPE::CHAMPION_ICONS && id == 0)
	{
		type = DOWN_TYPE::ICONS;
		id = 29;
	}

	std::string	save_folder = MyFolder + "\\";
	std::string	file_name = std::to_string(id);

	switch (type)
	{
	case DOWN_TYPE::ICONS:
		file_name.append(".jpg");
		save_folder.append(pre_icons);
		break;
	case DOWN_TYPE::CHAMPION_ICONS:
		file_name.append(".png");
		save_folder.append(pre_champion_icons);
		break;
	}
	save_folder.append(file_name);

	{
		std::ifstream file(save_folder, std::ifstream::binary);
		if (file) {
			file.seekg(0, std::ifstream::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0, std::ifstream::beg);

			if (fileSize > 0) {
				return save_folder;
			}
		}
	}

	if (download(type, file_name, save_folder))
		return	save_folder;
	return	std::string();
}
bool	GAME_RESOURCES::GAME_RES::download(DOWN_TYPE type, std::string child_url, std::string save_name) {
	LCU_REQUEST& request = LCU_REQUEST::getInstance(); // add REQUEST as a member variable
	FILE* fp;
	std::string url;

	switch (type)
	{
	case GAME_RESOURCES::ICONS:
		url = ("/lol-game-data/assets/v1/profile-icons/");
		break;
	case GAME_RESOURCES::CHAMPION_ICONS:
		url = ("/lol-game-data/assets/v1/champion-icons/");
		break;
	}
	url.append(child_url);

	if (fopen_s(&fp, save_name.c_str(), "wb") == 0)
	{
		std::string	response;
		response=request.request(LCU_REQUEST::RequestMethod::GET_METHOD, url);
		try {
			json json_data = json::parse(response);
			std::cout << "Response is a valid JSON." << std::endl;
			fclose(fp);
			DeleteFileA(save_name.c_str());
			return	false;
		}
		catch (const nlohmann::json::parse_error&) {
			std::cout << "Response is not a valid JSON. Saving to file." << std::endl;
			fwrite(response.c_str(), 1, response.length(), fp);
		}
		fclose(fp);
		return	true;
	}
	return	false;
}


bool GAME_RESOURCES::GAME_RES::folderExists(const std::string& folderPath) {
	DWORD fileAttributes = ::GetFileAttributesA(folderPath.c_str());
	return (fileAttributes != INVALID_FILE_ATTRIBUTES && (fileAttributes & FILE_ATTRIBUTE_DIRECTORY));
}

