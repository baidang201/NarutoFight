#include "NFServerChangeScrollLayer.h"

#define _ONE_PAGE_SERVER_NUM_	12		//ÿһҳ��ӵ�еķ������ĸ���
#define _X_START_POS_	78				//x���ʼ����
#define _PAGE_WIDTH_	564				//ҳ����
#define _PAGE_HEIGHT_	285				//ҳ��߶�

/************************************************************************/
/*				������                                                  */
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
		//��ʼ������
		CC_BREAK_IF(CCLayerColor::initWithColor(ccc4(0,0,0,0))==false);

		//��ռ����ע��ֵС��-128��������Menu(MenuĬ����-128)
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);

		//��ʼx������
		m_fStartNodePosX = _X_START_POS_ + _NF_SCREEN_WIDTH_DIS_;

;		//ҳ������
		int nPageNum = nItemNum%_ONE_PAGE_SERVER_NUM_ > 0 ? nItemNum/_ONE_PAGE_SERVER_NUM_+1 : nItemNum/_ONE_PAGE_SERVER_NUM_;

		//ÿһҳ���
		m_fPageWidth = _PAGE_WIDTH_;

		//ҳ��߶�
		float	fPageHeight = _PAGE_HEIGHT_;

		//x����С����
		m_fEndLayerPos = m_fPageWidth*(1-nPageNum);

		//x��y��֮��ļ��
		float fItemDisX = 11;
		float fItemDisY = 20;


		/************************************************************************/
		/*			������ť                                                                     */
		/************************************************************************/
		CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/serverselect_xml.xml");
		CC_BREAK_IF(pDicLang==NULL);

		CCMenu * pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(CCPointZero);
		addChild(pMenu,enZOrderFront,enTagMenu);

		//��������
		CCObject *pObj = NULL;
		for (int k=1;k<=nItemNum;k++)
		{
			int i = k%_ONE_PAGE_SERVER_NUM_ > 0 ? k%_ONE_PAGE_SERVER_NUM_ : _ONE_PAGE_SERVER_NUM_;			//ҳ����ITem��ID��1~12
			int j = k%_ONE_PAGE_SERVER_NUM_ > 0 ? k/_ONE_PAGE_SERVER_NUM_+1 : k/_ONE_PAGE_SERVER_NUM_;		//��ǰҳ��ID

			int nCrossNum = i%3 == 0 ? 3 : i%3;				//����
			int nVerticalNum = i%3 > 0 ? i/3+1 : i/3;		//����

			//������ť
			CCMenuItemSprite * pBtn = CCMenuItemSprite::create(
				CCSprite::create("ui/btn_enter_n.png"),
				CCSprite::create("ui/btn_enter_p.png"),
				this,
				menu_selector(CNFServerChangeScrollLayer::OnBtnCallBack));
			CC_BREAK_IF(pBtn==NULL);
			pBtn->setPosition(ccp( fItemDisX+ (nCrossNum-1)*(pBtn->getContentSize().width+fItemDisX) + m_fStartNodePosX + pBtn->getContentSize().width*0.5f + (j-1)*m_fPageWidth , fPageHeight-nVerticalNum*(pBtn->getContentSize().height+fItemDisY) + fItemDisY ));
			pMenu->addChild(pBtn,enZOrderFront,enTagBtn+k);

			//����������label
			CCString *pStrServerName = dynamic_cast<CCString*>(pDicLang->objectForKey("server_name"));
			char szName[NAME_LEN] = {0};
			sprintf(szName,pStrServerName->m_sString.c_str(),k);
			CCLabelTTF * pLabelServerName = CCLabelTTF::create(szName,"Arial",20);
			CC_BREAK_IF(pLabelServerName==NULL);
			pLabelServerName->setPosition(pBtn->getPosition());
			addChild(pLabelServerName,enZOrderFront);
			
		}


		//��ʾ����
		m_DisPlayRect = CCRectMake(m_fStartNodePosX ,10 ,m_fPageWidth,fPageHeight);

		m_bTouching = false;
		m_bIsTouchBtn = false;

		return true;
	} while (false);
	CCLog("Fun CQXPVEItemScrollLayer::initWithNodes Error!");
	return false;

}

