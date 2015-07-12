#ifndef _NF_BASICPERSON_HEAD_
#define _NF_BASICPERSON_HEAD_
#include "../NFBasicUnit.h"


////��������
//enum
//{
//	enAttackBlunt=1,			//�����
//	enAttackSharp=2,			//������
//	enAttackThunder=3,			//�׵�
//	enAttackFlame=4,			//����
//	enAttackCryo=5,				//��˪
//};

//����
enum
{
	enTagDirectionR = 1,		//��
	enTagDirectionL = -1,		//��
};

//�������:������������������
class CNFBasicPerson :public CNFBasicUnit
{
private:
	int m_nIRGB1;		//��ʱ
	int m_nSpeedRGB1;	//��ʱ
protected:

	unsigned int m_nExistTime;					//����ʱ�䣨֡����
	bool		m_bIsDeath;						//�Ƿ�����
	bool		m_bIsInvincible;				//�Ƿ��޵�

	int			m_nLV;							//�ȼ�
	//float		m_fPower;						//����
	//float		m_fWit;							//����
	//float		m_fAgility;						//����
	float		m_fHP;							//����ֵ
	float		m_fMP;							//����ֵ
	float		m_fDefense;						//������
	float		m_fRefillHP;					//�����ָ�
	float		m_fRefillMP;					//�����ָ�
	float		m_fAttack;						//������
	float		m_fDomination;					//�ƿ���
	float		m_fMoveSpeed;					//�ƶ��ٶ�
	float		m_fMoveRunSpeed;				//�����ٶ�
	float		m_fAttackSpeed;					//�����ٶ�	100�ǻ���ֵ��1���٣�
	int			m_nCrit;						//������
	float		m_fCritDamage;					//�����˺�
	int			m_nCrickTime;					//Ӳֱʱ��
	int			m_nConverselyTime;				//����ʱ��
	int			m_nHitTarget;					//����
	int			m_nAvoid;						//�ر�

	float		m_fMadter;						//��ͨ
	float		m_fTacitUnderstanding;			//Ĭ��
	float		m_fPerseverance;				//����


	float		m_fTreatCoefficient;			//����ϵ��
	float		m_fAttackUpCoefficient;			//��������ǿϵ��

	int			m_nSuperArmorTime;				//����ʱ��
	int			m_nAstheniaTime;				//˥��ʱ��
	int			m_nAttackSpeedCutTime;			//����ʱ��
	float		m_fAttackSpeedCut_Quantity;		//������
	int			m_nMoveSpeedCutTime;			//����ʱ��
	float		m_fMoveSpeedCut_Quantity;		//������
	int			m_nPoisoningTime;				//�ж�ʱ��
	float		m_fPoisoning_Quantity;			//�ж���
	int			m_nAttackUpTime;				//��������ǿʱ��
	float		m_fAttackUp_Quantity;			//��������ǿ��
	int			m_nAttackUpSTime;				//��������ǿ2ʱ��
	float		m_fAttackUpS_Quantity;			//��������ǿ2��



	float		m_fAttackSpeedTemp;				//��ǰ����
	int			m_nConverselyTimeTemp;			//��ǰ����ʱ��
	int			m_nCrickTimeTemp;				//��ǰӲֱʱ��
	float		m_fHPTemp;						//��ǰHP
	float		m_fHP2Temp;						//��ǰ����Ѫ��HP
	float		m_fMPTemp;						//��ǰMP
	float		m_fMoveSpeedTemp;				//��ǰ�ƶ��ٶ�
	float		m_fMoveRate;					//�ƶ��Ƕ�
	int			m_nDirection;					//���� 
	CCArray		*m_pArticleArray;				//��������
	bool		m_bIsFixationMoveRateStatus;	//�������ƶ�״̬


	tagSize3D	m_HoldImpactSize;				//����ʱ�����
	tagSize3D	m_MoveImpactSize;				//�ƶ�ʱ�����
	tagSize3D	m_RunImpactSize;				//����ʱ�����
	tagSize3D	m_ConverselyImpactSize;			//����ʱ�����
public:
	//�Ƿ�����
	bool IsDeath();
	//���º���
	virtual void CallPerFrame(float fT);
	//�˺�
	virtual float HarmFormula(tagTakeABeating tTakeABeating);
	//����
	virtual float TreatFormula(float fTreatPoint);
	//�ٷֱ�����
	virtual float BFBTreatFormula(float fBFBTreatPoint);
	//�ٷֱȻָ�MP
	virtual float BFBRecoverMP(float fBFBMPPoint);
	//��ȡ��ǰhp�ٷֱ�
	float GetHPPercent();
	//������ﵱǰ����
	int GetDirection();
	//�������ﵱǰ����
	void SetDirection(int nDirection);
	void SetDirectionEnemy(int nDirection);
	//��������CCArmature
	CCArmature* GetCCArmature();
	//��ӿ���ײ��������
	void SetArticleArray(CCArray *pArticleArray);
	//�������ĵ�ǰ�ƶ�����
	float	GetMoveRate();
	//������ﵱǰ�Ƿ��ڶ������ƶ�״̬
	bool	GetIsFixationMoveRateStatus();
	//������ﵱǰ�Ƿ��ڵ�����
	bool	GetIsOnGroundStatus();
	//��õ�ǰ������
	float GetAttack(){return m_fAttack*m_fAttackUpCoefficient;}
	//��õ�ǰ����
	int GetCrit(){return m_nCrit;}
	//��õ�ǰ�����˺�
	float GetCritDamage(){return m_fCritDamage;}
	//��õ�ǰ����
	int	GetHitTarget(){return m_nHitTarget;}
	//��õ�ǰ�ƿ���
	float	GetDomination(){return m_fDomination;}
protected:
	//�������Ĺ��췽��
	virtual bool init(const char* pArmatureName,float fRate,tagSize3D szSize);
	//onExit
	virtual void onExit();
public:
	//��Ӱ���buff
	void AddSuperArmorBuff(int nTime);
	//���˥��buff  ����˥��״̬������������
	void AddAstheniaBuff(int nTime);
	//��ӻ���buff
	void AddAttackSpeedCutBuff(int nTime,float fQuantity);
	//��Ӽ���buff
	void AddMoveSpeedCutBuff(int nTime,float fQuantityBFB);
	//����ж�buff
	void AddPoisoningBuff(int nTime,float fQuantity);
	//��ӹ�������ǿbuff.ʱ�������˺�����
	void AddAttackUpBuff(int nTime,float fQuantity);
	//��ӹ�������ǿ2buff.ʱ�������˺�����
	void AddAttackUpSBuff(int nTime,float fQuantity);
protected:
	enum
	{
		enTagBody=20000,
	};
};
#endif