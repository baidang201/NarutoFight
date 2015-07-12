#ifndef _NF_BASICSKILL_HEAD_
#define _NF_BASICSKILL_HEAD_
#include "../NFBasicUnit.h"
#include "../person/NFBasicPerson.h"

//人物基类:不可以用来创建对象
class CNFBasicSkill :public CNFBasicUnit
{
private:
	
protected:
	bool			m_bIsVanish;					//是否死亡
	CNFBasicPerson* m_pReleaser;					//释放者
	CCArray*		m_pArrayTarget;					//目标数组
	CCArray*		m_pArticleArray;				//物体数组
public:
	//是否死亡
	bool IsDeath();
	//添加可碰撞物体数组
	void SetArticleArray(CCArray *pArticleArray);
	//获得技能的CCArmature
	CCArmature* GetCCArmature();
protected:
	//初始化。pArmatureName：Armature名。pReleaser：释放者。pArrayTarget：目标数组。fRate：倾斜度。szSize：体积。nDirection：方向
	virtual bool init(const char* pArmatureName,CNFBasicPerson* pReleaser,CCArray *pArrayTarget,float fRate,tagSize3D szSize,int nDirection);
	//初始化。pReleaser：释放者。pArrayTarget：目标数组。fRate：倾斜度。szSize：体积。nDirection：方向
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