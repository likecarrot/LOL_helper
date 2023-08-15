//  To parse this JSON data, first install
//
//      Boost     http://www.boost.org
//      json.hpp  https://github.com/nlohmann/json
//
//  Then include this file, and then do
//
//     LolGameData data = nlohmann::json::parse(jsonString);
//     LolChampions data = nlohmann::json::parse(jsonString);
//     LolChat data = nlohmann::json::parse(jsonString);
//     LolGameflow data = nlohmann::json::parse(jsonString);
//     LolCollections data = nlohmann::json::parse(jsonString);
//     LolMatchHistory data = nlohmann::json::parse(jsonString);
//     LolChampSelect data = nlohmann::json::parse(jsonString);
//     LolRanked data = nlohmann::json::parse(jsonString);
//     LolSummoner data = nlohmann::json::parse(jsonString);

#pragma once

#include <boost/optional.hpp>
#include "lcu_riot/json.hpp"

#ifndef NLOHMANN_OPT_HELPER
#define NLOHMANN_OPT_HELPER
namespace nlohmann {
    template <typename T>
    struct adl_serializer<std::shared_ptr<T>> {
        static void to_json(json& j, const std::shared_ptr<T>& opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static std::shared_ptr<T> from_json(const json& j) {
            if (j.is_null()) return std::make_shared<T>(); else return std::make_shared<T>(j.get<T>());
        }
    };
    template <typename T>
    struct adl_serializer<boost::optional<T>> {
        static void to_json(json& j, const boost::optional<T>& opt) {
            if (!opt) j = nullptr; else j = *opt;
        }

        static boost::optional<T> from_json(const json& j) {
            if (j.is_null()) return boost::optional<T>(); else return boost::optional<T>(j.get<T>());
        }
    };
}
#endif

namespace LCU_JSON_RESPONSE {
    using nlohmann::json;

#ifndef NLOHMANN_UNTYPED_LCU_JSON_RESPONSE_HELPER
#define NLOHMANN_UNTYPED_LCU_JSON_RESPONSE_HELPER
    inline json get_untyped(const json& j, const char* property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json& j, std::string property) {
        return get_untyped(j, property.data());
    }
#endif

#ifndef NLOHMANN_OPTIONAL_LCU_JSON_RESPONSE_HELPER
#define NLOHMANN_OPTIONAL_LCU_JSON_RESPONSE_HELPER
    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json& j, const char* property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json& j, std::string property) {
        return get_heap_optional<T>(j, property.data());
    }
    template <typename T>
    inline boost::optional<T> get_stack_optional(const json& j, const char* property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<boost::optional<T>>();
        }
        return boost::optional<T>();
    }

    template <typename T>
    inline boost::optional<T> get_stack_optional(const json& j, std::string property) {
        return get_stack_optional<T>(j, property.data());
    }
#endif

    struct LolGameDatum {
        int64_t id;
        std::string name;
        std::string alias;
        std::string square_portrait_path;
        std::vector<std::string> roles;
    };

    struct Rental {
        int64_t end_date;
        double purchase_date;
        bool rented;
        int64_t win_count_remaining;
    };

    struct Ownership {
        bool loyalty_reward;
        bool owned;
        Rental rental;
        bool xbox_gp_reward;
    };

    struct LolOwnerChampion {
        bool active;
        std::string alias;
        std::string ban_vo_path;
        std::string base_load_screen_path;
        std::string base_splash_path;
        bool bot_enabled;
        std::string choose_vo_path;
        std::vector<std::string> disabled_queues;
        bool free_to_play;
        int64_t id;
        std::string name;
        Ownership ownership;
        double purchased;
        bool ranked_play_enabled;
        std::vector<std::string> roles;
        std::string square_portrait_path;
        std::string stinger_sfx_path;
        std::string title;
    };

    struct MucJwtDto {
        std::string channel_claim;
        std::string domain;
        std::string jwt;
        std::string target_region;
    };

    struct LolChatElement {
        std::string game_name;
        std::string game_tag;
        std::string id;
        std::string inviter_id;
        bool is_muted;
        nlohmann::json last_message;
        MucJwtDto muc_jwt_dto;
        std::string name;
        std::string password;
        std::string pid;
        std::string target_region;
        std::string type;
        int64_t unread_message_count;
    };

    struct GameClient {
        std::string observer_server_ip;
        int64_t observer_server_port;
        bool running;
        std::string server_ip;
        int64_t server_port;
        bool visible;
    };

    struct GameTypeConfig {
        bool advanced_learning_quests;
        bool allow_trades;
        std::string ban_mode;
        int64_t ban_timer_duration;
        bool battle_boost;
        bool cross_team_champion_pool;
        bool death_match;
        bool do_not_remove;
        bool duplicate_pick;
        bool exclusive_pick;
        int64_t id;
        bool learning_quests;
        int64_t main_pick_timer_duration;
        int64_t max_allowable_bans;
        std::string name;
        bool onboard_coop_beginner;
        std::string pick_mode;
        int64_t post_pick_timer_duration;
        bool reroll;
        bool team_champion_pool;
    };

    struct QueueRewards {
        bool is_champion_points_enabled;
        bool is_ip_enabled;
        bool is_xp_enabled;
        std::vector<nlohmann::json> party_size_ip_rewards;
    };

    struct Queue {
        std::vector<int64_t> allowable_premade_sizes;
        bool are_free_champions_allowed;
        std::string asset_mutator;
        std::string category;
        int64_t champions_required_to_play;
        std::string description;
        std::string detailed_description;
        std::string game_mode;
        GameTypeConfig game_type_config;
        int64_t id;
        bool is_ranked;
        bool is_team_builder_managed;
        int64_t last_toggled_off_time;
        int64_t last_toggled_on_time;
        int64_t map_id;
        int64_t maximum_participant_list_size;
        int64_t min_level;
        int64_t minimum_participant_list_size;
        std::string name;
        int64_t num_players_per_team;
        std::string queue_availability;
        QueueRewards queue_rewards;
        bool removal_from_game_allowed;
        int64_t removal_from_game_delay_minutes;
        std::string short_name;
        bool show_position_selector;
        bool spectator_enabled;
        std::string type;
    };

    struct GameData {
        int64_t game_id;
        std::string game_name;
        bool is_custom_game;
        std::string password;
        std::vector<nlohmann::json> player_champion_selections;
        Queue queue;
        bool spectators_allowed;
        std::vector<nlohmann::json> team_one;
        std::vector<nlohmann::json> team_two;
    };

    struct GameDodge {
        std::vector<nlohmann::json> dodge_ids;
        std::string phase;
        std::string state;
    };

    struct Assets {
        std::string champ_select_background_sound;
        std::string champ_select_flyout_background;
        std::string champ_select_planning_intro;
        std::string game_select_icon_active;
        std::string game_select_icon_active_video;
        std::string game_select_icon_default;
        std::string game_select_icon_disabled;
        std::string game_select_icon_hover;
        std::string game_select_icon_intro_video;
        std::string gameflow_background;
        std::string gameflow_background_dark;
        std::string gameselect_button_hover_sound;
        std::string icon_defeat;
        std::string icon_defeat_v2;
        std::string icon_defeat_video;
        std::string icon_empty;
        std::string icon_hover;
        std::string icon_leaver;
        std::string icon_leaver_v2;
        std::string icon_loss_forgiven_v2;
        std::string icon_v2;
        std::string icon_victory;
        std::string icon_victory_video;
        std::string map_north;
        std::string map_south;
        std::string music_inqueue_loop_sound;
        std::string parties_background;
        std::string postgame_ambience_loop_sound;
        std::string ready_check_background;
        std::string ready_check_background_sound;
        std::string sfx_ambience_pregame_loop_sound;
        std::string social_icon_leaver;
        std::string social_icon_victory;
    };

    struct SplitsProgress {
    };

    struct Properties {
        bool suppress_runes_masteries_perks;
    };

    struct Map {
        Assets assets;
        SplitsProgress categorized_content_bundles;
        std::string description;
        std::string game_mode;
        std::string game_mode_name;
        std::string game_mode_short_name;
        std::string game_mutator;
        int64_t id;
        bool is_rgm;
        std::string map_string_id;
        std::string name;
        SplitsProgress per_position_disallowed_summoner_spells;
        SplitsProgress per_position_required_summoner_spells;
        std::string platform_id;
        std::string platform_name;
        Properties properties;
    };

    struct LolGameflow {
        GameClient game_client;
        GameData game_data;
        GameDodge game_dodge;
        Map map;
        std::string phase;
    };

    struct Mastery {
        int64_t champion_id;
        int64_t champion_level;
        int64_t champion_points;
        int64_t champion_points_since_last_level;
        int64_t champion_points_until_next_level;
        bool chest_granted;
        std::string formatted_champion_points;
        std::string formatted_mastery_goal;
        std::string highest_grade;
        int64_t last_play_time;
        int64_t player_id;
        int64_t tokens_earned;
    };

    struct LolCollections {
        std::vector<Mastery> masteries;
        int64_t score;
        int64_t summoner_id;
    };

    struct Player {
        int64_t account_id;
        int64_t current_account_id;
        std::string current_platform_id;
        std::string match_history_uri;
        std::string platform_id;
        int64_t profile_icon;
        std::string puuid;
        int64_t summoner_id;
        std::string summoner_name;
    };

    struct ParticipantIdentity {
        int64_t participant_id;
        Player player;
    };

    struct Stats {
        int64_t assists;
        bool caused_early_surrender;
        int64_t champ_level;
        int64_t combat_player_score;
        int64_t damage_dealt_to_objectives;
        int64_t damage_dealt_to_turrets;
        int64_t damage_self_mitigated;
        int64_t deaths;
        int64_t double_kills;
        bool early_surrender_accomplice;
        bool first_blood_assist;
        bool first_blood_kill;
        bool first_inhibitor_assist;
        bool first_inhibitor_kill;
        bool first_tower_assist;
        bool first_tower_kill;
        bool game_ended_in_early_surrender;
        bool game_ended_in_surrender;
        int64_t gold_earned;
        int64_t gold_spent;
        int64_t inhibitor_kills;
        int64_t item0;
        int64_t item1;
        int64_t item2;
        int64_t item3;
        int64_t item4;
        int64_t item5;
        int64_t item6;
        int64_t killing_sprees;
        int64_t kills;
        int64_t largest_critical_strike;
        int64_t largest_killing_spree;
        int64_t largest_multi_kill;
        int64_t longest_time_spent_living;
        int64_t magic_damage_dealt;
        int64_t magic_damage_dealt_to_champions;
        int64_t magical_damage_taken;
        int64_t neutral_minions_killed;
        int64_t neutral_minions_killed_enemy_jungle;
        int64_t neutral_minions_killed_team_jungle;
        int64_t objective_player_score;
        int64_t participant_id;
        int64_t penta_kills;
        int64_t perk0;
        int64_t perk0_var1;
        int64_t perk0_var2;
        int64_t perk0_var3;
        int64_t perk1;
        int64_t perk1_var1;
        int64_t perk1_var2;
        int64_t perk1_var3;
        int64_t perk2;
        int64_t perk2_var1;
        int64_t perk2_var2;
        int64_t perk2_var3;
        int64_t perk3;
        int64_t perk3_var1;
        int64_t perk3_var2;
        int64_t perk3_var3;
        int64_t perk4;
        int64_t perk4_var1;
        int64_t perk4_var2;
        int64_t perk4_var3;
        int64_t perk5;
        int64_t perk5_var1;
        int64_t perk5_var2;
        int64_t perk5_var3;
        int64_t perk_primary_style;
        int64_t perk_sub_style;
        int64_t physical_damage_dealt;
        int64_t physical_damage_dealt_to_champions;
        int64_t physical_damage_taken;
        int64_t player_augment1;
        int64_t player_augment2;
        int64_t player_augment3;
        int64_t player_augment4;
        int64_t player_score0;
        int64_t player_score1;
        int64_t player_score2;
        int64_t player_score3;
        int64_t player_score4;
        int64_t player_score5;
        int64_t player_score6;
        int64_t player_score7;
        int64_t player_score8;
        int64_t player_score9;
        int64_t player_subteam_id;
        int64_t quadra_kills;
        int64_t sight_wards_bought_in_game;
        int64_t subteam_placement;
        bool team_early_surrendered;
        int64_t time_c_cing_others;
        int64_t total_damage_dealt;
        int64_t total_damage_dealt_to_champions;
        int64_t total_damage_taken;
        int64_t total_heal;
        int64_t total_minions_killed;
        int64_t total_player_score;
        int64_t total_score_rank;
        int64_t total_time_crowd_control_dealt;
        int64_t total_units_healed;
        int64_t triple_kills;
        int64_t true_damage_dealt;
        int64_t true_damage_dealt_to_champions;
        int64_t true_damage_taken;
        int64_t turret_kills;
        int64_t unreal_kills;
        int64_t vision_score;
        int64_t vision_wards_bought_in_game;
        int64_t wards_killed;
        int64_t wards_placed;
        bool win;
    };

    struct Timeline {
        SplitsProgress creeps_per_min_deltas;
        SplitsProgress cs_diff_per_min_deltas;
        SplitsProgress damage_taken_diff_per_min_deltas;
        SplitsProgress damage_taken_per_min_deltas;
        SplitsProgress gold_per_min_deltas;
        std::string lane;
        int64_t participant_id;
        std::string role;
        SplitsProgress xp_diff_per_min_deltas;
        SplitsProgress xp_per_min_deltas;
    };

    struct Participant {
        int64_t champion_id;
        std::string highest_achieved_season_tier;
        int64_t participant_id;
        int64_t spell1_id;
        int64_t spell2_id;
        Stats stats;
        int64_t team_id;
        Timeline timeline;
    };

    struct Team {
        std::vector<nlohmann::json> bans;
        int64_t baron_kills;
        int64_t dominion_victory_score;
        int64_t dragon_kills;
        bool first_baron;
        bool first_blood;
        bool first_dargon;
        bool first_inhibitor;
        bool first_tower;
        int64_t inhibitor_kills;
        int64_t rift_herald_kills;
        int64_t team_id;
        int64_t tower_kills;
        int64_t vilemaw_kills;
        std::string win;
    };

    struct Games;

    struct LolMatchHistory;

    struct Games {
        std::string game_begin_date;
        int64_t game_count;
        std::string game_end_date;
        int64_t game_index_begin;
        int64_t game_index_end;
        std::vector<LolMatchHistory> games;
    };

    struct LolMatchHistory {
        boost::optional<int64_t> account_id;
        std::shared_ptr<Games> games;
        std::string platform_id;
        boost::optional<int64_t> game_creation;
        boost::optional<std::string> game_creation_date;
        boost::optional<int64_t> game_duration;
        boost::optional<int64_t> game_id;
        boost::optional<std::string> game_mode;
        boost::optional<std::string> game_type;
        boost::optional<std::string> game_version;
        boost::optional<int64_t> map_id;
        boost::optional<std::vector<ParticipantIdentity>> participant_identities;
        boost::optional<std::vector<Participant>> participants;
        boost::optional<int64_t> queue_id;
        boost::optional<int64_t> season_id;
        boost::optional<std::vector<Team>> teams;
    };

    struct Action {
        int64_t actor_cell_id;
        int64_t champion_id;
        bool completed;
        int64_t id;
        bool is_ally_action;
        bool is_in_progress;
        int64_t pick_turn;
        std::string type;
    };

    struct Bans {
        std::vector<nlohmann::json> my_team_bans;
        int64_t num_bans;
        std::vector<nlohmann::json> their_team_bans;
    };

    struct ChatDetails {
        MucJwtDto muc_jwt_dto;
        std::string multi_user_chat_id;
        std::string multi_user_chat_password;
    };

    struct EntitledFeatureState {
        int64_t additional_rerolls;
        std::vector<nlohmann::json> unlocked_skin_ids;
    };

    struct MyTeam {
        std::string assigned_position;
        int64_t cell_id;
        int64_t champion_id;
        int64_t champion_pick_intent;
        std::string entitled_feature_type;
        std::string name_visibility_type;
        std::string obfuscated_puuid;
        int64_t obfuscated_summoner_id;
        std::string puuid;
        int64_t selected_skin_id;
        int64_t spell1_id;
        int64_t spell2_id;
        int64_t summoner_id;
        int64_t team;
        int64_t ward_skin_id;
        std::string display_name;   //这里是自己加上去的成员 目的是为了再其他窗口显示的时候可以快速获取名称
    };

    struct Timer {
        int64_t adjusted_time_left_in_phase;
        int64_t internal_now_in_epoch_ms;
        bool is_infinite;
        std::string phase;
        int64_t total_time_in_phase;
    };

    struct LolChampSelect {
        std::vector<std::vector<Action>> actions;
        bool allow_battle_boost;
        bool allow_duplicate_picks;
        bool allow_locked_events;
        bool allow_rerolling;
        bool allow_skin_selection;
        Bans bans;
        std::vector<nlohmann::json> bench_champions;
        bool bench_enabled;
        int64_t boostable_skin_count;
        ChatDetails chat_details;
        int64_t counter;
        EntitledFeatureState entitled_feature_state;
        int64_t game_id;
        bool has_simultaneous_bans;
        bool has_simultaneous_picks;
        bool is_custom_game;
        bool is_spectating;
        int64_t local_player_cell_id;
        int64_t locked_event_index;
        std::vector<MyTeam> my_team;
        std::vector<nlohmann::json> pick_order_swaps;
        int64_t recovery_counter;
        int64_t rerolls_remaining;
        bool skip_champion_select;
        std::vector<nlohmann::json> their_team;
        Timer timer;
        std::vector<nlohmann::json> trades;
    };

    struct HighestRankedEntry {
        std::string division;
        std::string highest_division;
        std::string highest_tier;
        bool is_provisional;
        int64_t league_points;
        int64_t losses;
        std::string mini_series_progress;
        std::string previous_season_achieved_division;
        std::string previous_season_achieved_tier;
        std::string previous_season_end_division;
        std::string previous_season_end_tier;
        int64_t provisional_game_threshold;
        int64_t provisional_games_remaining;
        std::string queue_type;
        int64_t rated_rating;
        std::string rated_tier;
        std::string tier;
        nlohmann::json warnings;
        int64_t wins;
    };

    struct QueueMap {
        HighestRankedEntry cherry;
        HighestRankedEntry ranked_flex_sr;
        HighestRankedEntry ranked_solo_5_x5;
        HighestRankedEntry ranked_tft;
        HighestRankedEntry ranked_tft_double_up;
        HighestRankedEntry ranked_tft_turbo;
    };

    struct Cherry {
        int64_t current_season_end;
        int64_t current_season_id;
        int64_t next_season_start;
    };

    struct Seasons {
        Cherry cherry;
        Cherry ranked_flex_sr;
        Cherry ranked_solo_5_x5;
        Cherry ranked_tft;
        Cherry ranked_tft_double_up;
        Cherry ranked_tft_turbo;
    };

    struct LolRankedData {
        std::vector<nlohmann::json> earned_regalia_reward_ids;
        std::string highest_current_season_reached_tier_sr;
        std::string highest_previous_season_achieved_division;
        std::string highest_previous_season_achieved_tier;
        std::string highest_previous_season_end_division;
        std::string highest_previous_season_end_tier;
        HighestRankedEntry highest_ranked_entry;
        HighestRankedEntry highest_ranked_entry_sr;
        QueueMap queue_map;
        std::vector<HighestRankedEntry> queues;
        int64_t ranked_regalia_level;
        Seasons seasons;
        SplitsProgress splits_progress;
    };

    struct RerollPoints {
        int64_t current_points;
        int64_t max_rolls;
        int64_t number_of_rolls;
        int64_t points_cost_to_roll;
        int64_t points_to_reroll;
    };

    struct LolSummonerInfo {
        int64_t account_id;
        std::string display_name;
        std::string internal_name;
        bool name_change_flag;
        int64_t percent_complete_for_next_level;
        std::string privacy;
        int64_t profile_icon_id;
        std::string puuid;
        RerollPoints reroll_points;
        int64_t summoner_id;
        int64_t summoner_level;
        bool unnamed;
        int64_t xp_since_last_level;
        int64_t xp_until_next_level;
    };

    using LolGameDataChampSummary = std::vector<LolGameDatum>;
    using LolOwnerChampions = std::vector<LolOwnerChampion>;
    using LolChat = std::vector<LolChatElement>;
}

