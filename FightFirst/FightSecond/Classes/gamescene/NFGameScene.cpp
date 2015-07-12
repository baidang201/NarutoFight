#include "NFGameScene.h"
#include "NFRocker.h"
#include "NF3DWorldLayer.h"
#include "../gameunit/person/NFProtagonist.h"
#include "../tiledmap/NFTMXTiledMap.h"
#include "../datamanager/NFDataManager.h"
#include "../ui/NFTownScene.h"

#define _NF_RUNSPACE_	7

CCScene* CNFGameScene::CreateGameScene(int nStageID,int nRoleID)
{
	do 
	{
		//建立场景
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		//建立层
		CNFGameScene* pLayer = CNFGameScene::CreateLayer(nStageID,nRoleID);
		CC_BREAK_IF(pLayer==NULL);

		//将层加入场景
		pScene->addChild(pLayer);
		return pScene;
	} while (false);
	CCLog("Fun GameScene::CreateScene Error!");
	return NULL;
}

CNFGameScene * CNFGameScene::CreateLayer(int nStageID,int nRoleID)
{
	CNFGameScene *pRet = new CNFGameScene(); 
	if (pRet && pRet->Init(nStageID,nRoleID)) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 

	delete pRet; 
	pRet = NULL; 
	return NULL; 	
}

