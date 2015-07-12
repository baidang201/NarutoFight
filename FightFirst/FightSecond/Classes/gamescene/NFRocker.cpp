#include "NFRocker.h"
#include "../gameunit/person/NFProtagonist.h"
#include "../ui/NFTownScene.h"

#define		_NF_PUBLIC_SKILL_CD_		0.8f		//���ܹ���CDʱ�䣨�룩
#define		_NF_ROCKER_MAX_DIS_			50.f		//ҡ�����ƫ����
#define		_NF_ROCKER_SPEED_			3.f			//ҡ�˵��ٶ�

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
		//��ʼ������
		CC_BREAK_IF(CCLayerColor::initWithColor(ccc4(255,0,0,50))==false);

		//ע��
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this,-1,false);

		//���ò��С
		setContentSize(CCSizeMake(200,200));//(CCSizeMake(SCREEN_WIDTH*0.5f,SCREEN_HEIGHT));

		m_p3DLayer = pLayer;
		m_bIsTouching = false;
		m_bIsPublicCD = false;
		m_fRockerSpeed = _NF_ROCKER_SPEED_;		//ҡ�˵��ƶ��ٶ�
		m_fRockerMaxDis = _NF_ROCKER_MAX_DIS_;	//ҡ�����ƫ����
		m_nStageID = nStageID;

		//���Ǽ���ID
		CNFProtagonist * pPro = dynamic_cast<CNFProtagonist *>(pLayer->getChildByTag(enTagProtagonist));
		CC_BREAK_IF(pPro==NULL);
		m_nSkill_1_ID = pPro->GetSkillInfo_1().nSkill_SkillItemID;
		m_nSkill_2_ID = pPro->GetSkillInfo_2().nSkill_SkillItemID;
		m_nSkill_3_ID = pPro->GetSkillInfo_3().nSkill_SkillItemID;
		m_nSkill_4_ID = pPro->GetSkillInfo_4().nSkill_SkillItemID;
		m_nSkill_5_ID = pPro->GetSkillInfo_5().nSkill_SkillItemID;
		
		//����ҡ�˱���
		CCSprite * pRockerBg = CCSprite::create("gameui/rocker_bg.png");
		CC_BREAK_IF(pRockerBg==NULL);
		//pRockerBg->setScale(2.5f);
		pRockerBg->setPosition(ccp(100,100));
		addChild(pRockerBg,enZOrderFront,enTagRockerBg);
		pRockerBg->setOpacity(100.f);

		//����ҡ��
		CCSprite * pRocker = CCSprite::create("gameui/rocker_ball.png");
		CC_BREAK_IF(pRocker==NULL);
		pRocker->setPosition(pRockerBg->getPosition());
		addChild(pRocker,enZOrderFront+1,enTagRocker);
		pRocker->setOpacity(100.f);

		//��Ϊ�������򴴽���ť
		if (m_nStageID >= _NF_TOWN_OR_BATTLE_ID_ && m_nStageID <_NF_TOWN_FB_ID_)
		{
			/************************************************************************/
			/*				������ť                                                                     */
			/************************************************************************/
			//�����˵�
			CCMenu * pMenu = CCMenu::create();
			CC_BREAK_IF(pMenu==NULL);
			pMenu->setPosition(CCPointZero);
			addChild(pMenu,enZOrderFront,enTagMenu);

			//������ͨ������ť
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
			
			//������ͨ����CD
			CCProgressTimer* pProgressCommonAttackCD = CCProgressTimer::create(pCommon3);
			CC_BREAK_IF(pProgressCommonAttackCD==NULL);
			pProgressCommonAttackCD->setType(kCCProgressTimerTypeRadial);
			pProgressCommonAttackCD->setMidpoint(ccp(0.5f,0.5f));
			pProgressCommonAttackCD->setPercentage(0.f);
			pProgressCommonAttackCD->setPosition(pBtnCommon->getPosition());
			this->addChild(pProgressCommonAttackCD,enZOrderFront+1,enTagCommonAttackCD);

			//�������ܰ�ť
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

				//�������ܰ�ť
				CCMenuItemSprite * pBtn = CCMenuItemSprite::create(pSpr_n,pSpr_p,pSpr_d,this,menu_selector(CNFRockerLayer::OnBtnCallBack));
				CC_BREAK_IF(pBtn==NULL);
				pMenu->addChild(pBtn,enZOrderFront,i);
				if(i==enTagBtnSkill1)		pBtn->setPosition(ccp(pBtnCommon->getPositionX()+10,pBtnCommon->getPositionY()+90));
				else if(i==enTagBtnSkill2)	pBtn->setPosition(ccp(pBtnCommon->getPositionX()-110,pBtnCommon->getPositionY()-5));
				else if(i==enTagBtnSkill3)	pBtn->setPosition(ccp(pBtnCommon->getPositionX()-70,pBtnCommon->getPositionY()+70));
				else if(i==enTagBtnSkill4)	pBtn->setPosition(ccp(pBtnCommon->getPositionX()-200,pBtnCommon->getPositionY()-5));
				else if(i==enTagBtnSkill5)	pBtn->setPosition(ccp(pBtnCommon->getPositionX()+10,pBtnCommon->getPositionY()+170));

				//��������CD
				CCProgressTimer* pProgress = CCProgressTimer::create(pSpr_p);
				CC_BREAK_IF(pProgress==NULL);
				pProgress->setType(kCCProgressTimerTypeRadial);
				pProgress->setMidpoint(ccp(0.5f,0.5f));
				pProgress->setPercentage(0.f);
				pProgress->setPosition(pBtn->getPosition());
				this->addChild(pProgress,enZOrderFront+1,i+100);		//����CD��ǩ = ���ܱ�ǩ + 100

				//��������CD
				CCProgressTimer* pProgressCD = CCProgressTimer::create(pSpr_CD);
				CC_BREAK_IF(pProgressCD==NULL);
				pProgressCD->setType(kCCProgressTimerTypeRadial);
				pProgressCD->setMidpoint(ccp(0.5f,0.5f));
				pProgressCD->setPercentage(0.f);
				pProgressCD->setPosition(pBtn->getPosition());
				this->addChild(pProgressCD,enZOrderFront+2,i+200);		//����CD��ǩ = ���ܱ�ǩ + 200
				pProgressCD->setVisible(false);
			}
		}
		

		//���º���
		schedule(schedule_selector(CNFRockerLayer::update));

		return true;
	} while (false);
	CCLog("Fun CNFRockerLayer::InitLayer Error!");
	return false;
}

