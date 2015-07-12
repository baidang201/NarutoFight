#include "NFCommonAttack.h"
#include "../../datamanager/NFDataManager.h"
#include "../person/NFEnemy.h"

CNFCommonAttack* CNFCommonAttack::CreateCommonAttack(tagCommonAttack *tCommonAttackInfo,CCArray *pArrayTarget)
{
	do
	{
		CNFCommonAttack * pSpr = new  CNFCommonAttack();
		CC_BREAK_IF(pSpr==NULL);
		CC_BREAK_IF(pSpr->InitCommonAttack(tCommonAttackInfo,pArrayTarget)==false);
		pSpr->autorelease();
		return pSpr;
	} while (false);
	CCLog("Fun CNFCommonAttack::CreateCommonAttack Error!");
	return NULL;
}

bool CNFCommonAttack::InitCommonAttack(tagCommonAttack *tCommonAttackInfo,CCArray *pArrayTarget)
{
	do
	{

		tagSize3D tagNewSize;
		tagNewSize.fLength = tCommonAttackInfo->fOldLength;
		tagNewSize.fWidth = tCommonAttackInfo->fOldWidth;
		tagNewSize.fHeight = tCommonAttackInfo->fOldHeight;

		if(tCommonAttackInfo->nIsArmature!=0){
			CC_BREAK_IF(CNFBasicSkill::init(tCommonAttackInfo->strArmatureName.c_str(),tCommonAttackInfo->pReleaser,pArrayTarget,0.4f,tagNewSize,tCommonAttackInfo->pReleaser->GetDirection())==false);
		}else{
			CC_BREAK_IF(CNFBasicSkill::init(tCommonAttackInfo->pReleaser,pArrayTarget,0.4f,tagNewSize,tCommonAttackInfo->pReleaser->GetDirection())==false);
		}
		SetGravityOpen(tCommonAttackInfo->nIsGravity>0?true:false);
		
		m_pHitOverArray = CCArray::create();
		m_pHitOverArray->retain();
		m_tTakeABeatingInfo = tCommonAttackInfo->tTakeABeating;
		int nDirection = tCommonAttackInfo->pReleaser->GetDirection();


		//技能块的移动和缩放
		tagVector3D vec3D;
		vec3D.fX = _NF_ABS(tCommonAttackInfo->fNewX - tCommonAttackInfo->fOldX)*nDirection;
		vec3D.fY = tCommonAttackInfo->fNewY - tCommonAttackInfo->fOldY;
		vec3D.fZ = tCommonAttackInfo->fNewZ - tCommonAttackInfo->fOldZ;
		MoveBy3D(tCommonAttackInfo->fChangeTime,vec3D,true);

		vec3D.fX = tCommonAttackInfo->fNewLength - tCommonAttackInfo->fOldLength;
		vec3D.fY = tCommonAttackInfo->fNewWidth - tCommonAttackInfo->fOldWidth;
		vec3D.fZ = tCommonAttackInfo->fNewHeight - tCommonAttackInfo->fOldHeight;
		ScaleBy3D(tCommonAttackInfo->fChangeTime,vec3D,true);
		
		return true;
	} while (false);
	CCLog("Fun CNFCommonAttack::InitCommonAttack Error!");
	return false;
}

