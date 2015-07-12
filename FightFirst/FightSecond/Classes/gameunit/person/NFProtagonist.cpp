#include "NFProtagonist.h"
#include "../article/NFBasicArticle.h"
#include "../../datamanager/NFDataManager.h"
#include "../../datamanager/NFServerDataManager.h"

//Animation名称
#define _NF_POWER1_HOLD_		"hold"
#define _NF_POWER1_WALK_		"run"
#define _NF_POWER1_RUN_			"run"
#define _NF_POWER1_HIT_			"hit"
#define _NF_POWER1_JUMP_		"jump"
#define _NF_POWER1_FALL_		"dead"
#define _NF_POWER1_DEAD_		"dead"

CNFProtagonist* CNFProtagonist::Create(int nID,CCArray *pArrayFriend,CCArray *pArrayTarget)
{
	do
	{
		CNFProtagonist * pSpr = new  CNFProtagonist();
		CC_BREAK_IF(pSpr==NULL);
		CC_BREAK_IF(pSpr->Init(nID,pArrayFriend,pArrayTarget)==false);
		pSpr->autorelease();
		return pSpr;
	} while (false);
	CCLog("Fun CNFProtagonist::Create Error!");
	return NULL;
}

bool CNFProtagonist::Init(int nID,CCArray *pArrayFriend,CCArray *pArrayTarget)
{
	do
	{
		m_pArrayFriend = pArrayFriend;
		m_pArrayTarget = pArrayTarget;

		m_nLV = 1;
		m_nRoleID = nID;
		//m_nRoleType = enRoleType_Power;


		tagProtagonistStaticInfo ProInfo;
		CNFStaticDataManager::SharedData()->GetProtagonistStaticInfo(m_nRoleID,ProInfo);

		m_HoldImpactSize.fLength = ProInfo.fLength_Hold;
		m_HoldImpactSize.fWidth = ProInfo.fWidth_Hold;
		m_HoldImpactSize.fHeight = ProInfo.fHeight_Hold;
		m_MoveImpactSize.fLength = ProInfo.fLength_Move;
		m_MoveImpactSize.fWidth = ProInfo.fWidth_Move;
		m_MoveImpactSize.fHeight = ProInfo.fHeight_Move;
		m_RunImpactSize.fLength = 60;
		m_RunImpactSize.fWidth = 50;
		m_RunImpactSize.fHeight = 80;
		m_ConverselyImpactSize.fLength = ProInfo.fLength_Conversely;
		m_ConverselyImpactSize.fWidth = ProInfo.fWidth_Conversely;
		m_ConverselyImpactSize.fHeight = ProInfo.fHeight_Conversely;

		CC_BREAK_IF(CNFBasicPerson::init(ProInfo.strArmatureName.c_str(),0.4f,m_HoldImpactSize)==false);

		m_nExistTime = 0;
		m_fJumpPower = 20;

		m_enCommonAttackStatus = enStatusCommonAttack_1;
		
		m_fMoveSpeed = ProInfo.fMoveSpeed;
		m_fMoveRunSpeed = 7;
		m_fMoveSpeedTemp = m_fMoveSpeed;

		m_nCommonAttackCDTemp = 0;
		m_nCommonAttackCD = _NF_HIT_CD_;

		m_nCrickTime = ProInfo.nCrickTime;
		m_nCrickTimeTemp = 0;

		m_nConverselyTime = ProInfo.nConverselyTime;
		m_nConverselyTimeTemp = 0;

		m_nMoveTimeTemp = 0;

		m_bIsAttacking = false;

		m_bIsMoveAnimation = false;
		m_bIsRunAnimation = false;
		m_bIsHoldAnimation = false;

		

		//m_fPower = ProInfo.fPower + m_nLV*ProInfo.fPower_g;
		//m_fWit = ProInfo.fWit + m_nLV*ProInfo.fWit_g;
		//m_fAgility = ProInfo.fAgility + m_nLV*ProInfo.fAgility_g;
		m_fHP = ProInfo.fHP/* + m_fPower*_NF_POWER_HP_*/;
		m_fMP = ProInfo.fMP/* + m_fWit*_NF_WIT_MP_*/;
		m_fHPTemp = m_fHP;
		m_fHP2Temp = m_fHP;
		m_fMP = ProInfo.fMP/* + m_fWit*_NF_WIT_MP_*/;
		m_fMPTemp = m_fMP;
		m_fDefense = ProInfo.fDefense;
		m_fRefillHP = 0.f;
		m_fRefillMP = 0.f;
		//根据角色的类型，计算对应的攻击力和掌控力
		//if(m_nRoleType==enRoleType_Power)
		//{
			m_fAttack = ProInfo.fAttack/* + m_fPower*_NF_ATTACK_UP_RATE_POWER_*/;
			m_fDomination = ProInfo.fDomination/* + m_fPower*_NF_DOMINATION_UP_RATE_POWER_*/;
		//}
		//else if(m_nRoleType==enRoleType_Wit)
		//{
		//	m_fAttack = ProInfo.fAttack/* + m_fWit*_NF_ATTACK_UP_RATE_WIT_*/;
		//	m_fDomination = ProInfo.fDomination/* + m_fWit*_NF_DOMINATION_UP_RATE_WIT_*/;
		//}
		//else if(m_nRoleType==enRoleType_Agility)
		//{
		//	m_fAttack = ProInfo.fAttack/* + m_fAgility*_NF_ATTACK_UP_RATE_AGILITY_*/;
		//	m_fDomination = ProInfo.fDomination/* + m_fAgility*_NF_DOMINATION_UP_RATE_AGILITY_*/;
		//}
		
		m_fAttackSpeed = ProInfo.fAttackSpeed;
		m_nCrit = ProInfo.nCrit;
		m_fCritDamage = ProInfo.fCritDamage;
		m_nHitTarget = ProInfo.nHitTarget;
		m_nAvoid = ProInfo.nAvoid;
		m_fTreatCoefficient = ProInfo.fTreatCoefficient;

		m_nCommonAttackNum = ProInfo.nCommonAttackNum;

		m_nCommonAttack_1_SkillItemID = 1;
		m_nCommonAttack_2_SkillItemID = 2;
		m_nCommonAttack_3_SkillItemID = 3;
		m_nCommonAttack_4_SkillItemID = 4;

		//默认技能等级
		int	nSkill_1_LV = 1;
		int	nSkill_2_LV = 1;
		int	nSkill_3_LV = 1;
		int	nSkill_4_LV = 1;
		int	nSkill_5_LV = 1;

		m_Skill_1_SkillItem.nSkill_SkillItemID = -1;
		m_Skill_2_SkillItem.nSkill_SkillItemID = -1;
		m_Skill_3_SkillItem.nSkill_SkillItemID = -1;
		m_Skill_4_SkillItem.nSkill_SkillItemID = -1;
		m_Skill_5_SkillItem.nSkill_SkillItemID = -1;

		
		CNFServerSkillInfoVec tempSkillInfoMap;
		CNFServerDataManager::SharedData()->GetSkillInfo(m_nRoleID,tempSkillInfoMap);

		for (CNFServerSkillInfoVec::iterator iter = tempSkillInfoMap.begin(); iter != tempSkillInfoMap.end();iter++)
		{
			if(iter->nPos==1){
				m_Skill_1_SkillItem.nSkill_SkillItemID = iter->nSkillID;
				nSkill_1_LV = iter->nSkillLevel;
			}else if(iter->nPos==2){
				m_Skill_2_SkillItem.nSkill_SkillItemID = iter->nSkillID;
				nSkill_2_LV = iter->nSkillLevel;
			}else if(iter->nPos==3){
				m_Skill_3_SkillItem.nSkill_SkillItemID = iter->nSkillID;
				nSkill_3_LV = iter->nSkillLevel;
			}else if(iter->nPos==4){
				m_Skill_4_SkillItem.nSkill_SkillItemID = iter->nSkillID;
				nSkill_4_LV = iter->nSkillLevel;
			}else if(iter->nPos==5){
				m_Skill_5_SkillItem.nSkill_SkillItemID = iter->nSkillID;
				nSkill_5_LV = iter->nSkillLevel;
			}
		}
		

		tagSkillStaticInfo SkillInfo1;
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_1_SkillItem.nSkill_SkillItemID,SkillInfo1);
		m_Skill_1_SkillItem.nSkill_CD_Time = SkillInfo1.nSkillCD + nSkill_1_LV*SkillInfo1.nSkillCD_g;		

		tagSkillStaticInfo SkillInfo2;
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_2_SkillItem.nSkill_SkillItemID,SkillInfo2);
		m_Skill_2_SkillItem.nSkill_CD_Time = SkillInfo2.nSkillCD + nSkill_2_LV*SkillInfo2.nSkillCD_g;		

		tagSkillStaticInfo SkillInfo3;
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_3_SkillItem.nSkill_SkillItemID,SkillInfo3);
		m_Skill_3_SkillItem.nSkill_CD_Time = SkillInfo3.nSkillCD + nSkill_3_LV*SkillInfo3.nSkillCD_g;		

		tagSkillStaticInfo SkillInfo4;
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_4_SkillItem.nSkill_SkillItemID,SkillInfo4);
		m_Skill_4_SkillItem.nSkill_CD_Time = SkillInfo4.nSkillCD + nSkill_4_LV*SkillInfo4.nSkillCD_g;		

		tagSkillStaticInfo SkillInfo5;
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_5_SkillItem.nSkill_SkillItemID,SkillInfo5);
		m_Skill_5_SkillItem.nSkill_CD_Time = SkillInfo5.nSkillCD + nSkill_5_LV*SkillInfo5.nSkillCD_g;

		m_Skill_1_SkillItem.nSkill_CD_Time_Temp = m_Skill_1_SkillItem.nSkill_CD_Time;
		m_Skill_2_SkillItem.nSkill_CD_Time_Temp = m_Skill_2_SkillItem.nSkill_CD_Time;
		m_Skill_3_SkillItem.nSkill_CD_Time_Temp = m_Skill_3_SkillItem.nSkill_CD_Time;
		m_Skill_4_SkillItem.nSkill_CD_Time_Temp = m_Skill_4_SkillItem.nSkill_CD_Time;
		m_Skill_5_SkillItem.nSkill_CD_Time_Temp = m_Skill_5_SkillItem.nSkill_CD_Time;

		m_Skill_1_SkillItem.strSkill_AniName = SkillInfo1.strAniName;
		m_Skill_2_SkillItem.strSkill_AniName = SkillInfo2.strAniName;
		m_Skill_3_SkillItem.strSkill_AniName = SkillInfo3.strAniName;
		m_Skill_4_SkillItem.strSkill_AniName = SkillInfo4.strAniName;
		m_Skill_5_SkillItem.strSkill_AniName = SkillInfo5.strAniName;

		m_Skill_1_SkillItem.fCostMp = SkillInfo1.fCostMp + nSkill_1_LV*SkillInfo1.fCostMp_g;
		m_Skill_2_SkillItem.fCostMp = SkillInfo2.fCostMp + nSkill_2_LV*SkillInfo2.fCostMp_g;
		m_Skill_3_SkillItem.fCostMp = SkillInfo3.fCostMp + nSkill_3_LV*SkillInfo3.fCostMp_g;
		m_Skill_4_SkillItem.fCostMp = SkillInfo4.fCostMp + nSkill_4_LV*SkillInfo4.fCostMp_g;
		m_Skill_5_SkillItem.fCostMp = SkillInfo5.fCostMp + nSkill_5_LV*SkillInfo5.fCostMp_g;


		//主角AI
		m_tagProtagonistAIInfo = ProInfo.tProtagonistAIInfo;

		m_bIsAutoMode = false;
		m_bIsAutoAttacking = false;
		m_bIsAutoListAttacking = false;
		m_nAutoListTime = 0;


		GetCCArmature()->getAnimation()->setMovementEventCallFunc(this,movementEvent_selector(CNFProtagonist::OnCCArmatureCallBack));
		GetCCArmature()->getAnimation()->play(_NF_POWER1_HOLD_);

		return true;
	} while (false);
	CCLog("Fun CNFProtagonist::Init Error!");
	return false;
}

