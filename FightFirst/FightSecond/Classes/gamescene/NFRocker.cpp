#include "NFRocker.h"
#include "../gameunit/person/NFProtagonist.h"
#include "../ui/NFTownScene.h"

#define		_NF_PUBLIC_SKILL_CD_		0.8f		//技能公共CD时间（秒）
#define		_NF_ROCKER_MAX_DIS_			50.f		//摇杆最大偏移量
#define		_NF_ROCKER_SPEED_			3.f			//摇杆的速度

CNFRockerLayer * CNFRockerLayer::CreateLayer(CNF3DWorldLayer * pLayer,int nStageID)
{
	CNFRockerLayer *pRet = new CNFRockerLayer(); 
	if (pRet && pRet->InitLayer(pLayer,nStageID)) 
	{ 
		pRet->autorelease(); 
		return pRet; 
	} 

	delete pRet; 
	pRet = NULL; 
	return NULL; 	
}

bool CNFRockerLayer::InitLayer(CNF3DWorldLayer * pLayer,int nStageID)
{
	do 
	{
		//初始化父类
		CC_BREAK_IF(CCLayerColor::initWithColor(ccc4(255,0,0,50))==false);

		//注册
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-1,false);

		//设置层大小
		setContentSize(CCSizeMake(200,200));//(CCSizeMake(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT));

		m_p3DLayer = pLayer;
		m_bIsTouching = false;
		m_bIsPublicCD = false;
		m_fRockerSpeed = _NF_ROCKER_SPEED_;		//摇杆的移动速度
		m_fRockerMaxDis = _NF_ROCKER_MAX_DIS_;	//摇杆最大偏移量
		m_nStageID = nStageID;

		//主角技能ID
		CNFProtagonist * pPro = dynamic_cast<CNFProtagonist *>(pLayer->getChildByTag(enTagProtagonist));
		CC_BREAK_IF(pPro==NULL);
		m_nSkill_1_ID = pPro->GetSkillInfo_1().nSkill_SkillItemID;
		m_nSkill_2_ID = pPro->GetSkillInfo_2().nSkill_SkillItemID;
		m_nSkill_3_ID = pPro->GetSkillInfo_3().nSkill_SkillItemID;
		m_nSkill_4_ID = pPro->GetSkillInfo_4().nSkill_SkillItemID;
		m_nSkill_5_ID = pPro->GetSkillInfo_5().nSkill_SkillItemID;
		
		//创建摇杆背景
		CCSprite * pRockerBg = CCSprite::create("gameui/rocker_bg.png");
		CC_BREAK_IF(pRockerBg==NULL);
		//pRockerBg->setScale(2.5f);
		pRockerBg->setPosition(ccp(100,100));
		addChild(pRockerBg,enZOrderFront,enTagRockerBg);
		pRockerBg->setOpacity(100.f);

		//创建摇杆
		CCSprite * pRocker = CCSprite::create("gameui/rocker_ball.png");
		CC_BREAK_IF(pRocker==NULL);
		pRocker->setPosition(pRockerBg->getPosition());
		addChild(pRocker,enZOrderFront+1,enTagRocker);
		pRocker->setOpacity(100.f);

		//若为副本，则创建按钮
		if (m_nStageID >= _NF_TOWN_OR_BATTLE_ID_ && m_nStageID <_NF_TOWN_FB_ID_)
		{
			/************************************************************************/
			/*				创建按钮                                                                     */
			/************************************************************************/
			//创建菜单
			CCMenu * pMenu = CCMenu::create();
			CC_BREAK_IF(pMenu==NULL);
			pMenu->setPosition(CCPointZero);
			addChild(pMenu,enZOrderFront,enTagMenu);

			//创建普通攻击按钮
			CCSprite * pCommon1 = CCSprite::create("gameui/skill_common.png");
			CC_BREAK_IF(pCommon1==NULL);
			CCSprite * pCommon2 = CCSprite::create("gameui/skill_common.png");
			CC_BREAK_IF(pCommon2==NULL);
			pCommon2->setOpacity(150.f);
			CCMenuItemSprite * pBtnCommon = CCMenuItemSprite::create(pCommon1,pCommon2,this,menu_selector(CNFRockerLayer::OnBtnCallBack));
			CC_BREAK_IF(pBtnCommon==NULL);
			pBtnCommon->setPosition(ccp(SCREEN_WIDTH - 60,58));
			pMenu->addChild(pBtnCommon,enZOrderFront,enTagBtnCommonAttack);



			CCLabelBMFont* pCommonAttackFont = CCLabelBMFont::create("1","fonts/mhp_num.fnt");
			pCommonAttackFont->setPosition(pBtnCommon->getPosition());
			this->addChild(pCommonAttackFont,enZOrderFront,enTagCommonAttackFont);

			CCSprite * pCommon3 = CCSprite::create("gameui/skill_common.png");
			CC_BREAK_IF(pCommon3==NULL);
			pCommon3->setColor(ccRED);
			pCommon3->setOpacity(100.f);
			
			//创建普通攻击CD
			CCProgressTimer* pProgressCommonAttackCD = CCProgressTimer::create(pCommon3);
			CC_BREAK_IF(pProgressCommonAttackCD==NULL);
			pProgressCommonAttackCD->setType(kCCProgressTimerTypeRadial);
			pProgressCommonAttackCD->setMidpoint(ccp(0.5f,0.5f));
			pProgressCommonAttackCD->setPercentage(0.f);
			pProgressCommonAttackCD->setPosition(pBtnCommon->getPosition());
			this->addChild(pProgressCommonAttackCD,enZOrderFront+1,enTagCommonAttackCD);

			//创建技能按钮
			for (int i=enTagBtnSkill1;i<=enTagBtnSkill5;i++)
			{
				char szName[NAME_LEN] = {0};

				int skilltemp = -1;
				if(i==enTagBtnSkill1){
					skilltemp = m_nSkill_1_ID;
				}else if(i==enTagBtnSkill2){
					skilltemp = m_nSkill_2_ID;
				}else if(i==enTagBtnSkill3){
					skilltemp = m_nSkill_3_ID;
				}else if(i==enTagBtnSkill4){
					skilltemp = m_nSkill_4_ID;
				}else if(i==enTagBtnSkill5){
					skilltemp = m_nSkill_5_ID;
				}

				if(skilltemp==-1){
					continue;
				}

				sprintf(szName,"Skill/r%d_s%d.png",pPro->GetRoleID(),skilltemp);

				CCSprite * pSpr_n = CCSprite::create(szName);
				CC_BREAK_IF(pSpr_n==NULL);
				CCSprite * pSpr_p = CCSprite::create(szName);
				CC_BREAK_IF(pSpr_p==NULL);;
				pSpr_p->setColor(ccBLUE);
				CCSprite * pSpr_d = CCSprite::create(szName);
				CC_BREAK_IF(pSpr_d==NULL);
				pSpr_d->setOpacity(150.f);
				CCSprite * pSpr_CD = CCSprite::create(szName);
				CC_BREAK_IF(pSpr_CD==NULL);
				pSpr_CD->setColor(ccRED);

				//创建技能按钮
				CCMenuItemSprite * pBtn = CCMenuItemSprite::create(pSpr_n,pSpr_p,pSpr_d,this,menu_selector(CNFRockerLayer::OnBtnCallBack));
				CC_BREAK_IF(pBtn==NULL);
				pMenu->addChild(pBtn,enZOrderFront,i);
				if(i==enTagBtnSkill1)		pBtn->setPosition(ccp(pBtnCommon->getPositionX()+10,pBtnCommon->getPositionY()+90));
				else if(i==enTagBtnSkill2)	pBtn->setPosition(ccp(pBtnCommon->getPositionX()-110,pBtnCommon->getPositionY()-5));
				else if(i==enTagBtnSkill3)	pBtn->setPosition(ccp(pBtnCommon->getPositionX()-70,pBtnCommon->getPositionY()+70));
				else if(i==enTagBtnSkill4)	pBtn->setPosition(ccp(pBtnCommon->getPositionX()-200,pBtnCommon->getPositionY()-5));
				else if(i==enTagBtnSkill5)	pBtn->setPosition(ccp(pBtnCommon->getPositionX()+10,pBtnCommon->getPositionY()+170));

				//创建技能CD
				CCProgressTimer* pProgress = CCProgressTimer::create(pSpr_p);
				CC_BREAK_IF(pProgress==NULL);
				pProgress->setType(kCCProgressTimerTypeRadial);
				pProgress->setMidpoint(ccp(0.5f,0.5f));
				pProgress->setPercentage(0.f);
				pProgress->setPosition(pBtn->getPosition());
				this->addChild(pProgress,enZOrderFront+1,i+100);		//技能CD标签 = 技能标签 + 100

				//创建公共CD
				CCProgressTimer* pProgressCD = CCProgressTimer::create(pSpr_CD);
				CC_BREAK_IF(pProgressCD==NULL);
				pProgressCD->setType(kCCProgressTimerTypeRadial);
				pProgressCD->setMidpoint(ccp(0.5f,0.5f));
				pProgressCD->setPercentage(0.f);
				pProgressCD->setPosition(pBtn->getPosition());
				this->addChild(pProgressCD,enZOrderFront+2,i+200);		//技能CD标签 = 技能标签 + 200
				pProgressCD->setVisible(false);
			}
		}
		

		//更新函数
		schedule(schedule_selector(CNFRockerLayer::update));

		return true;
	} while (false);
	CCLog("Fun CNFRockerLayer::InitLayer Error!");
	return false;
}

