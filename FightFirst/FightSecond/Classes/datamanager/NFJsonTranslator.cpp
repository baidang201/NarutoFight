#include "NFJsonTranslator.h"

//输出头文件
#include "CocoStudio/Json/rapidjson/stringbuffer.h"
#include "CocoStudio/Json/rapidjson/writer.h"

//Json错误判定标识
#define			_JSON_ERROR_ID_			-100000000
/************************************************************************/
/*					Json检验                                                                     */
/************************************************************************/
//校验Json：float型
float GetFloatFromJson(const rapidjson::Value& root, const char* key,int nDef1,int nDef2)
{	
	//判断Json：错误，或者无此Json
	if ( _JSON_ERROR_ID_ == DICTOOL->getFloatValue_json(root, key,_JSON_ERROR_ID_) )
	{
		CCLog("Float Json Error! [%s] [%d,%d]",key,nDef1,nDef2);
		return 0.f;
	}
	return DICTOOL->getFloatValue_json(root, key );
}
//校验Json：int型
int GetIntFromJson(const rapidjson::Value& root, const char* key,int nDef1,int nDef2)
{	
	//判断Json：错误，或者无此Json
	if ( _JSON_ERROR_ID_ == DICTOOL->getIntValue_json(root, key,_JSON_ERROR_ID_) )
	{
		CCLog("Int Json Error! [%s] [%d,%d]",key,nDef1,nDef2);
		return 0;
	}
	return DICTOOL->getIntValue_json(root, key );
}
//校验Json：string型
const char * GetStringFromJson(const rapidjson::Value& root, const char* key,int nDef1,int nDef2)
{	
	//判断Json：错误，或者无此Json
	char szJsonErrorID[NAME_LEN] = {0};
	sprintf(szJsonErrorID,"%d",_JSON_ERROR_ID_);
	if ( strcmp(szJsonErrorID,DICTOOL->getStringValue_json(root, key,szJsonErrorID))==0  )
	{
		CCLog("String Json Error! [%s] [%d,%d]",key,nDef1,nDef2);
		return "";
	}
	return DICTOOL->getStringValue_json(root, key );
}

/************************************************************************/
/*					初始化Json                                                                     */
/************************************************************************/
static CNFJsonTranslater * g_pJsonTranslator=NULL;
CNFJsonTranslater * CNFJsonTranslater::SharedData()
{
	if(g_pJsonTranslator==NULL)
	{
		g_pJsonTranslator = new CNFJsonTranslater();
	}
	return g_pJsonTranslator;
}


