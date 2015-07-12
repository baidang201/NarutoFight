#include "NFDataManager.h"
#include "sqlite3.h"				//SQLite相关头文件
#include "NFJsonTranslator.h"		//Json解析
#include <algorithm>				//vector排序
#define TS_SQL_CMD_LENTH	1024	//SQL语句长度限制

//容器排序：技能延迟时间
bool lessmark(const tagSkillItemStaticInfo& s1,const tagSkillItemStaticInfo& s2)	//升序
{
	return s1.fDelayTime < s2.fDelayTime;  
}
bool greatermark(const tagSkillItemStaticInfo& s1,const tagSkillItemStaticInfo& s2)  //降序
{  
	return s1.fDelayTime > s2.fDelayTime;  
}


/************************************************************************/
/*					数据库校验                                                                     */
/************************************************************************/
//整数
int GetIntFromTable( char * szItemName, int n_column, char ** column_value, char ** column_name )
{
	for(int i=0;i<n_column;i++)
	{
		if(strcmp(column_name[i],szItemName)==0)
		{
			return atoi(column_value[i]);
		}
	}
	CCLOG("Can't Find int [%s]",szItemName);
	return 0;
}
//小数
float GetFloatFromTable( char * szItemName, int n_column, char ** column_value, char ** column_name )
{
	for(int i=0;i<n_column;i++)
	{
		if(strcmp(column_name[i],szItemName)==0)
		{
			return atof(column_value[i]);
		}
	}
	CCLOG("Can't Find float [%s]",szItemName);
	return 0;
}
//字符串
string GetStringFromTable( char * szItemName, int n_column, char ** column_value, char ** column_name)
{
	for(int i=0;i<n_column;i++)
	{
		if(strcmp(column_name[i],szItemName)==0)
		{
			return column_value[i];
		}
	}
	CCLOG("Can't Find string [%s]",szItemName);
	return NULL;
}


/************************************************************************/
/*				静态数据管理                                             */
/************************************************************************/
static CNFStaticDataManager * g_pStaticDataManager=NULL;
CNFStaticDataManager::CNFStaticDataManager()
{
	m_StageStaticInfoMap.clear();
	m_ProtagonistStaticInfoMap.clear();
	m_EnemyStaticInfoMap.clear();
	m_ArticleStaticInfoMap.clear();
	m_EnemyAIStaticInfoMap.clear();
	m_LevelUpexpStaticInfoMap.clear();
	m_SkillStaticInfoMap.clear();
	m_StageWaveStaticInfoMap.clear();
	m_SkillArmatureInfoVec.clear();		//临时
	m_EquipStaticInfoMap.clear();
	m_NPCStaticInfoMap.clear();
	m_DoorStaticInfoMap.clear();
}
CNFStaticDataManager * CNFStaticDataManager::SharedData()
{
	if(g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	}
	return g_pStaticDataManager;
}

void CNFStaticDataManager::ReleaseData()
{
	g_pStaticDataManager->m_StageStaticInfoMap.clear();
	g_pStaticDataManager->m_ProtagonistStaticInfoMap.clear();
	g_pStaticDataManager->m_EnemyStaticInfoMap.clear();
	g_pStaticDataManager->m_ArticleStaticInfoMap.clear();
	g_pStaticDataManager->m_EnemyAIStaticInfoMap.clear();
	g_pStaticDataManager->m_LevelUpexpStaticInfoMap.clear();
	g_pStaticDataManager->m_SkillStaticInfoMap.clear();
	g_pStaticDataManager->m_StageWaveStaticInfoMap.clear();
	g_pStaticDataManager->m_SkillArmatureInfoVec.clear();			//临时
	g_pStaticDataManager->m_EquipStaticInfoMap.clear();
	g_pStaticDataManager->m_NPCStaticInfoMap.clear();
	g_pStaticDataManager->m_DoorStaticInfoMap.clear();
}

void CNFStaticDataManager::initBasicData()
{
	do 
	{
		//读取npc静态信息：必须在读取关卡信息之前读取
		ReadNPCStaticFromDB();

		//读取door静态信息：必须在读取关卡信息之前读取
		ReadDoorStaticFromDB();

		//读取关卡信息
		ReadStageStaticFromDB();

		//读取主角信息
		ReadProtagonistStaticFromDB();

		//读取敌人信息
		ReadEnemyStaticFromDB();

		//读取物品信息
		ReadArticleStaticFromDB();

		//读取敌人AI信息
		ReadEnemyAIStaticFromDB();

		//读取升级经验信息
		ReadLevelUpExpStaticFromDB();

		//读取技能静态信息
		ReadSkillStaticFromDB();

		//读取波次静态信息
		ReadWaveStaticFromDB();

		//临时：读取技能骨骼信息
		//ReadSkillArmatureInfo();

		//读取装备静态信息
		ReadEquipStaticFromDB();

		return ;
	} while (false);
	CCLog("Fun CNFStaticDataManager::initBasicData Error!");
}

/************************************************************************/
/*					关卡静态信息                                                    */
/************************************************************************/
bool CNFStaticDataManager::GetStageStaticInfo( int nStageID,tagStageStaticInfo &Info)
{
	CNFStageStaticInfoMap::iterator iFind = m_StageStaticInfoMap.find(nStageID);
	if (iFind==m_StageStaticInfoMap.end())
		return false;

	Info=m_StageStaticInfoMap[nStageID];
	return true;
}

