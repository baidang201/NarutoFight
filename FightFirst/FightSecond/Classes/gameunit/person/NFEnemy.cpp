#include "NFEnemy.h"
#include "../article/NFBasicArticle.h"
#include "../../datamanager/NFDataManager.h"

//Animation名称
#define _NF_ENEMY_HOLD_		"hold"
#define _NF_ENEMY_WALK_		"run"
#define _NF_ENEMY_RUN_		"run"
#define _NF_ENEMY_HIT_		"hit"
#define _NF_ENEMY_FALL_		"fall"
#define _NF_ENEMY_DEAD_		"dead"

CNFEnemy* CNFEnemy::CreateEnemy(int nEnemyID,int nEnemyLevel,CCArray *pArrayFriend,CCArray *pArrayTarget)
{
	do
	{
		CNFEnemy * pSpr = new  CNFEnemy();
		CC_BREAK_IF(pSpr==NULL);
		CC_BREAK_IF(pSpr->InitEnemy(nEnemyID,nEnemyLevel,pArrayFriend,pArrayTarget)==false);
		pSpr->autorelease();
		return pSpr;
	} while (false);
	CCLog("Fun CNFEnemy::CreateProtagonist Error!");
	return NULL;
}

bool CNFEnemy::InitEnemy(int nEnemyID,int nEnemyLevel,CCArray *pArrayFriend,CCArray *pArrayTarget)
{
	do
	{
		m_nEnemyID = nEnemyID;

		m_pArrayFriend = pArrayFriend;
		m_pArrayTarget = pArrayTarget;

		tagEnemyStaticInfo EnemyInfo;
		CNFStaticDataManager::SharedData()->GetEnemyStaticInfo(m_nEnemyID,EnemyInfo);

		m_HoldImpactSize.fLength = EnemyInfo.fLength_Hold;
		m_HoldImpactSize.fWidth = EnemyInfo.fWidth_Hold;
		m_HoldImpactSize.fHeight = EnemyInfo.fHeight_Hold;
		m_MoveImpactSize.fLength = EnemyInfo.fLength_Move;
		m_MoveImpactSize.fWidth = EnemyInfo.fWidth_Move;
		m_MoveImpactSize.fHeight = EnemyInfo.fHeight_Move;
		m_RunImpactSize.fLength = 60;
		m_RunImpactSize.fWidth = 50;
		m_RunImpactSize.fHeight = 80;
		m_ConverselyImpactSize.fLength = EnemyInfo.fLength_Conversely;
		m_ConverselyImpactSize.fWidth = EnemyInfo.fWidth_Conversely;
		m_ConverselyImpactSize.fHeight = EnemyInfo.fHeight_Conversely;


		m_AINearDis = EnemyInfo.fNearDis;
		m_AIFarDis = EnemyInfo.fFarDis;
		
		CC_BREAK_IF(CNFBasicPerson::init(EnemyInfo.strArmatureName.c_str(),0.4f,m_HoldImpactSize)==false);//muye
		
		m_nExistTime = 0;
		m_pTarget = NULL;
		m_fJumpPower = 15;
		m_bIsJump = false;
		m_nCrickTimeTemp = 0;

		m_nTurnRoundTime = EnemyInfo.nTurnRoundTime;
		m_nTurnRoundTimeTemp = m_nTurnRoundTime;

		m_nConverselyTimeTemp = 0;

		m_nDirection = enTagDirectionL;

		m_nLV = nEnemyLevel;
		//m_fPower = EnemyInfo.fPower + m_nLV*EnemyInfo.fPower_g;
		//m_fWit = EnemyInfo.fWit + m_nLV*EnemyInfo.fWit_g;
		//m_fAgility = EnemyInfo.fAgility + m_nLV*EnemyInfo.fAgility_g;
		m_fHP = EnemyInfo.fHP/* + m_fPower*_NF_POWER_HP_*/;
		m_fHPTemp = m_fHP;
		m_fHP2Temp = m_fHP;
		m_fMP = EnemyInfo.fMP/* + m_fWit*_NF_WIT_MP_*/;
		m_fMPTemp = m_fMP;
		m_fDefense = EnemyInfo.fDefense;
		m_fRefillHP = 0.f;
		m_fRefillMP = 0.f;
		m_fAttack = EnemyInfo.fAttack;
		m_fDomination = EnemyInfo.fDomination;
		m_fMoveSpeed = EnemyInfo.fMoveSpeed;
		m_fMoveSpeedTemp = EnemyInfo.fMoveSpeed;
		m_fMoveRunSpeed = 7;
		m_fAttackSpeed = EnemyInfo.fAttackSpeed;
		m_nCrit = EnemyInfo.nCrit;
		m_fCritDamage = EnemyInfo.fCritDamage;
		m_nCrickTime = EnemyInfo.nCrickTime;
		m_nConverselyTime = EnemyInfo.nConverselyTime;
		m_nHitTarget = EnemyInfo.nHitTarget;
		m_nAvoid = EnemyInfo.nAvoid;

		m_fTreatCoefficient = EnemyInfo.fTreatCoefficient;

		m_nMoveTimeTemp = 0;

		m_bIsActionRuning = false;
		m_bIsHoldAnimation = false;
		m_bIsDeadAnimation = false;

		//默认技能等级
		int	nSkill_1_LV = 1;
		int	nSkill_2_LV = 1;
		int	nSkill_3_LV = 1;
		int	nSkill_4_LV = 1;

		m_Skill_1_SkillItem.nSkill_SkillItemID = 1;	
		m_Skill_2_SkillItem.nSkill_SkillItemID = 2;
		m_Skill_3_SkillItem.nSkill_SkillItemID = -1;
		m_Skill_4_SkillItem.nSkill_SkillItemID = -1;

		m_Skill_3_SkillItem.strSkill_AniName = "wu";
		m_Skill_4_SkillItem.strSkill_AniName = "wu";
		
		tagSkillStaticInfo SkillInfo1;
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nEnemyID,m_Skill_1_SkillItem.nSkill_SkillItemID,SkillInfo1);
		m_Skill_1_SkillItem.nSkill_CD_Time = SkillInfo1.nSkillCD + nSkill_1_LV*SkillInfo1.nSkillCD_g;		

		tagSkillStaticInfo SkillInfo2;
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nEnemyID,m_Skill_2_SkillItem.nSkill_SkillItemID,SkillInfo2);
		m_Skill_2_SkillItem.nSkill_CD_Time = SkillInfo2.nSkillCD + nSkill_2_LV*SkillInfo2.nSkillCD_g;

		CNFSkillStaticInfoVec SkillInfoVec;
		CNFStaticDataManager::SharedData()->GetDesignSkillStaticInfoVec(m_nEnemyID,0,SkillInfoVec);
		CNFSkillStaticInfoVec::iterator iterBegin = SkillInfoVec.begin();
		if(iterBegin!=SkillInfoVec.end())
		{
			m_Skill_3_SkillItem.nSkill_SkillItemID = iterBegin->nSkillID;
			m_Skill_3_SkillItem.strSkill_AniName = iterBegin->strAniName;
			m_Skill_3_SkillItem.fCostMp = iterBegin->fCostMp + nSkill_3_LV*iterBegin->fCostMp_g;
			m_Skill_3_SkillItem.nSkill_CD_Time = iterBegin->nSkillCD + nSkill_3_LV*iterBegin->nSkillCD_g;

			iterBegin++;
		}
		if(iterBegin!=SkillInfoVec.end())
		{
			m_Skill_4_SkillItem.nSkill_SkillItemID = iterBegin->nSkillID;
			m_Skill_4_SkillItem.strSkill_AniName = iterBegin->strAniName;
			m_Skill_4_SkillItem.fCostMp = iterBegin->fCostMp + nSkill_4_LV*iterBegin->fCostMp_g;
			m_Skill_4_SkillItem.nSkill_CD_Time = iterBegin->nSkillCD + nSkill_4_LV*iterBegin->nSkillCD_g;
		}

		m_Skill_1_SkillItem.nSkill_CD_Time_Temp = 30;
		m_Skill_2_SkillItem.nSkill_CD_Time_Temp = 30;
		m_Skill_3_SkillItem.nSkill_CD_Time_Temp = 30;
		m_Skill_4_SkillItem.nSkill_CD_Time_Temp = 30;

 		m_Skill_1_SkillItem.strSkill_AniName = SkillInfo1.strAniName;
 		m_Skill_2_SkillItem.strSkill_AniName = SkillInfo2.strAniName;
 		
 		m_Skill_1_SkillItem.fCostMp = SkillInfo1.fCostMp + nSkill_1_LV*SkillInfo1.fCostMp_g;
 		m_Skill_2_SkillItem.fCostMp = SkillInfo2.fCostMp + nSkill_2_LV*SkillInfo2.fCostMp_g;
 		
		//临时缩放
		GetCCArmature()->setScale(0.7f);
		GetCCArmature()->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CNFEnemy::OnCCArmatureCallBack));
		GetCCArmature()->getAnimation()->play(_NF_ENEMY_HOLD_);

		CNFStaticDataManager::SharedData()->GetEnemyAIStaticInfo(m_nEnemyID,m_MapAI);

		/************************************************************************/
		/*				创建敌人血条                                            */
		/************************************************************************/
		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		CC_BREAK_IF(pBody==NULL);
		//添加敌人血条背景
		CCSprite* pEnemyBloodBG = CCSprite::create("gameui/value_enemy_hp_bg.png");
		CC_BREAK_IF(pEnemyBloodBG==NULL);
		pEnemyBloodBG->setPosition(ccp(0,110));
		if (nEnemyID==1005)
			pEnemyBloodBG->setPosition(ccp(0,220));
		pBody->addChild(pEnemyBloodBG,enZOrderMid,enTagHPEnemyProgressBg);
		pEnemyBloodBG->setColor(ccBLACK);

		CCSprite* pEnemySpr2 = CCSprite::create("gameui/value_enemy_hp.png");
		CC_BREAK_IF(pEnemySpr2==NULL);
		CCProgressTimer* pEnemyProgress2 = CCProgressTimer::create(pEnemySpr2);
		CC_BREAK_IF(pEnemyProgress2==NULL);
		pEnemyProgress2->setType(kCCProgressTimerTypeBar);
		pEnemyProgress2->setMidpoint(ccp(0.f,0.f));
		pEnemyProgress2->setBarChangeRate(ccp(1.f,0.f));
		pEnemyProgress2->setPercentage(100.f);
		pEnemyProgress2->setPosition(pEnemyBloodBG->getPosition());

		pBody->addChild(pEnemyProgress2,enZOrderFront,enTagHPEnemyProgress2);
		pEnemyProgress2->setOpacity(150.f);

		CCSprite* pEnemySpr1 = CCSprite::create("gameui/value_enemy_hp.png");
		CC_BREAK_IF(pEnemySpr1==NULL);
		CCProgressTimer* pEnemyProgress1 = CCProgressTimer::create(pEnemySpr1);
		CC_BREAK_IF(pEnemyProgress1==NULL);
		pEnemyProgress1->setType(kCCProgressTimerTypeBar);
		pEnemyProgress1->setMidpoint(ccp(0.f,0.f));
		pEnemyProgress1->setBarChangeRate(ccp(1.f,0.f));
		pEnemyProgress1->setPercentage(100.f);
		pEnemyProgress1->setPosition(pEnemyBloodBG->getPosition());
		
		pBody->addChild(pEnemyProgress1,enZOrderFront,enTagHPEnemyProgress1);

		m_fNowHPProgress2 = 100;
		/************************************************************************/


		return true;
	} while (false);
	CCLog("Fun CNFEnemy::InitProtagonist Error!");
	return false;
}

