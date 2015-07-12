#include "PublicDef.h"
#include "../gameunit/person/NFProtagonist.h"


//静态数据库
std::string GetStaticDataName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return _TR_STATIC_DATABASE_FILE_NAME_;
#else
	std::string strResult = CCFileUtils::sharedFileUtils()->getWritablePath();
	strResult+= _TR_STATIC_DATABASE_FILE_NAME_;
	return strResult;
#endif
}

//存储数据库
std::string GetSaveDataName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return _NF_SAVE_DATABASE_FILE_NAME_;
#else
	std::string strResult = CCFileUtils::sharedFileUtils()->getWritablePath();
	strResult+= _NF_SAVE_DATABASE_FILE_NAME_;
	return strResult;
#endif
}

tagCommonAttack::tagCommonAttack(CNFBasicPerson* pReleaserU,tagSkillItemStaticInfo* pInfo,int nSkillLV)
{
	pReleaser = pReleaserU;
	pArrayTarget = NULL;
	fChangeTime = pInfo->tSkillChange.fChangeTime;
	nIsGravity = pInfo->tSkillChange.nIsGravity;
	fOldX = pInfo->tSkillChange.fOldX;
	fOldY = pInfo->tSkillChange.fOldY;
	fOldZ = pInfo->tSkillChange.fOldZ;
	fOldLength = pInfo->tSkillChange.fOldLength;
	fOldWidth = pInfo->tSkillChange.fOldWidth;
	fOldHeight = pInfo->tSkillChange.fOldHeight;
	fNewX = pInfo->tSkillChange.fNewX;
	fNewY = pInfo->tSkillChange.fNewY;
	fNewZ = pInfo->tSkillChange.fNewZ;
	fNewLength = pInfo->tSkillChange.fNewLength;
	fNewWidth = pInfo->tSkillChange.fNewWidth;
	fNewHeight = pInfo->tSkillChange.fNewHeight;
	tTakeABeating.bIsConversely = pInfo->tSkillDamage.nIsConversely==0?false:true;
	if(pReleaser!=NULL)
	{
		if(pInfo->bIsCommonSkill){
			tTakeABeating.fDamagePoint = pReleaser->GetAttack();
		}else{
			//技能攻击力 = 施放者掌控力 + （技能基础攻击力 + 技能攻击力成长*技能等级）+ 施放者攻击力*（技能基础百分比倍数 + 技能成长百分比倍数*技能等级）
			tTakeABeating.fDamagePoint = pReleaser->GetDomination() + (pInfo->tSkillDamage.fDamagePoint + pInfo->tSkillDamage.fDamagePoint_g*nSkillLV) + pReleaser->GetAttack()*(1 + pInfo->tSkillDamage.fDamagePercent + pInfo->tSkillDamage.fDamagePercent_g*nSkillLV);
		}
		
		tTakeABeating.nCrit = pReleaser->GetCrit() + pInfo->tSkillDamage.nCritAdd;
		tTakeABeating.fCritDamage = pReleaser->GetCritDamage() + pInfo->tSkillDamage.fCritDamageAdd;
		tTakeABeating.nHitTarget = pReleaser->GetHitTarget() + pInfo->tSkillDamage.nHitTargetAdd;
	}
	tTakeABeating.fXTime = pInfo->tSkillDamage.fXTime;
	tTakeABeating.nXDis = pInfo->tSkillDamage.fXDis;
	if(pInfo->tSkillDamage.nIsDirection>0){
		fOldX = fOldX*pReleaser->GetDirection();
		fNewX = fNewX*pReleaser->GetDirection();
		
		tTakeABeating.nXDis = tTakeABeating.nXDis*pReleaser->GetDirection();
	}
	tTakeABeating.fYTime = pInfo->tSkillDamage.fYTime;
	tTakeABeating.nYDis = pInfo->tSkillDamage.fYDis;
	tTakeABeating.fZSpeed = pInfo->tSkillDamage.fZSpeed;
	tTakeABeating.nCrickAdd = pInfo->tSkillDamage.nCrickAdd;
	tTakeABeating.nAttackSpeed = pInfo->tSkillDamage.nAttackSpeed;
	tTakeABeating.bIsExplodeMode = pInfo->tSkillDamage.nIsExplodeMode>0?true:false;
	tTakeABeating.bIsPullMode = pInfo->tSkillDamage.nIsPullMode>0?true:false;


	tTakeABeating.bIsSuctionMode = pInfo->tSkillDamage.nIsSuctionMode>0?true:false;
	tTakeABeating.fSuctionSpeed = pInfo->tSkillDamage.fSuctionSpeed;
	tTakeABeating.fSuctionTime = pInfo->tSkillDamage.fSuctionTime;
	tTakeABeating.fSuctionPosX = pInfo->tSkillDamage.fSuctionPosX;
	tTakeABeating.fSuctionPosY = pInfo->tSkillDamage.fSuctionPosY;
	tTakeABeating.bIsOnlyDamageNotHit = pInfo->tSkillDamage.nIsOnlyDamageNotHit>0?true:false;
	tTakeABeating.bIsSuctionSkillItemCenter = pInfo->tSkillDamage.nIsSuctionSkillItemCenter>0?true:false;
	tTakeABeating.bIsDamage = pInfo->tSkillDamage.nIsDamage>0?true:false;
	tTakeABeating.bIsFollow = pInfo->tSkillDamage.nIsFollow>0?true:false;
	tTakeABeating.bIsBreakSiperarmor = pInfo->tSkillDamage.nIsBreakSiperarmor>0?true:false;
	tTakeABeating.nHitEffectType = pInfo->tSkillDamage.nHitEffectType;

	for (CNFBuffStaticInfoVec::iterator iter = pInfo->tSkillDamage.vecTargetBuff.begin();iter != pInfo->tSkillDamage.vecTargetBuff.end();iter++)
	{
		tagBuffInfo BuffInfo;
		BuffInfo.nBuffID = iter->nBuffID;
		BuffInfo.nBuffTime = iter->nBuffTime + nSkillLV*iter->nBuffTime_g;
		BuffInfo.fBuffPoint1 = iter->fBuffPoint1 + nSkillLV*iter->fBuffPoint1_g;
		BuffInfo.fBuffPoint2 = iter->fBuffPoint2 + nSkillLV*iter->fBuffPoint2_g;

		tTakeABeating.vecBuff.push_back(BuffInfo);
	}

	nIsArmature = pInfo->nIsArmature;
	strArmatureName = pInfo->strArmatureName;


}
