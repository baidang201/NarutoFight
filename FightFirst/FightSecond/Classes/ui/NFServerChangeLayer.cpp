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
		//��ʼ������
		CC_BREAK_IF(CCLayer::init()==false);

		m_nCurrentServerID = nCurrentServerID;

		/************************************************************************/
		/*			����ͼƬ                                                                     */
		/************************************************************************/
		//��������
		CCSprite * pBg = CCSprite::create("ui/serverselect_bg.png");
		CC_BREAK_IF(pBg==NULL);
		pBg->setPosition(SCREEN_CENTER);
		pBg->setScaleX( (float)CCDirector::sharedDirector()->getWinSize().width/(float)pBg->getContentSize().width );
		addChild(pBg,enZOrderBack);

		//�����ϴε�¼ͼƬ
		CCSprite * pBg1 = CCSprite::create("ui/serverselect_title_1.png");
		CC_BREAK_IF(pBg1==NULL);
		pBg1->setPosition(ccp(150+_NF_SCREEN_WIDTH_DIS_,420));
		addChild(pBg1,enZOrderBack);

		//����ȫ������ͼƬ
		CCSprite * pBg2 = CCSprite::create("ui/serverselect_title_2.png");
		CC_BREAK_IF(pBg2==NULL);
		pBg2->setPosition(ccp(160+_NF_SCREEN_WIDTH_DIS_,300));
		addChild(pBg2,enZOrderBack);

		/************************************************************************/
		/*			������ť                                                                     */
		/************************************************************************/
		CCMenu * pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(CCPointZero);
		addChild(pMenu,enZOrderFront);

		//�������ذ�ť
		//CCMenuItemSprite * pBtnBack = CCMenuItemSprite::create(
		//	CCSprite::create("ui/serverselect_btn_back_n.png"),
		//	CCSprite::create("ui/serverselect_btn_back_p.png"),
		//	this,
		//	menu_selector(CNFServerChangeLayer::OnBtnCallBack));
		//CC_BREAK_IF(pBtnBack==NULL);
		//pBtnBack->setPosition(ccp(SCREEN_WIDTH - 30,SCREEN_HEIGHT-30));
		//pMenu->addChild(pBtnBack,enZOrderFront);

		//������ǰ��������ť
		CCMenuItemSprite * pBtnCurrentServer = CCMenuItemSprite::create(
			CCSprite::create("ui/btn_enter_n.png"),
			CCSprite::create("ui/btn_enter_p.png"),
			this,
			menu_selector(CNFServerChangeLayer::OnBtnCallBack));
		CC_BREAK_IF(pBtnCurrentServer==NULL);
		pBtnCurrentServer->setPosition(ccp(190+_NF_SCREEN_WIDTH_DIS_,380));
		pMenu->addChild(pBtnCurrentServer,enZOrderFront);

		//������ఴť
		CCMenuItemSprite * pBtnLeft = CCMenuItemSprite::create(
			CCSprite::create("ui/serverselect_btn_back_n.png"),
			CCSprite::create("ui/serverselect_btn_back_p.png"),
			this,
			menu_selector(CNFServerChangeLayer::OnBtnLeftMove));
		CC_BREAK_IF(pBtnLeft==NULL);
		pBtnLeft->setPosition(ccp(50+_NF_SCREEN_WIDTH_DIS_,150));
		pMenu->addChild(pBtnLeft,enZOrderFront);

		//�����Ҳఴť
		CCMenuItemSprite * pBtnRight = CCMenuItemSprite::create(
			CCSprite::create("ui/serverselect_btn_back_n.png"),
			CCSprite::create("ui/serverselect_btn_back_p.png"),
			this,
			menu_selector(CNFServerChangeLayer::OnBtnRightMove));
		CC_BREAK_IF(pBtnRight==NULL);
		pBtnRight->setPosition(ccp(SCREEN_WIDTH-_NF_SCREEN_WIDTH_DIS_-50,150));
		pMenu->addChild(pBtnRight,enZOrderFront);

		/************************************************************************/
		/*				����label                                                                     */
		/************************************************************************/
		CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/serverselect_xml.xml");
		CC_BREAK_IF(pDicLang==NULL);

		//����������label
		CCString *pStrServerName = dynamic_cast<CCString*>(pDicLang->objectForKey("server_name"));
		char szName[NAME_LEN] = {0};
		sprintf(szName,pStrServerName->m_sString.c_str(),nCurrentServerID);
		CCLabelTTF * pLabelServerName = CCLabelTTF::create(szName,"Arial",20);
		CC_BREAK_IF(pLabelServerName==NULL);
		pLabelServerName->setPosition(pBtnCurrentServer->getPosition());
		addChild(pLabelServerName,enZOrderFront);


		/************************************************************************/
		/*					����������                                                                     */
		/************************************************************************/
		//����������
		CNFServerChangeScrollLayer * pLayer = CNFServerChangeScrollLayer::CreateLayer(nServerTotalNum);	//Item���飬x����ʼλ��
		CC_BREAK_IF(pLayer==NULL);
		pLayer->SetMsg(this);
		addChild(pLayer,enZOrderFront,enTagScrollLayer);


		return true;
	} while (false);
	CCLog("Fun CNFServerChangeLayer::init Error!");
	return false;
}

//��ť�ص�������
void CNFServerChangeLayer::OnBtnCallBack( CCObject * pObj )
{
	//����Ϣ�����ķ�����
	tagServerSelect Info;
	Info.nServerID = m_nCurrentServerID;
	SendMsg(enMsg_ServerSelect,&Info,sizeof(Info));

	//�Ƴ�����
	removeFromParentAndCleanup(true);
}

//��ť�ص������󻬶�
void CNFServerChangeLayer::OnBtnLeftMove( CCObject * pObj )
{
	do 
	{
		//������
		CNFServerChangeScrollLayer * pLayer = dynamic_cast<CNFServerChangeScrollLayer *>(getChildByTag(enTagScrollLayer));
		CC_BREAK_IF(pLayer==NULL);
		pLayer->SetRightMove();

		return ;
	} while (false);
	CCLog("Fun CNFServerChangeLayer::OnBtnLeftMove Error!");
}

//��ť�ص������һ���
void CNFServerChangeLayer::OnBtnRightMove( CCObject * pObj )
{
	do 
	{
		//������
		CNFServerChangeScrollLayer * pLayer = dynamic_cast<CNFServerChangeScrollLayer *>(getChildByTag(enTagScrollLayer));
		CC_BREAK_IF(pLayer==NULL);
		pLayer->SetLeftMove();

		return ;
	} while (false);
	CCLog("Fun CNFServerChangeLayer::OnBtnRightMove Error!");
}


//��Ϣ����
void CNFServerChangeLayer::RecMsg( int nMsgID,void* pInfo,int nSize )
{
	switch (nMsgID)
	{
	case enMsg_ServerSelect:
		{
			tagServerSelect * pCmd = (tagServerSelect *)pInfo;

			//����Ϣ�����ķ�����
			tagServerSelect Info;
			Info.nServerID = pCmd->nServerID;

			SendMsg(enMsg_ServerSelect,&Info,sizeof(Info));

			//�Ƴ�����
			removeFromParentAndCleanup(true);

		}break;
	}
}



