#ifndef _PUBLIC_DEF_HEAD_
#define _PUBLIC_DEF_HEAD_

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

#include <list>
#include <map>
#include <vector>
#include "SimpleAudioEngine.h"
#include <string>
#include <sstream>	//用来格式化字符串的头文件//

using namespace cocos2d;
using namespace std;
using namespace gui;



#define NF_PLATFORM					1	//当前版本（默认为普通版）

//版本列表
#define NF_PLATFORM_NORMAL			1	//normal
#define NF_PLATFORM_91				2	//91 playform
#define NF_PLATFORM_PP				3	//PP playform
#define NF_PLATFORM_UC_ANDROID		4	//UC playform
#define NF_PLATFORM_360_ANDROID		5	//360 playform
#define NF_PLATFORM_DK_ANDROID		6	//DK playform
#define NF_PLATFORM_MI_ANDROID		7	//MI playform
#define	NF_PLATFORM_WDJ_ANDROID		8   //WDJ playform
#define NF_PLATFORM_OPPO_ANDROID	9	//OPPO playform
#define NF_PLATFORM_WYX_ANDROID	   10	//WYX  playform


//静态数据管理
#define _TR_STATIC_DATABASE_FILE_NAME_		"nf_static.db"
std::string GetStaticDataName();

//存储数据管理
#define _NF_SAVE_DATABASE_FILE_NAME_		"nf_save.db"
std::string GetSaveDataName();

#define SCREEN_WIDTH		CCDirector::sharedDirector()->getWinSize().width	//屏幕宽(指定分辨率)
#define SCREEN_HEIGHT		480													//屏幕高(指定分辨率)
#define SCREEN_CENTER		ccp(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.5f)			//屏幕中心
#define NAME_LEN			128
#define _NF_ABS(x)				(    (x)>=0 ? (x) : -(x)   )

//非Win32平台的一些数据类型定义
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
typedef unsigned int UINT;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef unsigned char BYTE;
#endif


//消息接收
class CMsgReceiver
{
public:
	virtual void RecMsg(int nMsgID,void* pInfo,int nSize)
	{

	}
};

//消息发送
class CMsgSender
{
protected:
	CMsgReceiver * m_pReceiver;;

public:
	void SetMsg(CMsgReceiver * pReceiver)
	{
		m_pReceiver = pReceiver;
	}

	bool SendMsg(int nMsgID,void* pInfo,int nSize)
	{
		if (m_pReceiver == NULL)
		{
			return false;
		}
		else
		{
			m_pReceiver->RecMsg(nMsgID,pInfo,nSize);
			return true;
		}
	}
};

//z轴
enum
{
	enZOrderBack=100,
	enZOrderMid=100000,
	enZOrderFront=300000,
};

//玩家控制的角色
enum
{
	enTagProtagonist,
};


///=======================================================================
//消息标签
enum 
{
	enMsgCommonAttack,					//普通攻击
	enMsg_Hit_Enemy,					//击中敌人
	enMsg_RemoveEnemy,					//移除敌人
	enMsg_RemoveProtagonist,			//移除主角
	enMsgEnemyCommonAttack,				//敌人普通攻击
	enMsgProtagonistSkill,				//技能
	enMsgProtagonistTransfer,			//传送门
	enMsgCreateEnemy,					//创建敌人
	enMsgGameWin,						//游戏胜利
	enMsgCreateGameWinLayer,			//创建游戏胜利页
	enMsgCreateGameOverLayer,			//创建游戏失败页
};

//城镇消息标签
enum 
{
	enMsgCreateFbSelectLayer,			//创建副本选择页
};


//消息标签：根据服务器数据，进行操作
enum 
{
	enHttpMsg_CheckUserExist = 1,				//用户角色检测
	enHttpMsg_CreateHero,	
};


//敌人创建结构体
struct Cmd_CreateEnemyInfo
{
	int		nEnemyID;			//敌人ID
	int		nEnemyLevel;		//敌人等级
	float	fPosX;				//敌人创建x轴位置
	float	fPosY;				//敌人创建y轴位置
};


