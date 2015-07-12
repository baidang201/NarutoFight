#include "NFTownScene.h"
#include "../gamescene/NFRocker.h"
#include "../gamescene/NF3DWorldLayer.h"
#include "../gameunit/person/NFProtagonist.h"
#include "../tiledmap/NFTMXTiledMap.h"
#include "../datamanager/NFDataManager.h"
#include "../gamescene/NFGameScene.h"
#include "../ui/NFLoginScene.h"
#include "../ui/NFRoleSelectLayer.h"
#include "./datamanager/NFServerDataManager.h"
#include "support\zip_support\ZipUtils.h"
#include "./datamanager/NFDataManager.h"

#define _NF_RUNSPACE_	7

CCScene* CNFTownScene::CreateTownScene(int nStageID,int nRoleID)
{
	do 
	{
		//建立场景
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		//建立层
		CNFTownScene* pLayer = CNFTownScene::CreateLayer(nStageID,nRoleID);
		CC_BREAK_IF(pLayer==NULL);

		//将层加入场景
		pScene->addChild(pLayer);
		return pScene;
	} while (false);
	CCLog("Fun CNFTownScene::CreateTownScene Error!");
	return NULL;
}

CNFTownScene * CNFTownScene::CreateLayer(int nStageID,int nRoleID)
{
	CNFTownScene *pRet = new CNFTownScene(); 
	if (pRet && pRet->Init(nStageID,nRoleID)) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 

	delete pRet; 
	pRet = NULL; 
	return NULL; 	
}

bool CNFTownScene::Init(int nStageID,int nRoleID)
{
	do 
	{

		ZipUtils::ccSetPvrEncryptionKeyPart(0,0x11111111);
		ZipUtils::ccSetPvrEncryptionKeyPart(1,0x22222222);
		ZipUtils::ccSetPvrEncryptionKeyPart(2,0x33333333);
		ZipUtils::ccSetPvrEncryptionKeyPart(3,0xaaaaaaaa);
		//srand((unsigned)time(NULL));
		//初始化父类
		CC_BREAK_IF(CCLayer::init()==false);

		//注册设备按键
		setKeypadEnabled(true);
		
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

		m_bBtnListSwitchMoveOver = true;
		m_bBtnListSwitchOpen = false;
	
		m_bBtnFriendSwitchMoveOver = true;
		m_bBtnFriendSwitchOpen = false;	
	
		m_nStageID = nStageID;
		m_nRoleID = nRoleID;

		//创建3D世界
		CNF3DWorldLayer * pLayer = CNF3DWorldLayer::CreateLayer(m_nStageID,nRoleID);
		CC_BREAK_IF(pLayer==NULL);
		pLayer->SetMsg(this);
		addChild(pLayer,enZOrderBack,enTag3D);

		//创建摇杆
		CNFRockerLayer * pRocker = CNFRockerLayer::CreateLayer(pLayer,m_nStageID);
		CC_BREAK_IF(pRocker==NULL);
		addChild(pRocker,enZOrderBack,enTagRocker);


		//创建主场景UI层
		UILayer* pUiLayer = UILayer::create(); 
		CC_BREAK_IF(pUiLayer==NULL);

		//-------------------加入主场景UI----------------------------
		UILayout *pMainSceneWidget_Part_1 = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("MainSceneUi_1.json"));
		CC_BREAK_IF(pMainSceneWidget_Part_1==NULL);
		pUiLayer->addWidget(pMainSceneWidget_Part_1);
		pMainSceneWidget_Part_1->setName("MainSceneWidget_Part_1");
		pMainSceneWidget_Part_1->setSize(getContentSize());


		//设置领奖按钮回调
		UIButton* pBtnGetReward = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Get_Reward"));
		CC_BREAK_IF(pBtnGetReward==NULL);
		pBtnGetReward->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToGetReward));

		//设置宝箱按钮回调
		UIButton* pBtnGoldReward = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Gold_reward"));
		CC_BREAK_IF(pBtnGoldReward==NULL);
		pBtnGoldReward->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToGoldReward));

		//设置擂台按钮回调
		UIButton* pBtnPvP = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_PvP"));
		CC_BREAK_IF(pBtnPvP==NULL);
		pBtnPvP->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToPvP));

		//设置活动按钮回调
		UIButton* pBtnActive = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Active"));
		CC_BREAK_IF(pBtnActive==NULL);
		pBtnActive->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToActive));

		//设置vip按钮回调
		UIButton* pBtnvip = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Vip"));
		CC_BREAK_IF(pBtnvip==NULL);
		pBtnvip->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToVip));

		//设置幸运抽奖按钮回调
		UIButton* pBtnDraw = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Button_45"));
		CC_BREAK_IF(pBtnDraw==NULL);
		pBtnDraw->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToDraw));

		//设置好友副本按钮回调
		UIButton* pBtnFb = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Firend_Fb"));
		CC_BREAK_IF(pBtnFb==NULL);
		pBtnFb->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToFirendFb));

		//设置神秘商人按钮回调
		UIButton* pBtnShop = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Shop"));
		CC_BREAK_IF(pBtnShop==NULL);
		pBtnShop->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToShop));

		//设置幻兽竞技场按钮回调
		UIButton* pBtnPiePvp = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Pie_PvP"));
		CC_BREAK_IF(pBtnPiePvp==NULL);
		pBtnPiePvp->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToPiePvP));

		//设置邮件按钮回调
		UIButton* pBtnEmail = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Email"));
		CC_BREAK_IF(pBtnEmail==NULL);
		pBtnEmail->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToEmail));

		//设置小助手按钮回调
		UIButton* pBtnHelp = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Help"));
		CC_BREAK_IF(pBtnHelp==NULL);
		pBtnHelp->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToHelp));

		//设置对话框按钮回调
		UIButton* pBtntalk = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_talk"));
		CC_BREAK_IF(pBtntalk==NULL);
		pBtntalk->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchTotalk));

		//设置技能升级按钮回调
		UIButton* pBtnSkill_up = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Skill_up"));
		CC_BREAK_IF(pBtnSkill_up==NULL);
		pBtnSkill_up->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToSkillUp));

		//设置装备改造按钮回调
		UIButton* pBtnEqu_up = dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Equ_up"));
		CC_BREAK_IF(pBtnEqu_up==NULL);
		pBtnEqu_up->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToEquUp));

		//临时：进入战场按钮回调
		UIButton* pBtnBattle =dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Task"));
		CC_BREAK_IF(pBtnBattle==NULL);
		pBtnBattle->addTouchEventListener(this,toucheventselector(CNFTownScene::OnBtnBattleCallBack));

		//进入精英副本回调
		UIButton* pBtnEx_Fb =dynamic_cast<UIButton*>(pMainSceneWidget_Part_1->getChildByName("Btn_Ex_Fb"));
		CC_BREAK_IF(pBtnEx_Fb==NULL);
		pBtnEx_Fb->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToExFbPage));



		//-------------------加入主场景UI扩展--------------------------
		UILayout *pMainSceneWidget_Part_2 = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("MainSceneUi_2.json"));
		CC_BREAK_IF(pMainSceneWidget_Part_2==NULL);
		pUiLayer->addWidget(pMainSceneWidget_Part_2);
		pMainSceneWidget_Part_2->setSize(getContentSize());
		pMainSceneWidget_Part_2->setName("MainSceneWidget_Part_2");

		//设置扩展按钮栏开关按钮回调
		UIButton* pBtnSwitch=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_Swich_New"));
		CC_BREAK_IF(pBtnSwitch==NULL);
		pBtnSwitch->addTouchEventListener(this,toucheventselector(CNFTownScene::OnExUiBtnListSwitch));

		//设置人物按钮回调
		UIButton* pBtnPlayerInfo=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_List_Panel")->getChildByName("Btn_Player"));
		CC_BREAK_IF(pBtnPlayerInfo==NULL);
		pBtnPlayerInfo->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToPlayerInfoPage));


		//设置设置页面按钮回调
		UIButton* pBtnSetting=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_List_Panel")->getChildByName("Btn_Set"));
		CC_BREAK_IF(pBtnSetting==NULL);
		pBtnSetting->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToSettingPage));

		//设置技能页面按钮回调
		UIButton* pBtnSkill=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_List_Panel")->getChildByName("Btn_Skill"));
		CC_BREAK_IF(pBtnSkill==NULL);
		pBtnSkill->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToSkill));

		//设置忍术页面按钮回调
		UIButton* pBtnNinjaSkill=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_List_Panel")->getChildByName("Btn_Ninja_Skill"));
		CC_BREAK_IF(pBtnNinjaSkill==NULL);
		pBtnNinjaSkill->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToNinjaSkill));

		//设置奥义页面按钮回调
		UIButton* pBtnBigSkill=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_List_Panel")->getChildByName("Btn_BigSkill"));
		CC_BREAK_IF(pBtnBigSkill==NULL);
		pBtnBigSkill->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToBigSkill));

		//设置任务页面按钮回调
		UIButton* pBtnTask=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_List_Panel")->getChildByName("Btn_Task"));
		CC_BREAK_IF(pBtnTask==NULL);
		pBtnTask->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToTask));

		//设置公会页面按钮回调
		UIButton* pBtnGuild=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_List_Panel")->getChildByName("Btn_Guild"));
		CC_BREAK_IF(pBtnGuild==NULL);
		pBtnGuild->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToGuild));

		//设置排行页面按钮回调
		UIButton* pBtnRank=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_List_Panel")->getChildByName("Btn_Rank"));
		CC_BREAK_IF(pBtnRank==NULL);
		pBtnRank->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToRank));


		//-------------------加入主场景好友UI--------------------------
		UILayout *pMainSceneWidget_Friend = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("MainSceneUi_3.json"));
		CC_BREAK_IF(pMainSceneWidget_Friend==NULL);
		pUiLayer->addWidget(pMainSceneWidget_Friend);
		pMainSceneWidget_Friend->setName("MainSceneWidget_Friend");
		pMainSceneWidget_Friend->setSize(getContentSize());
		pMainSceneWidget_Friend->setPositionX(SCREEN_WIDTH+5);
		//设置好友UI开关按钮回调
		UIButton* pBtnFriend=dynamic_cast<UIButton*>(pMainSceneWidget_Friend->getChildByName("MainUi")->getChildByName("Btn_Open_Friend_Ui"));
		CC_BREAK_IF(pBtnFriend==NULL);
		pBtnFriend->addTouchEventListener(this,toucheventselector(CNFTownScene::OnBtnFriendSwitchCallBack));

		//设置好友按钮回调
		UIButton* pBtn_Friend=dynamic_cast<UIButton*>(pMainSceneWidget_Friend->getChildByName("MainUi")->getChildByName("Btn_Friend"));
		CC_BREAK_IF(pBtn_Friend==NULL);
		pBtn_Friend->addTouchEventListener(this,toucheventselector(CNFTownScene::OnBtnFriendCallBack));

		//设置周围按钮回调
		UIButton* pBtn_Arround=dynamic_cast<UIButton*>(pMainSceneWidget_Friend->getChildByName("MainUi")->getChildByName("Btn_Arround"));
		CC_BREAK_IF(pBtn_Arround==NULL);
		pBtn_Arround->addTouchEventListener(this,toucheventselector(CNFTownScene::OnBtnArroundCallBack));

		//设置搜索按钮回调
		UIButton* pBtn_Search=dynamic_cast<UIButton*>(pMainSceneWidget_Friend->getChildByName("MainUi")->getChildByName("Btn_Search"));
		CC_BREAK_IF(pBtn_Search==NULL);
		pBtn_Search->addTouchEventListener(this,toucheventselector(CNFTownScene::OnBtnSearchCallBack));


		//获取滑动条
		UIScrollView* pScrollView=dynamic_cast<UIScrollView*>(pMainSceneWidget_Friend->getChildByName("MainUi")->getChildByName("Friend_Scroll_View"));
		CC_BREAK_IF(pScrollView==NULL);


		//创建头像
		UILayout *pFriendHead = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Friend_Head.json"));
		CC_BREAK_IF(pFriendHead==NULL);


		float innerWidth = pScrollView->getSize().width;
		float innerHeight =pFriendHead->getSize().height*(7);

		for (int i=0;i<7;i++)
		{
			//创建头像
			UILayout *pFriendHead_1 = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Friend_Head.json"));
			CC_BREAK_IF(pFriendHead_1==NULL);

			pFriendHead_1->setPosition(ccp(0, innerHeight - pFriendHead_1->getSize().height*(i+1)));
			pScrollView->addChild(pFriendHead_1);

		}

		pScrollView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));                
		pScrollView->setTouchEnabled(true);
		pScrollView->setBounceEnabled(true);





		addChild(pUiLayer,enZOrderMid,enTagStudioMainUiLayer);
		
		//注册Update函数
		this->schedule(schedule_selector(CNFTownScene::OnCallPerFrame));

		return true;
	} while (false);
	CCLog("Fun CNFTownScene::init Error!");
	return false;
}


