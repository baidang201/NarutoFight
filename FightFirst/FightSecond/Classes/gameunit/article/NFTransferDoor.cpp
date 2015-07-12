#include "NFTransferDoor.h"
#include "../../datamanager/NFDataManager.h"


CNFTransferDoor* CNFTransferDoor::Create(int nId,CNFUnit3D* pProtagonist,const char* pArmaturePlayName,float fL,float fW,float fH)
{
	do
	{
		CNFTransferDoor * pSpr = new  CNFTransferDoor();
		CC_BREAK_IF(pSpr==NULL);
		CC_BREAK_IF(pSpr->Init(nId,pProtagonist,pArmaturePlayName,fL,fW,fH)==false);
		pSpr->autorelease();
		return pSpr;
	} while (false);
	CCLog("Fun CNFTransferDoor::Create Error!");
	return NULL;
}

bool CNFTransferDoor::Init(int nId,CNFUnit3D* pProtagonist,const char* pArmaturePlayName,float fL,float fW,float fH)
{
	do
	{
		tagSize3D tagNewSize;
		tagNewSize.fLength = fL;
		tagNewSize.fWidth = fW;
		tagNewSize.fHeight = fH;

		CC_BREAK_IF(CNFBasicArticle::init("portal",0.4f,tagNewSize)==false);
		GetCCArmature()->getAnimation()->play(pArmaturePlayName);
		
		m_pProtagonist = pProtagonist;
		m_nId = nId;


		m_nExistTime = 0;
		

		return true;
	} while (false);
	CCLog("Fun CNFTransferDoor::Init Error!");
	return false;
}

void CNFTransferDoor::CallPerFrame(float fT)
{
	m_nExistTime++;

	CNFUnit3D* pProtagonist= dynamic_cast<CNFUnit3D*>(m_pProtagonist);
	if(pProtagonist!=NULL){
		tagVector3D vec3D;
		//ÈôÅö×²
		if(int nIntersects = GetIntersectsVec(pProtagonist,vec3D))
		{
			if(m_bIsDeath){return;}
			OnTransfer();
			m_bIsDeath = true;
			return;
		}else{
			m_bIsDeath = false;
		}
	}
}

void CNFTransferDoor::onExit()
{
	CNFBasicArticle::onExit();
}

int CNFTransferDoor::GetID()
{
	return m_nId;
}

void CNFTransferDoor::OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID )
{
	if(movementType==START){
		
	}
	if(movementType==COMPLETE){
		
	}
	if(movementType==LOOP_COMPLETE){
		
	}
}

void CNFTransferDoor::OnTransfer()
{
	do 
	{
		tagTransferDoor Cmd;
		Cmd.nID = m_nId;
		if(SendMsg(enMsgProtagonistTransfer,&Cmd,sizeof(Cmd))==false)	 
		{
			CCLog("CNFTransferDoor::OnTransfer SendMsg Error!");
		}
		return;
	} while (false);
	CCLog("Fun CNFTransferDoor::OnTransfer Error!");
}



