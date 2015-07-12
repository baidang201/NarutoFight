
//  http://www.9miao.com

#ifndef _NF_JSON_TRANSLATER_HEAD_
#define _NF_JSON_TRANSLATER_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*					Json校验                                                            */
/************************************************************************/
//校验Json：float型
float GetFloatFromJson(const rapidjson::Value& root, const char* key ,int nDef1=0,int nDef2=0);
//校验Json：int型
int GetIntFromJson(const rapidjson::Value& root, const char* key ,int nDef1=0,int nDef2=0);
//校验Json：string型
const char * GetStringFromJson(const rapidjson::Value& root, const char* key ,int nDef1=0,int nDef2=0);

/************************************************************************/
/*						键值宏定义                                       */
/************************************************************************/
//技能块信息
#define			_SKILL_ARRAY_				"skill_array"					//技能数组
#define			_SKILL_DELAY_TIME_			"skill_delaytime"				//技能：延迟时间
#define			_SKILL_DIS_X_				"skill_dis_x"					//技能：x轴位移
#define			_SKILL_TIME_X_				"skill_time_x"					//技能：x轴时间
#define			_SKILL_DIS_Y_				"skill_dis_y"					//技能：y轴位移
#define			_SKILL_TIME_Y_				"skill_time_y"					//技能：y轴时间
#define			_SKILL_SPEED_Z_				"skill_speed_z"					//技能：z轴速度

#define			_SKILL_IS_SCREEN_SHAKE_		"skill_is_screen_shake"			//技能：屏幕是否震动：1.震动	0.不震动
#define			_SKILL_SHAKE_DUR_TIME_		"skill_shake_durtime"			//技能：屏幕震动持续时间
#define			_SKILL_SHAKE_DEGREE_		"skill_shake_degree"			//技能：屏幕震动幅度
#define			_SKILL_SHAKE_COUNT_			"skill_shake_count"				//技能：屏幕震动次数
#define			_SKILL_IS_LENSFEATURE_		"skill_is_lensfeature"			//技能：屏幕是否拉伸：1.拉伸	0.不拉伸
#define			_SKILL_LENSFRATURE_TIME_	"skill_lensfeature_time"		//技能：屏幕拉伸时间		
#define			_SKILL_IS_SCREEN_CHANGE_COLOR_		"skill_is_screen_change_color"		//屏幕是否改变颜色		0.不改变	1.变黑
#define			_SKILL_SCREEN_CHANGE_COLOR_TIME_	"skill_screen_change_color_time"	//屏幕颜色改变时间
#define			_SKILL_IS_TURN_ROUND_		"skill_is_turn_round"			//技能：释放者是否转头	1.是	0.不是

#define			_SKILL_BUFF_ARRAY_			"skill_buff"					//技能：释放者，buff数组
#define			_SKILL_TARGET_BUFF_ARRAY_	"skill_target_buff"				//技能：被攻击者，buff数组

#define			_SKILL_BUFF_ID_				"skill_buff_id"					//buff ID
#define			_SKILL_BUFF_TIME_			"skill_buff_time"				//buff持续时间
#define			_SKILL_BUFF_TIME_G_			"skill_buff_time_g"				//buff持续时间成长
#define			_SKILL_BUFF_POINT_1_		"skill_buff_point_1"			//buff点数1
#define			_SKILL_BUFF_POINT_1_G_		"skill_buff_point_1_g"			//buff点数1成长
#define			_SKILL_BUFF_POINT_2_		"skill_buff_point_2"			//buff点数2
#define			_SKILL_BUFF_POINT_2_G_		"skill_buff_point_2_g"			//buff点数2成长

