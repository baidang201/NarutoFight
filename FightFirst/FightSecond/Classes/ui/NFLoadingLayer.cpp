#include "NFLoadingLayer.h"


CNFLoadingLayer * CNFLoadingLayer::CreateLayer()
{
	CNFLoadingLayer *pRet = new CNFLoadingLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CCLog("Fun CNFLoadingLayer::CreateLayer Error!");
	delete pRet; 
	pRet = NULL; 
	return NULL;
}

bool CNFLoadingLayer::init()
{
	do 
	{
		//��ʼ������
		CC_BREAK_IF(CCLayerColor::initWithColor(ccc4(0,0,0,255))==false);

		//��ռ����
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);

		/************************************************************************/
		/*				����ͼƬ                                                                     */
		/************************************************************************/
		//��������ͼƬ
		CCSprite * pPersonBg = CCSprite::create("ui/loading_bg.png");
		CC_BREAK_IF(pPersonBg==NULL);
		pPersonBg->setPosition(SCREEN_CENTER);
		addChild(pPersonBg,enZOrderBack);

		//����logoͼƬ
		CCSprite * pLogo = CCSprite::create("ui/logo_bg.png");
		CC_BREAK_IF(pLogo==NULL);
		pLogo->setScale(0.95f);
		pLogo->setPosition(ccp(SCREEN_WIDTH*0.5f,410));
		addChild(pLogo,enZOrderBack);

		
		/************************************************************************/
		/*				������                                                                     */
		/************************************************************************/
		//�������ؽ���������
		CCSprite * pProBg = CCSprite::create("ui/loading_progress_bg.png");
		CC_BREAK_IF(pProBg==NULL);
		pProBg->setPosition(ccp(SCREEN_WIDTH*0.5f,100));
		addChild(pProBg,enZOrderMid);

		//�������ؽ�����
		CCProgressTimer *pProLoadRec = CCProgressTimer::create(CCSprite::create("ui/loading_progress_bar.png"));
		CC_BREAK_IF(pProLoadRec==NULL);
		pProLoadRec->setPosition(pProBg->getPosition());
		pProLoadRec->setType(kCCProgressTimerTypeBar);	//���ý���������
		pProLoadRec->setMidpoint(ccp(0,0));				//����
		pProLoadRec->setBarChangeRate(ccp(1, 0));		//Y�᲻����
		pProLoadRec->setPercentage(0.f);
		addChild(pProLoadRec,enZOrderMid+1,enTagPro);

		//�������
		CCSprite * pLight = CCSprite::create("ui/loading_progress_light.png");
		CC_BREAK_IF(pLight==NULL);
		addChild(pLight,enZOrderFront,enTagLight);
		pLight->setPosition( ccp( pProLoadRec->getPositionX()-pProLoadRec->getContentSize().width*0.5f, pProLoadRec->getPositionY() ) );


		/************************************************************************/
		/*					label                                                                    */
		/************************************************************************/
		CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/loading_xml.xml");
		CC_BREAK_IF(pDicLang==NULL);

		//������Ϣlabel
		CCString *pStrLoading = dynamic_cast<CCString*>(pDicLang->objectForKey("loading_now"));
		CCLabelTTF * pLabelLoading = CCLabelTTF::create(pStrLoading->m_sString.c_str(),"Arial",10);
		CC_BREAK_IF(pLabelLoading==NULL);
		pLabelLoading->setPosition(pProLoadRec->getPosition());
		addChild(pLabelLoading,enZOrderMid+2,enTagLabel);

		return true;
	} while (false);
	CCLog("Fun CNFLoadingLayer::init Error!");
	return false;
}

void CNFLoadingLayer::SetPro( float fRate )
{
	do 
	{
		//ͬ��������
		CCProgressTimer *pProLoadRec = dynamic_cast<CCProgressTimer *>(getChildByTag(enTagPro));
		if (pProLoadRec!=NULL)
			pProLoadRec->setPercentage(fRate*100);
	
		//ͬ�����
		CCSprite * pLight = dynamic_cast<CCSprite *>(getChildByTag(enTagLight));
		if (pLight!=NULL)
			pLight->setPositionX( pProLoadRec->getPositionX() + pProLoadRec->getContentSize().width*(fRate-0.5f) );

		//���������
		if (fRate >= 1)
		{
			CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/loading_xml.xml");
			CC_BREAK_IF(pDicLang==NULL);

			//�õ�������Ϣlabel
			CCString *pStrLoading = dynamic_cast<CCString*>(pDicLang->objectForKey("loading_end"));
			CCLabelTTF * pLabelLoading = dynamic_cast<CCLabelTTF *>(getChildByTag(enTagLabel));
			CC_BREAK_IF(pLabelLoading==NULL);
			pLabelLoading->setString(pStrLoading->m_sString.c_str());
		}

		return ;
	} while (false);
	CCLog("Fun CNFLoadingLayer::SetPro Error!");
}

bool CNFLoadingLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	return true;
}

void CNFLoadingLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	return ;
}

void CNFLoadingLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	return ;
}

void CNFLoadingLayer::onExit()
{
	//ȡ����ռ
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	CCLayerColor::onExit();
}