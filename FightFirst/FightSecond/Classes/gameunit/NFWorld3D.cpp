#include "NFWorld3D.h"

#define _RATE_X_	0.1
#define _MIN_Z_SPEED_	-15

/************************************************************************/
/*				世界类                                                                     */
/************************************************************************/
CNFWorld3DLayer* CNFWorld3DLayer::CreateLayer(float rate,tagSize3D sz3DWorld)
{
	do 
	{
		 CNFWorld3DLayer* pLayer = new CNFWorld3DLayer();
		 if (pLayer!=NULL && pLayer->init(rate,sz3DWorld)==true)
		 {
			 pLayer->autorelease();
			 return pLayer;
		 }
		 
		 delete pLayer; 
		 pLayer = NULL; 
		 return NULL; 
	} while (false);
	CCLog("Fun CLTShiJieLayer::CreateLayer Error!");
	return NULL;
}

bool CNFWorld3DLayer::init(float rate,tagSize3D sz3DWorld)
{
	do 
	{
		//初始化父类
		CC_BREAK_IF(CCLayer::init()==false);

		CCAssert(sz3DWorld.fHeight>0&&sz3DWorld.fLength>0&&sz3DWorld.fWidth>0,"Error:Wrong 3D Size!");

		CCAssert(rate>=0&&rate<=1,"Error:Rate is out range!");
		
		m_fRate = rate;

		m_bIsLensFeatureOver = false;

		//将镜头默认设置到屏幕中心(大小默认为(100,100,140))
		m_ShotCube.ptPos3D.fX = CCDirector::sharedDirector()->getWinSize().width/2;
		m_ShotCube.ptPos3D.fY = CCDirector::sharedDirector()->getWinSize().height/3/m_fRate;
		m_ShotCube.ptPos3D.fZ = 0;
		m_ShotCube.szCube.fLength = 100;
		m_ShotCube.szCube.fWidth = 100;
		m_ShotCube.szCube.fHeight = 140;

		m_ImpactSize=sz3DWorld;

		m_pProtagonist = NULL;

		//调试阶段默认为true
		m_bIsBorderVisible = true;

		setAnchorPoint(ccp(0,0));

		//创建特效层
		CNFEffectLayer * pEffectLayer = CNFEffectLayer::CreateLayer();
		CC_BREAK_IF(pEffectLayer==NULL);
		pEffectLayer->setContentSize(CCSizeMake(sz3DWorld.fLength,CCDirector::sharedDirector()->getWinSize().height));
		addChild(pEffectLayer,-1,enTagEffectLayer);
		//pEffectLayer->setColor(ccRED);
		//pEffectLayer->setOpacity(150.f);

		return true;
	} while (false);
	CCLog("Fun CLTShiJieLayer::init Error!");
	return false;
}

