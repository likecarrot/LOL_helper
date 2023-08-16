#include	"game_helper.h"

namespace GAME_HELPER
{
	Match_helper::Match_helper()//战绩助手实现
	{
	}

	Match_helper::~Match_helper()
	{
	}

	Classic_helper::Classic_helper()
	{
		
	}

	Classic_helper::~Classic_helper()
	{
	}

	void Classic_helper::getconfig_for_configfiles()
	{
		lock_champ = config_settings.configs._classic_config.champ_id;
		iscompleted = config_settings.configs._classic_config.auto_lock;
		lock_timeout = config_settings.configs._classic_config.lockchamp_timeout;
	}

	void Classic_helper::set_lock_champ(int champ)
	{
		lock_champ = champ;
	}
	bool GAME_HELPER::Classic_helper::run()
	{
		owner_champs = request.get_owner_champions();
		getconfig_for_configfiles();
		if (lock_champ == 0)
		{
			return	false;
		}

		if (check_is_have_champs(lock_champ) && check_is_selected_champs(lock_champ) == Status::CAN)
		{
			locking_champs(lock_champ, iscompleted);
		}
		return	true;
	}
	void Classic_helper::locking_champs(int champ_id, bool completed)
	{
		if (champ_id == 0)
		{
			champ_id = lock_champ;
		}
		if (check_is_have_champs(champ_id) && check_is_selected_champs(champ_id) == Status::CAN)
		{
			if (completed==false)
			{
				request.lock_champions(champ_id, false);
			}
			else {
				if (lock_timeout==0)
				{
					request.lock_champions(champ_id, true);
				}
				else {
					request.lock_champions(champ_id, false);
					Sleep(lock_timeout * 1000);
					request.lock_champions(champ_id, true);
				}
			}
			
		}
	}

	Status Classic_helper::check_is_selected_champs(int champ_id)
	{
		auto ret = request.get_lol_champ_select_v1_session();
		auto index = ret.actions.at(0);
		for (const auto i : index) {
			if (i.champion_id != champ_id)
			{
				continue;
			}
			else {
				if (i.completed == true)
					return	Status::CANOT;
				else
					return	Status::WAIT;
			}
		}
		return Status::CAN;
	}

	bool Classic_helper::check_is_have_champs(int champ_id)
	{
		if (champ_id == 0)
		{
			champ_id = lock_champ;
		}
		for (const auto i : owner_champs)
		{
			if (i.id == champ_id)
			{
				return	true;
			}
		}
		return false;
	}




	Aram_helper::Aram_helper()
	{
		set_my_summoner_id();
	}

	Aram_helper::~Aram_helper()
	{
	}
	void Aram_helper::getconfig_for_configfiles()
	{
		wait_max_sec = config_settings.configs._aram_config.wait_lova_champs_max_sec;
		score_champs = config_settings.configs._aram_config.love_champs;
		use_reroller = config_settings.configs._aram_config.Use_Reroller;
	}
	void Aram_helper::set_my_summoner_id()
	{
		_my_summoner_id = request.get_summoner_datas().summoner_id;
	}

	int Aram_helper::get_score_(int champ_id)
	{
		for (const auto in : score_champs) {
			if (in.first==champ_id)
			{
				return	in.second;
			}
		}
		return 0;
	}

	std::vector<int> Aram_helper::get_all_bech_champs()
	{
		std::vector<int>	ret;
		auto c = request.get_lol_champ_select_v1_session();
		if (c.bench_enabled == true && c.bench_champions.size() != 0)
		{
			for (const auto index : c.bench_champions) {
				if (index.contains("championId"))
				{
					ret.push_back(index.at("championId"));
				}
			}
		}
		return ret;
	}
	int64_t Aram_helper::get_now_select_champ()
	{
		int64_t	ret = 0;
		auto c = request.get_lol_champ_select_v1_session();
		for (const auto index : c.my_team) {
			if (_my_summoner_id == index.summoner_id)
			{
				ret = index.champion_id;
			}
		}
		if (ret == 0)
		{
			set_my_summoner_id();
			for (const auto index : c.my_team) {
				if (_my_summoner_id == index.summoner_id)
				{
					ret = index.champion_id;
				}
			}
		}
		assert(ret != 0);
		return ret;
	}
	bool Aram_helper::run()
	{
		int	_last_champ = 0;//用来标识 如果是玩家 自己更改了英雄 那么 好感度再高也不切换了 直接退出这个循环
		getconfig_for_configfiles();
		int	clock_sec = 0;
		int	rolls = 0;
		while (request.get_game_status() == GAME_STATUS::ChampSelect) {
			std::map<int, int>	now_all_champs;	//指的是 我所选择的 和 备战席上的英雄 并不包括其他玩家选择的英雄 英雄id -- 好感度
			if (use_reroller==1&&rolls==0)
			{
				request.ReRoll(); rolls++;
				request.ReRoll(); rolls++;
			}//执行骰子策略

			int select = get_now_select_champ();
			if (get_score_(select)<=0)
			{
				if (use_reroller == 2 && rolls == 0)
				{
					request.ReRoll(); rolls++;
					request.ReRoll(); rolls++;
				}
			}

			now_all_champs.emplace(select, get_score_(select));
			int now_select_score = get_score_(select);
			auto champions = get_all_bech_champs();
			for (const auto index : champions) {
				int s = get_score_(index);
				now_all_champs.emplace(index, s);
			}

			// 遍历 now_all_champs 的所有键值对
			int maxKey = 0;
			int maxValue = 0;
			for (const auto pair : now_all_champs) {
				if (pair.second > maxValue) {
					maxValue = pair.second;
					maxKey = pair.first;
				}
			}
			if (maxValue!=0&&maxValue>now_select_score)
			{
				request.bench_champiosn(maxKey);
				_last_champ = maxKey;
			}
			if (wait_max_sec != 0)
			{
				clock_sec++;
				if (clock_sec > wait_max_sec)
				{
					break;
				}
			}
			if (_last_champ!=0&& _last_champ != get_now_select_champ())
			{
				OutputDebugStringA("玩家自己更改了英雄 所以提前退出了循环\n");
				break;	//退出循环
			}
			Sleep(1000);
		}
		return	true;
	}

}

