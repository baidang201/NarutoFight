#include "NFLoginScene.h"
#include "NFLoadingLayer.h"
#include "NFServerChangeLayer.h"
#include "NFTownScene.h"
#include "NFRoleSelectLayer.h"
#include "NFCreateTeamLayer.h"
#include "datamanager/NFServerDataManager.h"

#define _LOADREC_NUM_		16
/************************************************************************/
/*			登录场景													*/
/************************************************************************/
CCScene* CNFLoginScene::scene()
{
	do 
	{
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		CNFLoginScene* pLayer = CNFLoginScene::CreateLayer();
		CC_BREAK_IF(pLayer==NULL);

		pScene->addChild(pLayer);
		return pScene;

	} while (false);
	CCLog("Fun CNFLoginScene::scene Error!");
	return NULL;
}

CNFLoginScene * CNFLoginScene::CreateLayer()
{
	CNFLoginScene *pRet = new CNFLoginScene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CCLog("Fun CNFLoginScene::CreateLayer Error!");
	delete pRet; 
	pRet = NULL; 
	return NULL;
}

void CNFLoginScene::onEnter()
{
	CCLayer::onEnter();
	do 
	{
		//创建Laoding层
		CNFLoadingLayer * pLoading = CNFLoadingLayer::CreateLayer();
		CC_BREAK_IF(pLoading==NULL);
		addChild(pLoading,enZOrderFront+10000,enTagLoading);
		
		m_nLoadRecNum = 0;
		m_nLoadRecTotalNum = _LOADREC_NUM_;

		//预加载本层资源
		LoadRec();

		return ;
	} while (false);
	CCLog("Fun CNFLoginScene::onEnter Error!");
}

//资源预加载
void CNFLoginScene::LoadRec()
{
	CCTextureCache::sharedTextureCache()->addImageAsync("ui/serverselect_bg.png",this,callfuncO_selector(CNFLoginScene::OnLoadRecCallBack));
	
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/kulougongshou.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/kulouzhanshi.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/mayi.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/bianyikunchong.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/bubing.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/xiaoyin.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/NewProject.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/minren1.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/kulou_arrow.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/naili.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/NPC_kakaxi.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/portal.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));

	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/hited_light.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/public_casting.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
	CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfoAsync("armature/skill_light_1.ExportJson",this,schedule_selector(CNFLoginScene::OnLoadArmatureData));
}


void CNFLoginScene::OnLoadArmatureData( float percent )
{
	//CCLog("==================%f",percent);
	
	m_nLoadRecNum ++;

	//得到Laoding层
	CNFLoadingLayer * pLoading = dynamic_cast<CNFLoadingLayer *>(getChildByTag(enTagLoading));
	if(pLoading!=NULL){
		pLoading->SetPro( (float)m_nLoadRecNum/(float)m_nLoadRecTotalNum );
	}else{
		CCLog("Fun CNFLoginScene::OnLoadArmatureData Error!");
	}
		
	if(m_nLoadRecNum==m_nLoadRecTotalNum){
		//延迟1秒
		this->runAction(CCSequence::create(
			CCDelayTime::create(2.f),
			CCCallFunc::create(this,callfunc_selector(CNFLoginScene::InitSceneUI)),
			NULL));
	}
	if (percent >= 1)
	{
		CCLog("addArmatureFileInfoAsync over");
	}
}

//资源加载回调
void CNFLoginScene::OnLoadRecCallBack( CCObject *pObj )
{
	do 
	{
		m_nLoadRecNum ++;

		//得到Laoding层
		CNFLoadingLayer * pLoading = dynamic_cast<CNFLoadingLayer *>(getChildByTag(enTagLoading));
		CC_BREAK_IF(pLoading==NULL);
		pLoading->SetPro( (float)m_nLoadRecNum/(float)m_nLoadRecTotalNum );

		if(m_nLoadRecNum==m_nLoadRecTotalNum){
			//延迟1秒
			this->runAction(CCSequence::create(
				CCDelayTime::create(2.f),
				CCCallFunc::create(this,callfunc_selector(CNFLoginScene::InitSceneUI)),
				NULL));
		}

		return ;
	} while (false);
	CCLog("Fun CNFLoginScene::OnLoadRecCallBack Error!");
}