void CNFWorld3DLayer::draw()
{
	CCLayer::draw();

	//不允许子元素移动出世界3D盒子外
	CCObject *pItem = NULL;
	tagSize3D szNew;
	CCARRAY_FOREACH(getChildren(),pItem)
	{
		CNFUnit3D *pUnitItem = dynamic_cast<CNFUnit3D*>(pItem);
		if(pUnitItem!=NULL&&pUnitItem->GetOnOutside()==false)
		{
			//体积校验 不允许 子模型的体积超过世界模型
			if(pUnitItem->GetCurrentCube()->szCube.fLength>m_ImpactSize.fLength)
			{
				szNew.fLength = m_ImpactSize.fLength;
				szNew.fWidth = pUnitItem->GetCurrentCube()->szCube.fWidth;
				szNew.fHeight = pUnitItem->GetCurrentCube()->szCube.fHeight;
				pUnitItem->setSize3D(szNew);
			}
			if(pUnitItem->GetCurrentCube()->szCube.fWidth>m_ImpactSize.fWidth)
			{
				szNew.fLength = pUnitItem->GetCurrentCube()->szCube.fLength;
				szNew.fWidth = m_ImpactSize.fWidth;
				szNew.fHeight = pUnitItem->GetCurrentCube()->szCube.fHeight;
				pUnitItem->setSize3D(szNew);
			}
			if(pUnitItem->GetCurrentCube()->szCube.fHeight>m_ImpactSize.fHeight)
			{
				szNew.fLength = pUnitItem->GetCurrentCube()->szCube.fLength;
				szNew.fWidth = pUnitItem->GetCurrentCube()->szCube.fWidth;
				szNew.fHeight = m_ImpactSize.fHeight;
				pUnitItem->setSize3D(szNew);
			}
			tagPosition3D ptPos3D;
			//边界校验X
			if((pUnitItem->GetCurrentCube()->ptPos3D.fX - pUnitItem->GetCurrentCube()->szCube.fLength/2)<0)
			{
				ptPos3D.fX = pUnitItem->GetCurrentCube()->szCube.fLength/2;
				ptPos3D.fY = pUnitItem->GetCurrentCube()->ptPos3D.fY;
				ptPos3D.fZ = 0;
				pUnitItem->setPosition3D(ptPos3D);
			}
			if((pUnitItem->GetCurrentCube()->ptPos3D.fX + pUnitItem->GetCurrentCube()->szCube.fLength/2)>m_ImpactSize.fLength)
			{
				ptPos3D.fX = m_ImpactSize.fLength - pUnitItem->GetCurrentCube()->szCube.fLength/2;
				ptPos3D.fY = pUnitItem->GetCurrentCube()->ptPos3D.fY;
				ptPos3D.fZ = 0;
				pUnitItem->setPosition3D(ptPos3D);
			}
			
			//边界校验Y
			if((pUnitItem->GetCurrentCube()->ptPos3D.fY - pUnitItem->GetCurrentCube()->szCube.fWidth/2)<0)
			{
				ptPos3D.fX = pUnitItem->GetCurrentCube()->ptPos3D.fX;
				ptPos3D.fY = pUnitItem->GetCurrentCube()->szCube.fWidth/2;
				ptPos3D.fZ = 0;
				pUnitItem->setPosition3D(ptPos3D);
			}
			if((pUnitItem->GetCurrentCube()->ptPos3D.fY + pUnitItem->GetCurrentCube()->szCube.fWidth/2)>m_ImpactSize.fWidth)
			{
				ptPos3D.fX = pUnitItem->GetCurrentCube()->ptPos3D.fX;
				ptPos3D.fY = m_ImpactSize.fWidth - pUnitItem->GetCurrentCube()->szCube.fWidth/2;
				ptPos3D.fZ = 0;
				pUnitItem->setPosition3D(ptPos3D);
			}
			////边界校验Z(只校验一个方向)
			//if((pUnitItem->GetCurrentCube()->ptPos3D.fZ + pUnitItem->GetCurrentCube()->szCube.fHeight)>m_ImpactSize.fHeight)
			//{
			//	ptPos3D.fX = pUnitItem->GetCurrentCube()->ptPos3D.fX;
			//	ptPos3D.fY = pUnitItem->GetCurrentCube()->ptPos3D.fY;
			//	ptPos3D.fZ = m_ImpactSize.fHeight - pUnitItem->GetCurrentCube()->szCube.fHeight;
			//	pUnitItem->setPosition3D(ptPos3D);
			//}
		}
	}

	//镜头最小不得小于主角碰撞空间
	if(m_pProtagonist!=NULL)
	{
		if(m_ShotCube.szCube.fLength < m_pProtagonist->GetCurrentCube()->szCube.fLength)
		{
			m_ShotCube.szCube.fLength = m_pProtagonist->GetCurrentCube()->szCube.fLength;
		}
		if(m_ShotCube.szCube.fWidth < m_pProtagonist->GetCurrentCube()->szCube.fWidth)
		{
			m_ShotCube.szCube.fWidth = m_pProtagonist->GetCurrentCube()->szCube.fWidth;
		}
		if(m_ShotCube.szCube.fHeight < m_pProtagonist->GetCurrentCube()->szCube.fHeight)
		{
			m_ShotCube.szCube.fHeight = m_pProtagonist->GetCurrentCube()->szCube.fHeight;
		}
	}

	
	//屏幕推进(尽量保持镜头在屏幕中央)
	if(m_pProtagonist!=NULL)
	{
		//X方向移动
		float  temp = getPositionX();
		if(getPositionX()<0)
		{
			if((m_pProtagonist->GetCurrentCube()->ptPos3D.fX - m_pProtagonist->GetCurrentCube()->szCube.fLength/2)<(m_ShotCube.ptPos3D.fX - m_ShotCube.szCube.fLength/2))
			{
				float fDisX = (m_ShotCube.ptPos3D.fX - m_ShotCube.szCube.fLength/2) - (m_pProtagonist->GetCurrentCube()->ptPos3D.fX - m_pProtagonist->GetCurrentCube()->szCube.fLength/2);
				m_ShotCube.ptPos3D.fX -= fDisX;
				setPositionX(getPositionX() + fDisX);
			}
		}

		if(getPositionX()>(CCDirector::sharedDirector()->getWinSize().width - m_ImpactSize.fLength))
		{
			if(m_pProtagonist->GetCurrentCube()->ptPos3D.fX < m_ImpactSize.fLength - CCDirector::sharedDirector()->getWinSize().width/2  + m_ShotCube.szCube.fLength/2 - m_pProtagonist->GetCurrentCube()->szCube.fLength/2){
				if((m_pProtagonist->GetCurrentCube()->ptPos3D.fX + m_pProtagonist->GetCurrentCube()->szCube.fLength/2)>m_ShotCube.ptPos3D.fX + m_ShotCube.szCube.fLength/2 )
				{
					float fDisX = (m_pProtagonist->GetCurrentCube()->ptPos3D.fX + m_pProtagonist->GetCurrentCube()->szCube.fLength/2) - (m_ShotCube.ptPos3D.fX + m_ShotCube.szCube.fLength/2);
					m_ShotCube.ptPos3D.fX += fDisX;
					setPositionX(getPositionX() - fDisX);
				}
			}else{
				m_ShotCube.ptPos3D.fX = m_ImpactSize.fLength - CCDirector::sharedDirector()->getWinSize().width/2;
				setPositionX(CCDirector::sharedDirector()->getWinSize().width - m_ImpactSize.fLength);
			}
			
		}

		////Y方向移动
		//if((m_pProtagonist->GetCurrentCube()->ptPos3D.fY - m_pProtagonist->GetCurrentCube()->szCube.fWidth/2)<m_ShotCube.ptPos3D.fW - m_ShotCube.szCube.fWidth/2)
		//{
		//	float fDisY = (m_ShotCube.ptPos3D.fW - m_ShotCube.szCube.fWidth/2) - (m_pProtagonist->GetCurrentCube()->ptPos3D.fY - m_pProtagonist->GetCurrentCube()->szCube.fWidth/2);
		//	m_ShotCube.ptPos3D.fW -= fDisY;
		//	setPositionY(getPositionY() + fDisY*m_fRate);
		//}
		//if((m_pProtagonist->GetCurrentCube()->ptPos3D.fY + m_pProtagonist->GetCurrentCube()->szCube.fWidth/2)>m_ShotCube.ptPos3D.fW + m_ShotCube.szCube.fWidth/2)
		//{
		//	float fDisY = (m_pProtagonist->GetCurrentCube()->ptPos3D.fY + m_pProtagonist->GetCurrentCube()->szCube.fWidth/2) - (m_ShotCube.ptPos3D.fW + m_ShotCube.szCube.fWidth/2);
		//	m_ShotCube.ptPos3D.fW += fDisY;
		//	setPositionY(getPositionY() - fDisY*m_fRate);
		//}

		////Z方向移动
		//if((m_pProtagonist->GetCurrentCube()->ptPos3D.fZ)<m_ShotCube.ptPos3D.fH){
		//	float fDisZ = (m_ShotCube.ptPos3D.fH) - (m_pProtagonist->GetCurrentCube()->ptPos3D.fZ);
		//	m_ShotCube.ptPos3D.fH -= fDisZ;
		//	setPositionY(getPositionY() + fDisZ);
		//}
		//if((m_pProtagonist->GetCurrentCube()->ptPos3D.fZ + m_pProtagonist->GetCurrentCube()->szCube.fHeight)>m_ShotCube.ptPos3D.fH + m_ShotCube.szCube.fHeight)
		//{
		//	float fDisZ = (m_pProtagonist->GetCurrentCube()->ptPos3D.fZ + m_pProtagonist->GetCurrentCube()->szCube.fHeight) - (m_ShotCube.ptPos3D.fH + m_ShotCube.szCube.fHeight);
		//	m_ShotCube.ptPos3D.fH += fDisZ;
		//	setPositionY(getPositionY() - fDisZ);
		//}
	}

	//绘制碰撞边线
	if(m_bIsBorderVisible)
	{
		//体积线
		CCPoint p1,p2,p3,p4,p5,p6,p7,p8;//立方体的8个顶点

		p1.x = 0;	
		p1.y = 0;
		p2.x = m_ImpactSize.fLength;
		p2.y = 0 ;
		p3.x = m_ImpactSize.fLength + 10;
		p3.y = m_ImpactSize.fWidth*m_fRate;
		p4.x = 0 + 10;
		p4.y = m_ImpactSize.fWidth*m_fRate;

		p5.x = p1.x;
		p5.y = p1.y +  m_ImpactSize.fHeight;
		p6.x = p2.x;
		p6.y = p2.y +  m_ImpactSize.fHeight;
		p7.x = p3.x;
		p7.y = p3.y +  m_ImpactSize.fHeight;
		p8.x = p4.x;
		p8.y = p4.y +  m_ImpactSize.fHeight;

		//画线
		ccDrawColor4B(0,0,255,255);

		ccDrawLine(p2,p3);
		ccDrawLine(p3,p4);
		ccDrawLine(p4,p1);

		ccDrawLine(p6,p7);
		ccDrawLine(p7,p8);
		ccDrawLine(p8,p5);

		ccDrawLine(p1,p5);
		ccDrawLine(p2,p6);

		ccDrawLine(p3,p7);
		ccDrawLine(p4,p8);

		ccDrawLine(p1,p2);
		ccDrawLine(p5,p6);


		//绘制镜头线
		p1.x = m_ShotCube.ptPos3D.fX-m_ShotCube.szCube.fLength*0.5f;	
		p1.y = m_ShotCube.ptPos3D.fY*m_fRate-m_ShotCube.szCube.fWidth*0.5f*m_fRate + m_ShotCube.ptPos3D.fZ;
		p2.x = m_ShotCube.ptPos3D.fX+m_ShotCube.szCube.fLength*0.5f;
		p2.y = m_ShotCube.ptPos3D.fY*m_fRate-m_ShotCube.szCube.fWidth*0.5f*m_fRate + m_ShotCube.ptPos3D.fZ;
		p3.x = m_ShotCube.ptPos3D.fX+m_ShotCube.szCube.fLength*0.5f + m_ShotCube.szCube.fWidth*0.5f*_RATE_X_;
		p3.y = m_ShotCube.ptPos3D.fY*m_fRate+m_ShotCube.szCube.fWidth*0.5f*m_fRate + m_ShotCube.ptPos3D.fZ;
		p4.x = m_ShotCube.ptPos3D.fX-m_ShotCube.szCube.fLength*0.5f + m_ShotCube.szCube.fWidth*0.5f*_RATE_X_;
		p4.y = m_ShotCube.ptPos3D.fY*m_fRate+m_ShotCube.szCube.fWidth*0.5f*m_fRate + m_ShotCube.ptPos3D.fZ;

		p5.x = p1.x;
		p5.y = p1.y + m_ShotCube.szCube.fHeight;
		p6.x = p2.x;
		p6.y = p2.y + m_ShotCube.szCube.fHeight;
		p7.x = p3.x;
		p7.y = p3.y + m_ShotCube.szCube.fHeight;
		p8.x = p4.x;
		p8.y = p4.y + m_ShotCube.szCube.fHeight;

		//画线
		ccDrawColor4B(0,255,0,255);

		ccDrawLine(p2,p3);
		ccDrawLine(p3,p4);
		ccDrawLine(p4,p1);

		ccDrawLine(p6,p7);
		ccDrawLine(p7,p8);
		ccDrawLine(p8,p5);

		ccDrawLine(p1,p5);
		ccDrawLine(p2,p6);

		ccDrawLine(p3,p7);
		ccDrawLine(p4,p8);

		ccDrawLine(p1,p2);
		ccDrawLine(p5,p6);
	}
}