bool CNFGameScene::Init(int nStageID,int nRoleID)
{
	do 
	{
		//srand((unsigned)time(NULL));
		//初始化父类
		CC_BREAK_IF(CCLayer::init()==false);

		m_nStageID = nStageID;
		m_nRoleID = nRoleID;

		//键盘
		m_bIsPressW = false;
		m_bIsPressA = false;
		m_bIsPressS = false;
		m_bIsPressD = false;
		m_bIsPressJ = false;
		m_bIsPressK = false;
		m_bIsPressL = false;
		m_bIsPressI = false;
		m_nRunSpace = _NF_RUNSPACE_;
		m_fFirectionOld = -1;
		m_bIsRun = false;

		m_nHitNum = 0;					//连击数
		m_nHitTime = 60;				//连击间隔时间
		m_nHitTimeTemp = -1;			//当前连击时间
		m_bBtnListSwitchMoveOver=true;
		m_bBtnListSwitch=false;

		//创建3D世界
		CNF3DWorldLayer * pLayer = CNF3DWorldLayer::CreateLayer(m_nStageID,m_nRoleID);
		CC_BREAK_IF(pLayer==NULL);
		pLayer->SetMsg(this);
		addChild(pLayer,enZOrderBack,enTag3D);

		//创建摇杆
		CNFRockerLayer * pRocker = CNFRockerLayer::CreateLayer(pLayer,m_nStageID);
		CC_BREAK_IF(pRocker==NULL);
		addChild(pRocker,enZOrderFront,enTagRocker);

		//创建血条背景
		CCSprite * pBloodBG = CCSprite::create("gameui/bg_status.png");
		CC_BREAK_IF(pBloodBG==NULL);
		pBloodBG->setPosition(ccp(175,SCREEN_HEIGHT-55));
		addChild(pBloodBG,enZOrderMid);

		//创建主角血条
		CCSprite* pSpr2 = CCSprite::create("gameui/value_hp.png");
		CC_BREAK_IF(pSpr2==NULL);
		CCProgressTimer* pProgress2 = CCProgressTimer::create(pSpr2);
		CC_BREAK_IF(pProgress2==NULL);
		pProgress2->setType(kCCProgressTimerTypeBar);
		pProgress2->setMidpoint(ccp(0.f,0.f));
		pProgress2->setBarChangeRate(ccp(1.f,0.f));
		pProgress2->setPercentage(100.f);
		pProgress2->setPosition(ccp(pBloodBG->getPositionX()+15,pBloodBG->getPositionY()+18));
		this->addChild(pProgress2,enZOrderFront,enTagHPProgress2);
		pProgress2->setOpacity(150.f);

		CCSprite* pSpr1 = CCSprite::create("gameui/value_hp.png");
		CC_BREAK_IF(pSpr1==NULL);
		CCProgressTimer* pProgress1 = CCProgressTimer::create(pSpr1);
		CC_BREAK_IF(pProgress1==NULL);
		pProgress1->setType(kCCProgressTimerTypeBar);
		pProgress1->setMidpoint(ccp(0.f,0.f));
		pProgress1->setBarChangeRate(ccp(1.f,0.f));
		pProgress1->setPercentage(100.f);
		pProgress1->setPosition(pProgress2->getPosition());
		this->addChild(pProgress1,enZOrderFront,enTagHPProgress1);

		//创建主角查克拉
		CCSprite* pSprChakela2 = CCSprite::create("gameui/value_magic.png");
		CC_BREAK_IF(pSprChakela2==NULL);
		CCProgressTimer* pProgressChakela2 = CCProgressTimer::create(pSprChakela2);
		CC_BREAK_IF(pProgressChakela2==NULL);
		pProgressChakela2->setType(kCCProgressTimerTypeBar);
		pProgressChakela2->setMidpoint(ccp(0.f,0.f));
		pProgressChakela2->setBarChangeRate(ccp(1.f,0.f));
		pProgressChakela2->setPercentage(100.f);
		pProgressChakela2->setPosition(ccp(pBloodBG->getPositionX()+7,pBloodBG->getPositionY()-1));
		this->addChild(pProgressChakela2,enZOrderFront,enTagMPProgressChakela2);
		pProgressChakela2->setOpacity(150.f);

// 		CCSprite* pSprChakela1 = CCSprite::create("gameui/value_magic.png");
// 		CC_BREAK_IF(pSprChakela1==NULL);
// 		CCProgressTimer* pProgressChakela1 = CCProgressTimer::create(pSprChakela1);
// 		CC_BREAK_IF(pProgressChakela1==NULL);
// 		pProgressChakela1->setType(kCCProgressTimerTypeBar);
// 		pProgressChakela1->setMidpoint(ccp(0.f,0.f));
// 		pProgressChakela1->setBarChangeRate(ccp(1.f,0.f));
// 		pProgressChakela1->setPercentage(0.f);
// 		pProgressChakela1->setPosition(pProgressChakela2->getPosition());
// 		this->addChild(pProgressChakela1,enZOrderFront,enTagMPProgressChakela1);

		//创建主角头像
		CCSprite * pProgonistHead = CCSprite::create("gameui/head_zuozhu.png");
		CC_BREAK_IF(pProgonistHead==NULL);
		pProgonistHead->setPosition(ccp(pBloodBG->getPositionX()-100,pBloodBG->getPositionY()+10));
		addChild(pProgonistHead,enZOrderMid);

		m_fNowHPProgress2 = 100.f;
		m_pTargetEnemy = NULL;
		m_nEnemyBloodVisibleTime = 0;
		m_nEnemyBloodVisibleTimeTemp = 200;

		//添加敌人血条背景
		CCSprite* pEnemyBloodBG = CCSprite::create("gameui/value_enemy_hp_bg.png");
		CC_BREAK_IF(pEnemyBloodBG==NULL);
		pEnemyBloodBG->setPosition(ccp(SCREEN_WIDTH-60,SCREEN_HEIGHT-90));
		this->addChild(pEnemyBloodBG,enZOrderMid,enTagHPEnemyProgressBg);
		pEnemyBloodBG->setColor(ccBLACK);
		pEnemyBloodBG->setVisible(false);

		CCSprite* pEnemySpr2 = CCSprite::create("gameui/value_enemy_hp.png");
		CC_BREAK_IF(pEnemySpr2==NULL);
		CCProgressTimer* pEnemyProgress2 = CCProgressTimer::create(pEnemySpr2);
		CC_BREAK_IF(pEnemyProgress2==NULL);
		pEnemyProgress2->setType(kCCProgressTimerTypeBar);
		pEnemyProgress2->setMidpoint(ccp(0.f,0.f));
		pEnemyProgress2->setBarChangeRate(ccp(1.f,0.f));
		pEnemyProgress2->setPercentage(100.f);
		pEnemyProgress2->setPosition(pEnemyBloodBG->getPosition());
		this->addChild(pEnemyProgress2,enZOrderFront,enTagHPEnemyProgress2);
		pEnemyProgress2->setOpacity(150.f);
		pEnemyProgress2->setVisible(false);

		CCSprite* pEnemySpr1 = CCSprite::create("gameui/value_enemy_hp.png");
		CC_BREAK_IF(pEnemySpr1==NULL);
		CCProgressTimer* pEnemyProgress1 = CCProgressTimer::create(pEnemySpr1);
		CC_BREAK_IF(pEnemyProgress1==NULL);
		pEnemyProgress1->setType(kCCProgressTimerTypeBar);
		pEnemyProgress1->setMidpoint(ccp(0.f,0.f));
		pEnemyProgress1->setBarChangeRate(ccp(1.f,0.f));
		pEnemyProgress1->setPercentage(100.f);
		pEnemyProgress1->setPosition(pEnemyBloodBG->getPosition());
		this->addChild(pEnemyProgress1,enZOrderFront,enTagHPEnemyProgress1);
		pEnemyProgress1->setVisible(false);

		m_fNowHPProgress2 = 100.f;
		m_fNowEnemyHpProgress2 = 100.f;
		m_pTargetEnemy = NULL;


		/************************************************************************/
		/*					创建菜单                                                                     */
		/************************************************************************/
		//创建按钮
		CCMenu * pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(CCPointZero);
		addChild(pMenu,enZOrderFront+100,enTagMenu);

		//若为副本，则创建自动AI按钮
		if (nStageID >= _NF_TOWN_OR_BATTLE_ID_ && nStageID <_NF_TOWN_FB_ID_)
		{
			//临时：创建主角AI控制按钮
			CCMenuItemLabel * pBtnProAICtrl = CCMenuItemLabel::create(CCLabelTTF::create("Manual","Arial",20),this,menu_selector(CNFGameScene::OnBtnCallBack));
			CC_BREAK_IF(pBtnProAICtrl==NULL);
			pBtnProAICtrl->setPosition(ccp(SCREEN_WIDTH-60.f,SCREEN_HEIGHT-40.f));
			pMenu->addChild(pBtnProAICtrl,enZOrderFront,enTagBtnProAICtrl);

			//临时：创建返回城镇按钮
			CCMenuItemLabel * pBtnBackToTown = CCMenuItemLabel::create(CCLabelTTF::create("Town","Arial",20),this,menu_selector(CNFGameScene::OnBtnCallBack));
			CC_BREAK_IF(pBtnBackToTown==NULL);
			pBtnBackToTown->setPosition(ccp(60.f,350));
			pMenu->addChild(pBtnBackToTown,enZOrderFront,enTagBtnBackToTown);

		}

		m_nProgonistLevel = 1;
		m_fProgonistExp = 0;
		tagLevelUpExpStaticInfo LevelUpInfo;
		CNFStaticDataManager::SharedData()->GetLevelUpExpStaticInfo(m_nProgonistLevel,LevelUpInfo);
		m_fProgonistExpLevel = LevelUpInfo.fLevelUpExp;

		//创建等级数字
		char szName[NAME_LEN] = {0};
		sprintf(szName,"%d",m_nProgonistLevel);
		CCLabelTTF * pLevel = CCLabelTTF::create(szName,"Arial",18);
		CC_BREAK_IF(pLevel==NULL);
		pLevel->setPosition(ccp(pBloodBG->getPositionX()-130,pBloodBG->getPositionY()-32));
		addChild(pLevel,enZOrderFront+90,enTagLevel);

		//创建经验进度条
		CCSprite* pExpSpr = CCSprite::create("gameui/value_exp.png");
		CC_BREAK_IF(pExpSpr==NULL);
		CCProgressTimer* pExpPro = CCProgressTimer::create(pExpSpr);
		CC_BREAK_IF(pExpPro==NULL);
		pExpPro->setType(kCCProgressTimerTypeBar);
		pExpPro->setMidpoint(ccp(0.f,0.f));
		pExpPro->setBarChangeRate(ccp(1.f,0.f));
		pExpPro->setPercentage(0.f);
		pExpPro->setPosition(ccp(86,SCREEN_HEIGHT - 94.f));
		this->addChild(pExpPro,enZOrderFront+95,enTagExpPro);
		
		//连击数数字
		sprintf(szName,"%dhit",m_nHitNum);
		CCLabelBMFont* pHitTime = CCLabelBMFont::create(szName,"fonts/futura-48.fnt");
		CC_BREAK_IF(!pHitTime);
		pHitTime->setPosition(ccp(90,SCREEN_HEIGHT*0.7f));
		addChild(pHitTime,enZOrderFront,enTagHitNum);
		pHitTime->setVisible(false);

		//注册Update函数
		this->schedule(schedule_selector(CNFGameScene::OnCallPerFrame));


		return true;
	} while (false);
	CCLog("Fun CMGameScene::init Error!");
	return false;
}


