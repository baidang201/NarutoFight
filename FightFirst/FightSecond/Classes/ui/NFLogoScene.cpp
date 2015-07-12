#include "NFLogoScene.h"
#include "NFLoginScene.h"			//������ѡ�񳡾�
#include "../datamanager/NFDataManager.h"	//���ݹ���

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
		/*				��ʼ�����ݿ�                                                                     */
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


		//��ʼ������
		CC_BREAK_IF(CCLayer::init()==false);

		/************************************************************************/
		/*			����ͼƬ                                                                     */
		/************************************************************************/
		//����logoͼƬ
		CCSprite * pLogoBg = CCSprite::create("ui/logo_bg.png");
		CC_BREAK_IF(pLogoBg==NULL);
		pLogoBg->setPosition(SCREEN_CENTER);
		addChild(pLogoBg,enZOrderFront);
   
		
		/************************************************************************/
		/*			1.UI��������ȷ���Ƿ�������                                                                     */
		/************************************************************************/
		bool bIsNet = false;
		bIsNet = true;

		//��������
		if (bIsNet == true)
		{
			//�ӳ�1��
			this->runAction(CCSequence::create(
				CCDelayTime::create(1.f),
				CCCallFunc::create(this,callfunc_selector(CNFLogoScene::OnChangeScene)),
				NULL));
		}
		//��������
		else 
		{
			////�ӳ�2��
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

//��ʱ���л�����
void CNFLogoScene::OnChangeScene()
{
	do 
	{
		//��ת��������ѡ�����
		CCScene * pScene = CNFLoginScene::scene();
		CC_BREAK_IF(pScene==NULL);
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1.f,pScene));
		
		return ;
	} while (false);
	CCLog("Fun CNFLogoScene::OnChangeScene Error!");
}

////��ʱ�������ʾ��
//void CNFLogoScene::OnAddTipLayer()
//{
//	do 
//	{
//
//		return ;
//	} while (false);
//	CCLog("Fun CNFLogoScene::OnAddTipLayer Error!");
//}



