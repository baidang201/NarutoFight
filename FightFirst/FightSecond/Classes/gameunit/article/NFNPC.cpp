#include "NFNPC.h"
#include "../../datamanager/NFDataManager.h"

CNFNPC* CNFNPC::Create(int nId,const char* pArmatureName,float fL,float fW,float fH)
{
	do
	{
		CNFNPC * pSpr = new  CNFNPC();
		CC_BREAK_IF(pSpr==NULL);
		CC_BREAK_IF(pSpr->Init(nId,pArmatureName,fL,fW,fH)==false);
		pSpr->autorelease();
		return pSpr;
	} while (false);
	CCLog("Fun CNFNPC::Create Error!");
	return NULL;
}

bool CNFNPC::Init(int nId,const char* pArmatureName,float fL,float fW,float fH)
{
	do
	{
		m_nId = nId;

		tagSize3D tagNewSize;
		tagNewSize.fLength = fL;
		tagNewSize.fWidth = fW;
		tagNewSize.fHeight = fH;

		CC_BREAK_IF(CNFBasicArticle::init(pArmatureName,0.4f,tagNewSize)==false);
		GetCCArmature()->getAnimation()->play("hold");


		m_nExistTime = 0;
		

		return true;
	} while (false);
	CCLog("Fun CNFNPC::Init Error!");
	return false;
}

void CNFNPC::CallPerFrame(float fT)
{
	//Èç¹ûËÀÍöÍ£Ö¹¸ü
	if(m_bIsDeath){return;}
	m_nExistTime++;
}

void CNFNPC::onExit()
{
	CNFBasicArticle::onExit();
}

int CNFNPC::GetID()
{
	return m_nId;
}

void CNFNPC::OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID )
{
	if(movementType==START){
		
	}
	if(movementType==COMPLETE){
		
	}
	if(movementType==LOOP_COMPLETE){
		if(strcmp(movementID,"hold")==0){

		}
	}
}