void CNFProtagonist::CallPerFrame(float fT)
{
	CNFBasicPerson::CallPerFrame(fT);

	//如果死亡停止
	if(m_bIsDeath){OnDead();return;}

	//硬直恢复
	if(m_nCrickTimeTemp>0){
		if(m_nCrickTimeTemp==1&&m_nConverselyTimeTemp<=0){
			m_bIsAttacking = false;
		}
		if(m_nGround + m_fSubtractSpeedl<m_Cube3D.ptPos3D.fZ&&m_nCrickTimeTemp==1){
			
		}else{
			m_nCrickTimeTemp--;
		}
	}
	//倒地恢复
	if(m_nConverselyTimeTemp>0){
		if(m_nConverselyTimeTemp==1){
			m_bIsAttacking = false;
		}
		if(m_nGround + m_fSubtractSpeedl>=m_Cube3D.ptPos3D.fZ){
			m_nConverselyTimeTemp--;
		}
	}

	//普通连续攻击的cd
	if(m_nCommonAttackCDTemp>0){
		if(m_nCommonAttackCDTemp==1){
			m_enCommonAttackStatus = enStatusCommonAttack_1;
		}
		m_nCommonAttackCDTemp--;
	}

	//自动攻击
	if(m_bIsAutoMode==true){
 		if(m_bIsAutoAttacking==false&&m_bIsAutoListAttacking==false){
 			//治疗
 			for (CNFTreatSkillVec::iterator iter = m_tagProtagonistAIInfo.vecTreatSkill.begin();iter!=m_tagProtagonistAIInfo.vecTreatSkill.end();iter++)
 			{
 				if(m_bIsAutoAttacking==true){break;}
 				if(IsHaveSkillIDAndCD(iter->nSkillID)){
 					CCObject *pItem = NULL;
 					CCARRAY_FOREACH(m_pArrayFriend,pItem)
 					{
 						CNFBasicPerson *pNodeItem = dynamic_cast<CNFBasicPerson*>(pItem);
 						if(pNodeItem!=NULL)
 						{
 							if(pNodeItem->GetHPPercent()<iter->fBloodPer){
 								if(OnCtrlSkill(iter->nSkillID)){
 									m_bIsAutoAttacking = true;
 									break;
 								}
 							}
 						}
 					}
 				}
 			}
 		}
		if(m_bIsAutoAttacking==false&&m_bIsAutoListAttacking==false){
			//必然
			for (CNFMustSkillVec::iterator iter = m_tagProtagonistAIInfo.vecMustSkill.begin();iter!=m_tagProtagonistAIInfo.vecMustSkill.end();iter++)
			{
				if(IsHaveSkillIDAndCD(iter->nSkillID)){
					if(OnCtrlSkill(iter->nSkillID)){
						m_bIsAutoAttacking = true;
						break;
					}
				}
			}
		}
		if(m_bIsAutoAttacking==false&&m_bIsAutoListAttacking==false){
			//距离
			for (CNFDistanceSkillVec::iterator iter = m_tagProtagonistAIInfo.vecDistanceSkill.begin();iter!=m_tagProtagonistAIInfo.vecDistanceSkill.end();iter++)
			{
				if(m_bIsAutoAttacking==true){break;}

				CCObject *pItem = NULL;
				CCARRAY_FOREACH(m_pArrayTarget,pItem)
				{
					CNFBasicPerson *pNodeItem = dynamic_cast<CNFBasicPerson*>(pItem);
					if(pNodeItem!=NULL)
					{
						float fLen = _NF_ABS(pNodeItem->GetCurrentCube()->ptPos3D.fX - GetCurrentCube()->ptPos3D.fX);
						float fLenY = _NF_ABS(pNodeItem->GetCurrentCube()->ptPos3D.fY - GetCurrentCube()->ptPos3D.fY);
						if(fLen<=iter->fSkillDistance&&fLenY<25){

							bool bTempIsNoAllCD = false;
							for (CNFDistanceItemVec::iterator iter2 = iter->vecDistanceItem.begin();iter2!=iter->vecDistanceItem.end();iter2++)
							{
								if(iter2->nSkillID==m_nCommonAttack_1_SkillItemID||iter2->nSkillID==m_nCommonAttack_2_SkillItemID||iter2->nSkillID==m_nCommonAttack_3_SkillItemID||iter2->nSkillID==m_nCommonAttack_4_SkillItemID){

								}else{
									if(!IsHaveSkillIDAndCD(iter2->nSkillID)){
										bTempIsNoAllCD = true;
									}
								}
							}
							if(bTempIsNoAllCD==false){
								m_AutoListAttack = iter;

								m_bIsAutoListAttacking = true;
								m_bIsAutoAttacking = true;
								m_nAutoListTime = 0;
								break;
							}
						}
					}
				}


			}

			
			
		}
		if(m_bIsAutoAttacking==false&&m_bIsAutoListAttacking==false){
			//移动
			CCPoint ptPosTemp = ccp(GetCurrentCube()->ptPos3D.fX,GetCurrentCube()->ptPos3D.fY);
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
						ptPosTemp = ccp(pNodeItem->GetCurrentCube()->ptPos3D.fX,pNodeItem->GetCurrentCube()->ptPos3D.fY);
					}
				}
			}
			if(m_nExistTime%40==0){
				OnCtrlMoveToPonit(ptPosTemp);
				m_bIsAutoAttacking = false;
			}
		}

	}else{
		m_bIsAutoAttacking = false;
		m_bIsAutoListAttacking = false;
		m_nAutoListTime = 0;
	}

	if(m_bIsAutoListAttacking==true){
		m_bIsAutoAttacking = false;
		for (CNFDistanceItemVec::iterator iter2 = m_AutoListAttack->vecDistanceItem.begin();iter2!=m_AutoListAttack->vecDistanceItem.end();)
		{

			if(m_nAutoListTime<iter2->nDelayTime){break;}

			if(m_nAutoListTime==iter2->nDelayTime){
				if(iter2->nSkillID==m_nCommonAttack_1_SkillItemID||iter2->nSkillID==m_nCommonAttack_2_SkillItemID||iter2->nSkillID==m_nCommonAttack_3_SkillItemID||iter2->nSkillID==m_nCommonAttack_4_SkillItemID){
					OnCtrlCommonAttack();
				}else{
					OnCtrlSkill(iter2->nSkillID);
				}
			}

			iter2++;
			if(iter2==m_AutoListAttack->vecDistanceItem.end()){
				m_bIsAutoListAttacking = false;
				m_nAutoListTime = 0;
			}
		}
		
		m_nAutoListTime++;
	}


	//自动移动时间
	if(m_nMoveTimeTemp>0){
		if(m_nMoveTimeTemp==1){
			m_fMoveRate = -1;
		}
		m_nMoveTimeTemp--;
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
	if(m_Skill_5_SkillItem.nSkill_CD_Time_Temp>0){
		m_Skill_5_SkillItem.nSkill_CD_Time_Temp--;
	}

	//移动
	if (m_fMoveRate >= 0 && m_fMoveRate < 360)
	{
		if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_bIsAttacking==true){m_fMoveRate = -1;return;}
		GetCCArmature()->getAnimation()->setSpeedScale(1 - m_fMoveSpeedCut_Quantity);

		CCPoint ptMoveTo;
		ptMoveTo.x = sin(m_fMoveRate/180*3.1415)*m_fMoveSpeedTemp*(1 - m_fMoveSpeedCut_Quantity);
		ptMoveTo.y = cos(m_fMoveRate/180*3.1415)*m_fMoveSpeedTemp*(1 - m_fMoveSpeedCut_Quantity);

		ptMoveTo.x += GetCurrentCube()->ptPos3D.fX;
		ptMoveTo.y += GetCurrentCube()->ptPos3D.fY;
		tagPosition3D ptPos3D;
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
					if(m_bIsFixationMoveRateStatus==false){
						pNodeBody->setRotationY(180);
					}
					
				}
			}
		}else{
			if(m_fMoveRate==0||m_fMoveRate==180){return;}
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
		}
	}

	OnHold();
}