namespace LCU_JSON_RESPONSE {
    void from_json(const json& j, LolGameDatum& x);
    void to_json(json& j, const LolGameDatum& x);

    void from_json(const json& j, Rental& x);
    void to_json(json& j, const Rental& x);

    void from_json(const json& j, Ownership& x);
    void to_json(json& j, const Ownership& x);

    void from_json(const json& j, LolOwnerChampion& x);
    void to_json(json& j, const LolOwnerChampion& x);

    void from_json(const json& j, MucJwtDto& x);
    void to_json(json& j, const MucJwtDto& x);

    void from_json(const json& j, LolChatElement& x);
    void to_json(json& j, const LolChatElement& x);

    void from_json(const json& j, GameClient& x);
    void to_json(json& j, const GameClient& x);

    void from_json(const json& j, GameTypeConfig& x);
    void to_json(json& j, const GameTypeConfig& x);

    void from_json(const json& j, QueueRewards& x);
    void to_json(json& j, const QueueRewards& x);

    void from_json(const json& j, Queue& x);
    void to_json(json& j, const Queue& x);

    void from_json(const json& j, GameData& x);
    void to_json(json& j, const GameData& x);

    void from_json(const json& j, GameDodge& x);
    void to_json(json& j, const GameDodge& x);

    void from_json(const json& j, Assets& x);
    void to_json(json& j, const Assets& x);

    void from_json(const json& j, SplitsProgress& x);
    void to_json(json& j, const SplitsProgress& x);

    void from_json(const json& j, Properties& x);
    void to_json(json& j, const Properties& x);

    void from_json(const json& j, Map& x);
    void to_json(json& j, const Map& x);

    void from_json(const json& j, LolGameflow& x);
    void to_json(json& j, const LolGameflow& x);

    void from_json(const json& j, Mastery& x);
    void to_json(json& j, const Mastery& x);

    void from_json(const json& j, LolCollections& x);
    void to_json(json& j, const LolCollections& x);

    void from_json(const json& j, Player& x);
    void to_json(json& j, const Player& x);

    void from_json(const json& j, ParticipantIdentity& x);
    void to_json(json& j, const ParticipantIdentity& x);

    void from_json(const json& j, Stats& x);
    void to_json(json& j, const Stats& x);

    void from_json(const json& j, Timeline& x);
    void to_json(json& j, const Timeline& x);

    void from_json(const json& j, Participant& x);
    void to_json(json& j, const Participant& x);

    void from_json(const json& j, Team& x);
    void to_json(json& j, const Team& x);

    void from_json(const json& j, Games& x);
    void to_json(json& j, const Games& x);

    void from_json(const json& j, LolMatchHistory& x);
    void to_json(json& j, const LolMatchHistory& x);

    void from_json(const json& j, Action& x);
    void to_json(json& j, const Action& x);

    void from_json(const json& j, Bans& x);
    void to_json(json& j, const Bans& x);

    void from_json(const json& j, ChatDetails& x);
    void to_json(json& j, const ChatDetails& x);

    void from_json(const json& j, EntitledFeatureState& x);
    void to_json(json& j, const EntitledFeatureState& x);

    void from_json(const json& j, MyTeam& x);
    void to_json(json& j, const MyTeam& x);