/************************************************************************/
/*						解析Json                                                                     */
/************************************************************************/	
//得到技能块信息
bool CNFJsonTranslater::GetSkillItemInfoFromJson(const char * szJson,CNFSkillItemVec &Info,int nRoleID,int nSkillID)
{
	do 
	{
		//创建Json
		rapidjson::Document json;

		//复制字符串数据
		json.Parse<0>(szJson);
		
		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun CNFJsonTranslater::GetSkillItemInfoFromJson GetParseError %s\n",json.GetParseError());
		}

		/************************************************************************/
		/*				Json解析                                                                     */
		/************************************************************************/
		//技能数组大小
		int nArrayCount = DICTOOL->getArrayCount_json(json,_SKILL_ARRAY_);
		for (int i = 0; i < nArrayCount; ++i)
		{
			//技能数组
			const rapidjson::Value &ArrayJson = DICTOOL->getSubDictionary_json(json,_SKILL_ARRAY_, i);
			
			tagSkillItemStaticInfo ItemInfo;

			/************************************************************************/
			/*				释放者                                                                     */
			/************************************************************************/			
			//延迟时间
			ItemInfo.fDelayTime = GetFloatFromJson(ArrayJson, _SKILL_DELAY_TIME_,nRoleID,nSkillID);
			
			//释放者，x轴位移
			ItemInfo.fDisX = GetFloatFromJson(ArrayJson, _SKILL_DIS_X_,nRoleID,nSkillID);

			//释放者，x轴时间
			ItemInfo.fTimeX = GetFloatFromJson(ArrayJson, _SKILL_TIME_X_,nRoleID,nSkillID);

			//释放者，y轴位移
			ItemInfo.fDisY = GetFloatFromJson(ArrayJson, _SKILL_DIS_Y_,nRoleID,nSkillID);

			//释放者，y轴时间
			ItemInfo.fTimeY = GetFloatFromJson(ArrayJson, _SKILL_TIME_Y_,nRoleID,nSkillID);

			//释放者，z轴速度
			ItemInfo.fSpeedZ = GetFloatFromJson(ArrayJson, _SKILL_SPEED_Z_,nRoleID,nSkillID);
			
			//屏幕是否震动
			ItemInfo.nScreenIsShake = GetIntFromJson(ArrayJson, _SKILL_IS_SCREEN_SHAKE_,nRoleID,nSkillID);

			//屏幕震动持续时间
			ItemInfo.fShakeDurTime = GetFloatFromJson(ArrayJson, _SKILL_SHAKE_DUR_TIME_,nRoleID,nSkillID);

			//屏幕震动幅度
			ItemInfo.fShakeDegree = GetFloatFromJson(ArrayJson, _SKILL_SHAKE_DEGREE_,nRoleID,nSkillID);

			//屏幕震动次数
			ItemInfo.nShakeCount= GetIntFromJson(ArrayJson, _SKILL_SHAKE_COUNT_,nRoleID,nSkillID);

			//屏幕是否拉伸
			ItemInfo.nIsLensfeatrue = GetIntFromJson(ArrayJson, _SKILL_IS_LENSFEATURE_,nRoleID,nSkillID);

			//屏幕拉伸时间
			ItemInfo.fLensfeatureTime = GetFloatFromJson(ArrayJson, _SKILL_LENSFRATURE_TIME_,nRoleID,nSkillID);

			//屏幕是否变色
			ItemInfo.nIsScreenChangeColor = GetIntFromJson(ArrayJson, _SKILL_IS_SCREEN_CHANGE_COLOR_,nRoleID,nSkillID);

			//屏幕变色时间
			ItemInfo.fScreenChangeColorTime = GetFloatFromJson(ArrayJson, _SKILL_SCREEN_CHANGE_COLOR_TIME_,nRoleID,nSkillID);

			//释放者是否转头
			ItemInfo.nIsTurnRound = GetIntFromJson(ArrayJson, _SKILL_IS_TURN_ROUND_,nRoleID,nSkillID);
			
			/************************************************************************/
			/*				释放者buff                                                                     */
			/************************************************************************/
			//buff 数组大小
			int nBuffArrayCount = DICTOOL->getArrayCount_json(ArrayJson,_SKILL_BUFF_ARRAY_);
			for (int j = 0; j < nBuffArrayCount; j++)
			{
				//数组
				const rapidjson::Value &buffArray = DICTOOL->getSubDictionary_json(ArrayJson,_SKILL_BUFF_ARRAY_, j);

				tagBuffStaticInfo BuffInfo;
				BuffInfo.nBuffID = GetIntFromJson(buffArray, _SKILL_BUFF_ID_,nRoleID,nSkillID);
				BuffInfo.nBuffTime = GetIntFromJson(buffArray, _SKILL_BUFF_TIME_,nRoleID,nSkillID);
				BuffInfo.nBuffTime_g = GetIntFromJson(buffArray, _SKILL_BUFF_TIME_G_,nRoleID,nSkillID);
				BuffInfo.fBuffPoint1 = GetFloatFromJson(buffArray, _SKILL_BUFF_POINT_1_,nRoleID,nSkillID);
				BuffInfo.fBuffPoint1_g = GetFloatFromJson(buffArray, _SKILL_BUFF_POINT_1_G_,nRoleID,nSkillID);
				BuffInfo.fBuffPoint2 = GetFloatFromJson(buffArray, _SKILL_BUFF_POINT_2_,nRoleID,nSkillID);
				BuffInfo.fBuffPoint2_g = GetFloatFromJson(buffArray, _SKILL_BUFF_POINT_2_G_,nRoleID,nSkillID);

				ItemInfo.vecBuff.push_back(BuffInfo);
			}

			/************************************************************************/
			/*				被攻击者buff                                                                     */
			/************************************************************************/
			//buff 数组大小
			int nTargetBuffArrayCount = DICTOOL->getArrayCount_json(ArrayJson,_SKILL_TARGET_BUFF_ARRAY_);
			for (int j = 0; j < nTargetBuffArrayCount; j++)
			{
				//数组
				const rapidjson::Value &TargetbuffArray = DICTOOL->getSubDictionary_json(ArrayJson,_SKILL_TARGET_BUFF_ARRAY_, j);

				tagBuffStaticInfo TargetBuffInfo;
				TargetBuffInfo.nBuffID = GetIntFromJson(TargetbuffArray, _SKILL_BUFF_ID_,nRoleID,nSkillID);
				TargetBuffInfo.nBuffTime = GetIntFromJson(TargetbuffArray, _SKILL_BUFF_TIME_,nRoleID,nSkillID);
				TargetBuffInfo.nBuffTime_g = GetIntFromJson(TargetbuffArray, _SKILL_BUFF_TIME_G_,nRoleID,nSkillID);
				TargetBuffInfo.fBuffPoint1 = GetFloatFromJson(TargetbuffArray, _SKILL_BUFF_POINT_1_,nRoleID,nSkillID);
				TargetBuffInfo.fBuffPoint1_g = GetFloatFromJson(TargetbuffArray, _SKILL_BUFF_POINT_1_G_,nRoleID,nSkillID);
				TargetBuffInfo.fBuffPoint2 = GetFloatFromJson(TargetbuffArray, _SKILL_BUFF_POINT_2_,nRoleID,nSkillID);
				TargetBuffInfo.fBuffPoint2_g = GetFloatFromJson(TargetbuffArray, _SKILL_BUFF_POINT_2_G_,nRoleID,nSkillID);

				ItemInfo.tSkillDamage.vecTargetBuff.push_back(TargetBuffInfo);
			}
	
			/************************************************************************/
			/*				  技能变化信息                                                                   */
			/************************************************************************/
			//变化时间
			ItemInfo.tSkillChange.fChangeTime = GetFloatFromJson(ArrayJson, _SKILL_CHANGE_TIME_,nRoleID,nSkillID);
			 
			//是否有重力：1.有	0.没有
			ItemInfo.tSkillChange.nIsGravity = GetIntFromJson(ArrayJson,_SKILL_IS_GRAVITY_,nRoleID,nSkillID);

			//旧的x轴位置
			ItemInfo.tSkillChange.fOldX = GetFloatFromJson(ArrayJson,_SKILL_OLD_X_,nRoleID,nSkillID);

			//旧的y轴位置
			ItemInfo.tSkillChange.fOldY = GetFloatFromJson(ArrayJson,_SKILL_OLD_Y_,nRoleID,nSkillID);

			//旧的z轴位置
			ItemInfo.tSkillChange.fOldZ = GetFloatFromJson(ArrayJson,_SKILL_OLD_Z_,nRoleID,nSkillID);

			//旧的长度
			ItemInfo.tSkillChange.fOldLength = GetFloatFromJson(ArrayJson,_SKILL_OLD_LENGTH_,nRoleID,nSkillID);

			//旧的宽度
			ItemInfo.tSkillChange.fOldWidth = GetFloatFromJson(ArrayJson,_SKILL_OLD_WIDTH_,nRoleID,nSkillID);

			//旧的高度
			ItemInfo.tSkillChange.fOldHeight = GetFloatFromJson(ArrayJson,_SKILL_OLD_HEIGHT_,nRoleID,nSkillID);

			//新的x轴位置
			ItemInfo.tSkillChange.fNewX = GetFloatFromJson(ArrayJson,_SKILL_NEW_X_,nRoleID,nSkillID);

			//新的y轴位置
			ItemInfo.tSkillChange.fNewY = GetFloatFromJson(ArrayJson,_SKILL_NEW_Y_,nRoleID,nSkillID);

			//新的z轴位置
			ItemInfo.tSkillChange.fNewZ = GetFloatFromJson(ArrayJson,_SKILL_NEW_Z_,nRoleID,nSkillID);

			//新的长度
			ItemInfo.tSkillChange.fNewLength = GetFloatFromJson(ArrayJson,_SKILL_NEW_LENGTH_,nRoleID,nSkillID);

			//新的宽度
			ItemInfo.tSkillChange.fNewWidth = GetFloatFromJson(ArrayJson,_SKILL_NEW_WIDTH_,nRoleID,nSkillID);

			//新的高度
			ItemInfo.tSkillChange.fNewHeight = GetFloatFromJson(ArrayJson,_SKILL_NEW_HEIGHT_,nRoleID,nSkillID);
			

			/************************************************************************/
			/*					技能伤害信息                                                                     */
			/************************************************************************/
			//基础攻击力
			ItemInfo.tSkillDamage.fDamagePoint = GetFloatFromJson(ArrayJson,_SKILL_DAMAGE_POINT_,nRoleID,nSkillID);

			//攻击力成长
			ItemInfo.tSkillDamage.fDamagePoint_g = GetFloatFromJson(ArrayJson,_SKILL_DAMAGE_POINT_G_,nRoleID,nSkillID);

			//伤害百分比
			ItemInfo.tSkillDamage.fDamagePercent = GetFloatFromJson(ArrayJson,_SKILL_DAMAGE_PERCENT_,nRoleID,nSkillID);

			//伤害百分比成长
			ItemInfo.tSkillDamage.fDamagePercent_g = GetFloatFromJson(ArrayJson,_SKILL_DAMAGE_PERCENT_G_,nRoleID,nSkillID);

			//被攻击者，x轴的变化量
			ItemInfo.tSkillDamage.fXDis = GetFloatFromJson(ArrayJson,_SKILL_TARGET_DIS_X_,nRoleID,nSkillID);

			//被攻击者，x轴的时间
			ItemInfo.tSkillDamage.fXTime = GetFloatFromJson(ArrayJson,_SKILL_TARGET_TIME_X_,nRoleID,nSkillID);

			//被攻击者，y轴的变化量
			ItemInfo.tSkillDamage.fYDis = GetFloatFromJson(ArrayJson,_SKILL_TARGET_DIS_Y_,nRoleID,nSkillID);

			//被攻击者，y轴的时间
			ItemInfo.tSkillDamage.fYTime = GetFloatFromJson(ArrayJson,_SKILL_TARGET_TIME_Y_,nRoleID,nSkillID);

			//被攻击者，z轴的速度
			ItemInfo.tSkillDamage.fZSpeed = GetFloatFromJson(ArrayJson,_SKILL_TARGET_SPEED_Z_,nRoleID,nSkillID);

			//被攻击者，是否倒地
			ItemInfo.tSkillDamage.nIsConversely = GetIntFromJson(ArrayJson,_SKILL_IS_CONVERSELY_,nRoleID,nSkillID);

			//被攻击者，硬直增加量
			ItemInfo.tSkillDamage.nCrickAdd = GetIntFromJson(ArrayJson,_SKILL_CRICK_ADD_,nRoleID,nSkillID);

			//攻击之间的间隔（帧）
			ItemInfo.tSkillDamage.nAttackSpeed = GetIntFromJson(ArrayJson,_SKILL_ATTACK_SPEED_,nRoleID,nSkillID);

			//是否为爆炸模式
			ItemInfo.tSkillDamage.nIsExplodeMode = GetIntFromJson(ArrayJson,_SKILL_IS_EXPLODE_MODE_,nRoleID,nSkillID);

			//是否为拉近模式
			ItemInfo.tSkillDamage.nIsPullMode = GetIntFromJson(ArrayJson,_SKILL_IS_PULL_MODE_,nRoleID,nSkillID);

			//暴击率增加量
			ItemInfo.tSkillDamage.nCritAdd = GetIntFromJson(ArrayJson,_SKILL_CRIT_ADD_,nRoleID,nSkillID);

			//暴击伤害增加量
			ItemInfo.tSkillDamage.fCritDamageAdd = GetFloatFromJson(ArrayJson,_SKILL_CRIT_DAMAGE_ADD_,nRoleID,nSkillID);

			//命中增加量
			ItemInfo.tSkillDamage.nHitTargetAdd = GetIntFromJson(ArrayJson,_SKILL_HIT_TARGET_ADD_,nRoleID,nSkillID);

			//是否有方向
			ItemInfo.tSkillDamage.nIsDirection = GetIntFromJson(ArrayJson,_SKILL_IS_DRIECTION,nRoleID,nSkillID);

			//是否有骨骼动画
			ItemInfo.nIsArmature = GetIntFromJson(ArrayJson,_SKILL_IS_ARMATURE_,nRoleID,nSkillID);

			//骨骼动画名称
			ItemInfo.strArmatureName = GetStringFromJson(ArrayJson,_SKILL_ARMATURE_NAME_,nRoleID,nSkillID);

			//是否有音效
			ItemInfo.nIsSound = GetIntFromJson(ArrayJson,_SKILL_IS_SOUND_,nRoleID,nSkillID);

			//音效名称
			ItemInfo.strSoundName = GetStringFromJson(ArrayJson,_SKILL_SOUND_NAME_,nRoleID,nSkillID);

			//是否为吸引模式
			ItemInfo.tSkillDamage.nIsSuctionMode = GetIntFromJson(ArrayJson,_SKILL_IS_SUCTION_MODE_,nRoleID,nSkillID);

			//吸引的速度
			ItemInfo.tSkillDamage.fSuctionSpeed = GetFloatFromJson(ArrayJson,_SKILL_SUCTION_SPEED_,nRoleID,nSkillID);

			//吸引的时间
			ItemInfo.tSkillDamage.fSuctionTime = GetFloatFromJson(ArrayJson,_SKILL_SUCTION_TIME_,nRoleID,nSkillID);

			//吸引到的x轴位置
			ItemInfo.tSkillDamage.fSuctionPosX = GetFloatFromJson(ArrayJson,_SKILL_SUCTION_POS_X_,nRoleID,nSkillID);

			//吸引到的y轴位置
			ItemInfo.tSkillDamage.fSuctionPosY = GetFloatFromJson(ArrayJson,_SKILL_SUCTION_POS_Y_,nRoleID,nSkillID);

			//是否只受伤害，不显示被击动画
			ItemInfo.tSkillDamage.nIsOnlyDamageNotHit = GetIntFromJson(ArrayJson,_SKILL_IS_ONLY_DAMAGE_NOT_HIT_,nRoleID,nSkillID);
			
			//是否吸引到技能块中心
			ItemInfo.tSkillDamage.nIsSuctionSkillItemCenter = GetIntFromJson(ArrayJson,_SKILL_IS_SUCTION_SKILL_ITEM_CENTER_,nRoleID,nSkillID); 

			//是否计算伤害
			ItemInfo.tSkillDamage.nIsDamage = GetIntFromJson(ArrayJson,_SKILL_IS_DAMAGE_,nRoleID,nSkillID); 

			//技能块，是否跟随释放泽
			ItemInfo.tSkillDamage.nIsFollow = GetIntFromJson(ArrayJson,_SKILL_IS_FOLLOW_,nRoleID,nSkillID); 

			//技能块，是否破霸体
			ItemInfo.tSkillDamage.nIsBreakSiperarmor = GetIntFromJson(ArrayJson,_SKILL_IS_BREAK_SIPERARMOR_,nRoleID,nSkillID); 

			//技能块，拳花类型
			ItemInfo.tSkillDamage.nHitEffectType = GetIntFromJson(ArrayJson,_SKILL_HIT_EFFECT_TYPE_,nRoleID,nSkillID); 

			Info.push_back(ItemInfo);
		}

		return true;
	} while (false);
	CCLOG("Fun CNFJsonTranslater::GetSkillItemInfoFromJson Error!");
	return false;
}

