#include "NFServerChangeScrollLayer.h"

#define _ONE_PAGE_SERVER_NUM_	12		//每一页所拥有的服务器的个数
#define _X_START_POS_	78				//x轴初始坐标
#define _PAGE_WIDTH_	564				//页面宽度
#define _PAGE_HEIGHT_	285				//页面高度

/************************************************************************/
/*				滑动层                                                  */
/************************************************************************/
CNFServerChangeScrollLayer* CNFServerChangeScrollLayer::CreateLayer(int nItemNum)
{	
	CNFServerChangeScrollLayer *pRet = new CNFServerChangeScrollLayer();
	if (pRet && pRet->InitLayer(nItemNum))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CNFServerChangeScrollLayer::InitLayer(int nItemNum)
{	
	do 
	{
		//初始化父类
		CC_BREAK_IF(CCLayerColor::initWithColor(ccc4(0,0,0,0))==false);

		//独占触屏注册值小于-128才能屏蔽Menu(Menu默认是-128)
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);

		//初始x轴坐标
		m_fStartNodePosX = _X_START_POS_ + _NF_SCREEN_WIDTH_DIS_;

;		//页面数量
		int nPageNum = nItemNum%_ONE_PAGE_SERVER_NUM_ > 0 ? nItemNum/_ONE_PAGE_SERVER_NUM_+1 : nItemNum/_ONE_PAGE_SERVER_NUM_;

		//每一页宽度
		m_fPageWidth = _PAGE_WIDTH_;

		//页面高度
		float	fPageHeight = _PAGE_HEIGHT_;

		//x轴最小坐标
		m_fEndLayerPos = m_fPageWidth*(1-nPageNum);

		//x，y轴之间的间隔
		float fItemDisX = 11;
		float fItemDisY = 20;


		/************************************************************************/
		/*			创建按钮                                                                     */
		/************************************************************************/
		CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/serverselect_xml.xml");
		CC_BREAK_IF(pDicLang==NULL);

		CCMenu * pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(CCPointZero);
		addChild(pMenu,enZOrderFront,enTagMenu);

		//创建数组
		CCObject *pObj = NULL;
		for (int k=1;k<=nItemNum;k++)
		{
			int i = k%_ONE_PAGE_SERVER_NUM_ > 0 ? k%_ONE_PAGE_SERVER_NUM_ : _ONE_PAGE_SERVER_NUM_;			//页面内ITem的ID：1~12
			int j = k%_ONE_PAGE_SERVER_NUM_ > 0 ? k/_ONE_PAGE_SERVER_NUM_+1 : k/_ONE_PAGE_SERVER_NUM_;		//当前页面ID

			int nCrossNum = i%3 == 0 ? 3 : i%3;				//横排
			int nVerticalNum = i%3 > 0 ? i/3+1 : i/3;		//竖排

			//创建按钮
			CCMenuItemSprite * pBtn = CCMenuItemSprite::create(
				CCSprite::create("ui/btn_enter_n.png"),
				CCSprite::create("ui/btn_enter_p.png"),
				this,
				menu_selector(CNFServerChangeScrollLayer::OnBtnCallBack));
			CC_BREAK_IF(pBtn==NULL);
			pBtn->setPosition(ccp( fItemDisX+ (nCrossNum-1)*(pBtn->getContentSize().width+fItemDisX) + m_fStartNodePosX + pBtn->getContentSize().width*0.5f + (j-1)*m_fPageWidth , fPageHeight-nVerticalNum*(pBtn->getContentSize().height+fItemDisY) + fItemDisY ));
			pMenu->addChild(pBtn,enZOrderFront,enTagBtn+k);

			//服务器名称label
			CCString *pStrServerName = dynamic_cast<CCString*>(pDicLang->objectForKey("server_name"));
			char szName[NAME_LEN] = {0};
			sprintf(szName,pStrServerName->m_sString.c_str(),k);
			CCLabelTTF * pLabelServerName = CCLabelTTF::create(szName,"Arial",20);
			CC_BREAK_IF(pLabelServerName==NULL);
			pLabelServerName->setPosition(pBtn->getPosition());
			addChild(pLabelServerName,enZOrderFront);
			
		}


		//显示区域
		m_DisPlayRect = CCRectMake(m_fStartNodePosX ,10 ,m_fPageWidth,fPageHeight);

		m_bTouching = false;
		m_bIsTouchBtn = false;

		return true;
	} while (false);
	CCLog("Fun CQXPVEItemScrollLayer::initWithNodes Error!");
	return false;

}