void CNFTownScene::OnCallPerFrame(float dt)
{
	do 
	{
		//得到3D世界层
		CNF3DWorldLayer * pLayer = dynamic_cast<CNF3DWorldLayer *>(getChildByTag(enTag3D));
		CC_BREAK_IF(pLayer==NULL);
		pLayer->update(dt);

		//=============================================================================键盘
// 		if(m_nRunSpace>0){
// 			m_nRunSpace--;
// 		}
// 		float fMoveRate = -1;
// 		//上
// 		if((GetAsyncKeyState(0x57) & 0x8000 ? 1 : 0))
// 		{
// 			fMoveRate = 0;
// 			if(m_bIsPressW==false){
// 				if(m_nRunSpace>0&&m_fFirectionOld==fMoveRate){
// 					m_bIsRun = true;
// 				}
// 				if(m_nRunSpace<=0){
// 					m_bIsRun = false;
// 				}
// 				m_fFirectionOld = fMoveRate;
// 			}
// 			m_nRunSpace = _NF_RUNSPACE_;
// 		}m_bIsPressW = (GetAsyncKeyState(0x57) & 0x8000 ? 1 : 0);
// 		//左
// 		if((GetAsyncKeyState(0x41) & 0x8000 ? 1 : 0))
// 		{
// 			if (fMoveRate == -1)
// 			{
// 				fMoveRate = 270;			
// 			}
// 			else if (fMoveRate==0)
// 			{
// 				fMoveRate = 315;
// 			} 
// 			if(m_bIsPressA==false){
// 				if(m_nRunSpace>0&&m_fFirectionOld==fMoveRate){
// 					m_bIsRun = true;
// 				}
// 				if(m_nRunSpace<=0){
// 					m_bIsRun = false;
// 				}
// 				m_fFirectionOld = fMoveRate;
// 			}
// 			m_nRunSpace = _NF_RUNSPACE_;
// 		}m_bIsPressA = (GetAsyncKeyState(0x41) & 0x8000 ? 1 : 0);
// 		//下
// 		if((GetAsyncKeyState(0x53) & 0x8000 ? 1 : 0))
// 		{
// 			if (fMoveRate==-1)
// 			{
// 				fMoveRate = 180;
// 			}
// 			else if (fMoveRate == 270)
// 			{
// 				fMoveRate = 225;			
// 			}
// 			if(m_bIsPressS==false){
// 				if(m_nRunSpace>0&&m_fFirectionOld==fMoveRate){
// 					m_bIsRun = true;
// 				}
// 				if(m_nRunSpace<=0){
// 					m_bIsRun = false;
// 				}
// 				m_fFirectionOld = fMoveRate;
// 			}
// 			m_nRunSpace = _NF_RUNSPACE_;
// 		}m_bIsPressS = (GetAsyncKeyState(0x53) & 0x8000 ? 1 : 0);
// 		//右
// 		if((GetAsyncKeyState(0x44) & 0x8000 ? 1 : 0))
// 		{
// 			if (fMoveRate == -1)
// 			{
// 				fMoveRate = 90;
// 			}
// 			else if (fMoveRate == 0)
// 			{
// 				fMoveRate = 45;
// 			}
// 			else if (fMoveRate == 180)
// 			{
// 				fMoveRate = 135;
// 			}
// 			if(m_bIsPressD==false){
// 				if(m_nRunSpace>0&&m_fFirectionOld==fMoveRate){
// 					m_bIsRun = true;
// 				}
// 				if(m_nRunSpace<=0){
// 					m_bIsRun = false;
// 				}
// 				m_fFirectionOld = fMoveRate;
// 			}
// 			m_nRunSpace = _NF_RUNSPACE_;
// 		}m_bIsPressD = (GetAsyncKeyState(0x44) & 0x8000 ? 1 : 0);
// 
// 		CNFProtagonist* pSprite = dynamic_cast<CNFProtagonist*>(pLayer->getChildByTag(enTagProtagonist));
// 		if(pSprite!=NULL){
// 			if(fMoveRate<0){
// 				CNFRockerLayer * pRocker = dynamic_cast<CNFRockerLayer*>(this->getChildByTag(enTagRocker));
// 				if(pRocker!=NULL){
// 					if(pRocker->GetIsTouching()==false){
// 						pSprite->OnCtrlStop();
// 					}
// 				}
// 			}else{
// 				if(m_bIsRun){
// 					pSprite->OnCtrlRunByRotation(fMoveRate);
// 				}else{
// 					pSprite->OnCtrlMoveByRotation(fMoveRate);
// 				}
// 			}
// 		}

		//
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

		return;
	} while (false);
	CCLog("fun CNFTownScene::Update Error!");
}

void CNFTownScene::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -127, false);
}

void CNFTownScene::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

bool CNFTownScene::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void CNFTownScene::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{

}

void CNFTownScene::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
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

void CNFTownScene::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{

}

void CNFTownScene::RecMsg( int enMsg,void* pData,int nSize )
{
	do 
	{

		switch(enMsg)
		{
		case enMsgCreateFbSelectLayer:
			{

				OnSwitchToNormalExFbPage();
			}break;
		}

		return ;
	} while (false);
	CCLog("Fun CNFTownScene::RecMsg Error!");
}

void CNFTownScene::OnBtnCallBack( CCObject *pObj )
{
	do 
	{

		return ;
	} while (false);
	CCLog("Fun CNFTownScene::OnBtnCallBack Error!");
}


void CNFTownScene::OnExUiBtnListSwitch( CCObject *pSender, TouchEventType type )
{

	if (m_bBtnListSwitchMoveOver==false)return;
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		{
			UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
			CC_BREAK_IF(pUiLayer==NULL);

			if (m_bBtnListSwitchOpen==false)
			{
				//获取扩展开关按钮
				ActionManager::shareManager()->playActionByName("MainSceneUi_2.json","Animation_Open",NULL);
				UILayout *pMainSceneWidget_Part_2 = dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("MainSceneWidget_Part_2"));
				CC_BREAK_IF(pMainSceneWidget_Part_2==NULL);
				UIButton* pBtnSwitch=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_Swich_New"));
				CC_BREAK_IF(pBtnSwitch==NULL);				

				m_bBtnListSwitchMoveOver=false;
				m_bBtnListSwitchOpen=true;
				pBtnSwitch->runAction(CCSequence::create(CCRotateTo::create(0.3f,-50),CCCallFunc::create(this, callfunc_selector(CNFTownScene::OnTouchSwichMoveOver)),NULL));	

			}
			else
			{
				//获取扩展开关按钮
				ActionManager::shareManager()->playActionByName("MainSceneUi_2.json","Animation_Close",NULL);
				UILayout *pMainSceneWidget_Part_2 = dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("MainSceneWidget_Part_2"));
				CC_BREAK_IF(pMainSceneWidget_Part_2==NULL);					
				UIButton* pBtnSwitch=dynamic_cast<UIButton*>(pMainSceneWidget_Part_2->getChildByName("Btn_Swich_New"));
				CC_BREAK_IF(pBtnSwitch==NULL);		
				m_bBtnListSwitchMoveOver=false;
				m_bBtnListSwitchOpen=false;

				pBtnSwitch->runAction(CCSequence::create(CCRotateTo::create(0.3f,0),CCCallFunc::create(this, callfunc_selector(CNFTownScene::OnTouchSwichMoveOver)),NULL));	

			}



		}
		break;
	default:
		break;
	}
}

void CNFTownScene::OnTouchSwichMoveOver()
{
	m_bBtnListSwitchMoveOver=true;
}

void CNFTownScene::OnSwitchToPlayerInfoPage( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{

			if(getChildByTag(enTagStudioCurrentChildUiLayer))return;
			//创建主场景UI层
			UILayer* pUiLayer = UILayer::create(); 
			CC_BREAK_IF(pUiLayer==NULL);


			//加入主场景UI
			UIWidget *pPlayerInfoPageLayer = dynamic_cast<UIWidget*>(GUIReader::shareReader()->widgetFromJsonFile("PlayerInfo.json"));
			CC_BREAK_IF(pPlayerInfoPageLayer==NULL);
			pUiLayer->addWidget(pPlayerInfoPageLayer);
			pPlayerInfoPageLayer->setName("PlayerInfoPageLayer");
			pPlayerInfoPageLayer->setSize(getContentSize());	
			pPlayerInfoPageLayer->addTouchEventListener(this,toucheventselector(CNFTownScene::OnTouchPlayerInfoPage));
			pPlayerInfoPageLayer->setTouchEnabled(true);
	


			//设置关闭按钮回调
			UIButton* pBtnClose=dynamic_cast<UIButton*>(pPlayerInfoPageLayer->getChildByName("Btn_Close"));
			CC_BREAK_IF(pBtnClose==NULL);
			pBtnClose->addTouchEventListener(this,toucheventselector(CNFTownScene::OnClosePlayerInfoPage));


			//设置背包按钮回调
			UIButton* pBtnBag=dynamic_cast<UIButton*>(pPlayerInfoPageLayer->getChildByName("Btn_Bag"));
			CC_BREAK_IF(pBtnBag==NULL);
			pBtnBag->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToBagPage));

			//设置人物信息按钮回调
			UIButton* pBtnInfo=dynamic_cast<UIButton*>(pPlayerInfoPageLayer->getChildByName("Btn_Info"));
			CC_BREAK_IF(pBtnInfo==NULL);
			pBtnInfo->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToPlayerAtribuePage));

			//设置称号信息回调
			UIButton* pBtnTitle=dynamic_cast<UIButton*>(pPlayerInfoPageLayer->getChildByName("Btn_Title"));
			CC_BREAK_IF(pBtnTitle==NULL);
			pBtnTitle->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToTitlePage));

			//构造背包道具页
			UIPageView* pItem_Page_View=dynamic_cast<UIPageView*>(pPlayerInfoPageLayer->getChildByName("Bag_Panel")->getChildByName("Item_Panel")->getChildByName("Item_Page_View"));
			CC_BREAK_IF(pItem_Page_View==NULL);
			for (int i=0;i<4;i++)
			{
				UILayout *pPlayerInfoItemPageLayer = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("PlayerInfoItemPage.json"));
				CC_BREAK_IF(pPlayerInfoItemPageLayer==NULL);
				pItem_Page_View->addPage(pPlayerInfoItemPageLayer);
			}


			//设置装备按钮回调
			UILayout* pEquPanel=dynamic_cast<UILayout*>(pPlayerInfoPageLayer->getChildByName("Bag_Panel")->getChildByName("Equ_Panel"));
			CC_BREAK_IF(pEquPanel==NULL);
			for (int i=0;i<10;i++)
			{
				if (i==5)continue;
				UIButton *pEquBtn = dynamic_cast<UIButton*>(pEquPanel->getChildByTag(i));
				CC_BREAK_IF(pEquBtn==NULL);
				pEquBtn->addTouchEventListener(this,toucheventselector(CNFTownScene::OnEquClick));
			}

	
			pItem_Page_View->addEventListenerPageView(this, pagevieweventselector(CNFTownScene::OnPlayerInfoItemBagPageViewEvent));

			addChild(pUiLayer,enZOrderFront,enTagStudioCurrentChildUiLayer);



			UILayout* pPlayer_Title_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Title_Panel"));
			CC_BREAK_IF(pPlayer_Title_Panel==NULL);
			pPlayer_Title_Panel->setEnabled(false);

		}break;
	default:
		break;
	}
}

void CNFTownScene::OnClosePlayerInfoPage( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{

			if(getChildByTag(enTagStudioCurrentChildUiLayer))removeChildByTag(enTagStudioCurrentChildUiLayer);

		}

		break;
	default:
		break;
	}
}

void CNFTownScene::OnPlayerInfoItemBagPageViewEvent( CCObject* pSender, PageViewEventType type )
{
	switch (type)
	{
	case PAGEVIEW_EVENT_TURNING:
		{
			UIPageView* pageView = dynamic_cast<UIPageView*>(pSender);

			int nCurrentPageIndex = pageView->getCurPageIndex();

			UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
			CC_BREAK_IF(pUiLayer==NULL);

			for (int i=0;i<4;i++)
			{
				char cPage_Select_Name[256]={0};
				sprintf(cPage_Select_Name,"Page_Select_%d",i+1);
				//获取背包页码
				UIImageView* pPage_Select_Img=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Item_Panel")->getChildByName(cPage_Select_Name));
				CC_BREAK_IF(pPage_Select_Img==NULL);
				if (i==nCurrentPageIndex)
				{
					pPage_Select_Img->setVisible(true);
				}
				else
				{
					pPage_Select_Img->setVisible(false);
				}
			}
		}
		break;

	default:
		break;
	}
}