#define			_SKILL_CHANGE_TIME_			"skill_change_time"				//技能：变化时间
#define			_SKILL_IS_GRAVITY_			"skill_is_gravity"				//技能：是否有重力
#define			_SKILL_OLD_X_				"skill_old_x"					//技能：技能旧的x轴位置
#define			_SKILL_OLD_Y_				"skill_old_y"					//技能：旧的y轴位置
#define			_SKILL_OLD_Z_				"skill_old_z"					//技能：旧的z轴位置
#define			_SKILL_OLD_LENGTH_			"skill_old_length"				//技能：旧的长度
#define			_SKILL_OLD_WIDTH_			"skill_old_width"				//技能：旧的宽度
#define			_SKILL_OLD_HEIGHT_			"skill_old_height"				//技能：旧的高度
#define			_SKILL_NEW_X_				"skill_new_x"					//技能：新的x轴位置
#define			_SKILL_NEW_Y_				"skill_new_y"					//技能：新的y轴位置
#define			_SKILL_NEW_Z_				"skill_new_z"					//技能：新的z轴位置
#define			_SKILL_NEW_LENGTH_			"skill_new_length"				//技能：新的长度
#define			_SKILL_NEW_WIDTH_			"skill_new_width"				//技能：新的宽度
#define			_SKILL_NEW_HEIGHT_			"skill_new_height"				//技能：新的高度

#define			_SKILL_DAMAGE_POINT_		"skill_damage_point"			//技能：伤害
#define			_SKILL_DAMAGE_POINT_G_		"skill_damage_point_g"			//技能：伤害成长
#define			_SKILL_DAMAGE_PERCENT_		"skill_damage_percent"			//技能：伤害百分比
#define			_SKILL_DAMAGE_PERCENT_G_	"skill_damage_percent_g"		//技能：伤害百分比成长
#define			_SKILL_TARGET_DIS_X_		"skill_target_dis_x"			//技能：被攻击者，x轴位移
#define			_SKILL_TARGET_TIME_X_		"skill_target_time_x"			//技能：被攻击者，x轴时间
#define			_SKILL_TARGET_DIS_Y_		"skill_target_dis_y"			//技能：被攻击者，y轴位移
#define			_SKILL_TARGET_TIME_Y_		"skill_target_time_y"			//技能：被攻击者，y轴时间
#define			_SKILL_TARGET_SPEED_Z_		"skill_target_speed_z"			//技能：被攻击者，z轴速度
#define			_SKILL_IS_CONVERSELY_		"skill_is_conversely"			//技能：是否打倒		1.是	0.不是
#define			_SKILL_CRICK_ADD_			"skill_crick_add"				//技能：硬直增加量
#define			_SKILL_ATTACK_SPEED_		"skill_attack_speed"			//技能：攻击之间的间隔（帧）
#define			_SKILL_IS_EXPLODE_MODE_		"skill_is_explode_mode"			//技能：是否为爆炸模式	1.是	0.不是
#define			_SKILL_IS_PULL_MODE_		"skill_is_pull_mode"			//技能：是否为拉近到	1.是	0.不是
#define			_SKILL_CRIT_ADD_			"skill_crit_add"				//技能：暴击率增加量
#define			_SKILL_CRIT_DAMAGE_ADD_		"skill_crit_damage_add"			//技能：暴击伤害增加量
#define			_SKILL_HIT_TARGET_ADD_		"skill_hit_target_add"			//技能：命中增加量
#define			_SKILL_IS_DRIECTION			"skill_is_direction"			//技能：是否有方向		1.有	0.没有

#define			_SKILL_IS_ARMATURE_			"skill_is_armature"				//技能：有否有骨骼动画	1.有	0.没有
#define			_SKILL_ARMATURE_NAME_		"skill_armature_name"			//技能：骨骼动画名称
#define			_SKILL_IS_SOUND_			"skill_is_sound"				//技能：有否有音效		1.有	0.没有
#define			_SKILL_SOUND_NAME_			"skill_sound_name"				//技能：音效名称

#define			_SKILL_IS_SUCTION_MODE_		"skill_is_suction_mode"			//技能：是否为吸引模式	1.是	0.不是
#define			_SKILL_SUCTION_SPEED_		"skill_suction_speed"			//技能：吸引模式速度
#define			_SKILL_SUCTION_TIME_		"skill_suction_time"			//技能：吸引模式时间（秒）
#define			_SKILL_SUCTION_POS_X_		"skill_suction_pos_x"			//技能：吸引到的x轴坐标
#define			_SKILL_SUCTION_POS_Y_		"skill_suction_pos_y"			//技能：吸引到的y轴坐标

