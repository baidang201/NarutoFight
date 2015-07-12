#include "NFBasicUnit.h"
#include "../datamanager/NFDataManager.h"

bool CNFBasicUnit::init(float fRate,tagSize3D szSize)
{
	do
	{
		tagUnit3DBasicInfo unitInfo ;
		unitInfo.fRate = fRate;
		CC_BREAK_IF(CNFUnit3D::init(unitInfo)==false);
		//CNFUnit3D::SetLTRate(fRate);
		CNFUnit3D::setSize3D(szSize);
		CNFUnit3D::SetBorderVisible(true);

		//ÁÙÊ±
		tagStageStaticInfo StageInfo;
		CNFStaticDataManager::SharedData()->GetStageStaticInfo(1,StageInfo);
		m_fSubtractSpeedl = StageInfo.fGravity;

		return true;
	} while (false);
	CCLog("Fun CNFBasicUnit::init Error!");
	return false;
}

void CNFBasicUnit::CallPerFrame(float fT)
{

}

void CNFBasicUnit::displayHPLable( int sh,int type )
{
	do
	{
		if(enDisplayHPLableMiss==type){
			CCLabelBMFont* pHPFont = CCLabelBMFont::create("miss","fonts/futura-48.fnt");
			
			pHPFont->setPosition(ccp(rand()%40 - 20,GetCurrentCube()->szCube.fHeight/2 + rand()%40 - 25));
			CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
			if(pBody!=NULL){
				pBody->addChild(pHPFont,enZOrderFront,enTagDisplayHPLable);
			}

			CCFiniteTimeAction* pSpawnAction = CCSpawn::create(
				CCMoveBy::create(0.4f,ccp(0,40)),
				CCScaleTo::create(0.4f,1.5f,1.2f),
				CCFadeOut::create(0.4f),
				NULL
				);
			CCAction* pSequencemAction = CCSequence::create(
				CCMoveBy::create(0.2f,ccp(0,50)),
				pSpawnAction,
				CCRemoveSelf::create(true),
				NULL);
			CC_BREAK_IF(!pSequencemAction);
			pHPFont->runAction(pSequencemAction);
			return;
		}



		char HPTemp[NAME_LEN] = {0};
		sprintf(HPTemp,"%d",sh);

		CCLabelBMFont* pHPFont = CCLabelBMFont::create(HPTemp,"fonts/futura-48.fnt");
		if(enDisplayHPLableCrit==type){
			pHPFont->setScale(1.5);
			pHPFont->setColor(ccRED);
		}else if(enDisplayHPLableTreat==type){
			pHPFont->setColor(ccGREEN);
		}else if(enDisplayMPLableRecover==type){
			pHPFont->setColor(ccBLUE);
		}


		pHPFont->setPosition(ccp(rand()%40 - 20,GetCurrentCube()->szCube.fHeight/2 + rand()%40 - 25));

		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			pBody->addChild(pHPFont,enZOrderFront,enTagDisplayHPLable);
		}
		
		CCFiniteTimeAction* pSpawnAction = CCSpawn::create(
			CCMoveBy::create(0.4f,ccp(0,40)),
			CCScaleTo::create(0.4f,1.5f,1.2f),
			CCFadeOut::create(0.4f),
			NULL
			);
		CCAction* pSequencemAction = CCSequence::create(
			CCMoveBy::create(0.2f,ccp(0,50)),
			pSpawnAction,
			CCRemoveSelf::create(true),
			NULL);
		CC_BREAK_IF(!pSequencemAction);
		pHPFont->runAction(pSequencemAction);

		return;
	}while(false);
	CCLog("Fun CNFBasicUnit::displayHPLable Error!");
}

void CNFBasicUnit::displayHitLable( int type )
{
	do
	{
		CCArmature *pArmatureHited = CCArmature::create("hited_light");
		pArmatureHited->setPosition(ccp(rand()%40 - 20,GetCurrentCube()->szCube.fHeight/2 + rand()%40 - 25));
		if(type==enTagAttackType_Fire){
			pArmatureHited->getAnimation()->play("play_1");
		}else if(type==enTagAttackType_Electricity){
			pArmatureHited->getAnimation()->play("play_3");
		}else if(type==enTagAttackType_Sharp){
			pArmatureHited->getAnimation()->play("play_4");
		}else{
			pArmatureHited->getAnimation()->play("play_2");
		}
		
		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			pBody->addChild(pArmatureHited,enZOrderFront);
		}

		CCAction* pArmatureHitedAction = CCSequence::create(
			CCDelayTime::create(1.2f),
			CCRemoveSelf::create(true),
			NULL);
		pArmatureHited->runAction(pArmatureHitedAction);
		return;
	}while(false);
	CCLog("Fun CNFBasicUnit::displayHitLable Error!");
}
