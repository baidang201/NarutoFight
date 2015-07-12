#include "NFBox.h"
#include "../../datamanager/NFDataManager.h"

CNFBox* CNFBox::CreateBox(int nBoxID)
{
	do
	{
		CNFBox * pSpr = new  CNFBox();
		CC_BREAK_IF(pSpr==NULL);
		CC_BREAK_IF(pSpr->InitBox(nBoxID)==false);
		pSpr->autorelease();
		return pSpr;
	} while (false);
	CCLog("Fun CNFBox::CreateBox Error!");
	return NULL;
}

bool CNFBox::InitBox(int nBoxID)
{
	do
	{
		tagArticleStaticInfo ArticleInfo;
		CNFStaticDataManager::SharedData()->GetArticleStaticInfo(nBoxID,ArticleInfo);

		tagStageStaticInfo StageInfo;
		CNFStaticDataManager::SharedData()->GetStageStaticInfo(1,StageInfo);

		tagSize3D tagNewSize;
		tagNewSize.fLength = ArticleInfo.fLength_Hold;
		tagNewSize.fWidth = ArticleInfo.fWidth_Hold;
		tagNewSize.fHeight = ArticleInfo.fHeight_Hold;

		if(nBoxID==1){
			CC_BREAK_IF(CNFBox::init(StageInfo.fRate,tagNewSize)==false);
		}else{
			CC_BREAK_IF(CNFBox::init(StageInfo.fRate,tagNewSize)==false);
			SetGravityOpen(false);
		}
		
		m_nExistTime = 0;

		return true;
	} while (false);
	CCLog("Fun CNFBox::InitBox Error!");
	return false;
}

void CNFBox::CallPerFrame(float fT)
{
	//如果死亡停止更
	if(m_bIsDeath){return;}
	m_nExistTime++;
}

float CNFBox::GetHPPercent()
{
	if(m_fHP!=0){
		return m_fHPTemp/m_fHP;
	}else{
		return 0;
	}
}

void CNFBox::OnDead()
{
	// 	TCmd_SceneRemoveProtagonist Cmd;
	// 	Cmd.pProtagonist = this;
	// 	if(SendMsg(enMsg_SceneRemoveProtagonist,&Cmd,sizeof(Cmd))==false)	 
	// 	{
	// 		CCLog("SendMsg(enMsg_SceneRemoveProtagonist,this,sizeof(this))==false");
	// 	}
}


void CNFBox::onExit()
{
	CNFBasicArticle::onExit();
}

float CNFBox::HarmFormula( float fDamagePoint,int wDamageType,int nCrit )
{
	//如果死亡停止更
	if(m_bIsDeath){return 0.1f;}
	return 0.1f;
}