#define			_SKILL_IS_ONLY_DAMAGE_NOT_HIT_			"skill_is_only_damage_not_hit"			//技能：是否只有伤害，不显示被击动画	1.是	0.不是
#define			_SKILL_IS_SUCTION_SKILL_ITEM_CENTER_	"skill_is_suction_skill_item_center"	//技能：是否吸引到技能块中心			1.是	0.不是
#define			_SKILL_IS_DAMAGE_						"skill_is_damage"						//技能：是否技能伤害					1.是	0.不是
#define			_SKILL_IS_FOLLOW_			"skill_is_follow"				//技能：技能快是否跟随释放者	1.是	0.不是
#define			_SKILL_IS_BREAK_SIPERARMOR_	"skill_is_break_siperarmor"		//技能：是否破霸体
#define			_SKILL_HIT_EFFECT_TYPE_		"skill_hit_effect_type"			//技能：拳花类型


//tmx信息
#define			_TMX_ARRAY_					"tmx_array"						//tmx数组
#define			_TMX_NAME_					"tmx_name"						//tmx：名称
#define			_TMX_START_POS_X_			"tmx_start_pos_x"				//tmx：初始坐标：x轴
#define			_TMX_START_POS_Y_			"tmx_start_pos_y"				//tmx：初始坐标：y轴

//NPC信息
#define			_NPC_ARR_					"npc_arr"						//NPC数组
#define			_NPC_ID_					"npc_id"						//NPC ID
#define			_NPC_X_						"npc_x"							//NPC x轴位置
#define			_NPC_Y_						"npc_y"							//NPC y轴位置
#define			_NPC_Z_						"npc_z"							//NPC z轴位置

//DOOR信息
#define			_DOOR_ARR_					"door_arr"						//Door数组
#define			_DOOR_ID_					"door_id"						//Door ID
#define			_DOOR_X_					"door_x"						//Door x轴位置
#define			_DOOR_Y_					"door_y"						//Door y轴位置
#define			_DOOR_Z_					"door_z"						//Door z轴位置

//主角ai信息
#define			_PRO_AI_TREAT_SKILL_		"pro_ai_treat_skill"			//治疗技能数组
#define			_PRO_AI_MUST_SKILL_			"pro_ai_must_skill"				//必然技能数组
#define			_PRO_AI_DISTANCE_SKILL_		"pro_ai_distance_skill"			//距离技能数组
#define			_PRO_AI_DISTANCE_ITEM_		"pro_ai_distance_item"			//距离技能块数组

#define			_PRO_AI_SKILL_ID_			"pro_ai_skill_id"				//技能ID
#define			_PRO_AI_BLOOD_PER_			"pro_ai_blood_per"				//治疗技能：出发加血的最小血量百分比
#define			_PRO_AI_SKILL_DISTANCE_		"pro_ai_skill_distance"			//距离技能：攻击的最大范围
#define			_PRO_AI_DELAY_TIME_			"pro_ai_delay_time"				//技能延迟时间

/************************************************************************/
/*					Json解析                                                                     */
/************************************************************************/
class CNFJsonTranslater
{
public:
	static CNFJsonTranslater *SharedData();		

	//解析Json，得到技能块信息
	static bool GetSkillItemInfoFromJson(const char * szJson,CNFSkillItemVec &Info,int nRoleID,int nSkillID);						

	//解析Json，得到tmx信息
	static bool GetTmxInfoFromJson(const char * szJson,CNFTmxStaticInfoMap &Info,int nStageID);						

	//解析Json，得到NPC信息
	static bool GetNPCInfoFromJson(const char * szJson,CNFNPCJsonInfoVec &Info,int nStageID);						

	//解析Json，得到Door信息
	static bool GetDoorInfoFromJson(const char * szJson,CNFDoorJsonInfoVec &Info,int nStageID);						

	//解析Json，得到主角ai信息
	static bool GetProtagonistAIInfoFromJson(const char * szJson,tagProtagonistAIInfo &Info,int nRoleID);						


//public:
//	//通过技能块信息，反解析成Json
//	static bool GetJsonFromSkillItemInfo(const char * szJson,tagSkillItemStaticInfo Info);	

};



#endif


















