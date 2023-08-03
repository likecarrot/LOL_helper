#pragma	once
#include	<Windows.h>
#include	<string>
#include	<regex>
#include	<vector>
#include	<TlHelp32.h>
#include	<mutex>
#include "lcu_riot/struct.h"
#include	"lcu_riot/request_utils.h"


enum class CHAMP_TYPE
{
	love, hate, normal
};
struct ARAM_CHAMPION
{
	CHAMP_TYPE	_type;
	int	champ_id;
	int	score;
};
class Aram_Helper
{
public:
	Aram_Helper();
	~Aram_Helper();
	std::string	output_config_json();//导出配置
	bool	input_config_json();//导入配置 返回解析结果
	void	use_reroll();		//使用骰子
	void	get_current_select_champion();	//获取现在选择的英雄
	void	get_all_bach_champ();	//获取所有备战席英雄
	void	set_champ_type();		//设置英雄的分类 比如 喜欢 一定不喜欢 和 一般
	void	change_champ_score();	//设置英雄的优先级

private:
	std::vector<CHAMPION>	bach_champions;

};