void CNFEnemy::CallPerFrame(float fT)
{
	CNFBasicPerson::CallPerFrame(fT);
	if(m_nGround + m_fSubtractSpeedl>=m_Cube3D.ptPos3D.fZ){
		m_bIsJump = false;
	}
	//如果死亡停止更
	if(m_bIsDeath){OnDead();return;}


	//选择目标
	if(m_pArrayTarget->count()==0){
		m_pTarget = NULL;
		OnCtrlHold();
	}else{

		if(m_nExistTime%60==0){
			float  MinTemp = 100000000;
			CCObject *pItem = NULL;
			CCARRAY_FOREACH(m_pArrayTarget,pItem)
			{
				CNFBasicPerson *pNodeItem = dynamic_cast<CNFBasicPerson*>(pItem);
				if(pNodeItem!=NULL)
				{
					float fLen = sqrt(pow((pNodeItem->GetCurrentCube()->ptPos3D.fX - GetCurrentCube()->ptPos3D.fX),2) + pow((pNodeItem->GetCurrentCube()->ptPos3D.fY - GetCurrentCube()->ptPos3D.fY),2));
					if(fLen<MinTemp){
						MinTemp = fLen;
						m_pTarget = pNodeItem;
					}
				}
			}
		}
	}

	
	//AI
 	if(m_nExistTime%20==0&&m_bIsActionRuning==false&&m_nCrickTimeTemp<=0&&m_nConverselyTimeTemp<=0){

		if(m_pTarget!=NULL){
			enumEnemyAIKind enumAIKind = AI_getCurrentAICmdKind(AI_getCurrnetCondition());
			if(IsSkillNOCD(m_Skill_3_SkillItem.nSkill_SkillItemID)&&IsSkillAttactkable(m_Skill_3_SkillItem.nSkill_SkillItemID)){
				OnCtrlAttack3();
			}else if(IsSkillNOCD(m_Skill_4_SkillItem.nSkill_SkillItemID)&&IsSkillAttactkable(m_Skill_4_SkillItem.nSkill_SkillItemID)){
				OnCtrlAttack4();
			}else if(enumAIKind==enEnemyAIHold){
				OnCtrlHold();
			}else if(enumAIKind==enEnemyAICrosswiseApproach){
				OnCtrlCrosswiseApproach(true);
			}else if(enumAIKind==enEnemyAICrosswiseDisapproach){
				OnCtrlCrosswiseApproach(false);
			}else if(enumAIKind==enEnemyLengthwaysApproach){
				OnCtrlLengthwaysApproach(true);
			}else if(enumAIKind==enEnemyLengthwaysDisapproach){
				OnCtrlLengthwaysApproach(false);
			}else if(enumAIKind==enEnemyDirectlyApproach){
				OnCtrlApproach();
			}else if(enumAIKind==enEnemyCircuity){
				OnCtrlCircuity();
			}else if(enumAIKind==enEnemyAttackTypeShort){
				if(m_nCrickTimeTemp<=0&&m_nConverselyTimeTemp<=0&&m_bIsActionRuning==false){
					float o = m_pTarget->GetCurrentCube()->ptPos3D.fX - GetCurrentCube()->ptPos3D.fX;
					float a = m_pTarget->GetCurrentCube()->ptPos3D.fY - GetCurrentCube()->ptPos3D.fY;
					float at = (float)CC_RADIANS_TO_DEGREES( atanf(o/a) );
					if(a<0){
						at += 180.f;
					}else if(o<0){
						at += 360.f;
					}
					if(at>180){
						CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
						if(pBody!=NULL){
							CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
							if(pNodeBody!=NULL){
								m_nDirection = enTagDirectionL;
								pNodeBody->setRotationY(0);
							}
						}
					}else{
						if(at==0||at==180){return;}
						CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
						if(pBody!=NULL){
							CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
							if(pNodeBody!=NULL){
								m_nDirection = enTagDirectionR;
								pNodeBody->setRotationY(180);
							}
						}
					}
				}

				if(OnCtrlAttack1()==false){
					OnCtrlAttack2();
				}

			}else if(enumAIKind==enEnemyAttackTypeLong){
				if(m_nCrickTimeTemp<=0&&m_nConverselyTimeTemp<=0&&m_bIsActionRuning==false){
					float o = m_pTarget->GetCurrentCube()->ptPos3D.fX - GetCurrentCube()->ptPos3D.fX;
					float a = m_pTarget->GetCurrentCube()->ptPos3D.fY - GetCurrentCube()->ptPos3D.fY;
					float at = (float)CC_RADIANS_TO_DEGREES( atanf(o/a) );
					if(a<0){
						at += 180.f;
					}else if(o<0){
						at += 360.f;
					}
					if(at>180){
						CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
						if(pBody!=NULL){
							CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
							if(pNodeBody!=NULL){
								m_nDirection = enTagDirectionL;
								pNodeBody->setRotationY(0);
							}
						}
					}else{
						if(at==0||at==180){return;}
						CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
						if(pBody!=NULL){
							CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
							if(pNodeBody!=NULL){
								m_nDirection = enTagDirectionR;
								pNodeBody->setRotationY(180);
							}
						}
					}
				}

				if(OnCtrlAttack2()==false){
					OnCtrlAttack1();
				}
			}



		}
 	}



	//硬直恢复
	if(m_nCrickTimeTemp>0){
		if(m_nCrickTimeTemp==1){
			m_bIsActionRuning = false;
			//OnHold();
		}
		m_nCrickTimeTemp--;
	}

	//倒地恢复
	if(m_nConverselyTimeTemp>0){
		if(m_nConverselyTimeTemp==1){
			m_bIsActionRuning = false;
			//OnHold();
		}
		m_nConverselyTimeTemp--;
	}

	//移动恢复
	if(m_nMoveTimeTemp>0){
		if(m_nMoveTimeTemp==1){
			m_bIsActionRuning = false;
			m_fMoveRate = -1;
			m_nTurnRoundTimeTemp = 1;
			OnHold();
		}
		m_nMoveTimeTemp--;
	}

	//转身恢复
	if(m_pTarget!=NULL&&m_nTurnRoundTimeTemp>0){
		if(m_nTurnRoundTimeTemp==1){
			if(m_nCrickTimeTemp<=0&&m_nConverselyTimeTemp<=0&&m_bIsActionRuning==false){
				float o = m_pTarget->GetCurrentCube()->ptPos3D.fX - GetCurrentCube()->ptPos3D.fX;
				float a = m_pTarget->GetCurrentCube()->ptPos3D.fY - GetCurrentCube()->ptPos3D.fY;
				float at = (float)CC_RADIANS_TO_DEGREES( atanf(o/a) );
				if(a<0){
					at += 180.f;
				}else if(o<0){
					at += 360.f;
				}
				if(at>180){
					CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
					if(pBody!=NULL){
						CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
						if(pNodeBody!=NULL){
							m_nDirection = enTagDirectionL;
							pNodeBody->setRotationY(0);
						}
					}
				}else{
					if(at==0||at==180){return;}
					CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
					if(pBody!=NULL){
						CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
						if(pNodeBody!=NULL){
							m_nDirection = enTagDirectionR;
							pNodeBody->setRotationY(180);
						}
					}
				}
			}
		}
		m_nTurnRoundTimeTemp--;
	}else{
		m_nTurnRoundTimeTemp = m_nTurnRoundTime;
	}


	//技能CD
	if(m_Skill_1_SkillItem.nSkill_CD_Time_Temp>0){
		m_Skill_1_SkillItem.nSkill_CD_Time_Temp--;
	}
	if(m_Skill_2_SkillItem.nSkill_CD_Time_Temp>0){
		m_Skill_2_SkillItem.nSkill_CD_Time_Temp--;
	}
	if(m_Skill_3_SkillItem.nSkill_CD_Time_Temp>0){
		m_Skill_3_SkillItem.nSkill_CD_Time_Temp--;
	}
	if(m_Skill_4_SkillItem.nSkill_CD_Time_Temp>0){
		m_Skill_4_SkillItem.nSkill_CD_Time_Temp--;
	}

	tagPosition3D ptPos3D;
	//移动
	if (m_fMoveRate >= 0 && m_fMoveRate < 360)
	{
		if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0){m_fMoveRate = -1;return;}
		GetCCArmature()->getAnimation()->setSpeedScale(1 - m_fMoveSpeedCut_Quantity);

		CCPoint ptMoveTo;
		ptMoveTo.x = sin(m_fMoveRate/180*3.1415)*m_fMoveSpeedTemp*(1 - m_fMoveSpeedCut_Quantity);
		ptMoveTo.y = cos(m_fMoveRate/180*3.1415)*m_fMoveSpeedTemp*(1 - m_fMoveSpeedCut_Quantity);

		ptMoveTo.x += GetCurrentCube()->ptPos3D.fX;
		ptMoveTo.y += GetCurrentCube()->ptPos3D.fY;

		ptPos3D.fX = ptMoveTo.x;
		ptPos3D.fY = ptMoveTo.y;
		ptPos3D.fZ = 0;
		this->setPosition3D(ptPos3D);
		if(m_fMoveRate>180){
			CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
			if(pBody!=NULL){
				CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
				if(pNodeBody!=NULL){
					m_nDirection = enTagDirectionL;
					pNodeBody->setRotationY(0);
				}
			}
		}else{
			if(m_fMoveRate==0||m_fMoveRate==180){return;}
			CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
			if(pBody!=NULL){
				CCNode * pNodeBody = dynamic_cast<CCNode *>(pBody->getChildByTag(enTagBody));
				if(pNodeBody!=NULL){
					m_nDirection = enTagDirectionR;
					pNodeBody->setRotationY(180);
				}
			}
		}
	}

	CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
	if(pBody!=NULL){
		//临时：血量
		CCProgressTimer* pProgress1 = dynamic_cast<CCProgressTimer*>(pBody->getChildByTag(enTagHPEnemyProgress1));
		CCProgressTimer* pProgress2 = dynamic_cast<CCProgressTimer*>(pBody->getChildByTag(enTagHPEnemyProgress2));

		pProgress1->setPercentage(GetHPPercent()*100);
		if((GetHPPercent()*100)<m_fNowHPProgress2)
		{
			m_fNowHPProgress2 -= 0.5;
		}
		else
		{
			m_fNowHPProgress2 = (GetHPPercent()*100);
			pProgress1->setPercentage(GetHPPercent()*100);
		}
		if(m_fNowHPProgress2<0)
		{
			m_fNowHPProgress2 = 0;
		}
		pProgress2->setPercentage(m_fNowHPProgress2);
	}
	OnHold();
}


