#include "NFRoleSelectLayer.h"
#include "NFTownScene.h"
#include "NFLoginScene.h"
#include "datamanager/NFServerDataManager.h"
/************************************************************************/
/*人物选择层															*/
/************************************************************************/

CCScene * CNFRoleSelectLayer::scene()
{
	do 
	{
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		CNFRoleSelectLayer* pLayer = CNFRoleSelectLayer::CreateLayer();
		CC_BREAK_IF(pLayer==NULL);

		pScene->addChild(pLayer);
		return pScene;

	} while (false);
	CCLog("Fun CNFRoleSelectLayer::scene Error!");
	return NULL;
}

CNFRoleSelectLayer * CNFRoleSelectLayer::CreateLayer()
{
	CNFRoleSelectLayer *pRet = new CNFRoleSelectLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CCLog("Fun CNFRoleSelectLayer::CreateLayer Error!");
	delete pRet; 
	pRet = NULL; 
	return NULL;
}

bool CNFRoleSelectLayer::init()
{
	do 
	{
		//初始化父类
		CC_BREAK_IF(CCLayer::init()==false);

		m_nRoleID = 0;
		m_nRelaseRoleID = 0;
		//创建主场景UI层
		UILayer* pUiLayer = UILayer::create(); 
		CC_BREAK_IF(pUiLayer==NULL);

		//-------------------加入主场景UI----------------------------
		UILayout *pBg = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Character_Select_Bg_NF.json"));
		CC_BREAK_IF(pBg==NULL);
		addChild(pBg,enZOrderBack);
		pBg->setSize(getContentSize());


		UILayout *pCharacter_Select = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("Character_Select_NF.json"));
		CC_BREAK_IF(pCharacter_Select==NULL);
		pUiLayer->addWidget(pCharacter_Select);
		pCharacter_Select->setName("Character_Select_Layer");
		pCharacter_Select->setSize(getContentSize());

		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/xiaoyin.ExportJson");
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/NewProject.ExportJson");
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/minren1.ExportJson");

		//设置开始游戏回调
		UIButton* pBtn_Start_Game = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName("Btn_Start_Game"));
		CC_BREAK_IF(pBtn_Start_Game==NULL);
		pBtn_Start_Game->addTouchEventListener(this,toucheventselector(CNFRoleSelectLayer::OnGameStartPage));


		//读取角色信息
		CNFServerRoleSelectInfoVec RoleSelectInfoVec;
		CNFServerDataManager::SharedData()->GetRoleSelectInfo(RoleSelectInfoVec);
		CNFServerRoleSelectInfoVec::iterator itRole=RoleSelectInfoVec.begin();

		char cName[3][256]={"minren1","xiaoyin","NewProject"};
		for (int i=0;itRole!=RoleSelectInfoVec.end();itRole++,i++)
		{
			UIWidget* pChar=dynamic_cast<UIWidget*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString()));
			CC_BREAK_IF(pChar==NULL);


			UIButton* pBtn_Main_Role = dynamic_cast<UIButton*>(pChar->getChildByName("Btn_Main_Role"));
			CC_BREAK_IF(pBtn_Main_Role==NULL);
			pBtn_Main_Role->addTouchEventListener(this,toucheventselector(CNFRoleSelectLayer::OnRoleSelect));
			pBtn_Main_Role->setEnabled(false);

			UIButton* pBtnCancel_Follow = dynamic_cast<UIButton*>(pChar->getChildByName("Btn_Cancel_Follow"));
			CC_BREAK_IF(pBtnCancel_Follow==NULL);
			pBtnCancel_Follow->setEnabled(false);
			pBtnCancel_Follow->addTouchEventListener(this,toucheventselector(CNFRoleSelectLayer::OnCancelFollowSelect));

			UIButton* pBtnFollow = dynamic_cast<UIButton*>(pChar->getChildByName("Btn_Follow"));
			CC_BREAK_IF(pBtnFollow==NULL);
			pBtnFollow->setEnabled(false);
			pBtnFollow->addTouchEventListener(this,toucheventselector(CNFRoleSelectLayer::OnFollowSelect));

			CCArmature *pRole = CCArmature::create(cName[i]);
			CC_BREAK_IF(pRole==NULL);
			pRole->setPosition(ccp(pChar->getPositionX()+SCREEN_WIDTH/2,pChar->getPositionY()-100));
			pRole->setAnchorPoint(ccp(pRole->getAnchorPoint().x,0));
			pRole->setScale(1.5f);
			pRole->getAnimation()->play("hold",-1,-1,-1,10000);
			addChild(pRole,enZOrderFront,enTagRole1+i);

			UIButton* pBtn_Release_Lock = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Btn_Release_Lock"));
			CC_BREAK_IF(pBtn_Release_Lock==NULL);
			pBtn_Release_Lock->addTouchEventListener(this,toucheventselector(CNFRoleSelectLayer::OnShowReleaseDlg));


			if (itRole->nState==0)//空闲
			{
				pBtn_Release_Lock->setEnabled(false);
				pBtn_Main_Role->setEnabled(true);
				pBtn_Main_Role->setVisible(true);
				pBtnFollow->setEnabled(true);
				pBtnFollow->setVisible(true);
			}

			if (itRole->nState==1)//出战
			{
				pBtn_Release_Lock->setEnabled(false);
				pBtn_Main_Role->setEnabled(false);
				pBtn_Main_Role->setVisible(false);

				//获取按下图
				UIImageView* pBtn_Select_Img=dynamic_cast<UIImageView*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName("Select_Main_Role_Img"));
				CC_BREAK_IF(pBtn_Select_Img==NULL);

				pBtn_Select_Img->setVisible(true);
				pBtn_Select_Img->setPosition(pChar->getPosition());
				pBtn_Start_Game->setVisible(true);

				m_nRoleID=itRole->nRoleID;
			}

			if (itRole->nState==2)//助战
			{
				pBtn_Release_Lock->setEnabled(false);
				pBtn_Main_Role->setEnabled(true);
				pBtn_Main_Role->setVisible(true);
				pBtnCancel_Follow->setEnabled(true);
				pBtnCancel_Follow->setVisible(true);

				//获取按下图
				UIImageView* pBtn_Select_Img=dynamic_cast<UIImageView*>(pChar->getChildByName("Select_Follow_Img"));
				CC_BREAK_IF(pBtn_Select_Img==NULL);

				pBtn_Select_Img->setVisible(true);
			}

		}





		//设置返回开始界面回调
		UIButton* pBtn_Return = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName("Btn_Return"));
		CC_BREAK_IF(pBtn_Return==NULL);
		pBtn_Return->addTouchEventListener(this,toucheventselector(CNFRoleSelectLayer::OnBack));


		//隐藏解锁对话框
		UIImageView *pReleaseDlg = dynamic_cast<UIImageView*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName("ReleaseDlg"));
		CC_BREAK_IF(pReleaseDlg==NULL);
		pReleaseDlg->setEnabled(false);
		pReleaseDlg->setVisible(false);

		UIButton *pCloseDlg=dynamic_cast<UIButton*>(pReleaseDlg->getChildByName("Btn_Close"));
		CC_BREAK_IF(pCloseDlg==NULL);
		pCloseDlg->addTouchEventListener(this,toucheventselector(CNFRoleSelectLayer::OnHideReleaseDlg));


		UIButton *pBtn_Sure=dynamic_cast<UIButton*>(pReleaseDlg->getChildByName("Btn_Sure"));
		CC_BREAK_IF(pBtn_Sure==NULL);
		pBtn_Sure->addTouchEventListener(this,toucheventselector(CNFRoleSelectLayer::OnBtnSureRelease));

		addChild(pUiLayer,enZOrderFront,enTagStudioMainUiLayer);





		return true;
	} while (false);
	CCLog("Fun CNFRoleSelectLayer::init Error!");
	return false;
}

