#ifndef _NF_NPC_HEAD_
#define _NF_NPC_HEAD_

#include "NFBasicArticle.h"

/************************************************************************/
/*			NPC类														*/
/************************************************************************/

class CNFNPC :public CNFBasicArticle,public CMsgSender
{
protected:
	unsigned		int m_nExistTime;		//存在时间（帧数）
	int				m_nId;					//NPCID
public:
	//create
	static CNFNPC* Create(int nId,const char* pArmatureName,float fL,float fW,float fH);
	//更新函数
	virtual void CallPerFrame(float fT);
	//
	int GetID();
protected:
	//init
	bool Init(int nId,const char* pArmatureName,float fL,float fW,float fH);
	//onExit
	virtual void onExit();
	//CCArmature的回调函数
	void OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID );
};
#endif