float CNFEnemy::GetHPPercent()
{
	if(m_fHP!=0){
		return m_fHPTemp/m_fHP;
	}else{
		return 0;
	}
}

float CNFEnemy::GetHP2Percent()
{
	if(m_fHP!=0){
		return m_fHP2Temp/m_fHP;
	}else{
		return 0;
	}
}

void CNFEnemy::OnDead()
{
	if(m_bIsDeadAnimation==false){
		m_bIsDeadAnimation = true;
		GetCCArmature()->getAnimation()->play(_NF_ENEMY_DEAD_);
	}
}


void CNFEnemy::onExit()
{
	CNFBasicPerson::onExit();
}

float CNFEnemy::HarmFormula( tagTakeABeating tTakeABeating )
{
	//如果死亡停止更
	if(m_bIsDeath){return 0;}

	if(tTakeABeating.fCritDamage < 1){
		tTakeABeating.fCritDamage = 1;
	}
	if(m_fDefense < 0){
		m_fDefense = 0;
	}
	tagSize3D szNew;
	tagVector3D vec3D;
	if(rand()%(tTakeABeating.nHitTarget + m_nAvoid) < tTakeABeating.nHitTarget*1.1f){
		stopAllActions();
		//m_enCommonAttackStatus = enStatusCommonAttack_1;
		if(tTakeABeating.bIsOnlyDamageNotHit==false){
			if(m_nSuperArmorTime<=0||tTakeABeating.bIsBreakSiperarmor==true){
				if(tTakeABeating.bIsConversely==true){
					if(m_nConverselyTimeTemp>0){
						tTakeABeating.fZSpeed = tTakeABeating.fZSpeed*0.2f;
						tTakeABeating.fXTime = tTakeABeating.fXTime*0.2f;
						tTakeABeating.nXDis = tTakeABeating.nXDis*0.2f;
						tTakeABeating.fYTime = tTakeABeating.fYTime*0.2f;
						tTakeABeating.nYDis = tTakeABeating.nYDis*0.2f;
					}
					m_nConverselyTimeTemp = m_nConverselyTime;
					szNew.fLength = m_ConverselyImpactSize.fLength;
					szNew.fWidth = m_ConverselyImpactSize.fWidth;
					szNew.fHeight = m_ConverselyImpactSize.fHeight;
					setSize3D(szNew);
					GetCCArmature()->getAnimation()->play(_NF_ENEMY_FALL_);
				}else{
					if(m_nConverselyTimeTemp>0){
						GetCCArmature()->getAnimation()->play(_NF_ENEMY_FALL_);
						m_nConverselyTimeTemp = m_nConverselyTime*(1 - (m_fPerseverance/(m_fPerseverance + _NF_PERSEVERANCE_COEFFICIENT_))*0.7f);

						szNew.fLength = m_ConverselyImpactSize.fLength;
						szNew.fWidth = m_ConverselyImpactSize.fWidth;
						szNew.fHeight = m_ConverselyImpactSize.fHeight;
						setSize3D(szNew);

						tTakeABeating.fZSpeed = tTakeABeating.fZSpeed*0.2f;
						tTakeABeating.fXTime = tTakeABeating.fXTime*0.2f;
						tTakeABeating.nXDis = tTakeABeating.nXDis*0.2f;
						tTakeABeating.fYTime = tTakeABeating.fYTime*0.2f;
						tTakeABeating.nYDis = tTakeABeating.nYDis*0.2f;
					}else{
						GetCCArmature()->getAnimation()->play(_NF_ENEMY_HIT_);
						m_nCrickTimeTemp = (m_nCrickTime + tTakeABeating.nCrickAdd)*(1 - (m_fPerseverance/(m_fPerseverance + _NF_PERSEVERANCE_COEFFICIENT_))*0.7f);
						szNew.fLength = m_HoldImpactSize.fLength;
						szNew.fWidth = m_HoldImpactSize.fWidth;
						szNew.fHeight = m_HoldImpactSize.fHeight;
						setSize3D(szNew);
					}
				}
				SetZSpeed(tTakeABeating.fZSpeed);
			}
		}
		

		vec3D.fX = tTakeABeating.nXDis;
		vec3D.fY = 0;
		vec3D.fZ = 0;
		MoveBy3D(tTakeABeating.fXTime,vec3D);

		vec3D.fX = 0;
		vec3D.fY = tTakeABeating.nYDis;
		vec3D.fZ = 0;
		MoveBy3D(tTakeABeating.fYTime,vec3D);

		for (CNFBuffInfoVec::iterator iter = tTakeABeating.vecBuff.begin();iter!=tTakeABeating.vecBuff.end();iter++)
		{
			switch (iter->nBuffID)
			{
			case _NF_BUFF_TREATFORMULA_ID:		//治疗
				{
					TreatFormula(iter->fBuffPoint1);
				}break;
			case _NF_BUFF_SIPERARMOR_ID:		//霸体
				{
					AddSuperArmorBuff(iter->nBuffTime);
				}break;
			case _NF_BUFF_ASTHENIA_ID:			//衰弱
				{
					AddAstheniaBuff(iter->nBuffTime);
				}break;
			case _NF_BUFF_ATTACKSPEEDCUT_ID:	//缓慢
				{
					AddAttackSpeedCutBuff(iter->nBuffTime,iter->fBuffPoint1);
				}break;
			case _NF_BUFF_MOVESPEEDCUT_ID:		//减速
				{
					AddMoveSpeedCutBuff(iter->nBuffTime,iter->fBuffPoint1);
				}break;
			case _NF_BUFF_POISONING_ID:			//中毒
				{
					AddPoisoningBuff(iter->nBuffTime,iter->fBuffPoint1);
				}break;
			case _NF_BUFF_BFB_TREATFORMULA_ID:	//百分比治疗
				{
					BFBTreatFormula(iter->fBuffPoint1);
				}break;
			case _NF_BUFF_SINGLE_ATTACKUP_ID:	//增加单体攻击力
				{
					AddAttackUpSBuff(iter->nBuffTime,iter->fBuffPoint1);
				}break;
			case _NF_BUFF_ATTACKUP_ID:			//增加群体攻击力
				{
					AddAttackUpBuff(iter->nBuffTime,iter->fBuffPoint1);
				}break;
			case _NF_BUFF_S_SIPERARMOR_ID:		//群体霸体
				{
					AddSuperArmorBuff(iter->nBuffTime);
				}break;
			case _NF_BUFF_S_MP_RECOVER_ID:		//群体百分比恢复MP
				{
					BFBRecoverMP(iter->fBuffPoint1);
				}break;
			}
		}

		float	fTempDamagePoint = tTakeABeating.fDamagePoint;
		int		nTempDisplay = enDisplayHPLableCommon;
		if(rand()%1000 < tTakeABeating.nCrit){
			nTempDisplay = enDisplayHPLableCrit;
			fTempDamagePoint = fTempDamagePoint*tTakeABeating.fCritDamage;
		}
		fTempDamagePoint = fTempDamagePoint*(1 - (m_fDefense/(_NF_DEFENSE_COEFFICIENT_ + m_fDefense))*0.9f);

		int zAttack = 0;
		if(fTempDamagePoint>10){
			zAttack =  rand()%((int)(fTempDamagePoint*0.2)) - ((int)(fTempDamagePoint*0.1));
		}

		fTempDamagePoint = (int)(fTempDamagePoint + zAttack);

		if(tTakeABeating.bIsDamage==true){
			m_fHP2Temp = m_fHPTemp;
			m_fHPTemp -= fTempDamagePoint;
			if(m_fHPTemp<=0){
				m_fHPTemp = 0;
				m_bIsDeath = true;
				//OnDead();
			}
			displayHPLable(fTempDamagePoint,nTempDisplay);
			displayHitLable(tTakeABeating.nHitEffectType);
			return fTempDamagePoint;
		}
		return 0;
	}else{
		displayHPLable(0,enDisplayHPLableMiss);
		return 0;
	}
}

