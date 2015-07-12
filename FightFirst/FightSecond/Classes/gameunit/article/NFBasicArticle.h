#ifndef _NF_BASIC_ARTICLE_HEAD_
#define _NF_BASIC_ARTICLE_HEAD_

#include "../NFBasicUnit.h"

/************************************************************************/
/*				��Ʒ����:������������������                             */
/************************************************************************/

class CNFBasicArticle :public CNFBasicUnit
{
protected:
	//��ǩ
	enum
	{
		enTagBody=20000,
	};

protected:
	bool		m_bIsDeath;						//�Ƿ�����
	bool		m_bIsInvincible;				//�Ƿ��޵�
	float		m_fHP;							//����
	float		m_fHPTemp;						//HP
	float		m_fMoveSpeed;					//�ƶ��ٶ�
	float		m_fMoveRate;					//�ƶ��Ƕ�
public:
	//�Ƿ�����
	bool IsDeath();
	//�˺�
	virtual float HarmFormula(float fDamagePoint,int wDamageType,int nCrit){return 0;};
	//��ȡ��ǰhp�ٷֱ�
	float GetHPPercent();
	//��������CCArmature
	CCArmature* GetCCArmature();
protected:
	//��ʼ����pArmatureName��Armature����fRate����б�ȡ�szSize�����
	virtual bool init(const char* pArmatureName,float fRate,tagSize3D szSize);
	//��ʼ����fRate����б�ȡ�szSize�����
	virtual bool init(float fRate,tagSize3D szSize);
	//��ʼ����fRate����б�ȡ�szSize�������pPngName��png��
	virtual bool init(float fRate,tagSize3D szSize,const char* pPngName);
	//onExit
	virtual void onExit();
};


#endif