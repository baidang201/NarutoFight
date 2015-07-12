#ifndef _NF_BASICSKILL_HEAD_
#define _NF_BASICSKILL_HEAD_
#include "../NFBasicUnit.h"
#include "../person/NFBasicPerson.h"

//�������:������������������
class CNFBasicSkill :public CNFBasicUnit
{
private:
	
protected:
	bool			m_bIsVanish;					//�Ƿ�����
	CNFBasicPerson* m_pReleaser;					//�ͷ���
	CCArray*		m_pArrayTarget;					//Ŀ������
	CCArray*		m_pArticleArray;				//��������
public:
	//�Ƿ�����
	bool IsDeath();
	//��ӿ���ײ��������
	void SetArticleArray(CCArray *pArticleArray);
	//��ü��ܵ�CCArmature
	CCArmature* GetCCArmature();
protected:
	//��ʼ����pArmatureName��Armature����pReleaser���ͷ��ߡ�pArrayTarget��Ŀ�����顣fRate����б�ȡ�szSize�������nDirection������
	virtual bool init(const char* pArmatureName,CNFBasicPerson* pReleaser,CCArray *pArrayTarget,float fRate,tagSize3D szSize,int nDirection);
	//��ʼ����pReleaser���ͷ��ߡ�pArrayTarget��Ŀ�����顣fRate����б�ȡ�szSize�������nDirection������
	virtual bool init(CNFBasicPerson* pReleaser,CCArray *pArrayTarget,float fRate,tagSize3D szSize,int nDirection);
	//onExit
	virtual void onExit();
protected:
	enum
	{
		enTagBody=20000,
	};
};
#endif