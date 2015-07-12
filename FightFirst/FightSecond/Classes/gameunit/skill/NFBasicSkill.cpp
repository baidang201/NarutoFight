#include "NFBasicSkill.h"

bool CNFBasicSkill::init(const char* pArmatureName,CNFBasicPerson* pReleaser,CCArray *pArrayTarget,float fRate,tagSize3D szSize,int nDirection)
{
	do
	{
		CC_BREAK_IF(CNFBasicUnit::init(fRate,szSize)==false);
		CNFUnit3D::SetGravityOpen(false);
		CNFUnit3D::SetShadowVisible(false);
		m_bIsVanish = false;
		m_pReleaser = pReleaser;
		m_pArrayTarget = pArrayTarget;

		m_pArticleArray = NULL;
		
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/protagonist_skill.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/hero_1_skill.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/kulou_arrow.ExportJson");
		CCArmature *pNodeBody = CCArmature::create(pArmatureName);
		if(pNodeBody==NULL)
		{
			CCLog("Can't create Node !");
			break;
		}
		pNodeBody->setPosition(CCPointZero);

		if(nDirection==-1){
			pNodeBody->setRotationY(180);
		}

		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			pBody->addChild(pNodeBody,enZOrderBack,enTagBody);
		}

		pNodeBody->getAnimation()->play("play");
		return true;
	} while (false);
	CCLog("Fun CNFBasicSkill::init Error!");
	return false;
}
bool CNFBasicSkill::init(CNFBasicPerson* pReleaser,CCArray *pArrayTarget,float fRate,tagSize3D szSize,int nDirection)
{
	do
	{
		CC_BREAK_IF(CNFBasicUnit::init(fRate,szSize)==false);
		CNFUnit3D::SetGravityOpen(false);
		CNFUnit3D::SetShadowVisible(false);
		m_bIsVanish = false;
		m_pReleaser = pReleaser;
		m_pArrayTarget = pArrayTarget;
		m_pArticleArray = NULL;

		return true;
	} while (false);
	CCLog("Fun CNFBasicSkill::init Error!");
	return false;
}
void CNFBasicSkill::onExit()
{
	CNFBasicUnit::onExit();
}

bool CNFBasicSkill::IsDeath()
{
	return m_bIsVanish;
}

void CNFBasicSkill::SetArticleArray( CCArray *pArticleArray )
{
	m_pArticleArray = pArticleArray;
}

CCArmature* CNFBasicSkill::GetCCArmature()
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