void CNFTownScene::OnSwitchToBagPage( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		{

			UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
			CC_BREAK_IF(pUiLayer==NULL);
			//获取按下图
			UIImageView* pPage_Select_Img=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("BtnSelectImg"));
			CC_BREAK_IF(pPage_Select_Img==NULL);


			UIButton* pBtn_Bag=dynamic_cast<UIButton*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Btn_Bag"));
			CC_BREAK_IF(pBtn_Bag==NULL);

			pPage_Select_Img->setPosition(pBtn_Bag->getPosition());


			UILayout* pEqu_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Equ_Panel"));
			CC_BREAK_IF(pEqu_Panel==NULL);
			pEqu_Panel->setVisible(true);

			UILayout* pItem_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Item_Panel"));
			CC_BREAK_IF(pItem_Panel==NULL);
			pItem_Panel->setVisible(true);

			UILayout* pPlayer_Title_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Title_Panel"));
			CC_BREAK_IF(pPlayer_Title_Panel==NULL);
			pPlayer_Title_Panel->setVisible(false);
			pPlayer_Title_Panel->setEnabled(false);

			UILayout* pPlayer_Info_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Player_Info_Panel"));
			CC_BREAK_IF(pPlayer_Info_Panel==NULL);
			pPlayer_Info_Panel->setVisible(false);
		}
		break;
	default:
		break;
	}
}

void CNFTownScene::OnSwitchToPlayerAtribuePage( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		{
			UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
			CC_BREAK_IF(pUiLayer==NULL);
			//获取按下图
			UIImageView* pPage_Select_Img=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("BtnSelectImg"));
			CC_BREAK_IF(pPage_Select_Img==NULL);


			UIButton* pBtn_PlayerAtribue=dynamic_cast<UIButton*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Btn_Info"));
			CC_BREAK_IF(pBtn_PlayerAtribue==NULL);

			pPage_Select_Img->setPosition(pBtn_PlayerAtribue->getPosition());


			UILayout* pEqu_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Equ_Panel"));
			CC_BREAK_IF(pEqu_Panel==NULL);
			pEqu_Panel->setVisible(true);

			UILayout* pItem_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Item_Panel"));
			CC_BREAK_IF(pItem_Panel==NULL);
			pItem_Panel->setVisible(false);

			UILayout* pPlayer_Title_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Title_Panel"));
			CC_BREAK_IF(pPlayer_Title_Panel==NULL);
			pPlayer_Title_Panel->setVisible(false);
			pPlayer_Title_Panel->setEnabled(false);

			UILayout* pPlayer_Info_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Player_Info_Panel"));
			CC_BREAK_IF(pPlayer_Info_Panel==NULL);
			pPlayer_Info_Panel->setVisible(true);


		}
		break;
	default:
		break;
	}
}


void CNFTownScene::OnSwitchToTitlePage( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_BEGAN:
		{
			UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
			CC_BREAK_IF(pUiLayer==NULL);
			//获取按下图
			UIImageView* pPage_Select_Img=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("BtnSelectImg"));
			CC_BREAK_IF(pPage_Select_Img==NULL);


			UIButton* pBtn_PlayerAtribue=dynamic_cast<UIButton*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Btn_Title"));
			CC_BREAK_IF(pBtn_PlayerAtribue==NULL);

			pPage_Select_Img->setPosition(pBtn_PlayerAtribue->getPosition());

			UILayout* pEqu_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Equ_Panel"));
			CC_BREAK_IF(pEqu_Panel==NULL);
			pEqu_Panel->setVisible(false);

			UILayout* pItem_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Item_Panel"));
			CC_BREAK_IF(pItem_Panel==NULL);
			pItem_Panel->setVisible(false);

			UILayout* pPlayer_Info_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Player_Info_Panel"));
			CC_BREAK_IF(pPlayer_Info_Panel==NULL);
			pPlayer_Info_Panel->setVisible(false);

			UILayout* pPlayer_Title_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Title_Panel"));
			CC_BREAK_IF(pPlayer_Title_Panel==NULL);
			pPlayer_Title_Panel->setVisible(true);
			pPlayer_Title_Panel->setEnabled(true);

		}
		break;
	default:
		break;
	}
}


void CNFTownScene::OnEquClick( CCObject *pSender, TouchEventType type )
{
	do 
	{
		UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
		CC_BREAK_IF(pUiLayer==NULL);
		//获取按下图
		UIImageView* pEqu_Btn_Select_Img=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Equ_Panel")->getChildByName("Equ_Selected"));
		CC_BREAK_IF(pEqu_Btn_Select_Img==NULL);

		//获取详细信息图
		UIImageView* pEquDetail=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Equ_Panel")->getChildByName("Equ_Detail_Panel"));
		CC_BREAK_IF(pEquDetail==NULL);

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				UIButton *pBtn=dynamic_cast<UIButton*>(pSender);
				if (pBtn)
				{
					pEqu_Btn_Select_Img->setPosition(pBtn->getPosition());
					pEqu_Btn_Select_Img->setVisible(true);

					pEquDetail->setVisible(true);

					switch(pBtn->getTag())
					{
					case 0://头盔
						{
							pEquDetail->setPosition(ccp(pBtn->getPositionX(),pBtn->getPositionY()-pEquDetail->getSize().height));
						}break;
					case 1://项链
						{
							pEquDetail->setPosition(ccp(pBtn->getPositionX(),pBtn->getPositionY()-pEquDetail->getSize().height));
						}break;
					case 2://手镯
						{
							pEquDetail->setAnchorPoint(ccp(0,0));
							pEquDetail->setPosition(pBtn->getPosition());
						}break;
					case 3://戒指
						{
							pEquDetail->setAnchorPoint(ccp(0,0));
							pEquDetail->setPosition(pBtn->getPosition());
						}break;
					case 4://武器
						{
							pEquDetail->setAnchorPoint(ccp(0,0));
							pEquDetail->setPosition(pBtn->getPosition());
						}break;
					case 5://武器装扮
						{
							pEquDetail->setAnchorPoint(ccp(1,0));
						}break;
					case 6://鞋子
						{
							pEquDetail->setPosition(ccp(pBtn->getPositionX()-pEquDetail->getSize().width,pBtn->getPositionY()));

						}break;
					case 7://裤子
						{
							pEquDetail->setPosition(ccp(pBtn->getPositionX()-pEquDetail->getSize().width,pBtn->getPositionY()));
						}break;
					case 8://手套
						{
							pEquDetail->setPosition(ccp(pBtn->getPositionX()-pEquDetail->getSize().width,pBtn->getPositionY()-pEquDetail->getSize().height));
						}break;
					case 9://衣服
						{
							pEquDetail->setPosition(ccp(pBtn->getPositionX()-pEquDetail->getSize().width,pBtn->getPositionY()-pEquDetail->getSize().height));						}break;
						}
				}

			}break;
		}
		return;
	} while (false);
}


void CNFTownScene::OnTouchPlayerInfoPage( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_BEGAN:
			{

				UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
				CC_BREAK_IF(pUiLayer==NULL);

				//获取按下图
				UIImageView* pEqu_Btn_Select_Img=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Equ_Panel")->getChildByName("Equ_Selected"));
				CC_BREAK_IF(pEqu_Btn_Select_Img==NULL);
				pEqu_Btn_Select_Img->setVisible(false);

				//获取详细信息图
				UIImageView* pEquDetail=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("PlayerInfoPageLayer")->getChildByName("Bag_Panel")->getChildByName("Equ_Panel")->getChildByName("Equ_Detail_Panel"));
				CC_BREAK_IF(pEquDetail==NULL);
				pEquDetail->setVisible(false);
			}break;
		}

		return ;
	} while (false);
	CCLog("Fun CNFTownScene::OnBtnBattleCallBack Error!");
}


void CNFTownScene::OnBtnBattleCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
// 				//跳转到副本
// 				CCScene * pScene = CNFGameScene::CreateGameScene(10001);
// 				CC_BREAK_IF(pScene==NULL);
// 				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));
			}break;
		}

		return ;
	} while (false);
	CCLog("Fun CNFTownScene::OnBtnBattleCallBack Error!");
}

//----------精英副本---------//
void CNFTownScene::OnSwitchToExFbPage( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{

			if(getChildByTag(enTagStudioCurrentChildUiLayer))return;
			//创建主场景UI层
			UILayer* pUiLayer = UILayer::create(); 
			CC_BREAK_IF(pUiLayer==NULL);


			//加入主场景UI
			UIWidget *pExFbPageLayer = dynamic_cast<UIWidget*>(GUIReader::shareReader()->widgetFromJsonFile("Ex_Fb.json"));
			CC_BREAK_IF(pExFbPageLayer==NULL);
			pUiLayer->addWidget(pExFbPageLayer);
			pExFbPageLayer->setName("ExFbPage");
			pExFbPageLayer->setSize(getContentSize());	
			pExFbPageLayer->setTouchEnabled(true);



			//设置关闭按钮回调
			UIButton* pBtnClose=dynamic_cast<UIButton*>(pExFbPageLayer->getChildByName("MainUi")->getChildByName("Btn_Close"));
			CC_BREAK_IF(pBtnClose==NULL);
			pBtnClose->addTouchEventListener(this,toucheventselector(CNFTownScene::OnClosePlayerInfoPage));



			//构造BOSS页
			UIPageView* pItem_Page_View=dynamic_cast<UIPageView*>(pExFbPageLayer->getChildByName("MainUi")->getChildByName("Ex_Fb_PageView"));
			CC_BREAK_IF(pItem_Page_View==NULL);
			for (int i=0;i<2;i++)
			{
				UILayout *pEx_Fb_Scroll_PageLayer = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("Ex_Fb_Scroll_Page.json"));
				CC_BREAK_IF(pEx_Fb_Scroll_PageLayer==NULL);
				pItem_Page_View->addPage(pEx_Fb_Scroll_PageLayer);
				

				if (i==0)
				{
					for (int j=0;j<8;j++)
					{
						UIButton* pBtnFb=dynamic_cast<UIButton*>(pEx_Fb_Scroll_PageLayer->getChildByName(CCString::createWithFormat("Btn_Boss_%d",j+1)->getCString()));
						CC_BREAK_IF(pBtnFb==NULL);
						pBtnFb->addTouchEventListener(this,toucheventselector(CNFTownScene::OnExFbClick));

						UIImageView *pImg = dynamic_cast<UIImageView*>(pEx_Fb_Scroll_PageLayer->getChildByName(CCString::createWithFormat("Btn_Boss_%d",j+1)->getCString())->getChildByName("Boss_Head") );
						CC_BREAK_IF(pImg==NULL);
						pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",j+1)->getCString(),UI_TEX_TYPE_LOCAL);
					}
				}
				else
				{
					for (int j=0;j<8;j++)
					{
						UIButton* pBtnFb=dynamic_cast<UIButton*>(pEx_Fb_Scroll_PageLayer->getChildByName(CCString::createWithFormat("Btn_Boss_%d",j+1)->getCString()));
						CC_BREAK_IF(pBtnFb==NULL);
						pBtnFb->addTouchEventListener(this,toucheventselector(CNFTownScene::OnExFbClick));


						UIImageView *pImg = dynamic_cast<UIImageView*>(pEx_Fb_Scroll_PageLayer->getChildByName(CCString::createWithFormat("Btn_Boss_%d",j+1)->getCString())->getChildByName("Boss_Head") );
						CC_BREAK_IF(pImg==NULL);
						pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",j+9)->getCString(),UI_TEX_TYPE_LOCAL);
					}
				}



			}

			pItem_Page_View->addEventListenerPageView(this, pagevieweventselector(CNFTownScene::OnExFbPageViewEvent));

			addChild(pUiLayer,enZOrderFront,enTagStudioCurrentChildUiLayer);





		}break;
	default:
		break;
	}
}

void CNFTownScene::OnExFbPageViewEvent( CCObject* pSender, PageViewEventType type )
{

}

void CNFTownScene::OnExFbClick( CCObject* pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{


			}break;
		}
		return ;
	} while (false);
	CCLog("Fun CNFTownScene::OnExFbClick Error!");
}

//----------普通副本---------//