void CNFWorld3DLayer::SetProtagonist(CNFUnit3D* pProtagonist)
{
	m_pProtagonist = pProtagonist;
	//镜头的Z坐标 与 主角保持一致
	if(m_pProtagonist!=NULL){
		m_ShotCube.ptPos3D.fZ = m_pProtagonist->GetCurrentCube()->ptPos3D.fZ;
	}
}

void CNFWorld3DLayer::WorldEffectTremble(float fDurTime,int nDegree,int nTimes)
{
	do 
	{
		CCAssert(fDurTime>0&&nDegree>0&&nTimes>0,"Error:WorldEffectTremble Call Error!");

		if(m_pProtagonist!=NULL)
		{
			int nShakeX = nDegree;	//横向震动宽度

			if(m_pProtagonist->GetCurrentCube()->ptPos3D.fX > m_ImpactSize.fLength*0.5f)
			{
				nShakeX = -nShakeX;
			}
			int nShakeY = nDegree*2;	//纵向震动幅度

			float fTimePerAction = fDurTime/(nTimes*7);
			
			//每一波震动，由5个步骤组成
			CCArray *pAcionArray = CCArray::create();
			CC_BREAK_IF(pAcionArray==NULL);
			
			for(int i=0;i<nTimes;i++)
			{
				CCActionInterval* pTremble = CCMoveBy::create(fTimePerAction*2,ccp(0,nShakeY));
				pAcionArray->addObject(pTremble);

				CCActionInterval* pTremble2 = pTremble->reverse();
				pAcionArray->addObject(pTremble2);

				CCActionInterval* pTremble3 = CCMoveBy::create(fTimePerAction,ccp(nShakeX*2,0));
				pAcionArray->addObject(pTremble3);

				CCActionInterval* pTremble4 = CCMoveBy::create(fTimePerAction,ccp(-nShakeX,0));
				pAcionArray->addObject(pTremble4);

				CCActionInterval* pTremble5 = CCMoveBy::create(fTimePerAction,ccp(-nShakeX,0));
				pAcionArray->addObject(pTremble5);
			}
			CCFiniteTimeAction* pActionShake = CCSequence::create(pAcionArray);
			this->runAction(pActionShake);

			CC_SAFE_RELEASE(pAcionArray);
		}
		return;
	} while (false);
	CCLOG(" CNFWorld3DLayer::WorldEffectTremble Run Error!");
}