void CNFProtagonist:: OnCtrlMoveByRotation( float fRotation )
{	
	//如果死亡停止
	if(m_bIsDeath){return;}
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_bIsAttacking==true){return;}
	m_fMoveRate = fRotation;
	m_fMoveSpeedTemp = m_fMoveSpeed;
	tagSize3D szNew;
	szNew.fLength = m_MoveImpactSize.fLength;
	szNew.fWidth = m_MoveImpactSize.fWidth;
	szNew.fHeight = m_MoveImpactSize.fHeight;
	setSize3D(szNew);
	if(m_bIsMoveAnimation==false){
		GetCCArmature()->getAnimation()->play(_NF_POWER1_WALK_);
	}
	m_nMoveTimeTemp = 0;
}

void CNFProtagonist:: OnCtrlRunByRotation( float fRotation )
{	
	//如果死亡停止
	if(m_bIsDeath){return;}
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_bIsAttacking==true){return;}
	m_fMoveRate = fRotation;
	m_fMoveSpeedTemp = m_fMoveRunSpeed;

	tagSize3D szNew;
	szNew.fLength = m_RunImpactSize.fLength;
	szNew.fWidth = m_RunImpactSize.fWidth;
	szNew.fHeight = m_RunImpactSize.fHeight;
	setSize3D(szNew);
	if(m_bIsRunAnimation==false){
		GetCCArmature()->getAnimation()->play(_NF_POWER1_RUN_);
	}
	m_nMoveTimeTemp = 0;
}