void CNFTownScene::OnSwitchToNormalExFbPage()
{
	do 
	{

		if(getChildByTag(enTagStudioCurrentChildUiLayer))return;
		//创建主场景UI层
		UILayer* pUiLayer = UILayer::create(); 
		CC_BREAK_IF(pUiLayer==NULL);


		//加入主场景UI
		UIWidget *pExFbPageLayer = dynamic_cast<UIWidget*>(GUIReader::shareReader()->widgetFromJsonFile("Fb.json"));
		CC_BREAK_IF(pExFbPageLayer==NULL);
		pUiLayer->addWidget(pExFbPageLayer);
		pExFbPageLayer->setName("FbSelectPageLayer");
		pExFbPageLayer->setSize(getContentSize());	
		pExFbPageLayer->setTouchEnabled(true);



		//设置关闭按钮回调
		UIButton* pBtnClose=dynamic_cast<UIButton*>(pExFbPageLayer->getChildByName("MainUi")->getChildByName("Btn_Close"));
		CC_BREAK_IF(pBtnClose==NULL);
		pBtnClose->addTouchEventListener(this,toucheventselector(CNFTownScene::OnClosePlayerInfoPage));



		//构造BOSS页
		UIPageView* pItem_Page_View=dynamic_cast<UIPageView*>(pExFbPageLayer->getChildByName("MainUi")->getChildByName("Fb_PageView"));
		CC_BREAK_IF(pItem_Page_View==NULL);
		for (int i=0;i<2;i++)
		{
			UILayout *pEx_Fb_Scroll_PageLayer = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("Ex_Fb_Scroll_Page.json"));
			CC_BREAK_IF(pEx_Fb_Scroll_PageLayer==NULL);
			pItem_Page_View->addPage(pEx_Fb_Scroll_PageLayer);


			if (i==0)
			{
				for (int j=0;j<8;j++)
				{
					UIButton* pBtnFb=dynamic_cast<UIButton*>(pEx_Fb_Scroll_PageLayer->getChildByName(CCString::createWithFormat("Btn_Boss_%d",j+1)->getCString()));
					CC_BREAK_IF(pBtnFb==NULL);
					pBtnFb->addTouchEventListener(this,toucheventselector(CNFTownScene::OnNormalFbClick));

					UIImageView *pImg = dynamic_cast<UIImageView*>(pEx_Fb_Scroll_PageLayer->getChildByName(CCString::createWithFormat("Btn_Boss_%d",j+1)->getCString())->getChildByName("Boss_Head") );
					CC_BREAK_IF(pImg==NULL);
					pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",j+1)->getCString(),UI_TEX_TYPE_LOCAL);
				}
			}
			else
			{
				for (int j=0;j<8;j++)
				{
					UIButton* pBtnFb=dynamic_cast<UIButton*>(pEx_Fb_Scroll_PageLayer->getChildByName(CCString::createWithFormat("Btn_Boss_%d",j+1)->getCString()));
					CC_BREAK_IF(pBtnFb==NULL);
					pBtnFb->addTouchEventListener(this,toucheventselector(CNFTownScene::OnNormalFbClick));


					UIImageView *pImg = dynamic_cast<UIImageView*>(pEx_Fb_Scroll_PageLayer->getChildByName(CCString::createWithFormat("Btn_Boss_%d",j+1)->getCString())->getChildByName("Boss_Head") );
					CC_BREAK_IF(pImg==NULL);
					pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",j+9)->getCString(),UI_TEX_TYPE_LOCAL);
				}
			}

		}

		pItem_Page_View->addEventListenerPageView(this, pagevieweventselector(CNFTownScene::OnNormalFbPageViewEvent));


		//获取详细信息图
		UIWidget* pFbDetail=dynamic_cast<UIWidget*>(pExFbPageLayer->getChildByName("Fb_Info"));
		CC_BREAK_IF(pFbDetail==NULL);

		UIButton *pBtn_Fast_Battle=dynamic_cast<UIButton*>(pFbDetail->getChildByName("Btn_Fast_Battle"));
		CC_BREAK_IF(pBtn_Fast_Battle==NULL);
		pBtn_Fast_Battle->addTouchEventListener(this,toucheventselector(CNFTownScene::OnNormalFbFastBattleClick));

		UIButton *pBtn_Start_Battle=dynamic_cast<UIButton*>(pFbDetail->getChildByName("Btn_Start_Battle"));
		CC_BREAK_IF(pBtn_Start_Battle==NULL);
		pBtn_Start_Battle->addTouchEventListener(this,toucheventselector(CNFTownScene::OnNormalFbStartBattleClick));

		UIButton *pBtn_Fb_Info_Close=dynamic_cast<UIButton*>(pFbDetail->getChildByName("Btn_Close"));
		CC_BREAK_IF(pBtn_Fb_Info_Close==NULL);
		pBtn_Fb_Info_Close->addTouchEventListener(this,toucheventselector(CNFTownScene::OnNormalFbInfoCloseClick));

		pFbDetail->setEnabled(false);

		addChild(pUiLayer,enZOrderFront,enTagStudioCurrentChildUiLayer);



		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToNormalExFbPage Error");
}

void CNFTownScene::OnNormalFbPageViewEvent( CCObject* pSender, PageViewEventType type )
{

}

void CNFTownScene::OnNormalFbClick( CCObject* pSender, TouchEventType type )
{
	do 
	{
		UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
		CC_BREAK_IF(pUiLayer==NULL);

		//获取详细信息图
		UIWidget* pFbDetail=dynamic_cast<UIWidget*>(pUiLayer->getWidgetByName("FbSelectPageLayer")->getChildByName("Fb_Info"));
		CC_BREAK_IF(pFbDetail==NULL);



		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				UIButton *pBtn=dynamic_cast<UIButton*>(pSender);
				if (pBtn)
				{
					switch(pBtn->getTag())
					{
					case 1://Boss 1
						{
							m_nStageID = 10001;
							UIImageView *pImg = dynamic_cast<UIImageView*>(pFbDetail->getChildByName("Boss_Head"));
							CC_BREAK_IF(pImg==NULL);
							pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",1)->getCString(),UI_TEX_TYPE_LOCAL);

						}break;
					case 2://Boss 2
						{
							m_nStageID = 10002;
							UIImageView *pImg = dynamic_cast<UIImageView*>(pFbDetail->getChildByName("Boss_Head"));
							CC_BREAK_IF(pImg==NULL);
							pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",2)->getCString(),UI_TEX_TYPE_LOCAL);
						}break;
					case 3://Boss 3
						{
							m_nStageID = 10003;
							UIImageView *pImg = dynamic_cast<UIImageView*>(pFbDetail->getChildByName("Boss_Head"));
							CC_BREAK_IF(pImg==NULL);
							pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",3)->getCString(),UI_TEX_TYPE_LOCAL);
						}break;
					case 4://Boss 4
						{
							m_nStageID = 0;
							UIImageView *pImg = dynamic_cast<UIImageView*>(pFbDetail->getChildByName("Boss_Head"));
							CC_BREAK_IF(pImg==NULL);
							pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",4)->getCString(),UI_TEX_TYPE_LOCAL);
						}break;
					case 5://Boss 5
						{
							m_nStageID = 0;
							UIImageView *pImg = dynamic_cast<UIImageView*>(pFbDetail->getChildByName("Boss_Head"));
							CC_BREAK_IF(pImg==NULL);
							pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",5)->getCString(),UI_TEX_TYPE_LOCAL);
						}break;
					case 6://Boss 6
						{
							m_nStageID = 0;
							UIImageView *pImg = dynamic_cast<UIImageView*>(pFbDetail->getChildByName("Boss_Head"));
							CC_BREAK_IF(pImg==NULL);
							pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",6)->getCString(),UI_TEX_TYPE_LOCAL);
						}break;
					case 7://Boss 7
						{
							m_nStageID = 0;
							UIImageView *pImg = dynamic_cast<UIImageView*>(pFbDetail->getChildByName("Boss_Head"));
							CC_BREAK_IF(pImg==NULL);
							pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",7)->getCString(),UI_TEX_TYPE_LOCAL);
						}break;
					case 8://Boss 8
						{
							m_nStageID = 0;
							UIImageView *pImg = dynamic_cast<UIImageView*>(pFbDetail->getChildByName("Boss_Head"));
							CC_BREAK_IF(pImg==NULL);
							pImg->loadTexture(CCString::createWithFormat("boss_head/boss_%d.png",8)->getCString(),UI_TEX_TYPE_LOCAL);
						}break;
					}

					pFbDetail->setEnabled(true);
					pFbDetail->setVisible(true);
				}break;
			}
		}


		return;
	} while (false);
	CCLog("CNFTownScene::OnNormalFbClick Error");
}

void CNFTownScene::OnNormalFbStartBattleClick( CCObject* pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				if(m_nStageID>_NF_TOWN_OR_BATTLE_ID_&&m_nStageID<_NF_TOWN_FB_ID_){
					//跳转到副本
					CCScene * pScene = CNFGameScene::CreateGameScene(m_nStageID,m_nRoleID);
					CC_BREAK_IF(pScene==NULL);
					CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));
				}
			}break;
		}


		return;
	} while (false);
}

void CNFTownScene::OnNormalFbFastBattleClick( CCObject* pSender, TouchEventType type )
{

}

void CNFTownScene::OnNormalFbInfoCloseClick( CCObject* pSender, TouchEventType type )
{
	do 
	{
		UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
		CC_BREAK_IF(pUiLayer==NULL);

		//获取详细信息图
		UIWidget* pFbDetail=dynamic_cast<UIWidget*>(pUiLayer->getWidgetByName("FbSelectPageLayer")->getChildByName("Fb_Info"));
		CC_BREAK_IF(pFbDetail==NULL);

		pFbDetail->setVisible(false);
		pFbDetail->setEnabled(false);


		return;
	} while (false);
}
//-------------------加入主场景UI按钮回调----------------------------
void CNFTownScene::OnSwitchToGetReward( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into  Get Reward!");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToGetReward Error!");
}

void CNFTownScene::OnSwitchToGoldReward( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   GoldReward!");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToGoldReward Error!");
}

void CNFTownScene::OnSwitchToPvP( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   PvP!");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToPvP Error!");
}

void CNFTownScene::OnSwitchToActive( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   Active!");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToActive Error!");
}

void CNFTownScene::OnSwitchToVip( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   Vip !");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToVip Error!");
}

void CNFTownScene::OnSwitchToDraw( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   Draw!");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToDraw Error!");
}

void CNFTownScene::OnSwitchToFirendFb( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   FirendFb!");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToFirendFb Error!");
}

void CNFTownScene::OnSwitchToShop( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   Shop!");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToShop Error!");
}

void CNFTownScene::OnSwitchToPiePvP( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   PiePvP!");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToPiePvP Error!");
}

void CNFTownScene::OnSwitchToEmail( CCObject *pSender, TouchEventType type )
{
	do 
	{


		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				if(getChildByTag(enTagStudioCurrentChildUiLayer))return;
				//创建主场景UI层
				UILayer* pUiLayer = UILayer::create(); 
				CC_BREAK_IF(pUiLayer==NULL);


				//加入主场景UI
				UIWidget *pEmail = dynamic_cast<UIWidget*>(GUIReader::shareReader()->widgetFromJsonFile("Email.json"));
				CC_BREAK_IF(pEmail==NULL);
				pUiLayer->addWidget(pEmail);
				pEmail->setName("EmailPage");
				pEmail->setSize(getContentSize());	
				pEmail->setTouchEnabled(true);



				//设置关闭按钮回调
				UIButton* pBtnClose=dynamic_cast<UIButton*>(pEmail->getChildByName("MainUi")->getChildByName("Btn_Close"));
				CC_BREAK_IF(pBtnClose==NULL);
				pBtnClose->addTouchEventListener(this,toucheventselector(CNFTownScene::OnClosePlayerInfoPage));

				//获取滑动条
				UIScrollView* pScrollView=dynamic_cast<UIScrollView*>(pEmail->getChildByName("MainUi")->getChildByName("Email_ScrollView"));
				CC_BREAK_IF(pScrollView==NULL);


				//创建头像
				UILayout *pEmailPageTest = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Email_Info_Page.json"));
				CC_BREAK_IF(pEmailPageTest==NULL);


				float innerWidth = pScrollView->getSize().width;
				float innerHeight =pEmailPageTest->getSize().height*(4);

				for (int i=0;i<4;i++)
				{
					//创建头像
					UILayout *pEmailPage = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Email_Info_Page.json"));
					CC_BREAK_IF(pEmailPage==NULL);

					pEmailPage->setPosition(ccp(0, innerHeight - pEmailPage->getSize().height*(i+1)));
					pScrollView->addChild(pEmailPage);
					if (i>=2)
					{
						pEmailPage->setVisible(false);
					}
				}

				pScrollView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));                
				pScrollView->setTouchEnabled(true);
				pScrollView->setBounceEnabled(true);

				addChild(pUiLayer,enZOrderFront,enTagStudioCurrentChildUiLayer);
			}break;
		default:
			break;
		}



		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToEmail Error!");
}

