#pragma once
#include	<base/base.h>
#include	<duilib/UIlib.h>
#include	"lcu_riot/struct.h"
#include	<Windows.h>
#include	"lcu_riot/config.h"
#include	"lock_champs_ui.h"
#include	"lova_champ_ui.h"
#include	"lcu_riot/lcu_api.h"
#include	"global_.h"
#include <commdlg.h>


class Helper_Setting : public ui::WindowImplBase
{
public:
	Helper_Setting();

	virtual std::wstring GetSkinFolder() override;
	virtual std::wstring GetSkinFile() override;
	virtual std::wstring GetWindowClassName() const override;

	virtual void InitWindow() override;
	void	AddSettingsConfig(Settings_Config Config);
	virtual LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	static const std::wstring kClassName;

private:
	bool	child_close_notify(ui::EventArgs* args);	//子窗口关闭后通知
	bool	OnBtnClicked(ui::EventArgs* args);
	bool	OnControlSelected(ui::EventArgs* Args);
	bool	isWhichReroller(ui::EventArgs* Args);
	void	saveToConfigJson();
	void	initAllSubControls();
	int		getIdByChampName(std::string champ_name);
	std::string	getNameByChampId(int champ_id);
	void	init_ui_for_config();
	void	foreg_get_configfiledatas();	//强制从config_settings中取数据

	ui::Label* _ui_lockChamp;	//用来标识匹配助手里需要秒锁的是谁
	ui::Button* _ui_lockAotherChamp;//选择其他英雄作为秒锁英雄
	ui::CheckBox* _ui_autoLock;	//秒锁后自动锁定
	ui::RichEdit* _ui_autoLockTimeout;//锁定延时
	ui::RichEdit* _ui_acceptTimeout;//接受对局延时
	ui::RichEdit* _ui_arammaxtimeout;//aram助手最大时长
	ui::Option* _ui_reroller1;	//骰子使用策略1	总是使用
	ui::Option* _ui_reroller2;	//骰子使用策略2	有就不使用
	ui::Option* _ui_reroller3;	//骰子使用策略3	总是不使用
	ui::Combo* _ui_lovaChampList;	//英雄喜好显示列表
	ui::Button* _ui_lovaAotherChamp;//选择其他英雄喜好
	ui::Button* _ui_loadConfig;//导入配置
	ui::Button* _ui_outputConfig;//导出配置
	ui::Button* _ui_saveConfig;//保存配置
	ui::Button* _ui_calcelConfig;//取消配置

	Settings_Config	_config;
	std::vector<CHAMP_INFO>	all_champs;
};
