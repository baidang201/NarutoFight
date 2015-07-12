#ifndef _NF_BASIC_ARTICLE_HEAD_
#define _NF_BASIC_ARTICLE_HEAD_

#include "../NFBasicUnit.h"

/************************************************************************/
/*				物品基类:不可以用来创建对象                             */
/************************************************************************/

class CNFBasicArticle :public CNFBasicUnit
{
protected:
	//标签
	enum
	{
		enTagBody=20000,
	};

protected:
	bool		m_bIsDeath;						//是否死亡
	bool		m_bIsInvincible;				//是否无敌
	float		m_fHP;							//体力
	float		m_fHPTemp;						//HP
	float		m_fMoveSpeed;					//移动速度
	float		m_fMoveRate;					//移动角度
public:
	//是否死亡
	bool IsDeath();
	//伤害
	virtual float HarmFormula(float fDamagePoint,int wDamageType,int nCrit){return 0;};
	//获取当前hp百分比
	float GetHPPercent();
	//获得人物的CCArmature
	CCArmature* GetCCArmature();
protected:
	//初始化。pArmatureName：Armature名。fRate：倾斜度。szSize：体积
	virtual bool init(const char* pArmatureName,float fRate,tagSize3D szSize);
	//初始化。fRate：倾斜度。szSize：体积
	virtual bool init(float fRate,tagSize3D szSize);
	//初始化。fRate：倾斜度。szSize：体积。pPngName：png名
	virtual bool init(float fRate,tagSize3D szSize,const char* pPngName);
	//onExit
	virtual void onExit();
};


#endif