//得到景深地图信息
bool CNFJsonTranslater::GetTmxInfoFromJson( const char * szJson,CNFTmxStaticInfoMap &Info,int nStageID)
{
	do 
	{
		//创建Json
		rapidjson::Document json;

		//复制字符串数据
		json.Parse<0>(szJson);

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun CNFJsonTranslater::GetTmxInfoFromJson GetParseError %s\n",json.GetParseError());
		}


		//tmx数组大小
		int nArrayCount = DICTOOL->getArrayCount_json(json,_TMX_ARRAY_);
		for (int i = 0; i < nArrayCount; ++i)
		{
			//tmx数组
			const rapidjson::Value &ArrayJson = DICTOOL->getSubDictionary_json(json,_TMX_ARRAY_, i);

			tagTmxStaticInfo TmxInfo;
			TmxInfo.strTmxName = GetStringFromJson(ArrayJson,_TMX_NAME_,nStageID);
			TmxInfo.fStartPosX = GetFloatFromJson(ArrayJson,_TMX_START_POS_X_);
			TmxInfo.fStartPosY = GetFloatFromJson(ArrayJson,_TMX_START_POS_Y_);

			Info[i] = TmxInfo;

		}

		return true;
	} while (false);
	CCLog("Fun CNFJsonTranslater::GetTmxInfoFromJson Error!");
	return false;
}

