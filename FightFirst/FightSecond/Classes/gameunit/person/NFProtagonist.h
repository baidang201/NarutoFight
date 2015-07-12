#ifndef _NF_POWER1_HEAD_
#define _NF_POWER1_HEAD_

#include "NFBasicPerson.h"

//普通攻击的状态标签
enum enCommonAttackStatus
{
	enStatusCommonAttack_1 = 1,
	enStatusCommonAttack_2 = 2,
	enStatusCommonAttack_3 = 3,
	enStatusCommonAttack_4 = 4,
	enStatusCommonAttack_NO = 0,
};

//角色类型
enum enRoleType
{
	enRoleType_Power = 1,	//力量
	enRoleType_Wit = 2,		//智力
	enRoleType_Agility = 3,	//敏捷
};

//
class CNFProtagonist :public CNFBasicPerson,public CMsgSender{
private:
	float		m_fJumpPower;						//跳跃力
	enCommonAttackStatus m_enCommonAttackStatus;	//普通攻击的状态标签

	int			m_nRoleID;							//角色ID
	//int			m_nRoleType;					//角色类型

	int			m_nCommonAttackCDTemp;	
	int			m_nCommonAttackCD;					//普通连续攻击的cd

	int			m_nMoveTimeTemp;

	bool		m_bIsAttacking;						//是否处于攻击状态中
	bool		m_bIsMoveAnimation;					//是否播移动的Animation
	bool		m_bIsRunAnimation;					//是否播奔跑的Animation
	bool		m_bIsHoldAnimation;					//是否播待机的Animation
	int			m_nCommonAttackNum;					//普通攻击次数

	int			m_nCommonAttack_1_SkillItemID;		//技能块ID
	int			m_nCommonAttack_2_SkillItemID;		//技能块ID
	int			m_nCommonAttack_3_SkillItemID;		//技能块ID
	int			m_nCommonAttack_4_SkillItemID;		//技能块ID
	
	tagSkillStaticInfo m_StaticSkillInfo;			//技能信息
	CNFSkillItemVec::iterator m_SkillItem;			//技能快的迭代器

	tagSkillCDInfo m_Skill_1_SkillItem;				//技能块
	tagSkillCDInfo m_Skill_2_SkillItem;				//技能块
	tagSkillCDInfo m_Skill_3_SkillItem;				//技能块
	tagSkillCDInfo m_Skill_4_SkillItem;				//技能块
	tagSkillCDInfo m_Skill_5_SkillItem;				//技能块

	CCArray		*m_pArrayFriend;					//友军数组
	CCArray		*m_pArrayTarget;					//目标数组

	//=======================================AI
	bool		m_bIsAutoMode;						//是否开启自动
	bool		m_bIsAutoAttacking;					//是否正在执行自动技能中
	bool		m_bIsAutoListAttacking;				//是否正在执行自动序列技能中
	int			m_nAutoListTime;					//序列时间
	CNFDistanceSkillVec::iterator m_AutoListAttack;	//序列技能

	tagProtagonistAIInfo m_tagProtagonistAIInfo;


public:
	//create
	static CNFProtagonist* Create(int nID,CCArray *pArrayFriend,CCArray *pArrayTarget);
	//更新函数
	virtual void CallPerFrame(float fT);
	//通过角度移动
	void  OnCtrlMoveByRotation(float fRotation);
	void  OnCtrlRunByRotation(float fRotation);
	void  OnCtrlStop();
	//移动到指定的地面上的点（城镇用）
	void OnCtrlMoveToPonit(CCPoint ptPos);
	//跳跃
	void ProtagonistJump();
	//获取当前hp百分比
	float GetHPPercent();
	//获取当前hp2百分比
	float GetHP2Percent();
	//获取当前mp百分比
	float GetMPPercent();
	//得到角色ID
	int GetRoleID(){return m_nRoleID;};
	
	//伤害
	virtual float HarmFormula(tagTakeABeating tTakeABeating);
	//治疗
	virtual float TreatFormula(float fTreatPoint);
	//百分比治疗
	virtual float BFBTreatFormula(float fBFBTreatPoint);
	//百分比恢复MP
	virtual float BFBRecoverMP(float fBFBMPPoint);
	//普通攻击
	void OnCtrlCommonAttack();
	
	//临时测试用
	void OnCtrlTemp();
	//技能
	bool OnCtrlSkill(int nID);

	//得到技能1信息
	tagSkillCDInfo GetSkillInfo_1(){return m_Skill_1_SkillItem;}

	//得到技能2信息
	tagSkillCDInfo GetSkillInfo_2(){return m_Skill_2_SkillItem;}

	//得到技能3信息
	tagSkillCDInfo GetSkillInfo_3(){return m_Skill_3_SkillItem;}

	//得到技能4信息
	tagSkillCDInfo GetSkillInfo_4(){return m_Skill_4_SkillItem;}

	//得到技能5信息
	tagSkillCDInfo GetSkillInfo_5(){return m_Skill_5_SkillItem;}

	//得到普通攻击状态
	int	GetCommonAttackStatus(){return m_enCommonAttackStatus;}
	
	//得到普通攻击CD百分比
	float GetCommonAttackCDBFB(){return m_nCommonAttackCDTemp/(float)m_nCommonAttackCD;}

	//是否开启自动
	void SetAutoMode(bool bIsAutoMode){m_bIsAutoMode = bIsAutoMode;}

protected:
	//init
	bool Init(int nID,CCArray *pArrayFriend,CCArray *pArrayTarget);
	//设置死亡
	void OnDead();
	//onExit
	virtual void onExit();
private:
	//设置为hold
	void OnHold();
	//技能回调
	void OnCtrlSkillCallBack();
	//CCArmature的回调函数
	void OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID );

	//判断是否携带有此技能ID的技能并且是否没有冷却中
	bool IsHaveSkillIDAndCD(int nSkillID);

	//
	void AutoAttackOverCallBack();
};
#endif