bool CNFStaticDataManager::ReadStageStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_stage_info",CNFStaticDataManager::LoadStageStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadStageStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadStageStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nStageID = GetIntFromTable("i_stage_id",n_column,column_value,column_name);

	tagStageStaticInfo NewStageStaticInfo;
	NewStageStaticInfo.fRate = GetFloatFromTable("f_stage_rate",n_column,column_value,column_name);
	NewStageStaticInfo.fGravity = GetFloatFromTable("f_stage_gravity",n_column,column_value,column_name);
	NewStageStaticInfo.fLength = GetFloatFromTable("f_3d_length",n_column,column_value,column_name);
	NewStageStaticInfo.fWidth = GetFloatFromTable("f_3d_width",n_column,column_value,column_name);
	NewStageStaticInfo.fHeight = GetFloatFromTable("f_3d_height",n_column,column_value,column_name);
	NewStageStaticInfo.fProtagonistX =GetFloatFromTable("f_protagonist_x",n_column,column_value,column_name);
	NewStageStaticInfo.fProtagonistY = GetFloatFromTable("f_protagonist_y",n_column,column_value,column_name);
	NewStageStaticInfo.fProtagonistZ = GetFloatFromTable("f_protagonist_z",n_column,column_value,column_name);
	NewStageStaticInfo.strTMXWorldName = GetStringFromTable("s_tmx_world_name",n_column,column_value,column_name);
	
	//得到tmx地图 Json信息
	string strTmxJson = GetStringFromTable("s_tmx_json",n_column,column_value,column_name);
	CNFJsonTranslater::SharedData()->GetTmxInfoFromJson(strTmxJson.c_str(),NewStageStaticInfo.TmxStaticInfoMap,nStageID);

	//得到npc Json信息
	string strNPCJson = GetStringFromTable("s_npc_json",n_column,column_value,column_name);
	CNFJsonTranslater::SharedData()->GetNPCInfoFromJson(strNPCJson.c_str(),NewStageStaticInfo.NPCJsonInfoVec,nStageID);
	//得到npc 静态信息
	for (CNFNPCJsonInfoVec::iterator iter = NewStageStaticInfo.NPCJsonInfoVec.begin();iter != NewStageStaticInfo.NPCJsonInfoVec.end();iter++)
		CNFStaticDataManager::SharedData()->GetNPCStaticInfo(iter->nNPCID,iter->NPCStaticInfo);

	//得到door Json信息
	string strDoorJson = GetStringFromTable("s_door_json",n_column,column_value,column_name);
	CNFJsonTranslater::SharedData()->GetDoorInfoFromJson(strDoorJson.c_str(),NewStageStaticInfo.DoorJsonInfoVec,nStageID);
	//得到door 静态信息
	for (CNFDoorJsonInfoVec::iterator iter = NewStageStaticInfo.DoorJsonInfoVec.begin();iter != NewStageStaticInfo.DoorJsonInfoVec.end();iter++)
		CNFStaticDataManager::SharedData()->GetDoorStaticInfo(iter->nDoorID,iter->DoorStaticInfo);

	g_pStaticDataManager->m_StageStaticInfoMap[nStageID]=NewStageStaticInfo;

	return 0;
}

/************************************************************************/
/*					主角静态信息                                                                     */
/************************************************************************/
bool CNFStaticDataManager::GetProtagonistStaticInfo( const int nProtagonistID,tagProtagonistStaticInfo &Info )
{
	CNFProtagonistStaticInfoMap::iterator iFind = m_ProtagonistStaticInfoMap.find(nProtagonistID);
	if (iFind==m_ProtagonistStaticInfoMap.end())
		return false;

	Info=m_ProtagonistStaticInfoMap[nProtagonistID];
	return true;
}