//得到主角AI信息
bool CNFJsonTranslater::GetProtagonistAIInfoFromJson( const char * szJson,tagProtagonistAIInfo &Info,int nRoleID )
{
	do 
	{
		//创建Json
		rapidjson::Document json;

		//复制字符串数据
		json.Parse<0>(szJson);

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun CNFJsonTranslater::GetProtagonistAIInfoFromJson GetParseError %s\n",json.GetParseError());
		}

		/************************************************************************/
		/*				Json解析                                                                     */
		/************************************************************************/
		//治疗技能数组
		int nArrayCount = DICTOOL->getArrayCount_json(json,_PRO_AI_TREAT_SKILL_);
		for (int i = 0; i < nArrayCount; ++i)
		{
			const rapidjson::Value &TreatArray = DICTOOL->getSubDictionary_json(json,_PRO_AI_TREAT_SKILL_, i);

			tagTreatSkillInfo ItemInfo;

			//技能ID
			ItemInfo.nSkillID = GetIntFromJson(TreatArray, _PRO_AI_SKILL_ID_,nRoleID);

			//出发治疗的最小血量百分比
			ItemInfo.fBloodPer = GetFloatFromJson(TreatArray, _PRO_AI_BLOOD_PER_,nRoleID);

			Info.vecTreatSkill.push_back(ItemInfo);
		}

		//必然技能数组
		nArrayCount = DICTOOL->getArrayCount_json(json,_PRO_AI_MUST_SKILL_);
		for (int i = 0; i < nArrayCount; ++i)
		{
			const rapidjson::Value &MustArray = DICTOOL->getSubDictionary_json(json,_PRO_AI_MUST_SKILL_, i);

			tagMustSkillInfo ItemInfo;

			//技能ID
			ItemInfo.nSkillID = GetIntFromJson(MustArray, _PRO_AI_SKILL_ID_,nRoleID);

			Info.vecMustSkill.push_back(ItemInfo);
		}

		//距离技能数组
		nArrayCount = DICTOOL->getArrayCount_json(json,_PRO_AI_DISTANCE_SKILL_);
		for (int i = 0; i < nArrayCount; ++i)
		{
			const rapidjson::Value &DistanceArray = DICTOOL->getSubDictionary_json(json,_PRO_AI_DISTANCE_SKILL_, i);

			tagDistanceSkillInfo ItemInfo;

			//技能攻击的最大距离
			ItemInfo.fSkillDistance = GetFloatFromJson(DistanceArray, _PRO_AI_SKILL_DISTANCE_,nRoleID);

			//距离技能块数组
			int nArrayItemCount = DICTOOL->getArrayCount_json(DistanceArray,_PRO_AI_DISTANCE_ITEM_);
			for (int i = 0; i < nArrayItemCount; ++i)
			{
				const rapidjson::Value &DistanceItemArray = DICTOOL->getSubDictionary_json(DistanceArray,_PRO_AI_DISTANCE_ITEM_, i);

				tagDistanceItem DistanceItem;

				//技能ID
				DistanceItem.nSkillID = GetIntFromJson(DistanceItemArray, _PRO_AI_SKILL_ID_,nRoleID);

				//技能延迟时间
				DistanceItem.nDelayTime = GetIntFromJson(DistanceItemArray, _PRO_AI_DELAY_TIME_,nRoleID);

				ItemInfo.vecDistanceItem.push_back(DistanceItem);
			}

			Info.vecDistanceSkill.push_back(ItemInfo);
		}


		return true;
	} while (false);
	CCLOG("Fun CNFJsonTranslater::GetProtagonistAIInfoFromJson Error!");
	return false;
}