    void from_json(const json& j, Timer& x);
    void to_json(json& j, const Timer& x);

    void from_json(const json& j, LolChampSelect& x);
    void to_json(json& j, const LolChampSelect& x);

    void from_json(const json& j, HighestRankedEntry& x);
    void to_json(json& j, const HighestRankedEntry& x);

    void from_json(const json& j, QueueMap& x);
    void to_json(json& j, const QueueMap& x);

    void from_json(const json& j, Cherry& x);
    void to_json(json& j, const Cherry& x);

    void from_json(const json& j, Seasons& x);
    void to_json(json& j, const Seasons& x);

    void from_json(const json& j, LolRankedData& x);
    void to_json(json& j, const LolRankedData& x);

    void from_json(const json& j, RerollPoints& x);
    void to_json(json& j, const RerollPoints& x);

    void from_json(const json& j, LolSummonerInfo& x);
    void to_json(json& j, const LolSummonerInfo& x);

    inline void from_json(const json& j, LolGameDatum& x) {
        x.id = j.at("id").get<int64_t>();
        x.name = j.at("name").get<std::string>();
        x.alias = j.at("alias").get<std::string>();
        x.square_portrait_path = j.at("squarePortraitPath").get<std::string>();
        x.roles = j.at("roles").get<std::vector<std::string>>();
    }

    inline void to_json(json& j, const LolGameDatum& x) {
        j = json::object();
        j["id"] = x.id;
        j["name"] = x.name;
        j["alias"] = x.alias;
        j["squarePortraitPath"] = x.square_portrait_path;
        j["roles"] = x.roles;
    }

    inline void from_json(const json& j, Rental& x) {
        x.end_date = j.at("endDate").get<int64_t>();
        x.purchase_date = j.at("purchaseDate").get<double>();
        x.rented = j.at("rented").get<bool>();
        x.win_count_remaining = j.at("winCountRemaining").get<int64_t>();
    }

    inline void to_json(json& j, const Rental& x) {
        j = json::object();
        j["endDate"] = x.end_date;
        j["purchaseDate"] = x.purchase_date;
        j["rented"] = x.rented;
        j["winCountRemaining"] = x.win_count_remaining;
    }

    inline void from_json(const json& j, Ownership& x) {
        x.loyalty_reward = j.at("loyaltyReward").get<bool>();
        x.owned = j.at("owned").get<bool>();
        x.rental = j.at("rental").get<Rental>();
        x.xbox_gp_reward = j.at("xboxGPReward").get<bool>();
    }

    inline void to_json(json& j, const Ownership& x) {
        j = json::object();
        j["loyaltyReward"] = x.loyalty_reward;
        j["owned"] = x.owned;
        j["rental"] = x.rental;
        j["xboxGPReward"] = x.xbox_gp_reward;
    }

    inline void from_json(const json& j, LolOwnerChampion& x) {
        x.active = j.at("active").get<bool>();
        x.alias = j.at("alias").get<std::string>();
        x.ban_vo_path = j.at("banVoPath").get<std::string>();
        x.base_load_screen_path = j.at("baseLoadScreenPath").get<std::string>();
        x.base_splash_path = j.at("baseSplashPath").get<std::string>();
        x.bot_enabled = j.at("botEnabled").get<bool>();
        x.choose_vo_path = j.at("chooseVoPath").get<std::string>();
        x.disabled_queues = j.at("disabledQueues").get<std::vector<std::string>>();
        x.free_to_play = j.at("freeToPlay").get<bool>();
        x.id = j.at("id").get<int64_t>();
        x.name = j.at("name").get<std::string>();
        x.ownership = j.at("ownership").get<Ownership>();
        x.purchased = j.at("purchased").get<double>();
        x.ranked_play_enabled = j.at("rankedPlayEnabled").get<bool>();
        x.roles = j.at("roles").get<std::vector<std::string>>();
        x.square_portrait_path = j.at("squarePortraitPath").get<std::string>();
        x.stinger_sfx_path = j.at("stingerSfxPath").get<std::string>();
        x.title = j.at("title").get<std::string>();
    }

    inline void to_json(json& j, const LolOwnerChampion& x) {
        j = json::object();
        j["active"] = x.active;
        j["alias"] = x.alias;
        j["banVoPath"] = x.ban_vo_path;
        j["baseLoadScreenPath"] = x.base_load_screen_path;
        j["baseSplashPath"] = x.base_splash_path;
        j["botEnabled"] = x.bot_enabled;
        j["chooseVoPath"] = x.choose_vo_path;
        j["disabledQueues"] = x.disabled_queues;
        j["freeToPlay"] = x.free_to_play;
        j["id"] = x.id;
        j["name"] = x.name;
        j["ownership"] = x.ownership;
        j["purchased"] = x.purchased;
        j["rankedPlayEnabled"] = x.ranked_play_enabled;
        j["roles"] = x.roles;
        j["squarePortraitPath"] = x.square_portrait_path;
        j["stingerSfxPath"] = x.stinger_sfx_path;
        j["title"] = x.title;
    }

    inline void from_json(const json& j, MucJwtDto& x) {
        x.channel_claim = j.at("channelClaim").get<std::string>();
        x.domain = j.at("domain").get<std::string>();
        x.jwt = j.at("jwt").get<std::string>();
        x.target_region = j.at("targetRegion").get<std::string>();
    }

    inline void to_json(json& j, const MucJwtDto& x) {
        j = json::object();
        j["channelClaim"] = x.channel_claim;
        j["domain"] = x.domain;
        j["jwt"] = x.jwt;
        j["targetRegion"] = x.target_region;
    }

    inline void from_json(const json& j, LolChatElement& x) {
        x.game_name = j.at("gameName").get<std::string>();
        x.game_tag = j.at("gameTag").get<std::string>();
        x.id = j.at("id").get<std::string>();
        x.inviter_id = j.at("inviterId").get<std::string>();
        x.is_muted = j.at("isMuted").get<bool>();
        x.last_message = get_untyped(j, "lastMessage");
        x.muc_jwt_dto = j.at("mucJwtDto").get<MucJwtDto>();
        x.name = j.at("name").get<std::string>();
        x.password = j.at("password").get<std::string>();
        x.pid = j.at("pid").get<std::string>();
        x.target_region = j.at("targetRegion").get<std::string>();
        x.type = j.at("type").get<std::string>();
        x.unread_message_count = j.at("unreadMessageCount").get<int64_t>();
    }

    inline void to_json(json& j, const LolChatElement& x) {
        j = json::object();
        j["gameName"] = x.game_name;
        j["gameTag"] = x.game_tag;
        j["id"] = x.id;
        j["inviterId"] = x.inviter_id;
        j["isMuted"] = x.is_muted;
        j["lastMessage"] = x.last_message;
        j["mucJwtDto"] = x.muc_jwt_dto;
        j["name"] = x.name;
        j["password"] = x.password;
        j["pid"] = x.pid;
        j["targetRegion"] = x.target_region;
        j["type"] = x.type;
        j["unreadMessageCount"] = x.unread_message_count;
    }

    inline void from_json(const json& j, GameClient& x) {
        x.observer_server_ip = j.at("observerServerIp").get<std::string>();
        x.observer_server_port = j.at("observerServerPort").get<int64_t>();
        x.running = j.at("running").get<bool>();
        x.server_ip = j.at("serverIp").get<std::string>();
        x.server_port = j.at("serverPort").get<int64_t>();
        x.visible = j.at("visible").get<bool>();
    }

    inline void to_json(json& j, const GameClient& x) {
        j = json::object();
        j["observerServerIp"] = x.observer_server_ip;
        j["observerServerPort"] = x.observer_server_port;
        j["running"] = x.running;
        j["serverIp"] = x.server_ip;
        j["serverPort"] = x.server_port;
        j["visible"] = x.visible;
    }

    inline void from_json(const json& j, GameTypeConfig& x) {
        x.advanced_learning_quests = j.at("advancedLearningQuests").get<bool>();
        x.allow_trades = j.at("allowTrades").get<bool>();
        x.ban_mode = j.at("banMode").get<std::string>();
        x.ban_timer_duration = j.at("banTimerDuration").get<int64_t>();
        x.battle_boost = j.at("battleBoost").get<bool>();
        x.cross_team_champion_pool = j.at("crossTeamChampionPool").get<bool>();
        x.death_match = j.at("deathMatch").get<bool>();
        x.do_not_remove = j.at("doNotRemove").get<bool>();
        x.duplicate_pick = j.at("duplicatePick").get<bool>();
        x.exclusive_pick = j.at("exclusivePick").get<bool>();
        x.id = j.at("id").get<int64_t>();
        x.learning_quests = j.at("learningQuests").get<bool>();
        x.main_pick_timer_duration = j.at("mainPickTimerDuration").get<int64_t>();
        x.max_allowable_bans = j.at("maxAllowableBans").get<int64_t>();
        x.name = j.at("name").get<std::string>();
        x.onboard_coop_beginner = j.at("onboardCoopBeginner").get<bool>();
        x.pick_mode = j.at("pickMode").get<std::string>();
        x.post_pick_timer_duration = j.at("postPickTimerDuration").get<int64_t>();
        x.reroll = j.at("reroll").get<bool>();
        x.team_champion_pool = j.at("teamChampionPool").get<bool>();
    }

    inline void to_json(json& j, const GameTypeConfig& x) {
        j = json::object();
        j["advancedLearningQuests"] = x.advanced_learning_quests;
        j["allowTrades"] = x.allow_trades;
        j["banMode"] = x.ban_mode;
        j["banTimerDuration"] = x.ban_timer_duration;
        j["battleBoost"] = x.battle_boost;
        j["crossTeamChampionPool"] = x.cross_team_champion_pool;
        j["deathMatch"] = x.death_match;
        j["doNotRemove"] = x.do_not_remove;
        j["duplicatePick"] = x.duplicate_pick;
        j["exclusivePick"] = x.exclusive_pick;
        j["id"] = x.id;
        j["learningQuests"] = x.learning_quests;
        j["mainPickTimerDuration"] = x.main_pick_timer_duration;
        j["maxAllowableBans"] = x.max_allowable_bans;
        j["name"] = x.name;
        j["onboardCoopBeginner"] = x.onboard_coop_beginner;
        j["pickMode"] = x.pick_mode;
        j["postPickTimerDuration"] = x.post_pick_timer_duration;
        j["reroll"] = x.reroll;
        j["teamChampionPool"] = x.team_champion_pool;
    }

    inline void from_json(const json& j, QueueRewards& x) {
        x.is_champion_points_enabled = j.at("isChampionPointsEnabled").get<bool>();
        x.is_ip_enabled = j.at("isIpEnabled").get<bool>();
        x.is_xp_enabled = j.at("isXpEnabled").get<bool>();
        x.party_size_ip_rewards = j.at("partySizeIpRewards").get<std::vector<nlohmann::json>>();
    }

    inline void to_json(json& j, const QueueRewards& x) {
        j = json::object();
        j["isChampionPointsEnabled"] = x.is_champion_points_enabled;
        j["isIpEnabled"] = x.is_ip_enabled;
        j["isXpEnabled"] = x.is_xp_enabled;
        j["partySizeIpRewards"] = x.party_size_ip_rewards;
    }

