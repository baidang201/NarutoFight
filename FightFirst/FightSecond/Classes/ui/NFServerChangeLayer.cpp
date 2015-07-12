#include "NFServerChangeLayer.h"
#include "NFServerChangeScrollLayer.h"

CNFServerChangeLayer * CNFServerChangeLayer::CreateLayer(int nCurrentServerID,int nServerTotalNum)
{
	CNFServerChangeLayer *pRet = new CNFServerChangeLayer();
	if (pRet && pRet->InitLayer(nCurrentServerID,nServerTotalNum))
	{
		pRet->autorelease();
		return pRet;
	}
	CCLog("Fun CNFServerChangeLayer::CreateLayer Error!");
	delete pRet; 
	pRet = NULL; 
	return NULL;
}

bool CNFServerChangeLayer::InitLayer(int nCurrentServerID,int nServerTotalNum)
{
	do 
	{
		//初始化父类
		CC_BREAK_IF(CCLayer::init()==false);

		m_nCurrentServerID = nCurrentServerID;

		/************************************************************************/
		/*			背景图片                                                                     */
		/************************************************************************/
		//创建背景
		CCSprite * pBg = CCSprite::create("ui/serverselect_bg.png");
		CC_BREAK_IF(pBg==NULL);
		pBg->setPosition(SCREEN_CENTER);
		pBg->setScaleX( (float)CCDirector::sharedDirector()->getWinSize().width/(float)pBg->getContentSize().width );
		addChild(pBg,enZOrderBack);

		//创建上次登录图片
		CCSprite * pBg1 = CCSprite::create("ui/serverselect_title_1.png");
		CC_BREAK_IF(pBg1==NULL);
		pBg1->setPosition(ccp(150+_NF_SCREEN_WIDTH_DIS_,420));
		addChild(pBg1,enZOrderBack);

		//创建全服务器图片
		CCSprite * pBg2 = CCSprite::create("ui/serverselect_title_2.png");
		CC_BREAK_IF(pBg2==NULL);
		pBg2->setPosition(ccp(160+_NF_SCREEN_WIDTH_DIS_,300));
		addChild(pBg2,enZOrderBack);

		/************************************************************************/
		/*			创建按钮                                                                     */
		/************************************************************************/
		CCMenu * pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(CCPointZero);
		addChild(pMenu,enZOrderFront);

		//创建返回按钮
		//CCMenuItemSprite * pBtnBack = CCMenuItemSprite::create(
		//	CCSprite::create("ui/serverselect_btn_back_n.png"),
		//	CCSprite::create("ui/serverselect_btn_back_p.png"),
		//	this,
		//	menu_selector(CNFServerChangeLayer::OnBtnCallBack));
		//CC_BREAK_IF(pBtnBack==NULL);
		//pBtnBack->setPosition(ccp(SCREEN_WIDTH - 30,SCREEN_HEIGHT-30));
		//pMenu->addChild(pBtnBack,enZOrderFront);

		//创建当前服务器按钮
		CCMenuItemSprite * pBtnCurrentServer = CCMenuItemSprite::create(
			CCSprite::create("ui/btn_enter_n.png"),
			CCSprite::create("ui/btn_enter_p.png"),
			this,
			menu_selector(CNFServerChangeLayer::OnBtnCallBack));
		CC_BREAK_IF(pBtnCurrentServer==NULL);
		pBtnCurrentServer->setPosition(ccp(190+_NF_SCREEN_WIDTH_DIS_,380));
		pMenu->addChild(pBtnCurrentServer,enZOrderFront);

		//创建左侧按钮
		CCMenuItemSprite * pBtnLeft = CCMenuItemSprite::create(
			CCSprite::create("ui/serverselect_btn_back_n.png"),
			CCSprite::create("ui/serverselect_btn_back_p.png"),
			this,
			menu_selector(CNFServerChangeLayer::OnBtnLeftMove));
		CC_BREAK_IF(pBtnLeft==NULL);
		pBtnLeft->setPosition(ccp(50+_NF_SCREEN_WIDTH_DIS_,150));
		pMenu->addChild(pBtnLeft,enZOrderFront);

		//创建右侧按钮
		CCMenuItemSprite * pBtnRight = CCMenuItemSprite::create(
			CCSprite::create("ui/serverselect_btn_back_n.png"),
			CCSprite::create("ui/serverselect_btn_back_p.png"),
			this,
			menu_selector(CNFServerChangeLayer::OnBtnRightMove));
		CC_BREAK_IF(pBtnRight==NULL);
		pBtnRight->setPosition(ccp(SCREEN_WIDTH-_NF_SCREEN_WIDTH_DIS_-50,150));
		pMenu->addChild(pBtnRight,enZOrderFront);

		/************************************************************************/
		/*				创建label                                                                     */
		/************************************************************************/
		CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/serverselect_xml.xml");
		CC_BREAK_IF(pDicLang==NULL);

		//服务器名称label
		CCString *pStrServerName = dynamic_cast<CCString*>(pDicLang->objectForKey("server_name"));
		char szName[NAME_LEN] = {0};
		sprintf(szName,pStrServerName->m_sString.c_str(),nCurrentServerID);
		CCLabelTTF * pLabelServerName = CCLabelTTF::create(szName,"Arial",20);
		CC_BREAK_IF(pLabelServerName==NULL);
		pLabelServerName->setPosition(pBtnCurrentServer->getPosition());
		addChild(pLabelServerName,enZOrderFront);


		/************************************************************************/
		/*					创建滑动层                                                                     */
		/************************************************************************/
		//创建滑动层
		CNFServerChangeScrollLayer * pLayer = CNFServerChangeScrollLayer::CreateLayer(nServerTotalNum);	//Item数组，x轴起始位置
		CC_BREAK_IF(pLayer==NULL);
		pLayer->SetMsg(this);
		addChild(pLayer,enZOrderFront,enTagScrollLayer);


		return true;
	} while (false);
	CCLog("Fun CNFServerChangeLayer::init Error!");
	return false;
}

