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
		//初始化父类
		CC_BREAK_IF(CCLayerColor::initWithColor(ccc4(0,0,0,255))==false);

		//独占触屏
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -10000, true);

		/************************************************************************/
		/*				背景图片                                                                     */
		/************************************************************************/
		//创建人物图片
		CCSprite * pPersonBg = CCSprite::create("ui/loading_bg.png");
		CC_BREAK_IF(pPersonBg==NULL);
		pPersonBg->setPosition(SCREEN_CENTER);
		addChild(pPersonBg,enZOrderBack);

		//创建logo图片
		CCSprite * pLogo = CCSprite::create("ui/logo_bg.png");
		CC_BREAK_IF(pLogo==NULL);
		pLogo->setScale(0.95f);
		pLogo->setPosition(ccp(SCREEN_WIDTH*0.5f,410));
		addChild(pLogo,enZOrderBack);

		
		/************************************************************************/
		/*				进度条                                                                     */
		/************************************************************************/
		//创建加载进度条背景
		CCSprite * pProBg = CCSprite::create("ui/loading_progress_bg.png");
		CC_BREAK_IF(pProBg==NULL);
		pProBg->setPosition(ccp(SCREEN_WIDTH*0.5f,100));
		addChild(pProBg,enZOrderMid);

		//创建加载进度条
		CCProgressTimer *pProLoadRec = CCProgressTimer::create(CCSprite::create("ui/loading_progress_bar.png"));
		CC_BREAK_IF(pProLoadRec==NULL);
		pProLoadRec->setPosition(pProBg->getPosition());
		pProLoadRec->setType(kCCProgressTimerTypeBar);	//设置进度条类型
		pProLoadRec->setMidpoint(ccp(0,0));				//靠左
		pProLoadRec->setBarChangeRate(ccp(1, 0));		//Y轴不缩放
		pProLoadRec->setPercentage(0.f);
		addChild(pProLoadRec,enZOrderMid+1,enTagPro);

		//创建光点
		CCSprite * pLight = CCSprite::create("ui/loading_progress_light.png");
		CC_BREAK_IF(pLight==NULL);
		addChild(pLight,enZOrderFront,enTagLight);
		pLight->setPosition( ccp( pProLoadRec->getPositionX()-pProLoadRec->getContentSize().width*0.5f, pProLoadRec->getPositionY() ) );


		/************************************************************************/
		/*					label                                                                    */
		/************************************************************************/
		CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/loading_xml.xml");
		CC_BREAK_IF(pDicLang==NULL);

		//加载信息label
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
		//同步进度条
		CCProgressTimer *pProLoadRec = dynamic_cast<CCProgressTimer *>(getChildByTag(enTagPro));
		if (pProLoadRec!=NULL)
			pProLoadRec->setPercentage(fRate*100);
	
		//同步光点
		CCSprite * pLight = dynamic_cast<CCSprite *>(getChildByTag(enTagLight));
		if (pLight!=NULL)
			pLight->setPositionX( pProLoadRec->getPositionX() + pProLoadRec->getContentSize().width*(fRate-0.5f) );

		//若加载完成
		if (fRate >= 1)
		{
			CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/loading_xml.xml");
			CC_BREAK_IF(pDicLang==NULL);

			//得到加载信息label
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
	//取消独占
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	CCLayerColor::onExit();
}