    inline void from_json(const json& j, Queue& x) {
        x.allowable_premade_sizes = j.at("allowablePremadeSizes").get<std::vector<int64_t>>();
        x.are_free_champions_allowed = j.at("areFreeChampionsAllowed").get<bool>();
        x.asset_mutator = j.at("assetMutator").get<std::string>();
        x.category = j.at("category").get<std::string>();
        x.champions_required_to_play = j.at("championsRequiredToPlay").get<int64_t>();
        x.description = j.at("description").get<std::string>();
        x.detailed_description = j.at("detailedDescription").get<std::string>();
        x.game_mode = j.at("gameMode").get<std::string>();
        x.game_type_config = j.at("gameTypeConfig").get<GameTypeConfig>();
        x.id = j.at("id").get<int64_t>();
        x.is_ranked = j.at("isRanked").get<bool>();
        x.is_team_builder_managed = j.at("isTeamBuilderManaged").get<bool>();
        x.last_toggled_off_time = j.at("lastToggledOffTime").get<int64_t>();
        x.last_toggled_on_time = j.at("lastToggledOnTime").get<int64_t>();
        x.map_id = j.at("mapId").get<int64_t>();
        x.maximum_participant_list_size = j.at("maximumParticipantListSize").get<int64_t>();
        x.min_level = j.at("minLevel").get<int64_t>();
        x.minimum_participant_list_size = j.at("minimumParticipantListSize").get<int64_t>();
        x.name = j.at("name").get<std::string>();
        x.num_players_per_team = j.at("numPlayersPerTeam").get<int64_t>();
        x.queue_availability = j.at("queueAvailability").get<std::string>();
        x.queue_rewards = j.at("queueRewards").get<QueueRewards>();
        x.removal_from_game_allowed = j.at("removalFromGameAllowed").get<bool>();
        x.removal_from_game_delay_minutes = j.at("removalFromGameDelayMinutes").get<int64_t>();
        x.short_name = j.at("shortName").get<std::string>();
        x.show_position_selector = j.at("showPositionSelector").get<bool>();
        x.spectator_enabled = j.at("spectatorEnabled").get<bool>();
        x.type = j.at("type").get<std::string>();
    }

    inline void to_json(json& j, const Queue& x) {
        j = json::object();
        j["allowablePremadeSizes"] = x.allowable_premade_sizes;
        j["areFreeChampionsAllowed"] = x.are_free_champions_allowed;
        j["assetMutator"] = x.asset_mutator;
        j["category"] = x.category;
        j["championsRequiredToPlay"] = x.champions_required_to_play;
        j["description"] = x.description;
        j["detailedDescription"] = x.detailed_description;
        j["gameMode"] = x.game_mode;
        j["gameTypeConfig"] = x.game_type_config;
        j["id"] = x.id;
        j["isRanked"] = x.is_ranked;
        j["isTeamBuilderManaged"] = x.is_team_builder_managed;
        j["lastToggledOffTime"] = x.last_toggled_off_time;
        j["lastToggledOnTime"] = x.last_toggled_on_time;
        j["mapId"] = x.map_id;
        j["maximumParticipantListSize"] = x.maximum_participant_list_size;
        j["minLevel"] = x.min_level;
        j["minimumParticipantListSize"] = x.minimum_participant_list_size;
        j["name"] = x.name;
        j["numPlayersPerTeam"] = x.num_players_per_team;
        j["queueAvailability"] = x.queue_availability;
        j["queueRewards"] = x.queue_rewards;
        j["removalFromGameAllowed"] = x.removal_from_game_allowed;
        j["removalFromGameDelayMinutes"] = x.removal_from_game_delay_minutes;
        j["shortName"] = x.short_name;
        j["showPositionSelector"] = x.show_position_selector;
        j["spectatorEnabled"] = x.spectator_enabled;
        j["type"] = x.type;
    }

    inline void from_json(const json& j, GameData& x) {
        x.game_id = j.at("gameId").get<int64_t>();
        x.game_name = j.at("gameName").get<std::string>();
        x.is_custom_game = j.at("isCustomGame").get<bool>();
        x.password = j.at("password").get<std::string>();
        x.player_champion_selections = j.at("playerChampionSelections").get<std::vector<nlohmann::json>>();
        x.queue = j.at("queue").get<Queue>();
        x.spectators_allowed = j.at("spectatorsAllowed").get<bool>();
        x.team_one = j.at("teamOne").get<std::vector<nlohmann::json>>();
        x.team_two = j.at("teamTwo").get<std::vector<nlohmann::json>>();
    }

    inline void to_json(json& j, const GameData& x) {
        j = json::object();
        j["gameId"] = x.game_id;
        j["gameName"] = x.game_name;
        j["isCustomGame"] = x.is_custom_game;
        j["password"] = x.password;
        j["playerChampionSelections"] = x.player_champion_selections;
        j["queue"] = x.queue;
        j["spectatorsAllowed"] = x.spectators_allowed;
        j["teamOne"] = x.team_one;
        j["teamTwo"] = x.team_two;
    }

    inline void from_json(const json& j, GameDodge& x) {
        x.dodge_ids = j.at("dodgeIds").get<std::vector<nlohmann::json>>();
        x.phase = j.at("phase").get<std::string>();
        x.state = j.at("state").get<std::string>();
    }

    inline void to_json(json& j, const GameDodge& x) {
        j = json::object();
        j["dodgeIds"] = x.dodge_ids;
        j["phase"] = x.phase;
        j["state"] = x.state;
    }

    inline void from_json(const json& j, Assets& x) {
        x.champ_select_background_sound = j.at("champ-select-background-sound").get<std::string>();
        x.champ_select_flyout_background = j.at("champ-select-flyout-background").get<std::string>();
        x.champ_select_planning_intro = j.at("champ-select-planning-intro").get<std::string>();
        x.game_select_icon_active = j.at("game-select-icon-active").get<std::string>();
        x.game_select_icon_active_video = j.at("game-select-icon-active-video").get<std::string>();
        x.game_select_icon_default = j.at("game-select-icon-default").get<std::string>();
        x.game_select_icon_disabled = j.at("game-select-icon-disabled").get<std::string>();
        x.game_select_icon_hover = j.at("game-select-icon-hover").get<std::string>();
        x.game_select_icon_intro_video = j.at("game-select-icon-intro-video").get<std::string>();
        x.gameflow_background = j.at("gameflow-background").get<std::string>();
        x.gameflow_background_dark = j.at("gameflow-background-dark").get<std::string>();
        x.gameselect_button_hover_sound = j.at("gameselect-button-hover-sound").get<std::string>();
        x.icon_defeat = j.at("icon-defeat").get<std::string>();
        x.icon_defeat_v2 = j.at("icon-defeat-v2").get<std::string>();
        x.icon_defeat_video = j.at("icon-defeat-video").get<std::string>();
        x.icon_empty = j.at("icon-empty").get<std::string>();
        x.icon_hover = j.at("icon-hover").get<std::string>();
        x.icon_leaver = j.at("icon-leaver").get<std::string>();
        x.icon_leaver_v2 = j.at("icon-leaver-v2").get<std::string>();
        x.icon_loss_forgiven_v2 = j.at("icon-loss-forgiven-v2").get<std::string>();
        x.icon_v2 = j.at("icon-v2").get<std::string>();
        x.icon_victory = j.at("icon-victory").get<std::string>();
        x.icon_victory_video = j.at("icon-victory-video").get<std::string>();
        x.map_north = j.at("map-north").get<std::string>();
        x.map_south = j.at("map-south").get<std::string>();
        x.music_inqueue_loop_sound = j.at("music-inqueue-loop-sound").get<std::string>();
        x.parties_background = j.at("parties-background").get<std::string>();
        x.postgame_ambience_loop_sound = j.at("postgame-ambience-loop-sound").get<std::string>();
        x.ready_check_background = j.at("ready-check-background").get<std::string>();
        x.ready_check_background_sound = j.at("ready-check-background-sound").get<std::string>();
        x.sfx_ambience_pregame_loop_sound = j.at("sfx-ambience-pregame-loop-sound").get<std::string>();
        x.social_icon_leaver = j.at("social-icon-leaver").get<std::string>();
        x.social_icon_victory = j.at("social-icon-victory").get<std::string>();
    }

    inline void to_json(json& j, const Assets& x) {
        j = json::object();
        j["champ-select-background-sound"] = x.champ_select_background_sound;
        j["champ-select-flyout-background"] = x.champ_select_flyout_background;
        j["champ-select-planning-intro"] = x.champ_select_planning_intro;
        j["game-select-icon-active"] = x.game_select_icon_active;
        j["game-select-icon-active-video"] = x.game_select_icon_active_video;
        j["game-select-icon-default"] = x.game_select_icon_default;
        j["game-select-icon-disabled"] = x.game_select_icon_disabled;
        j["game-select-icon-hover"] = x.game_select_icon_hover;
        j["game-select-icon-intro-video"] = x.game_select_icon_intro_video;
        j["gameflow-background"] = x.gameflow_background;
        j["gameflow-background-dark"] = x.gameflow_background_dark;
        j["gameselect-button-hover-sound"] = x.gameselect_button_hover_sound;
        j["icon-defeat"] = x.icon_defeat;
        j["icon-defeat-v2"] = x.icon_defeat_v2;
        j["icon-defeat-video"] = x.icon_defeat_video;
        j["icon-empty"] = x.icon_empty;
        j["icon-hover"] = x.icon_hover;
        j["icon-leaver"] = x.icon_leaver;
        j["icon-leaver-v2"] = x.icon_leaver_v2;
        j["icon-loss-forgiven-v2"] = x.icon_loss_forgiven_v2;
        j["icon-v2"] = x.icon_v2;
        j["icon-victory"] = x.icon_victory;
        j["icon-victory-video"] = x.icon_victory_video;
        j["map-north"] = x.map_north;
        j["map-south"] = x.map_south;
        j["music-inqueue-loop-sound"] = x.music_inqueue_loop_sound;
        j["parties-background"] = x.parties_background;
        j["postgame-ambience-loop-sound"] = x.postgame_ambience_loop_sound;
        j["ready-check-background"] = x.ready_check_background;
        j["ready-check-background-sound"] = x.ready_check_background_sound;
        j["sfx-ambience-pregame-loop-sound"] = x.sfx_ambience_pregame_loop_sound;
        j["social-icon-leaver"] = x.social_icon_leaver;
        j["social-icon-victory"] = x.social_icon_victory;
    }

    inline void from_json(const json& j, SplitsProgress& x) {
    }

    inline void to_json(json& j, const SplitsProgress& x) {
        j = json::object();
    }

    inline void from_json(const json& j, Properties& x) {
        x.suppress_runes_masteries_perks = j.at("suppressRunesMasteriesPerks").get<bool>();
    }

    inline void to_json(json& j, const Properties& x) {
        j = json::object();
        j["suppressRunesMasteriesPerks"] = x.suppress_runes_masteries_perks;
    }

    inline void from_json(const json& j, Map& x) {
        x.assets = j.at("assets").get<Assets>();
        x.categorized_content_bundles = j.at("categorizedContentBundles").get<SplitsProgress>();
        x.description = j.at("description").get<std::string>();
        x.game_mode = j.at("gameMode").get<std::string>();
        x.game_mode_name = j.at("gameModeName").get<std::string>();
        x.game_mode_short_name = j.at("gameModeShortName").get<std::string>();
        x.game_mutator = j.at("gameMutator").get<std::string>();
        x.id = j.at("id").get<int64_t>();
        x.is_rgm = j.at("isRGM").get<bool>();
        x.map_string_id = j.at("mapStringId").get<std::string>();
        x.name = j.at("name").get<std::string>();
        x.per_position_disallowed_summoner_spells = j.at("perPositionDisallowedSummonerSpells").get<SplitsProgress>();
        x.per_position_required_summoner_spells = j.at("perPositionRequiredSummonerSpells").get<SplitsProgress>();
        x.platform_id = j.at("platformId").get<std::string>();
        x.platform_name = j.at("platformName").get<std::string>();
        x.properties = j.at("properties").get<Properties>();
    }

