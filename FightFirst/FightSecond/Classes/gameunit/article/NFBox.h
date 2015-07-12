#ifndef _NF_BOX_HEAD_
#define _NF_BOX_HEAD_

#include "NFBasicArticle.h"

/************************************************************************/
/*			�����ࣺ������������						*/
/************************************************************************/

class CNFBox :public CNFBasicArticle,public CMsgSender
{
protected:
	unsigned		int m_nExistTime;		//����ʱ�䣨֡����
public:
	//create
	static CNFBox* CreateBox(int nBoxID);
	//���º���
	virtual void CallPerFrame(float fT);
	//��ȡ��ǰhp�ٷֱ�
	float GetHPPercent();
	//�˺�
	virtual float HarmFormula(float fDamagePoint,int wDamageType,int nCrit);
protected:
	//init
	bool InitBox(int nBoxID);
	//��������
	void OnDead();
	//onExit
	virtual void onExit();
};
#endif