float CNFEnemy::TreatFormula( float fTreatPoint )
{
	//如果死亡停止
	if(m_bIsDeath){return 0;}

	fTreatPoint = fTreatPoint*m_fTreatCoefficient;
	if(fTreatPoint<0){
		fTreatPoint = 0;
	}
	m_fHPTemp += fTreatPoint;
	if(m_fHPTemp>m_fHP){
		m_fHPTemp = m_fHP;
	}
	displayHPLable(fTreatPoint,enDisplayHPLableTreat);
	return fTreatPoint;
}

float CNFEnemy::BFBTreatFormula( float fBFBTreatPoint )
{
	//如果死亡停止
	if(m_bIsDeath){return 0;}

	float fTreatPoint = m_fHP*fBFBTreatPoint;
	fTreatPoint = fTreatPoint*m_fTreatCoefficient;
	if(fTreatPoint<0){
		fTreatPoint = 0;
	}
	m_fHPTemp += fTreatPoint;
	if(m_fHPTemp>m_fHP){
		m_fHPTemp = m_fHP;
	}
	displayHPLable(fTreatPoint,enDisplayHPLableTreat);
	return fTreatPoint;
}

float CNFEnemy::BFBRecoverMP( float fBFBMPPoint )
{
	//如果死亡停止
	if(m_bIsDeath){return 0;}

	float fTreatPoint = m_fMP*fBFBMPPoint;
	fTreatPoint = fTreatPoint*m_fTreatCoefficient;
	if(fTreatPoint<0){
		fTreatPoint = 0;
	}
	m_fMPTemp += fTreatPoint;
	if(m_fMPTemp>m_fMP){
		m_fMPTemp = m_fMP;
	}
	displayHPLable(fTreatPoint,enDisplayMPLableRecover);
	return fTreatPoint;
}