    inline void to_json(json& j, const Map& x) {
        j = json::object();
        j["assets"] = x.assets;
        j["categorizedContentBundles"] = x.categorized_content_bundles;
        j["description"] = x.description;
        j["gameMode"] = x.game_mode;
        j["gameModeName"] = x.game_mode_name;
        j["gameModeShortName"] = x.game_mode_short_name;
        j["gameMutator"] = x.game_mutator;
        j["id"] = x.id;
        j["isRGM"] = x.is_rgm;
        j["mapStringId"] = x.map_string_id;
        j["name"] = x.name;
        j["perPositionDisallowedSummonerSpells"] = x.per_position_disallowed_summoner_spells;
        j["perPositionRequiredSummonerSpells"] = x.per_position_required_summoner_spells;
        j["platformId"] = x.platform_id;
        j["platformName"] = x.platform_name;
        j["properties"] = x.properties;
    }

    inline void from_json(const json& j, LolGameflow& x) {
        x.game_client = j.at("gameClient").get<GameClient>();
        x.game_data = j.at("gameData").get<GameData>();
        x.game_dodge = j.at("gameDodge").get<GameDodge>();
        x.map = j.at("map").get<Map>();
        x.phase = j.at("phase").get<std::string>();
    }

    inline void to_json(json& j, const LolGameflow& x) {
        j = json::object();
        j["gameClient"] = x.game_client;
        j["gameData"] = x.game_data;
        j["gameDodge"] = x.game_dodge;
        j["map"] = x.map;
        j["phase"] = x.phase;
    }

    inline void from_json(const json& j, Mastery& x) {
        x.champion_id = j.at("championId").get<int64_t>();
        x.champion_level = j.at("championLevel").get<int64_t>();
        x.champion_points = j.at("championPoints").get<int64_t>();
        x.champion_points_since_last_level = j.at("championPointsSinceLastLevel").get<int64_t>();
        x.champion_points_until_next_level = j.at("championPointsUntilNextLevel").get<int64_t>();
        x.chest_granted = j.at("chestGranted").get<bool>();
        x.formatted_champion_points = j.at("formattedChampionPoints").get<std::string>();
        x.formatted_mastery_goal = j.at("formattedMasteryGoal").get<std::string>();
        x.highest_grade = j.at("highestGrade").get<std::string>();
        x.last_play_time = j.at("lastPlayTime").get<int64_t>();
        x.player_id = j.at("playerId").get<int64_t>();
        x.tokens_earned = j.at("tokensEarned").get<int64_t>();
    }

    inline void to_json(json& j, const Mastery& x) {
        j = json::object();
        j["championId"] = x.champion_id;
        j["championLevel"] = x.champion_level;
        j["championPoints"] = x.champion_points;
        j["championPointsSinceLastLevel"] = x.champion_points_since_last_level;
        j["championPointsUntilNextLevel"] = x.champion_points_until_next_level;
        j["chestGranted"] = x.chest_granted;
        j["formattedChampionPoints"] = x.formatted_champion_points;
        j["formattedMasteryGoal"] = x.formatted_mastery_goal;
        j["highestGrade"] = x.highest_grade;
        j["lastPlayTime"] = x.last_play_time;
        j["playerId"] = x.player_id;
        j["tokensEarned"] = x.tokens_earned;
    }

    inline void from_json(const json& j, LolCollections& x) {
        x.masteries = j.at("masteries").get<std::vector<Mastery>>();
        x.score = j.at("score").get<int64_t>();
        x.summoner_id = j.at("summonerId").get<int64_t>();
    }

    inline void to_json(json& j, const LolCollections& x) {
        j = json::object();
        j["masteries"] = x.masteries;
        j["score"] = x.score;
        j["summonerId"] = x.summoner_id;
    }

    inline void from_json(const json& j, Player& x) {
        x.account_id = j.at("accountId").get<int64_t>();
        x.current_account_id = j.at("currentAccountId").get<int64_t>();
        x.current_platform_id = j.at("currentPlatformId").get<std::string>();
        x.match_history_uri = j.at("matchHistoryUri").get<std::string>();
        x.platform_id = j.at("platformId").get<std::string>();
        x.profile_icon = j.at("profileIcon").get<int64_t>();
        x.puuid = j.at("puuid").get<std::string>();
        x.summoner_id = j.at("summonerId").get<int64_t>();
        x.summoner_name = j.at("summonerName").get<std::string>();
    }

    inline void to_json(json& j, const Player& x) {
        j = json::object();
        j["accountId"] = x.account_id;
        j["currentAccountId"] = x.current_account_id;
        j["currentPlatformId"] = x.current_platform_id;
        j["matchHistoryUri"] = x.match_history_uri;
        j["platformId"] = x.platform_id;
        j["profileIcon"] = x.profile_icon;
        j["puuid"] = x.puuid;
        j["summonerId"] = x.summoner_id;
        j["summonerName"] = x.summoner_name;
    }

    inline void from_json(const json& j, ParticipantIdentity& x) {
        x.participant_id = j.at("participantId").get<int64_t>();
        x.player = j.at("player").get<Player>();
    }

    inline void to_json(json& j, const ParticipantIdentity& x) {
        j = json::object();
        j["participantId"] = x.participant_id;
        j["player"] = x.player;
    }

    inline void from_json(const json& j, Stats& x) {
        x.assists = j.at("assists").get<int64_t>();
        x.caused_early_surrender = j.at("causedEarlySurrender").get<bool>();
        x.champ_level = j.at("champLevel").get<int64_t>();
        x.combat_player_score = j.at("combatPlayerScore").get<int64_t>();
        x.damage_dealt_to_objectives = j.at("damageDealtToObjectives").get<int64_t>();
        x.damage_dealt_to_turrets = j.at("damageDealtToTurrets").get<int64_t>();
        x.damage_self_mitigated = j.at("damageSelfMitigated").get<int64_t>();
        x.deaths = j.at("deaths").get<int64_t>();
        x.double_kills = j.at("doubleKills").get<int64_t>();
        x.early_surrender_accomplice = j.at("earlySurrenderAccomplice").get<bool>();
        x.first_blood_assist = j.at("firstBloodAssist").get<bool>();
        x.first_blood_kill = j.at("firstBloodKill").get<bool>();
        x.first_inhibitor_assist = j.at("firstInhibitorAssist").get<bool>();
        x.first_inhibitor_kill = j.at("firstInhibitorKill").get<bool>();
        x.first_tower_assist = j.at("firstTowerAssist").get<bool>();
        x.first_tower_kill = j.at("firstTowerKill").get<bool>();
        x.game_ended_in_early_surrender = j.at("gameEndedInEarlySurrender").get<bool>();
        x.game_ended_in_surrender = j.at("gameEndedInSurrender").get<bool>();
        x.gold_earned = j.at("goldEarned").get<int64_t>();
        x.gold_spent = j.at("goldSpent").get<int64_t>();
        x.inhibitor_kills = j.at("inhibitorKills").get<int64_t>();
        x.item0 = j.at("item0").get<int64_t>();
        x.item1 = j.at("item1").get<int64_t>();
        x.item2 = j.at("item2").get<int64_t>();
        x.item3 = j.at("item3").get<int64_t>();
        x.item4 = j.at("item4").get<int64_t>();
        x.item5 = j.at("item5").get<int64_t>();
        x.item6 = j.at("item6").get<int64_t>();
        x.killing_sprees = j.at("killingSprees").get<int64_t>();
        x.kills = j.at("kills").get<int64_t>();
        x.largest_critical_strike = j.at("largestCriticalStrike").get<int64_t>();
        x.largest_killing_spree = j.at("largestKillingSpree").get<int64_t>();
        x.largest_multi_kill = j.at("largestMultiKill").get<int64_t>();
        x.longest_time_spent_living = j.at("longestTimeSpentLiving").get<int64_t>();
        x.magic_damage_dealt = j.at("magicDamageDealt").get<int64_t>();
        x.magic_damage_dealt_to_champions = j.at("magicDamageDealtToChampions").get<int64_t>();
        x.magical_damage_taken = j.at("magicalDamageTaken").get<int64_t>();
        x.neutral_minions_killed = j.at("neutralMinionsKilled").get<int64_t>();
        x.neutral_minions_killed_enemy_jungle = j.at("neutralMinionsKilledEnemyJungle").get<int64_t>();
        x.neutral_minions_killed_team_jungle = j.at("neutralMinionsKilledTeamJungle").get<int64_t>();
        x.objective_player_score = j.at("objectivePlayerScore").get<int64_t>();
        x.participant_id = j.at("participantId").get<int64_t>();
        x.penta_kills = j.at("pentaKills").get<int64_t>();
        x.perk0 = j.at("perk0").get<int64_t>();
        x.perk0_var1 = j.at("perk0Var1").get<int64_t>();
        x.perk0_var2 = j.at("perk0Var2").get<int64_t>();
        x.perk0_var3 = j.at("perk0Var3").get<int64_t>();
        x.perk1 = j.at("perk1").get<int64_t>();
        x.perk1_var1 = j.at("perk1Var1").get<int64_t>();
        x.perk1_var2 = j.at("perk1Var2").get<int64_t>();
        x.perk1_var3 = j.at("perk1Var3").get<int64_t>();
        x.perk2 = j.at("perk2").get<int64_t>();
        x.perk2_var1 = j.at("perk2Var1").get<int64_t>();
        x.perk2_var2 = j.at("perk2Var2").get<int64_t>();
        x.perk2_var3 = j.at("perk2Var3").get<int64_t>();
        x.perk3 = j.at("perk3").get<int64_t>();
        x.perk3_var1 = j.at("perk3Var1").get<int64_t>();
        x.perk3_var2 = j.at("perk3Var2").get<int64_t>();
        x.perk3_var3 = j.at("perk3Var3").get<int64_t>();
        x.perk4 = j.at("perk4").get<int64_t>();
        x.perk4_var1 = j.at("perk4Var1").get<int64_t>();
        x.perk4_var2 = j.at("perk4Var2").get<int64_t>();
        x.perk4_var3 = j.at("perk4Var3").get<int64_t>();
        x.perk5 = j.at("perk5").get<int64_t>();
        x.perk5_var1 = j.at("perk5Var1").get<int64_t>();
        x.perk5_var2 = j.at("perk5Var2").get<int64_t>();
        x.perk5_var3 = j.at("perk5Var3").get<int64_t>();
        x.perk_primary_style = j.at("perkPrimaryStyle").get<int64_t>();
        x.perk_sub_style = j.at("perkSubStyle").get<int64_t>();
        x.physical_damage_dealt = j.at("physicalDamageDealt").get<int64_t>();
        x.physical_damage_dealt_to_champions = j.at("physicalDamageDealtToChampions").get<int64_t>();
        x.physical_damage_taken = j.at("physicalDamageTaken").get<int64_t>();
        x.player_augment1 = j.at("playerAugment1").get<int64_t>();
        x.player_augment2 = j.at("playerAugment2").get<int64_t>();
        x.player_augment3 = j.at("playerAugment3").get<int64_t>();
        x.player_augment4 = j.at("playerAugment4").get<int64_t>();
        x.player_score0 = j.at("playerScore0").get<int64_t>();
        x.player_score1 = j.at("playerScore1").get<int64_t>();
        x.player_score2 = j.at("playerScore2").get<int64_t>();
        x.player_score3 = j.at("playerScore3").get<int64_t>();
        x.player_score4 = j.at("playerScore4").get<int64_t>();
        x.player_score5 = j.at("playerScore5").get<int64_t>();
        x.player_score6 = j.at("playerScore6").get<int64_t>();
        x.player_score7 = j.at("playerScore7").get<int64_t>();
        x.player_score8 = j.at("playerScore8").get<int64_t>();
        x.player_score9 = j.at("playerScore9").get<int64_t>();
        x.player_subteam_id = j.at("playerSubteamId").get<int64_t>();
        x.quadra_kills = j.at("quadraKills").get<int64_t>();
        x.sight_wards_bought_in_game = j.at("sightWardsBoughtInGame").get<int64_t>();
        x.subteam_placement = j.at("subteamPlacement").get<int64_t>();
        x.team_early_surrendered = j.at("teamEarlySurrendered").get<bool>();
        x.time_c_cing_others = j.at("timeCCingOthers").get<int64_t>();
        x.total_damage_dealt = j.at("totalDamageDealt").get<int64_t>();
        x.total_damage_dealt_to_champions = j.at("totalDamageDealtToChampions").get<int64_t>();
        x.total_damage_taken = j.at("totalDamageTaken").get<int64_t>();
        x.total_heal = j.at("totalHeal").get<int64_t>();
        x.total_minions_killed = j.at("totalMinionsKilled").get<int64_t>();
        x.total_player_score = j.at("totalPlayerScore").get<int64_t>();
        x.total_score_rank = j.at("totalScoreRank").get<int64_t>();
        x.total_time_crowd_control_dealt = j.at("totalTimeCrowdControlDealt").get<int64_t>();
        x.total_units_healed = j.at("totalUnitsHealed").get<int64_t>();
        x.triple_kills = j.at("tripleKills").get<int64_t>();
        x.true_damage_dealt = j.at("trueDamageDealt").get<int64_t>();
        x.true_damage_dealt_to_champions = j.at("trueDamageDealtToChampions").get<int64_t>();
        x.true_damage_taken = j.at("trueDamageTaken").get<int64_t>();
        x.turret_kills = j.at("turretKills").get<int64_t>();
        x.unreal_kills = j.at("unrealKills").get<int64_t>();
        x.vision_score = j.at("visionScore").get<int64_t>();
        x.vision_wards_bought_in_game = j.at("visionWardsBoughtInGame").get<int64_t>();
        x.wards_killed = j.at("wardsKilled").get<int64_t>();
        x.wards_placed = j.at("wardsPlaced").get<int64_t>();
        x.win = j.at("win").get<bool>();
    }