//
struct tagTransferDoor
{
	int nID;
	tagTransferDoor(){
		nID = 0;
	}
};

//移除人物的结构体
class CNFBasicPerson;
struct tagRemovePerson
{
	CCArray* pArrayTarget;		
	CNFBasicPerson* pPerson;
	tagRemovePerson(){
		pArrayTarget = NULL;
	}
};



//区分城镇和副本的ID范围		ID：0-10000为城镇，10001-19999为副本 ，20000以上为城镇副本
#define			_NF_TOWN_OR_BATTLE_ID_			10000
#define			_NF_TOWN_FB_ID_					20000



/************************************************************************/
/*					力量，智力与hp，mp的转化量                          */
/************************************************************************/
#define		_NF_POWER_HP_		16
#define		_NF_WIT_MP_			14
#define		_NF_ATTACK_UP_RATE_POWER_			14
#define		_NF_ATTACK_UP_RATE_WIT_				14
#define		_NF_ATTACK_UP_RATE_AGILITY_			14
#define		_NF_DOMINATION_UP_RATE_POWER_			20
#define		_NF_DOMINATION_UP_RATE_WIT_				20
#define		_NF_DOMINATION_UP_RATE_AGILITY_			20

#define		_NF_HIT_CD_			180

//毅力计算系数
#define		_NF_PERSEVERANCE_COEFFICIENT_			4285

//防御计算系数
#define		_NF_DEFENSE_COEFFICIENT_			2857



/************************************************************************/
/*						buff编号                                        */
/************************************************************************/
#define		_NF_BUFF_TREATFORMULA_ID			1	//治疗
#define		_NF_BUFF_SIPERARMOR_ID				2	//霸体
#define		_NF_BUFF_ASTHENIA_ID				3	//衰弱
#define		_NF_BUFF_ATTACKSPEEDCUT_ID			4	//缓慢
#define		_NF_BUFF_MOVESPEEDCUT_ID			5	//减速
#define		_NF_BUFF_POISONING_ID				6	//中毒
#define		_NF_BUFF_BFB_TREATFORMULA_ID		7	//百分比治疗
#define		_NF_BUFF_SINGLE_ATTACKUP_ID			8	//增加单体攻击力
#define		_NF_BUFF_ATTACKUP_ID				9	//增加群体攻击力
#define		_NF_BUFF_S_SIPERARMOR_ID			10	//群体霸体
#define		_NF_BUFF_S_MP_RECOVER_ID			11	//群体百分比恢复MP



//拳花类型
enum 
{
	enTagAttackType_Common = 1,						//普通
	enTagAttackType_Sharp = 2,						//锐利
	enTagAttackType_Fire = 3,						//火焰
	enTagAttackType_Electricity = 4,				//雷电

};

/************************************************************************/
/*                       技能摇杆                                       */
/************************************************************************/

//技能CD结构体
struct tagSkillCDInfo
{
	int			nSkill_SkillItemID;				//技能块ID
	int			nSkill_CD_Time;					//技能的CD时间
	int			nSkill_CD_Time_Temp;			//技能的当前CD时间
	string		strSkill_AniName;				//动画名
	float		fCostMp;						//消耗魔法值
};



/************************************************************************/
/*					UI                                                  */
/************************************************************************/
//屏幕适配，宽度差值
#define  _NF_SCREEN_WIDTH_DIS_	( CCDirector::sharedDirector()->getWinSize().width*0.5f - 360 )


//服务器选择结构体
struct tagServerSelect
{
	int nServerID;			//服务器ID
};

//消息发送：外部UI
enum enUI_Msg
{
	enMsg_ServerSelect,			//服务器选择
};


/************************************************************************/
/*				结构体                                                                     */
/************************************************************************/
//景深地图信息结构体
struct tagTmxStaticInfo
{
	string	strTmxName;			//景深地图名字
	float	fStartPosX;			//景深地图x轴初始位置
	float	fStartPosY;			//景深地图y轴初始位置
};
typedef map<int,tagTmxStaticInfo>	CNFTmxStaticInfoMap;	//景深地图静态信息map