float CNFProtagonist::GetHPPercent()
{
	if(m_fHP!=0){
		return m_fHPTemp/m_fHP;
	}else{
		return 0;
	}
}

float CNFProtagonist::GetHP2Percent()
{
	if(m_fHP!=0){
		return m_fHP2Temp/m_fHP;
	}else{
		return 0;
	}
}

float CNFProtagonist::GetMPPercent()
{
	if(m_fMP!=0){
		return m_fMPTemp/m_fMP;
	}else{
		return 0;
	}
}

void CNFProtagonist::OnDead()
{
	tagRemovePerson Cmd;
	Cmd.pPerson = this;
	Cmd.pArrayTarget = m_pArrayFriend;
	if(SendMsg(enMsg_RemoveProtagonist,&Cmd,sizeof(Cmd))==false)	 
	{
		CCLog("CNFProtagonist::OnDead Error!");
	}
}

void CNFProtagonist::onExit()
{
	CNFBasicPerson::onExit();
}

float CNFProtagonist::HarmFormula( tagTakeABeating tTakeABeating )
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
		m_enCommonAttackStatus = enStatusCommonAttack_1;
		stopAllActions();
		
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
					//m_nConverselyTimeTemp = m_nConverselyTime;

					szNew.fLength = m_ConverselyImpactSize.fLength;
					szNew.fWidth = m_ConverselyImpactSize.fWidth;
					szNew.fHeight = m_ConverselyImpactSize.fHeight;
					setSize3D(szNew);
					GetCCArmature()->getAnimation()->play(_NF_POWER1_FALL_);

				}else{
					if(m_nConverselyTimeTemp>0){
						GetCCArmature()->getAnimation()->play(_NF_POWER1_FALL_);
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
						GetCCArmature()->getAnimation()->play(_NF_POWER1_HIT_);
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
		}else{
			return 0;
		}
	}else{
		displayHPLable(0,enDisplayHPLableMiss);
		return 0;
	}
}

