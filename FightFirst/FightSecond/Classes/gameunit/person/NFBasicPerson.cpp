#include "NFBasicPerson.h"
#include "../article/NFBasicArticle.h"

bool CNFBasicPerson::init(const char* pArmatureName,float fRate,tagSize3D szSize)
{
	do
	{
		CC_BREAK_IF(CNFBasicUnit::init(fRate,szSize)==false);
		CNFUnit3D::SetGravityOpen(true);
		CNFUnit3D::SetOnOutside(false);
		m_nIRGB1 = 0;
		m_nSpeedRGB1 = 5;
		m_nExistTime = 0;

		m_bIsDeath = false;
		m_bIsInvincible = false;
		m_nLV = 1;
		//m_fPower = 0;
		//m_fWit = 0;
		//m_fAgility = 0;
		m_fHP = 10000;
		m_fMP = 10000;
		m_fDefense = 10000000;
		m_fRefillHP = 0.001f;
		m_fRefillMP = 0.001f;
		m_fAttack = 20;
		m_fDomination = 100;
		m_fMoveSpeed = 4;
		m_fMoveRunSpeed = 7;
		m_fAttackSpeed = 100;
		m_nCrit = 20;
		m_fCritDamage = 1.5f;
		m_nCrickTime = 30;
		m_nConverselyTime = 60;
		m_nHitTarget = 500;
		m_nAvoid = 20;

		m_fMadter = 0;
		m_fTacitUnderstanding = 0;
		m_fPerseverance = 0;

		m_fTreatCoefficient = 1.0f;
		m_fAttackUpCoefficient = 1.0f;

		m_nSuperArmorTime = 0;
		m_nAstheniaTime = 0;
		m_nAttackSpeedCutTime = 0;
		m_fAttackSpeedCut_Quantity = 0;
		m_nMoveSpeedCutTime = 0;
		m_fMoveSpeedCut_Quantity = 0;
		m_nPoisoningTime = 0;
		m_fPoisoning_Quantity = 0;
		m_nAttackUpTime = 0;
		m_fAttackUp_Quantity = 0;
		m_nAttackUpSTime = 0;
		m_fAttackUpS_Quantity = 0;


		m_fAttackSpeedTemp = m_fAttackSpeed;
		m_fHPTemp = m_fHP;
		m_fHP2Temp = m_fHP;
		m_fMPTemp = m_fMP;
		m_fMoveSpeedTemp = m_fMoveSpeed;
		m_fMoveRate = -1;
		m_nDirection = enTagDirectionR;
		m_pArticleArray = NULL;
		m_bIsFixationMoveRateStatus = false;
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/protagonist.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/muye.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/mingren.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/zuozhu.ExportJson");NewProject
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/hero_1.ExportJson");

		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/kulougongshou.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/kulouzhanshi.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/mayi.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/bianyikunchong.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/bubing.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/xiaoyin.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/NewProject.ExportJson");
		//CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/minren1.ExportJson");
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
	CCLog("Fun CNFBasicPerson::init Error!");
	return false;
}

void CNFBasicPerson::CallPerFrame( float fT )
{
	m_nExistTime++;

	//单位的物理效果
	bool  bIsArticleI = false;
	int nGTemp = 0;
	CCObject *pItem = NULL;
	tagPosition3D ptPos3D;
	CCARRAY_FOREACH(m_pArticleArray,pItem)
	{
		CNFBasicArticle *pNodeItem = dynamic_cast<CNFBasicArticle*>(pItem);
		if(pNodeItem!=NULL)
		{
			//若碰撞
			if(int nIntersects = IntersectsLT_Z(pNodeItem,m_fMoveSpeedTemp + 5))
			{
				if(nIntersects==enTagIntersects_LEFT){
					ptPos3D.fX = pNodeItem->GetCurrentCube()->ptPos3D.fX + pNodeItem->GetCurrentCube()->szCube.fLength*0.5f + GetCurrentCube()->szCube.fLength*0.5f;
					ptPos3D.fY = GetCurrentCube()->ptPos3D.fY;
					ptPos3D.fZ = GetCurrentCube()->ptPos3D.fZ;
					setPosition3D(ptPos3D);
				}else if(nIntersects==enTagIntersects_RIGHT){

					ptPos3D.fX = pNodeItem->GetCurrentCube()->ptPos3D.fX - pNodeItem->GetCurrentCube()->szCube.fLength*0.5f - GetCurrentCube()->szCube.fLength*0.5f;
					ptPos3D.fY = GetCurrentCube()->ptPos3D.fY;
					ptPos3D.fZ = GetCurrentCube()->ptPos3D.fZ;
					setPosition3D(ptPos3D);

				}else if(nIntersects==enTagIntersects_UP){

					ptPos3D.fX = GetCurrentCube()->ptPos3D.fX;
					ptPos3D.fY = pNodeItem->GetCurrentCube()->ptPos3D.fY + pNodeItem->GetCurrentCube()->szCube.fWidth*0.5f + GetCurrentCube()->szCube.fWidth*0.5f;
					ptPos3D.fZ = GetCurrentCube()->ptPos3D.fZ;
					setPosition3D(ptPos3D);

				}else if(nIntersects==enTagIntersects_BACK){

					ptPos3D.fX = GetCurrentCube()->ptPos3D.fX;
					ptPos3D.fY = pNodeItem->GetCurrentCube()->ptPos3D.fY - pNodeItem->GetCurrentCube()->szCube.fWidth*0.5f - GetCurrentCube()->szCube.fWidth*0.5f;
					ptPos3D.fZ = GetCurrentCube()->ptPos3D.fZ;
					//GetCurrentCube()->ptPos3D.fX,pNodeItem->GetCurrentCube()->ptPos3D.fY - pNodeItem->GetCurrentCube()->szCube.fWidth*0.5f - GetCurrentCube()->szCube.fWidth*0.5f,GetCurrentCube()->ptPos3D.fZ
					setPosition3D(ptPos3D);

				}else if(nIntersects==enTagIntersects_TOP){
					if(GetZSpeed()>0){
						SetZSpeed(-GetZSpeed());
					}
				}
			}
			if(IntersectsLT_TopProjection(pNodeItem,m_fMoveSpeedTemp + 5)&&GetCurrentCube()->ptPos3D.fZ>pNodeItem->GetCurrentCube()->ptPos3D.fZ){
				int nGTempTemp = pNodeItem->GetCurrentCube()->ptPos3D.fZ + pNodeItem->GetCurrentCube()->szCube.fHeight;
				if(nGTempTemp>nGTemp){
					nGTemp = nGTempTemp;
				}
			}
		}
	}
	if(!bIsArticleI){
		SetGround(nGTemp);
	}


	//
	if(m_bIsDeath){return;}

	//霸体
	if(m_nSuperArmorTime>0){
		m_nIRGB1 += m_nSpeedRGB1;
		if(m_nIRGB1>100||m_nIRGB1<1){
			m_nSpeedRGB1 = -m_nSpeedRGB1;
		}
		//GetCCArmature()->setColor(ccc3(m_nIRGB1 + 150,155,0));
		if(m_nSuperArmorTime==1){
			GetCCArmature()->setColor(ccc3(255,255,255));
		}
		m_nSuperArmorTime--;
	}

	//衰弱
	if(m_nAstheniaTime>0){
		m_fTreatCoefficient = 0.5f;
		if(m_nAstheniaTime==1){
			m_fTreatCoefficient = 1.f;
		}
		m_nAstheniaTime--;
	}

	//缓慢
	if(m_nAttackSpeedCutTime>0){
		if(m_nAttackSpeedCutTime==1){
			m_fAttackSpeedTemp = m_fAttackSpeed;
			m_fAttackSpeedCut_Quantity = 0;
		}
		m_nAttackSpeedCutTime--;
	}

	//减速
	if(m_nMoveSpeedCutTime>0){
		if(m_nMoveSpeedCutTime==1){
			m_fMoveSpeedCut_Quantity = 0;
		}
		m_nMoveSpeedCutTime--;
	}

	//中毒
	if(m_nPoisoningTime>0){
		if(m_nExistTime%30==0){
			m_fHPTemp -= m_fPoisoning_Quantity;
			displayHPLable(m_fPoisoning_Quantity,enDisplayHPLableCommon);
			if(m_fHPTemp<=0){
				m_fHPTemp = 0;
				m_bIsDeath = true;
				//OnDead();
			}
		}
		if(m_nPoisoningTime==1){
			m_fPoisoning_Quantity = 0;
		}
		m_nPoisoningTime--;
	}
	//攻击力增强
	if(m_nAttackUpTime>0){
		if(m_nAttackUpTime==1){
			m_fAttackUpCoefficient -= m_fAttackUp_Quantity;
			m_fAttackUp_Quantity = 0;
		}
		m_nAttackUpTime--;
	}
	//攻击力增强2
	if(m_nAttackUpSTime>0){
		if(m_nAttackUpSTime==1){
			m_fAttackUpCoefficient -= m_fAttackUpS_Quantity;
			m_fAttackUpS_Quantity = 0;
		}
		m_nAttackUpSTime--;
	}

	//生命和能量恢复
	if(m_fHPTemp>0&&m_fHPTemp<m_fHP){
		m_fHPTemp += m_fRefillHP;
	}
	if(m_fMPTemp>0&&m_fMPTemp<m_fMP){
		m_fMPTemp += m_fRefillMP;
	}
}

void CNFBasicPerson::onExit()
{
	CNFBasicUnit::onExit();
}

bool CNFBasicPerson::IsDeath()
{
	return m_bIsDeath;
}

float CNFBasicPerson::GetHPPercent()
{
	if(m_fHP!=0){
		return m_fHPTemp/m_fHP;
	}else{
		return 0;
	}
}

int CNFBasicPerson::GetDirection()
{
	return m_nDirection;
}

void CNFBasicPerson::SetDirection( int nDirection )
{
	if(nDirection>0){
		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
			if(pNodeBody!=NULL){
				m_nDirection = enTagDirectionR;
				if(m_bIsFixationMoveRateStatus==false){
					pNodeBody->setRotationY(0);
				}
			}
		}
	}else{
		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
			if(pNodeBody!=NULL){
				m_nDirection = enTagDirectionL;
				if(m_bIsFixationMoveRateStatus==false){
					pNodeBody->setRotationY(180);
				}
			}
		}
	}
}