void CNFGameScene::OnCallPerFrame(float dt)
{
	do 
	{
		//得到3D世界层
		CNF3DWorldLayer * pLayer = dynamic_cast<CNF3DWorldLayer *>(getChildByTag(enTag3D));
		CC_BREAK_IF(pLayer==NULL);
		pLayer->update(dt);

		////=============================================================================键盘
		

		
		//if((GetAsyncKeyState(0x4a) & 0x8000 ? 1 : 0)){
		//	if(m_bIsPressJ==false){
		//		CCLog("j");
		//		CNFProtagonist* pSprite = dynamic_cast<CNFProtagonist*>(pLayer->getChildByTag(CNF3DWorldLayer::enTagProtagonist));
		//		if(pSprite!=NULL){
		//			pSprite->OnCtrlCommonAttack();
		//		}
		//	}
		//}m_bIsPressJ = (GetAsyncKeyState(0x4a) & 0x8000 ? 1 : 0);

		//if((GetAsyncKeyState(0x4b) & 0x8000 ? 1 : 0)){
		//	if(m_bIsPressK==false){
		//		CCLog("k");
		//		CNFProtagonist* pSprite = dynamic_cast<CNFProtagonist*>(pLayer->getChildByTag(CNF3DWorldLayer::enTagProtagonist));
		//		if(pSprite!=NULL){
		//			pSprite->ProtagonistJump();
		//		}
		//	}
		//}m_bIsPressK = (GetAsyncKeyState(0x4b) & 0x8000 ? 1 : 0);

		//if((GetAsyncKeyState(0x4c) & 0x8000 ? 1 : 0)){
		//	if(m_bIsPressL==false){
		//		CCLog("l");
		//		CNFProtagonist* pSprite = dynamic_cast<CNFProtagonist*>(pLayer->getChildByTag(CNF3DWorldLayer::enTagProtagonist));
		//		if(pSprite!=NULL){
		//			pSprite->OnCtrlSkill1();
		//		}
		//	}
		//}m_bIsPressL = (GetAsyncKeyState(0x4c) & 0x8000 ? 1 : 0);

		//if((GetAsyncKeyState(0x49) & 0x8000 ? 1 : 0)){
		//	if(m_bIsPressI==false){
		//		CCLog("i");
		//		CNFProtagonist* pSprite = dynamic_cast<CNFProtagonist*>(pLayer->getChildByTag(CNF3DWorldLayer::enTagProtagonist));
		//		if(pSprite!=NULL){
		//			pSprite->OnCtrlSkill3();
		//		}
		//	}
		//}m_bIsPressI = (GetAsyncKeyState(0x49) & 0x8000 ? 1 : 0);

		//CNFProtagonist* pSprite = dynamic_cast<CNFProtagonist*>(pLayer->getChildByTag(CNF3DWorldLayer::enTagProtagonist));
		//if(pSprite!=NULL){
		//	if(fMoveRate<0){
		//		CNFRockerLayer * pRocker = dynamic_cast<CNFRockerLayer*>(this->getChildByTag(enTagRocker));
		//		if(pRocker!=NULL){
		//			if(pRocker->GetIsTouching()==false){
		//				pSprite->SetStop();
		//			}
		//		}
		//	}else{
		//		if(m_bIsRun){
		//			pSprite->SetRunByRotation(fMoveRate);
		//		}else{
		//			pSprite->SetMoveByRotation(fMoveRate);
		//		}
		//	}
		//}
		//=============================================================================键盘end


		//同步主角血条
		CNFProtagonist * pProtagonist = dynamic_cast<CNFProtagonist *>(pLayer->getChildByTag(enTagProtagonist));
		if(pProtagonist!=NULL){
			CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPProgress1));
			if(pProgress!=NULL){
				pProgress->setPercentage(pProtagonist->GetHPPercent()*100);
			}
			CCProgressTimer* pProgress2 = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagMPProgressChakela2));
			if(pProgress2!=NULL){
				pProgress2->setPercentage(pProtagonist->GetMPPercent()*100);
			}
		}
		if(pProtagonist!=NULL)
		{
			if((pProtagonist->GetHPPercent()*100)<m_fNowHPProgress2){
				m_fNowHPProgress2 -= 0.5;
			}else{
				m_fNowHPProgress2 = (pProtagonist->GetHPPercent()*100);

				CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPProgress1));
				if(pProgress!=NULL){
					pProgress->setPercentage(pProtagonist->GetHPPercent()*100);
				}
			}
			if(m_fNowHPProgress2<0){m_fNowHPProgress2 = 0;}
			CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPProgress2));
			if(pProgress!=NULL)
			{
				pProgress->setPercentage(m_fNowHPProgress2);
			}
		}

		//敌人血条
		if (m_nEnemyBloodVisibleTime > 0)
		{
			m_nEnemyBloodVisibleTime--;
		}
		else
		{
			CCProgressTimer* pProgress1 = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPEnemyProgress1));
			CCProgressTimer* pProgress2 = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPEnemyProgress2));
			CCSprite* pEnemyBloodBG = dynamic_cast<CCSprite *>(getChildByTag(enTagHPEnemyProgressBg));
			pProgress1->setVisible(false);
			pProgress2->setVisible(false);
			pEnemyBloodBG->setVisible(false);
		}

		//同步被击中的敌人血条
		if (m_pTargetEnemy!=NULL&&m_pTargetEnemy->IsDeath()==false)
		{
			CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPEnemyProgress1));
			if(pProgress!=NULL){
				pProgress->setPercentage(m_pTargetEnemy->GetHPPercent()*100);
			}
		}
		if (m_pTargetEnemy!=NULL&&m_pTargetEnemy->IsDeath()==false)
		{

			if((m_pTargetEnemy->GetHPPercent()*100)<m_fNowEnemyHpProgress2){
				m_fNowEnemyHpProgress2 -= 0.5;
			}else{
				m_fNowEnemyHpProgress2 = (m_pTargetEnemy->GetHPPercent()*100);

				CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPEnemyProgress1));
				if(pProgress!=NULL){
					pProgress->setPercentage(m_pTargetEnemy->GetHPPercent()*100);
				}
			}
			if(m_fNowHPProgress2<0){m_fNowEnemyHpProgress2 = 0;}
			CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPEnemyProgress2));
			if(pProgress!=NULL)
			{
				pProgress->setPercentage(m_fNowEnemyHpProgress2);
			}
		}

		//连击
		if (m_nHitTimeTemp > 0)
		{
			m_nHitTimeTemp--;
		}
		else if (m_nHitTimeTemp==0)
		{
			//连击结束
			CCLabelBMFont* pHitTime = dynamic_cast<CCLabelBMFont *>(getChildByTag(enTagHitNum));
			if (pHitTime!=NULL)
			{
				pHitTime->setVisible(false);
			}
			//显示连击结果
			ShowHitResult();
		}
		return;
	} while (false);
	CCLog("fun CNFGameScene::Update Error!");
}

