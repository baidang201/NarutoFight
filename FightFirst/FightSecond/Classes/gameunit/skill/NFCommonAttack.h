#ifndef _NF_COMMONATTACK_HEAD_
#define _NF_COMMONATTACK_HEAD_

#include "NFBasicSkill.h"
/************************************************************************/
/*			普通攻击类：用来创建普通攻击技能						    */
/************************************************************************/
class CNFCommonAttack :public CNFBasicSkill,public CMsgSender{
private:
	unsigned	int m_nExistTime;			//存在时间（帧数）
	CCArray		*m_pHitOverArray;			//碰撞完数组
	tagTakeABeating m_tTakeABeatingInfo;	//攻击的信息
public:
	//create。tCommonAttackInfo：技能块的信息。pArrayTarget：目标数组
	static CNFCommonAttack* CreateCommonAttack(tagCommonAttack *tCommonAttackInfo,CCArray *pArrayTarget);
	//更新函数
	virtual void CallPerFrame(float fT);
protected:
	//init。tCommonAttackInfo：技能块的信息。pArrayTarget：目标数组
	bool InitCommonAttack(tagCommonAttack *tCommonAttackInfo,CCArray *pArrayTarget);
	//onExit
	virtual void onExit();
};
#endif