void CNFWorld3DLayer::LensFeature(float fPauseTime,CNFUnit3D *pFocusOn /* = NULL */)
{
	do 
	{
		if(m_bIsLensFeatureOver==true){return;}
		m_bIsLensFeatureOver = true;

		CNFUnit3D *pFTarget = pFocusOn==NULL?m_pProtagonist:pFocusOn;
		
		CC_BREAK_IF(pFTarget==NULL);
		CC_BREAK_IF(fPauseTime<=0.f);
		
		CCActionInterval* pTremble = CCScaleTo::create(0.2f,2.f);
		CCActionInterval* pTremble2 = CCDelayTime::create(fPauseTime);
		CCActionInterval* pTremble3 = CCScaleTo::create(0.2f,1);
		CCActionInterval* pTremble4 = CCMoveBy::create(0.2f,ccp(-pFTarget->GetCurrentCube()->ptPos3D.fX*1.f,-(pFTarget->GetCurrentCube()->ptPos3D.fY*m_fRate + pFTarget->GetCurrentCube()->ptPos3D.fZ)));
		CCActionInterval* pTremble5 = CCMoveBy::create(0.2f,ccp(pFTarget->GetCurrentCube()->ptPos3D.fX*1.f,(pFTarget->GetCurrentCube()->ptPos3D.fY*m_fRate + pFTarget->GetCurrentCube()->ptPos3D.fZ)));
		CCActionInterval* pTremble8 = CCMoveBy::create(0.2f,ccp(0,-(pFTarget->GetCurrentCube()->ptPos3D.fY*m_fRate + pFTarget->GetCurrentCube()->ptPos3D.fZ)));
		CCActionInterval* pTremble9 = CCMoveBy::create(0.2f,ccp(0,(pFTarget->GetCurrentCube()->ptPos3D.fY*m_fRate + pFTarget->GetCurrentCube()->ptPos3D.fZ)));

		CCActionInterval* pTremble6 = NULL;
		CCActionInterval* pTremble7 = NULL;

		if(pFTarget->GetCurrentCube()->ptPos3D.fX - pFTarget->GetCurrentCube()->szCube.fLength*0.5f >= m_ShotCube.ptPos3D.fX - m_ShotCube.szCube.fLength*0.5f)
		{
			pTremble6 = CCSpawn::create(pTremble,pTremble4,NULL);
			pTremble7 = CCSpawn::create(pTremble3,pTremble5,NULL);
		}
		else
		{
			pTremble6 = CCSpawn::create(pTremble,pTremble8,NULL);
			pTremble7 = CCSpawn::create(pTremble3,pTremble9,NULL);
		}


		CCFiniteTimeAction* pLensFeature = CCSequence::create(pTremble6,pTremble2,pTremble7,CCCallFunc::create(this,callfunc_selector(CNFWorld3DLayer::OnLensFeatureCallBack)),NULL);
		this->runAction(pLensFeature);
		
		return;
	} while (false);
	CCLOG("CNFWorld3DLayer::LensFeature Run Error!");
}

void CNFWorld3DLayer::OnLensFeatureCallBack()
{
	m_bIsLensFeatureOver = false;
}


void CNFWorld3DLayer::SetScreenColor( int nColorID,float fTime )
{
	do 
	{
		CNFEffectLayer * pEffectLayer = dynamic_cast<CNFEffectLayer *>(getChildByTag(enTagEffectLayer));
		CC_BREAK_IF(pEffectLayer==NULL);

		switch (nColorID)
		{
		case 1:		//黑色
			{
				pEffectLayer->setColor(ccBLACK);
				pEffectLayer->setOpacity(255.f);
			}break;
		}

		runAction(CCSequence::create(CCDelayTime::create(fTime),CCCallFunc::create(this,callfunc_selector(CNFWorld3DLayer::OnScreenChangeColorOver)),NULL));

		return ;
	} while (false);
	CCLog("Fun CNFWorld3DLayer::SetScreenColor Error!");
}

void CNFWorld3DLayer::OnScreenChangeColorOver()
{
	do 
	{
		CNFEffectLayer * pEffectLayer = dynamic_cast<CNFEffectLayer *>(getChildByTag(enTagEffectLayer));
		CC_BREAK_IF(pEffectLayer==NULL);
		pEffectLayer->setColor(ccc3(0,0,0));
		pEffectLayer->setOpacity(0.f);

		return ;
	} while (false);
	CCLog("Fun CNFWorld3DLayer::OnScreenChangeColorOver Error!");
}




/************************************************************************/
/*				特效层                                                                     */
/************************************************************************/
CNFEffectLayer * CNFEffectLayer::CreateLayer()
{
	do 
	{
		CNFEffectLayer* pLayer = new CNFEffectLayer();
		if (pLayer!=NULL && pLayer->InitLayer()==true)
		{
			pLayer->autorelease();
			return pLayer;
		}

		delete pLayer; 
		pLayer = NULL; 
		return NULL; 
	} while (false);
	CCLog("Fun CNFEffectLayer::CreateLayer Error!");
	return NULL;
}

bool CNFEffectLayer::InitLayer()
{
	do 
	{
		CC_BREAK_IF(CCLayerColor::initWithColor(ccc4(0,0,0,0))==false);


		return true;
	} while (false);
	CCLog("Fnn CNFEffectLayer::InitLayer Error!");
	return false;
}



/************************************************************************/
/*				单位类                                                  */
/************************************************************************/
//////////////////////////////////////////////////////////////////////////

CNFUnit3D* CNFUnit3D::create(tagUnit3DBasicInfo unitInfo)
{
	do 
	{
		CNFUnit3D* pUnit = new CNFUnit3D();
		CC_BREAK_IF(pUnit==NULL);
		CC_BREAK_IF(!pUnit->init(unitInfo));
		pUnit->autorelease();
		return pUnit;
	} while (false);
	CCLog("Fun CLTShiJieUnit* CLTShiJieUnit::create Error!");
	return NULL;
}

bool CNFUnit3D::init(tagUnit3DBasicInfo unitInfo)
{
	do 
	{
		CC_BREAK_IF(!CCNode::init());
		//=============================================
		m_Cube3D.ptPos3D.fX = 0;			
		m_Cube3D.ptPos3D.fY = 0;			
		m_Cube3D.ptPos3D.fZ = 0;			
		m_Cube3D.szCube.fLength = 0;		
		m_Cube3D.szCube.fWidth = 0;		
		m_Cube3D.szCube.fHeight = 0;	


		if(unitInfo.fRate<0){
			unitInfo.fRate = 0;
		}
		if(unitInfo.fRate>1){
			unitInfo.fRate = 1;
		}
		m_fRate = unitInfo.fRate;//0.4f;
		m_nGround = 0;
		m_fZSpeed = 0;
		m_fSubtractSpeedl = 0.4f;

		m_bIsBorderVisible = false;
		m_bIsGravity = false;
		m_bIsOnOutside = true;
		
		CCSprite* pBody = CCSprite::create("ltshijie/body.png");
		CC_BREAK_IF(pBody==NULL);
		pBody->setAnchorPoint(ccp(0.5f,0));
		//pBody->setPosition(ccp(pBody->getContentSize().width/2,0));
		addChild(pBody,-1,enTagLTBody);

		//创建影子
		CCSprite * pShadow = CCSprite::create("ltshijie/shadow.png");
		CC_BREAK_IF(pShadow==NULL);
		m_fShadowW = pShadow->getContentSize().width;
		m_fShadowH = pShadow->getContentSize().height;
		pShadow->setScaleX((m_Cube3D.szCube.fLength)/(m_fShadowW));
		pShadow->setScaleY(((m_Cube3D.szCube.fWidth)/(m_fShadowH))*m_fRate);
		pShadow->setPositionY(m_nGround);
		addChild(pShadow,-2,enTagLTShadow);

		//设置位置
		setPosition(CCPointMake(m_Cube3D.ptPos3D.fX,m_Cube3D.ptPos3D.fY*m_fRate));
		return true;
	} while (false);
	CCLog("Fun CLTShiJieUnit::init Error!");
	return false;
}