//敌人AI
enum enumEnemyAIKind	//行动种类
{
	enEnemyAIHold,					//待机
	enEnemyAICrosswiseApproach,		//直线接近
	enEnemyAICrosswiseDisapproach,	//直线远离
	enEnemyLengthwaysApproach,		//并线接近
	enEnemyLengthwaysDisapproach,	//并线远离
	enEnemyDirectlyApproach,		//斜角靠近
	enEnemyCircuity,				//绕背
	enEnemyAttackTypeShort,			//攻击-近
	enEnemyAttackTypeLong,			//攻击-远
};
enum enumEnemyAICondition	//AI条件
{
	enEnemyCondition_SameCrosswise_InRangeLong,				//同线 且范围处于 远距离攻击 和近距离攻击范围之间
	enEnemyCondition_SameCrosswise_InRangeShort,			//同线 且范围处于 近距离攻击范围内
	enEnemyCondition_SameCrosswise_NotInRange,				//同线 且范围处于 远距离攻击 范围外
	enEnemyCondition_NotSameCrosswiss_Near,					//不同线 但距离很近
	enEnemyCondition_NotSameCrosswiss_Far_FacePlayer,		//不同线 距离远 面对玩家正面
	enEnemyCondition_NotSameCrosswiss_Far_NotFacePlayer,	//不同线 距离远 面对玩家背面
};

typedef map<enumEnemyAIKind,int> CMapAIActionOnKind;			//敌人行动类型map
typedef map<enumEnemyAICondition,CMapAIActionOnKind> CMapAI;	//敌人AI条件map
typedef map<int,CMapAI> CEnemyAIStaticInfoMap;						//通过ID，得到敌人AI信息

//buff(debuff)静态信息结构体
struct tagBuffStaticInfo
{
	int			nBuffID;		//buffID
	int			nBuffTime;		//buff持续时间
	int			nBuffTime_g;	//buff持续时间成长
	float		fBuffPoint1;	//buff点数1
	float		fBuffPoint1_g;	//buff点数1成长
	float		fBuffPoint2;	//buff点数2
	float		fBuffPoint2_g;	//buff点数2成长
};
typedef map<int,tagBuffStaticInfo> CNFBuffStaticInfoMap;	//buff静态信息map
typedef vector<tagBuffStaticInfo> CNFBuffStaticInfoVec;	//buff静态信息

//buff(debuff)信息结构体
struct tagBuffInfo
{
	int			nBuffID;		//buffID
	int			nBuffTime;		//buff持续时间
	float		fBuffPoint1;	//buff点数1
	float		fBuffPoint2;	//buff点数2
};
typedef vector<tagBuffInfo> CNFBuffInfoVec;	//buff信息


//技能块自身的变化信息
struct tagSkillChangeInfo
{
	float		fChangeTime;		//变化时间
	int 		nIsGravity;			//是否开启重力	1.开启	0.不开启

	float		fOldX;				//技能块，旧x轴位置
	float		fOldY;				//技能块，旧y轴位置
	float		fOldZ;				//技能块，旧z轴位置
	float		fOldLength;			//技能块，旧长度
	float		fOldWidth;			//技能块，旧宽度
	float		fOldHeight;			//技能块，旧高度

	float		fNewX;				//技能块，新x轴位置
	float		fNewY;				//技能块，新y轴位置
	float		fNewZ;				//技能块，新z轴位置
	float		fNewLength;			//技能块，新长度
	float		fNewWidth;			//技能块，新宽度
	float		fNewHeight;			//技能块，新高度


};