void CNFBasicPerson::SetDirectionEnemy( int nDirection )
{
	if(nDirection>0){
		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
			if(pNodeBody!=NULL){
				m_nDirection = enTagDirectionR;
				if(m_bIsFixationMoveRateStatus==false){
					pNodeBody->setRotationY(180);
				}
			}
		}
	}else{
		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
			if(pNodeBody!=NULL){
				m_nDirection = enTagDirectionL;
				if(m_bIsFixationMoveRateStatus==false){
					pNodeBody->setRotationY(0);
				}
			}
		}
	}
}

void CNFBasicPerson::SetArticleArray( CCArray *pArticleArray )
{
	m_pArticleArray = pArticleArray;
}

CCArmature* CNFBasicPerson::GetCCArmature()
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

float CNFBasicPerson::GetMoveRate()
{
	return m_fMoveRate;
}

bool CNFBasicPerson::GetIsFixationMoveRateStatus()
{
	return m_bIsFixationMoveRateStatus;
}

bool CNFBasicPerson::GetIsOnGroundStatus()
{
	if(m_nGround + m_fSubtractSpeedl>=m_Cube3D.ptPos3D.fZ){
		return true;
	}else{
		return false;
	}
}

float CNFBasicPerson::HarmFormula( tagTakeABeating tTakeABeating )
{
	return 0.f;
}