// //最后一个资源加载回调
// void CNFLoginScene::OnLoadLastRecCallBack( CCObject *pObj )
// {
// 	do 
// 	{
// 		//CCLog("==================last");
// 		m_nLoadRecNum ++;
// 
// 		//得到Laoding层
// 		CNFLoadingLayer * pLoading = dynamic_cast<CNFLoadingLayer *>(getChildByTag(enTagLoading));
// 		CC_BREAK_IF(pLoading==NULL);
// 		pLoading->SetPro( 1.f );
// 
// 		//延迟1秒
// 		this->runAction(CCSequence::create(
// 			CCDelayTime::create(1.f),
// 			CCCallFunc::create(this,callfunc_selector(CNFLoginScene::InitSceneUI)),
// 			NULL));
// 
// 		return ;
// 	} while (false);
// 	CCLog("Fun CNFLoginScene::OnLoadLastRecCallBack Error!");
// }

void CNFLoginScene::InitSceneUI()
{
	do 
	{
		//移除Laoding层 
		CNFLoadingLayer * pLoading = dynamic_cast<CNFLoadingLayer *>(getChildByTag(enTagLoading));
		if (pLoading!=NULL)
		removeChildByTag(enTagLoading,true);

		/************************************************************************/
		/*				2.联网：得到服务器ID                                                                     */
		/************************************************************************/
		m_nCurrentServerID = 16;
		m_nServerTotalNum = 31;



		//创建登录UI层
		UILayer* pLoginUiLayer = UILayer::create(); 
		CC_BREAK_IF(pLoginUiLayer==NULL);


		UILayout *pLoginWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("NormalLogin.json"));
		CC_BREAK_IF(pLoginWidget==NULL);
		pLoginUiLayer->addWidget(pLoginWidget);
		pLoginWidget->setName("LoginLayer");
		pLoginWidget->setSize(getContentSize());

		addChild(pLoginUiLayer,enZOrderBack,enTagStudioLoginLayer);


		//设置切换服务器按钮回调
		UIButton* pBtn_Server_Select = dynamic_cast<UIButton*>(pLoginWidget->getChildByName("MainUi")->getChildByName("Btn_Change_Server"));
		CC_BREAK_IF(pBtn_Server_Select==NULL);
		pBtn_Server_Select->addTouchEventListener(this,toucheventselector(CNFLoginScene::OnBtnServerSelectCallBack));


		/************************************************************************/
		/*			背景图片                                                                     */
		/************************************************************************/
		////创建背景图片
		//CCSprite * pServerSelectBg = CCSprite::create("ui/serverselect_bg.png");
		//CC_BREAK_IF(pServerSelectBg==NULL);
		//pServerSelectBg->setScaleX( (float)CCDirector::sharedDirector()->getWinSize().width/(float)pServerSelectBg->getContentSize().width );
		//pServerSelectBg->setPosition(SCREEN_CENTER);
		//addChild(pServerSelectBg,enZOrderBack);

		////创建logo图片
		//CCSprite * pName = CCSprite::create("ui/logo_bg.png");
		//CC_BREAK_IF(pName==NULL);
		//pName->setPosition(ccp(SCREEN_WIDTH*0.5f,350));
		//addChild(pName,enZOrderBack);

		////创建服务器名称背景
		//CCScale9Sprite * pServerNameBg = CCScale9Sprite::create("ui/bg_1.png");
		//CC_BREAK_IF(pServerNameBg==NULL);
		//pServerNameBg->setContentSize(CCSizeMake(173,40));
		//pServerNameBg->setPosition(ccp(SCREEN_WIDTH*0.5f,220));
		//addChild(pServerNameBg,enZOrderBack);


		/************************************************************************/
		/*			创建按钮                                                                     */
		/************************************************************************/
		//CCMenu * pMenu = CCMenu::create();
		//CC_BREAK_IF(pMenu==NULL);
		//pMenu->setPosition(CCPointZero);
		//addChild(pMenu,enZOrderMid);



		////选择服务器按钮
		//CCMenuItemSprite * pBtnServerSelect = CCMenuItemSprite::create(
		//	CCSprite::create("ui/btn_enter_n.png"),
		//	CCSprite::create("ui/btn_enter_p.png"),
		//	this,
		//	menu_selector(CNFLoginScene::OnBtnServerSelectCallBack));
		//CC_BREAK_IF(pBtnServerSelect==NULL);
		//pBtnServerSelect->setScaleY(0.8f);
		//pBtnServerSelect->setScaleX(0.2f);
		//pBtnServerSelect->setPosition(ccp(pServerNameBg->getPositionX() + 63  ,pServerNameBg->getPositionY()));
		//pMenu->addChild(pBtnServerSelect,enZOrderFront);




		/************************************************************************/
		/*					label                                                                    */
		/************************************************************************/
		//CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/serverselect_xml.xml");
		//CC_BREAK_IF(pDicLang==NULL);

		////服务器label
		//CCString *pStrServer = dynamic_cast<CCString*>(pDicLang->objectForKey("server"));
		//CCLabelTTF * pLabelServer = CCLabelTTF::create(pStrServer->m_sString.c_str(),"Arial",20);
		//CC_BREAK_IF(pLabelServer==NULL);
		//pLabelServer->setPosition(ccp(pServerNameBg->getPositionX() - 120,pServerNameBg->getPositionY()));
		//pLabelServer->setColor(ccBLACK);
		//addChild(pLabelServer,enZOrderFront);

		////服务器名称label
		//CCString *pStrServerName = dynamic_cast<CCString*>(pDicLang->objectForKey("server_name"));
		//char szName[NAME_LEN] = {0};
		//sprintf(szName,pStrServerName->m_sString.c_str(),m_nCurrentServerID);
		//CCLabelTTF * pLabelServerName = CCLabelTTF::create(szName,"Arial",20);
		//CC_BREAK_IF(pLabelServerName==NULL);
		//pLabelServerName->setPosition(ccp(pServerNameBg->getPositionX() - 20,pServerNameBg->getPositionY()));
		//addChild(pLabelServerName,enZOrderFront,enTagServerNameLabel);

		////切换label
		//CCString *pStrChange = dynamic_cast<CCString*>(pDicLang->objectForKey("server_change"));
		//CCLabelTTF * pLabelChange = CCLabelTTF::create(pStrChange->m_sString.c_str(),"Arial",15);
		//CC_BREAK_IF(pLabelChange==NULL);
		//pLabelChange->setPosition(pBtnServerSelect->getPosition());
		//addChild(pLabelChange,enZOrderFront);


		
#if(NF_PLATFORM == NF_PLATFORM_NORMAL)//没接SDK版

		//隐藏SDK登录模块
		UILayout* pSdk_Login_Panel = dynamic_cast<UILayout*>(pLoginWidget->getChildByName("MainUi")->getChildByName("Sdk_Login_Panel"));
		CC_BREAK_IF(pSdk_Login_Panel==NULL);
		pSdk_Login_Panel->setEnabled(false);
		pSdk_Login_Panel->setVisible(false);


		//显示正常登录模块
		UILayout* pNormalLogin_Panel = dynamic_cast<UILayout*>(pLoginWidget->getChildByName("MainUi")->getChildByName("NormalLogin_Panel"));
		CC_BREAK_IF(pNormalLogin_Panel==NULL);
		pNormalLogin_Panel->setEnabled(true);
		pNormalLogin_Panel->setVisible(true);

		//设置注册按钮回调
		UIButton* pBtn_Register = dynamic_cast<UIButton*>(pLoginWidget->getChildByName("MainUi")->getChildByName("NormalLogin_Panel")->getChildByName("Btn_rg"));
		CC_BREAK_IF(pBtn_Register==NULL);
		pBtn_Register->addTouchEventListener(this,toucheventselector(CNFLoginScene::OnBtnSwitchToRegisterCallBack));

		//设置登录按钮回调
		UIButton* pBtn_Login = dynamic_cast<UIButton*>(pLoginWidget->getChildByName("MainUi")->getChildByName("NormalLogin_Panel")->getChildByName("Btn_Log"));
		CC_BREAK_IF(pBtn_Login==NULL);
		pBtn_Login->addTouchEventListener(this,toucheventselector(CNFLoginScene::OnBtnLoginCallBack));


		//设置注册按钮回调
		UITextField* pAccount_TextField = dynamic_cast<UITextField*>(pLoginWidget->getChildByName("MainUi")->getChildByName("NormalLogin_Panel")->getChildByName("Account_TextField"));
		CC_BREAK_IF(pAccount_TextField==NULL);
		pAccount_TextField->setTouchSize(CCSizeMake(161,30));
		pAccount_TextField->didNotSelectSelf();


#else

		////进入游戏按钮
		//CCMenuItemSprite * pBtnEnter = CCMenuItemSprite::create(
		//	CCSprite::create("ui/btn_enter_n.png"),
		//	CCSprite::create("ui/btn_enter_p.png"),
		//	this,
		//	menu_selector(CNFLoginScene::OnBtnEnterCallBack));
		//CC_BREAK_IF(pBtnEnter==NULL);
		//pBtnEnter->setPosition(ccp(SCREEN_WIDTH*0.5f,140));
		//pMenu->addChild(pBtnEnter,enZOrderFront);

		////进入游戏label
		//CCString *pStrStart = dynamic_cast<CCString*>(pDicLang->objectForKey("start"));
		//CCLabelTTF * pLabelStart = CCLabelTTF::create(pStrStart->m_sString.c_str(),"Arial",20);
		//CC_BREAK_IF(pLabelStart==NULL);
		//pLabelStart->setPosition(pBtnEnter->getPosition());
		//addChild(pLabelStart,enZOrderFront);

		//设置进入游戏按钮回调
		UIButton* pBtn_SDK_Start = dynamic_cast<UIButton*>(pLoginWidget->getChildByName("MainUi")->getChildByName("Sdk_Login_Panel")->getChildByName("Btn_SDK_Start"));
		CC_BREAK_IF(pBtn_SDK_Start==NULL);
		pBtn_SDK_Start->addTouchEventListener(this,toucheventselector(CNFLoginScene::OnBtnEnterCallBack));	

#endif



		return ;
	} while (false);
	CCLog("Fun CNFLoginScene::InitSceneUI Error!");
}

