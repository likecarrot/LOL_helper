#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
// 从 ui::ListContainerElement 中继承所有可用功能
class Item : public ui::ListContainerElement
{
public:
	Item();
	~Item();

	// 提供外部调用来初始化 item 数据
	void InitSubControls(const std::wstring& title);

private:
	bool OnRemove(ui::EventArgs* args);

private:
	ui::Label* label_title_;
};

