#include "NFBasicArticle.h"

bool CNFBasicArticle::init(const char* pArmatureName,float fRate,tagSize3D szSize)
{
	do
	{
		//初始化父类
		CC_BREAK_IF(CNFBasicUnit::init(fRate,szSize)==false);
		//开启重力
		CNFUnit3D::SetGravityOpen(true);
		CNFUnit3D::SetOnOutside(false);

		m_bIsDeath = false;
		m_bIsInvincible = false;
		m_fHP =100;
		m_fHPTemp = 100;
		m_fMoveSpeed = 4;
		m_fMoveRate = -1;

		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/naili.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/portal.ExportJson");
		CCArmature *pNodeBody = CCArmature::create(pArmatureName);
		if(pNodeBody==NULL)
		{
			CCLog("Can't create Node !");
			break;
		}
		pNodeBody->setPosition(CCPointZero);
		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			pBody->addChild(pNodeBody,enZOrderBack,enTagBody);
		}

		return true;
	} while (false);
	CCLog("Fun CNFBasicArticle::init Error!");
	return false;
}

bool CNFBasicArticle::init( float fRate,tagSize3D szSize )
{
	do
	{
		//初始化父类
		CC_BREAK_IF(CNFBasicUnit::init(fRate,szSize)==false);
		//开启重力
		CNFUnit3D::SetGravityOpen(true);

		m_bIsDeath = false;
		m_bIsInvincible = false;
		m_fHP =100;
		m_fHPTemp = 100;
		m_fMoveSpeed = 4;
		m_fMoveRate = -1;

		return true;
	} while (false);
	CCLog("Fun CNFBasicArticle::init Error!");
	return false;
}

bool CNFBasicArticle::init(float fRate,tagSize3D szSize,const char* pPngName)
{
	do
	{
		//初始化父类
		CC_BREAK_IF(CNFBasicUnit::init(fRate,szSize)==false);
		//开启重力
		CNFUnit3D::SetGravityOpen(true);

		m_bIsDeath = false;
		m_bIsInvincible = false;
		m_fHP =100;
		m_fHPTemp = 100;
		m_fMoveSpeed = 4;
		m_fMoveRate = -1;
	
		CCSprite* pNodeBody = CCSprite::create(pPngName);
		pNodeBody->setPosition(CCPointZero);
		pNodeBody->setAnchorPoint(ccp(0.5f,0.2f));
		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			pBody->addChild(pNodeBody,enZOrderBack,enTagBody);
		}

		return true;
	} while (false);
	CCLog("Fun CNFBasicArticle::init Error!");
	return false;
}


void CNFBasicArticle::onExit()
{
	CNFBasicUnit::onExit();
}

bool CNFBasicArticle::IsDeath()
{
	return m_bIsDeath;
}

float CNFBasicArticle::GetHPPercent()
{
	if(m_fHP!=0){
		return m_fHPTemp/m_fHP;
	}else{
		return 0;
	}
}

CCArmature* CNFBasicArticle::GetCCArmature()
{
	CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
	if(pBody!=NULL){
		CCArmature* pArmature = dynamic_cast<CCArmature*>(pBody->getChildByTag(enTagBody));
		if(pArmature!=NULL){
			return pArmature;
		}
		return NULL;
	}
	return NULL;
}