void CNFLoginScene::OnBtnEnterCallBack( CCObject *pSender, TouchEventType type)
{
	do 
	{


		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{



				////人物选择场景
				//CCScene * pScene = CNFRoleSelectLayer::scene();
				//CC_BREAK_IF(pScene==NULL);
				//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));

				//人物选择场景
				CCScene * pScene = CNFTeamCreateLayer::scene();
				CC_BREAK_IF(pScene==NULL);
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));
			}break;
		default:
			break;
		}


		return ;
	} while (false);
	CCLog("Fun CNFLoginScene::OnBtnEnterCallBack Error!");
}

void CNFLoginScene::OnBtnServerSelectCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{

		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				//创建切换服务器层
				CNFServerChangeLayer * pLayer = CNFServerChangeLayer::CreateLayer(m_nCurrentServerID,m_nServerTotalNum);
				CC_BREAK_IF(pLayer==NULL);
				pLayer->SetMsg(this);
				addChild(pLayer,enZOrderFront+100);


				//创建登录UI层
				UILayer* pLoginUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioLoginLayer));
				CC_BREAK_IF(pLoginUiLayer==NULL);

				UILayout *pNormalLogin = dynamic_cast<UILayout*>(pLoginUiLayer->getWidgetByName("LoginLayer"));
				CC_BREAK_IF(pNormalLogin==NULL);

				//显示正常登录模块
				UILayout* pNormalLogin_Panel = dynamic_cast<UILayout*>(pNormalLogin->getChildByName("MainUi")->getChildByName("NormalLogin_Panel"));
				CC_BREAK_IF(pNormalLogin_Panel==NULL);
				pNormalLogin_Panel->setEnabled(false);
				pNormalLogin_Panel->setVisible(false);


			}break;
		default:
			break;
		}


		return ;
	} while (false);
	CCLog("Fun CNFLoginScene::OnBtnServerSelectCallBack Error!");
}

