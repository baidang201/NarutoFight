#include "NF3DWorldLayer.h"
#include "../tiledmap/NFTMXTiledMap.h"
#include "../gameunit/person/NFProtagonist.h"
#include "../gameunit/skill/NFCommonAttack.h"
#include "../gameunit/person/NFEnemy.h"
#include "../gameunit/article/NFBox.h"
#include "../gameunit/article/NFNPC.h"
#include "../gameunit/article/NFTransferDoor.h"
#include "../datamanager/NFDataManager.h"
#include "gamescene/NFGameScene.h"
#include "../ui/NFTownScene.h"

#define _TMX_START_X_		-30			//景深地图初始x轴坐标

CNF3DWorldLayer* CNF3DWorldLayer::CreateLayer(int nStageID,int nRoleID)
{
	do 
	{
		CNF3DWorldLayer* pLayer = new CNF3DWorldLayer();
		if (pLayer!=NULL && pLayer->InitLayer(nStageID,nRoleID)==true)
		{
			pLayer->autorelease();
			return pLayer;
		}

		delete pLayer; 
		pLayer = NULL; 
		return NULL; 
	} while (false);
	CCLog("Fun CNF3DWorldLayer::CreateLayer Error!");
	return NULL;
}

bool CNF3DWorldLayer::InitLayer(int nStageID,int nRoleID)
{
	do 
	{
		//读取关卡信息
		tagStageStaticInfo StageInfo;
		CNFStaticDataManager::SharedData()->GetStageStaticInfo(nStageID,StageInfo);
		if (StageInfo.fRate < 0)
		{
			CCLog("Fun 3D WorldLayer Create Error!");
			return false;
		}

		m_nStageID = nStageID;
		m_nRoleID = nRoleID;

		tagSize3D StageSize3DInfo;
		StageSize3DInfo.fHeight=StageInfo.fHeight;
		StageSize3DInfo.fLength=StageInfo.fLength;
		StageSize3DInfo.fWidth=StageInfo.fWidth;
		//初始化父类
		CC_BREAK_IF(CNFWorld3DLayer::init(StageInfo.fRate,StageSize3DInfo)==false);

		m_pArticleArray = CCArray::create();
		m_pArticleArray->retain();
		m_pFriendArray = CCArray::create();
		m_pFriendArray->retain();
		m_pEnemyArray = CCArray::create();
		m_pEnemyArray->retain();

		//CNFBox* pBox = CNFBox::CreateBox(1);
		//pBox->SetMsg(this);
		//pBox->setPosition3D(400,400);
		//addChild(pBox,enZOrderMid);
		//m_pArticleArray->addObject(pBox);

		tagPosition3D ptPos3D;
		//创建角色
		CNFProtagonist* pProtagonist = CNFProtagonist::Create(m_nRoleID,m_pFriendArray,m_pEnemyArray);
		ptPos3D.fX = StageInfo.fProtagonistX;
		ptPos3D.fY = StageInfo.fProtagonistY;
		ptPos3D.fZ = 0;
		pProtagonist->SetMsg(this);
		pProtagonist->setPosition3D(ptPos3D);
		pProtagonist->SetArticleArray(m_pArticleArray);
		addChild(pProtagonist,enZOrderMid,enTagProtagonist);
		m_pFriendArray->addObject(pProtagonist);
		SetProtagonist(pProtagonist);


		//创建NPC
		for (CNFNPCJsonInfoVec::iterator it = StageInfo.NPCJsonInfoVec.begin();it != StageInfo.NPCJsonInfoVec.end();it++)
		{
			CNFNPC* pNPC1 = CNFNPC::Create(it->nNPCID,it->NPCStaticInfo.strArmatureName.c_str(),it->NPCStaticInfo.fLength,it->NPCStaticInfo.fWidth,it->NPCStaticInfo.fHeight);
			pNPC1->SetMsg(this);
			ptPos3D.fX = it->fPosX;
			ptPos3D.fY = it->fPosY;
			ptPos3D.fZ = it->fPosZ;
			pNPC1->setPosition3D(ptPos3D);
			addChild(pNPC1,enZOrderMid);
		}

		//创建门
		for (CNFDoorJsonInfoVec::iterator it = StageInfo.DoorJsonInfoVec.begin();it != StageInfo.DoorJsonInfoVec.end();it++)
		{
			CNFTransferDoor* pTransferDoor = CNFTransferDoor::Create(it->DoorStaticInfo.nRelaceStageID,pProtagonist,it->DoorStaticInfo.strArmatureName.c_str(),it->DoorStaticInfo.fLength,it->DoorStaticInfo.fWidth,it->DoorStaticInfo.fHeight);
			pTransferDoor->SetMsg(this);
			ptPos3D.fX = it->fPosX;
			ptPos3D.fY = it->fPosY;
			ptPos3D.fZ = it->fPosZ;
			pTransferDoor->setPosition3D(ptPos3D);
			addChild(pTransferDoor,enZOrderMid);
		}


		//若为副本
		if (nStageID >= _NF_TOWN_OR_BATTLE_ID_ && nStageID <_NF_TOWN_FB_ID_)
		{
			//创建敌人波次
			m_pEnemyWave = CNFEnemyWave::CreateEnemyWave(nStageID,m_pEnemyArray);
			CC_BREAK_IF(m_pEnemyWave==NULL);
			m_pEnemyWave->SetMsg(this);

// 			CNFProtagonist* pProtagonist2 = CNFProtagonist::Create(2,m_pFriendArray,m_pEnemyArray);
// 			ptPos3D.fX = 700;
// 			ptPos3D.fY = 500;
// 			ptPos3D.fZ = 0;
// 			pProtagonist2->SetMsg(this);
// 			pProtagonist2->setPosition3D(ptPos3D);
// 			pProtagonist2->SetArticleArray(m_pArticleArray);
// 			pProtagonist2->SetAutoMode(true);
// 			addChild(pProtagonist2,enZOrderMid);
// 			m_pFriendArray->addObject(pProtagonist2);
//
//  			CNFProtagonist* pProtagonist3 = CNFProtagonist::Create(1,m_pEnemyArray,m_pFriendArray);
//  			ptPos3D.fX = 700;
//  			ptPos3D.fY = 100;
//  			ptPos3D.fZ = 0;
//  			pProtagonist3->SetMsg(this);
//  			pProtagonist3->setPosition3D(ptPos3D);
//  			pProtagonist3->SetArticleArray(m_pArticleArray);
//  			pProtagonist3->SetAutoMode(true);
//  			addChild(pProtagonist3,enZOrderMid);
//  			m_pEnemyArray->addObject(pProtagonist3);
//
//			//
//			CNFEnemy* pEnemy = CNFEnemy::CreateEnemy(pProtagonist);
//			pEnemy->SetMsg(this);
//			ptPos3D.fX = 700;
//			ptPos3D.fY = 20;
//			ptPos3D.fZ = 0;
//			pEnemy->setPosition3D(ptPos3D);
//			addChild(pEnemy,enZOrderMid);
//			pEnemy->SetArticleArray(m_pArticleArray);
//			m_pEnemyArray->addObject(pEnemy);
//
//
		}
		
		//tiledmap
		CNFMidTMXMap* pGameMap = CNFMidTMXMap::CreateGameMap(StageInfo.strTMXWorldName.c_str(),this);
		CC_BREAK_IF(pGameMap==NULL);
		pGameMap->setPosition(ccp(_TMX_START_X_,0));
		addChild(pGameMap,-enTagMidTMXMap,enTagMidTMXMap);
		
		//创建景深地图
		int i = enTagTMXMap1;
		for (CNFTmxStaticInfoMap::iterator it = StageInfo.TmxStaticInfoMap.begin();it != StageInfo.TmxStaticInfoMap.end();it++)
		{
			CMGameMapLayer* pGameMap = CMGameMapLayer::CreateGameMap(it->second.strTmxName.c_str());
			CC_BREAK_IF(pGameMap==NULL);
			pGameMap->setAnchorPoint(ccp(0,0));
			pGameMap->setPosition(ccp(it->second.fStartPosX,it->second.fStartPosY));
			addChild(pGameMap,-i,i);

			if (i==enTagTMXMap1)	pGameMap->SetSpeedMultipleX(0.9f);
			if (i==enTagTMXMap2)	pGameMap->SetSpeedMultipleX(0.1f);
			if (i==enTagTMXMap3)	pGameMap->SetSpeedMultipleX(1.0f);

			i++;
		}

		return true;
	} while (false);
	CCLog("Fun CNF3DWorldLayer::init Error!");
	return false;
}