void CNFRoleSelectLayer::OnGameStartPage( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				//跳转到城镇
				CCScene * pScene = CNFTownScene::CreateTownScene(1,m_nRoleID);
				CC_BREAK_IF(pScene==NULL);
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));
				
			}break;
		default:
			break;
		}
		return;
	} while (false);
	CCLog("CNFRoleSelectLayer::OnGameStartPage Error");
}

void CNFRoleSelectLayer::OnRoleSelect( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				UILayer* pMainUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
				CC_BREAK_IF(pMainUiLayer==NULL);

				UILayout *pCharacter_Select = dynamic_cast<UILayout*>	(pMainUiLayer->getWidgetByName("Character_Select_Layer"));
				CC_BREAK_IF(pCharacter_Select==NULL);

				for (int i=0;i<3;i++)
				{

					UIButton* pBtn_Release_Lock = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Btn_Release_Lock"));
					CC_BREAK_IF(pBtn_Release_Lock==NULL);
					if(pBtn_Release_Lock->isEnabled())continue;

					UIButton* pBtnSelect = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Btn_Main_Role"));
					CC_BREAK_IF(pBtnSelect==NULL);
					pBtnSelect->setVisible(true);
					pBtnSelect->setEnabled(true);

					UIButton* pBtnFollow = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Btn_Follow"));
					CC_BREAK_IF(pBtnFollow==NULL);
					pBtnFollow->setVisible(true);
					pBtnFollow->setEnabled(true);

					UIButton* pBtnCancel_Follow = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Btn_Cancel_Follow"));
					CC_BREAK_IF(pBtnCancel_Follow==NULL);
					pBtnCancel_Follow->setEnabled(false);
					pBtnCancel_Follow->setVisible(false);

					//获取按下图
					UIImageView* pBtn_Select_Img=dynamic_cast<UIImageView*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Select_Follow_Img"));
					CC_BREAK_IF(pBtn_Select_Img==NULL);
					pBtn_Select_Img->setVisible(false);

				}



				UIButton* pBtn = dynamic_cast<UIButton*>(pSender);
				CC_BREAK_IF(pBtn==NULL);
				pBtn->setVisible(false);
			

				UIWidget* pUiLayer = dynamic_cast<UIWidget*>(pBtn->getParent()->getParent());
				CC_BREAK_IF(pUiLayer==NULL);

				UIWidget* pChar = dynamic_cast<UIWidget*>(pBtn->getParent());
				CC_BREAK_IF(pChar==NULL);


				//判断选择的那个角色
				if (strcmp("Char_1",pChar->getName())==0)	m_nRoleID = 1;	//鸣人
				else if (strcmp("Char_2",pChar->getName())==0)	m_nRoleID = 2;	//小樱
				else if (strcmp("Char_3",pChar->getName())==0)	m_nRoleID = 3;	//佐助
				//CCLog("%d",m_nRoleID);

				for (int i=0;i<3;i++)
				{
					CCArmature *pRole = dynamic_cast<CCArmature*>(getChildByTag(i+enTagRole1));
					CC_BREAK_IF(pRole==NULL);

					if (pRole->getPositionX()-SCREEN_WIDTH/2==pChar->getPositionX())
					{
						
						
						if (strcmp("Char_1",pChar->getName())==0)	pRole->getAnimation()->play("skill_2",-1,-1,-1,10000);	//鸣人
						else if (strcmp("Char_2",pChar->getName())==0)	pRole->getAnimation()->play("skill_3",-1,-1,-1,10000);	//小樱
						else if (strcmp("Char_3",pChar->getName())==0)	pRole->getAnimation()->play("skill_5",-1,-1,-1,10000);	//佐助
					}
					else
					{
						pRole->getAnimation()->play("hold",-1,-1,-1,10000);
					}
				}


				UIButton* pBtnFollowSelect = dynamic_cast<UIButton*>(pChar->getChildByName("Btn_Follow"));
				CC_BREAK_IF(pBtnFollowSelect==NULL);
				pBtnFollowSelect->setEnabled(false);
				pBtnFollowSelect->setVisible(false);

				//获取按下图
				UIImageView* pBtn_Select_Img=dynamic_cast<UIImageView*>(pUiLayer->getChildByName("Select_Main_Role_Img"));
				CC_BREAK_IF(pBtn_Select_Img==NULL);

				pBtn_Select_Img->setVisible(true);
				pBtn_Select_Img->setPosition(pChar->getPosition());


				//设置开始游戏回调
				UIButton* pBtn_Start_Game = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName("Btn_Start_Game"));
				CC_BREAK_IF(pBtn_Start_Game==NULL);
				pBtn_Start_Game->setVisible(true);

			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFRoleSelectLayer::OnRoleSelect Error");
}