void CNFLoginScene::RecMsg( int nMsgID,void* pInfo,int nSize )
{
	switch (nMsgID)
	{
	case enMsg_ServerSelect:
		{
			tagServerSelect * pCmd = (tagServerSelect *)pInfo;

			m_nCurrentServerID = pCmd->nServerID;

			//创建登录UI层
			UILayer* pLoginUiLayer = dynamic_cast<UILayer*>(getChildByTag(enTagStudioLoginLayer));
			CC_BREAK_IF(pLoginUiLayer==NULL);

			UILayout *pLoginWidget = dynamic_cast<UILayout*>(pLoginUiLayer->getWidgetByName("LoginLayer"));
			CC_BREAK_IF(pLoginWidget==NULL);


			//服务器名称label
			CCDictionary *pDicLang = CCDictionary::createWithContentsOfFile("ui_xml/serverselect_xml.xml");
			CC_BREAK_IF(pDicLang==NULL);
			CCString *pStrServerName = dynamic_cast<CCString*>(pDicLang->objectForKey("server_name"));

			char szName[NAME_LEN] = {0};
			sprintf(szName,pStrServerName->m_sString.c_str(),m_nCurrentServerID);

			UILabel* pServer_Name_Lable = dynamic_cast<UILabel*>(pLoginWidget->getChildByName("MainUi")->getChildByName("Server_Bg")->getChildByName("Server_Name_Lable"));
			CC_BREAK_IF(pServer_Name_Lable==NULL);

			pServer_Name_Lable->setText(szName);


			//显示正常登录模块
			UILayout* pNormalLogin_Panel = dynamic_cast<UILayout*>(pLoginWidget->getChildByName("MainUi")->getChildByName("NormalLogin_Panel"));
			CC_BREAK_IF(pNormalLogin_Panel==NULL);
			pNormalLogin_Panel->setEnabled(true);
			pNormalLogin_Panel->setVisible(true);

		}break;
	}
}