//按钮回调
void CNFRockerLayer::OnBtnCallBack( CCObject * pObj )
{
	do 
	{
		//得到标签
		int nTag = (dynamic_cast<CCMenuItemSprite *>(pObj))->getTag();

		//主角攻击
		CNFProtagonist * pRole = dynamic_cast<CNFProtagonist *>(m_p3DLayer->getChildByTag(enTagProtagonist));
		CC_BREAK_IF(pRole==NULL);

		if (nTag==enTagBtnCommonAttack)		pRole->OnCtrlCommonAttack();
		else if (nTag==enTagBtnSkill1)		pRole->OnCtrlSkill(m_nSkill_1_ID);
		else if (nTag==enTagBtnSkill2)		pRole->OnCtrlSkill(m_nSkill_2_ID);
		else if (nTag==enTagBtnSkill3)		pRole->OnCtrlSkill(m_nSkill_3_ID);
		else if (nTag==enTagBtnSkill4)		pRole->OnCtrlSkill(m_nSkill_4_ID);
		else if (nTag==enTagBtnSkill5)		pRole->OnCtrlSkill(m_nSkill_5_ID);

		return ;
	} while (false);
	CCLog("Fun CNFRockerLayer::OnBtnCallBack Error!");
}

void CNFRockerLayer::update( float delta )
{
	do 
	{
		//得到主角
		CNFProtagonist * pPro = dynamic_cast<CNFProtagonist *>(m_p3DLayer->getChildByTag(enTagProtagonist));
		if (pPro!=NULL)
		{
			//得到摇杆
			CCSprite * pRockerBg = dynamic_cast<CCSprite *>(getChildByTag(enTagRockerBg));
			CC_BREAK_IF(pRockerBg==NULL);
			CCSprite * pRocker = dynamic_cast<CCSprite *>(getChildByTag(enTagRocker));
			CC_BREAK_IF(pRocker==NULL);

			//若正在触摸
			if (m_bIsTouching==true)
			{
				pRocker->setOpacity(255.f);
				pRockerBg->setOpacity(255.f);

				//触摸点与摇杆背景的距离
				float fRockerDisX = m_ptTouching.x - pRockerBg->getPositionX();
				float fRockerDisY = m_ptTouching.y - pRockerBg->getPositionY();
				float fLen = sqrt(pow(fRockerDisX,2)+pow(fRockerDisY,2));

				//触摸点与摇杆背景的角度
				float fRate = (float)CC_RADIANS_TO_DEGREES( atanf(fRockerDisX/fRockerDisY) );
				if(fRockerDisY<0)			fRate += 180.f;
				else if(fRockerDisX<0)		fRate += 360.f;

				//触摸点的实际位置
				CCPoint ptRockerMoveTo;
				if (fLen >= m_fRockerMaxDis)		fLen = m_fRockerMaxDis;
				ptRockerMoveTo.x = sin(fRate/180*3.1415)*fLen + pRockerBg->getPositionX();		
				ptRockerMoveTo.y = cos(fRate/180*3.1415)*fLen + pRockerBg->getPositionY();

				//触摸点与摇杆的距离
				float fDisX = ptRockerMoveTo.x - pRocker->getPositionX();
				float fDisY = ptRockerMoveTo.y - pRocker->getPositionY();
				float fLength = sqrt(pow(fDisX,2)+pow(fDisY,2));

				//出发每次移动的条件：距离至少大于速度。	防止颤动
				if (fLength > m_fRockerSpeed)
				{   
					//触摸点与摇杆的角度
					float fRateRocker = (float)CC_RADIANS_TO_DEGREES( atanf(fDisX/fDisY) );
					if(fDisY<0)				fRateRocker += 180.f;
					else if(fDisX<0)		fRateRocker += 360.f;

					ptRockerMoveTo.x = sin(fRateRocker/180*3.1415)*m_fRockerSpeed + pRocker->getPositionX();		
					ptRockerMoveTo.y = cos(fRateRocker/180*3.1415)*m_fRockerSpeed + pRocker->getPositionY();

					pRocker->setPosition(ptRockerMoveTo);
				}
				else	//距离小于速度，则直接命中
				{
					pRocker->setPosition(ptRockerMoveTo);
				}

				//主角移动
				pPro->OnCtrlMoveByRotation(fRate);

			}
			//若触摸结束
			else 
			{
				pRocker->setOpacity(100.f);
				pRockerBg->setOpacity(100.f);

				pPro->OnCtrlStop();

				//移动距离
				float fDisX = pRockerBg->getPositionX() - pRocker->getPositionX();
				float fDisY = pRockerBg->getPositionY() - pRocker->getPositionY();
				float fLen = sqrt(pow(fDisX,2)+pow(fDisY,2));

				//若距离大于速度，则移动
				if (fLen > m_fRockerSpeed)
				{  
					float at = (float)CC_RADIANS_TO_DEGREES( atanf(fDisX/fDisY) );
					if(fDisY<0)			at += 180.f;
					else if(fDisX<0)	at += 360.f;

					CCPoint ptRockerMoveTo;
					ptRockerMoveTo.x = sin(at/180*3.1415)*m_fRockerSpeed + pRocker->getPositionX();		
					ptRockerMoveTo.y = cos(at/180*3.1415)*m_fRockerSpeed + pRocker->getPositionY();
					pRocker->setPosition(ptRockerMoveTo);
				}
				else //若距离小雨速度，则直接命中
				{
					pRocker->setPosition(pRockerBg->getPosition());
				}			
			}

			//若为副本，则执行按钮逻辑
			if (m_nStageID >= _NF_TOWN_OR_BATTLE_ID_ && m_nStageID <_NF_TOWN_FB_ID_)
			{
				//得到菜单
				CCMenu * pMenu = dynamic_cast<CCMenu *>(getChildByTag(enTagMenu));
				CC_BREAK_IF(pMenu==NULL);

				//遍历按钮，同步CD
				for (int i=enTagBtnSkill1;i<=enTagBtnSkill5;i++)
				{
					//技能CD
					CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer *>(getChildByTag(i+100));
					CC_BREAK_IF(pProgress==NULL);

					//得到按钮
					CCMenuItemSprite * pBtn = dynamic_cast<CCMenuItemSprite *>(pMenu->getChildByTag(i));
					CC_BREAK_IF(pBtn==NULL);

					switch (i)
					{
					case enTagBtnSkill1:
						{
							//同步技能CD
							pProgress->setPercentage( 100*(float)pPro->GetSkillInfo_1().nSkill_CD_Time_Temp/(float)pPro->GetSkillInfo_1().nSkill_CD_Time );

							//判断按钮是否可按				
							if (pPro->GetSkillInfo_1().nSkill_CD_Time_Temp == 0 && m_bIsPublicCD==false)	pBtn->setEnabled(true);				
							else	pBtn->setEnabled(false);
						}break;
					case enTagBtnSkill2:
						{
							pProgress->setPercentage( 100*(float)pPro->GetSkillInfo_2().nSkill_CD_Time_Temp/(float)pPro->GetSkillInfo_2().nSkill_CD_Time );	
							if (pPro->GetSkillInfo_2().nSkill_CD_Time_Temp == 0 && m_bIsPublicCD==false)	pBtn->setEnabled(true);
							else	pBtn->setEnabled(false);
						}break;
					case enTagBtnSkill3:
						{
							pProgress->setPercentage( 100*(float)pPro->GetSkillInfo_3().nSkill_CD_Time_Temp/(float)pPro->GetSkillInfo_3().nSkill_CD_Time );			
							if (pPro->GetSkillInfo_3().nSkill_CD_Time_Temp == 0 && m_bIsPublicCD==false)	pBtn->setEnabled(true);
							else	pBtn->setEnabled(false);
						}break;
					case enTagBtnSkill4:
						{
							pProgress->setPercentage( 100*(float)pPro->GetSkillInfo_4().nSkill_CD_Time_Temp/(float)pPro->GetSkillInfo_4().nSkill_CD_Time );			
							if (pPro->GetSkillInfo_4().nSkill_CD_Time_Temp == 0 && m_bIsPublicCD==false)	pBtn->setEnabled(true);
							else	pBtn->setEnabled(false);
						}break;
					case enTagBtnSkill5:
						{
							pProgress->setPercentage( 100*(float)pPro->GetSkillInfo_5().nSkill_CD_Time_Temp/(float)pPro->GetSkillInfo_5().nSkill_CD_Time );			
							if (pPro->GetSkillInfo_5().nSkill_CD_Time_Temp == 0 && m_bIsPublicCD==false)	
								pBtn->setEnabled(true);
							else	
								pBtn->setEnabled(false);
						}break;
					}		
				}

				CCProgressTimer* pProgressCom = dynamic_cast<CCProgressTimer *>(getChildByTag(enTagCommonAttackCD));
				if(pProgressCom!=NULL){
					pProgressCom->setPercentage( 100*pPro->GetCommonAttackCDBFB());
				}

				CCLabelBMFont* pComonFont = dynamic_cast<CCLabelBMFont *>(getChildByTag(enTagCommonAttackFont));
				if(pComonFont!=NULL){
					if(pPro->GetCommonAttackStatus()!=0){
						pComonFont->setString(CCString::createWithFormat("%d",pPro->GetCommonAttackStatus())->m_sString.c_str());
					}
				}


				//判断是否有技能刚释放
				bool bIsSkillStart = false;
				if (pPro->GetSkillInfo_1().nSkill_CD_Time_Temp==pPro->GetSkillInfo_1().nSkill_CD_Time-1)	bIsSkillStart = true;
				if (pPro->GetSkillInfo_2().nSkill_CD_Time_Temp==pPro->GetSkillInfo_2().nSkill_CD_Time-1)	bIsSkillStart = true;
				if (pPro->GetSkillInfo_3().nSkill_CD_Time_Temp==pPro->GetSkillInfo_3().nSkill_CD_Time-1)	bIsSkillStart = true;
				if (pPro->GetSkillInfo_4().nSkill_CD_Time_Temp==pPro->GetSkillInfo_4().nSkill_CD_Time-1)	bIsSkillStart = true;
				if (pPro->GetSkillInfo_5().nSkill_CD_Time_Temp==pPro->GetSkillInfo_5().nSkill_CD_Time-1)	bIsSkillStart = true;

				//若有技能刚释放
				if (bIsSkillStart==true)
				{
					m_bIsPublicCD = true;
					this->runAction(CCSequence::create(CCDelayTime::create(1.f),CCCallFunc::create(this,callfunc_selector(CNFRockerLayer::OnSkillPublicCallBack)),NULL));

					for (int i=enTagBtnSkill1;i<=enTagBtnSkill5;i++)
					{	
						//判断是否显示公共CD
						bool bIsSkillPublicCD = false;

						if (i==enTagBtnSkill1 && pPro->GetSkillInfo_1().nSkill_CD_Time_Temp==0)			bIsSkillPublicCD = true;
						else if (i==enTagBtnSkill2 && pPro->GetSkillInfo_2().nSkill_CD_Time_Temp==0)	bIsSkillPublicCD = true;					
						else if (i==enTagBtnSkill3 && pPro->GetSkillInfo_3().nSkill_CD_Time_Temp==0)	bIsSkillPublicCD = true;
						else if (i==enTagBtnSkill4 && pPro->GetSkillInfo_4().nSkill_CD_Time_Temp==0)	bIsSkillPublicCD = true;
						else if (i==enTagBtnSkill5 && pPro->GetSkillInfo_5().nSkill_CD_Time_Temp==0)	bIsSkillPublicCD = true;

						//若显示公共CD
						if (bIsSkillPublicCD==true)
						{
							//显示公共CD
							CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer *>(getChildByTag(i+200));
							CC_BREAK_IF(pProgress==NULL);
							pProgress->setVisible(true);
							pProgress->setPercentage(99.f);	
							pProgress->runAction(CCSequence::create(CCProgressTo::create(_NF_PUBLIC_SKILL_CD_,0),NULL));	
						}
					}
				}
			}
		}

		return ;
	} while (false);
	CCLog("Fun CNFRockerLayer::update Error!");
}

