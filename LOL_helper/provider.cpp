#include "provider.h"
#include "item.h"
#include	<base/base.h>
#include	<duilib/UIlib.h>

Provider::Provider()
{

}


Provider::~Provider()
= default;

ui::Control* Provider::CreateElement()
{
	Champ_Item_Ui* item = new Champ_Item_Ui;
	ui::GlobalManager::FillBoxWithCache(item, L"basic/selectchamp.xml");
	return item;
}

void Provider::FillElement(ui::Control *control, int index)
{
	Champ_Item_Ui* pItem = dynamic_cast<Champ_Item_Ui*>(control);
	pItem->InitSubControls(m_vTasks.at(index));
}

int Provider::GetElementtCount()
{
	// 加锁
	nbase::NAutoLock auto_lock(&lock_);
	return m_vTasks.size();
}

void Provider::SetDatas(std::vector<CHAMP_INFO> info)
{
	if (info.size()==0)
	{
		return;
	}
	// 加锁
	lock_.Lock();
	m_vTasks.clear();
	m_vTasks = info;
	lock_.Unlock();
	// 通知TileBox数据总数变动
	EmitCountChanged();
}