void CNFTownScene::OnSwitchToHelp( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   Help !");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToHelp Error!");
}

void CNFTownScene::OnSwitchTotalk( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into   talk!");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchTotalk Error!");
}

void CNFTownScene::OnSwitchToSkillUp( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				if(getChildByTag(enTagStudioCurrentChildUiLayer))return;
				//创建主场景UI层
				UILayer* pUiLayer = UILayer::create(); 
				CC_BREAK_IF(pUiLayer==NULL);

				//加入主场景UI
				UIWidget *pSkill = dynamic_cast<UIWidget*>(GUIReader::shareReader()->widgetFromJsonFile("Skill.json"));
				CC_BREAK_IF(pSkill==NULL);
				pUiLayer->addWidget(pSkill);
				pSkill->setName("SkillPage");
				pSkill->setSize(getContentSize());	


				//设置技能按钮回调
				for (int i=0;i<5;i++)
				{
					UIButton *pSkill_Btn=dynamic_cast<UIButton*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName(CCString::createWithFormat("Skill_%d",i+1)->getCString()));
					CC_BREAK_IF(pSkill_Btn==NULL);
					pSkill_Btn->addTouchEventListener(this,toucheventselector(CNFTownScene::OnChooseSkill));

				}

				
				//设置头像回调
				for (int i=0;i<3;i++)
				{
					UIButton *pBtn_Char=dynamic_cast<UIButton*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName(CCString::createWithFormat("Btn_Char_%d",i+1)->getCString()));
					CC_BREAK_IF(pBtn_Char==NULL);
					pBtn_Char->addTouchEventListener(this,toucheventselector(CNFTownScene::OnChooseRole));
				}

				//设置关闭按钮回调
				UIButton* pBtnClose=dynamic_cast<UIButton*>(pSkill->getChildByName("Info_Sq_Bg")->getChildByName("Btn_Close"));
				CC_BREAK_IF(pBtnClose==NULL);
				pBtnClose->addTouchEventListener(this,toucheventselector(CNFTownScene::OnClosePlayerInfoPage));


				//设置更换忍术回调
				UIButton* pBtn_Change_Skill=dynamic_cast<UIButton*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill_Info")->getChildByName("Btn_Change_Skill"));
				CC_BREAK_IF(pBtn_Change_Skill==NULL);
				pBtn_Change_Skill->addTouchEventListener(this,toucheventselector(CNFTownScene::OnCreateChangeSkillLayer));

				//读取角色
				CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/xiaoyin.ExportJson");
				CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/NewProject.ExportJson");
				CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/minren1.ExportJson");
				char cName[3][256]={"minren1","xiaoyin","NewProject"};

				//获取角色位置
				UIWidget* pChar=dynamic_cast<UIWidget*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Role_Pos_Img"));
				CC_BREAK_IF(pChar==NULL);

				//创建角色
				CCArmature *pRole = CCArmature::create(cName[m_nRoleID-1]);
				CC_BREAK_IF(pRole==NULL);
				pRole->setPosition(ccp(pChar->getPositionX()+SCREEN_WIDTH/2,pChar->getPositionY()));
				pRole->setAnchorPoint(ccp(pRole->getAnchorPoint().x,0));
				pRole->setScale(1.5f);
				pRole->getAnimation()->play("hold",-1,-1,-1,10000);
				pUiLayer->addChild(pRole,enZOrderFront,enTagRole);
				

				//默认隐藏技能图标和等级
				for (int i=0;i<5;i++)
				{
					UIButton *pBtn=dynamic_cast<UIButton*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName(CCString::createWithFormat("Skill_%d",i+1)->getCString()));
					CC_BREAK_IF(pBtn==NULL);

					UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pBtn->getChildByName("Skill_Img"));
					CC_BREAK_IF(pSkill_Img==NULL);
					pSkill_Img->setVisible(false);

					UILabel *pLv_Num_Lable=dynamic_cast<UILabel*>(pBtn->getChildByName("Lv_Num_Lable"));
					CC_BREAK_IF(pLv_Num_Lable==NULL);
					pLv_Num_Lable->setVisible(false);
				}


				//根据当前角色ID初始化技能图标
				CNFServerSkillInfoVec SkillInfoVec;
				CNFServerDataManager::SharedData()->GetSkillInfo(m_nRoleID,SkillInfoVec);
				CNFServerSkillInfoVec::iterator itSkill=SkillInfoVec.begin();
				for (;itSkill!=SkillInfoVec.end();itSkill++)
				{
					if (itSkill->nPos>0)
					{
						switch(itSkill->nPos)
						{
						case 1:
							{
								//设置图标
								UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_1")->getChildByName("Skill_Img"));
								CC_BREAK_IF(pSkill_Img==NULL);
								pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
								pSkill_Img->setVisible(true);

								//设置等级
								UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_1")->getChildByName("Lv_Num_Lable"));
								CC_BREAK_IF(pSkill_Lv==NULL);
								pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
								pSkill_Lv->setVisible(true);

								m_nCurrentSelectSkillID=itSkill->nSkillID;
							}break;
						case 2:
							{
								//设置图标
								UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_2")->getChildByName("Skill_Img"));
								CC_BREAK_IF(pSkill_Img==NULL);
								pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
								pSkill_Img->setVisible(true);

								//设置等级
								UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_2")->getChildByName("Lv_Num_Lable"));
								CC_BREAK_IF(pSkill_Lv==NULL);
								pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
								pSkill_Lv->setVisible(true);

							}break;
						case 3:
							{
								//设置图标
								UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_3")->getChildByName("Skill_Img"));
								CC_BREAK_IF(pSkill_Img==NULL);
								pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
								pSkill_Img->setVisible(true);

								//设置等级
								UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_3")->getChildByName("Lv_Num_Lable"));
								CC_BREAK_IF(pSkill_Lv==NULL);
								pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
							}break;
						case 4:
							{
								//设置图标
								UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_4")->getChildByName("Skill_Img"));
								CC_BREAK_IF(pSkill_Img==NULL);
								pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
								pSkill_Img->setVisible(true);

								//设置等级
								UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_4")->getChildByName("Lv_Num_Lable"));
								CC_BREAK_IF(pSkill_Lv==NULL);
								pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
							}break;
						case 5:
							{
								//设置图标
								UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_5")->getChildByName("Skill_Img"));
								CC_BREAK_IF(pSkill_Img==NULL);
								pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
								pSkill_Img->setVisible(true);

								//设置等级
								UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_5")->getChildByName("Lv_Num_Lable"));
								CC_BREAK_IF(pSkill_Lv==NULL);
								pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
							}break;
						}
					}
				}
				

				//设置人物选中背景图
				UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName(CCString::createWithFormat("Btn_Char_%d",m_nRoleID)->getCString())->getChildByName("Role_Select_Img"));
				CC_BREAK_IF(pSelect_Img==NULL);
				pSelect_Img->setVisible(true);

				//初始化技能选择角色ID
				m_nCurrentSkillUpRoleID=m_nRoleID;

				addChild(pUiLayer,enZOrderFront,enTagStudioCurrentChildUiLayer);
			}break;
		default:
			break;
		}


		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToSkillUp Error!");
}

void CNFTownScene::OnChooseSkill( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			//获取技能页
			UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
			CC_BREAK_IF(pUiLayer==NULL);

			//获取技能页UI
			UIWidget *pSkill = dynamic_cast<UIWidget*>(pUiLayer->getWidgetByName("SkillPage"));
			CC_BREAK_IF(pSkill==NULL);
	
			//隐藏技能
			UIWidget *pSkill_Info=dynamic_cast<UIWidget*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill_Info"));
			CC_BREAK_IF(pSkill_Info==NULL);
			pSkill_Info->setVisible(true);

			//编辑技能图标
			bool  bEquSkill=false;
			for (int i=0;i<5;i++)
			{
				UIButton *pBtn=dynamic_cast<UIButton*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName(CCString::createWithFormat("Skill_%d",i+1)->getCString()));
				CC_BREAK_IF(pBtn==NULL);

				UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pBtn->getChildByName("Select_Img"));
				CC_BREAK_IF(pSelect_Img==NULL);
				pSelect_Img->setVisible(false);

				if (pBtn==pSender)
				{

					//初始化技能图标
					CNFServerSkillInfoVec SkillInfoVec;
					CNFServerDataManager::SharedData()->GetSkillInfo(m_nCurrentSkillUpRoleID,SkillInfoVec);

					CNFServerSkillInfoVec::iterator itSkill=SkillInfoVec.begin();

					for (;itSkill!=SkillInfoVec.end();itSkill++)
					{
						if (itSkill->nPos==(i+1))
						{
							if (itSkill->nPos>0)
							{
								bEquSkill=true;

								//设置技能信息
								UILabel *pSkillName=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill_Info")->getChildByName("Skill_Name"));
								CC_BREAK_IF(pSkillName==NULL);

								//获取技能静态信息
								tagSkillStaticInfo SkillStaticInfo;
								CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nCurrentSkillUpRoleID,itSkill->nSkillID,SkillStaticInfo);

								//设置技能名字
								pSkillName->setText(CCString::createWithFormat("%s",SkillStaticInfo.strSkillName.c_str())->getCString());

								//设置当前技能ID
								m_nCurrentSelectSkillID=itSkill->nSkillID;

								//获取按钮
								UIButton *pSelectBtn=dynamic_cast<UIButton*>(pSender);
								CC_BREAK_IF(pSelectBtn==NULL);


								//显示选中图标
								UIImageView *pBtn_Select_Img=dynamic_cast<UIImageView*>(pSelectBtn->getChildByName("Select_Img"));
								CC_BREAK_IF(pBtn_Select_Img==NULL);
								pBtn_Select_Img->setVisible(true);
							}
						}

					}
				}
			}


			//若该技能栏没有装备技能
			if (bEquSkill==false)
			{
				//隐藏技能
				UIWidget *pSkill_Info=dynamic_cast<UIWidget*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill_Info"));
				CC_BREAK_IF(pSkill_Info==NULL);
				pSkill_Info->setVisible(false);

				//设置当前技能ID
				m_nCurrentSelectSkillID=0;

				UIButton *pSelectBtn=dynamic_cast<UIButton*>(pSender);
				CC_BREAK_IF(pSelectBtn==NULL);

				//显示选中图标
				UIImageView *pBtn_Select_Img=dynamic_cast<UIImageView*>(pSelectBtn->getChildByName("Select_Img"));
				CC_BREAK_IF(pBtn_Select_Img==NULL);
				pBtn_Select_Img->setVisible(true);

				//直接弹出技能选择页
				OnCreateChangeSkillLayer(pSender,type);	

			}




		}break;
	default:
		break;
	}
}