//触摸：开始
bool CNFServerChangeScrollLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint ptTouchPoint =pTouch->getLocation();

	//设置初始触摸点
	m_fTouchPassPosX = ptTouchPoint.x;
	m_fTouchBeginPosX = ptTouchPoint.x;

	//判断是否触摸到裁切区域内
	if(m_DisPlayRect.containsPoint(ptTouchPoint))
	{
		m_bTouching = true;

		//判断是否触摸到按钮
		CCMenu *pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagMenu));
		if(pMenu!=NULL)
		{
			CCArray *pChildren = pMenu->getChildren();
			if(pChildren!=NULL)
			{
				CCObject *pItem=NULL;
				CCARRAY_FOREACH(pChildren,pItem)
				{
					CCMenuItemSprite *pBtn= dynamic_cast<CCMenuItemSprite*>(pItem);
					if(pBtn!=NULL)
					{
						CCPoint local = pBtn->convertToNodeSpace(pTouch->getLocation());
						CCRect r = pBtn->rect();
						r.origin = CCPointZero;
						if (r.containsPoint(local))
						{
							pMenu->ccTouchBegan(pTouch,pEvent);
							m_bIsTouchBtn=true;
						}
					}
				}
			}
		}


		return true;	
	}

	return false;
}

//触摸：移动
void CNFServerChangeScrollLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{	
	if(m_bTouching==false)return;

	CCPoint ptTouchPoint =pTouch->getLocation();
	CCPoint pNewPos=getPosition();

	//移动层
	pNewPos.x+=ptTouchPoint.x-m_fTouchPassPosX;
	this->setPosition(pNewPos);

	//重置初始点
	m_fTouchPassPosX=ptTouchPoint.x;

	//若触摸到按钮
	if (m_bIsTouchBtn==true)
	{
		CCMenu *pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagMenu));
		if (pMenu!=NULL)
		{
			pMenu->ccTouchCancelled(pTouch,pEvent);
			m_bIsTouchBtn = false;
		}
	}
}

//触摸：结束
void CNFServerChangeScrollLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_bTouching==false)return;

	m_bTouching = false;

	CCPoint ptTouchPoint =pTouch->getLocation();
	CCPoint ptMyPos = getPosition();


	//判断移到哪一页
	float fDisX = ptTouchPoint.x - m_fTouchBeginPosX;
	
	//判定滑动
	if ( _NF_ABS(fDisX)>= 20)
	{
		//向左滑动
		if (fDisX < 0)
		{
			//若未过界
			if (getPositionX() > m_fEndLayerPos)
			{
				int fLen = (int)getPositionX()%(int)m_fPageWidth;
				ptMyPos.x = (int)( getPositionX() - fLen - m_fPageWidth);
				CCAction *pMoveTo=CCMoveTo::create(0.3f,ptMyPos);
				runAction(pMoveTo);
			}
		}
		//向右滑
		if (fDisX > 0)
		{
			//若未过界
			if (getPositionX() < 0)
			{
				int fLen = (int)getPositionX()%(int)m_fPageWidth;
				ptMyPos.x = (int)( getPositionX() - fLen);
				CCAction *pMoveTo=CCMoveTo::create(0.3f,ptMyPos);
				runAction(pMoveTo);
			}
		}
	}

	//判断是否越界
	if (getPositionX()>0)
	{	
		ptMyPos.x = 0;
		CCAction *pMoveTo=CCMoveTo::create(0.3f,ptMyPos);
		runAction(pMoveTo);
	}
	else if (getPositionX()<m_fEndLayerPos)
	{
		ptMyPos.x = m_fEndLayerPos<0?m_fEndLayerPos:0;
		CCAction *pMoveTo=CCMoveTo::create(0.3f,ptMyPos);
		runAction(pMoveTo);
	}

	//滑动层，当前坐标
	CCPoint LayerPos;
	LayerPos.x = getPositionX();
	LayerPos.y = getPositionY();

	//若触摸到按钮
	if (m_bIsTouchBtn==true)
	{
		CCMenu *pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagMenu));
		if (pMenu!=NULL)
		{
			pMenu->ccTouchEnded(pTouch,pEvent);
		}
		m_bIsTouchBtn = false;
	}

}