void CNFCommonAttack::CallPerFrame(float fT)
{
	//如果死亡停止更
	if(m_bIsVanish){return;}
	m_nExistTime++;

	CNFBasicPerson* pReleaser = dynamic_cast<CNFBasicPerson*>(m_pReleaser);

	//是否跟随释放者
	if(m_tTakeABeatingInfo.bIsFollow==true&&pReleaser!=NULL){
		setPosition3D(pReleaser->GetCurrentCube()->ptPos3D);
	}

	//攻击间隔
	if(m_tTakeABeatingInfo.nAttackSpeed>0&&m_nExistTime%m_tTakeABeatingInfo.nAttackSpeed==0){
		m_pHitOverArray->removeAllObjects();
	}

	CCObject *pItem = NULL;
	CCARRAY_FOREACH(m_pArrayTarget,pItem)
	{
		CNFBasicPerson *pTargetUnit = dynamic_cast<CNFBasicPerson*>(pItem);
		if(pTargetUnit==NULL||pTargetUnit->IsDeath())continue;
		tagVector3D vec3D;
		//若碰撞
		if(int nIntersects = GetIntersectsVec(pTargetUnit,vec3D)&&UINT_MAX==m_pHitOverArray->indexOfObject(pTargetUnit))
		{
			//将被击中目标添加到碰撞完数组
			m_pHitOverArray->addObject(pTargetUnit);

			//判断是否是爆炸模式
			if(m_tTakeABeatingInfo.bIsExplodeMode){
				m_tTakeABeatingInfo.nXDis = _NF_ABS(m_tTakeABeatingInfo.nXDis);
				int nFangXiang = 1;
				if(GetCurrentCube()->ptPos3D.fX - pTargetUnit->GetCurrentCube()->ptPos3D.fX > 0){
					nFangXiang = -1;
				}
				m_tTakeABeatingInfo.nXDis = m_tTakeABeatingInfo.nXDis*nFangXiang;
			}
			//判断是否是拉倒面前模式
			if(m_tTakeABeatingInfo.bIsPullMode&&pReleaser!=NULL){
				float speedTemp = 500;
				if(m_tTakeABeatingInfo.fXTime>0){
					speedTemp = m_tTakeABeatingInfo.nXDis/m_tTakeABeatingInfo.fXTime;
				}
				m_tTakeABeatingInfo.nXDis = (pReleaser->GetCurrentCube()->ptPos3D.fX + (pReleaser->GetCurrentCube()->szCube.fLength + pTargetUnit->GetCurrentCube()->szCube.fLength)*pReleaser->GetDirection()) - pTargetUnit->GetCurrentCube()->ptPos3D.fX;
				m_tTakeABeatingInfo.fXTime = m_tTakeABeatingInfo.nXDis/speedTemp;
			}
			//判断是否是吸引模式
			if(m_tTakeABeatingInfo.bIsSuctionMode&&pReleaser!=NULL){
				float fXPosTemp =  pReleaser->GetCurrentCube()->ptPos3D.fX + m_tTakeABeatingInfo.fSuctionPosX*pReleaser->GetDirection();
				float fYPosTemp =  pReleaser->GetCurrentCube()->ptPos3D.fY + m_tTakeABeatingInfo.fSuctionPosY*pReleaser->GetDirection();

				float o = 0;
				float a = 0;
				if(m_tTakeABeatingInfo.bIsSuctionSkillItemCenter){
					o = GetCurrentCube()->ptPos3D.fX - pTargetUnit->GetCurrentCube()->ptPos3D.fX;
					a = GetCurrentCube()->ptPos3D.fY - pTargetUnit->GetCurrentCube()->ptPos3D.fY;
				}else{
					o = fXPosTemp - pTargetUnit->GetCurrentCube()->ptPos3D.fX;
					a = fYPosTemp - pTargetUnit->GetCurrentCube()->ptPos3D.fY;
				}
				float fLen = sqrt(pow(o,2)+pow(a,2));


				if(m_tTakeABeatingInfo.fSuctionSpeed>fLen){
					m_tTakeABeatingInfo.fSuctionSpeed = fLen;
				}
				tagVector3D vec3DTemp;
				vec3DTemp.fX = o*m_tTakeABeatingInfo.fSuctionSpeed/fLen;
				vec3DTemp.fY = a*m_tTakeABeatingInfo.fSuctionSpeed/fLen;
				vec3DTemp.fZ = 0;
				pTargetUnit->MoveBy3D(m_tTakeABeatingInfo.fSuctionTime,vec3DTemp,false);

			}
			//使被攻击者转头
			CNFEnemy *pEnemy = dynamic_cast<CNFEnemy*>(pTargetUnit);
			if(pEnemy!=NULL){
				pTargetUnit->SetDirectionEnemy(GetCurrentCube()->ptPos3D.fX - pTargetUnit->GetCurrentCube()->ptPos3D.fX);
			}else{
				pTargetUnit->SetDirection(GetCurrentCube()->ptPos3D.fX - pTargetUnit->GetCurrentCube()->ptPos3D.fX);
			}

			//调用伤害公式
			pTargetUnit->HarmFormula(m_tTakeABeatingInfo);

			//发消息，得到被击中的敌人是谁
			SendMsg(enMsg_Hit_Enemy,pTargetUnit,sizeof(*pTargetUnit));
			return;
		}
	}
// 	//如果技能飞出边界 则移除
// 	if(getLTLayerParent()==NULL){return;}
// 	if(GetCurrentCube()->ptPos3D.fX - GetCurrentCube()->szCube.fLength *0.5f < 0||GetCurrentCube()->ptPos3D.fX + GetCurrentCube()->szCube.fLength*0.5f > getLTLayerParent()->Get3DWorldSize()->fLength)
// 	{
// 		tagVector3D vec3D;
// 		vec3D.fX = 0;
// 		vec3D.fY = 0;
// 		vec3D.fZ = 0;
// 		MoveBy3D(0.02f,vec3D,true);
// 	}
}

void CNFCommonAttack::onExit()
{
	m_pHitOverArray->release();
	CNFBasicSkill::onExit();
}






