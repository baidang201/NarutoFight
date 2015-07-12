#include "NFRoleSelectLayer.h"
#include "NFTownScene.h"
#include "NFLoginScene.h"
#include "NFCreateTeamLayer.h"
#include "datamanager/NFServerDataManager.h"
/************************************************************************/
/*创建忍队层															*/
/************************************************************************/

CCScene * CNFTeamCreateLayer::scene()
{
	do 
	{
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		CNFTeamCreateLayer* pLayer = CNFTeamCreateLayer::CreateLayer();
		CC_BREAK_IF(pLayer==NULL);

		pScene->addChild(pLayer);
		return pScene;

	} while (false);
	CCLog("Fun CNFTeamCreateLayer::scene Error!");
	return NULL;
}

CNFTeamCreateLayer * CNFTeamCreateLayer::CreateLayer()
{
	CNFTeamCreateLayer *pRet = new CNFTeamCreateLayer();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CCLog("Fun CNFTeamCreateLayer::CreateLayer Error!");
	delete pRet; 
	pRet = NULL; 
	return NULL;
}

bool CNFTeamCreateLayer::init()
{
	do 
	{
		//初始化父类
		CC_BREAK_IF(CCLayer::init()==false);

		m_nRoleID = 1;

		//创建主场景UI层
		UILayer* pUiLayer = UILayer::create(); 
		CC_BREAK_IF(pUiLayer==NULL);

		//-------------------加入主场景UI----------------------------
		UILayout *pCreateNinja = dynamic_cast<UILayout*>	(GUIReader::shareReader()->widgetFromJsonFile("CreateNinja.json"));
		CC_BREAK_IF(pCreateNinja==NULL);
		pUiLayer->addWidget(pCreateNinja);
		pCreateNinja->setName("CreateNinjaLayer");
		pCreateNinja->setSize(getContentSize());

		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/xiaoyin.ExportJson");
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/NewProject.ExportJson");
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/minren1.ExportJson");


		//获取返回按钮
		UIButton *pBtn_Back=dynamic_cast<UIButton*>(pCreateNinja->getChildByName("MainUi")->getChildByName("Btn_Return"));
		CC_BREAK_IF(pBtn_Back==NULL);
		pBtn_Back->addTouchEventListener(this,toucheventselector(CNFTeamCreateLayer::OnBackToLoginPage));

		//获取开始按钮
		UIButton *pBtn_Create_Team=dynamic_cast<UIButton*>(pCreateNinja->getChildByName("MainUi")->getChildByName("Btn_Create_Team"));
		CC_BREAK_IF(pBtn_Create_Team==NULL);
		pBtn_Create_Team->addTouchEventListener(this,toucheventselector(CNFTeamCreateLayer::OnGameStartPage));

		//获取输入框
        UITextField* pTextField = dynamic_cast<UITextField*>(pCreateNinja->getChildByName("MainUi")->getChildByName("Team_Name_TextField"));
		CC_BREAK_IF(pTextField==NULL);
		pTextField->setTouchEnabled(true);
        pTextField->addEventListenerTextField(this, textfieldeventselector(CNFTeamCreateLayer::textFieldEvent));
		pTextField->setText("");


		for (int i=0;i<3;i++)
		{
			//获取人物按钮
			UIButton *pBtn_Role=dynamic_cast<UIButton*>(pCreateNinja->getChildByName("MainUi")->getChildByName(CCString::createWithFormat("Btn_Char_%d",i+1)->getCString()));
			CC_BREAK_IF(pBtn_Role==NULL);
			pBtn_Role->addTouchEventListener(this,toucheventselector(CNFTeamCreateLayer::OnRoleSelect));

			if (i==0)
			{
				//获取按下图片
				UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pBtn_Role->getChildByName("Select_Img"));
				CC_BREAK_IF(pSelect_Img==NULL);
				pSelect_Img->setVisible(true);
			}
		}



		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/xiaoyin.ExportJson");
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/NewProject.ExportJson");
		CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo("armature/minren1.ExportJson");

		char cName[3][256]={"minren1","xiaoyin","NewProject"};



		CCArmature *pRole = CCArmature::create(cName[0]);
		CC_BREAK_IF(pRole==NULL);
		pRole->setPosition(ccp(SCREEN_WIDTH/2,210));
		pRole->setAnchorPoint(ccp(pRole->getAnchorPoint().x,0));
		pRole->getAnimation()->play("hold",-1,-1,-1,10000);
		addChild(pRole,enZOrderMid,enTagRole);
		




		addChild(pUiLayer,enZOrderBack,enTagStudioMainUiLayer);
		return true;
	} while (false);
	CCLog("Fun CNFTeamCreateLayer::init Error!");
	return false;
}