    inline void to_json(json& j, const Stats& x) {
        j = json::object();
        j["assists"] = x.assists;
        j["causedEarlySurrender"] = x.caused_early_surrender;
        j["champLevel"] = x.champ_level;
        j["combatPlayerScore"] = x.combat_player_score;
        j["damageDealtToObjectives"] = x.damage_dealt_to_objectives;
        j["damageDealtToTurrets"] = x.damage_dealt_to_turrets;
        j["damageSelfMitigated"] = x.damage_self_mitigated;
        j["deaths"] = x.deaths;
        j["doubleKills"] = x.double_kills;
        j["earlySurrenderAccomplice"] = x.early_surrender_accomplice;
        j["firstBloodAssist"] = x.first_blood_assist;
        j["firstBloodKill"] = x.first_blood_kill;
        j["firstInhibitorAssist"] = x.first_inhibitor_assist;
        j["firstInhibitorKill"] = x.first_inhibitor_kill;
        j["firstTowerAssist"] = x.first_tower_assist;
        j["firstTowerKill"] = x.first_tower_kill;
        j["gameEndedInEarlySurrender"] = x.game_ended_in_early_surrender;
        j["gameEndedInSurrender"] = x.game_ended_in_surrender;
        j["goldEarned"] = x.gold_earned;
        j["goldSpent"] = x.gold_spent;
        j["inhibitorKills"] = x.inhibitor_kills;
        j["item0"] = x.item0;
        j["item1"] = x.item1;
        j["item2"] = x.item2;
        j["item3"] = x.item3;
        j["item4"] = x.item4;
        j["item5"] = x.item5;
        j["item6"] = x.item6;
        j["killingSprees"] = x.killing_sprees;
        j["kills"] = x.kills;
        j["largestCriticalStrike"] = x.largest_critical_strike;
        j["largestKillingSpree"] = x.largest_killing_spree;
        j["largestMultiKill"] = x.largest_multi_kill;
        j["longestTimeSpentLiving"] = x.longest_time_spent_living;
        j["magicDamageDealt"] = x.magic_damage_dealt;
        j["magicDamageDealtToChampions"] = x.magic_damage_dealt_to_champions;
        j["magicalDamageTaken"] = x.magical_damage_taken;
        j["neutralMinionsKilled"] = x.neutral_minions_killed;
        j["neutralMinionsKilledEnemyJungle"] = x.neutral_minions_killed_enemy_jungle;
        j["neutralMinionsKilledTeamJungle"] = x.neutral_minions_killed_team_jungle;
        j["objectivePlayerScore"] = x.objective_player_score;
        j["participantId"] = x.participant_id;
        j["pentaKills"] = x.penta_kills;
        j["perk0"] = x.perk0;
        j["perk0Var1"] = x.perk0_var1;
        j["perk0Var2"] = x.perk0_var2;
        j["perk0Var3"] = x.perk0_var3;
        j["perk1"] = x.perk1;
        j["perk1Var1"] = x.perk1_var1;
        j["perk1Var2"] = x.perk1_var2;
        j["perk1Var3"] = x.perk1_var3;
        j["perk2"] = x.perk2;
        j["perk2Var1"] = x.perk2_var1;
        j["perk2Var2"] = x.perk2_var2;
        j["perk2Var3"] = x.perk2_var3;
        j["perk3"] = x.perk3;
        j["perk3Var1"] = x.perk3_var1;
        j["perk3Var2"] = x.perk3_var2;
        j["perk3Var3"] = x.perk3_var3;
        j["perk4"] = x.perk4;
        j["perk4Var1"] = x.perk4_var1;
        j["perk4Var2"] = x.perk4_var2;
        j["perk4Var3"] = x.perk4_var3;
        j["perk5"] = x.perk5;
        j["perk5Var1"] = x.perk5_var1;
        j["perk5Var2"] = x.perk5_var2;
        j["perk5Var3"] = x.perk5_var3;
        j["perkPrimaryStyle"] = x.perk_primary_style;
        j["perkSubStyle"] = x.perk_sub_style;
        j["physicalDamageDealt"] = x.physical_damage_dealt;
        j["physicalDamageDealtToChampions"] = x.physical_damage_dealt_to_champions;
        j["physicalDamageTaken"] = x.physical_damage_taken;
        j["playerAugment1"] = x.player_augment1;
        j["playerAugment2"] = x.player_augment2;
        j["playerAugment3"] = x.player_augment3;
        j["playerAugment4"] = x.player_augment4;
        j["playerScore0"] = x.player_score0;
        j["playerScore1"] = x.player_score1;
        j["playerScore2"] = x.player_score2;
        j["playerScore3"] = x.player_score3;
        j["playerScore4"] = x.player_score4;
        j["playerScore5"] = x.player_score5;
        j["playerScore6"] = x.player_score6;
        j["playerScore7"] = x.player_score7;
        j["playerScore8"] = x.player_score8;
        j["playerScore9"] = x.player_score9;
        j["playerSubteamId"] = x.player_subteam_id;
        j["quadraKills"] = x.quadra_kills;
        j["sightWardsBoughtInGame"] = x.sight_wards_bought_in_game;
        j["subteamPlacement"] = x.subteam_placement;
        j["teamEarlySurrendered"] = x.team_early_surrendered;
        j["timeCCingOthers"] = x.time_c_cing_others;
        j["totalDamageDealt"] = x.total_damage_dealt;
        j["totalDamageDealtToChampions"] = x.total_damage_dealt_to_champions;
        j["totalDamageTaken"] = x.total_damage_taken;
        j["totalHeal"] = x.total_heal;
        j["totalMinionsKilled"] = x.total_minions_killed;
        j["totalPlayerScore"] = x.total_player_score;
        j["totalScoreRank"] = x.total_score_rank;
        j["totalTimeCrowdControlDealt"] = x.total_time_crowd_control_dealt;
        j["totalUnitsHealed"] = x.total_units_healed;
        j["tripleKills"] = x.triple_kills;
        j["trueDamageDealt"] = x.true_damage_dealt;
        j["trueDamageDealtToChampions"] = x.true_damage_dealt_to_champions;
        j["trueDamageTaken"] = x.true_damage_taken;
        j["turretKills"] = x.turret_kills;
        j["unrealKills"] = x.unreal_kills;
        j["visionScore"] = x.vision_score;
        j["visionWardsBoughtInGame"] = x.vision_wards_bought_in_game;
        j["wardsKilled"] = x.wards_killed;
        j["wardsPlaced"] = x.wards_placed;
        j["win"] = x.win;
    }

    inline void from_json(const json& j, Timeline& x) {
        x.creeps_per_min_deltas = j.at("creepsPerMinDeltas").get<SplitsProgress>();
        x.cs_diff_per_min_deltas = j.at("csDiffPerMinDeltas").get<SplitsProgress>();
        x.damage_taken_diff_per_min_deltas = j.at("damageTakenDiffPerMinDeltas").get<SplitsProgress>();
        x.damage_taken_per_min_deltas = j.at("damageTakenPerMinDeltas").get<SplitsProgress>();
        x.gold_per_min_deltas = j.at("goldPerMinDeltas").get<SplitsProgress>();
        x.lane = j.at("lane").get<std::string>();
        x.participant_id = j.at("participantId").get<int64_t>();
        x.role = j.at("role").get<std::string>();
        x.xp_diff_per_min_deltas = j.at("xpDiffPerMinDeltas").get<SplitsProgress>();
        x.xp_per_min_deltas = j.at("xpPerMinDeltas").get<SplitsProgress>();
    }

    inline void to_json(json& j, const Timeline& x) {
        j = json::object();
        j["creepsPerMinDeltas"] = x.creeps_per_min_deltas;
        j["csDiffPerMinDeltas"] = x.cs_diff_per_min_deltas;
        j["damageTakenDiffPerMinDeltas"] = x.damage_taken_diff_per_min_deltas;
        j["damageTakenPerMinDeltas"] = x.damage_taken_per_min_deltas;
        j["goldPerMinDeltas"] = x.gold_per_min_deltas;
        j["lane"] = x.lane;
        j["participantId"] = x.participant_id;
        j["role"] = x.role;
        j["xpDiffPerMinDeltas"] = x.xp_diff_per_min_deltas;
        j["xpPerMinDeltas"] = x.xp_per_min_deltas;
    }

    inline void from_json(const json& j, Participant& x) {
        x.champion_id = j.at("championId").get<int64_t>();
        x.highest_achieved_season_tier = j.at("highestAchievedSeasonTier").get<std::string>();
        x.participant_id = j.at("participantId").get<int64_t>();
        x.spell1_id = j.at("spell1Id").get<int64_t>();
        x.spell2_id = j.at("spell2Id").get<int64_t>();
        x.stats = j.at("stats").get<Stats>();
        x.team_id = j.at("teamId").get<int64_t>();
        x.timeline = j.at("timeline").get<Timeline>();
    }

    inline void to_json(json& j, const Participant& x) {
        j = json::object();
        j["championId"] = x.champion_id;
        j["highestAchievedSeasonTier"] = x.highest_achieved_season_tier;
        j["participantId"] = x.participant_id;
        j["spell1Id"] = x.spell1_id;
        j["spell2Id"] = x.spell2_id;
        j["stats"] = x.stats;
        j["teamId"] = x.team_id;
        j["timeline"] = x.timeline;
    }

    inline void from_json(const json& j, Team& x) {
        x.bans = j.at("bans").get<std::vector<nlohmann::json>>();
        x.baron_kills = j.at("baronKills").get<int64_t>();
        x.dominion_victory_score = j.at("dominionVictoryScore").get<int64_t>();
        x.dragon_kills = j.at("dragonKills").get<int64_t>();
        x.first_baron = j.at("firstBaron").get<bool>();
        x.first_blood = j.at("firstBlood").get<bool>();
        x.first_dargon = j.at("firstDargon").get<bool>();
        x.first_inhibitor = j.at("firstInhibitor").get<bool>();
        x.first_tower = j.at("firstTower").get<bool>();
        x.inhibitor_kills = j.at("inhibitorKills").get<int64_t>();
        x.rift_herald_kills = j.at("riftHeraldKills").get<int64_t>();
        x.team_id = j.at("teamId").get<int64_t>();
        x.tower_kills = j.at("towerKills").get<int64_t>();
        x.vilemaw_kills = j.at("vilemawKills").get<int64_t>();
        x.win = j.at("win").get<std::string>();
    }