void CNFEnemy::OnHold()
{
	tagSize3D szNew;
	if(m_nGround + m_fSubtractSpeedl>=m_Cube3D.ptPos3D.fZ&&m_nConverselyTimeTemp<=0&&m_nCrickTimeTemp<=0&&m_fMoveRate<0&&m_bIsActionRuning==false){
		szNew.fLength = m_HoldImpactSize.fLength;
		szNew.fWidth = m_HoldImpactSize.fWidth;
		szNew.fHeight = m_HoldImpactSize.fHeight;
		setSize3D(szNew);
		if(m_bIsHoldAnimation==false){
			GetCCArmature()->getAnimation()->play(_NF_ENEMY_HOLD_);
		}
		GetCCArmature()->getAnimation()->setSpeedScale(1);
	}
}

bool CNFEnemy::OnCtrlHold()
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_Cube3D.ptPos3D.fZ>m_nGround + m_fSubtractSpeedl){
		return false;
	}else{
		if(m_bIsDeath||m_bIsActionRuning){return false;}
		//OnHold();
		return true;
	}
}

bool CNFEnemy::OnCtrlCrosswiseApproach(bool bIsApproach)
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_Cube3D.ptPos3D.fZ>m_nGround + m_fSubtractSpeedl){
		return false;
	}else{
		if(m_bIsDeath||m_bIsActionRuning){return false;}

		int Ztime = (int)((_NF_ABS(GetCurrentCube()->ptPos3D.fX - m_pTarget->GetCurrentCube()->ptPos3D.fX)/m_fMoveSpeedTemp)*0.8f);
		if(Ztime<=0){
			Ztime	=	1;
		}
		m_nMoveTimeTemp = rand()%Ztime + 5;
		m_bIsActionRuning = true;
		GetCCArmature()->getAnimation()->play(_NF_ENEMY_RUN_);
		float fMoveRate = 270;
		if(GetCurrentCube()->ptPos3D.fX - m_pTarget->GetCurrentCube()->ptPos3D.fX > 0){
			if(bIsApproach){
				fMoveRate = 270;
			}else{
				fMoveRate = 90;
			}
		}else{
			if(bIsApproach){
				fMoveRate = 90;
			}else{
				fMoveRate = 270;
			}
		}
		m_fMoveRate = fMoveRate;

		return true;
	}
}

