#include "item.h"


Item::Item()
{
}


Item::~Item()
{
}

void Item::InitSubControls(const std::wstring& title)
{
	// 查找 Item 下的控件
	label_title_ = dynamic_cast<ui::Label*>(FindSubControl(L"label_title"));
	label_title_->SetText(title);
}

bool Item::OnRemove(ui::EventArgs* args)
{
	ui::ListBox* parent = dynamic_cast<ui::ListBox*>(this->GetParent());
	return parent->Remove(this);
}