//按钮回调：返回
void CNFServerChangeLayer::OnBtnCallBack( CCObject * pObj )
{
	//发消息：更改服务器
	tagServerSelect Info;
	Info.nServerID = m_nCurrentServerID;
	SendMsg(enMsg_ServerSelect,&Info,sizeof(Info));

	//移除自身
	removeFromParentAndCleanup(true);
}

//按钮回调：向左滑动
void CNFServerChangeLayer::OnBtnLeftMove( CCObject * pObj )
{
	do 
	{
		//滑动层
		CNFServerChangeScrollLayer * pLayer = dynamic_cast<CNFServerChangeScrollLayer *>(getChildByTag(enTagScrollLayer));
		CC_BREAK_IF(pLayer==NULL);
		pLayer->SetRightMove();

		return ;
	} while (false);
	CCLog("Fun CNFServerChangeLayer::OnBtnLeftMove Error!");
}

//按钮回调：向右滑动
void CNFServerChangeLayer::OnBtnRightMove( CCObject * pObj )
{
	do 
	{
		//滑动层
		CNFServerChangeScrollLayer * pLayer = dynamic_cast<CNFServerChangeScrollLayer *>(getChildByTag(enTagScrollLayer));
		CC_BREAK_IF(pLayer==NULL);
		pLayer->SetLeftMove();

		return ;
	} while (false);
	CCLog("Fun CNFServerChangeLayer::OnBtnRightMove Error!");
}


//消息接收
void CNFServerChangeLayer::RecMsg( int nMsgID,void* pInfo,int nSize )
{
	switch (nMsgID)
	{
	case enMsg_ServerSelect:
		{
			tagServerSelect * pCmd = (tagServerSelect *)pInfo;

			//发消息：更改服务器
			tagServerSelect Info;
			Info.nServerID = pCmd->nServerID;

			SendMsg(enMsg_ServerSelect,&Info,sizeof(Info));

			//移除自身
			removeFromParentAndCleanup(true);

		}break;
	}
}