//技能块伤害信息
struct tagSkillDamageInfo
{
	CNFBuffStaticInfoVec	vecTargetBuff;				//被攻击者的buff或debuff
	float					fDamagePoint;				//基础攻击力
	float					fDamagePoint_g;				//攻击力系数
	float					fDamagePercent;				//技能百分比
	float					fDamagePercent_g;			//技能百分比成长
	float					fXDis;						//被攻击者x轴的变化量
	float					fXTime;						//被攻击者x轴的变化时间
	float					fYDis;						//被攻击者y轴的变化量
	float					fYTime;						//被攻击者y轴的变化时间
	float					fZSpeed;					//被攻击者z轴的速度变化
	int						nIsConversely;				//被攻击者是否倒地		1.倒地	0.不倒地
	int						nCrickAdd;					//被攻击者硬直增加量
	int						nAttackSpeed;				//攻击之间的间隔（帧）
	int						nIsExplodeMode;				//是否为爆炸模式		1.是	0.不是
	int						nIsPullMode;				//是否为拉近到面前模式	1.是	0.不是

	int						nCritAdd;					//暴击率增加量
	float					fCritDamageAdd;				//暴击伤害增加量
	int						nHitTargetAdd;				//命中增加量
	int						nIsDirection;				//是否有方向			1.有	0.没有

	int						nIsSuctionMode;				//是否为吸引模式		1.是	0.不是
	float					fSuctionSpeed;				//吸引的速度
	float					fSuctionTime;				//吸引的时间（秒）
	float					fSuctionPosX;				//吸引到的x轴位置：相对于角色，面部向右
	float					fSuctionPosY;				//吸引到的y轴位置

	int						nIsOnlyDamageNotHit;		//是否只受到伤害，但不播放被击动画（用于地面AOE）	1.是	0.不是
	int						nIsSuctionSkillItemCenter;	//是否吸引到技能块中心	1.是	0.不是			ps：若“是”，则fSuctionPosX和fSuctionPosY无意义。

	int						nIsDamage;					//是否计算伤害			1.是	0.不是
	int						nIsFollow;					//技能块是否跟随释放者	1.是	0.不是

	int						nIsBreakSiperarmor;			//是否破霸体
	int						nHitEffectType;				//拳花类型				

};

//技能Item结构体
struct tagSkillItemStaticInfo
{
	bool					bIsCommonSkill;			//是否为普通攻击
	float					fDelayTime;				//延迟时间
	float					fDisX;					//释放者，x轴位移量
	float					fTimeX;					//释放者，x轴时间
	float					fDisY;					//释放者，y轴位移量
	float					fTimeY;					//释放者，y轴时间
	float					fSpeedZ;				//释放者，z轴速度
	int						nScreenIsShake;			//屏幕是否震动	1.震动	0.不震动
	float					fShakeDurTime;			//屏幕震动持续时间（秒）
	float					fShakeDegree;			//屏幕震动幅度
	int						nShakeCount;			//屏幕震动次数
	int						nIsLensfeatrue;			//屏幕是否拉伸
	float					fLensfeatureTime;		//屏幕拉伸时间（秒）
	int						nIsScreenChangeColor;	//屏幕是否改变颜色	0.不改变	1.变黑
	float					fScreenChangeColorTime;	//屏幕颜色改变时间（秒）
	int						nIsTurnRound;			//释放者是否转头	1.是	0.不是

	CNFBuffStaticInfoVec	vecBuff;				//释放者的buff或debuff
	tagSkillChangeInfo		tSkillChange;			//技能块，自身变化信息
	tagSkillDamageInfo		tSkillDamage;			//技能块，伤害信息

	int						nIsArmature;			//技能块，是否有骨骼动画：1.有	0，没有
	string					strArmatureName;		//骨骼动画名称
	int						nIsSound;				//技能块，是否有音效：1.有	0，没有
	string					strSoundName;			//音效名称
};
typedef	vector<tagSkillItemStaticInfo> CNFSkillItemVec;		//技能块容器