    inline void to_json(json& j, const Team& x) {
        j = json::object();
        j["bans"] = x.bans;
        j["baronKills"] = x.baron_kills;
        j["dominionVictoryScore"] = x.dominion_victory_score;
        j["dragonKills"] = x.dragon_kills;
        j["firstBaron"] = x.first_baron;
        j["firstBlood"] = x.first_blood;
        j["firstDargon"] = x.first_dargon;
        j["firstInhibitor"] = x.first_inhibitor;
        j["firstTower"] = x.first_tower;
        j["inhibitorKills"] = x.inhibitor_kills;
        j["riftHeraldKills"] = x.rift_herald_kills;
        j["teamId"] = x.team_id;
        j["towerKills"] = x.tower_kills;
        j["vilemawKills"] = x.vilemaw_kills;
        j["win"] = x.win;
    }

    inline void from_json(const json& j, Games& x) {
        x.game_begin_date = j.at("gameBeginDate").get<std::string>();
        x.game_count = j.at("gameCount").get<int64_t>();
        x.game_end_date = j.at("gameEndDate").get<std::string>();
        x.game_index_begin = j.at("gameIndexBegin").get<int64_t>();
        x.game_index_end = j.at("gameIndexEnd").get<int64_t>();
        x.games = j.at("games").get<std::vector<LolMatchHistory>>();
    }

    inline void to_json(json& j, const Games& x) {
        j = json::object();
        j["gameBeginDate"] = x.game_begin_date;
        j["gameCount"] = x.game_count;
        j["gameEndDate"] = x.game_end_date;
        j["gameIndexBegin"] = x.game_index_begin;
        j["gameIndexEnd"] = x.game_index_end;
        j["games"] = x.games;
    }

    inline void from_json(const json& j, LolMatchHistory& x) {
        x.account_id = get_stack_optional<int64_t>(j, "accountId");
        x.games = get_heap_optional<Games>(j, "games");
        x.platform_id = j.at("platformId").get<std::string>();
        x.game_creation = get_stack_optional<int64_t>(j, "gameCreation");
        x.game_creation_date = get_stack_optional<std::string>(j, "gameCreationDate");
        x.game_duration = get_stack_optional<int64_t>(j, "gameDuration");
        x.game_id = get_stack_optional<int64_t>(j, "gameId");
        x.game_mode = get_stack_optional<std::string>(j, "gameMode");
        x.game_type = get_stack_optional<std::string>(j, "gameType");
        x.game_version = get_stack_optional<std::string>(j, "gameVersion");
        x.map_id = get_stack_optional<int64_t>(j, "mapId");
        x.participant_identities = get_stack_optional<std::vector<ParticipantIdentity>>(j, "participantIdentities");
        x.participants = get_stack_optional<std::vector<Participant>>(j, "participants");
        x.queue_id = get_stack_optional<int64_t>(j, "queueId");
        x.season_id = get_stack_optional<int64_t>(j, "seasonId");
        x.teams = get_stack_optional<std::vector<Team>>(j, "teams");
    }

    inline void to_json(json& j, const LolMatchHistory& x) {
        j = json::object();
        j["accountId"] = x.account_id;
        j["games"] = x.games;
        j["platformId"] = x.platform_id;
        j["gameCreation"] = x.game_creation;
        j["gameCreationDate"] = x.game_creation_date;
        j["gameDuration"] = x.game_duration;
        j["gameId"] = x.game_id;
        j["gameMode"] = x.game_mode;
        j["gameType"] = x.game_type;
        j["gameVersion"] = x.game_version;
        j["mapId"] = x.map_id;
        j["participantIdentities"] = x.participant_identities;
        j["participants"] = x.participants;
        j["queueId"] = x.queue_id;
        j["seasonId"] = x.season_id;
        j["teams"] = x.teams;
    }

    inline void from_json(const json& j, Action& x) {
        x.actor_cell_id = j.at("actorCellId").get<int64_t>();
        x.champion_id = j.at("championId").get<int64_t>();
        x.completed = j.at("completed").get<bool>();
        x.id = j.at("id").get<int64_t>();
        x.is_ally_action = j.at("isAllyAction").get<bool>();
        x.is_in_progress = j.at("isInProgress").get<bool>();
        x.pick_turn = j.at("pickTurn").get<int64_t>();
        x.type = j.at("type").get<std::string>();
    }

    inline void to_json(json& j, const Action& x) {
        j = json::object();
        j["actorCellId"] = x.actor_cell_id;
        j["championId"] = x.champion_id;
        j["completed"] = x.completed;
        j["id"] = x.id;
        j["isAllyAction"] = x.is_ally_action;
        j["isInProgress"] = x.is_in_progress;
        j["pickTurn"] = x.pick_turn;
        j["type"] = x.type;
    }

    inline void from_json(const json& j, Bans& x) {
        x.my_team_bans = j.at("myTeamBans").get<std::vector<nlohmann::json>>();
        x.num_bans = j.at("numBans").get<int64_t>();
        x.their_team_bans = j.at("theirTeamBans").get<std::vector<nlohmann::json>>();
    }

    inline void to_json(json& j, const Bans& x) {
        j = json::object();
        j["myTeamBans"] = x.my_team_bans;
        j["numBans"] = x.num_bans;
        j["theirTeamBans"] = x.their_team_bans;
    }

    inline void from_json(const json& j, ChatDetails& x) {
        x.muc_jwt_dto = j.at("mucJwtDto").get<MucJwtDto>();
        x.multi_user_chat_id = j.at("multiUserChatId").get<std::string>();
        x.multi_user_chat_password = j.at("multiUserChatPassword").get<std::string>();
    }

    inline void to_json(json& j, const ChatDetails& x) {
        j = json::object();
        j["mucJwtDto"] = x.muc_jwt_dto;
        j["multiUserChatId"] = x.multi_user_chat_id;
        j["multiUserChatPassword"] = x.multi_user_chat_password;
    }

    inline void from_json(const json& j, EntitledFeatureState& x) {
        x.additional_rerolls = j.at("additionalRerolls").get<int64_t>();
        x.unlocked_skin_ids = j.at("unlockedSkinIds").get<std::vector<nlohmann::json>>();
    }

    inline void to_json(json& j, const EntitledFeatureState& x) {
        j = json::object();
        j["additionalRerolls"] = x.additional_rerolls;
        j["unlockedSkinIds"] = x.unlocked_skin_ids;
    }

    inline void from_json(const json& j, MyTeam& x) {
        x.assigned_position = j.at("assignedPosition").get<std::string>();
        x.cell_id = j.at("cellId").get<int64_t>();
        x.champion_id = j.at("championId").get<int64_t>();
        x.champion_pick_intent = j.at("championPickIntent").get<int64_t>();
        x.entitled_feature_type = j.at("entitledFeatureType").get<std::string>();
        x.name_visibility_type = j.at("nameVisibilityType").get<std::string>();
        x.obfuscated_puuid = j.at("obfuscatedPuuid").get<std::string>();
        x.obfuscated_summoner_id = j.at("obfuscatedSummonerId").get<int64_t>();
        x.puuid = j.at("puuid").get<std::string>();
        x.selected_skin_id = j.at("selectedSkinId").get<int64_t>();
        x.spell1_id = j.at("spell1Id").get<int64_t>();
        x.spell2_id = j.at("spell2Id").get<int64_t>();
        x.summoner_id = j.at("summonerId").get<int64_t>();
        x.team = j.at("team").get<int64_t>();
        x.ward_skin_id = j.at("wardSkinId").get<int64_t>();
    }

    inline void to_json(json& j, const MyTeam& x) {
        j = json::object();
        j["assignedPosition"] = x.assigned_position;
        j["cellId"] = x.cell_id;
        j["championId"] = x.champion_id;
        j["championPickIntent"] = x.champion_pick_intent;
        j["entitledFeatureType"] = x.entitled_feature_type;
        j["nameVisibilityType"] = x.name_visibility_type;
        j["obfuscatedPuuid"] = x.obfuscated_puuid;
        j["obfuscatedSummonerId"] = x.obfuscated_summoner_id;
        j["puuid"] = x.puuid;
        j["selectedSkinId"] = x.selected_skin_id;
        j["spell1Id"] = x.spell1_id;
        j["spell2Id"] = x.spell2_id;
        j["summonerId"] = x.summoner_id;
        j["team"] = x.team;
        j["wardSkinId"] = x.ward_skin_id;
    }

    inline void from_json(const json& j, Timer& x) {
        x.adjusted_time_left_in_phase = j.at("adjustedTimeLeftInPhase").get<int64_t>();
        x.internal_now_in_epoch_ms = j.at("internalNowInEpochMs").get<int64_t>();
        x.is_infinite = j.at("isInfinite").get<bool>();
        x.phase = j.at("phase").get<std::string>();
        x.total_time_in_phase = j.at("totalTimeInPhase").get<int64_t>();
    }

    inline void to_json(json& j, const Timer& x) {
        j = json::object();
        j["adjustedTimeLeftInPhase"] = x.adjusted_time_left_in_phase;
        j["internalNowInEpochMs"] = x.internal_now_in_epoch_ms;
        j["isInfinite"] = x.is_infinite;
        j["phase"] = x.phase;
        j["totalTimeInPhase"] = x.total_time_in_phase;
    }

    inline void from_json(const json& j, LolChampSelect& x) {
        x.actions = j.at("actions").get<std::vector<std::vector<Action>>>();
        x.allow_battle_boost = j.at("allowBattleBoost").get<bool>();
        x.allow_duplicate_picks = j.at("allowDuplicatePicks").get<bool>();
        x.allow_locked_events = j.at("allowLockedEvents").get<bool>();
        x.allow_rerolling = j.at("allowRerolling").get<bool>();
        x.allow_skin_selection = j.at("allowSkinSelection").get<bool>();
        x.bans = j.at("bans").get<Bans>();
        x.bench_champions = j.at("benchChampions").get<std::vector<nlohmann::json>>();
        x.bench_enabled = j.at("benchEnabled").get<bool>();
        x.boostable_skin_count = j.at("boostableSkinCount").get<int64_t>();
        x.chat_details = j.at("chatDetails").get<ChatDetails>();
        x.counter = j.at("counter").get<int64_t>();
        x.entitled_feature_state = j.at("entitledFeatureState").get<EntitledFeatureState>();
        x.game_id = j.at("gameId").get<int64_t>();
        x.has_simultaneous_bans = j.at("hasSimultaneousBans").get<bool>();
        x.has_simultaneous_picks = j.at("hasSimultaneousPicks").get<bool>();
        x.is_custom_game = j.at("isCustomGame").get<bool>();
        x.is_spectating = j.at("isSpectating").get<bool>();
        x.local_player_cell_id = j.at("localPlayerCellId").get<int64_t>();
        x.locked_event_index = j.at("lockedEventIndex").get<int64_t>();
        x.my_team = j.at("myTeam").get<std::vector<MyTeam>>();
        x.pick_order_swaps = j.at("pickOrderSwaps").get<std::vector<nlohmann::json>>();
        x.recovery_counter = j.at("recoveryCounter").get<int64_t>();
        x.rerolls_remaining = j.at("rerollsRemaining").get<int64_t>();
        x.skip_champion_select = j.at("skipChampionSelect").get<bool>();
        x.their_team = j.at("theirTeam").get<std::vector<nlohmann::json>>();
        x.timer = j.at("timer").get<Timer>();
        x.trades = j.at("trades").get<std::vector<nlohmann::json>>();
    }