void CNFLoginScene::OnBtnSwitchToRegisterCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{

				//创建登录UI层
				UILayer* pRegisterUiLayer = UILayer::create(); 
				CC_BREAK_IF(pRegisterUiLayer==NULL);

				UILayout *pRegisterWidget = dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("NormalRegister.json"));
				CC_BREAK_IF(pRegisterWidget==NULL);
				pRegisterUiLayer->addWidget(pRegisterWidget);
				pRegisterWidget->setName("RegistLayer");
				pRegisterWidget->setSize(getContentSize());


				//设置返回登录按钮回调
				UIButton* pBtn_Back = dynamic_cast<UIButton*>(pRegisterWidget->getChildByName("MainUi")->getChildByName("Btn_Return"));
				CC_BREAK_IF(pBtn_Back==NULL);
				pBtn_Back->addTouchEventListener(this,toucheventselector(CNFLoginScene::OnBackToLoginCallBack));

				//注册按钮回调
				UIButton* pBtn_rg = dynamic_cast<UIButton*>(pRegisterWidget->getChildByName("MainUi")->getChildByName("NormalRegister_Panel")->getChildByName("Btn_rg"));
				CC_BREAK_IF(pBtn_rg==NULL);
				pBtn_rg->addTouchEventListener(this,toucheventselector(CNFLoginScene::OnBtnRegisterCallBack));

				addChild(pRegisterUiLayer,enZOrderBack,enTagStudioRegisterLayer);



			}break;
		default:
			break;
		}

		return ;
	} while (false);
	CCLog("Fun CNFLoginScene::OnBtnRegisterCallBack Error!");
}


void CNFLoginScene::OnBtnLoginCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{


				//读取角色信息
				CNFServerRoleSelectInfoVec RoleSelectInfoVec;
				CNFServerDataManager::SharedData()->GetRoleSelectInfo(RoleSelectInfoVec);
				CNFServerRoleSelectInfoVec::iterator itRole=RoleSelectInfoVec.begin();


				for (;itRole!=RoleSelectInfoVec.end();itRole++)
				{
					if (itRole->nState>=0)
					{
						//人物选择场景
						CCScene * pScene = CNFRoleSelectLayer::scene();
						CC_BREAK_IF(pScene==NULL);
						CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));

						return;
					}
				}






				//人物选择场景
				CCScene * pScene = CNFTeamCreateLayer::scene();
				CC_BREAK_IF(pScene==NULL);
				CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));

			}break;
		default:
			break;
		}

		return ;
	} while (false);
	CCLog("Fun CNFLoginScene::OnBtnLoginCallBack Error!");
}


void CNFLoginScene::OnAccountTextFieldEvent( CCObject *pSender, TextFiledEventType type )
{
	do 
	{


		return;
	} while (false);

}


/************************************************************************/
/*			注册模块													*/
/************************************************************************/


void CNFLoginScene::OnBackToLoginCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{


				removeChildByTag(enTagStudioRegisterLayer);


			}break;
		default:
			break;
		}

		return ;
	} while (false);
	CCLog("Fun CNFLoginScene::OnBackToLoginCallBack Error!");
}


void CNFLoginScene::OnBtnRegisterCallBack( CCObject *pSender, TouchEventType type )
{
	do 
	{
		switch (type)
		{
		case TOUCH_EVENT_ENDED:
			{


				CCLog("CNFLoginScene::OnBtnRegisterCallBack");

			}break;
		default:
			break;
		}

		return ;
	} while (false);
	CCLog("Fun CNFLoginScene::OnBtnRegisterCallBack Error!");
}