void CNFRoleSelectLayer::OnBack( CCObject *pSender, TouchEventType type )
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
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFRoleSelectLayer::OnBack Error");
}

void CNFRoleSelectLayer::OnFollowSelect( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				//UILayer* pMainUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
				//CC_BREAK_IF(pMainUiLayer==NULL);

				//UILayout *pCharacter_Select = dynamic_cast<UILayout*>	(pMainUiLayer->getWidgetByName("Character_Select_Layer"));
				//CC_BREAK_IF(pCharacter_Select==NULL);

				//for (int i=0;i<3;i++)
				//{

				//	UIButton* pBtnSelect = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Btn_Main_Role"));
				//	CC_BREAK_IF(pBtnSelect==NULL);
				//	pBtnSelect->setVisible(true);

				//	UIButton* pBtnFollow = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Btn_Follow"));
				//	CC_BREAK_IF(pBtnFollow==NULL);
				//	pBtnFollow->setVisible(true);

				//}



				UIButton* pBtn = dynamic_cast<UIButton*>(pSender);
				CC_BREAK_IF(pBtn==NULL);
				pBtn->setVisible(false);
				pBtn->setEnabled(false);

				UIWidget* pChar = dynamic_cast<UIWidget*>(pBtn->getParent());
				CC_BREAK_IF(pChar==NULL);

				UIButton* pBtn_Cancel_Follow = dynamic_cast<UIButton*>(pChar->getChildByName("Btn_Cancel_Follow"));
				CC_BREAK_IF(pBtn_Cancel_Follow==NULL);

				pBtn_Cancel_Follow->setVisible(true);
				pBtn_Cancel_Follow->setEnabled(true);
				//获取按下图
				UIImageView* pBtn_Select_Img=dynamic_cast<UIImageView*>(pChar->getChildByName("Select_Follow_Img"));
				CC_BREAK_IF(pBtn_Select_Img==NULL);

				pBtn_Select_Img->setVisible(true);




			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFRoleSelectLayer::OnFollowSelect Error");
}

