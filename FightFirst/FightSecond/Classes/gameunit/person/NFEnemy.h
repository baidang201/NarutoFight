#ifndef _NF_ENMEY_HEAD_
#define _NF_ENMEY_HEAD_

#include "NFBasicPerson.h"
#include <map>
/************************************************************************/
/*			�����ࣺ������������										*/
/************************************************************************/

class CNFEnemy :public CNFBasicPerson,public CMsgSender
{
private:
	CMapAI				m_MapAI;						//����AI

	float				m_AINearDis;					//AI������
	float				m_AIFarDis;						//AIԶ����
	
	CNFBasicPerson*		m_pTarget;						//Ŀ��
	float				m_fJumpPower;					//��Ծ��
	bool				m_bIsJump;						//�Ƿ�����Ծ״̬
	
	int					m_nTurnRoundTime;				//ת��ʱ��
	int					m_nTurnRoundTimeTemp;	

	int					m_nMoveTimeTemp;

	bool				m_bIsActionRuning;				//�Ƿ��ڹ���״̬��
	bool				m_bIsHoldAnimation;				//�Ƿ񲥴�����Animation

	bool				m_bIsDeadAnimation;				//�Ƿ񲥹�������Animation

	int					m_nEnemyID;						//��ɫID

	CCArray				*m_pArrayFriend;				//�Ѿ�����
	CCArray				*m_pArrayTarget;				//Ŀ������

	tagSkillCDInfo		m_Skill_1_SkillItem;			//���ܿ�
	tagSkillCDInfo		m_Skill_2_SkillItem;			//���ܿ�
	tagSkillCDInfo		m_Skill_3_SkillItem;			//���ܿ�
	tagSkillCDInfo		m_Skill_4_SkillItem;			//���ܿ�

	tagSkillStaticInfo m_StaticSkillInfo;				//������Ϣ
	CNFSkillItemVec::iterator m_SkillItem;				//���ܿ�ĵ�����

	//��ʱ	
	float				m_fNowHPProgress2;

public:
	//create
	static CNFEnemy* CreateEnemy(int nEnemyID,int nEnemyLevel,CCArray *pArrayFriend,CCArray *pArrayTarget);
	//���º���
	virtual void CallPerFrame(float fT);
	//AI����:�жϵ�ǰAI����
	enumEnemyAICondition AI_getCurrnetCondition();
	//AI����:��ǰ�ж�����
	enumEnemyAIKind AI_getCurrentAICmdKind(enumEnemyAICondition enCondition);

	//��ȡ��ǰhp�ٷֱ�
	float GetHPPercent();
	//��ȡ��ǰhp2�ٷֱ�
	float GetHP2Percent();
	
	//�˺�
	virtual float HarmFormula(tagTakeABeating tTakeABeating);
	//����
	virtual float TreatFormula(float fTreatPoint);
	//�ٷֱ�����
	virtual float BFBTreatFormula(float fBFBTreatPoint);
	//�ٷֱȻָ�MP
	virtual float BFBRecoverMP(float fBFBMPPoint);
protected:
	//����
	bool OnCtrlHold();
	//ֱ�߽ӽ�/Զ��
	bool OnCtrlCrosswiseApproach(bool bIsApproach);
	//���߽ӽ�/Զ��
	bool OnCtrlLengthwaysApproach(bool bIsApproach);
	//б�ǿ���
	bool OnCtrlApproach();
	//�Ʊ�
	bool OnCtrlCircuity();
	//����1
	bool OnCtrlAttack1();
	//����2
	bool OnCtrlAttack2();

	//����3
	bool OnCtrlAttack3();
	//����4
	bool OnCtrlAttack4();

	//��ʼ����pProtagonist������ָ��
	bool InitEnemy(int nEnemyID,int nEnemyLevel,CCArray *pArrayFriend,CCArray *pArrayTarget);
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
	//�ж��Ƿ�û����ȴ��
	bool IsSkillNOCD(int nSkillID);
	//�жϼ���ID�ļ����Ƿ��ڹ�����Χ֮��
	bool IsSkillAttactkable(int nSkillID);
protected:
	//��ǩ
	enum 
	{
		enTagHPEnemyProgress1,
		enTagHPEnemyProgress2,
		enTagHPEnemyProgressBg,
	};

};
#endif