//技能静态信息结构体
struct tagSkillStaticInfo
{
	int					nSkillID;					//技能ID
	int					nIsCommonSkill;				//是否为普通攻击：1.是	0.不是
	int					nSkillCD;					//技能CD（帧）
	int					nSkillCD_g;					//技能CD成长
	float				fCostMp;					//魔法消耗
	float				fCostMp_g;					//魔法消耗成长
	string				strAniName;					//动画名称
	string				strSkillName;				//技能名称
	string				strSkillIntroduce;			//技能介绍
	CNFSkillItemVec		vecSkillItem;				//技能块容器

};
typedef		map<int,tagSkillStaticInfo>				CNFSkillStaticMapBySkillID;				//通过技能ID，得到tagSkillStaticInfo
typedef		map<int,CNFSkillStaticMapBySkillID>		CNFSkillStaticInfoMapByProtagonistID;	//通过主角ID，得到CNFSkillStaticMapBySkillID
typedef		vector<tagSkillStaticInfo>				CNFSkillStaticInfoVec;					//技能静态信息vec


//攻击的结构体
struct tagTakeABeating
{
	CNFBuffInfoVec	vecBuff;				//目标的buff或debuff
	float	fDamagePoint;					//攻击力
	int		nHitTarget;						//命中
	int		nCrit;							//暴击
	float	fCritDamage;					//暴击伤害
	int		nXDis;							//被攻击者x轴的变化量
	float	fXTime;							//被攻击者x轴的变化时间
	int		nYDis;							//被攻击者y轴的变化量
	float	fYTime;							//被攻击者y轴的变化时间
	float	fZSpeed;						//被攻击者z轴的速度变化
	bool	bIsConversely;					//被攻击者是否倒地
	int		nCrickAdd;						//被攻击者硬直增加量
	int		nAttackSpeed;					//攻击之间的间隔（帧）
	bool	bIsExplodeMode;					//是否是爆炸模式
	bool	bIsPullMode;					//是否是拉倒面前模式
	bool	bIsSuctionMode;					//是否为吸引模式
	float	fSuctionSpeed;					//吸引的速度
	float	fSuctionTime;					//吸引的时间（秒）
	float	fSuctionPosX;					//吸引到的x轴位置：相对于角色，面部向右
	float	fSuctionPosY;					//吸引到的y轴位置
	bool	bIsOnlyDamageNotHit;			//是否只受到伤害，但不播放被击动画（用于地面AOE）
	bool	bIsSuctionSkillItemCenter;		//是否吸引到技能块中心			ps：若“是”，则fSuctionPosX和fSuctionPosY无意义。
	bool	bIsDamage;						//是否计算伤害
	bool	bIsFollow;						//是否跟随释放者
	bool	bIsBreakSiperarmor;				//是否破霸体
	int		nHitEffectType;					//拳花类型

	tagTakeABeating()
	{
		bIsExplodeMode = false;
		bIsPullMode = false;
		bIsDamage = true;
	}
};

//普通攻击结构体
class CNFBasicPerson;
struct tagCommonAttack
{
	CNFBasicPerson* pReleaser;	//释放者
	CCArray	*pArrayTarget;		//目标数组
	float	fChangeTime;		//变化时间
	int 	nIsGravity;			//是否开启重力	1.开启	0.不开启
	float	fOldX;				//技能块，旧x轴位置
	float	fOldY;				//技能块，旧y轴位置
	float	fOldZ;				//技能块，旧z轴位置
	float	fOldLength;			//技能块，旧长度
	float	fOldWidth;			//技能块，旧宽度
	float	fOldHeight;			//技能块，旧高度

	float	fNewX;				//技能块，新x轴位置
	float	fNewY;				//技能块，新y轴位置
	float	fNewZ;				//技能块，新z轴位置
	float	fNewLength;			//技能块，新长度
	float	fNewWidth;			//技能块，新宽度
	float	fNewHeight;			//技能块，新高度

	int		nIsArmature;		//技能块，是否有骨骼动画：1.有	0，没有
	string	strArmatureName;	//骨骼动画名称

	tagTakeABeating tTakeABeating;
	tagCommonAttack(CNFBasicPerson* pReleaserU,tagSkillItemStaticInfo* pInfo,int nSkillLV);
	tagCommonAttack(){
	}
};