void CNF3DWorldLayer::update(float delta)
{
	do 
	{
		CCObject *pItem = NULL;
		CCARRAY_FOREACH(getChildren(),pItem)
		{
			CNFBasicUnit *pNodeItem = dynamic_cast<CNFBasicUnit*>(pItem);
			if(pNodeItem!=NULL)
			{
				//调整显示层级
				int nZOrder = enZOrderMid - pNodeItem->getPositionY();
				pNodeItem->setZOrder(nZOrder);

				pNodeItem->CallPerFrame(delta);
			}
		}

		//遍历敌人波次
		if (m_nStageID >= _NF_TOWN_OR_BATTLE_ID_ && m_nStageID <_NF_TOWN_FB_ID_)
			m_pEnemyWave->update();

		//景深地图同步位置
		for (int i=enTagTMXMap1;i<=enTagTMXMap3;i++)
		{
			CMGameMapLayer* pGameMap = dynamic_cast<CMGameMapLayer *>(getChildByTag(i));
			if (pGameMap!=NULL)
			{
				//若存在缩放
				if (getScale() != 1)
				{
				}
				//若不存在缩放
				else
				{
					pGameMap->setPositionX( getPositionX()*(pGameMap->GetSpeedMultipleX()-1) + _TMX_START_X_ );
					
				}

			}
		}

		CNFProtagonist* pProtagonist = dynamic_cast<CNFProtagonist*>(getChildByTag(enTagProtagonist));
		if(pProtagonist==NULL){
			SetProtagonist(NULL);
			SendMsg(enMsgCreateGameOverLayer,NULL,sizeof(NULL));
			
			//CCLog("=========%f====%f",getPositionX(),getPositionY());
		}

		return;
	} while (false);
	CCLog("Fun CNF3DWorldLayer::update Error!");
}