void CNFUnit3D::onExit()
{
	CCNode::onExit();
}

void CNFUnit3D::draw()
{
	CCNode::draw();
	//位置
	if(m_ActionInfoVec.size()!=0){
		for(VecActionInfo::iterator it = m_ActionInfoVec.begin();it != m_ActionInfoVec.end();it++)
		{
			if(it->lActionTime<=0)
			{
				m_ActionInfoVec.erase(it);
				break;
			}
			this->m_Cube3D.ptPos3D.fX += it->vec3dSpeed.fX;
			this->m_Cube3D.ptPos3D.fY += it->vec3dSpeed.fY;
			this->m_Cube3D.ptPos3D.fZ += it->vec3dSpeed.fZ;
			tagPosition3D ptPos3D;
			ptPos3D.fX = this->m_Cube3D.ptPos3D.fX;
			ptPos3D.fY = this->m_Cube3D.ptPos3D.fY;
			ptPos3D.fZ = this->m_Cube3D.ptPos3D.fZ;
			setPosition3D(ptPos3D);
			it->lActionTime--;
		}
	}

	//体积
	if(m_ScaleActionInfoVec.size()!=0){
		for(VecScaleActionInfo::iterator it = m_ScaleActionInfoVec.begin();it != m_ScaleActionInfoVec.end();it++)
		{
			if(it->lActionTime<=0)
			{
				m_ScaleActionInfoVec.erase(it);
				break;
			}
			this->m_Cube3D.szCube.fLength += it->vec3dScale.fX;			
			this->m_Cube3D.szCube.fWidth += it->vec3dScale.fY;
			this->m_Cube3D.szCube.fHeight += it->vec3dScale.fZ;
			if(this->m_Cube3D.ptPos3D.fX<0){
				this->m_Cube3D.ptPos3D.fX = 0;
			}
			if(this->m_Cube3D.szCube.fWidth<0){
				this->m_Cube3D.szCube.fWidth = 0;
			}
			if(this->m_Cube3D.szCube.fHeight<0){
				this->m_Cube3D.szCube.fHeight = 0;
			}
			CCSprite* pShadow = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTShadow));
			if(pShadow!=NULL){
				pShadow->setScaleX((m_Cube3D.szCube.fLength)/(m_fShadowW));
				pShadow->setScaleY(((m_Cube3D.szCube.fWidth)/(m_fShadowH))*m_fRate);
			}
			it->lActionTime--;
		}
	}


	//重力
	if(m_bIsGravity){
		if(m_Cube3D.ptPos3D.fZ >= m_nGround){
			if((m_Cube3D.ptPos3D.fZ + m_fZSpeed) - m_nGround > m_fSubtractSpeedl){
				m_Cube3D.ptPos3D.fZ += m_fZSpeed;
			}else{
				m_Cube3D.ptPos3D.fZ = m_nGround;
			}

			if(m_fZSpeed>_MIN_Z_SPEED_){
				m_fZSpeed -= m_fSubtractSpeedl;
			}else{
				m_fZSpeed = _MIN_Z_SPEED_;
			}
			CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
			if(pBody!=NULL){
				pBody->setPositionY(m_Cube3D.ptPos3D.fZ);
			}
		}else{
			m_Cube3D.ptPos3D.fZ = m_nGround;
			m_fZSpeed = 0;
			CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
			if(pBody!=NULL){
				pBody->setPositionY(m_nGround);
			}
		}
	}
	//边线
	if(m_bIsBorderVisible){
		CCPoint p1,p2,p3,p4,p5,p6,p7,p8;

		p1.x = -m_Cube3D.szCube.fLength*0.5f;	
		p1.y = -m_Cube3D.szCube.fWidth*0.5f*m_fRate + m_Cube3D.ptPos3D.fZ;			//宽度乘以斜率
		p2.x = m_Cube3D.szCube.fLength*0.5f;
		p2.y = -m_Cube3D.szCube.fWidth*0.5f*m_fRate + m_Cube3D.ptPos3D.fZ;
		p3.x = m_Cube3D.szCube.fLength*0.5f + m_Cube3D.szCube.fWidth*0.5f*_RATE_X_;
		p3.y = m_Cube3D.szCube.fWidth*0.5f*m_fRate + m_Cube3D.ptPos3D.fZ;
		p4.x = -m_Cube3D.szCube.fLength*0.5f + m_Cube3D.szCube.fWidth*0.5f*_RATE_X_;
		p4.y = m_Cube3D.szCube.fWidth*0.5f*m_fRate + m_Cube3D.ptPos3D.fZ;

		p5.x = p1.x;
		p5.y = p1.y + m_Cube3D.szCube.fHeight;
		p6.x = p2.x;
		p6.y = p2.y + m_Cube3D.szCube.fHeight;
		p7.x = p3.x;
		p7.y = p3.y + m_Cube3D.szCube.fHeight;
		p8.x = p4.x;
		p8.y = p4.y + m_Cube3D.szCube.fHeight;

		//画线
		ccDrawColor4B(255,0,0,255);

		ccDrawLine(p2,p3);
		ccDrawLine(p3,p4);
		ccDrawLine(p4,p1);

		ccDrawLine(p6,p7);
		ccDrawLine(p7,p8);
		ccDrawLine(p8,p5);

		ccDrawLine(p1,p5);
		ccDrawLine(p2,p6);

		ccDrawLine(p3,p7);
		ccDrawLine(p4,p8);

		ccDrawLine(p1,p2);
		ccDrawLine(p5,p6);
	}
}

void CNFUnit3D::setPosition3D( tagPosition3D ptPos3D )
{
	m_Cube3D.ptPos3D.fX = ptPos3D.fX;			
	m_Cube3D.ptPos3D.fY = ptPos3D.fY;			
	this->setPositionX(m_Cube3D.ptPos3D.fX);
	this->setPositionY(m_Cube3D.ptPos3D.fY*m_fRate);

	if (ptPos3D.fZ!=0)
	{
		m_Cube3D.ptPos3D.fZ = ptPos3D.fZ;
		CCSprite* pBody = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTBody));
		if(pBody!=NULL){
			pBody->setPositionY(m_Cube3D.ptPos3D.fZ);
		}
	}
}


const tagCube3D* CNFUnit3D::GetCurrentCube()
{
	return &m_Cube3D;
}

void CNFUnit3D::SetBorderVisible( bool isVisible )
{
	m_bIsBorderVisible = isVisible;
}

void CNFUnit3D::SetGravityOpen( bool bGravityOpen )
{
	m_bIsGravity = bGravityOpen;
}