bool CNFEnemy::OnCtrlLengthwaysApproach(bool bIsApproach)
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_Cube3D.ptPos3D.fZ>m_nGround + m_fSubtractSpeedl){
		return false;
	}else{
		if(m_bIsDeath||m_bIsActionRuning){return false;}

		int Ztime = (int)((_NF_ABS(GetCurrentCube()->ptPos3D.fY - m_pTarget->GetCurrentCube()->ptPos3D.fY)/m_fMoveSpeedTemp)*0.8f);
		if(Ztime<=0){
			Ztime	=	1;
		}
		m_nMoveTimeTemp = rand()%Ztime + 5;
		m_bIsActionRuning = true;
		GetCCArmature()->getAnimation()->play(_NF_ENEMY_RUN_);
		float fMoveRate = 180;
		if(GetCurrentCube()->ptPos3D.fY - m_pTarget->GetCurrentCube()->ptPos3D.fY > 0){
			if(bIsApproach){
				fMoveRate = 180;
			}else{
				fMoveRate = 0;
			}
		}else{
			if(bIsApproach){
				fMoveRate = 0;
			}else{
				fMoveRate = 180;
			}
		}
		m_fMoveRate = fMoveRate;

		return true;
	}
}

bool CNFEnemy::OnCtrlApproach()
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_Cube3D.ptPos3D.fZ>m_nGround + m_fSubtractSpeedl){
		return false;
	}else{
		if(m_bIsDeath||m_bIsActionRuning){return false;}

		float o = m_pTarget->GetCurrentCube()->ptPos3D.fX - GetCurrentCube()->ptPos3D.fX;
		float a = m_pTarget->GetCurrentCube()->ptPos3D.fY - GetCurrentCube()->ptPos3D.fY;
		float at = (float)CC_RADIANS_TO_DEGREES( atanf(o/a) );
		if(a<0){
			at += 180.f;
		}else if(o<0){
			at += 360.f;
		}
		float fMoveRate = at;

		float fLen = sqrt(pow(o,2)+pow(a,2));
		int Ztime = (int)((fLen/m_fMoveSpeedTemp)*0.7f);
		if(Ztime<=0){
			Ztime	=	1;
		}
		m_nMoveTimeTemp = rand()%Ztime + 5;
		m_bIsActionRuning = true;
		GetCCArmature()->getAnimation()->play(_NF_ENEMY_RUN_);

		m_fMoveRate = fMoveRate;

		return true;
	}
}

bool CNFEnemy::OnCtrlCircuity()
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_Cube3D.ptPos3D.fZ>m_nGround + m_fSubtractSpeedl){
		return false;
	}else{
		if(m_bIsDeath||m_bIsActionRuning){return false;}
		int Ztime = (int)(((_NF_ABS(GetCurrentCube()->ptPos3D.fX - m_pTarget->GetCurrentCube()->ptPos3D.fX) + m_pTarget->GetCurrentCube()->szCube.fLength*0.5f + GetCurrentCube()->szCube.fLength*0.5f)/m_fMoveSpeedTemp));
		if(Ztime<=0){
			Ztime	=	1;
		}
		m_nMoveTimeTemp = Ztime + rand()%10;
		m_bIsActionRuning = true;
		GetCCArmature()->getAnimation()->play(_NF_ENEMY_RUN_);
		float fMoveRate = 270;
		if(GetCurrentCube()->ptPos3D.fX - m_pTarget->GetCurrentCube()->ptPos3D.fX > 0){
			fMoveRate = 270;
		}else{
			fMoveRate = 90;
		}
		m_fMoveRate = fMoveRate;

		return true;
	}
}

enumEnemyAICondition CNFEnemy::AI_getCurrnetCondition()
{
	if(_NF_ABS(GetCurrentCube()->ptPos3D.fY - m_pTarget->GetCurrentCube()->ptPos3D.fY) <= m_pTarget->GetCurrentCube()->szCube.fWidth*0.5f){
		float o = m_pTarget->GetCurrentCube()->ptPos3D.fX - GetCurrentCube()->ptPos3D.fX;
		float a = m_pTarget->GetCurrentCube()->ptPos3D.fY - GetCurrentCube()->ptPos3D.fY;
		float fLen = sqrt(pow(o,2)+pow(a,2));
		if(fLen<m_AINearDis){
			return enEnemyCondition_SameCrosswise_InRangeShort;
		}else if(fLen>=m_AINearDis&&fLen<m_AIFarDis){
			return enEnemyCondition_SameCrosswise_InRangeLong;
		}else{
			return enEnemyCondition_SameCrosswise_NotInRange;
		}
	}else{
		float o = m_pTarget->GetCurrentCube()->ptPos3D.fX - GetCurrentCube()->ptPos3D.fX;
		float a = m_pTarget->GetCurrentCube()->ptPos3D.fY - GetCurrentCube()->ptPos3D.fY;
		float fLen = sqrt(pow(o,2)+pow(a,2));
		if(fLen<m_AINearDis){
			return enEnemyCondition_NotSameCrosswiss_Near;
		}else{
			if(o>=0){
				if(m_pTarget->GetDirection()==enTagDirectionR){
					return enEnemyCondition_NotSameCrosswiss_Far_NotFacePlayer;
				}else{
					return enEnemyCondition_NotSameCrosswiss_Far_FacePlayer;
				}
			}else{
				if(m_pTarget->GetDirection()==enTagDirectionR){
					return enEnemyCondition_NotSameCrosswiss_Far_FacePlayer;
				}else{
					return enEnemyCondition_NotSameCrosswiss_Far_NotFacePlayer;
				}
			}
		}
	}
}

enumEnemyAIKind CNFEnemy::AI_getCurrentAICmdKind( enumEnemyAICondition enCondition )
{
	int nTotalWeight = 0;
	CMapAIActionOnKind it = m_MapAI[enCondition];
	for (CMapAIActionOnKind::iterator itWeight = it.begin();itWeight != it.end();itWeight++)
	{
		nTotalWeight+=itWeight->second;
	}
	int nRandIndex = rand()%nTotalWeight;

	for (CMapAIActionOnKind::iterator itWeight2 = it.begin();itWeight2 != it.end();itWeight2++)
	{
		if(nRandIndex > itWeight2->second){
			nRandIndex -= itWeight2->second;
		}else{
			return itWeight2->first;
		}
	}
	return enEnemyAttackTypeLong;
}


bool CNFEnemy::OnCtrlAttack1()
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_Skill_1_SkillItem.nSkill_CD_Time_Temp>0||m_Cube3D.ptPos3D.fZ>m_nGround + m_fSubtractSpeedl){
		return false;
	}else{
		if(m_bIsDeath||m_bIsActionRuning){return false;}

		m_Skill_1_SkillItem.nSkill_CD_Time_Temp = m_Skill_1_SkillItem.nSkill_CD_Time;
		m_bIsActionRuning = true;
		GetCCArmature()->getAnimation()->setSpeedScale(m_fAttackSpeedTemp*0.01f);
		GetCCArmature()->getAnimation()->play("attack_pre_short");
		return true;
	}
}