    inline void to_json(json& j, const LolChampSelect& x) {
        j = json::object();
        j["actions"] = x.actions;
        j["allowBattleBoost"] = x.allow_battle_boost;
        j["allowDuplicatePicks"] = x.allow_duplicate_picks;
        j["allowLockedEvents"] = x.allow_locked_events;
        j["allowRerolling"] = x.allow_rerolling;
        j["allowSkinSelection"] = x.allow_skin_selection;
        j["bans"] = x.bans;
        j["benchChampions"] = x.bench_champions;
        j["benchEnabled"] = x.bench_enabled;
        j["boostableSkinCount"] = x.boostable_skin_count;
        j["chatDetails"] = x.chat_details;
        j["counter"] = x.counter;
        j["entitledFeatureState"] = x.entitled_feature_state;
        j["gameId"] = x.game_id;
        j["hasSimultaneousBans"] = x.has_simultaneous_bans;
        j["hasSimultaneousPicks"] = x.has_simultaneous_picks;
        j["isCustomGame"] = x.is_custom_game;
        j["isSpectating"] = x.is_spectating;
        j["localPlayerCellId"] = x.local_player_cell_id;
        j["lockedEventIndex"] = x.locked_event_index;
        j["myTeam"] = x.my_team;
        j["pickOrderSwaps"] = x.pick_order_swaps;
        j["recoveryCounter"] = x.recovery_counter;
        j["rerollsRemaining"] = x.rerolls_remaining;
        j["skipChampionSelect"] = x.skip_champion_select;
        j["theirTeam"] = x.their_team;
        j["timer"] = x.timer;
        j["trades"] = x.trades;
    }

    inline void from_json(const json& j, HighestRankedEntry& x) {
        x.division = j.at("division").get<std::string>();
        x.highest_division = j.at("highestDivision").get<std::string>();
        x.highest_tier = j.at("highestTier").get<std::string>();
        x.is_provisional = j.at("isProvisional").get<bool>();
        x.league_points = j.at("leaguePoints").get<int64_t>();
        x.losses = j.at("losses").get<int64_t>();
        x.mini_series_progress = j.at("miniSeriesProgress").get<std::string>();
        x.previous_season_achieved_division = j.at("previousSeasonAchievedDivision").get<std::string>();
        x.previous_season_achieved_tier = j.at("previousSeasonAchievedTier").get<std::string>();
        x.previous_season_end_division = j.at("previousSeasonEndDivision").get<std::string>();
        x.previous_season_end_tier = j.at("previousSeasonEndTier").get<std::string>();
        x.provisional_game_threshold = j.at("provisionalGameThreshold").get<int64_t>();
        x.provisional_games_remaining = j.at("provisionalGamesRemaining").get<int64_t>();
        x.queue_type = j.at("queueType").get<std::string>();
        x.rated_rating = j.at("ratedRating").get<int64_t>();
        x.rated_tier = j.at("ratedTier").get<std::string>();
        x.tier = j.at("tier").get<std::string>();
        x.warnings = get_untyped(j, "warnings");
        x.wins = j.at("wins").get<int64_t>();
    }

    inline void to_json(json& j, const HighestRankedEntry& x) {
        j = json::object();
        j["division"] = x.division;
        j["highestDivision"] = x.highest_division;
        j["highestTier"] = x.highest_tier;
        j["isProvisional"] = x.is_provisional;
        j["leaguePoints"] = x.league_points;
        j["losses"] = x.losses;
        j["miniSeriesProgress"] = x.mini_series_progress;
        j["previousSeasonAchievedDivision"] = x.previous_season_achieved_division;
        j["previousSeasonAchievedTier"] = x.previous_season_achieved_tier;
        j["previousSeasonEndDivision"] = x.previous_season_end_division;
        j["previousSeasonEndTier"] = x.previous_season_end_tier;
        j["provisionalGameThreshold"] = x.provisional_game_threshold;
        j["provisionalGamesRemaining"] = x.provisional_games_remaining;
        j["queueType"] = x.queue_type;
        j["ratedRating"] = x.rated_rating;
        j["ratedTier"] = x.rated_tier;
        j["tier"] = x.tier;
        j["warnings"] = x.warnings;
        j["wins"] = x.wins;
    }

    inline void from_json(const json& j, QueueMap& x) {
        x.cherry = j.at("CHERRY").get<HighestRankedEntry>();
        x.ranked_flex_sr = j.at("RANKED_FLEX_SR").get<HighestRankedEntry>();
        x.ranked_solo_5_x5 = j.at("RANKED_SOLO_5x5").get<HighestRankedEntry>();
        x.ranked_tft = j.at("RANKED_TFT").get<HighestRankedEntry>();
        x.ranked_tft_double_up = j.at("RANKED_TFT_DOUBLE_UP").get<HighestRankedEntry>();
        x.ranked_tft_turbo = j.at("RANKED_TFT_TURBO").get<HighestRankedEntry>();
    }

    inline void to_json(json& j, const QueueMap& x) {
        j = json::object();
        j["CHERRY"] = x.cherry;
        j["RANKED_FLEX_SR"] = x.ranked_flex_sr;
        j["RANKED_SOLO_5x5"] = x.ranked_solo_5_x5;
        j["RANKED_TFT"] = x.ranked_tft;
        j["RANKED_TFT_DOUBLE_UP"] = x.ranked_tft_double_up;
        j["RANKED_TFT_TURBO"] = x.ranked_tft_turbo;
    }

    inline void from_json(const json& j, Cherry& x) {
        x.current_season_end = j.at("currentSeasonEnd").get<int64_t>();
        x.current_season_id = j.at("currentSeasonId").get<int64_t>();
        x.next_season_start = j.at("nextSeasonStart").get<int64_t>();
    }

    inline void to_json(json& j, const Cherry& x) {
        j = json::object();
        j["currentSeasonEnd"] = x.current_season_end;
        j["currentSeasonId"] = x.current_season_id;
        j["nextSeasonStart"] = x.next_season_start;
    }

    inline void from_json(const json& j, Seasons& x) {
        x.cherry = j.at("CHERRY").get<Cherry>();
        x.ranked_flex_sr = j.at("RANKED_FLEX_SR").get<Cherry>();
        x.ranked_solo_5_x5 = j.at("RANKED_SOLO_5x5").get<Cherry>();
        x.ranked_tft = j.at("RANKED_TFT").get<Cherry>();
        x.ranked_tft_double_up = j.at("RANKED_TFT_DOUBLE_UP").get<Cherry>();
        x.ranked_tft_turbo = j.at("RANKED_TFT_TURBO").get<Cherry>();
    }

    inline void to_json(json& j, const Seasons& x) {
        j = json::object();
        j["CHERRY"] = x.cherry;
        j["RANKED_FLEX_SR"] = x.ranked_flex_sr;
        j["RANKED_SOLO_5x5"] = x.ranked_solo_5_x5;
        j["RANKED_TFT"] = x.ranked_tft;
        j["RANKED_TFT_DOUBLE_UP"] = x.ranked_tft_double_up;
        j["RANKED_TFT_TURBO"] = x.ranked_tft_turbo;
    }

    inline void from_json(const json& j, LolRankedData& x) {
        x.earned_regalia_reward_ids = j.at("earnedRegaliaRewardIds").get<std::vector<nlohmann::json>>();
        x.highest_current_season_reached_tier_sr = j.at("highestCurrentSeasonReachedTierSR").get<std::string>();
        x.highest_previous_season_achieved_division = j.at("highestPreviousSeasonAchievedDivision").get<std::string>();
        x.highest_previous_season_achieved_tier = j.at("highestPreviousSeasonAchievedTier").get<std::string>();
        x.highest_previous_season_end_division = j.at("highestPreviousSeasonEndDivision").get<std::string>();
        x.highest_previous_season_end_tier = j.at("highestPreviousSeasonEndTier").get<std::string>();
        x.highest_ranked_entry = j.at("highestRankedEntry").get<HighestRankedEntry>();
        x.highest_ranked_entry_sr = j.at("highestRankedEntrySR").get<HighestRankedEntry>();
        x.queue_map = j.at("queueMap").get<QueueMap>();
        x.queues = j.at("queues").get<std::vector<HighestRankedEntry>>();
        x.ranked_regalia_level = j.at("rankedRegaliaLevel").get<int64_t>();
        x.seasons = j.at("seasons").get<Seasons>();
        x.splits_progress = j.at("splitsProgress").get<SplitsProgress>();
    }

    inline void to_json(json& j, const LolRankedData& x) {
        j = json::object();
        j["earnedRegaliaRewardIds"] = x.earned_regalia_reward_ids;
        j["highestCurrentSeasonReachedTierSR"] = x.highest_current_season_reached_tier_sr;
        j["highestPreviousSeasonAchievedDivision"] = x.highest_previous_season_achieved_division;
        j["highestPreviousSeasonAchievedTier"] = x.highest_previous_season_achieved_tier;
        j["highestPreviousSeasonEndDivision"] = x.highest_previous_season_end_division;
        j["highestPreviousSeasonEndTier"] = x.highest_previous_season_end_tier;
        j["highestRankedEntry"] = x.highest_ranked_entry;
        j["highestRankedEntrySR"] = x.highest_ranked_entry_sr;
        j["queueMap"] = x.queue_map;
        j["queues"] = x.queues;
        j["rankedRegaliaLevel"] = x.ranked_regalia_level;
        j["seasons"] = x.seasons;
        j["splitsProgress"] = x.splits_progress;
    }

    inline void from_json(const json& j, RerollPoints& x) {
        x.current_points = j.at("currentPoints").get<int64_t>();
        x.max_rolls = j.at("maxRolls").get<int64_t>();
        x.number_of_rolls = j.at("numberOfRolls").get<int64_t>();
        x.points_cost_to_roll = j.at("pointsCostToRoll").get<int64_t>();
        x.points_to_reroll = j.at("pointsToReroll").get<int64_t>();
    }

    inline void to_json(json& j, const RerollPoints& x) {
        j = json::object();
        j["currentPoints"] = x.current_points;
        j["maxRolls"] = x.max_rolls;
        j["numberOfRolls"] = x.number_of_rolls;
        j["pointsCostToRoll"] = x.points_cost_to_roll;
        j["pointsToReroll"] = x.points_to_reroll;
    }

    inline void from_json(const json& j, LolSummonerInfo& x) {
        x.account_id = j.at("accountId").get<int64_t>();
        x.display_name = j.at("displayName").get<std::string>();
        x.internal_name = j.at("internalName").get<std::string>();
        x.name_change_flag = j.at("nameChangeFlag").get<bool>();
        x.percent_complete_for_next_level = j.at("percentCompleteForNextLevel").get<int64_t>();
        x.privacy = j.at("privacy").get<std::string>();
        x.profile_icon_id = j.at("profileIconId").get<int64_t>();
        x.puuid = j.at("puuid").get<std::string>();
        x.reroll_points = j.at("rerollPoints").get<RerollPoints>();
        x.summoner_id = j.at("summonerId").get<int64_t>();
        x.summoner_level = j.at("summonerLevel").get<int64_t>();
        x.unnamed = j.at("unnamed").get<bool>();
        x.xp_since_last_level = j.at("xpSinceLastLevel").get<int64_t>();
        x.xp_until_next_level = j.at("xpUntilNextLevel").get<int64_t>();
    }

    inline void to_json(json& j, const LolSummonerInfo& x) {
        j = json::object();
        j["accountId"] = x.account_id;
        j["displayName"] = x.display_name;
        j["internalName"] = x.internal_name;
        j["nameChangeFlag"] = x.name_change_flag;
        j["percentCompleteForNextLevel"] = x.percent_complete_for_next_level;
        j["privacy"] = x.privacy;
        j["profileIconId"] = x.profile_icon_id;
        j["puuid"] = x.puuid;
        j["rerollPoints"] = x.reroll_points;
        j["summonerId"] = x.summoner_id;
        j["summonerLevel"] = x.summoner_level;
        j["unnamed"] = x.unnamed;
        j["xpSinceLastLevel"] = x.xp_since_last_level;
        j["xpUntilNextLevel"] = x.xp_until_next_level;
    }
}