//得到NPC信息
bool CNFJsonTranslater::GetNPCInfoFromJson( const char * szJson,CNFNPCJsonInfoVec &Info,int nStageID )
{
	do 
	{
		//创建Json
		rapidjson::Document json;

		//复制字符串数据
		json.Parse<0>(szJson);

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun CNFJsonTranslater::GetNPCInfoFromJson GetParseError %s\n",json.GetParseError());
		}

		int nArrayCount = DICTOOL->getArrayCount_json(json,_NPC_ARR_);
		for (int i = 0; i < nArrayCount; ++i)
		{
			const rapidjson::Value &ArrayJson = DICTOOL->getSubDictionary_json(json,_NPC_ARR_, i);

			tagNPCJsonInfo NPCInfo;
			NPCInfo.nNPCID = GetIntFromJson(ArrayJson,_NPC_ID_,nStageID);
			NPCInfo.fPosX = GetFloatFromJson(ArrayJson,_NPC_X_,nStageID);
			NPCInfo.fPosY = GetFloatFromJson(ArrayJson,_NPC_Y_,nStageID);
			NPCInfo.fPosZ = GetFloatFromJson(ArrayJson,_NPC_Z_,nStageID);

			Info.push_back(NPCInfo);
		}

		return true;
	} while (false);
	CCLog("Fun CNFJsonTranslater::GetNPCInfoFromJson Error!");
	return false;
}