void CNFTownScene::OnChooseRole( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{

			//获取主场景UI层
			UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
			CC_BREAK_IF(pUiLayer==NULL);


			//获取主场景UI
			UIWidget *pSkill = dynamic_cast<UIWidget*>(pUiLayer->getWidgetByName("SkillPage"));
			CC_BREAK_IF(pSkill==NULL);


			//隐藏技能
			UIWidget *pSkill_Info=dynamic_cast<UIWidget*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill_Info"));
			CC_BREAK_IF(pSkill_Info==NULL);
			pSkill_Info->setVisible(true);


			for (int i=0;i<3;i++)
			{
				UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName(CCString::createWithFormat("Btn_Char_%d",i+1)->getCString())->getChildByName("Role_Select_Img"));
				CC_BREAK_IF(pSelect_Img==NULL);
				pSelect_Img->setVisible(false);
			}



			UIButton *pBtn=dynamic_cast<UIButton*>(pSender);
			CC_BREAK_IF(pBtn==NULL);

			UIImageView *pBtn_Select_Img=dynamic_cast<UIImageView*>(pBtn->getChildByName("Role_Select_Img"));
			CC_BREAK_IF(pBtn_Select_Img==NULL);
			pBtn_Select_Img->setVisible(true);

			CCPoint ptRole=pUiLayer->getChildByTag(enTagRole)->getPosition();




			for (int i=0;i<5;i++)
			{
				UIButton *pSkillBtn=dynamic_cast<UIButton*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName(CCString::createWithFormat("Skill_%d",i+1)->getCString()));
				CC_BREAK_IF(pSkillBtn==NULL);


				UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkillBtn->getChildByName("Skill_Img"));
				CC_BREAK_IF(pSkill_Img==NULL);
				pSkill_Img->setVisible(false);

				UILabel *pLv_Num_Lable=dynamic_cast<UILabel*>(pSkillBtn->getChildByName("Lv_Num_Lable"));
				CC_BREAK_IF(pLv_Num_Lable==NULL);
				pLv_Num_Lable->setVisible(false);

			}



			char cName[3][256]={"minren1","xiaoyin","NewProject"};
			if (strcmp("Btn_Char_1",pBtn->getName())==0)	//鸣人
			{
	
				pUiLayer->removeChildByTag(enTagRole);

				CCArmature *pRole = CCArmature::create(cName[0]);
				CC_BREAK_IF(pRole==NULL);
				pRole->setPosition(ptRole);
				pRole->setAnchorPoint(ccp(pRole->getAnchorPoint().x,0));
				pRole->setScale(1.5f);
				pRole->getAnimation()->play("hold",-1,-1,-1,10000);
				pUiLayer->addChild(pRole,enZOrderFront,enTagRole);

				m_nCurrentSkillUpRoleID=1;
			}
			else if (strcmp("Btn_Char_2",pBtn->getName())==0)			//小樱
			{
				pUiLayer->removeChildByTag(enTagRole);

				CCArmature *pRole = CCArmature::create(cName[1]);
				CC_BREAK_IF(pRole==NULL);
				pRole->setPosition(ptRole);
				pRole->setAnchorPoint(ccp(pRole->getAnchorPoint().x,0));
				pRole->setScale(1.5f);
				pRole->getAnimation()->play("hold",-1,-1,-1,10000);
				pUiLayer->addChild(pRole,enZOrderFront,enTagRole);

				m_nCurrentSkillUpRoleID=2;
			}
			else if (strcmp("Btn_Char_3",pBtn->getName())==0)			//佐助
			{
				pUiLayer->removeChildByTag(enTagRole);

				CCArmature *pRole = CCArmature::create(cName[2]);
				CC_BREAK_IF(pRole==NULL);
				pRole->setPosition(ptRole);
				pRole->setAnchorPoint(ccp(pRole->getAnchorPoint().x,0));
				pRole->setScale(1.5f);
				pRole->getAnimation()->play("hold",-1,-1,-1,10000);
				pUiLayer->addChild(pRole,enZOrderFront,enTagRole);

				m_nCurrentSkillUpRoleID=3;
			}





			//初始化技能图标
			CNFServerSkillInfoVec SkillInfoVec;
			CNFServerDataManager::SharedData()->GetSkillInfo(m_nCurrentSkillUpRoleID,SkillInfoVec);
			CNFServerSkillInfoVec::iterator itSkill=SkillInfoVec.begin();

			for (;itSkill!=SkillInfoVec.end();itSkill++)
			{
				if (itSkill->nPos>0)
				{
					switch(itSkill->nPos)
					{
					case 1:
						{
							UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_1")->getChildByName("Skill_Img"));
							CC_BREAK_IF(pSkill_Img==NULL);
							pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
							pSkill_Img->setVisible(true);

							UILabel *pLv_Num_Lable=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_1")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pLv_Num_Lable==NULL);
							pLv_Num_Lable->setVisible(true);

							UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_1")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pSkill_Lv==NULL);
							pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());


							//设置技能信息
							UILabel *pSkillName=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill_Name"));
							CC_BREAK_IF(pSkillName==NULL);

							tagSkillStaticInfo SkillStaticInfo;
							CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nCurrentSkillUpRoleID,itSkill->nSkillID,SkillStaticInfo);

							pSkillName->setText(CCString::createWithFormat("%s",SkillStaticInfo.strSkillName.c_str())->getCString());
						}break;
					case 2:
						{
							UILabel *pLv_Num_Lable=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_2")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pLv_Num_Lable==NULL);
							pLv_Num_Lable->setVisible(true);

							UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_2")->getChildByName("Skill_Img"));
							CC_BREAK_IF(pSkill_Img==NULL);
							pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
							pSkill_Img->setVisible(true);

							UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_2")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pSkill_Lv==NULL);
							pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
						}break;
					case 3:
						{
							UILabel *pLv_Num_Lable=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_3")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pLv_Num_Lable==NULL);
							pLv_Num_Lable->setVisible(true);

							UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_3")->getChildByName("Skill_Img"));
							CC_BREAK_IF(pSkill_Img==NULL);
							pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
							pSkill_Img->setVisible(true);

							UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_3")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pSkill_Lv==NULL);
							pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
						}break;
					case 4:
						{
							UILabel *pLv_Num_Lable=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_4")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pLv_Num_Lable==NULL);
							pLv_Num_Lable->setVisible(true);

							UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_4")->getChildByName("Skill_Img"));
							CC_BREAK_IF(pSkill_Img==NULL);
							pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
							pSkill_Img->setVisible(true);

							UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_4")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pSkill_Lv==NULL);
							pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
						}break;
					case 5:
						{
							UILabel *pLv_Num_Lable=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_5")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pLv_Num_Lable==NULL);
							pLv_Num_Lable->setVisible(true);


							UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_5")->getChildByName("Skill_Img"));
							CC_BREAK_IF(pSkill_Img==NULL);
							pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
							pSkill_Img->setVisible(true);

							UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_5")->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pSkill_Lv==NULL);
							pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
						}break;
					}
				}
			}

			//重置选择技能框
			for (int i=0;i<5;i++)
			{
				UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName(CCString::createWithFormat("Skill_%d",i+1)->getCString())->getChildByName("Select_Img"));
				CC_BREAK_IF(pSelect_Img==NULL);
				if (i==0)
				{
					pSelect_Img->setVisible(true);
				}
				else
				{
					pSelect_Img->setVisible(false);
				}
			}


		}break;
	default:
		break;
	}
}


void CNFTownScene::OnCreateChangeSkillLayer( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{

			//创建技能选择页
			UILayer* pUiLayer = UILayer::create();
			CC_BREAK_IF(pUiLayer==NULL);

			//技能选择页UI
			UIWidget *pSkillChangeLayer = dynamic_cast<UIWidget*>(GUIReader::shareReader()->widgetFromJsonFile("Skill_Intro.json"));
			CC_BREAK_IF(pSkillChangeLayer==NULL);
			pUiLayer->addWidget(pSkillChangeLayer);
			pSkillChangeLayer->setName("SkillChangePage");
			pSkillChangeLayer->setSize(getContentSize());	


			//设置关闭按钮回调
			UIButton* pBtnClose=dynamic_cast<UIButton*>(pSkillChangeLayer->getChildByName("MainUi")->getChildByName("Btn_Close"));
			CC_BREAK_IF(pBtnClose==NULL);
			pBtnClose->addTouchEventListener(this,toucheventselector(CNFTownScene::OnRemoveSkillChangeLayer));


			//设置装备忍术按钮回调
			UIButton* pBtn_Equ=dynamic_cast<UIButton*>(pSkillChangeLayer->getChildByName("MainUi")->getChildByName("Skill_Info")->getChildByName("Btn_Equ"));
			CC_BREAK_IF(pBtn_Equ==NULL);
			pBtn_Equ->addTouchEventListener(this,toucheventselector(CNFTownScene::OnChangeSkill));

			//获取滑动条
			UIScrollView* pScrollView=dynamic_cast<UIScrollView*>(pSkillChangeLayer->getChildByName("MainUi")->getChildByName("Skill_ScrollView"));
			CC_BREAK_IF(pScrollView==NULL);


			//创建技能图标
			UILayout *pSkillPageTest = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Skill_Intro_Page.json"));
			CC_BREAK_IF(pSkillPageTest==NULL);


			CNFServerSkillInfoVec SkillInfoVec;
			CNFServerDataManager::SharedData()->GetSkillInfo(m_nCurrentSkillUpRoleID,SkillInfoVec);

			//计算滑动条高度
			int nSkillCount=0;		//小技能计数
			int nBigSkillCount=0;	//奥义计数
			CNFServerSkillInfoVec::iterator itSkillCu=SkillInfoVec.begin();
			for (;itSkillCu!=SkillInfoVec.end();itSkillCu++)
			{
				if (itSkillCu->nSkillID >=100 )
				{
					nBigSkillCount++;
				}
				else
				{
					nSkillCount++;
				}
			}

			float innerWidth = pScrollView->getSize().width;
			float innerHeight=0;


			//创建主场景UI层
			UILayer* pStudioCurrentChildUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
			CC_BREAK_IF(pStudioCurrentChildUiLayer==NULL);


			//加入主场景UI
			UIWidget *pSkill = dynamic_cast<UIWidget*>(pStudioCurrentChildUiLayer->getWidgetByName("SkillPage"));
			CC_BREAK_IF(pSkill==NULL);


			//获得选中技能槽位置
			int nChangeSkillPos=0;
			for (int k=0;k<5;k++)
			{
				UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName(CCString::createWithFormat("Skill_%d",k+1)->getCString())->getChildByName("Select_Img"));
				CC_BREAK_IF(pSelect_Img==NULL);

				if (pSelect_Img->isVisible() == true)
				{
					nChangeSkillPos=k+1;
				}
			}

			//技能槽4,5为奥义
			if (nChangeSkillPos>3)
			{
				if (nBigSkillCount<4)
				{
					nBigSkillCount=4;
				}
				innerHeight=pSkillPageTest->getSize().height*(nBigSkillCount);
			}
			else
			{
				innerHeight=pSkillPageTest->getSize().height*(nSkillCount);
			}



			//初始化技能图标
			CNFServerSkillInfoVec::iterator itSkill=SkillInfoVec.begin();
			for (int i=0;itSkill!=SkillInfoVec.end();itSkill++)
			{
				if (nChangeSkillPos<=3 && itSkill->nSkillID>100)continue;
				if (nChangeSkillPos>3 &&  itSkill->nSkillID<100)continue;

				//创建图标
				UILayout *pSkillPage = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Skill_Intro_Page.json"));
				CC_BREAK_IF(pSkillPage==NULL);

				//设置图标
				UIImageView *pSkill_Logo=dynamic_cast<UIImageView*>(pSkillPage->getChildByName("Skill_Logo"));
				CC_BREAK_IF(pSkill_Logo==NULL);
				pSkill_Logo->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);

				//设置回调
				UIButton *pBtn_Skill=dynamic_cast<UIButton*>(pSkillPage->getChildByName("Btn_Select"));
				CC_BREAK_IF(pBtn_Skill==NULL);
				pBtn_Skill->addTouchEventListener(this,toucheventselector(CNFTownScene::OnChangeSkillSelect));		

				//若已装备
				if (itSkill->nPos>0)
				{
					UILabel *pHave_Lable=dynamic_cast<UILabel*>(pSkillPage->getChildByName("Have_Lable"));
					CC_BREAK_IF(pHave_Lable==NULL);
					pHave_Lable->setVisible(true);
				}

				//设置技能名字
				UILabel *pSkillName=dynamic_cast<UILabel*>(pSkillPage->getChildByName("Skill_Name"));
				CC_BREAK_IF(pSkillName==NULL);
				tagSkillStaticInfo SkillStaticInfo;
				CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nCurrentSkillUpRoleID,itSkill->nSkillID,SkillStaticInfo);
				pSkillName->setText(CCString::createWithFormat("%s",SkillStaticInfo.strSkillName.c_str())->getCString());

				//设置技能等级
				UILabel *pSkillLv=dynamic_cast<UILabel*>(pSkillPage->getChildByName("Lv_Num_Lable"));
				CC_BREAK_IF(pSkillLv==NULL);
				pSkillLv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());

				//设置位置
				pSkillPage->setPosition(ccp(0, innerHeight - pSkillPage->getSize().height*(i+1)));
				pScrollView->addChild(pSkillPage);

				//设置技能Id
				UILabel *pSkill_Id_Lable=dynamic_cast<UILabel*>(pSkillPage->getChildByName("Skill_Id_Lable"));
				CC_BREAK_IF(pSkill_Id_Lable==NULL);
				pSkill_Id_Lable->setText(CCString::createWithFormat("%d",itSkill->nSkillID)->getCString());

				i++;

			}


			pScrollView->setInnerContainerSize(CCSizeMake(innerWidth, innerHeight));                
			pScrollView->setTouchEnabled(true);
			pScrollView->setBounceEnabled(true);


			addChild(pUiLayer,enZOrderFront,enTagStudioTopUiLayer);
		}break;
	default:
		break;
	}
}