//��������ʼ
bool CNFServerChangeScrollLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	CCPoint ptTouchPoint =pTouch->getLocation();

	//���ó�ʼ������
	m_fTouchPassPosX = ptTouchPoint.x;
	m_fTouchBeginPosX = ptTouchPoint.x;

	//�ж��Ƿ���������������
	if(m_DisPlayRect.containsPoint(ptTouchPoint))
	{
		m_bTouching = true;

		//�ж��Ƿ�������ť
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

//�������ƶ�
void CNFServerChangeScrollLayer::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{	
	if(m_bTouching==false)return;

	CCPoint ptTouchPoint =pTouch->getLocation();
	CCPoint pNewPos=getPosition();

	//�ƶ���
	pNewPos.x+=ptTouchPoint.x-m_fTouchPassPosX;
	this->setPosition(pNewPos);

	//���ó�ʼ��
	m_fTouchPassPosX=ptTouchPoint.x;

	//����������ť
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

//����������
void CNFServerChangeScrollLayer::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
	if(m_bTouching==false)return;

	m_bTouching = false;

	CCPoint ptTouchPoint =pTouch->getLocation();
	CCPoint ptMyPos = getPosition();


	//�ж��Ƶ���һҳ
	float fDisX = ptTouchPoint.x - m_fTouchBeginPosX;
	
	//�ж�����
	if ( _NF_ABS(fDisX)>= 20)
	{
		//���󻬶�
		if (fDisX < 0)
		{
			//��δ����
			if (getPositionX() > m_fEndLayerPos)
			{
				int fLen = (int)getPositionX()%(int)m_fPageWidth;
				ptMyPos.x = (int)( getPositionX() - fLen - m_fPageWidth);
				CCAction *pMoveTo=CCMoveTo::create(0.3f,ptMyPos);
				runAction(pMoveTo);
			}
		}
		//���һ�
		if (fDisX > 0)
		{
			//��δ����
			if (getPositionX() < 0)
			{
				int fLen = (int)getPositionX()%(int)m_fPageWidth;
				ptMyPos.x = (int)( getPositionX() - fLen);
				CCAction *pMoveTo=CCMoveTo::create(0.3f,ptMyPos);
				runAction(pMoveTo);
			}
		}
	}

	//�ж��Ƿ�Խ��
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

	//�����㣬��ǰ����
	CCPoint LayerPos;
	LayerPos.x = getPositionX();
	LayerPos.y = getPositionY();

	//����������ť
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

//����
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

//��ť�ص�
void CNFServerChangeScrollLayer::OnBtnCallBack( CCObject * pObj )
{
	do 
	{
		int nTag = (dynamic_cast<CCMenuItemSprite *>(pObj))->getTag();

		//����Ϣ�����ķ�����
		tagServerSelect Info;
		Info.nServerID = nTag-100;

		//����Ϣ���л�������
		SendMsg(enMsg_ServerSelect,&Info,sizeof(Info));

		//�Ƴ�����
		removeFromParentAndCleanup(true);
		
		return ;
	} while (false);
	CCLog("Fun CNFServerSelectScrollLayer::OnBtnCallBack Error!");
}

void CNFServerChangeScrollLayer::onExit()
{
	//ȡ����ռ
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	CCLayer::onExit();
}

void CNFServerChangeScrollLayer::SetLeftMove()
{
	//����ǰδ����
	if (m_bTouching==false)
	{
		//��δ����
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
	//����ǰδ����
	if (m_bTouching==false)
	{
		//��δ����
		if (getPositionX() < 0)
		{
			CCPoint ptMyPos;
			ptMyPos.x = (int)( getPositionX() + m_fPageWidth);
			CCAction *pMoveTo=CCMoveTo::create(0.3f,ptMyPos);
			runAction(pMoveTo);
		}
	}
}