void CNFGameScene::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -127, false);
}

void CNFGameScene::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

bool CNFGameScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void CNFGameScene::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	
}

void CNFGameScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//得到3D世界层
		CNF3DWorldLayer * pLayer = dynamic_cast<CNF3DWorldLayer *>(getChildByTag(enTag3D));
		CC_BREAK_IF(pLayer==NULL);
		pLayer->ClickNPC(pTouch);

		return;
	} while (false);
	return;
}

void CNFGameScene::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{

}

void CNFGameScene::RecMsg( int enMsg,void* pData,int nSize )
{
	do 
	{
		switch (enMsg)
		{
		case enMsg_Hit_Enemy:		//击中敌人
			{
				CNFBasicPerson * pPerson = (CNFBasicPerson *)pData;
				CNFEnemy * pEnemy = dynamic_cast<CNFEnemy *>(pPerson);
				if (pEnemy!=NULL)
				{  
					CCProgressTimer* pProgress1 = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPEnemyProgress1));
					CCProgressTimer* pProgress2 = dynamic_cast<CCProgressTimer*>(this->getChildByTag(enTagHPEnemyProgress2));
					CCSprite* pEnemyBloodBG = dynamic_cast<CCSprite *>(getChildByTag(enTagHPEnemyProgressBg));
					//pProgress1->setVisible(true);
					//pProgress2->setVisible(true);
					//pEnemyBloodBG->setVisible(true);

					m_pTargetEnemy = pEnemy;
					m_nEnemyBloodVisibleTime = m_nEnemyBloodVisibleTimeTemp;
					m_fNowEnemyHpProgress2 = m_pTargetEnemy->GetHP2Percent()*100;

					//若在连击时间内
					if (m_nHitTimeTemp > 0)
					{
						m_nHitNum ++;
					}
					else	
					{
						m_nHitNum = 1;
					}
					
					//重置连击时间
					m_nHitTimeTemp = m_nHitTime;

					//连击显示
					CCLabelBMFont* pHitTime = dynamic_cast<CCLabelBMFont *>(getChildByTag(enTagHitNum));
					if (pHitTime!=NULL)
					{
						char szName[NAME_LEN] = {0};
						sprintf(szName,"%dhit",m_nHitNum);
						pHitTime->setString(szName);
						pHitTime->setVisible(true);
					}
				}

			}break;
		case enMsg_RemoveEnemy:
			{
				m_fProgonistExp += 20;

				//若升级
				if (m_fProgonistExp >= m_fProgonistExpLevel)
				{
					m_fProgonistExp -= m_fProgonistExpLevel;
					m_nProgonistLevel ++;

					//显示数字
					char szName[NAME_LEN] = {0};
					sprintf(szName,"%d",m_nProgonistLevel);
					CCLabelTTF * pLevel = dynamic_cast<CCLabelTTF *>(getChildByTag(enTagLevel));
					CC_BREAK_IF(pLevel==NULL);
					pLevel->setString(szName);

					tagLevelUpExpStaticInfo LevelUpExpInfo;
					CNFStaticDataManager::SharedData()->GetLevelUpExpStaticInfo(m_nProgonistLevel,LevelUpExpInfo);
					m_fProgonistExpLevel = LevelUpExpInfo.fLevelUpExp;
				}

				//经验进度条
				CCProgressTimer* pExpPro = dynamic_cast<CCProgressTimer *>(getChildByTag(enTagExpPro));
				CC_BREAK_IF(pExpPro==NULL);
				pExpPro->setPercentage(m_fProgonistExp/m_fProgonistExpLevel*100);


			}break;
		case enMsgCreateGameWinLayer:
			{
				OnSubMsgGameWin(pData,nSize);
			}break;
		case enMsgCreateGameOverLayer:
			{
				OnSubMsgGameOver(pData,nSize);
			}break;
		}

		return ;
	} while (false);
}