bool CNFStaticDataManager::ReadProtagonistStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_protagonist_info",CNFStaticDataManager::LoadProtagonistStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadProtagonistStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadProtagonistStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nRoleID = GetIntFromTable("i_role_id",n_column,column_value,column_name);

	tagProtagonistStaticInfo NewProtagonistStaticInfo;
	NewProtagonistStaticInfo.fHP = GetFloatFromTable("f_hp",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fHp_g = GetFloatFromTable("f_hp_g",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fMP = GetFloatFromTable("f_mp",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fMp_g = GetFloatFromTable("f_mp_g",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fDefense = GetFloatFromTable("f_defense",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fDefense_g = GetFloatFromTable("f_defense_g",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fAttack = GetFloatFromTable("f_attack",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fAttack_g = GetFloatFromTable("f_attack_g",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fDomination = GetFloatFromTable("f_domination",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fMoveSpeed = GetFloatFromTable("f_move_speed",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fAttackSpeed = GetFloatFromTable("f_attack_speed",n_column,column_value,column_name);
	
	NewProtagonistStaticInfo.nCrit = GetIntFromTable("i_crit",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fCritDamage = GetFloatFromTable("f_crit_damage",n_column,column_value,column_name);
	NewProtagonistStaticInfo.nCrickTime = GetIntFromTable("i_crick_time",n_column,column_value,column_name);
	NewProtagonistStaticInfo.nConverselyTime = GetIntFromTable("i_conversely_time",n_column,column_value,column_name);
	NewProtagonistStaticInfo.nHitTarget = GetIntFromTable("i_hit_target",n_column,column_value,column_name);
	NewProtagonistStaticInfo.nAvoid = GetIntFromTable("i_avoid",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fTreatCoefficient = GetFloatFromTable("f_treat_coefficient",n_column,column_value,column_name);

	NewProtagonistStaticInfo.fLength_Hold = GetFloatFromTable("f_length_hold",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fWidth_Hold = GetFloatFromTable("f_width_hold",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fHeight_Hold = GetFloatFromTable("f_height_hold",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fLength_Move = GetFloatFromTable("f_length_move",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fWidth_Move = GetFloatFromTable("f_width_move",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fHeight_Move = GetFloatFromTable("f_height_move",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fLength_Conversely = GetFloatFromTable("f_length_conversely",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fWidth_Conversely = GetFloatFromTable("f_width_conversely",n_column,column_value,column_name);
	NewProtagonistStaticInfo.fHeight_Conversely = GetFloatFromTable("f_height_conversely",n_column,column_value,column_name);
	
	//主角ai的Json
	string strProAIJson = GetStringFromTable("s_protagonist_ai_json",n_column,column_value,column_name);
	CNFJsonTranslater::SharedData()->GetProtagonistAIInfoFromJson(strProAIJson.c_str(),NewProtagonistStaticInfo.tProtagonistAIInfo,nRoleID);
	
	NewProtagonistStaticInfo.strBeHitSoundName = GetStringFromTable("s_behit_sound_name",n_column,column_value,column_name);
	NewProtagonistStaticInfo.strDeadSoundName = GetStringFromTable("s_dead_sound_name",n_column,column_value,column_name);
	NewProtagonistStaticInfo.strArmatureName = GetStringFromTable("s_armature_name",n_column,column_value,column_name);
	NewProtagonistStaticInfo.nCommonAttackNum = GetIntFromTable("i_common_attack_num",n_column,column_value,column_name);

	g_pStaticDataManager->m_ProtagonistStaticInfoMap[nRoleID]=NewProtagonistStaticInfo;

	return 0;
}

/************************************************************************/
/*					敌人静态信息                                                                     */
/************************************************************************/
bool CNFStaticDataManager::GetEnemyStaticInfo( const int nRoleID,tagEnemyStaticInfo &Info)
{
	CNFEnemyStaticInfoMap::iterator iFind = m_EnemyStaticInfoMap.find(nRoleID);
	if (iFind==m_EnemyStaticInfoMap.end())
		return false;

	Info=m_EnemyStaticInfoMap[nRoleID];
	return true;
}

bool CNFStaticDataManager::ReadEnemyStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_enemy_info",CNFStaticDataManager::LoadEnemyStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadEnemyStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadEnemyStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 
	
	int nRoleID = GetIntFromTable("i_role_id",n_column,column_value,column_name);

	tagEnemyStaticInfo NewEnemyStaticInfo;
	NewEnemyStaticInfo.fHP = GetFloatFromTable("f_hp",n_column,column_value,column_name);
	NewEnemyStaticInfo.fHp_g = GetFloatFromTable("f_hp_g",n_column,column_value,column_name);
	NewEnemyStaticInfo.fMP = GetFloatFromTable("f_mp",n_column,column_value,column_name);
	NewEnemyStaticInfo.fMp_g = GetFloatFromTable("f_mp_g",n_column,column_value,column_name);
	NewEnemyStaticInfo.fDefense = GetFloatFromTable("f_defense",n_column,column_value,column_name);
	NewEnemyStaticInfo.fDefense_g = GetFloatFromTable("f_defense_g",n_column,column_value,column_name);
	NewEnemyStaticInfo.fAttack = GetFloatFromTable("f_attack",n_column,column_value,column_name);
	NewEnemyStaticInfo.fAttack_g = GetFloatFromTable("f_attack_g",n_column,column_value,column_name);
	NewEnemyStaticInfo.fDomination = GetFloatFromTable("f_domination",n_column,column_value,column_name);
	NewEnemyStaticInfo.fMoveSpeed = GetFloatFromTable("f_move_speed",n_column,column_value,column_name);
	NewEnemyStaticInfo.fAttackSpeed = GetFloatFromTable("f_attack_speed",n_column,column_value,column_name);

	NewEnemyStaticInfo.nCrit = GetIntFromTable("i_crit",n_column,column_value,column_name);
	NewEnemyStaticInfo.fCritDamage = GetFloatFromTable("f_crit_damage",n_column,column_value,column_name);
	NewEnemyStaticInfo.nCrickTime = GetIntFromTable("i_crick_time",n_column,column_value,column_name);
	NewEnemyStaticInfo.nConverselyTime = GetIntFromTable("i_conversely_time",n_column,column_value,column_name);
	NewEnemyStaticInfo.nTurnRoundTime = GetIntFromTable("i_turn_round_time",n_column,column_value,column_name);
	NewEnemyStaticInfo.nHitTarget = GetIntFromTable("i_hit_target",n_column,column_value,column_name);
	NewEnemyStaticInfo.nAvoid = GetIntFromTable("i_avoid",n_column,column_value,column_name);
	NewEnemyStaticInfo.fTreatCoefficient = GetFloatFromTable("f_treat_coefficient",n_column,column_value,column_name);

	NewEnemyStaticInfo.fLength_Hold = GetFloatFromTable("f_length_hold",n_column,column_value,column_name);
	NewEnemyStaticInfo.fWidth_Hold = GetFloatFromTable("f_width_hold",n_column,column_value,column_name);
	NewEnemyStaticInfo.fHeight_Hold = GetFloatFromTable("f_height_hold",n_column,column_value,column_name);
	NewEnemyStaticInfo.fLength_Move = GetFloatFromTable("f_length_move",n_column,column_value,column_name);
	NewEnemyStaticInfo.fWidth_Move = GetFloatFromTable("f_width_move",n_column,column_value,column_name);
	NewEnemyStaticInfo.fHeight_Move = GetFloatFromTable("f_height_move",n_column,column_value,column_name);
	NewEnemyStaticInfo.fLength_Conversely = GetFloatFromTable("f_length_conversely",n_column,column_value,column_name);
	NewEnemyStaticInfo.fWidth_Conversely = GetFloatFromTable("f_width_conversely",n_column,column_value,column_name);
	NewEnemyStaticInfo.fHeight_Conversely = GetFloatFromTable("f_height_conversely",n_column,column_value,column_name);
	NewEnemyStaticInfo.fNearDis = GetFloatFromTable("f_near_dis",n_column,column_value,column_name);
	NewEnemyStaticInfo.fFarDis = GetFloatFromTable("f_far_dis",n_column,column_value,column_name);
	NewEnemyStaticInfo.strArmatureName = GetStringFromTable("s_armature_name",n_column,column_value,column_name);

	g_pStaticDataManager->m_EnemyStaticInfoMap[nRoleID]=NewEnemyStaticInfo;

	return 0;
}


/************************************************************************/
/*					物品静态信息                                                                     */
/************************************************************************/
bool CNFStaticDataManager::GetArticleStaticInfo( const int nArticleID,tagArticleStaticInfo &Info )
{
	CNFArticleStaticInfoMap::iterator iFind = m_ArticleStaticInfoMap.find(nArticleID);
	if (iFind==m_ArticleStaticInfoMap.end())
		return false;

	Info=m_ArticleStaticInfoMap[nArticleID];
	return true;
}

bool CNFStaticDataManager::ReadArticleStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_article_info",CNFStaticDataManager::LoadArticleStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadArticleStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadArticleStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nArticleID = GetIntFromTable("i_article_id",n_column,column_value,column_name);

	tagArticleStaticInfo NewArticleStaticInfo;
	NewArticleStaticInfo.fHp = GetFloatFromTable("f_hp",n_column,column_value,column_name);
	NewArticleStaticInfo.nIsInvincible = GetIntFromTable("i_is_invincible",n_column,column_value,column_name);
	NewArticleStaticInfo.fLength_Hold = GetFloatFromTable("f_length_hold",n_column,column_value,column_name);
	NewArticleStaticInfo.fWidth_Hold = GetFloatFromTable("f_width_hold",n_column,column_value,column_name);
	NewArticleStaticInfo.fHeight_Hold = GetFloatFromTable("f_height_hold",n_column,column_value,column_name);

	g_pStaticDataManager->m_ArticleStaticInfoMap[nArticleID]=NewArticleStaticInfo;

	return 0;
}

/************************************************************************/
/*					敌人AI静态信息                                                                     */
/************************************************************************/
bool CNFStaticDataManager::GetEnemyAIStaticInfo( const int nRoleID,CMapAI &Info )
{
	CEnemyAIStaticInfoMap::iterator iFind = m_EnemyAIStaticInfoMap.find(nRoleID);
	if (iFind==m_EnemyAIStaticInfoMap.end())
		return false;

	Info=m_EnemyAIStaticInfoMap[nRoleID];
	return true;
}

bool CNFStaticDataManager::ReadEnemyAIStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_enemy_ai_info",CNFStaticDataManager::LoadEnemyAIStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadEnemyAIStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadEnemyAIStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nRoleID = GetIntFromTable("i_role_id",n_column,column_value,column_name);

	CMapAI NewMapAI;
	//敌人AI条件1
	CMapAIActionOnKind NewAIKind1;
	NewAIKind1[enEnemyAIHold] = GetIntFromTable("i_samecross_inrangelong_and_hold",n_column,column_value,column_name);
	NewAIKind1[enEnemyAICrosswiseApproach] = GetIntFromTable("i_samecross_inrangelong_and_crossapproach",n_column,column_value,column_name);
	NewAIKind1[enEnemyAICrosswiseDisapproach] = GetIntFromTable("i_samecross_inrangelong_and_crossdisapproach",n_column,column_value,column_name);
	NewAIKind1[enEnemyLengthwaysApproach] = GetIntFromTable("i_samecross_inrangelong_and_lengthwaysapproach",n_column,column_value,column_name);
	NewAIKind1[enEnemyLengthwaysDisapproach] = GetIntFromTable("i_samecross_inrangelong_and_lengthwaysdisapproach",n_column,column_value,column_name);
	NewAIKind1[enEnemyDirectlyApproach] = GetIntFromTable("i_samecross_inrangelong_and_directlyapproach",n_column,column_value,column_name);
	NewAIKind1[enEnemyCircuity] = GetIntFromTable("i_samecross_inrangelong_and_circuiy",n_column,column_value,column_name);
	NewAIKind1[enEnemyAttackTypeShort] = GetIntFromTable("i_samecross_inrangelong_and_attackshort",n_column,column_value,column_name);
	NewAIKind1[enEnemyAttackTypeLong] = GetIntFromTable("i_samecross_inrangelong_and_attacklong",n_column,column_value,column_name);
	NewMapAI[enEnemyCondition_SameCrosswise_InRangeLong] = NewAIKind1;
	
	//敌人AI条件2
	CMapAIActionOnKind NewAIKind2;
	NewAIKind2[enEnemyAIHold] = GetIntFromTable("i_samecross_inrangeshort_and_hold",n_column,column_value,column_name);
	NewAIKind2[enEnemyAICrosswiseApproach] = GetIntFromTable("i_samecross_inrangeshort_and_crossapproach",n_column,column_value,column_name);
	NewAIKind2[enEnemyAICrosswiseDisapproach] = GetIntFromTable("i_samecross_inrangeshort_and_crossdisapproach",n_column,column_value,column_name);
	NewAIKind2[enEnemyLengthwaysApproach] = GetIntFromTable("i_samecross_inrangeshort_and_lengthwaysapproach",n_column,column_value,column_name);
	NewAIKind2[enEnemyLengthwaysDisapproach] = GetIntFromTable("i_samecross_inrangeshort_and_lengthwaysdisapproach",n_column,column_value,column_name);
	NewAIKind2[enEnemyDirectlyApproach] = GetIntFromTable("i_samecross_inrangeshort_and_directlyapproach",n_column,column_value,column_name);
	NewAIKind2[enEnemyCircuity] = GetIntFromTable("i_samecross_inrangeshort_and_circuiy",n_column,column_value,column_name);
	NewAIKind2[enEnemyAttackTypeShort] = GetIntFromTable("i_samecross_inrangeshort_and_attackshort",n_column,column_value,column_name);
	NewAIKind2[enEnemyAttackTypeLong] = GetIntFromTable("i_samecross_inrangeshort_and_attacklong",n_column,column_value,column_name);
	NewMapAI[enEnemyCondition_SameCrosswise_InRangeShort] = NewAIKind2;

	//敌人AI条件3
	CMapAIActionOnKind NewAIKind3;
	NewAIKind3[enEnemyAIHold] = GetIntFromTable("i_samecross_noinrange_and_hold",n_column,column_value,column_name);
	NewAIKind3[enEnemyAICrosswiseApproach] = GetIntFromTable("i_samecross_noinrange_and_crossapproach",n_column,column_value,column_name);
	NewAIKind3[enEnemyAICrosswiseDisapproach] = GetIntFromTable("i_samecross_noinrange_and_crossdisapproach",n_column,column_value,column_name);
	NewAIKind3[enEnemyLengthwaysApproach] = GetIntFromTable("i_samecross_noinrange_and_lengthwaysapproach",n_column,column_value,column_name);
	NewAIKind3[enEnemyLengthwaysDisapproach] = GetIntFromTable("i_samecross_noinrange_and_lengthwaysdisapproach",n_column,column_value,column_name);
	NewAIKind3[enEnemyDirectlyApproach] = GetIntFromTable("i_samecross_noinrange_and_directlyapproach",n_column,column_value,column_name);
	NewAIKind3[enEnemyCircuity] = GetIntFromTable("i_samecross_noinrange_ands_circuiy",n_column,column_value,column_name);
	NewAIKind3[enEnemyAttackTypeShort] = GetIntFromTable("i_samecross_noinrange_and_attackshort",n_column,column_value,column_name);
	NewAIKind3[enEnemyAttackTypeLong] = GetIntFromTable("i_samecross_noinrange_and_attacklong",n_column,column_value,column_name);
	NewMapAI[enEnemyCondition_SameCrosswise_NotInRange] = NewAIKind3;

	//敌人AI条件4
	CMapAIActionOnKind NewAIKind4;
	NewAIKind4[enEnemyAIHold] = GetIntFromTable("i_notsamecross_near_and_hold",n_column,column_value,column_name);
	NewAIKind4[enEnemyAICrosswiseApproach] = GetIntFromTable("i_notsamecross_near_and_crossapproach",n_column,column_value,column_name);
	NewAIKind4[enEnemyAICrosswiseDisapproach] = GetIntFromTable("i_notsamecross_near_and_crossdisapproach",n_column,column_value,column_name);
	NewAIKind4[enEnemyLengthwaysApproach] = GetIntFromTable("i_notsamecross_near_and_lengthwaysapproach",n_column,column_value,column_name);
	NewAIKind4[enEnemyLengthwaysDisapproach] = GetIntFromTable("i_notsamecross_near_and_lengthwaysdisapproach",n_column,column_value,column_name);
	NewAIKind4[enEnemyDirectlyApproach] = GetIntFromTable("i_notsamecross_near_and_directlyapproach",n_column,column_value,column_name);
	NewAIKind4[enEnemyCircuity] = GetIntFromTable("i_notsamecross_near_and_circuiy",n_column,column_value,column_name);
	NewAIKind4[enEnemyAttackTypeShort] = GetIntFromTable("i_notsamecross_near_and_attackshort",n_column,column_value,column_name);
	NewAIKind4[enEnemyAttackTypeLong] = GetIntFromTable("i_notsamecross_near_and_attacklong",n_column,column_value,column_name);
	NewMapAI[enEnemyCondition_NotSameCrosswiss_Near] = NewAIKind4;

	//敌人AI条件5
	CMapAIActionOnKind NewAIKind5;
	NewAIKind5[enEnemyAIHold] = GetIntFromTable("i_notsamecross_far_faceplayer_and_hold",n_column,column_value,column_name);
	NewAIKind5[enEnemyAICrosswiseApproach] = GetIntFromTable("i_notsamecross_far_faceplayer_and_crossapproach",n_column,column_value,column_name);
	NewAIKind5[enEnemyAICrosswiseDisapproach] = GetIntFromTable("i_notsamecross_far_faceplayer_and_crossdisapproach",n_column,column_value,column_name);
	NewAIKind5[enEnemyLengthwaysApproach] = GetIntFromTable("i_notsamecross_far_faceplayer_and_lengthwaysapproach",n_column,column_value,column_name);
	NewAIKind5[enEnemyLengthwaysDisapproach] = GetIntFromTable("i_notsamecross_far_faceplayer_and_lengthwaysdisapproach",n_column,column_value,column_name);
	NewAIKind5[enEnemyDirectlyApproach] = GetIntFromTable("i_notsamecross_far_faceplayer_and_directlyapproach",n_column,column_value,column_name);
	NewAIKind5[enEnemyCircuity] = GetIntFromTable("i_notsamecross_far_faceplayer_and_circuiy",n_column,column_value,column_name);
	NewAIKind5[enEnemyAttackTypeShort] = GetIntFromTable("i_notsamecross_far_faceplayer_and_attackshort",n_column,column_value,column_name);
	NewAIKind5[enEnemyAttackTypeLong] = GetIntFromTable("i_notsamecross_far_faceplayer_and_attacklong",n_column,column_value,column_name);
	NewMapAI[enEnemyCondition_NotSameCrosswiss_Far_FacePlayer] = NewAIKind5;

	//敌人AI条件6
	CMapAIActionOnKind NewAIKind6;
	NewAIKind6[enEnemyAIHold] = GetIntFromTable("i_notsamecross_far_notfaceplayer_and_hold",n_column,column_value,column_name);
	NewAIKind6[enEnemyAICrosswiseApproach] = GetIntFromTable("i_notsamecross_far_notfaceplayer_and_crossapproach",n_column,column_value,column_name);
	NewAIKind6[enEnemyAICrosswiseDisapproach] = GetIntFromTable("i_notsamecross_far_notfaceplayer_and_crossdisapproach",n_column,column_value,column_name);
	NewAIKind6[enEnemyLengthwaysApproach] = GetIntFromTable("i_notsamecross_far_notfaceplayer_and_lengthwaysapproach",n_column,column_value,column_name);
	NewAIKind6[enEnemyLengthwaysDisapproach] = GetIntFromTable("i_notsamecross_far_notfaceplayer_and_lengthwaysdisapproach",n_column,column_value,column_name);
	NewAIKind6[enEnemyDirectlyApproach] = GetIntFromTable("i_notsamecross_far_notfaceplayer_and_directlyapproach",n_column,column_value,column_name);
	NewAIKind6[enEnemyCircuity] = GetIntFromTable("i_notsamecross_far_notfaceplayer_and_circuiy",n_column,column_value,column_name);
	NewAIKind6[enEnemyAttackTypeShort] = GetIntFromTable("i_notsamecross_far_notfaceplayer_and_attackshort",n_column,column_value,column_name);
	NewAIKind6[enEnemyAttackTypeLong] = GetIntFromTable("i_notsamecross_far_notfaceplayer_and_attacklong",n_column,column_value,column_name);
	NewMapAI[enEnemyCondition_NotSameCrosswiss_Far_NotFacePlayer] = NewAIKind6;

	g_pStaticDataManager->m_EnemyAIStaticInfoMap[nRoleID]=NewMapAI;

	return 0;
}

/************************************************************************/
/*					升级所需经验静态信息                                                                     */
/************************************************************************/
bool CNFStaticDataManager::GetLevelUpExpStaticInfo(const int nLevelID,tagLevelUpExpStaticInfo &Info)
{
	CNFLevelUpExpStaticInfoMap::iterator iFind = m_LevelUpexpStaticInfoMap.find(nLevelID);
	if (iFind==m_LevelUpexpStaticInfoMap.end())
		return false;

	Info=m_LevelUpexpStaticInfoMap[nLevelID];
	return true;
}

bool CNFStaticDataManager::ReadLevelUpExpStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_levelup_info",CNFStaticDataManager::LoadLevelUpExpStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadLevelUpExpStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadLevelUpExpStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nLevelID = GetIntFromTable("i_level_id",n_column,column_value,column_name);

	tagLevelUpExpStaticInfo NewLevelUpExpStaticInfo;
	NewLevelUpExpStaticInfo.fLevelUpExp = GetFloatFromTable("f_levelup_exp",n_column,column_value,column_name);
	
	g_pStaticDataManager->m_LevelUpexpStaticInfoMap[nLevelID]=NewLevelUpExpStaticInfo;

	return 0;
}


/************************************************************************/
/*					技能静态信息                                                                     */
/************************************************************************/
bool CNFStaticDataManager::GetSkillStaticInfo( const int nRoleID,const int nSkillID,tagSkillStaticInfo &Info )
{
	CNFSkillStaticInfoMapByProtagonistID::iterator iFind = m_SkillStaticInfoMap.find(nRoleID);
	if (iFind!=m_SkillStaticInfoMap.end())
	{
		CNFSkillStaticMapBySkillID::iterator it = iFind->second.find(nSkillID);
		if (it!=iFind->second.end())
		{
			Info=m_SkillStaticInfoMap[nRoleID][nSkillID];
			return true;
		}
	}

	return false;
}

bool CNFStaticDataManager::GetDesignSkillStaticInfoVec( const int nRoleID,const int nIsCommonSkill,CNFSkillStaticInfoVec &Info )
{
	CNFSkillStaticInfoMapByProtagonistID::iterator iFind = m_SkillStaticInfoMap.find(nRoleID);
	if (iFind!=m_SkillStaticInfoMap.end())
	{
		for (CNFSkillStaticMapBySkillID::iterator iter = iFind->second.begin();iter != iFind->second.end();iter++)
		{
			if (iter->second.nIsCommonSkill == nIsCommonSkill)
			{
				iter->second.nSkillID = iter->first;
				Info.push_back(iter->second);
			}
		}
	}

	return true;
}

bool CNFStaticDataManager::ReadSkillStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_skill_info",CNFStaticDataManager::LoadSkillStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadSkillStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadSkillStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nRoleID = GetIntFromTable("i_role_id",n_column,column_value,column_name);
	int nSkillID = GetIntFromTable("i_skill_id",n_column,column_value,column_name);

	tagSkillStaticInfo Info;
	Info.nSkillCD = GetIntFromTable("i_skill_cd",n_column,column_value,column_name);
	Info.nSkillCD_g = GetIntFromTable("i_skill_cd_g",n_column,column_value,column_name);
	Info.fCostMp = GetFloatFromTable("f_cost_mp",n_column,column_value,column_name);
	Info.fCostMp_g = GetFloatFromTable("f_cost_mp_g",n_column,column_value,column_name);
	Info.nIsCommonSkill = GetIntFromTable("i_is_common_skill",n_column,column_value,column_name);

	/************************************************************************/
	/*			技能块信息操作                                                                     */
	/************************************************************************/
	//1.得到Json的字符串
	string strSkillItem = GetStringFromTable("s_skill_item_vec",n_column,column_value,column_name);	
	//2.解析Json，得到Item信息
	CNFJsonTranslater::SharedData()->GetSkillItemInfoFromJson(strSkillItem.c_str(),Info.vecSkillItem,nRoleID,nSkillID);

	//排序：根据延迟时间，升序排列
	sort(Info.vecSkillItem.begin(),Info.vecSkillItem.end(),lessmark);
	
	for (CNFSkillItemVec::iterator iter = Info.vecSkillItem.begin();iter != Info.vecSkillItem.end();iter++)
	{
		iter->bIsCommonSkill = Info.nIsCommonSkill>0?true:false;
	}

	Info.strAniName = GetStringFromTable("s_ani_name",n_column,column_value,column_name);
	Info.strSkillName = GetStringFromTable("s_skill_name",n_column,column_value,column_name);
	Info.strSkillIntroduce = GetStringFromTable("s_skill_introduce",n_column,column_value,column_name);

	g_pStaticDataManager->m_SkillStaticInfoMap[nRoleID][nSkillID] = Info;

	////得到Json
	//char * szJson = NULL;
	//CNFJsonTranslater::SharedData()->GetJsonFromSkillItemInfo(szJson,ItemInfo);

	return 0;
}

//************************************************************************/
//*					敌人波次信息                                                    */
//************************************************************************/

bool CNFStaticDataManager::GetWaveStaticInfo( const int nStageID,const int nWaveID,CNFWaveItemInfoVec &Info )
{
	CNFStageWaveInfoMap::iterator iFind = m_StageWaveStaticInfoMap.find(nStageID);
	if (iFind!=m_StageWaveStaticInfoMap.end())
	{
		CNFWaveInfoMap::iterator it = iFind->second.find(nWaveID);
		if (it!=iFind->second.end())
		{
			Info = m_StageWaveStaticInfoMap[nStageID][nWaveID];
			return true;
		}
	}

	return false;
}

bool CNFStaticDataManager::ReadWaveStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_wave_info",CNFStaticDataManager::LoadWaveStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadWaveStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadWaveStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nStageID = GetIntFromTable("i_stage_id",n_column,column_value,column_name);
	int nWaveID = GetIntFromTable("i_wave_id",n_column,column_value,column_name);

	int nEnemyID = GetIntFromTable("i_enemy_id",n_column,column_value,column_name);
	int nEnemyLevel = GetIntFromTable("i_enemy_level",n_column,column_value,column_name);
	int nStartTime = GetIntFromTable("i_start_time",n_column,column_value,column_name);
	int nSpaceTime = GetIntFromTable("i_space_time",n_column,column_value,column_name);
	int nEnemyCount = GetIntFromTable("i_enemy_count",n_column,column_value,column_name);
	float fCreatePosX = GetIntFromTable("f_pos_x",n_column,column_value,column_name);
	float fCreatePosY = GetIntFromTable("f_pos_y",n_column,column_value,column_name);

	//遍历敌人个数，添加敌人
	for (int i=0;i<nEnemyCount;i++)
	{
		tagWaveItemInfo WaveItemInfo;
		WaveItemInfo.nEnemyID = nEnemyID;
		WaveItemInfo.nEnemyLevel = nEnemyLevel;
		WaveItemInfo.fCreatePosX = fCreatePosX;
		WaveItemInfo.fCreatePosY = fCreatePosY;
		WaveItemInfo.nStartTime = nStartTime + i*nSpaceTime;

		g_pStaticDataManager->m_StageWaveStaticInfoMap[nStageID][nWaveID].push_back(WaveItemInfo);
	}

	return 0;
}



/************************************************************************/
/*							临时：敌人技能骨骼动画                                                                     */
/************************************************************************/
bool CNFStaticDataManager::GetSkillArmatureInfo( const int nStageID,const int nWaveID,CNFWaveItemInfoVec &Info )
{
	return true;
}

bool CNFStaticDataManager::ReadSkillArmatureInfo()
{
	do 
	{
		//通过关卡ID，得到每一关的敌人ID容器（ID唯一）
		map<int,vector<int> >		mapEnemyByStageID;	

		//遍历波次数组，得到每一关敌人波次
		for (CNFStageWaveInfoMap::iterator iter = m_StageWaveStaticInfoMap.begin(); iter != m_StageWaveStaticInfoMap.end();iter++)
		{
			//关卡ID
			int nStageID = iter->first;
			
			//遍历每一管敌人，得到每一波敌人的信息
			for (CNFWaveInfoMap::iterator it = iter->second.begin();it != iter->second.end();it++)
			{   
				//遍历每一波敌人，得到每一个敌人信息
				for (CNFWaveItemInfoVec::iterator itEnemy = it->second.begin();itEnemy != it->second.end();itEnemy++)
				{
					bool	bIsHaveEnemy = false;
					//遍历关卡敌人ID容器，若无敌人ID，则添加；若有敌人ID，则跳过
					for (vector<int>::iterator itt = mapEnemyByStageID[nStageID].begin();itt != mapEnemyByStageID[nStageID].end();itt++)
					{
						if ( itEnemy->nEnemyID == *itt )
						{
							bIsHaveEnemy = true;
							break;
						}
					}

					//将敌人ID，见添加到敌人ID容器
					if (bIsHaveEnemy==false)
						mapEnemyByStageID[nStageID].push_back(itEnemy->nEnemyID);
				}
			}
		}

		return true;
	} while (false);
	CCLog("Fun CNFStaticDataManager::ReadSkillArmatureInfo Error!");
	return false;
}



/************************************************************************/
/*					装备静态信息                                                    */
/************************************************************************/
bool CNFStaticDataManager::GetEquipStaticInfo(const int nEquip,tagEquipStaticInfo &Info)
{
	CNFEquipStaticInfoMap::iterator iFind = m_EquipStaticInfoMap.find(nEquip);
	if (iFind==m_EquipStaticInfoMap.end())
		return false;

	Info=m_EquipStaticInfoMap[nEquip];
	return true;
}

bool CNFStaticDataManager::ReadEquipStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_equip_info",CNFStaticDataManager::LoadEquipStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadEquipStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadEquipStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nEquip = GetIntFromTable("i_equip_id",n_column,column_value,column_name);

	tagEquipStaticInfo Info;
	Info.strEquipName = GetStringFromTable("s_equip_name",n_column,column_value,column_name);
	Info.nEquipPositionID = GetIntFromTable("i_equip_position_id",n_column,column_value,column_name);
	Info.nEquipQuality = GetIntFromTable("i_equip_quality",n_column,column_value,column_name);
	Info.nEquipMustAID = GetIntFromTable("i_equip_must_a_id",n_column,column_value,column_name);
	Info.fEquipMustABasicPoint = GetIntFromTable("f_equip_must_a_basic_point",n_column,column_value,column_name);
	Info.fEquipMustAGroupPoint = GetIntFromTable("f_equip_must_a_group_point",n_column,column_value,column_name);

	g_pStaticDataManager->m_EquipStaticInfoMap[nEquip] = Info;

	return 0;
}


/************************************************************************/
/*					  NPC静态信息                                                                   */
/************************************************************************/
bool CNFStaticDataManager::GetNPCStaticInfo( const int nNPCID,tagNPCStaticInfo &Info )
{
	CNFNPCStaticInfoMap::iterator iFind = m_NPCStaticInfoMap.find(nNPCID);
	if (iFind==m_NPCStaticInfoMap.end())
		return false;

	Info=m_NPCStaticInfoMap[nNPCID];
	return true;
}
bool CNFStaticDataManager::ReadNPCStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_npc_info",CNFStaticDataManager::LoadNPCStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadNPCStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}
int CNFStaticDataManager::LoadNPCStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nNPCID = GetIntFromTable("i_npc_id",n_column,column_value,column_name);

	tagNPCStaticInfo Info;
	Info.strArmatureName = GetStringFromTable("s_armature_name",n_column,column_value,column_name);
	Info.fLength = GetFloatFromTable("f_length",n_column,column_value,column_name);
	Info.fWidth = GetFloatFromTable("f_width",n_column,column_value,column_name);
	Info.fHeight = GetFloatFromTable("f_height",n_column,column_value,column_name);

	g_pStaticDataManager->m_NPCStaticInfoMap[nNPCID] = Info;

	return 0;
}

/************************************************************************/
/*					  Door静态信息                                                                   */
/************************************************************************/
bool CNFStaticDataManager::GetDoorStaticInfo( const int nDoorID,tagDoorStaticInfo &Info )
{
	CNFDoorStaticInfoMap::iterator iFind = m_DoorStaticInfoMap.find(nDoorID);
	if (iFind==m_DoorStaticInfoMap.end())
		return false;

	Info=m_DoorStaticInfoMap[nDoorID];
	return true;
}

bool CNFStaticDataManager::ReadDoorStaticFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetStaticDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_door_info",CNFStaticDataManager::LoadDoorStaticCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFStaticDataManager::ReadDoorStaticFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}

int CNFStaticDataManager::LoadDoorStaticCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pStaticDataManager==NULL)
	{
		g_pStaticDataManager = new CNFStaticDataManager();
	} 

	int nDoorID = GetIntFromTable("i_door_id",n_column,column_value,column_name);

	tagDoorStaticInfo Info;
	Info.nRelaceStageID = GetIntFromTable("i_replace_stage_id",n_column,column_value,column_name);
	Info.strArmatureName = GetStringFromTable("s_armature_name",n_column,column_value,column_name);
	Info.fLength = GetFloatFromTable("f_length",n_column,column_value,column_name);
	Info.fWidth = GetFloatFromTable("f_width",n_column,column_value,column_name);
	Info.fHeight = GetFloatFromTable("f_height",n_column,column_value,column_name);

	g_pStaticDataManager->m_DoorStaticInfoMap[nDoorID] = Info;

	return 0;
}
























/************************************************************************/
/*					存储数据管理                                             */
/************************************************************************/
static CNFSaveDataManager * g_pSaveDataManager=NULL;
CNFSaveDataManager::CNFSaveDataManager()
{
}
CNFSaveDataManager * CNFSaveDataManager::SharedData()
{
	if(g_pSaveDataManager==NULL)
	{
		g_pSaveDataManager = new CNFSaveDataManager();
	}
	return g_pSaveDataManager;
}
void CNFSaveDataManager::ReleaseData()
{
}
void CNFSaveDataManager::initBasicData()
{
	do 
	{
		//读取用户信息
		ReadSaveUserFromDB();

		//读取服务器信息
		ReadSaveServerFromDB();

		return ;
	} while (false);
	CCLog("Fun CNFSaveDataManager::initBasicData Error!");
}

/************************************************************************/
/*						用户信息                                                                     */
/************************************************************************/
bool CNFSaveDataManager::SetSaveUserInfo(tagSaveUserInfo UserInfo)
{
	char *szErrMsg=NULL;			     
	do 
	{
		char szSQLCmd[NAME_LEN]={0};

		sqlite3 *pDB = NULL;			
		CC_BREAK_IF(sqlite3_open(GetSaveDataName().c_str(), &pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		sprintf(szSQLCmd,"update t_user_info set s_open_id='%s',s_username='%s',s_password='%s'",UserInfo.strOpenID.c_str(),UserInfo.strUserName.c_str(),UserInfo.strPassWord.c_str());
		CC_BREAK_IF(sqlite3_exec(pDB,szSQLCmd,NULL, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		g_pSaveDataManager->m_SaveUserInfo = UserInfo;

		return true;
	} while (false);
	CCLog("CNFSaveDataManager::SetSaveUserInfo Run Error! Msg=%s",szErrMsg);
	return false;
}
bool CNFSaveDataManager::GetSaveUserInfo( tagSaveUserInfo &UserInfo )
{
	UserInfo = m_SaveUserInfo;
	return true;
}
bool CNFSaveDataManager::ReadSaveUserFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetSaveDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_user_info",CNFSaveDataManager::LoadSaveUserCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFSaveDataManager::ReadSaveUserFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}
int CNFSaveDataManager::LoadSaveUserCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pSaveDataManager==NULL)
	{
		g_pSaveDataManager = new CNFSaveDataManager();
	} 

	tagSaveUserInfo Info;
	Info.strOpenID = GetStringFromTable("s_open_id",n_column,column_value,column_name);
	Info.strUserName = GetStringFromTable("s_username",n_column,column_value,column_name);
	Info.strPassWord = GetStringFromTable("s_password",n_column,column_value,column_name);

	g_pSaveDataManager->m_SaveUserInfo = Info;
	return 0;
}

/************************************************************************/
/*						服务器信息                                                                     */
/************************************************************************/
bool CNFSaveDataManager::SetSaveServerInfo(tagSaveServerInfo ServerInfo)
{
	char *szErrMsg=NULL;			    
	do 
	{
		char szSQLCmd[NAME_LEN]={0};

		sqlite3 *pDB = NULL;			
		CC_BREAK_IF(sqlite3_open(GetSaveDataName().c_str(), &pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		sprintf(szSQLCmd,"update t_server_info set i_server_id=%d,s_server_name='%s'",ServerInfo.nServerID,ServerInfo.strServerName.c_str());
		CC_BREAK_IF(sqlite3_exec(pDB,szSQLCmd,NULL, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		g_pSaveDataManager->m_SaveServerInfo = ServerInfo;

		return true;
	} while (false);
	CCLog("CNFSaveDataManager::SetSaveServerInfo Run Error! Msg=%s",szErrMsg);
	return false;
}
bool CNFSaveDataManager::GetSaveServerInfo(tagSaveServerInfo &ServerInfo)
{
	ServerInfo = m_SaveServerInfo;
	return true;
}
bool CNFSaveDataManager::ReadSaveServerFromDB()
{
	char *szErrMsg=NULL;
	do 
	{
		sqlite3 *pDB=NULL;
		CC_BREAK_IF(sqlite3_open(GetSaveDataName().c_str(),&pDB)!=SQLITE_OK);
		CC_BREAK_IF(pDB==NULL);

		CC_BREAK_IF(sqlite3_exec(pDB,"SELECT * from t_server_info",CNFSaveDataManager::LoadSaveServerCallBack, NULL, &szErrMsg )!=SQLITE_OK);
		sqlite3_close(pDB);

		return true;
	} while (false);
	CCLog("CNFSaveDataManager::ReadSaveServerFromDB Run Error! Msg=%s ",szErrMsg);
	return false;
}
int CNFSaveDataManager::LoadSaveServerCallBack( void * para, int n_column, char ** column_value, char ** column_name )
{
	if (g_pSaveDataManager==NULL)
	{
		g_pSaveDataManager = new CNFSaveDataManager();
	} 

	tagSaveServerInfo Info;
	Info.nServerID = GetIntFromTable("i_server_id",n_column,column_value,column_name);
	Info.strServerName = GetStringFromTable("s_server_name",n_column,column_value,column_name);

	g_pSaveDataManager->m_SaveServerInfo = Info;
	return 0;
}