float CNFProtagonist::TreatFormula( float fTreatPoint )
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

float CNFProtagonist::BFBTreatFormula( float fBFBTreatPoint )
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

void CNFProtagonist:: OnCtrlStop()
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_nMoveTimeTemp>0){return;}
	m_fMoveRate = -1;
}

void CNFProtagonist::ProtagonistJump()
{
	
}

void CNFProtagonist::OnCtrlCommonAttack()
{
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_bIsAttacking==true){return;}
	m_bIsAttacking = true;
	m_fMoveRate = -1;
	GetCCArmature()->getAnimation()->setSpeedScale(m_fAttackSpeedTemp*0.01f);
	if(m_enCommonAttackStatus==enStatusCommonAttack_2){
		//m_enCommonAttackStatus = enStatusCommonAttack_NO;
		m_nCommonAttackCDTemp = m_nCommonAttackCD;
		GetCCArmature()->getAnimation()->play("attack_pre_2");
		
		return;
	}else if(m_enCommonAttackStatus==enStatusCommonAttack_3){
		//m_enCommonAttackStatus = enStatusCommonAttack_NO;
		m_nCommonAttackCDTemp = m_nCommonAttackCD;
		GetCCArmature()->getAnimation()->play("attack_pre_3");
		
		return;
	}else if(m_enCommonAttackStatus==enStatusCommonAttack_4){
		m_enCommonAttackStatus = enStatusCommonAttack_NO;
		m_nCommonAttackCDTemp = m_nCommonAttackCD;
		GetCCArmature()->getAnimation()->play("attack_pre_4");

		return;
	}else if(m_enCommonAttackStatus==enStatusCommonAttack_1){
		//m_enCommonAttackStatus = enStatusCommonAttack_NO;
		m_nCommonAttackCDTemp = m_nCommonAttackCD;
		GetCCArmature()->getAnimation()->play("attack_pre_1");
		
		return;
	}
}