void CNFGameScene::ShowHitResult()
{
	do 
	{
		//连击结果
		CCLabelBMFont* pOldHitResult = dynamic_cast<CCLabelBMFont *>(getChildByTag(enTagHitResult));
		if (pOldHitResult!=NULL)
		{
			removeChildByTag(enTagHitResult,true);
		}

		//5次连击以上，才显示
		if (m_nHitNum >= 5)
		{
			char szName[NAME_LEN] = {0};
			sprintf(szName,"MAX%dHIT",m_nHitNum);
			CCLabelBMFont* pHitResult = CCLabelBMFont::create(szName,"fonts/futura-48.fnt");
			CC_BREAK_IF(!pHitResult);
			pHitResult->setPosition(ccp(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT*0.8f));
			addChild(pHitResult,enZOrderFront,enTagHitResult);
			pHitResult->setOpacity(0.f);

			CCFiniteTimeAction * pSpawnAction = CCSpawn::create(
				CCMoveBy::create(0.1f,ccp(0,40)),
				CCScaleBy::create(0.1f,1.1f),
				CCFadeIn::create(0.1f),
				NULL);
			CC_BREAK_IF(pSpawnAction==NULL);

			CCAction * pSequenceAction = CCSequence::create(
				pSpawnAction,
				CCDelayTime::create(1.f),
				CCCallFunc::create(this,callfunc_selector(CNFGameScene::OnHitResultOverCallBack)),
				NULL);
			CC_BREAK_IF(pSequenceAction==NULL);

			pHitResult->runAction(pSequenceAction);
		}

		m_nHitTimeTemp = -1;

		return ;
	} while (false);
	CCLog("Fun CNFGameScene::ShowHitResult Error!");
}

