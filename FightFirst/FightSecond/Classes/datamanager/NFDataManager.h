
//  http://www.9miao.com

#ifndef _NF_DATA_MANAGER_HEAD_
#define _NF_DATA_MANAGER_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*				数据库校验                                                                     */
/************************************************************************/
//校验int型
int GetIntFromTable(char * szItemName, int n_column, char ** column_value, char ** column_name);
//校验float型
float GetFloatFromTable(char * szItemName, int n_column, char ** column_value, char ** column_name);
//校验string型
string GetStringFromTable(char * szItemName, int n_column, char ** column_value, char ** column_name);

/************************************************************************/
/*					静态数据结构体                                                                     */
/************************************************************************/
//关卡静态信息结构体
struct tagStageStaticInfo 
{
	float 					fRate;					//世界倾斜角度（y轴）
	float					fGravity;				//重力（z轴减速度）
	float					fLength;				//长（3D世界）
	float					fWidth;					//宽（3D世界）
	float					fHeight;				//高（3D世界）
	float					fProtagonistX;			//主角x初始轴坐标
	float					fProtagonistY;			//主角y初始轴坐标
	float					fProtagonistZ;			//主角z初始轴坐标
	string					strTMXWorldName;		//景深地图世界的名字
	CNFTmxStaticInfoMap		TmxStaticInfoMap;		//景深地图map
	CNFNPCJsonInfoVec		NPCJsonInfoVec;			//NPC JSON 信息
	CNFDoorJsonInfoVec		DoorJsonInfoVec;		//Door Json 信息
};
typedef map<int,tagStageStaticInfo> CNFStageStaticInfoMap;		//关卡静态信息map，通过关卡ID，获得关卡信息


//主角静态信息结构体
struct tagProtagonistStaticInfo
{
	float			fHP;						//生命值
	float			fHp_g;						//生命值成长
	float			fMP;						//能量值
	float			fMp_g;						//能量值成长
	float			fDefense;					//防御力
	float			fDefense_g;					//防御力成长
	float			fAttack;					//攻击力
	float			fAttack_g;					//攻击力成长
	float			fDomination;				//掌控力
	float			fMoveSpeed;					//移动速度
	float			fAttackSpeed;				//攻击速度
	int				nCrit;						//暴击率
	float			fCritDamage;				//暴击伤害
	int				nCrickTime;					//硬直时间
	int				nConverselyTime;			//起身时间
	int				nHitTarget;					//命中
	int				nAvoid;						//回避
	float			fTreatCoefficient;			//治疗系数

	//hold状态的3D体积（长，宽，高）
	float			fLength_Hold;				//长
	float			fWidth_Hold;				//宽
	float			fHeight_Hold;				//高

	//move状态的3D体积
	float			fLength_Move;				//长
	float			fWidth_Move;				//宽
	float			fHeight_Move;				//高

	//conversely状态的3D体积
	float			fLength_Conversely;			//长
	float			fWidth_Conversely;			//宽
	float			fHeight_Conversely;			//高

	tagProtagonistAIInfo	tProtagonistAIInfo;	//主角ai
	
	string			strBeHitSoundName;			//被击音效名称
	string			strDeadSoundName;			//死亡音效名称

	string			strArmatureName;			//主角骨骼动画的名称
	int				nCommonAttackNum;			//普通攻击段数
};
typedef map<int,tagProtagonistStaticInfo> CNFProtagonistStaticInfoMap;		//主角静态信息map。通过主角ID，获得主角信息


//敌人静态信息结构体
struct tagEnemyStaticInfo 
{
	float			fHP;						//生命值
	float			fHp_g;						//生命值成长
	float			fMP;						//能量值
	float			fMp_g;						//能量值成长
	float			fDefense;					//防御力
	float			fDefense_g;					//防御力成长
	float			fAttack;					//攻击力
	float			fAttack_g;					//攻击力成长
	float			fDomination;				//掌控力
	float			fMoveSpeed;					//移动速度
	float			fAttackSpeed;				//攻击速度
	int				nCrit;						//暴击率
	float			fCritDamage;				//暴击伤害
	int				nCrickTime;					//硬直时间
	int				nConverselyTime;			//起身时间
	int				nTurnRoundTime;				//转身CD时间（帧）
	int				nHitTarget;					//命中
	int				nAvoid;						//回避
	float			fTreatCoefficient;			//治疗系数

	//hold状态
	float			fLength_Hold;				//长
	float			fWidth_Hold;				//宽
	float			fHeight_Hold;				//高

	//move状态
	float			fLength_Move;				//长
	float			fWidth_Move;				//宽
	float			fHeight_Move;				//高

	//conversely状态
	float			fLength_Conversely;			//长
	float			fWidth_Conversely;			//宽
	float			fHeight_Conversely;			//高

	float			fNearDis;					//AI近距离
	float			fFarDis;					//AI远距离

	string			strArmatureName;			//敌人骨骼动画名称
};
typedef map<int,tagEnemyStaticInfo> CNFEnemyStaticInfoMap;		//敌人静态信息map。通过敌人ID，获得敌人信息