//得到Door信息
bool CNFJsonTranslater::GetDoorInfoFromJson( const char * szJson,CNFDoorJsonInfoVec &Info,int nStageID )
{
	do 
	{
		//创建Json
		rapidjson::Document json;

		//复制字符串数据
		json.Parse<0>(szJson);

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun CNFJsonTranslater::GetDoorInfoFromJson GetParseError %s\n",json.GetParseError());
		}

		int nArrayCount = DICTOOL->getArrayCount_json(json,_DOOR_ARR_);
		for (int i = 0; i < nArrayCount; ++i)
		{
			const rapidjson::Value &ArrayJson = DICTOOL->getSubDictionary_json(json,_DOOR_ARR_, i);

			tagDoorJsonInfo DoorInfo;
			DoorInfo.nDoorID = GetIntFromJson(ArrayJson,_DOOR_ID_,nStageID);
			DoorInfo.fPosX = GetFloatFromJson(ArrayJson,_DOOR_X_,nStageID);
			DoorInfo.fPosY = GetFloatFromJson(ArrayJson,_DOOR_Y_,nStageID);
			DoorInfo.fPosZ = GetFloatFromJson(ArrayJson,_DOOR_Z_,nStageID);

			Info.push_back(DoorInfo);
		}

		return true;
	} while (false);
	CCLog("Fun CNFJsonTranslater::GetDoorInfoFromJson Error!");
	return false;
}