void CNFTeamCreateLayer::OnGameStartPage( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				if(m_nRoleID==0)return;

				UILayer* pMainUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
				CC_BREAK_IF(pMainUiLayer==NULL);

				UILayout *pCreateNinja = dynamic_cast<UILayout*>(pMainUiLayer->getWidgetByName("CreateNinjaLayer"));
				CC_BREAK_IF(pCreateNinja==NULL);

				//获取输入框
				UITextField* pTextField = dynamic_cast<UITextField*>(pCreateNinja->getChildByName("MainUi")->getChildByName("Team_Name_TextField"));
				CC_BREAK_IF(pTextField==NULL);

				if(strcmp(pTextField->getStringValue(),"")==0)return;

	
				//读取角色信息
				CNFServerRoleSelectInfoVec RoleSelectInfoVec;
				CNFServerDataManager::SharedData()->GetRoleSelectInfo(RoleSelectInfoVec);
				CNFServerRoleSelectInfoVec::iterator itRole=RoleSelectInfoVec.begin();

				for (;itRole!=RoleSelectInfoVec.end();itRole++)
				{
					if (itRole->nRoleID==m_nRoleID)
					{
						itRole->nState=0;
					}
				}

				CNFServerDataManager::SharedData()->SetRoleSelectInfo(RoleSelectInfoVec);


				//人物选择场景
				CCScene * pScene = CNFRoleSelectLayer::scene();
				CC_BREAK_IF(pScene==NULL);
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));


			}break;
		default:
			break;
		}
		return;
	} while (false);
	CCLog("CNFTeamCreateLayer::OnGameStartPage Error");
}

void CNFTeamCreateLayer::textFieldEvent( CCObject* pSender, TextFiledEventType type )
{

}

void CNFTeamCreateLayer::OnBackToLoginPage( CCObject *pSender, TouchEventType type )
{
	do 
	{

		//跳转到登陆界面
		CCScene * pScene = CNFLoginScene::scene();
		CC_BREAK_IF(pScene==NULL);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));


		return;
	} while (false);
	CCLog("CNFTeamCreateLayer::OnBackToLoginPage Error");
}

void CNFTeamCreateLayer::OnRoleSelect( CCObject *pSender, TouchEventType type )
{
	do 
	{

		UILayer* pMainUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioMainUiLayer));
		CC_BREAK_IF(pMainUiLayer==NULL);

		UILayout *pCreateNinja = dynamic_cast<UILayout*>	(pMainUiLayer->getWidgetByName("CreateNinjaLayer"));
		CC_BREAK_IF(pCreateNinja==NULL);

		for (int i=0;i<3;i++)
		{

			//获取人物按钮
			UIButton *pBtn_Role=dynamic_cast<UIButton*>(pCreateNinja->getChildByName("MainUi")->getChildByName(CCString::createWithFormat("Btn_Char_%d",i+1)->getCString()));
			CC_BREAK_IF(pBtn_Role==NULL);

			//获取按下图片
			UIImageView *pSelect_Img=dynamic_cast<UIImageView*>(pBtn_Role->getChildByName("Select_Img"));
			CC_BREAK_IF(pSelect_Img==NULL);

			if (pBtn_Role==pSender)
			{
				pSelect_Img->setVisible(true);
				m_nRoleID=i+1;

				removeChildByTag(enTagRole);


				char cName[3][256]={"minren1","xiaoyin","NewProject"};

				CCArmature *pRole = CCArmature::create(cName[i]);
				CC_BREAK_IF(pRole==NULL);
				pRole->setPosition(ccp(SCREEN_WIDTH/2,210));
				pRole->setAnchorPoint(ccp(pRole->getAnchorPoint().x,0));
				pRole->getAnimation()->play("hold",-1,-1,-1,10000);
				addChild(pRole,enZOrderMid,enTagRole);

			}
			else
			{
				pSelect_Img->setVisible(false);
			}

		}

		return;
	} while (false);
	CCLog("CNFTeamCreateLayer::OnRoleSelect Error");
}