int CNF3DWorldLayer::ClickNPC( CCTouch *pTouch )
{
	CCObject *pItem = NULL;
	CCARRAY_FOREACH(getChildren(),pItem)
	{
		CNFNPC *pNodeItem = dynamic_cast<CNFNPC*>(pItem);
		if(pNodeItem!=NULL)
		{
			tagPosition3D wzNPC = pNodeItem->GetCurrentCube()->ptPos3D;
			tagSize3D szNPC = pNodeItem->GetCurrentCube()->szCube;

			CCPoint NPC2DPoint = ccp(wzNPC.fX + getPositionX() - 50,wzNPC.fY*pNodeItem->GetLTRate() + wzNPC.fZ);
			CCRect NPC2DRect = CCRectMake(NPC2DPoint.x,NPC2DPoint.y,szNPC.fLength,szNPC.fHeight);
			if(NPC2DPoint.x>0&&NPC2DRect.containsPoint(pTouch->getLocation())){
				//CCLog("%d==================%f",pNodeItem->GetID(),wzNPC.fX);
				CNFProtagonist* pRole = dynamic_cast<CNFProtagonist*>(getChildByTag(enTagProtagonist));
				if(pRole!=NULL){
					pRole->OnCtrlMoveToPonit(ccp(pNodeItem->GetCurrentCube()->ptPos3D.fX,pNodeItem->GetCurrentCube()->ptPos3D.fY));
				}
			}
			
		}
	}
	return 0;
}