void CNFGameScene::OnHitResultOverCallBack()
{
	do 
	{
		//连击结果
		CCLabelBMFont* pOldHitResult = dynamic_cast<CCLabelBMFont *>(getChildByTag(enTagHitResult));
		if (pOldHitResult!=NULL)
		{
			removeChildByTag(enTagHitResult,true);
		}

		return ;
	} while (false);
	CCLog("Fun CNFGameScene::OnHitResultOverCallBack Error!");
}


void CNFGameScene::OnBtnCallBack( CCObject *pObj )
{
	do 
	{
		CCMenu * pMenu = dynamic_cast<CCMenu *>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenu==NULL);

		int nTag = (dynamic_cast<CCMenuItemLabel *>(pObj))->getTag();

		switch (nTag)
		{
		case enTagBtnProAICtrl:
			{
				CCMenuItemLabel * pBtnProAICtrl = dynamic_cast<CCMenuItemLabel *>(pMenu->getChildByTag(enTagBtnProAICtrl));
				CC_BREAK_IF(pBtnProAICtrl==NULL);

				CCLabelTTF * pLabel = dynamic_cast<CCLabelTTF *>(pBtnProAICtrl->getLabel());
				CC_BREAK_IF(pLabel==NULL);

				//得到3D世界层
				CNF3DWorldLayer * pLayer = dynamic_cast<CNF3DWorldLayer *>(getChildByTag(enTag3D));
				CC_BREAK_IF(pLayer==NULL);

				CNFProtagonist * pProtagonist = dynamic_cast<CNFProtagonist *>(pLayer->getChildByTag(enTagProtagonist));
				if(pProtagonist!=NULL)
				{
					//若当前为手动
					if ( strcmp(pLabel->getString(),"Manual")==0 )
					{
						pLabel->setString("Auto");
						pProtagonist->SetAutoMode(true);
					}
					//若当前为自动
					else if ( strcmp(pLabel->getString(),"Auto")==0 )
					{
						pLabel->setString("Manual");
						pProtagonist->SetAutoMode(false);
					}
				}
			}break;
		case enTagBtnBackToTown:
			{
				//跳转到城镇
				CCScene * pScene = CNFTownScene::CreateTownScene(1,m_nRoleID);
				CC_BREAK_IF(pScene==NULL);
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));
			}break;
		}

		return ;
	} while (false);
	CCLog("Fun CNFGameScene::OnBtnCallBack Error!");
}