void CNFTownScene::OnRemoveSkillChangeLayer( CCObject *pSender, TouchEventType type )
{
	do 
	{

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				if(getChildByTag(enTagStudioTopUiLayer))removeChildByTag(enTagStudioTopUiLayer);

			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnRemoveSkillChangeLayer Error");
}

void CNFTownScene::OnChangeSkillSelect( CCObject *pSender, TouchEventType type )
{
	do 
	{

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				//获取技能选择层
				UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioTopUiLayer));
				CC_BREAK_IF(pUiLayer==NULL);

				//获取技能选择UI
				UIWidget *pSkillChangeLayer = dynamic_cast<UIWidget*>(pUiLayer->getWidgetByName("SkillChangePage"));
				CC_BREAK_IF(pSkillChangeLayer==NULL);

				//获取滑动条
				UIScrollView* pScrollView=dynamic_cast<UIScrollView*>(pSkillChangeLayer->getChildByName("MainUi")->getChildByName("Skill_ScrollView"));
				CC_BREAK_IF(pScrollView==NULL);

				CCArray *pArray	=pScrollView->getChildren();
				CC_BREAK_IF(pArray==NULL);


				int i=0;
				CCObject *pObj=NULL;
				CCARRAY_FOREACH(pArray,pObj)
				{
					//创建图标
					UILayout *pSkillPage = dynamic_cast<UILayout*>	(pObj);
					CC_BREAK_IF(pSkillPage==NULL);

					//获取选中按钮
					UIButton *pBtn_Skill=dynamic_cast<UIButton*>(pSkillPage->getChildByName("Btn_Select"));
					CC_BREAK_IF(pBtn_Skill==NULL);

					//获取图片按钮
					UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pSkillPage->getChildByName("Select_Img"));
					CC_BREAK_IF(pSelect_Img==NULL);
	
					if (pBtn_Skill==pSender)
					{
						//显示选中图标
						pSelect_Img->setVisible(true);

						//加入主场景UI
						UILayout *pSkill_Info = dynamic_cast<UILayout*>(pSkillChangeLayer->getChildByName("MainUi")->getChildByName("Skill_Info"));
						CC_BREAK_IF(pSkill_Info==NULL);
						pSkill_Info->setVisible(true);

						//初始化技能图标
						CNFServerSkillInfoVec SkillInfoVec;
						CNFServerDataManager::SharedData()->GetSkillInfo(m_nCurrentSkillUpRoleID,SkillInfoVec);

						CNFServerSkillInfoVec::iterator itSkill=SkillInfoVec.begin();
						for (int j=0;itSkill!=SkillInfoVec.end();itSkill++)
						{
							//根据ID区分奥义和技能
							if (m_nCurrentSelectSkillID>=100 && itSkill->nSkillID<100)continue;
							if (m_nCurrentSelectSkillID<100 && itSkill->nSkillID>100)continue;

							if (j==i )
							{
								//设置技能信息
								UILabel *pSkill_Name = dynamic_cast<UILabel*>(pSkill_Info->getChildByName("Skill_Name"));
								CC_BREAK_IF(pSkill_Name==NULL);
								tagSkillStaticInfo SkillStaticInfo;
								CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nCurrentSkillUpRoleID,itSkill->nSkillID,SkillStaticInfo);
								pSkill_Name->setText(CCString::createWithFormat("%s",SkillStaticInfo.strSkillName.c_str())->getCString());
							}

							j++;
						}
					}
					else
					{
						pSelect_Img->setVisible(false);
					}

					i++;
				}


			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnChangeSkillSelect Error");
}


void CNFTownScene::OnChangeSkill( CCObject *pSender, TouchEventType type )
{
	do 
	{

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				//获取技能选择层
				UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioTopUiLayer));
				CC_BREAK_IF(pUiLayer==NULL);

				//获取技能选择UI
				UIWidget *pSkillChangeLayer = dynamic_cast<UIWidget*>(pUiLayer->getWidgetByName("SkillChangePage"));
				CC_BREAK_IF(pSkillChangeLayer==NULL);

				//获取滑动条
				UIScrollView* pScrollView=dynamic_cast<UIScrollView*>(pSkillChangeLayer->getChildByName("MainUi")->getChildByName("Skill_ScrollView"));
				CC_BREAK_IF(pScrollView==NULL);

				CCArray *pArray	=pScrollView->getChildren();
				CC_BREAK_IF(pArray==NULL);


				int i=0;
				CCObject *pObj=NULL;
				CCARRAY_FOREACH(pArray,pObj)
				{
					//创建图标
					UILayout *pSkillPage = dynamic_cast<UILayout*>	(pObj);
					CC_BREAK_IF(pSkillPage==NULL);

					UIButton *pBtn_Skill=dynamic_cast<UIButton*>(pSkillPage->getChildByName("Btn_Select"));
					CC_BREAK_IF(pBtn_Skill==NULL);

					UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pSkillPage->getChildByName("Select_Img"));
					CC_BREAK_IF(pSelect_Img==NULL);

		
					if (pSelect_Img->isVisible()==true)
					{


						//获取技能层
						UILayer* pStudioCurrentChildUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioCurrentChildUiLayer));
						CC_BREAK_IF(pStudioCurrentChildUiLayer==NULL);


						//获取技能层UI
						UIWidget *pSkill = dynamic_cast<UIWidget*>(pStudioCurrentChildUiLayer->getWidgetByName("SkillPage"));
						CC_BREAK_IF(pSkill==NULL);


						//获取技能Id
						UILabel *pSkill_Id_Lable=dynamic_cast<UILabel*>(pSkillPage->getChildByName("Skill_Id_Lable"));
						CC_BREAK_IF(pSkill_Id_Lable==NULL);
						int nSkillId =atoi(pSkill_Id_Lable->getStringValue());

						//修改技能数据
						CNFServerSkillInfoVec SkillInfoVec;
						CNFServerDataManager::SharedData()->GetSkillInfo(m_nCurrentSkillUpRoleID,SkillInfoVec);
						CNFServerSkillInfoVec::iterator itSkill=SkillInfoVec.begin();

						int nChangeSkillPos=0;

						//若选中技能槽已装备技能
						if (m_nCurrentSelectSkillID>0)
						{
							//获取技能位置，卸下原来技能
							for (;itSkill!=SkillInfoVec.end();itSkill++)
							{
								if (itSkill->nSkillID == m_nCurrentSelectSkillID && itSkill->nPos>0)
								{
									nChangeSkillPos=itSkill->nPos;
									itSkill->nPos=0;
								}
							}

						}
						//若选中技能槽没有装备技能
						else
						{
							//获取技能位置
							for (int k=0;k<5;k++)
							{
								UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName(CCString::createWithFormat("Skill_%d",k+1)->getCString())->getChildByName("Select_Img"));
								CC_BREAK_IF(pSelect_Img==NULL);

								if (pSelect_Img->isVisible() == true)
								{
									nChangeSkillPos=k+1;
								}
							}
						}


						//替换技能
						itSkill=SkillInfoVec.begin();
						for (;itSkill!=SkillInfoVec.end();itSkill++)
						{
							if (itSkill->nSkillID == nSkillId )
							{
								itSkill->nPos=nChangeSkillPos;
							}
						}

						//更新数据
						CNFServerDataManager::SharedData()->SetSkillInfo(m_nCurrentSkillUpRoleID,SkillInfoVec);


						//设置技能按钮回调
						for (int i=0;i<5;i++)
						{
							UIButton *pSkill_Btn=dynamic_cast<UIButton*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName(CCString::createWithFormat("Skill_%d",i+1)->getCString()));
							CC_BREAK_IF(pSkill_Btn==NULL);
							pSkill_Btn->addTouchEventListener(this,toucheventselector(CNFTownScene::OnChooseSkill));

						}


						//默认隐藏技能等级，技能图标
						UIWidget* pChar=dynamic_cast<UIWidget*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Role_Pos_Img"));
						CC_BREAK_IF(pChar==NULL);
						for (int i=0;i<5;i++)
						{
							UIButton *pBtn=dynamic_cast<UIButton*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName(CCString::createWithFormat("Skill_%d",i+1)->getCString()));
							CC_BREAK_IF(pBtn==NULL);


							UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pBtn->getChildByName("Skill_Img"));
							CC_BREAK_IF(pSkill_Img==NULL);
							pSkill_Img->setVisible(false);

							UILabel *pLv_Num_Lable=dynamic_cast<UILabel*>(pBtn->getChildByName("Lv_Num_Lable"));
							CC_BREAK_IF(pLv_Num_Lable==NULL);
							pLv_Num_Lable->setVisible(false);
						}



						itSkill=SkillInfoVec.begin();
						for (;itSkill!=SkillInfoVec.end();itSkill++)
						{
							if (itSkill->nPos>0)
							{
								//根据数据匹配技能槽
								if (itSkill->nPos==nChangeSkillPos)
								{
									//设置技能信息
									UILabel *pSkillName=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill_Info")->getChildByName("Skill_Name"));
									CC_BREAK_IF(pSkillName==NULL);

									tagSkillStaticInfo SkillStaticInfo;
									CNFStaticDataManager::SharedData()->GetSkillStaticInfo(m_nCurrentSkillUpRoleID,itSkill->nSkillID,SkillStaticInfo);

									pSkillName->setText(CCString::createWithFormat("%s",SkillStaticInfo.strSkillName.c_str())->getCString());

									//显示技能详细信息
									UIWidget *pSkill_Info=dynamic_cast<UIWidget*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill_Info"));
									CC_BREAK_IF(pSkill_Info==NULL);
									pSkill_Info->setVisible(true);


									//设置当前技能ID
									m_nCurrentSelectSkillID=itSkill->nSkillID;
								}



								switch(itSkill->nPos)
								{
								case 1:
									{
										//显示技能图标
										UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_1")->getChildByName("Skill_Img"));
										CC_BREAK_IF(pSkill_Img==NULL);
										pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
										pSkill_Img->setVisible(true);

										//显示等级
										UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_1")->getChildByName("Lv_Num_Lable"));
										CC_BREAK_IF(pSkill_Lv==NULL);
										pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
										pSkill_Lv->setVisible(true);

									}break;
								case 2:
									{
										UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_2")->getChildByName("Skill_Img"));
										CC_BREAK_IF(pSkill_Img==NULL);
										pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
										pSkill_Img->setVisible(true);

										UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_2")->getChildByName("Lv_Num_Lable"));
										CC_BREAK_IF(pSkill_Lv==NULL);
										pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
										pSkill_Lv->setVisible(true);
									}break;
								case 3:
									{
		
										UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_3")->getChildByName("Skill_Img"));
										CC_BREAK_IF(pSkill_Img==NULL);
										pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
										pSkill_Img->setVisible(true);

										UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_3")->getChildByName("Lv_Num_Lable"));
										CC_BREAK_IF(pSkill_Lv==NULL);
										pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
										pSkill_Lv->setVisible(true);
									}break;
								case 4:
									{
										UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_4")->getChildByName("Skill_Img"));
										CC_BREAK_IF(pSkill_Img==NULL);
										pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
										pSkill_Img->setVisible(true);

										UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_4")->getChildByName("Lv_Num_Lable"));
										CC_BREAK_IF(pSkill_Lv==NULL);
										pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
										pSkill_Lv->setVisible(true);

									}break;
								case 5:
									{
										UIImageView *pSkill_Img=dynamic_cast<UIImageView*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_5")->getChildByName("Skill_Img"));
										CC_BREAK_IF(pSkill_Img==NULL);
										pSkill_Img->loadTexture(CCString::createWithFormat("Skill/r%d_s%d.png",m_nCurrentSkillUpRoleID,itSkill->nSkillID)->getCString(),UI_TEX_TYPE_LOCAL);
										pSkill_Img->setVisible(true);

										UILabel *pSkill_Lv=dynamic_cast<UILabel*>(pSkill->getChildByName("Player_Skill_Panel")->getChildByName("Skill")->getChildByName("Skill_5")->getChildByName("Lv_Num_Lable"));
										CC_BREAK_IF(pSkill_Lv==NULL);
										pSkill_Lv->setText(CCString::createWithFormat("Lv%d",itSkill->nSkillLevel)->getCString());
										pSkill_Lv->setVisible(true);
									}break;
								}
							}
						}

						//移除选择层
						if(getChildByTag(enTagStudioTopUiLayer))removeChildByTag(enTagStudioTopUiLayer);
					}

					i++;
				}


			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnChangeSkillSelect Error");
}