/************************************************************************/
/*					主角AI                                              */
/************************************************************************/
//治疗技能
struct tagTreatSkillInfo 
{
	int			nSkillID;				//技能ID
	float		fBloodPer;				//触发加血技能的hp最低百分比
};
typedef	vector<tagTreatSkillInfo>		CNFTreatSkillVec;		//治疗技能容器

//必然技能
struct tagMustSkillInfo
{
	int			nSkillID;				//技能ID
};
typedef	vector<tagMustSkillInfo>		CNFMustSkillVec;		//必然技能容器

//距离技能块
struct tagDistanceItem
{
	int			nSkillID;				//技能ID
	int			nDelayTime;				//延迟时间（帧），总时间
};
typedef	vector<tagDistanceItem>			CNFDistanceItemVec;		//距离技能块容器

//距离技能
struct tagDistanceSkillInfo
{
	float					fSkillDistance;			//释放技能的距离范围
	CNFDistanceItemVec		vecDistanceItem;		//距离技能块容器
};
typedef	vector<tagDistanceSkillInfo>	CNFDistanceSkillVec;	//距离技能容器


//主角Ai结构体
struct tagProtagonistAIInfo
{
	CNFTreatSkillVec		vecTreatSkill;			//治疗技能
	CNFMustSkillVec			vecMustSkill;			//必然技能
	CNFDistanceSkillVec		vecDistanceSkill;		//距离技能
};
//typedef map<int,tagProtagonistAIInfo>	CNFProtagonistAIMap;	//主句ai容器


/************************************************************************/
/*				       NPC                                                              */
/************************************************************************/
//ncp静态信息结构体
struct tagNPCStaticInfo
{
	string			strArmatureName;		//NPC 动画名称
	float			fLength;				//NPC 长度
	float			fWidth;					//NPC 宽度
	float			fHeight;				//NPC 高度
};
typedef map<int,tagNPCStaticInfo>	CNFNPCStaticInfoMap;		//NPC静态信息容器

//npc Json信息
struct tagNPCJsonInfo
{
	int					nNPCID;					//NPC ID
	float				fPosX;					//NPC x轴位置
	float				fPosY;					//NPC y轴位置
	float				fPosZ;					//NPC z轴位置
	tagNPCStaticInfo	NPCStaticInfo;			//NPC 静态信息
};
typedef vector<tagNPCJsonInfo>	CNFNPCJsonInfoVec;		//NPC Json 信息容器



/************************************************************************/
/*					DOOR                                                                     */
/************************************************************************/
//door静态信息结构体
struct tagDoorStaticInfo
{
	string			strArmatureName;		//Door 动画名称
	float			fLength;				//Door 长度
	float			fWidth;					//Door 宽度
	float			fHeight;				//Door 高度
	int				nRelaceStageID;			//Door 跳转到的关卡ID
};
typedef map<int,tagDoorStaticInfo>	CNFDoorStaticInfoMap;		//Door静态信息容器

//door Json信息
struct tagDoorJsonInfo
{
	int					nDoorID;				//Door ID
	float				fPosX;					//Door x轴位置
	float				fPosY;					//Door y轴位置
	float				fPosZ;					//Door z轴位置
	tagDoorStaticInfo	DoorStaticInfo;			//Door 静态信息
};
typedef vector<tagDoorJsonInfo>	CNFDoorJsonInfoVec;		//Door Json 信息容器


/************************************************************************/
/*						http消息发送结构体                                                                     */
/************************************************************************/
//注册
struct THttpRegister 
{
	string			strUserName;		//用户名称
	string			strPassWord;		//用户密码
};

//登录
struct THttpLogin
{
	string			strUserName;		//用户名称
	string			strPassWord;		//用户密码
};

//创建角色
struct TTHttpCreateHero
{
	int				nHeroID;			//角色ID
	string			strHeroName;		//角色名称
};




#endif