bool CNFGameScene::OnSubMsgGameWin( void *pInfo,int nSize )
{
	do 
	{
		if(getChildByTag(enTagGameWinLayer))return false;

		//CNFTipLayer* pWinLayer = CNFTipLayer::CreateLayer(1);
		//CC_BREAK_IF(pWinLayer==NULL);
		//addChild(pWinLayer,enZOrderFront+10000,enTagGameWinLayer);

		//创建主场景UI层
		UILayer* pWinLayer = UILayer::create(); 
		CC_BREAK_IF(pWinLayer==NULL);

		//加入主场景UI
		UILayout *pFb_Win = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Fb_Win.json"));
		CC_BREAK_IF(pFb_Win==NULL);
		pWinLayer->addWidget(pFb_Win);
		pFb_Win->setName("pFb_Win");
		pFb_Win->setSize(getContentSize());

		addChild(pWinLayer,enZOrderFront+10000,enTagGameWinLayer);

		//回城按钮回调
		UIButton* pButton_back_town =dynamic_cast<UIButton*>(pFb_Win->getChildByName("Button_back_town"));
		CC_BREAK_IF(pButton_back_town==NULL);
		pButton_back_town->addTouchEventListener(this,toucheventselector(CNFGameScene::OnReturnToTownClick));

		return true;
	} while (false);
	CCLog("Fun CNFGameScene::OnSubMsgGameWin Error!");
	return false;
}

