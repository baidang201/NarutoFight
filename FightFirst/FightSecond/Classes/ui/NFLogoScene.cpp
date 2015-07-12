#include "NFLogoScene.h"
#include "NFLoginScene.h"			//服务器选择场景
#include "../datamanager/NFDataManager.h"	//数据管理

CCScene* CNFLogoScene::scene()
{
	do 
	{
		CCScene* pScene = CCScene::create();
		CC_BREAK_IF(pScene==NULL);

		CNFLogoScene* pLayer = CNFLogoScene::CreateLayer();
		CC_BREAK_IF(pLayer==NULL);

		pScene->addChild(pLayer);
		return pScene;

	} while (false);
	CCLog("Fun CNFLogoScene::scene Error!");
	return NULL;
}

CNFLogoScene * CNFLogoScene::CreateLayer()
{
	CNFLogoScene *pRet = new CNFLogoScene();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CCLog("Fun CNFLogoScene::CreateLayer Error!");
	pRet = NULL; 
	delete pRet; 
	return NULL;
}

bool CNFLogoScene::init()
{
	do 
	{
		/************************************************************************/
		/*				初始化数据库                                                                     */
		/************************************************************************/
		CNFStaticDataManager::SharedData()->initBasicData();
		CNFSaveDataManager::SharedData()->initBasicData();

		tagSkillStaticInfo Info;
		CNFStaticDataManager::SharedData()->GetSkillStaticInfo(1,1,Info);
		//tagStageStaticInfo StageInfo;
		//CNFStaticDataManager::SharedData()->GetStageStaticInfo(1,StageInfo);
		//tagProtagonistStaticInfo ProInfo;
		//CNFStaticDataManager::SharedData()->GetProtagonistStaticInfo(1,ProInfo);
		//tagEnemyStaticInfo EnemyInfo;
		//CNFStaticDataManager::SharedData()->GetEnemyStaticInfo(1,EnemyInfo);


		//初始化父类
		CC_BREAK_IF(CCLayer::init()==false);

		/************************************************************************/
		/*			背景图片                                                                     */
		/************************************************************************/
		//创建logo图片
		CCSprite * pLogoBg = CCSprite::create("ui/logo_bg.png");
		CC_BREAK_IF(pLogoBg==NULL);
		pLogoBg->setPosition(SCREEN_CENTER);
		addChild(pLogoBg,enZOrderFront);
   
		
		/************************************************************************/
		/*			1.UI：联网，确认是否有网络                                                                     */
		/************************************************************************/
		bool bIsNet = false;
		bIsNet = true;

		//若有网络
		if (bIsNet == true)
		{
			//延迟1秒
			this->runAction(CCSequence::create(
				CCDelayTime::create(1.f),
				CCCallFunc::create(this,callfunc_selector(CNFLogoScene::OnChangeScene)),
				NULL));
		}
		//若无网络
		else 
		{
			////延迟2秒
			//this->runAction(CCSequence::create(
			//	CCDelayTime::create(1.f),
			//	CCCallFunc::create(this,callfunc_selector(CNFLogoScene::OnAddTipLayer)),
			//	NULL));
		}

		return true;
	} while (false);
	CCLog("Fun CNFLogoScene::init Error!");
	return false;
}

//临时：切换场景
void CNFLogoScene::OnChangeScene()
{
	do 
	{
		//跳转到服务器选择界面
		CCScene * pScene = CNFLoginScene::scene();
		CC_BREAK_IF(pScene==NULL);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));
		
		return ;
	} while (false);
	CCLog("Fun CNFLogoScene::OnChangeScene Error!");
}

////临时：添加提示层
//void CNFLogoScene::OnAddTipLayer()
//{
//	do 
//	{
//
//		return ;
//	} while (false);
//	CCLog("Fun CNFLogoScene::OnAddTipLayer Error!");
//}