void CNFUnit3D::SetShadowVisible( bool bVisible )
{
	CCSprite* pShadow = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTShadow));
	if(pShadow!=NULL){
		pShadow->setVisible(bVisible);
	}
}

void CNFUnit3D::setSize3D( tagSize3D szNew )
{
	if(szNew.fLength>=0&&szNew.fWidth>=0&&szNew.fHeight>=0){
		m_Cube3D.szCube.fLength = szNew.fLength;
		m_Cube3D.szCube.fWidth = szNew.fWidth;
		m_Cube3D.szCube.fHeight = szNew.fHeight;

		CCSprite* pShadow = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTShadow));
		if(pShadow!=NULL){
			pShadow->setScaleX((m_Cube3D.szCube.fLength)/(m_fShadowW));
			pShadow->setScaleY(((m_Cube3D.szCube.fWidth)/(m_fShadowH))*m_fRate);
		}
	}else{
		CCLog("Fun CLTShiJieUnit::setLTContentSize Error! l,w,h < 0 ?");
	}
}

void CNFUnit3D::SetZSpeed( float zSpeed )
{
	m_fZSpeed = zSpeed;
}

float CNFUnit3D::GetZSpeed()
{
	return m_fZSpeed;
}

void CNFUnit3D::SetGround( int nHeight )
{
	m_nGround = nHeight;
	CCSprite* pShadow = dynamic_cast<CCSprite*>(this->getChildByTag(enTagLTShadow));
	if(pShadow!=NULL){
		pShadow->setPositionY(m_nGround);
	}
}

bool CNFUnit3D::GetIntersectsVec(const CNFUnit3D* pInitiative,tagVector3D &InsersectResult)
{
	//if((m_Cube3D.ptPos3D.fX - this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2)){}
	//if((m_Cube3D.ptPos3D.fX + this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2)){}
	//if((m_Cube3D.ptPos3D.fZ + this->height >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ + this->height <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight)){}
	//if((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight)){}
	//if((m_Cube3D.ptPos3D.fY - this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2)){}
	//if((m_Cube3D.ptPos3D.fY + this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2)){}


	
	float fLenX = pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX >= 0 ? pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX : -(pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX);
	float fLenY = pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY >= 0 ? pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY : -(pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY);
	float fLenZ = pInitiative->m_Cube3D.ptPos3D.fZ - m_Cube3D.ptPos3D.fZ >= 0 ? pInitiative->m_Cube3D.ptPos3D.fZ - m_Cube3D.ptPos3D.fZ : -(pInitiative->m_Cube3D.ptPos3D.fZ - m_Cube3D.ptPos3D.fZ);

	//if(fLenX <= this->length/2 + pInitiative->m_Cube3D.szCube.fLength/2){}
	//if(fLenY <= this->width/2 + pInitiative->m_Cube3D.szCube.fWidth/2){}
	//if(fLenZ <= this->height/2 + pInitiative->m_Cube3D.szCube.fHeight/2){}

	//if(pInitiative->m_Cube3D.ptPos3D.fL - m_Cube3D.ptPos3D.fX >= 0){}else{}
	//if(pInitiative->m_Cube3D.ptPos3D.fW - m_Cube3D.ptPos3D.fY >= 0){}else{}
	//if(pInitiative->m_Cube3D.ptPos3D.fH - m_Cube3D.ptPos3D.fZ >= 0){}else{}

	if(fLenX>=fLenY&&fLenX>=fLenZ){
// 		if((m_Cube3D.ptPos3D.fX - this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2)){
// 			if(((m_Cube3D.ptPos3D.fY - this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2))||((m_Cube3D.ptPos3D.fY + this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2))){
// 				if(((m_Cube3D.ptPos3D.fZ + this->height >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ + this->height <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight))||((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight))){
// 					return enTagIntersects_LEFT;
// 				}
// 			}
// 		}
// 		if((m_Cube3D.ptPos3D.fX + this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2)){
// 			if(((m_Cube3D.ptPos3D.fY - this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2))||((m_Cube3D.ptPos3D.fY + this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2))){
// 				if(((m_Cube3D.ptPos3D.fZ + this->height >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ + this->height <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight))||((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight))){
// 					return enTagIntersects_RIGHT;
// 				}
// 			}
// 		}
		
		if(fLenX <= m_Cube3D.szCube.fLength/2 + pInitiative->m_Cube3D.szCube.fLength/2){
			if((fLenY <= this->m_Cube3D.szCube.fWidth/2 + pInitiative->m_Cube3D.szCube.fWidth/2)&&(fLenZ <= this->m_Cube3D.szCube.fHeight/2 + pInitiative->m_Cube3D.szCube.fHeight/2)){

				InsersectResult.fX = m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.ptPos3D.fX;
				InsersectResult.fY = m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.ptPos3D.fY;
				InsersectResult.fZ = m_Cube3D.ptPos3D.fZ - pInitiative->m_Cube3D.ptPos3D.fZ;
				if(pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX >= 0){
					return true;//enTagIntersects_RIGHT;
				}else{
					return true;//enTagIntersects_LEFT;
				}
			}
		}

	}
	if(fLenZ>=fLenX&&fLenZ>=fLenY){
// 		if((m_Cube3D.ptPos3D.fZ + this->height >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ + this->height <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight)){
// 			if(((m_Cube3D.ptPos3D.fX - this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2))||((m_Cube3D.ptPos3D.fX + this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2))){
// 				if(((m_Cube3D.ptPos3D.fY - this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2))||((m_Cube3D.ptPos3D.fY + this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2))){
// 					return enTagIntersects_TOP;
// 				}
// 			}
// 		}
// 		if((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight)){
// 			if(((m_Cube3D.ptPos3D.fX - this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2))||((m_Cube3D.ptPos3D.fX + this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2))){
// 				if(((m_Cube3D.ptPos3D.fY - this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2))||((m_Cube3D.ptPos3D.fY + this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2))){
// 					return enTagIntersects_DOWN;
// 				}
// 			}
// 		}

		
		if((fLenX <= m_Cube3D.szCube.fLength/2 + pInitiative->m_Cube3D.szCube.fLength/2)&&(fLenY <= this->m_Cube3D.szCube.fWidth/2 + pInitiative->m_Cube3D.szCube.fWidth/2)){
			if(pInitiative->m_Cube3D.ptPos3D.fZ - m_Cube3D.ptPos3D.fZ >= 0){
				if(fLenZ <= this->m_Cube3D.szCube.fHeight){
					InsersectResult.fX = m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.ptPos3D.fX;
					InsersectResult.fY = m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.ptPos3D.fY;
					InsersectResult.fZ = m_Cube3D.ptPos3D.fZ - pInitiative->m_Cube3D.ptPos3D.fZ;
					return true;//enTagIntersects_TOP;
				}
			}else{
				if(fLenZ <= pInitiative->m_Cube3D.szCube.fHeight){
					InsersectResult.fX = m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.ptPos3D.fX;
					InsersectResult.fY = m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.ptPos3D.fY;
					InsersectResult.fZ = m_Cube3D.ptPos3D.fZ - pInitiative->m_Cube3D.ptPos3D.fZ;
					return true;//enTagIntersects_DOWN;
				}
			}
		}
		
	}
	if(fLenY>=fLenX&&fLenY>=fLenZ){
// 		if((m_Cube3D.ptPos3D.fY - this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2)){
// 			if(((m_Cube3D.ptPos3D.fX - this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2))||((m_Cube3D.ptPos3D.fX + this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2))){
// 				if(((m_Cube3D.ptPos3D.fZ + this->height >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ + this->height <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight))||((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight))){
// 					return enTagIntersects_UP;
// 				}
// 			}
// 		}
// 		if((m_Cube3D.ptPos3D.fY + this->width/2 >= pInitiative->m_Cube3D.ptPos3D.fW - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->width/2 <= pInitiative->m_Cube3D.ptPos3D.fW + pInitiative->m_Cube3D.szCube.fWidth/2)){
// 			if(((m_Cube3D.ptPos3D.fX - this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2))||((m_Cube3D.ptPos3D.fX + this->length/2 >= pInitiative->m_Cube3D.ptPos3D.fL - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + this->length/2 <= pInitiative->m_Cube3D.ptPos3D.fL + pInitiative->m_Cube3D.szCube.fLength/2))){
// 				if(((m_Cube3D.ptPos3D.fZ + this->height >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ + this->height <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight))||((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fH)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fH + pInitiative->m_Cube3D.szCube.fHeight))){
// 					return enTagIntersects_BACK;
// 				}
// 			}
// 		}

		if(fLenY <= this->m_Cube3D.szCube.fWidth/2 + pInitiative->m_Cube3D.szCube.fWidth/2){
			if((fLenX <= m_Cube3D.szCube.fLength/2 + pInitiative->m_Cube3D.szCube.fLength/2)&&(fLenZ <= this->m_Cube3D.szCube.fHeight/2 + pInitiative->m_Cube3D.szCube.fHeight/2)){
				InsersectResult.fX = m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.ptPos3D.fX;
				InsersectResult.fY = m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.ptPos3D.fY;
				InsersectResult.fZ = m_Cube3D.ptPos3D.fZ - pInitiative->m_Cube3D.ptPos3D.fZ;
				if(pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY >= 0){
					return true;//enTagIntersects_BACK;
				}else{
					return true;//enTagIntersects_UP;
				}
			}
		}
	}
	return false;//enTagIntersects_NO;
}