bool CNFEnemy::OnCtrlAttack2()
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_Skill_2_SkillItem.nSkill_CD_Time_Temp>0||m_Cube3D.ptPos3D.fZ>m_nGround + m_fSubtractSpeedl){
		return false;
	}else{
		if(m_bIsDeath||m_bIsActionRuning){return false;}

		m_Skill_2_SkillItem.nSkill_CD_Time_Temp = m_Skill_2_SkillItem.nSkill_CD_Time;
		m_bIsActionRuning = true;
		GetCCArmature()->getAnimation()->setSpeedScale(m_fAttackSpeedTemp*0.01f);
		GetCCArmature()->getAnimation()->play("attack_pre_long");
		return true;
	}
}

bool CNFEnemy::OnCtrlAttack3()
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_Skill_3_SkillItem.nSkill_CD_Time_Temp>0||m_Cube3D.ptPos3D.fZ>m_nGround + m_fSubtractSpeedl){
		return false;
	}else{
		if(m_bIsDeath||m_bIsActionRuning){return false;}

		m_Skill_3_SkillItem.nSkill_CD_Time_Temp = m_Skill_3_SkillItem.nSkill_CD_Time;
		m_bIsActionRuning = true;
		GetCCArmature()->getAnimation()->setSpeedScale(m_fAttackSpeedTemp*0.01f);
		GetCCArmature()->getAnimation()->play(m_Skill_3_SkillItem.strSkill_AniName.c_str());
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nEnemyID,m_Skill_3_SkillItem.nSkill_SkillItemID,m_StaticSkillInfo);
		m_Skill_3_SkillItem.nSkill_CD_Time_Temp = m_Skill_3_SkillItem.nSkill_CD_Time;
		
		m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
		//遍历所有技能快依次延时
		for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
		{
			runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFEnemy::OnCtrlSkillCallBack)),NULL));
		}
		return true;
	}
}

bool CNFEnemy::OnCtrlAttack4()
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_Skill_4_SkillItem.nSkill_CD_Time_Temp>0||m_Cube3D.ptPos3D.fZ>m_nGround + m_fSubtractSpeedl){
		return false;
	}else{
		if(m_bIsDeath||m_bIsActionRuning){return false;}

		m_Skill_4_SkillItem.nSkill_CD_Time_Temp = m_Skill_4_SkillItem.nSkill_CD_Time;
		m_bIsActionRuning = true;
		GetCCArmature()->getAnimation()->setSpeedScale(m_fAttackSpeedTemp*0.01f);
		GetCCArmature()->getAnimation()->play(m_Skill_4_SkillItem.strSkill_AniName.c_str());
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nEnemyID,m_Skill_4_SkillItem.nSkill_SkillItemID,m_StaticSkillInfo);
		m_Skill_4_SkillItem.nSkill_CD_Time_Temp = m_Skill_4_SkillItem.nSkill_CD_Time;

		m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
		//遍历所有技能快依次延时
		for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
		{
			runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFEnemy::OnCtrlSkillCallBack)),NULL));
		}
		return true;
	}
}

void CNFEnemy::OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID )
{
	if(movementType==START){
		if(strcmp(movementID,_NF_ENEMY_HOLD_)==0){
			m_bIsHoldAnimation = true;
		}else{
			m_bIsHoldAnimation = false;
		}
	}
	if(movementType==COMPLETE){
		if(strcmp(movementID,_NF_ENEMY_DEAD_)==0){
			tagRemovePerson Cmd;
			Cmd.pPerson = this;
			if(SendMsg(enMsg_RemoveEnemy,&Cmd,sizeof(Cmd))==false)	 
			{
				CCLog("CNFEnemy::OnDead Error!");
			}
		}else
		if(strcmp(movementID,"attack_aft_short")==0||strcmp(movementID,"attack_aft_long")==0||strcmp(movementID,m_Skill_3_SkillItem.strSkill_AniName.c_str())==0||strcmp(movementID,m_Skill_4_SkillItem.strSkill_AniName.c_str())==0){
			m_bIsActionRuning = false;
		}

		if(strcmp(movementID,"attack_pre_short")==0){
			//敌人普通攻击1
			GetCCArmature()->getAnimation()->play("attack_aft_short");
			CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nEnemyID,m_Skill_1_SkillItem.nSkill_SkillItemID,m_StaticSkillInfo);
			m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
			//遍历所有技能快依次延时
			for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
			{
				runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFEnemy::OnCtrlSkillCallBack)),NULL));
			}
		}else if(strcmp(movementID,"attack_pre_long")==0){
			//敌人普通攻击2
			GetCCArmature()->getAnimation()->play("attack_aft_long");
			CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nEnemyID,m_Skill_2_SkillItem.nSkill_SkillItemID,m_StaticSkillInfo);
			m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
			//遍历所有技能快依次延时
			for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
			{
				runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFEnemy::OnCtrlSkillCallBack)),NULL));
			}
		}
		
	}
	if(movementType==LOOP_COMPLETE){
		//CCLog("=============================s===%s",movementID);
	}
}

