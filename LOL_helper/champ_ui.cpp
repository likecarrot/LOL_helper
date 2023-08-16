#include "champ_ui.h"


Champ_Item_Ui::Champ_Item_Ui()
{
}


Champ_Item_Ui::~Champ_Item_Ui()
{
}

void Champ_Item_Ui::InitSubControls(const CHAMP_INFO data)
{
	champ_name = dynamic_cast<ui::Label*>(FindSubControl(L"champ_name"));
	champ_icon = dynamic_cast<ui::Control*>(FindSubControl(L"champ_img"));
	champ_name->SetText(string2wstring(data.champ_name));
	nbase::ThreadManager::PostTask(kThreadNetwork, nbase::Bind(&Champ_Item_Ui::SetBkImg, this, GAME_RESOURCES::GAME_RES::getInstance().getIconsPath(GAME_RESOURCES::CHAMPION_ICONS, data.champ_id)));
}

bool Champ_Item_Ui::OnRemove(ui::EventArgs* args)
{
	ui::ListBox* parent = dynamic_cast<ui::ListBox*>(this->GetParent());
	return parent->Remove(this);
}
void	Champ_Item_Ui::SetBkImg(std::string path) {
	if (!path.empty())
	{
		champ_icon->SetBkImage(string2wstring(path));
	}
}