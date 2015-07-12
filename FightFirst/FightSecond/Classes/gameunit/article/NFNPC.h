#ifndef _NF_NPC_HEAD_
#define _NF_NPC_HEAD_

#include "NFBasicArticle.h"

/************************************************************************/
/*			NPC��														*/
/************************************************************************/

class CNFNPC :public CNFBasicArticle,public CMsgSender
{
protected:
	unsigned		int m_nExistTime;		//����ʱ�䣨֡����
	int				m_nId;					//NPCID
public:
	//create
	static CNFNPC* Create(int nId,const char* pArmatureName,float fL,float fW,float fH);
	//���º���
	virtual void CallPerFrame(float fT);
	//
	int GetID();
protected:
	//init
	bool Init(int nId,const char* pArmatureName,float fL,float fW,float fH);
	//onExit
	virtual void onExit();
	//CCArmature�Ļص�����
	void OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID );
};
#endif