//物品静态信息结构体
struct tagArticleStaticInfo
{
	float			fHp;					//物品生命值
	int				nIsInvincible;			//物品是否无敌：0.不无敌	1.无敌

	float			fLength_Hold;			//长
	float			fWidth_Hold;			//宽
	float			fHeight_Hold;			//高
};
typedef map<int,tagArticleStaticInfo> CNFArticleStaticInfoMap;		//物品静态信息map。通过物品ID，获得物品信息


//升级经验静态信息结构体
struct tagLevelUpExpStaticInfo
{
	float		fLevelUpExp;		//升级所需经验
};
typedef map<int,tagLevelUpExpStaticInfo> CNFLevelUpExpStaticInfoMap;	//升级经验静态信息map。通过等级ID，获得升级经验


//波次信息Item结构体，一个敌人
struct tagWaveItemInfo
{
	int				nEnemyID;				//怪物ID
	int				nEnemyLevel;			//怪物等级
	int				nStartTime;				//怪物创建时间
	float			fCreatePosX;			//怪物x轴起始位置（相对于3D世界的位置）			
	float			fCreatePosY;			//怪物y轴起始位置（相对于3D世界的位置）
};
typedef vector<tagWaveItemInfo>					CNFWaveItemInfoVec;				//波次Item信息vec：一波敌人
typedef map<int,CNFWaveItemInfoVec>				CNFWaveInfoMap;					//波次信息map：一关敌人。通过波次ID，获得一波敌人信息
typedef map<int,CNFWaveInfoMap>					CNFStageWaveInfoMap;			//关卡波次信息map：全部关卡敌人。通过关卡ID，获得一关敌人信息


//临时：技能骨骼动画信息结构体，用来得到当前关卡中，用到的所有技能骨骼动画的信息
struct	tagSkillArmatureInfo
{
	string		strArmatureName;		//骨骼动画名称
};
typedef vector<tagSkillArmatureInfo>		CNFSkillArmatureInfoVec;			//技能骨骼动画信息容器
typedef	map<int , CNFSkillArmatureInfoVec>	CNFSkillArmatureInfoByStageID;		//通过关卡，获得本关卡用，到的所有技能骨骼动画的信息


//装备静态信息结构体
struct tagEquipStaticInfo
{
	string		strEquipName;				//装备名称				“防御 优良 胸甲”
	int			nEquipPositionID;			//装备部位ID			1.头  2.颈部  3.胸部  4.手部  5.腿部  6.脚部  7.腰部  8.武器  9.戒指  10.饰品
	int			nEquipQuality;				//装备品质				1.绿  2.蓝    3.紫    4.粉    5.金
	int			nEquipMustAID;				//装备必有属性ID		1.生命回复（百分比）  2.查克拉上限  3.防御力  4.命中  5.闪避  6.移动速度  7.生命上限  8.攻击力  9.查克拉上限（百分比）  10.攻击速度  11.暴击  12.精通  13.默契  14.毅力  15.暴击伤害
	float		fEquipMustABasicPoint;		//装备必有属性基础值
	float		fEquipMustAGroupPoint;		//装备必有属性成长值
};
typedef	map<int , tagEquipStaticInfo>	CNFEquipStaticInfoMap;				//装备静态信息map。通过装备ID，得到装备信息。