//��ť�ص�
void CNFRockerLayer::OnBtnCallBack( CCObject * pObj )
{
	do 
	{
		//�õ���ǩ
		int nTag = (dynamic_cast<CCMenuItemSprite *>(pObj))->getTag();

		//���ǹ���
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
		//�õ�����
		CNFProtagonist * pPro = dynamic_cast<CNFProtagonist *>(m_p3DLayer->getChildByTag(enTagProtagonist));
		if (pPro!=NULL)
		{
			//�õ�ҡ��
			CCSprite * pRockerBg = dynamic_cast<CCSprite *>(getChildByTag(enTagRockerBg));
			CC_BREAK_IF(pRockerBg==NULL);
			CCSprite * pRocker = dynamic_cast<CCSprite *>(getChildByTag(enTagRocker));
			CC_BREAK_IF(pRocker==NULL);

			//�����ڴ���
			if (m_bIsTouching==true)
			{
				pRocker->setOpacity(255.f);
				pRockerBg->setOpacity(255.f);

				//��������ҡ�˱����ľ���
				float fRockerDisX = m_ptTouching.x - pRockerBg->getPositionX();
				float fRockerDisY = m_ptTouching.y - pRockerBg->getPositionY();
				float fLen = sqrt(pow(fRockerDisX,2)+pow(fRockerDisY,2));

				//��������ҡ�˱����ĽǶ�
				float fRate = (float)CC_RADIANS_TO_DEGREES( atanf(fRockerDisX/fRockerDisY) );
				if(fRockerDisY<0)			fRate += 180.f;
				else if(fRockerDisX<0)		fRate += 360.f;

				//�������ʵ��λ��
				CCPoint ptRockerMoveTo;
				if (fLen >= m_fRockerMaxDis)		fLen = m_fRockerMaxDis;
				ptRockerMoveTo.x = sin(fRate/180*3.1415)*fLen + pRockerBg->getPositionX();		
				ptRockerMoveTo.y = cos(fRate/180*3.1415)*fLen + pRockerBg->getPositionY();

				//��������ҡ�˵ľ���
				float fDisX = ptRockerMoveTo.x - pRocker->getPositionX();
				float fDisY = ptRockerMoveTo.y - pRocker->getPositionY();
				float fLength = sqrt(pow(fDisX,2)+pow(fDisY,2));

				//����ÿ���ƶ����������������ٴ����ٶȡ�	��ֹ����
				if (fLength > m_fRockerSpeed)
				{   
					//��������ҡ�˵ĽǶ�
					float fRateRocker = (float)CC_RADIANS_TO_DEGREES( atanf(fDisX/fDisY) );
					if(fDisY<0)				fRateRocker += 180.f;
					else if(fDisX<0)		fRateRocker += 360.f;

					ptRockerMoveTo.x = sin(fRateRocker/180*3.1415)*m_fRockerSpeed + pRocker->getPositionX();		
					ptRockerMoveTo.y = cos(fRateRocker/180*3.1415)*m_fRockerSpeed + pRocker->getPositionY();

					pRocker->setPosition(ptRockerMoveTo);
				}
				else	//����С���ٶȣ���ֱ������
				{
					pRocker->setPosition(ptRockerMoveTo);
				}

				//�����ƶ�
				pPro->OnCtrlMoveByRotation(fRate);

			}
			//����������
			else 
			{
				pRocker->setOpacity(100.f);
				pRockerBg->setOpacity(100.f);

				pPro->OnCtrlStop();

				//�ƶ�����
				float fDisX = pRockerBg->getPositionX() - pRocker->getPositionX();
				float fDisY = pRockerBg->getPositionY() - pRocker->getPositionY();
				float fLen = sqrt(pow(fDisX,2)+pow(fDisY,2));

				//����������ٶȣ����ƶ�
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
				else //������С���ٶȣ���ֱ������
				{
					pRocker->setPosition(pRockerBg->getPosition());
				}			
			}

			//��Ϊ��������ִ�а�ť�߼�
			if (m_nStageID >= _NF_TOWN_OR_BATTLE_ID_ && m_nStageID <_NF_TOWN_FB_ID_)
			{
				//�õ��˵�
				CCMenu * pMenu = dynamic_cast<CCMenu *>(getChildByTag(enTagMenu));
				CC_BREAK_IF(pMenu==NULL);

				//������ť��ͬ��CD
				for (int i=enTagBtnSkill1;i<=enTagBtnSkill5;i++)
				{
					//����CD
					CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer *>(getChildByTag(i+100));
					CC_BREAK_IF(pProgress==NULL);

					//�õ���ť
					CCMenuItemSprite * pBtn = dynamic_cast<CCMenuItemSprite *>(pMenu->getChildByTag(i));
					CC_BREAK_IF(pBtn==NULL);

					switch (i)
					{
					case enTagBtnSkill1:
						{
							//ͬ������CD
							pProgress->setPercentage( 100*(float)pPro->GetSkillInfo_1().nSkill_CD_Time_Temp/(float)pPro->GetSkillInfo_1().nSkill_CD_Time );

							//�жϰ�ť�Ƿ�ɰ�				
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


				//�ж��Ƿ��м��ܸ��ͷ�
				bool bIsSkillStart = false;
				if (pPro->GetSkillInfo_1().nSkill_CD_Time_Temp==pPro->GetSkillInfo_1().nSkill_CD_Time-1)	bIsSkillStart = true;
				if (pPro->GetSkillInfo_2().nSkill_CD_Time_Temp==pPro->GetSkillInfo_2().nSkill_CD_Time-1)	bIsSkillStart = true;
				if (pPro->GetSkillInfo_3().nSkill_CD_Time_Temp==pPro->GetSkillInfo_3().nSkill_CD_Time-1)	bIsSkillStart = true;
				if (pPro->GetSkillInfo_4().nSkill_CD_Time_Temp==pPro->GetSkillInfo_4().nSkill_CD_Time-1)	bIsSkillStart = true;
				if (pPro->GetSkillInfo_5().nSkill_CD_Time_Temp==pPro->GetSkillInfo_5().nSkill_CD_Time-1)	bIsSkillStart = true;

				//���м��ܸ��ͷ�
				if (bIsSkillStart==true)
				{
					m_bIsPublicCD = true;
					this->runAction(CCSequence::create(CCDelayTime::create(1.f),CCCallFunc::create(this,callfunc_selector(CNFRockerLayer::OnSkillPublicCallBack)),NULL));

					for (int i=enTagBtnSkill1;i<=enTagBtnSkill5;i++)
					{	
						//�ж��Ƿ���ʾ����CD
						bool bIsSkillPublicCD = false;

						if (i==enTagBtnSkill1 && pPro->GetSkillInfo_1().nSkill_CD_Time_Temp==0)			bIsSkillPublicCD = true;
						else if (i==enTagBtnSkill2 && pPro->GetSkillInfo_2().nSkill_CD_Time_Temp==0)	bIsSkillPublicCD = true;					
						else if (i==enTagBtnSkill3 && pPro->GetSkillInfo_3().nSkill_CD_Time_Temp==0)	bIsSkillPublicCD = true;
						else if (i==enTagBtnSkill4 && pPro->GetSkillInfo_4().nSkill_CD_Time_Temp==0)	bIsSkillPublicCD = true;
						else if (i==enTagBtnSkill5 && pPro->GetSkillInfo_5().nSkill_CD_Time_Temp==0)	bIsSkillPublicCD = true;

						//����ʾ����CD
						if (bIsSkillPublicCD==true)
						{
							//��ʾ����CD
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
		//������ť�����ع���CD
		for (int i=enTagBtnSkill1;i<=enTagBtnSkill5;i++)
		{
			CCProgressTimer* pProgress = dynamic_cast<CCProgressTimer *>(getChildByTag(i+200));
			CC_BREAK_IF(pProgress==NULL);
			pProgress->setVisible(false);	
		}

		//����CD����
		m_bIsPublicCD = false;

		return ;
	} while (false);
	CCLog("Fun CNFRockerLayer::OnSkillPublicCallBack Error!");
}

/************************************************************************/
/*				����                                                                     */
/************************************************************************/
bool CNFRockerLayer::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//������Ϊ���򣬲���Ҫ�ж��Ƿ����ڸ�UI
		CNFTownScene *pCNFTownScene = dynamic_cast<CNFTownScene*>(getParent());
		if (pCNFTownScene)
		{
			if (pCNFTownScene->getChildByTag(CNFTownScene::enTagStudioCurrentChildUiLayer))return true;
		}
		


		//������ڲ��⣬����Ч
		if ( this->boundingBox().containsPoint(pTouch->getLocation())==false )
			return false;

		//������ʼ
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
		//������Ϊ���򣬲���Ҫ�ж��Ƿ����ڸ�UI
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

	////������Ϊ���򣬲���Ҫ�ж��Ƿ����ڸ�UI
	//CNFTownScene *pCNFTownScene = dynamic_cast<CNFTownScene*>(getParent());
	//if (pCNFTownScene)
	//{
	//	if (pCNFTownScene->getChildByTag(CNFTownScene::enTagStudioCurrentChildUiLayer))return;
	//}

	//��������
	m_bIsTouching = false;
}
	
void CNFRockerLayer::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);

	CCLayer::onExit();
}