void CNFEnemy::OnCtrlSkillCallBack()
{
	int SkillLV = 1;
	//得到对应技能快信息，并发送消息释放技能
	tagSkillItemStaticInfo pInfoNN = *m_SkillItem;
	tagCommonAttack Cmd(this,&pInfoNN,SkillLV);

	tagVector3D vec3D;
	vec3D.fX = pInfoNN.fDisX*GetDirection();
	vec3D.fY = 0;
	vec3D.fZ = 0;
	MoveBy3D(pInfoNN.fTimeX,vec3D);
	vec3D.fX = 0;
	vec3D.fY = pInfoNN.fDisY;
	vec3D.fZ = 0;
	MoveBy3D(pInfoNN.fTimeY,vec3D);

	//添加buff
	for (CNFBuffStaticInfoVec::iterator iter = pInfoNN.vecBuff.begin();iter!=pInfoNN.vecBuff.end();iter++)
	{
		switch (iter->nBuffID)
		{
		case _NF_BUFF_TREATFORMULA_ID:		//治疗
			{
				TreatFormula(iter->fBuffPoint1 + iter->fBuffPoint1_g*SkillLV);
			}break;
		case _NF_BUFF_SIPERARMOR_ID:		//霸体
			{
				AddSuperArmorBuff(iter->nBuffTime + iter->nBuffTime_g*SkillLV);
			}break;
		case _NF_BUFF_ASTHENIA_ID:			//衰弱
			{
				AddAstheniaBuff(iter->nBuffTime + iter->nBuffTime_g*SkillLV);
			}break;
		case _NF_BUFF_ATTACKSPEEDCUT_ID:	//缓慢
			{
				AddAttackSpeedCutBuff(iter->nBuffTime + iter->nBuffTime_g*SkillLV,iter->fBuffPoint1 + iter->fBuffPoint1_g*SkillLV);
			}break;
		case _NF_BUFF_MOVESPEEDCUT_ID:		//减速
			{
				AddMoveSpeedCutBuff(iter->nBuffTime + iter->nBuffTime_g*SkillLV,iter->fBuffPoint1 + iter->fBuffPoint1_g*SkillLV);
			}break;
		case _NF_BUFF_POISONING_ID:			//中毒
			{
				AddPoisoningBuff(iter->nBuffTime + iter->nBuffTime_g*SkillLV,iter->fBuffPoint1 + iter->fBuffPoint1_g*SkillLV);
			}break;
		case _NF_BUFF_BFB_TREATFORMULA_ID:	//百分比治疗
			{
				CCObject *pItem = NULL;
				CCARRAY_FOREACH(m_pArrayFriend,pItem)
				{
					CNFBasicPerson *pNodeItem = dynamic_cast<CNFBasicPerson*>(pItem);
					if(pNodeItem!=NULL)
					{
						pNodeItem->BFBTreatFormula(iter->fBuffPoint1 + iter->fBuffPoint1_g*SkillLV);
					}
				}
			}break;
		case _NF_BUFF_SINGLE_ATTACKUP_ID:	//增加单体攻击力
			{
				AddAttackUpSBuff(iter->nBuffTime + iter->nBuffTime_g*SkillLV,iter->fBuffPoint1 + iter->fBuffPoint1_g*SkillLV);
			}break;
		case _NF_BUFF_ATTACKUP_ID:				//增加群体攻击力
			{
				CCObject *pItem = NULL;
				CCARRAY_FOREACH(m_pArrayFriend,pItem)
				{
					CNFBasicPerson *pNodeItem = dynamic_cast<CNFBasicPerson*>(pItem);
					if(pNodeItem!=NULL)
					{
						pNodeItem->AddAttackUpBuff(iter->nBuffTime + iter->nBuffTime_g*SkillLV,iter->fBuffPoint1 + iter->fBuffPoint1_g*SkillLV);
					}
				}
			}break;
		case _NF_BUFF_S_SIPERARMOR_ID:				//群体霸体
			{
				CCObject *pItem = NULL;
				CCARRAY_FOREACH(m_pArrayFriend,pItem)
				{
					CNFBasicPerson *pNodeItem = dynamic_cast<CNFBasicPerson*>(pItem);
					if(pNodeItem!=NULL)
					{
						pNodeItem->AddSuperArmorBuff(iter->nBuffTime + iter->nBuffTime_g*SkillLV);
					}
				}
			}break;
		case _NF_BUFF_S_MP_RECOVER_ID:				//群体百分比恢复MP
			{
				CCObject *pItem = NULL;
				CCARRAY_FOREACH(m_pArrayFriend,pItem)
				{
					CNFBasicPerson *pNodeItem = dynamic_cast<CNFBasicPerson*>(pItem);
					if(pNodeItem!=NULL)
					{
						pNodeItem->BFBRecoverMP(iter->fBuffPoint1 + iter->fBuffPoint1_g*SkillLV);
					}
				}
			}break;
		}
	}

	//是否有屏幕震动
	if(pInfoNN.nScreenIsShake>0){
		getLTLayerParent()->WorldEffectTremble(pInfoNN.fShakeDurTime,pInfoNN.fShakeDegree,pInfoNN.nShakeCount);
	}

	//是否屏幕变颜色
	if (pInfoNN.nIsScreenChangeColor>0){
		getLTLayerParent()->SetScreenColor(pInfoNN.nIsScreenChangeColor,pInfoNN.fScreenChangeColorTime);
	}

	//是否有镜头拉伸
	if(pInfoNN.nIsLensfeatrue>0){
		getLTLayerParent()->LensFeature(pInfoNN.fLensfeatureTime,this);
	}

	//是否使释放者转头
	if(pInfoNN.nIsTurnRound>0){
		SetDirectionEnemy(GetDirection()*-1);
	}

	if(SendMsg(enMsgEnemyCommonAttack,&Cmd,sizeof(Cmd))==false)
	{
		CCLog("OnCtrlSkill1 Error!");
	}
	m_SkillItem++;
}

bool CNFEnemy::IsSkillNOCD( int nSkillID )
{
	if(nSkillID<=0){
		return false;
	}
	if(nSkillID==m_Skill_3_SkillItem.nSkill_SkillItemID){
		if(m_Skill_3_SkillItem.nSkill_CD_Time_Temp<=0){
			return true;
		}
	}
	if(nSkillID==m_Skill_4_SkillItem.nSkill_SkillItemID){
		if(m_Skill_4_SkillItem.nSkill_CD_Time_Temp<=0){
			return true;
		}
	}
	return false;
}

bool CNFEnemy::IsSkillAttactkable( int nSkillID )
{
	tagSkillStaticInfo tempStaticSkillInfo;
	CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nEnemyID,nSkillID,tempStaticSkillInfo);
	CNFSkillItemVec::iterator iter = tempStaticSkillInfo.vecSkillItem.begin();
	if(iter!=tempStaticSkillInfo.vecSkillItem.end()){
		if(m_pTarget!=NULL){
			float tMinX = m_pTarget->GetCurrentCube()->ptPos3D.fX - m_pTarget->GetCurrentCube()->szCube.fLength/2;
			float tMinY = m_pTarget->GetCurrentCube()->ptPos3D.fY - m_pTarget->GetCurrentCube()->szCube.fWidth/2;
			float tMaxX = m_pTarget->GetCurrentCube()->ptPos3D.fX + m_pTarget->GetCurrentCube()->szCube.fLength/2;
			float tMaxY = m_pTarget->GetCurrentCube()->ptPos3D.fY + m_pTarget->GetCurrentCube()->szCube.fWidth/2;

			float mMinX = GetCurrentCube()->ptPos3D.fX + (iter->tSkillChange.fNewX)*GetDirection() - iter->tSkillChange.fNewLength/2;
			float mMinY = GetCurrentCube()->ptPos3D.fY + iter->tSkillChange.fNewY - iter->tSkillChange.fNewWidth/2;
			float mMaxX = GetCurrentCube()->ptPos3D.fX + (iter->tSkillChange.fNewX)*GetDirection() + iter->tSkillChange.fNewLength/2;
			float mMaxY = GetCurrentCube()->ptPos3D.fY + iter->tSkillChange.fNewY + iter->tSkillChange.fNewWidth/2;

			//若碰撞
			if(!(mMaxX < tMinX ||tMaxX < mMinX ||mMaxY < tMinY ||tMaxY < mMinY))
			{
				return true;
			}
			
		}
	}

	return false;
}