/************************************************************************/
/*						静态数据库类                                                                     */
/************************************************************************/
class CNFStaticDataManager
{
	CNFStaticDataManager();

protected:
	CNFStageStaticInfoMap					m_StageStaticInfoMap;			//关卡静态信息
	CNFProtagonistStaticInfoMap				m_ProtagonistStaticInfoMap;		//主角静态信息
	CNFEnemyStaticInfoMap					m_EnemyStaticInfoMap;			//敌人静态信息
	CNFArticleStaticInfoMap					m_ArticleStaticInfoMap;			//物品静态信息
	CEnemyAIStaticInfoMap					m_EnemyAIStaticInfoMap;			//敌人AI静态信息
	CNFLevelUpExpStaticInfoMap				m_LevelUpexpStaticInfoMap;		//升级所需经验静态信息
	CNFSkillStaticInfoMapByProtagonistID	m_SkillStaticInfoMap;			//技能静态信息，
	CNFStageWaveInfoMap						m_StageWaveStaticInfoMap;		//波次静态信息
	CNFSkillArmatureInfoVec					m_SkillArmatureInfoVec;			//临时：技能骨骼动画信息
	CNFEquipStaticInfoMap					m_EquipStaticInfoMap;			//装备静态信息
	CNFNPCStaticInfoMap						m_NPCStaticInfoMap;				//NPC静态信息
	CNFDoorStaticInfoMap					m_DoorStaticInfoMap;			//Door静态信息

public:
	static CNFStaticDataManager *SharedData();
	void initBasicData();			//初始化数据
	static void ReleaseData();		//清空数据

public:
	//获得关卡静态信息（根据关卡ID）
	bool GetStageStaticInfo(const int nStageID,tagStageStaticInfo &Info);
	//读取关卡静态信息
	bool ReadStageStaticFromDB();
	static int LoadStageStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//获得主角静态信息（根据主角ID）
	bool GetProtagonistStaticInfo(const int nProtagonistID,tagProtagonistStaticInfo &Info);
	//读取主角静态信息
	bool ReadProtagonistStaticFromDB();
	static int LoadProtagonistStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//获得敌人静态信息（根据角色ID）
	bool GetEnemyStaticInfo(const int nRoleID,tagEnemyStaticInfo &Info);
	//读取敌人静态信息
	bool ReadEnemyStaticFromDB();
	static int LoadEnemyStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//获得物品信息（根据物品ID）
	bool GetArticleStaticInfo(const int nArticleID,tagArticleStaticInfo &Info);
	//读取物品静态信息
	bool ReadArticleStaticFromDB();
	static int LoadArticleStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//获得敌人AI信息（根据角色ID）
	bool GetEnemyAIStaticInfo(const int nRoleID,CMapAI &Info);
	//获得敌人AI静态信息
	bool ReadEnemyAIStaticFromDB();
	static int LoadEnemyAIStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//获得升级所需经验信息（根据等级ID）
	bool GetLevelUpExpStaticInfo(const int nLevelID,tagLevelUpExpStaticInfo &Info);
	//读取升级所需经验静态信息
	bool ReadLevelUpExpStaticFromDB();
	static int LoadLevelUpExpStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//获得技能静态信息（根据角色ID，技能ID）
	bool GetSkillStaticInfo(const int nRoleID,const int nSkillID,tagSkillStaticInfo &Info);
	//读取技能静态信息
	bool ReadSkillStaticFromDB();
	static int LoadSkillStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);
	
	//获得指定的技能静态信息容器：nRoleID为角色ID。nIsCommonSkill为0时，返回所有技能；nIsCommonSkill为1时，返回所有普通攻击。
	bool GetDesignSkillStaticInfoVec(const int nRoleID,const int nIsCommonSkill,CNFSkillStaticInfoVec &Info);

	//根据关卡ID，波次ID获得波次信息（一波敌人）
	bool GetWaveStaticInfo(const int nStageID,const int nWaveID,CNFWaveItemInfoVec &Info);
	//读取敌人波次静态信息
	bool ReadWaveStaticFromDB();
	static int LoadWaveStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);

	//获得装备静态信息（根据装备ID）
	bool GetEquipStaticInfo(const int nEquip,tagEquipStaticInfo &Info);
	//读取装备静态信息
	bool ReadEquipStaticFromDB();
	static int LoadEquipStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);

	//获得npc静态信息（根据npcID）
	bool GetNPCStaticInfo(const int nNPCID,tagNPCStaticInfo &Info);
	//读取ncp静态信息
	bool ReadNPCStaticFromDB();
	static int LoadNPCStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);

	//获得door静态信息（根据Door ID）
	bool GetDoorStaticInfo(const int nDoorID,tagDoorStaticInfo &Info);
	//读取door静态信息
	bool ReadDoorStaticFromDB();
	static int LoadDoorStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//临时：得到本关，所用到的，所有技能的，骨骼动画名称
	bool GetSkillArmatureInfo(const int nStageID,const int nWaveID,CNFWaveItemInfoVec &Info);
	//临时：读取敌人技能骨骼动画信息
	bool ReadSkillArmatureInfo();
};

















/************************************************************************/
/*					存储数据结构体                                                       */
/************************************************************************/
//用户姓名和密码
struct tagSaveUserInfo 
{
	string		strOpenID;					//用户ID，唯一标识
	string		strUserName;				//用户姓名
	string		strPassWord;				//用户密码
};

//服务器信息
struct tagSaveServerInfo
{
	int			nServerID;					//服务器ID
	string		strServerName;				//服务器名称
};

/************************************************************************/
/*					存储数据管理类                                                                   */
/************************************************************************/
class CNFSaveDataManager
{
	CNFSaveDataManager();

protected:
	tagSaveUserInfo				m_SaveUserInfo;				//用户信息
	tagSaveServerInfo			m_SaveServerInfo;			//服务器信息

public:
	static CNFSaveDataManager *SharedData();
	void initBasicData();			
	static void ReleaseData();		

public:
	//设置本地用户信息
	bool SetSaveUserInfo(tagSaveUserInfo UserInfo);
	//获得本地用户信息
	bool GetSaveUserInfo(tagSaveUserInfo &UserInfo);
	//读取本地用户信息
	bool ReadSaveUserFromDB();
	static int LoadSaveUserCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//设置服务器信息
	bool SetSaveServerInfo(tagSaveServerInfo ServerInfo);
	//获得服务器信息
	bool GetSaveServerInfo(tagSaveServerInfo &ServerInfo);
	//读取服务器信息
	bool ReadSaveServerFromDB();
	static int LoadSaveServerCallBack(void * para, int n_column, char ** column_value, char ** column_name);


};











#endif