void CNF3DWorldLayer::RecMsg( int enMsg,void* pData,int nSize )
{
	do 
	{
		switch(enMsg)
		{
		case enMsg_Hit_Enemy:
			{
				//发消息，得到被击中的敌人是谁
				SendMsg(enMsg_Hit_Enemy,pData,nSize);
			}break;
		case enMsg_RemoveEnemy:		
			{
				SendMsg(enMsg_RemoveEnemy,NULL,0);
				OnSubMsgEnemyRemove( pData,nSize );
			}break;
		case enMsg_RemoveProtagonist:		
			{
				OnSubMsgProtagonistRemove( pData,nSize );
			}break;
		case enMsgEnemyCommonAttack:		
			{
				OnSubMsgEnemyCommonAttack( pData,nSize );
			}break;
		case enMsgProtagonistSkill:		//消息处理:技能
			{
				OnSubMsgProtagonistSkill( pData,nSize );
			}break;
		case enMsgProtagonistTransfer:	//消息处理:传送门
			{
				OnSubMsgTransfer( pData,nSize );
			}break;
			//创建敌人
		case enMsgCreateEnemy:
			{
				OnSubMsgCreateEnemy(pData,nSize);
			}break;
			//游戏结束
		case enMsgGameWin:
			{
				OnSubMsgGameWin(pData,nSize);
			}
		}

		return ;
	} while (false);

}

void CNF3DWorldLayer::onExit()
{
	m_pArticleArray->release();
	m_pFriendArray->release();
	m_pEnemyArray->release();
	CNFWorld3DLayer::onExit();
}

bool CNF3DWorldLayer::OnSubMsgEnemyRemove( void *pInfo,int nSize )
{
	do 
	{
		//tagPosition3D ptPos3D;
		tagRemovePerson *pCmd = (tagRemovePerson*)pInfo;
		if(pCmd->pArrayTarget!=NULL){
			pCmd->pArrayTarget->removeObject(pCmd->pPerson);
		}else{
			m_pEnemyArray->removeObject(pCmd->pPerson);
		}

		this->removeChild(pCmd->pPerson);

		return true;
	} while (false);
	CCLog("Fun CNF3DWorldLayer::OnSubMsgEnemyRemove Error!");
	return false;
}

bool CNF3DWorldLayer::OnSubMsgProtagonistRemove( void *pInfo,int nSize )
{
	do 
	{
		tagRemovePerson *pCmd = (tagRemovePerson*)pInfo;
		if(pCmd->pArrayTarget!=NULL){
			pCmd->pArrayTarget->removeObject(pCmd->pPerson);
		}else{
			m_pFriendArray->removeObject(pCmd->pPerson);
		}
		
		this->removeChild(pCmd->pPerson);
		return true;
	} while (false);
	CCLog("Fun CNF3DWorldLayer::OnSubMsgProtagonistRemove Error!");
	return false;
}


bool CNF3DWorldLayer::OnSubMsgEnemyCommonAttack( void *pInfo,int nSize )
{
	do 
	{
		//return true;
		tagPosition3D ptPos3D;
		tagCommonAttack *pCmd = (tagCommonAttack*)pInfo;
		CNFCommonAttack* pAttack = CNFCommonAttack::CreateCommonAttack(pCmd,m_pFriendArray);
		if(pAttack!=NULL){
			ptPos3D.fX = pCmd->pReleaser->GetCurrentCube()->ptPos3D.fX + pCmd->fOldX;
			ptPos3D.fY = pCmd->pReleaser->GetCurrentCube()->ptPos3D.fY + pCmd->fOldY;
			ptPos3D.fZ = pCmd->pReleaser->GetCurrentCube()->ptPos3D.fZ + pCmd->fOldZ;
			pAttack->setPosition3D(ptPos3D);
			pAttack->SetGround(pCmd->pReleaser->GetGround());
			pAttack->SetMsg(this);
			this->addChild(pAttack,pCmd->pReleaser->getZOrder());
		}
		return true;
	} while (false);
	CCLog("Fun CNF3DWorldLayer::OnSubMsgEnemyCommonAttack Error!");
	return false;
}