float CNFBasicPerson::TreatFormula( float fTreatPoint )
{
	return 0.f;
}

float CNFBasicPerson::BFBTreatFormula( float fBFBTreatPoint )
{
	return 0.f;
}

void CNFBasicPerson::AddSuperArmorBuff( int nTime )
{
	if(nTime>1){
		m_nSuperArmorTime = nTime;
	}else{
		CCLog("Error CNFBasicPerson::AddSuperArmorBuff  nTime <= 1");
	}
}

void CNFBasicPerson::AddAstheniaBuff( int nTime )
{
	if(nTime>1){
		if(nTime>m_nAstheniaTime){
			m_nAstheniaTime = nTime;
		}
	}else{
		CCLog("Error CNFBasicPerson::AddAstheniaBuff  nTime <= 1");
	}
}

void CNFBasicPerson::AddAttackSpeedCutBuff( int nTime,float fQuantity )
{
	if(nTime>1){
		if(nTime>m_nAttackSpeedCutTime){
			m_nAttackSpeedCutTime = nTime;
		}
		if(m_fAttackSpeedCut_Quantity < fQuantity){
			m_fAttackSpeedCut_Quantity = fQuantity;
		}
		m_fAttackSpeedTemp -= m_fAttackSpeedCut_Quantity;
		if(m_fAttackSpeedTemp<0){
			m_fAttackSpeedTemp = 0;
		}
	}else{
		CCLog("Error CNFBasicPerson::AddAttackSpeedCutBuff  nTime <= 1");
	}
}

