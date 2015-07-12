#ifndef _NF_BOX_HEAD_
#define _NF_BOX_HEAD_

#include "NFBasicArticle.h"

/************************************************************************/
/*			箱子类：用来创建箱子						*/
/************************************************************************/

class CNFBox :public CNFBasicArticle,public CMsgSender
{
protected:
	unsigned		int m_nExistTime;		//存在时间（帧数）
public:
	//create
	static CNFBox* CreateBox(int nBoxID);
	//更新函数
	virtual void CallPerFrame(float fT);
	//获取当前hp百分比
	float GetHPPercent();
	//伤害
	virtual float HarmFormula(float fDamagePoint,int wDamageType,int nCrit);
protected:
	//init
	bool InitBox(int nBoxID);
	//设置死亡
	void OnDead();
	//onExit
	virtual void onExit();
};
#endif