void CNFServerChangeScrollLayer::ccTouchCancelled( CCTouch *pTouch, CCEvent *pEvent )
{
	m_bTouching = false;
}

//裁切
void CNFServerChangeScrollLayer::visit()
{
	kmGLPushMatrix();

	if (m_pGrid && m_pGrid->isActive())
	{
		m_pGrid->beforeDraw();
		this->transformAncestors();
	}
	this->transform();

	glEnable(GL_SCISSOR_TEST);

	CCSize size=CCEGLView::sharedOpenGLView()->getFrameSize();

	float fScaleX=size.width/SCREEN_WIDTH;
	float fScaleY=size.height/SCREEN_HEIGHT;

	glScissor(m_DisPlayRect.origin.x*fScaleX,m_DisPlayRect.origin.y*fScaleY,m_DisPlayRect.size.width*fScaleX,m_DisPlayRect.size.height*fScaleY);

	if (m_pChildren)
	{
		for (unsigned int i=0;i<m_pChildren->data->num;i++)
		{
			CCNode* pNode=(CCNode*)m_pChildren->data->arr[i];
			if (pNode->getZOrder()<0)
			{
				pNode->visit();
			}
			else
			{
				break;
			}
		}
		draw();
		for (unsigned int i=0;i<m_pChildren->data->num;i++)
		{
			CCNode* pNode=(CCNode*)m_pChildren->data->arr[i];
			pNode->visit();
		}
	}
	else
	{
		draw();
	}
	glDisable(GL_SCISSOR_TEST);
	if (m_pGrid &&m_pGrid->isActive())
	{
		m_pGrid->afterDraw(this);
	}
	kmGLPopMatrix();
}

//按钮回调
void CNFServerChangeScrollLayer::OnBtnCallBack( CCObject * pObj )
{
	do 
	{
		int nTag = (dynamic_cast<CCMenuItemSprite *>(pObj))->getTag();

		//发消息：更改服务器
		tagServerSelect Info;
		Info.nServerID = nTag-100;

		//发消息，切换服务器
		SendMsg(enMsg_ServerSelect,&Info,sizeof(Info));

		//移除自身
		removeFromParentAndCleanup(true);
		
		return ;
	} while (false);
	CCLog("Fun CNFServerSelectScrollLayer::OnBtnCallBack Error!");
}

void CNFServerChangeScrollLayer::onExit()
{
	//取消独占
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	CCLayer::onExit();
}

void CNFServerChangeScrollLayer::SetLeftMove()
{
	//若当前未触摸
	if (m_bTouching==false)
	{
		//若未过界
		if (getPositionX() > m_fEndLayerPos)
		{
			CCPoint ptMyPos;
			ptMyPos.x = (int)( getPositionX() - m_fPageWidth);
			CCAction *pMoveTo=CCMoveTo::create(0.3f,ptMyPos);
			runAction(pMoveTo);
		}
	}
}

void CNFServerChangeScrollLayer::SetRightMove()
{
	//若当前未触摸
	if (m_bTouching==false)
	{
		//若未过界
		if (getPositionX() < 0)
		{
			CCPoint ptMyPos;
			ptMyPos.x = (int)( getPositionX() + m_fPageWidth);
			CCAction *pMoveTo=CCMoveTo::create(0.3f,ptMyPos);
			runAction(pMoveTo);
		}
	}
}