int CNFUnit3D::IntersectsLT_Z( CNFUnit3D* pInitiative ,int nBian)
{
	int bian = nBian;
	float fLenX = pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX >= 0 ? pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX : -(pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX);
	float fLenY = pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY >= 0 ? pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY : -(pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY);
	float fLenZ = pInitiative->m_Cube3D.ptPos3D.fZ - m_Cube3D.ptPos3D.fZ >= 0 ? pInitiative->m_Cube3D.ptPos3D.fZ - m_Cube3D.ptPos3D.fZ : -(pInitiative->m_Cube3D.ptPos3D.fZ - m_Cube3D.ptPos3D.fZ);

	if(fLenX>=m_Cube3D.szCube.fLength/2 + pInitiative->m_Cube3D.szCube.fLength/2 - bian){
		if((m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2)){
			if(((m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2))||((m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2))){
				if(((m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight))||((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight))){
					return enTagIntersects_LEFT;
				}
			}
		}
		if((m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2)){
			if(((m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2))||((m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2))){
				if(((m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight))||((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight))){
					return enTagIntersects_RIGHT;
				}
			}
		}
// 		if(fLenX <= this->length/2 + pInitiative->m_Cube3D.szCube.fLength/2){
// 			if((fLenY <= this->width/2 + pInitiative->m_Cube3D.szCube.fWidth/2)&&(fLenZ <= this->height/2 + pInitiative->m_Cube3D.szCube.fHeight/2)){
// 				if(pInitiative->m_Cube3D.ptPos3D.fL - m_Cube3D.ptPos3D.fX >= 0){
// 					return enTagIntersects_RIGHT;
// 				}else{
// 					return enTagIntersects_LEFT;
// 				}
// 			}
// 		}
	}
	
	if(fLenY>=this->m_Cube3D.szCube.fWidth/2 + pInitiative->m_Cube3D.szCube.fWidth/2 - bian){
		if((m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2)){
			if(((m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2))||((m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2))){
				if(((m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight))||((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight))){
					return enTagIntersects_UP;
				}
			}
		}
		if((m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2)){
			if(((m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2))||((m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2))){
				if(((m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight))||((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight))){
					return enTagIntersects_BACK;
				}
			}
		}

// 		if(fLenY <= this->width/2 + pInitiative->m_Cube3D.szCube.fWidth/2){
// 			if((fLenX <= this->length/2 + pInitiative->m_Cube3D.szCube.fLength/2)&&(fLenZ <= this->height/2 + pInitiative->m_Cube3D.szCube.fHeight/2)){
// 				if(pInitiative->m_Cube3D.ptPos3D.fW - m_Cube3D.ptPos3D.fY >= 0){
// 					return enTagIntersects_BACK;
// 				}else{
// 					return enTagIntersects_UP;
// 				}
// 			}
// 		}
	}
	//if(fLenZ>=fLenX&&fLenZ>=fLenY){
		if((m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ + this->m_Cube3D.szCube.fHeight <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight)){
			if(((m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2))||((m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2))){
				if(((m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2))||((m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2))){
					return enTagIntersects_TOP;
				}
			}
		}
		if((m_Cube3D.ptPos3D.fZ >= pInitiative->m_Cube3D.ptPos3D.fZ)&&(m_Cube3D.ptPos3D.fZ <= pInitiative->m_Cube3D.ptPos3D.fZ + pInitiative->m_Cube3D.szCube.fHeight)){
			if(((m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2))||((m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 > pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2)&&(m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2))){
				if(((m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2))||((m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 > pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2)&&(m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2))){
					return enTagIntersects_DOWN;
				}
			}
		}
	//}

	return enTagIntersects_NO;
}