void CNFProtagonist::OnCCArmatureCallBack( cocos2d::extension::CCArmature *armature, MovementEventType movementType, const char *movementID )
{
	if(movementType==START){
		if(strcmp(movementID,_NF_POWER1_HOLD_)==0){
			m_bIsHoldAnimation = true;
		}else{
			m_bIsHoldAnimation = false;
		}
		if(strcmp(movementID,_NF_POWER1_WALK_)==0){
			m_bIsMoveAnimation = true;
		}else{
			m_bIsMoveAnimation = false;
		}
		if(strcmp(movementID,_NF_POWER1_RUN_)==0){
			m_bIsRunAnimation = true;
		}else{
			m_bIsRunAnimation = false;
		}
	}
	
	if(movementType==COMPLETE){
		if(strcmp(movementID,"attack_aft_1")==0||strcmp(movementID,"attack_aft_2")==0||strcmp(movementID,"attack_aft_3")==0||strcmp(movementID,"attack_aft_4")==0||strcmp(movementID,m_Skill_1_SkillItem.strSkill_AniName.c_str())==0||strcmp(movementID,m_Skill_2_SkillItem.strSkill_AniName.c_str())==0||strcmp(movementID,m_Skill_3_SkillItem.strSkill_AniName.c_str())==0||strcmp(movementID,m_Skill_4_SkillItem.strSkill_AniName.c_str())==0||strcmp(movementID,m_Skill_5_SkillItem.strSkill_AniName.c_str())==0){
			m_bIsAttacking = false;
		}
		

		if(strcmp(movementID,"attack_pre_1")==0){
			//普通攻击1
			if(m_nCommonAttackNum==m_enCommonAttackStatus){
				m_enCommonAttackStatus = enStatusCommonAttack_1;
			}else{
				m_enCommonAttackStatus = enStatusCommonAttack_2;
			}
 			GetCCArmature()->getAnimation()->play("attack_aft_1");
			CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_nCommonAttack_1_SkillItemID,m_StaticSkillInfo);
			//m_StaticSkillInfo.nIsCommonSkill

			m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
			//遍历所有技能快依次延时
			for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
			{
				runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::OnCtrlSkillCallBack)),NULL));
			}
		}else if(strcmp(movementID,"attack_pre_2")==0){
			//普通攻击2
			if(m_nCommonAttackNum==m_enCommonAttackStatus){
				m_enCommonAttackStatus = enStatusCommonAttack_1;
			}else{
				m_enCommonAttackStatus = enStatusCommonAttack_3;
			}
 			GetCCArmature()->getAnimation()->play("attack_aft_2");
			CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_nCommonAttack_2_SkillItemID,m_StaticSkillInfo);

			m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
			//遍历所有技能快依次延时
			for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
			{
				runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::OnCtrlSkillCallBack)),NULL));
			}
		}else if(strcmp(movementID,"attack_pre_3")==0){
			//普通攻击3
			if(m_nCommonAttackNum==m_enCommonAttackStatus){
				m_enCommonAttackStatus = enStatusCommonAttack_1;
			}else{
				m_enCommonAttackStatus = enStatusCommonAttack_4;
			}
 			GetCCArmature()->getAnimation()->play("attack_aft_3");
			CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_nCommonAttack_3_SkillItemID,m_StaticSkillInfo);

			m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
			//遍历所有技能快依次延时
			for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
			{
				runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::OnCtrlSkillCallBack)),NULL));
			}
		}else if(strcmp(movementID,"attack_pre_4")==0){
			//普通攻击4
			m_enCommonAttackStatus = enStatusCommonAttack_1;
 			GetCCArmature()->getAnimation()->play("attack_aft_4");
			CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_nCommonAttack_4_SkillItemID,m_StaticSkillInfo);

			m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
			//遍历所有技能快依次延时
			for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
			{
				runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::OnCtrlSkillCallBack)),NULL));
			}
		}
		
	}
	if(movementType==LOOP_COMPLETE){
		//CCLog("=============================s===%s",movementID);
	}
}

void CNFProtagonist::OnHold()
{
	tagSize3D szNew;
	if(m_nGround + m_fSubtractSpeedl>=m_Cube3D.ptPos3D.fZ && m_nConverselyTimeTemp<=0 && m_nCrickTimeTemp<=0 && m_fMoveRate<0 && m_bIsAttacking==false){
		szNew.fLength = m_HoldImpactSize.fLength;
		szNew.fWidth = m_HoldImpactSize.fWidth;
		szNew.fHeight = m_HoldImpactSize.fHeight;
		setSize3D(szNew);
		if(m_bIsHoldAnimation==false){
			GetCCArmature()->getAnimation()->play(_NF_POWER1_HOLD_);
		}
		GetCCArmature()->getAnimation()->setSpeedScale(1);
	}
}

