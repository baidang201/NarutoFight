#ifndef _NF_TRANSFERDOOR_HEAD_
#define _NF_TRANSFERDOOR_HEAD_

#include "NFBasicArticle.h"

/************************************************************************/
/*			 传送门类													*/
/************************************************************************/

class CNFTransferDoor :public CNFBasicArticle,public CMsgSender
{
protected:
	CNFUnit3D*		m_pProtagonist;			//主角
	unsigned		int m_nExistTime;		//存在时间（帧数）
	int				m_nId;					//ID
public:
	//create
	static CNFTransferDoor* Create(int nStageId,CNFUnit3D* pProtagonist,const char* pArmaturePlayName,float fL,float fW,float fH);
	//更新函数
	virtual void CallPerFrame(float fT);
	//
	int GetID();
protected:
	//init
	bool Init(int nStageId,CNFUnit3D* pProtagonist,const char* pArmaturePlayName,float fL,float fW,float fH);
	//onExit
	virtual void onExit();
	//
	void OnTransfer();
	//CCArmature的回调函数
	void OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID );
};
#endif