void CNFBasicPerson::AddMoveSpeedCutBuff( int nTime,float fQuantityBFB )
{
	if(nTime>1){
		if(nTime>m_nMoveSpeedCutTime){
			m_nMoveSpeedCutTime = nTime;
		}
		if(m_fMoveSpeedCut_Quantity < fQuantityBFB){
			m_fMoveSpeedCut_Quantity = fQuantityBFB;
		}
		if(m_fMoveSpeedCut_Quantity>1){
			m_fMoveSpeedCut_Quantity = 1;
		}
		if(m_fMoveSpeedCut_Quantity<0){
			m_fMoveSpeedCut_Quantity = 0;
		}
	}else{
		CCLog("Error CNFBasicPerson::AddMoveSpeedCutBuff  nTime <= 1");
	}
}

void CNFBasicPerson::AddPoisoningBuff( int nTime,float fQuantity )
{
	if(nTime>1){
		if(fQuantity<0){
			fQuantity = 0;
		}
		if(m_nPoisoningTime<=1){
			m_nPoisoningTime = nTime;
			m_fPoisoning_Quantity = fQuantity;
		}else{
			m_fPoisoning_Quantity += fQuantity;
		}
	}else{
		CCLog("Error CNFBasicPerson::AddPoisoningBuff  nTime <= 1");
	}
}

void CNFBasicPerson::AddAttackUpBuff( int nTime,float fQuantity )
{
	if(nTime>1){
		if(fQuantity<0){
			fQuantity = 0;
		}
		m_nAttackUpTime = nTime;
		m_fAttackUp_Quantity += fQuantity;
		m_fAttackUpCoefficient += fQuantity;
	}else{
		CCLog("Error CNFBasicPerson::AddAttackUpBuff  nTime <= 1");
	}
}

void CNFBasicPerson::AddAttackUpSBuff( int nTime,float fQuantity )
{
	if(nTime>1){
		if(fQuantity<0){
			fQuantity = 0;
		}
		m_nAttackUpSTime = nTime;
		m_fAttackUpCoefficient -= m_fAttackUpS_Quantity;
		m_fAttackUpS_Quantity = fQuantity;
		m_fAttackUpCoefficient += m_fAttackUpS_Quantity;
	}else{
		CCLog("Error CNFBasicPerson::AddAttackUpSBuff  nTime <= 1");
	}
}

float CNFBasicPerson::BFBRecoverMP( float fBFBMPPoint )
{
	return 0.f;
}