void CNFRoleSelectLayer::OnCancelFollowSelect( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				//UILayer* pMainUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
				//CC_BREAK_IF(pMainUiLayer==NULL);

				//UILayout *pCharacter_Select = dynamic_cast<UILayout*>	(pMainUiLayer->getWidgetByName("Character_Select_Layer"));
				//CC_BREAK_IF(pCharacter_Select==NULL);

				//for (int i=0;i<3;i++)
				//{

				//	UIButton* pBtnSelect = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Btn_Main_Role"));
				//	CC_BREAK_IF(pBtnSelect==NULL);
				//	pBtnSelect->setVisible(true);

				//	UIButton* pBtnFollow = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",i+1)->getCString())->getChildByName("Btn_Follow"));
				//	CC_BREAK_IF(pBtnFollow==NULL);
				//	pBtnFollow->setVisible(true);

				//}



				UIButton* pBtn = dynamic_cast<UIButton*>(pSender);
				CC_BREAK_IF(pBtn==NULL);
				pBtn->setVisible(false);
				pBtn->setEnabled(false);

				UIWidget* pChar = dynamic_cast<UIWidget*>(pBtn->getParent());
				CC_BREAK_IF(pChar==NULL);

				UIButton* pBtn_Follow = dynamic_cast<UIButton*>(pChar->getChildByName("Btn_Follow"));
				CC_BREAK_IF(pBtn_Follow==NULL);

				pBtn_Follow->setVisible(true);
				pBtn_Follow->setEnabled(true);
				//获取按下图
				UIImageView* pBtn_Select_Img=dynamic_cast<UIImageView*>(pChar->getChildByName("Select_Follow_Img"));
				CC_BREAK_IF(pBtn_Select_Img==NULL);

				pBtn_Select_Img->setVisible(false);




			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFRoleSelectLayer::OnCancelFollowSelect Error");
}

void CNFRoleSelectLayer::OnShowReleaseDlg( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				UILayer* pMainUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
				CC_BREAK_IF(pMainUiLayer==NULL);

				UILayout *pCharacter_Select = dynamic_cast<UILayout*>	(pMainUiLayer->getWidgetByName("Character_Select_Layer"));
				CC_BREAK_IF(pCharacter_Select==NULL);



				UIButton* pBtn = dynamic_cast<UIButton*>(pSender);
				CC_BREAK_IF(pBtn==NULL);


				UIWidget* pChar = dynamic_cast<UIWidget*>(pBtn->getParent());
				CC_BREAK_IF(pChar==NULL);


				//隐藏解锁对话框
				UIImageView *pReleaseDlg = dynamic_cast<UIImageView*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName("ReleaseDlg"));
				CC_BREAK_IF(pReleaseDlg==NULL);
				pReleaseDlg->setEnabled(true);
				pReleaseDlg->setVisible(true);

				UILabel* pCharName = dynamic_cast<UILabel*>(pChar->getChildByName("Char_Info")->getChildByName("Name_Lable"));
				CC_BREAK_IF(pCharName==NULL);


				UILabel *pName_Lable = dynamic_cast<UILabel*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName("ReleaseDlg")->getChildByName("Name_Lable"));
				CC_BREAK_IF(pName_Lable==NULL);
				pName_Lable->setText(pCharName->getStringValue());
		

				//判断选择的那个角色
				if (strcmp("Char_1",pChar->getName())==0)		//鸣人
				{
					m_nRelaseRoleID=1;
				}
				else if (strcmp("Char_2",pChar->getName())==0)	//小樱
				{
					m_nRelaseRoleID=2;
				}
				else if (strcmp("Char_3",pChar->getName())==0)	//佐助
				{
					m_nRelaseRoleID=3;
				}



			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFRoleSelectLayer::OnShowReleaseDlg Error");
}