bool CNFUnit3D::IntersectsLT_TopProjection( CNFUnit3D* pInitiative ,int nBian)
{
	int bian = nBian;
	float fLenX = pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX >= 0 ? pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX : -(pInitiative->m_Cube3D.ptPos3D.fX - m_Cube3D.ptPos3D.fX);
	float fLenY = pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY >= 0 ? pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY : -(pInitiative->m_Cube3D.ptPos3D.fY - m_Cube3D.ptPos3D.fY);
	if((fLenX<m_Cube3D.szCube.fLength/2 + pInitiative->m_Cube3D.szCube.fLength/2 - bian)&&(fLenY<this->m_Cube3D.szCube.fWidth/2 + pInitiative->m_Cube3D.szCube.fWidth/2 - bian)){
		return !(     m_Cube3D.ptPos3D.fX + m_Cube3D.szCube.fLength/2 < pInitiative->m_Cube3D.ptPos3D.fX - pInitiative->m_Cube3D.szCube.fLength/2 ||
			pInitiative->m_Cube3D.ptPos3D.fX + pInitiative->m_Cube3D.szCube.fLength/2 <      m_Cube3D.ptPos3D.fX - m_Cube3D.szCube.fLength/2 ||
			m_Cube3D.ptPos3D.fY + this->m_Cube3D.szCube.fWidth/2 < pInitiative->m_Cube3D.ptPos3D.fY - pInitiative->m_Cube3D.szCube.fWidth/2 ||
			pInitiative->m_Cube3D.ptPos3D.fY + pInitiative->m_Cube3D.szCube.fWidth/2 <      m_Cube3D.ptPos3D.fY - this->m_Cube3D.szCube.fWidth/2);
	}
	return false;
}

int CNFUnit3D::GetGround()
{
	return m_nGround;
}

CNFWorld3DLayer* CNFUnit3D::getLTLayerParent()
{
	return dynamic_cast<CNFWorld3DLayer *>(getParent());
}

void CNFUnit3D::MoveBy3D( float fTime,tagVector3D vec3D,bool bRemoveSelfAfterAction /*= false*/ )
{
	long	nNum = (int)(fTime*60);
	if(nNum<1){
		nNum = 1;
	}
	float	fFenX = (vec3D.fX)/nNum;
	float	fFenY = (vec3D.fY)/nNum;
	float	fFenZ = (vec3D.fZ)/nNum;

	tagActionInfo tActionInfo;
	tActionInfo.lActionTime = nNum;
	tActionInfo.vec3dSpeed.fX = fFenX;
	tActionInfo.vec3dSpeed.fY = fFenY;
	tActionInfo.vec3dSpeed.fZ = fFenZ;
	m_ActionInfoVec.push_back(tActionInfo);
	if (bRemoveSelfAfterAction==true)
	{
		CCAction* pAction= CCSequence::create(
		CCDelayTime::create(fTime),
		CCRemoveSelf::create(),
		NULL);
		runAction(pAction);
	}

}

void CNFUnit3D::MoveTo3D( float fTime,tagPosition3D ptTarget,bool bRemoveSelfAfterAction /*= false*/ )
{
	long	nNum = (int)(fTime*60);
	if(nNum<1){
		nNum = 1;
	}
	float	fFenX = (ptTarget.fX - this->m_Cube3D.ptPos3D.fX)/nNum;
	float	fFenY = (ptTarget.fY - this->m_Cube3D.ptPos3D.fY)/nNum;
	float	fFenZ = (ptTarget.fZ - this->m_Cube3D.ptPos3D.fZ)/nNum;

	tagActionInfo tActionInfo;
	tActionInfo.lActionTime = nNum;
	tActionInfo.vec3dSpeed.fX = fFenX;
	tActionInfo.vec3dSpeed.fY = fFenY;
	tActionInfo.vec3dSpeed.fZ = fFenZ;
	m_ActionInfoVec.push_back(tActionInfo);

	if (bRemoveSelfAfterAction==true)
	{
		CCAction* pAction= CCSequence::create(
			CCDelayTime::create(fTime),
			CCRemoveSelf::create(),
			NULL);
		runAction(pAction);
	}
}

void CNFUnit3D::ScaleTo3D( float fTime,tagSize3D szScaleTo,bool bRemoveSelfAfterAction /*= false*/ )
{
	long	nNum = (int)(fTime*60);
	if(nNum<1){
		nNum = 1;
	}
	float	fFenL = (szScaleTo.fLength - m_Cube3D.szCube.fLength)/nNum;
	float	fFenW = (szScaleTo.fWidth - this->m_Cube3D.szCube.fWidth)/nNum;
	float	fFenH = (szScaleTo.fHeight - this->m_Cube3D.szCube.fHeight)/nNum;

	tagScaleActionInfo tActionInfo;
	tActionInfo.lActionTime = nNum;
	tActionInfo.vec3dScale.fX = fFenL;
	tActionInfo.vec3dScale.fY = fFenW;
	tActionInfo.vec3dScale.fZ = fFenH;
	m_ScaleActionInfoVec.push_back(tActionInfo);
	if (bRemoveSelfAfterAction==true)
	{
		CCAction* pAction= CCSequence::create(
			CCDelayTime::create(fTime),
			CCRemoveSelf::create(),
			NULL);
		runAction(pAction);
	}
}

void CNFUnit3D::ScaleBy3D( float fTime,tagVector3D vec3D,bool bRemoveSelfAfterAction /*= false*/ )
{
	float fNum = fTime*60.f;
	if(fNum<1.f){
		fNum = 1.f;
	}
	
	tagScaleActionInfo tActionInfo;
	tActionInfo.lActionTime = (long)fNum;
	tActionInfo.vec3dScale.fX = (vec3D.fX)/fNum;
	tActionInfo.vec3dScale.fY = (vec3D.fY)/fNum;
	tActionInfo.vec3dScale.fZ = (vec3D.fZ)/fNum;

	m_ScaleActionInfoVec.push_back(tActionInfo);

	if (bRemoveSelfAfterAction==true)
	{
		CCAction* pAction= CCSequence::create(
			CCDelayTime::create(fTime),
			CCRemoveSelf::create(),
			NULL);
		runAction(pAction);
	}
}

void CNFUnit3D::StopMoveAction()
{
	m_ActionInfoVec.clear();
}

void CNFUnit3D::StopScaleAction()
{
	m_ScaleActionInfoVec.clear();
}

void CNFUnit3D::SetLTSubtractSpeedl( float fSubtractSpeedl )
{
	m_fSubtractSpeedl = fSubtractSpeedl;
}

float CNFUnit3D::GetLTSubtractSpeedl()
{
	return	m_fSubtractSpeedl;
}

void CNFUnit3D::SetLTRate( float fRate )
{
	if(fRate<0){
		fRate = 0;
	}
	if(fRate>1){
		fRate = 1;
	}
	this->m_fRate = fRate;
}

float CNFUnit3D::GetLTRate()
{
	return this->m_fRate;
}







































