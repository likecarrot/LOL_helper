#pragma	once
#include	"lcu_riot/lcu_api.h"
#include	<vector>
#include	"global_.h"

namespace GAME_HELPER
{
	enum class Status
	{
		CAN, CANOT, WAIT
	};

	//战绩助手实现
	class Match_helper
	{
	public:
		Match_helper();//战绩助手实现
		~Match_helper();//战绩助手实现

	private:
	};
	class Helper_base
	{
	public:
		virtual bool run()=0;
		virtual void getconfig_for_configfiles() = 0;
	protected:
		raw_lcu_api& request = raw_lcu_api::getInstance();
	};

	//匹配助手,实现秒抢英雄 自动锁定功能
	class Classic_helper:public	Helper_base
	{
	public:
		Classic_helper();
		~Classic_helper();
		bool	run()override;
		
	private:
		void	getconfig_for_configfiles()override;

		void	set_lock_champ(int champ);
		void	locking_champs(int champ_id = 0, bool completed = true);	//锁定英雄 如果为0 就锁定在个类中保存的英雄
		bool	check_is_have_champs(int champ_id = 0);	//检查是不是拥有这个英雄		如果为0 就检查是不是拥有 lock_champ 英雄
		Status  check_is_selected_champs(int champ_id);	//检查是不是已经被其他人锁定了
		LCU_JSON_RESPONSE::LolOwnerChampions	owner_champs;
		int		lock_champ = 0;
		bool	iscompleted = false;
		int		lock_timeout = 0;
	};

	//大乱斗助手 提供 抢英雄 功能
	class Aram_helper :public	Helper_base
	{
	public:
		Aram_helper();
		~Aram_helper();
		bool	run()override;
	private:
		void	getconfig_for_configfiles()override;
		void	set_my_summoner_id();
		int		get_score_(int champ_id);
		std::vector<int>	get_all_bech_champs();	//获取所有备战席英雄
		int64_t	get_now_select_champ();	//获取我现在的英雄id
		
		int		wait_max_sec = 0;		//考虑一种情况,如果有英雄A 的好感值是2但是在其他玩家手中 现在我们选择的英雄是B 好感度是1 其他玩家有可能会选择其他英雄, 这个变量代表了 我要进行等待多少秒 中间 如果有机会拿到 就选择 比如30秒 
		std::vector<std::pair<int,int>>	score_champs;	//英雄id -- 好感度
		int		use_reroller = 1;
		int64_t	_my_summoner_id = 0;
	};
}