bool CNFGameScene::OnSubMsgGameOver( void *pInfo,int nSize )
{
	do 
	{
		if(getChildByTag(enMsgGameOverLayer))return false;

		//CNFTipLayer* pWinLayer = CNFTipLayer::CreateLayer(0);
		//CC_BREAK_IF(pWinLayer==NULL);
		//addChild(pWinLayer,enZOrderFront+10000,enMsgGameOverLayer);

		//创建主场景UI层
		UILayer* pLoseLayer = UILayer::create(); 
		CC_BREAK_IF(pLoseLayer==NULL);

		//加入主场景UI
		UILayout *pFb_Lose = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Fb_Lose.json"));
		CC_BREAK_IF(pFb_Lose==NULL);
		pLoseLayer->addWidget(pFb_Lose);
		pFb_Lose->setSize(getContentSize());
		pFb_Lose->setName("Fb_Lose");
		addChild(pLoseLayer,enZOrderFront+10000,enMsgGameOverLayer);


		//回城按钮回调
		UIButton* pButton_back_town =dynamic_cast<UIButton*>(pFb_Lose->getChildByName("Button_back_town"));
		CC_BREAK_IF(pButton_back_town==NULL);
		pButton_back_town->addTouchEventListener(this,toucheventselector(CNFGameScene::OnReturnToTownClick));

		return true;
	} while (false);
	CCLog("Fun CNFGameScene::OnSubMsgGameOver Error!");
	return false;
}

void CNFGameScene::OnReturnToTownClick( CCObject* pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				//跳转到城镇
				CCScene * pScene = CNFTownScene::CreateTownScene(1,m_nRoleID);
				CC_BREAK_IF(pScene==NULL);
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));

			}break;
		}
		return ;
	} while (false);
	CCLog("Fun CNFGameScene::OnReturnToTownClick Error!");
}