bool CNF3DWorldLayer::OnSubMsgProtagonistSkill( void *pInfo,int nSize )
{
	do 
	{
		//return true;
		tagPosition3D ptPos3D;
		tagCommonAttack *pCmd = (tagCommonAttack*)pInfo;
		CNFCommonAttack* pAttack = CNFCommonAttack::CreateCommonAttack(pCmd,pCmd->pArrayTarget);
		if(pAttack!=NULL){
			ptPos3D.fX = pCmd->pReleaser->GetCurrentCube()->ptPos3D.fX + pCmd->fOldX;
			ptPos3D.fY = pCmd->pReleaser->GetCurrentCube()->ptPos3D.fY + pCmd->fOldY;
			ptPos3D.fZ = pCmd->pReleaser->GetCurrentCube()->ptPos3D.fZ + pCmd->fOldZ;
			pAttack->setPosition3D(ptPos3D);
			pAttack->SetGround(pCmd->pReleaser->GetGround());
			pAttack->SetMsg(this);
			this->addChild(pAttack,pCmd->pReleaser->getZOrder());
		}
		return true;
	} while (false);
	CCLog("Fun CNF3DWorldLayer::OnSubMsgProtagonistSkill Error!");
	return false;
}

bool CNF3DWorldLayer::OnSubMsgTransfer( void *pInfo,int nSize )
{
	do 
	{
		tagTransferDoor *pCmd = (tagTransferDoor*)pInfo;
		CC_BREAK_IF(pCmd->nID<=0);
		if(pCmd->nID>_NF_TOWN_OR_BATTLE_ID_ && pCmd->nID <_NF_TOWN_FB_ID_){
			//跳转到副本
			CCScene * pScene = CNFGameScene::CreateGameScene(pCmd->nID,m_nRoleID);
			CC_BREAK_IF(pScene==NULL);
			CCDirector::sharedDirector()->replaceScene(/*CCTransitionFade::create(1.f,pScene)*/pScene);

		}else if(pCmd->nID>0 && pCmd->nID <_NF_TOWN_OR_BATTLE_ID_)
		{
			//跳转到城镇
			CCScene * pScene = CNFTownScene::CreateTownScene(pCmd->nID,m_nRoleID);
			CC_BREAK_IF(pScene==NULL);
			CCDirector::sharedDirector()->replaceScene(/*CCTransitionFade::create(1.f,pScene)*/pScene);
		}
		else
		{
			SendMsg(enMsgCreateFbSelectLayer,&pCmd->nID,sizeof(pCmd->nID));
		}


		return true;
	} while (false);
	CCLog("Fun CNF3DWorldLayer::OnSubMsgTransferSkill Error!");
	return false;
}


//消息处理：创建敌人
bool CNF3DWorldLayer::OnSubMsgCreateEnemy( void *pInfo,int nSize )
{
	do 
	{
		Cmd_CreateEnemyInfo *pCmd = (Cmd_CreateEnemyInfo *)pInfo;

		CNFProtagonist* pProtagonist = dynamic_cast<CNFProtagonist*>(getChildByTag(enTagProtagonist));
		if(pProtagonist!=NULL){
			CNFEnemy* pEnemy = CNFEnemy::CreateEnemy(pCmd->nEnemyID,pCmd->nEnemyLevel,m_pEnemyArray,m_pFriendArray);
			CC_BREAK_IF(pEnemy==NULL);
			pEnemy->SetMsg(this);
			int nTempL = Get3DWorldSize()->fLength;
			int nTempW = Get3DWorldSize()->fWidth;
			tagPosition3D ptPos3D;
			ptPos3D.fX = pCmd->fPosX;
			ptPos3D.fY = pCmd->fPosY;
			ptPos3D.fZ = 0;
			pEnemy->setPosition3D(ptPos3D);
			pEnemy->SetArticleArray(m_pArticleArray);
			addChild(pEnemy,enZOrderMid);
			m_pEnemyArray->addObject(pEnemy);
		}

		return true;
	} while (false);
	CCLog("Fun CNF3DWorldLayer::OnSubMsgCreateEnemy Error!");
	return false;
}

//消息处理：游戏结束
bool CNF3DWorldLayer::OnSubMsgGameWin( void *pInfo,int nSize )
{
	do 
	{
		SendMsg(enMsgCreateGameWinLayer,NULL,sizeof(NULL));

		return true;
	} while (false);
	CCLog("Fun CNF3DWorldLayer::OnSubMsgGameWin Error!");
	return false;
}