void CNFTownScene::OnSwitchToEquUp( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				if(getChildByTag(enTagStudioCurrentChildUiLayer))return;
				//创建主场景UI层
				UILayer* pUiLayer = UILayer::create(); 
				CC_BREAK_IF(pUiLayer==NULL);


				//加入主场景UI
				UIWidget *pEqu_UpGrade = dynamic_cast<UIWidget*>(GUIReader::shareReader()->widgetFromJsonFile("Equ_UpGrade.json"));
				CC_BREAK_IF(pEqu_UpGrade==NULL);
				pUiLayer->addWidget(pEqu_UpGrade);
				pEqu_UpGrade->setName("EmailPage");
				pEqu_UpGrade->setSize(getContentSize());	


				//设置关闭按钮回调
				UIButton* pBtnClose=dynamic_cast<UIButton*>(pEqu_UpGrade->getChildByName("Btn_Close"));
				CC_BREAK_IF(pBtnClose==NULL);
				pBtnClose->addTouchEventListener(this,toucheventselector(CNFTownScene::OnClosePlayerInfoPage));

				//
				UILayout* pPlayer_Title_Panel=dynamic_cast<UILayout*>(pUiLayer->getWidgetByName("EmailPage")->getChildByName("Bag_Panel")->getChildByName("Title_Panel"));
				CC_BREAK_IF(pPlayer_Title_Panel==NULL);
				pPlayer_Title_Panel->setEnabled(false);


				addChild(pUiLayer,enZOrderFront,enTagStudioCurrentChildUiLayer);
			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToEquUp Error!");
}

void CNFTownScene::OnSwitchToSkill( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into Skill");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToSkill Error!");
}

void CNFTownScene::OnSwitchToNinjaSkill( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into NinjaSkill");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToNinjaSkill Error!");
}

void CNFTownScene::OnSwitchToBigSkill( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into BigSkill ");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToBigSkill Error!");
}

void CNFTownScene::OnSwitchToTask( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into Task");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwithToTask Error!");
}

void CNFTownScene::OnSwitchToGuild( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into Guild");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwithToGuild Error!");
}

void CNFTownScene::OnSwitchToRank( CCObject *pSender, TouchEventType type )
{
	do 
	{
		CCLog("into Rank");
		return;
	} while (false);
	CCLog("CNFTownScene::OnSwithToRank Error!");
}

//-------------------------------------------------------------------------

void CNFTownScene::OnBtnFriendSwitchCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{
		UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
		CC_BREAK_IF(pUiLayer==NULL);

		//获取按下图
		UIWidget* pFriendWit=dynamic_cast<UIWidget*>(pUiLayer->getWidgetByName("MainSceneWidget_Friend"));
		CC_BREAK_IF(pFriendWit==NULL);


		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				if (m_bBtnFriendSwitchOpen)
				{
					pFriendWit->setPositionX(SCREEN_WIDTH+5);
					m_bBtnFriendSwitchOpen=false;
				}
				else
				{
					pFriendWit->setPositionX(SCREEN_WIDTH-170);
					m_bBtnFriendSwitchOpen=true;

				}

			}break;
		}


		return;
	} while (false);
	CCLog("CNFTownScene::OnBtnFriendCallBack Error");
}

void CNFTownScene::OnBtnFriendCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{
		UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
		CC_BREAK_IF(pUiLayer==NULL);

		//获取按下图
		UIImageView* pSelectImg=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("MainSceneWidget_Friend")->getChildByName("MainUi")->getChildByName("Btn_Select_Img"));
		CC_BREAK_IF(pSelectImg==NULL);

		UIButton *pBtn=dynamic_cast<UIButton*>(pSender);		
		CC_BREAK_IF(pBtn==NULL);

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				pSelectImg->setPosition(pBtn->getPosition());


			}break;
		}


		return;
	} while (false);
	CCLog("CNFTownScene::OnBtnFriendCallBack Error");
}

void CNFTownScene::OnBtnArroundCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{
		UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
		CC_BREAK_IF(pUiLayer==NULL);

		//获取按下图
		UIImageView* pSelectImg=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("MainSceneWidget_Friend")->getChildByName("MainUi")->getChildByName("Btn_Select_Img"));
		CC_BREAK_IF(pSelectImg==NULL);

		UIButton *pBtn=dynamic_cast<UIButton*>(pSender);		
		CC_BREAK_IF(pBtn==NULL);

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				pSelectImg->setPosition(pBtn->getPosition());


			}break;
		}


		return;
	} while (false);
	CCLog("CNFTownScene::OnBtnArroundCallBack Error");
}

void CNFTownScene::OnBtnSearchCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{
		UILayer* pUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
		CC_BREAK_IF(pUiLayer==NULL);

		//获取按下图
		UIImageView* pSelectImg=dynamic_cast<UIImageView*>(pUiLayer->getWidgetByName("MainSceneWidget_Friend")->getChildByName("MainUi")->getChildByName("Btn_Select_Img"));
		CC_BREAK_IF(pSelectImg==NULL);

		UIButton *pBtn=dynamic_cast<UIButton*>(pSender);		
		CC_BREAK_IF(pBtn==NULL);

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				pSelectImg->setPosition(pBtn->getPosition());


			}break;
		}


		return;
	} while (false);
	CCLog("CNFTownScene::OnBtnSearchCallBack Error");
}

void CNFTownScene::OnSwitchToSettingPage( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				if(getChildByTag(enTagStudioCurrentChildUiLayer))return;
				//创建主场景UI层
				UILayer* pUiLayer = UILayer::create(); 
				CC_BREAK_IF(pUiLayer==NULL);


				//加入主场景UI
				UIWidget *pSettingLayer = dynamic_cast<UIWidget*>(GUIReader::shareReader()->widgetFromJsonFile("Setting.json"));
				CC_BREAK_IF(pSettingLayer==NULL);
				pUiLayer->addWidget(pSettingLayer);
				pSettingLayer->setName("SettingLayer");
				pSettingLayer->setSize(getContentSize());	
				pSettingLayer->setTouchEnabled(true);

				//设置关闭按钮回调
				UIButton* pBtnClose=dynamic_cast<UIButton*>(pSettingLayer->getChildByName("MainUi")->getChildByName("Btn_Close"));
				CC_BREAK_IF(pBtnClose==NULL);
				pBtnClose->addTouchEventListener(this,toucheventselector(CNFTownScene::OnClosePlayerInfoPage));


				//设置音乐按钮回调
				UIButton* pBtn_Music=dynamic_cast<UIButton*>(pSettingLayer->getChildByName("Btn_Bg")->getChildByName("Btn_Music"));
				CC_BREAK_IF(pBtn_Music==NULL);
				pBtn_Music->addTouchEventListener(this,toucheventselector(CNFTownScene::OnMusicBtnClick));

				//设置音效按钮回调
				UIButton* pBtn_Sound=dynamic_cast<UIButton*>(pSettingLayer->getChildByName("Btn_Bg")->getChildByName("Btn_Sound"));
				CC_BREAK_IF(pBtn_Sound==NULL);
				pBtn_Sound->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSoundBtnClickPage));

				//设置切换人物按钮回调
				UIButton* pBtn_Change_Role=dynamic_cast<UIButton*>(pSettingLayer->getChildByName("Btn_Change_Role"));
				CC_BREAK_IF(pBtn_Change_Role==NULL);
				pBtn_Change_Role->addTouchEventListener(this,toucheventselector(CNFTownScene::OnSwitchToChangeRoleLayerClick));


				//设置切换人物按钮回调
				UIButton* pBtn_Re_Log=dynamic_cast<UIButton*>(pSettingLayer->getChildByName("Btn_Re_Log"));
				CC_BREAK_IF(pBtn_Re_Log==NULL);
				pBtn_Re_Log->addTouchEventListener(this,toucheventselector(CNFTownScene::OnOnSwitchToLoginLayer));

				//设置隐藏其他玩家按钮回调
				UIButton* pBtn_Hide_Players=dynamic_cast<UIButton*>(pSettingLayer->getChildByName("Btn_Bg")->getChildByName("Btn_Hide_Players"));
				CC_BREAK_IF(pBtn_Hide_Players==NULL);
				pBtn_Hide_Players->addTouchEventListener(this,toucheventselector(CNFTownScene::OnHideOthersBtnClickPage));

				addChild(pUiLayer,enZOrderFront,enTagStudioCurrentChildUiLayer);
			}break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToSettingPage Error");
}

void CNFTownScene::OnMusicBtnClick( CCObject *pSender, TouchEventType type )
{
	do 
	{



		UIButton *pBtn=dynamic_cast<UIButton*>(pSender);		
		CC_BREAK_IF(pBtn==NULL);

		//获取按下图
		UIImageView* pSelectImg=dynamic_cast<UIImageView*>(pBtn->getChildByName("Mark"));
		CC_BREAK_IF(pSelectImg==NULL);

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				if (pSelectImg->isVisible())
				{
					pSelectImg->setVisible(false);
				}
				else
				{
					pSelectImg->setVisible(true);
				}

			}break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToMusicPage Error");
}

void CNFTownScene::OnSoundBtnClickPage( CCObject *pSender, TouchEventType type )
{
	do 
	{
		UIButton *pBtn=dynamic_cast<UIButton*>(pSender);		
		CC_BREAK_IF(pBtn==NULL);

		//获取按下图
		UIImageView* pSelectImg=dynamic_cast<UIImageView*>(pBtn->getChildByName("Mark"));
		CC_BREAK_IF(pSelectImg==NULL);

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				if (pSelectImg->isVisible())
				{
					pSelectImg->setVisible(false);
				}
				else
				{
					pSelectImg->setVisible(true);
				}

			}break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToSoundPage Error");
}

void CNFTownScene::OnSwitchToChangeRoleLayerClick( CCObject *pSender, TouchEventType type )
{
	do 
	{

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				//人物选择场景
				CCScene * pScene = CNFRoleSelectLayer::scene();
				CC_BREAK_IF(pScene==NULL);
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));

			}break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToChangeRoleLayerClick Error");
}

void CNFTownScene::OnOnSwitchToLoginLayer( CCObject *pSender, TouchEventType type )
{
	do 
	{

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				//跳转到服务器选择界面
				CCScene * pScene = CNFLoginScene::scene();
				CC_BREAK_IF(pScene==NULL);
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));

			}break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnOnSwitchToLoginLayer Error");
}

void CNFTownScene::OnHideOthersBtnClickPage( CCObject *pSender, TouchEventType type )
{
	do 
	{
		UIButton *pBtn=dynamic_cast<UIButton*>(pSender);		
		CC_BREAK_IF(pBtn==NULL);

		//获取按下图
		UIImageView* pSelectImg=dynamic_cast<UIImageView*>(pBtn->getChildByName("Mark"));
		CC_BREAK_IF(pSelectImg==NULL);

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				if (pSelectImg->isVisible())
				{
					pSelectImg->setVisible(false);
				}
				else
				{
					pSelectImg->setVisible(true);
				}

			}break;
		}

		return;
	} while (false);
	CCLog("CNFTownScene::OnSwitchToHideOthersPage Error");
}

void CNFTownScene::keyBackClicked()
{
	do 
	{
		if(getChildByTag(enTagStudioCurrentChildUiLayer))removeChildByTag(enTagStudioCurrentChildUiLayer);
		//创建主场景UI层
		UILayer* pUiLayer = UILayer::create(); 
		CC_BREAK_IF(pUiLayer==NULL);


		//加入主场景UI
		UIWidget *pAndroid_Back_OffLayer = dynamic_cast<UIWidget*>(GUIReader::shareReader()->widgetFromJsonFile("Android_Back_Off.json"));
		CC_BREAK_IF(pAndroid_Back_OffLayer==NULL);
		pUiLayer->addWidget(pAndroid_Back_OffLayer);

		pAndroid_Back_OffLayer->setSize(getContentSize());	
		pAndroid_Back_OffLayer->setTouchEnabled(true);

		//设置关闭按钮回调
		UIButton* pBtnClose=dynamic_cast<UIButton*>(pAndroid_Back_OffLayer->getChildByName("Dlg")->getChildByName("Btn_Close"));
		CC_BREAK_IF(pBtnClose==NULL);
		pBtnClose->addTouchEventListener(this,toucheventselector(CNFTownScene::OnClosePlayerInfoPage));

		//设置关闭游戏按钮回调
		UIButton* pBtnCloseGame=dynamic_cast<UIButton*>(pAndroid_Back_OffLayer->getChildByName("Dlg")->getChildByName("Button_back_town"));
		CC_BREAK_IF(pBtnCloseGame==NULL);
		pBtnCloseGame->addTouchEventListener(this,toucheventselector(CNFTownScene::OnCloseGame));


		addChild(pUiLayer,enZOrderFront,enTagStudioCurrentChildUiLayer);	
		return;
	} while (false);
	CCLog("CNFTownScene::keyBackClicked Error");
}

void CNFTownScene::OnCloseGame( CCObject *pSender, TouchEventType type )
{
	switch (type)
	{
	case TOUCH_EVENT_ENDED:
		{
			 CCDirector::sharedDirector()->end();
		}break;
	}
}