bool CNFProtagonist::OnCtrlSkill( int nID )
{
	if(m_bIsAttacking==false){
		GetCCArmature()->getAnimation()->setSpeedScale(m_fAttackSpeedTemp*0.01f);
		if(nID==m_Skill_1_SkillItem.nSkill_SkillItemID){
			if(m_Skill_1_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_1_SkillItem.fCostMp){
				m_bIsAttacking = true;
				GetCCArmature()->getAnimation()->play(m_Skill_1_SkillItem.strSkill_AniName.c_str());
				CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_1_SkillItem.nSkill_SkillItemID,m_StaticSkillInfo);
				m_Skill_1_SkillItem.nSkill_CD_Time_Temp = m_Skill_1_SkillItem.nSkill_CD_Time;
				m_fMPTemp -= m_Skill_1_SkillItem.fCostMp;

				m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
				//遍历所有技能快依次延时
				for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
				{
					runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::OnCtrlSkillCallBack)),NULL));
				}

				return true;
			}else{
				return false;
			}
		}else if(nID==m_Skill_2_SkillItem.nSkill_SkillItemID){
			if(m_Skill_2_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_2_SkillItem.fCostMp){
				m_bIsAttacking = true;
				GetCCArmature()->getAnimation()->play(m_Skill_2_SkillItem.strSkill_AniName.c_str());
				CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_2_SkillItem.nSkill_SkillItemID,m_StaticSkillInfo);
				m_Skill_2_SkillItem.nSkill_CD_Time_Temp = m_Skill_2_SkillItem.nSkill_CD_Time;
				m_fMPTemp -= m_Skill_2_SkillItem.fCostMp;

				m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
				//遍历所有技能快依次延时
				for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
				{
					runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::OnCtrlSkillCallBack)),NULL));
				}

				return true;
			}else{
				return false;
			}
		}else if(nID==m_Skill_3_SkillItem.nSkill_SkillItemID){
			if(m_Skill_3_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_3_SkillItem.fCostMp){
				m_bIsAttacking = true;
				GetCCArmature()->getAnimation()->play(m_Skill_3_SkillItem.strSkill_AniName.c_str());
				CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_3_SkillItem.nSkill_SkillItemID,m_StaticSkillInfo);
				m_Skill_3_SkillItem.nSkill_CD_Time_Temp = m_Skill_3_SkillItem.nSkill_CD_Time;
				m_fMPTemp -= m_Skill_3_SkillItem.fCostMp;

				m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
				//遍历所有技能快依次延时
				for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
				{
					runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::OnCtrlSkillCallBack)),NULL));
				}

				return true;
			}else{
				return false;
			}
		}else if(nID==m_Skill_4_SkillItem.nSkill_SkillItemID){
			if(m_Skill_4_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_4_SkillItem.fCostMp){
				m_bIsAttacking = true;
				GetCCArmature()->getAnimation()->play(m_Skill_4_SkillItem.strSkill_AniName.c_str());
				CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_4_SkillItem.nSkill_SkillItemID,m_StaticSkillInfo);
				m_Skill_4_SkillItem.nSkill_CD_Time_Temp = m_Skill_4_SkillItem.nSkill_CD_Time;
				m_fMPTemp -= m_Skill_4_SkillItem.fCostMp;

				m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
				//遍历所有技能快依次延时
				for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
				{
					runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::OnCtrlSkillCallBack)),NULL));
				}

				return true;
			}else{
				return false;
			}
		}else if(nID==m_Skill_5_SkillItem.nSkill_SkillItemID){
			if(m_Skill_5_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_5_SkillItem.fCostMp){
				m_bIsAttacking = true;
				GetCCArmature()->getAnimation()->play(m_Skill_5_SkillItem.strSkill_AniName.c_str());
				CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nRoleID,m_Skill_5_SkillItem.nSkill_SkillItemID,m_StaticSkillInfo);
				m_Skill_5_SkillItem.nSkill_CD_Time_Temp = m_Skill_5_SkillItem.nSkill_CD_Time;
				m_fMPTemp -= m_Skill_5_SkillItem.fCostMp;

				m_SkillItem = m_StaticSkillInfo.vecSkillItem.begin();
				//遍历所有技能快依次延时
				for (CNFSkillItemVec::iterator iter = m_StaticSkillInfo.vecSkillItem.begin();iter!=m_StaticSkillInfo.vecSkillItem.end();iter++)
				{
					runAction(CCSequence::create(CCDelayTime::create(iter->fDelayTime*(1/(m_fAttackSpeedTemp*0.01f))),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::OnCtrlSkillCallBack)),NULL));
				}

				return true;
			}else{
				return false;
			}
		}

	}
	return false;
}

