#ifndef _NF_POWER1_HEAD_
#define _NF_POWER1_HEAD_

#include "NFBasicPerson.h"

//��ͨ������״̬��ǩ
enum enCommonAttackStatus
{
	enStatusCommonAttack_1 = 1,
	enStatusCommonAttack_2 = 2,
	enStatusCommonAttack_3 = 3,
	enStatusCommonAttack_4 = 4,
	enStatusCommonAttack_NO = 0,
};

//��ɫ����
enum enRoleType
{
	enRoleType_Power = 1,	//����
	enRoleType_Wit = 2,		//����
	enRoleType_Agility = 3,	//����
};

//
class CNFProtagonist :public CNFBasicPerson,public CMsgSender{
private:
	float		m_fJumpPower;						//��Ծ��
	enCommonAttackStatus m_enCommonAttackStatus;	//��ͨ������״̬��ǩ

	int			m_nRoleID;							//��ɫID
	//int			m_nRoleType;					//��ɫ����

	int			m_nCommonAttackCDTemp;	
	int			m_nCommonAttackCD;					//��ͨ����������cd

	int			m_nMoveTimeTemp;

	bool		m_bIsAttacking;						//�Ƿ��ڹ���״̬��
	bool		m_bIsMoveAnimation;					//�Ƿ��ƶ���Animation
	bool		m_bIsRunAnimation;					//�Ƿ񲥱��ܵ�Animation
	bool		m_bIsHoldAnimation;					//�Ƿ񲥴�����Animation
	int			m_nCommonAttackNum;					//��ͨ��������

	int			m_nCommonAttack_1_SkillItemID;		//���ܿ�ID
	int			m_nCommonAttack_2_SkillItemID;		//���ܿ�ID
	int			m_nCommonAttack_3_SkillItemID;		//���ܿ�ID
	int			m_nCommonAttack_4_SkillItemID;		//���ܿ�ID
	
	tagSkillStaticInfo m_StaticSkillInfo;			//������Ϣ
	CNFSkillItemVec::iterator m_SkillItem;			//���ܿ�ĵ�����

	tagSkillCDInfo m_Skill_1_SkillItem;				//���ܿ�
	tagSkillCDInfo m_Skill_2_SkillItem;				//���ܿ�
	tagSkillCDInfo m_Skill_3_SkillItem;				//���ܿ�
	tagSkillCDInfo m_Skill_4_SkillItem;				//���ܿ�
	tagSkillCDInfo m_Skill_5_SkillItem;				//���ܿ�

	CCArray		*m_pArrayFriend;					//�Ѿ�����
	CCArray		*m_pArrayTarget;					//Ŀ������

	//=======================================AI
	bool		m_bIsAutoMode;						//�Ƿ����Զ�
	bool		m_bIsAutoAttacking;					//�Ƿ�����ִ���Զ�������
	bool		m_bIsAutoListAttacking;				//�Ƿ�����ִ���Զ����м�����
	int			m_nAutoListTime;					//����ʱ��
	CNFDistanceSkillVec::iterator m_AutoListAttack;	//���м���

	tagProtagonistAIInfo m_tagProtagonistAIInfo;


public:
	//create
	static CNFProtagonist* Create(int nID,CCArray *pArrayFriend,CCArray *pArrayTarget);
	//���º���
	virtual void CallPerFrame(float fT);
	//ͨ���Ƕ��ƶ�
	void  OnCtrlMoveByRotation(float fRotation);
	void  OnCtrlRunByRotation(float fRotation);
	void  OnCtrlStop();
	//�ƶ���ָ���ĵ����ϵĵ㣨�����ã�
	void OnCtrlMoveToPonit(CCPoint ptPos);
	//��Ծ
	void ProtagonistJump();
	//��ȡ��ǰhp�ٷֱ�
	float GetHPPercent();
	//��ȡ��ǰhp2�ٷֱ�
	float GetHP2Percent();
	//��ȡ��ǰmp�ٷֱ�
	float GetMPPercent();
	//�õ���ɫID
	int GetRoleID(){return m_nRoleID;};
	
	//�˺�
	virtual float HarmFormula(tagTakeABeating tTakeABeating);
	//����
	virtual float TreatFormula(float fTreatPoint);
	//�ٷֱ�����
	virtual float BFBTreatFormula(float fBFBTreatPoint);
	//�ٷֱȻָ�MP
	virtual float BFBRecoverMP(float fBFBMPPoint);
	//��ͨ����
	void OnCtrlCommonAttack();
	
	//��ʱ������
	void OnCtrlTemp();
	//����
	bool OnCtrlSkill(int nID);

	//�õ�����1��Ϣ
	tagSkillCDInfo GetSkillInfo_1(){return m_Skill_1_SkillItem;}

	//�õ�����2��Ϣ
	tagSkillCDInfo GetSkillInfo_2(){return m_Skill_2_SkillItem;}

	//�õ�����3��Ϣ
	tagSkillCDInfo GetSkillInfo_3(){return m_Skill_3_SkillItem;}

	//�õ�����4��Ϣ
	tagSkillCDInfo GetSkillInfo_4(){return m_Skill_4_SkillItem;}

	//�õ�����5��Ϣ
	tagSkillCDInfo GetSkillInfo_5(){return m_Skill_5_SkillItem;}

	//�õ���ͨ����״̬
	int	GetCommonAttackStatus(){return m_enCommonAttackStatus;}
	
	//�õ���ͨ����CD�ٷֱ�
	float GetCommonAttackCDBFB(){return m_nCommonAttackCDTemp/(float)m_nCommonAttackCD;}

	//�Ƿ����Զ�
	void SetAutoMode(bool bIsAutoMode){m_bIsAutoMode = bIsAutoMode;}

protected:
	//init
	bool Init(int nID,CCArray *pArrayFriend,CCArray *pArrayTarget);
	//��������
	void OnDead();
	//onExit
	virtual void onExit();
private:
	//����Ϊhold
	void OnHold();
	//���ܻص�
	void OnCtrlSkillCallBack();
	//CCArmature�Ļص�����
	void OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID );

	//�ж��Ƿ�Я���д˼���ID�ļ��ܲ����Ƿ�û����ȴ��
	bool IsHaveSkillIDAndCD(int nSkillID);

	//
	void AutoAttackOverCallBack();
};
#endif