///************************************************************************/
///*						反解析成Json                                                                     */
///************************************************************************/
//bool CNFJsonTranslater::GetJsonFromSkillItemInfo(const char * szJson,tagSkillItemStaticInfo Info)
//{
//	do
//	{
//		//初始化
//		rapidjson::Document document;
//		document.SetObject();
//		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
//
//		//创建一个数组
//		rapidjson::Value array(rapidjson::kArrayType);
//		for (int i = 0; i < 10; i++)
//		{
//			rapidjson::Value object(rapidjson::kObjectType);
//			object.AddMember("id", 1, allocator);
//			object.AddMember("name", "test", allocator);
//			object.AddMember("version", 1.01, allocator);
//			object.AddMember("vip", true, allocator);
//
//			array.PushBack(object, allocator);
//
//			//array.PushBack(i, allocator);
//		}
//		
//		//添加一个项
//		document.AddMember("title", "PLAYER INFO", allocator);
//		//添加一个项
//		document.AddMember("akb", 48 , allocator);
//		//添加一个数组
//		document.AddMember("players", array, allocator);
//
//		//输出
//		rapidjson::StringBuffer buffer;
//		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
//		document.Accept(writer);
//		auto out = buffer.GetString();
//		CCLOG("out: %s", out); 
//
//		return true;
//	} while (false);
//	CCLOG("Fun CNFJsonTranslater::GetJsonFromSkillItemInfo Error!");
//	return false;
//}



