void CNFRoleSelectLayer::OnBtnSureRelease( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{
				if(m_nRelaseRoleID==0)return;


				UILayer* pMainUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
				CC_BREAK_IF(pMainUiLayer==NULL);

				UILayout *pCharacter_Select = dynamic_cast<UILayout*>	(pMainUiLayer->getWidgetByName("Character_Select_Layer"));
				CC_BREAK_IF(pCharacter_Select==NULL);


				//隐藏解锁对话框
				UIImageView *pReleaseDlg = dynamic_cast<UIImageView*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName("ReleaseDlg"));
				CC_BREAK_IF(pReleaseDlg==NULL);
				pReleaseDlg->setEnabled(false);
				pReleaseDlg->setVisible(false);


				//读取角色信息
				CNFServerRoleSelectInfoVec RoleSelectInfoVec;
				CNFServerDataManager::SharedData()->GetRoleSelectInfo(RoleSelectInfoVec);
				CNFServerRoleSelectInfoVec::iterator itRole=RoleSelectInfoVec.begin();

				for (;itRole!=RoleSelectInfoVec.end();itRole++)
				{
					if (itRole->nRoleID==m_nRelaseRoleID)
					{
						itRole->nState=0;
					}
				}

				CNFServerDataManager::SharedData()->SetRoleSelectInfo(RoleSelectInfoVec);




				UIButton* pBtn_Release_Lock = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",m_nRelaseRoleID)->getCString())->getChildByName("Btn_Release_Lock"));
				CC_BREAK_IF(pBtn_Release_Lock==NULL);
				pBtn_Release_Lock->setEnabled(false);
				pBtn_Release_Lock->setVisible(false);

				UIButton* pBtnSelect = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",m_nRelaseRoleID)->getCString())->getChildByName("Btn_Main_Role"));
				CC_BREAK_IF(pBtnSelect==NULL);
				pBtnSelect->setVisible(true);
				pBtnSelect->setEnabled(true);

				UIButton* pBtnFollow = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",m_nRelaseRoleID)->getCString())->getChildByName("Btn_Follow"));
				CC_BREAK_IF(pBtnFollow==NULL);
				pBtnFollow->setVisible(true);
				pBtnFollow->setEnabled(true);

				UIButton* pBtnCancel_Follow = dynamic_cast<UIButton*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",m_nRelaseRoleID)->getCString())->getChildByName("Btn_Cancel_Follow"));
				CC_BREAK_IF(pBtnCancel_Follow==NULL);
				pBtnCancel_Follow->setEnabled(false);
				pBtnCancel_Follow->setVisible(false);

				//获取按下图
				UIImageView* pBtn_Select_Img=dynamic_cast<UIImageView*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName(CCString::createWithFormat("Char_%d",m_nRelaseRoleID)->getCString())->getChildByName("Select_Follow_Img"));
				CC_BREAK_IF(pBtn_Select_Img==NULL);
				pBtn_Select_Img->setVisible(false);

				


			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFRoleSelectLayer::OnHideReleaseDlg Error");
}

void CNFRoleSelectLayer::OnHideReleaseDlg( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				UILayer* pMainUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
				CC_BREAK_IF(pMainUiLayer==NULL);

				UILayout *pCharacter_Select = dynamic_cast<UILayout*>	(pMainUiLayer->getWidgetByName("Character_Select_Layer"));
				CC_BREAK_IF(pCharacter_Select==NULL);


				//隐藏解锁对话框
				UIImageView *pReleaseDlg = dynamic_cast<UIImageView*>(pCharacter_Select->getChildByName("Main_Ui")->getChildByName("ReleaseDlg"));
				CC_BREAK_IF(pReleaseDlg==NULL);
				pReleaseDlg->setEnabled(false);





			}break;
		default:
			break;
		}

		return;
	} while (false);
	CCLog("CNFRoleSelectLayer::OnHideReleaseDlg Error");
}