void CNFProtagonist::OnCtrlSkillCallBack()
{
	
	if(m_SkillItem==m_StaticSkillInfo.vecSkillItem.end()){return;}

	int SkillLV = 1;

	//得到对应技能快信息，并发送消息释放技能
	tagSkillItemStaticInfo pInfoNN = *m_SkillItem;

	//是否使释放者转头
	if(pInfoNN.nIsTurnRound>0){
		SetDirection(GetDirection()*-1);
	}

	tagCommonAttack Cmd(this,&pInfoNN,SkillLV);

	Cmd.pArrayTarget = m_pArrayTarget;

	tagVector3D vec3D;
	vec3D.fX = pInfoNN.fDisX*GetDirection();
	vec3D.fY = 0;
	vec3D.fZ = 0;
	MoveBy3D(pInfoNN.fTimeX,vec3D);
	vec3D.fX = 0;
	vec3D.fY = pInfoNN.fDisY;
	vec3D.fZ = 0;
	MoveBy3D(pInfoNN.fTimeY,vec3D);

	SetZSpeed(pInfoNN.fSpeedZ);

	//添加buff
	for (CNFBuffStaticInfoVec::iterator iter = pInfoNN.vecBuff.begin();iter!=pInfoNN.vecBuff.end();iter++)
	{
		switch (iter->nBuffID)
		{
		case _NF_BUFF_TREATFORMULA_ID:		//治疗
			{
				CCObject *pItem = NULL;
				CCARRAY_FOREACH(m_pArrayFriend,pItem)
				{
					CNFBasicPerson *pNodeItem = dynamic_cast<CNFBasicPerson*>(pItem);
					if(pNodeItem!=NULL)
					{
						pNodeItem->TreatFormula(iter->fBuffPoint1 + iter->fBuffPoint1_g*SkillLV);
					}
				}
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
		case _NF_BUFF_SINGLE_ATTACKUP_ID:		//增加单体攻击力
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

	if(getTag()==enTagProtagonist){
		//是否有镜头拉伸
		if(pInfoNN.nIsLensfeatrue>0){
			getLTLayerParent()->LensFeature(pInfoNN.fLensfeatureTime);
		}
	}
	
	if(SendMsg(enMsgProtagonistSkill,&Cmd,sizeof(Cmd))==false)
	{
		CCLog("OnCtrlSkill1 Error!");
	}

	
	m_SkillItem++;

	//最后一块
	if(m_SkillItem==m_StaticSkillInfo.vecSkillItem.end()){
		if(m_bIsAutoMode==true||m_bIsAutoAttacking==true){
			runAction(CCSequence::create(CCDelayTime::create(0.02f),CCCallFunc::create(this,callfunc_selector(CNFProtagonist::AutoAttackOverCallBack)),NULL));
		}
	}
}

void CNFProtagonist::OnCtrlTemp()
{
	// 	HarmFormula(tTemp);

	TreatFormula(100);

	//AddSuperArmorBuff(500);

	//AddAstheniaBuff(200);

	//AddAttackSpeedCutBuff(500,80);

	//AddMoveSpeedCutBuff(500,0.8f);

	//AddPoisoningBuff(240,10);

	//getLTLayerParent()->LensFeature();
}

void CNFProtagonist::OnCtrlMoveToPonit( CCPoint ptPos )
{
	if(m_bIsDeath){return;}
	if(m_nCrickTimeTemp>0||m_nConverselyTimeTemp>0||m_bIsAttacking==true||m_bIsAutoListAttacking==true){return;}

	float o = ptPos.x - GetCurrentCube()->ptPos3D.fX;
	float a = ptPos.y - GetCurrentCube()->ptPos3D.fY;
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
	m_nMoveTimeTemp = Ztime;

	m_fMoveRate = fMoveRate;
	m_fMoveSpeedTemp = m_fMoveRunSpeed;

	tagSize3D szNew;
	szNew.fLength = m_RunImpactSize.fLength;
	szNew.fWidth = m_RunImpactSize.fWidth;
	szNew.fHeight = m_RunImpactSize.fHeight;
	setSize3D(szNew);
	if(m_bIsRunAnimation==false){
		GetCCArmature()->getAnimation()->play(_NF_POWER1_RUN_);
	}
}

bool CNFProtagonist::IsHaveSkillIDAndCD( int nSkillID )
{
	if(nSkillID==m_Skill_1_SkillItem.nSkill_SkillItemID){
		if(m_Skill_1_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_1_SkillItem.fCostMp){
			return true;
		}
	}
	if(nSkillID==m_Skill_2_SkillItem.nSkill_SkillItemID){
		if(m_Skill_2_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_2_SkillItem.fCostMp){
			return true;
		}
	}
	if(nSkillID==m_Skill_3_SkillItem.nSkill_SkillItemID){
		if(m_Skill_3_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_3_SkillItem.fCostMp){
			return true;
		}
	}
	if(nSkillID==m_Skill_4_SkillItem.nSkill_SkillItemID){
		if(m_Skill_4_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_4_SkillItem.fCostMp){
			return true;
		}
	}
	if(nSkillID==m_Skill_5_SkillItem.nSkill_SkillItemID){
		if(m_Skill_5_SkillItem.nSkill_CD_Time_Temp<=0&&m_fMPTemp>=m_Skill_5_SkillItem.fCostMp){
			return true;
		}
	}

	return false;
}

void CNFProtagonist::AutoAttackOverCallBack()
{
	m_bIsAutoAttacking = false;
}

float CNFProtagonist::BFBRecoverMP( float fBFBMPPoint )
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