void CNFRockerLayer::OnSkillPublicCallBack()
{
	do 
	{
		//遍历按钮，隐藏公共CD
		for (int i=enTagBtnSkill1;i<=enTagBtnSkill5;i++)
		{
			CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer *>(getChildByTag(i+200));
			CC_BREAK_IF(pProgress==NULL);
			pProgress->setVisible(false);	
		}

		//公共CD结束
		m_bIsPublicCD = false;

		return ;
	} while (false);
	CCLog("Fun CNFRockerLayer::OnSkillPublicCallBack Error!");
}

/************************************************************************/
/*				触摸                                                                     */
/************************************************************************/
bool CNFRockerLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//若父层为城镇，测需要判断是否有遮盖UI
		CNFTownScene *pCNFTownScene = dynamic_cast<CNFTownScene*>(getParent());
		if (pCNFTownScene)
		{
			if (pCNFTownScene->getChildByTag(CNFTownScene::enTagStudioCurrentChildUiLayer))return true;
		}
		


		//若点击在层外，则无效
		if ( this->boundingBox().containsPoint(pTouch->getLocation())==false )
			return false;

		//触摸开始
		m_bIsTouching = true;

		m_ptTouching = pTouch->getLocation();

		return true;
	} while (false);
	CCLog("Fun CNFRockerLayer::ccTouchBegan Error!");
	return false;
}
	
void CNFRockerLayer::ccTouchMoved( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//若父层为城镇，测需要判断是否有遮盖UI
		CNFTownScene *pCNFTownScene = dynamic_cast<CNFTownScene*>(getParent());
		if (pCNFTownScene)
		{
			if (pCNFTownScene->getChildByTag(CNFTownScene::enTagStudioCurrentChildUiLayer))return;
		}


		m_ptTouching = pTouch->getLocation();

		return ;
	} while (false);
	CCLog("Fun CNFRockerLayer::ccTouchMoved Error!");
}

void CNFRockerLayer::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{

	////若父层为城镇，测需要判断是否有遮盖UI
	//CNFTownScene *pCNFTownScene = dynamic_cast<CNFTownScene*>(getParent());
	//if (pCNFTownScene)
	//{
	//	if (pCNFTownScene->getChildByTag(CNFTownScene::enTagStudioCurrentChildUiLayer))return;
	//}

	//触摸结束
	m_bIsTouching = false;
}
	
void CNFRockerLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	CCLayer::onExit();
}


