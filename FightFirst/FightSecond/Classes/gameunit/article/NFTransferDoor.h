#ifndef _NF_TRANSFERDOOR_HEAD_
#define _NF_TRANSFERDOOR_HEAD_

#include "NFBasicArticle.h"

/************************************************************************/
/*			 ��������													*/
/************************************************************************/

class CNFTransferDoor :public CNFBasicArticle,public CMsgSender
{
protected:
	CNFUnit3D*		m_pProtagonist;			//����
	unsigned		int m_nExistTime;		//����ʱ�䣨֡����
	int				m_nId;					//ID
public:
	//create
	static CNFTransferDoor* Create(int nStageId,CNFUnit3D* pProtagonist,const char* pArmaturePlayName,float fL,float fW,float fH);
	//���º���
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
	//CCArmature�Ļص�����
	void OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID );
};
#endif