#ifndef _NF_BASICUNIT_HEAD_
#define _NF_BASICUNIT_HEAD_
#include "../publicdef/PublicDef.h"
#include "NFWorld3D.h"

/************************************************************************/
/*			单位基类：单位基类是所有单位类的父类，所有单位继承他	    */
/************************************************************************/
//displayHPLable的类型
enum
{
	enDisplayHPLableCommon=1,		//普通HP显示

	enDisplayHPLableCrit=2,			//暴击HP显示

	enDisplayHPLableTreat=3,		//治疗HP显示

	enDisplayHPLableMiss=4,			//miss

	enDisplayMPLableRecover=5,		//MP恢复
};

//单位基类:不可以用来创建对象
class CNFBasicUnit :public CNFUnit3D
{
private:
	
protected:
	
public:
	//每周期更新函数(由战场的周期函数负责调用)
	virtual void CallPerFrame(float fT);
	//播放血量数字
	void displayHPLable(int sh,int type);
	//播放打击动画
	void displayHitLable(int type);
protected:
	//初始化。fRate：倾斜度。szSize：体积
	virtual bool init(float fRate,tagSize3D szSize);
protected:
	enum
	{
		enTagDisplayHPLable=700,
	};
};
#endif