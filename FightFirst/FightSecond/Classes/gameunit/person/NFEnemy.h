#ifndef _NF_ENMEY_HEAD_
#define _NF_ENMEY_HEAD_

#include "NFBasicPerson.h"
#include <map>
/************************************************************************/
/*			敌人类：用来创建敌人										*/
/************************************************************************/

class CNFEnemy :public CNFBasicPerson,public CMsgSender
{
private:
	CMapAI				m_MapAI;						//敌人AI

	float				m_AINearDis;					//AI近距离
	float				m_AIFarDis;						//AI远距离
	
	CNFBasicPerson*		m_pTarget;						//目标
	float				m_fJumpPower;					//跳跃力
	bool				m_bIsJump;						//是否处于跳跃状态
	
	int					m_nTurnRoundTime;				//转身时间
	int					m_nTurnRoundTimeTemp;	

	int					m_nMoveTimeTemp;

	bool				m_bIsActionRuning;				//是否处于攻击状态中
	bool				m_bIsHoldAnimation;				//是否播待机的Animation

	bool				m_bIsDeadAnimation;				//是否播过死亡的Animation

	int					m_nEnemyID;						//角色ID

	CCArray				*m_pArrayFriend;				//友军数组
	CCArray				*m_pArrayTarget;				//目标数组

	tagSkillCDInfo		m_Skill_1_SkillItem;			//技能块
	tagSkillCDInfo		m_Skill_2_SkillItem;			//技能块
	tagSkillCDInfo		m_Skill_3_SkillItem;			//技能块
	tagSkillCDInfo		m_Skill_4_SkillItem;			//技能块

	tagSkillStaticInfo m_StaticSkillInfo;				//技能信息
	CNFSkillItemVec::iterator m_SkillItem;				//技能快的迭代器

	//临时	
	float				m_fNowHPProgress2;

public:
	//create
	static CNFEnemy* CreateEnemy(int nEnemyID,int nEnemyLevel,CCArray *pArrayFriend,CCArray *pArrayTarget);
	//更新函数
	virtual void CallPerFrame(float fT);
	//AI函数:判断当前AI条件
	enumEnemyAICondition AI_getCurrnetCondition();
	//AI函数:当前行动种类
	enumEnemyAIKind AI_getCurrentAICmdKind(enumEnemyAICondition enCondition);

	//获取当前hp百分比
	float GetHPPercent();
	//获取当前hp2百分比
	float GetHP2Percent();
	
	//伤害
	virtual float HarmFormula(tagTakeABeating tTakeABeating);
	//治疗
	virtual float TreatFormula(float fTreatPoint);
	//百分比治疗
	virtual float BFBTreatFormula(float fBFBTreatPoint);
	//百分比恢复MP
	virtual float BFBRecoverMP(float fBFBMPPoint);
protected:
	//待机
	bool OnCtrlHold();
	//直线接近/远离
	bool OnCtrlCrosswiseApproach(bool bIsApproach);
	//并线接近/远离
	bool OnCtrlLengthwaysApproach(bool bIsApproach);
	//斜角靠近
	bool OnCtrlApproach();
	//绕背
	bool OnCtrlCircuity();
	//攻击1
	bool OnCtrlAttack1();
	//攻击2
	bool OnCtrlAttack2();

	//攻击3
	bool OnCtrlAttack3();
	//攻击4
	bool OnCtrlAttack4();

	//初始化。pProtagonist：主角指针
	bool InitEnemy(int nEnemyID,int nEnemyLevel,CCArray *pArrayFriend,CCArray *pArrayTarget);
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
	//判断是否没有冷却中
	bool IsSkillNOCD(int nSkillID);
	//判断技能ID的技能是否在攻击范围之内
	bool IsSkillAttactkable(int nSkillID);
protected:
	//标签
	enum 
	{
		enTagHPEnemyProgress1,
		enTagHPEnemyProgress2,
		enTagHPEnemyProgressBg,
	};

};
#endif