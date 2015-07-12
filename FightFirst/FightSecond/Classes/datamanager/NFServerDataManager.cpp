#include "NFServerDataManager.h"
#include <algorithm>				//vector����
#include "NFDataManager.h"

//���ͷ�ļ�
#include "CocoStudio/Json/rapidjson/stringbuffer.h"
#include "CocoStudio/Json/rapidjson/writer.h"


/************************************************************************/
/*					��������                                                                     */
/************************************************************************/
//�������б��Է�����ID����
bool lessmark(const tagServerInfo & s1,const tagServerInfo & s2)	//����
{
	return s1.nServerID	< s2.nServerID;  
}
bool greatermark(const tagServerInfo & s1,const tagServerInfo & s2)  //����
{  
	return s1.nServerID > s2.nServerID;  
}



/************************************************************************/
/*					���������ݹ�����                                                                     */
/************************************************************************/
static CNFServerDataManager * g_pServerDataManager = NULL;
CNFServerDataManager * CNFServerDataManager::SharedData()
{
	if(g_pServerDataManager==NULL)
	{
		g_pServerDataManager = new CNFServerDataManager();

		//��ʱ����ʼ��������Ϣ
		for (int i=1;i<=3;i++)
		{
			CNFServerSkillInfoVec PosInfo;
			for (int j=1;j<=8;j++)
			{
				tagServerSkillInfo Info;			
				if (j==1)		Info.nSkillID = 11;
				if (j==2)		Info.nSkillID = 12;
				if (j==3)		Info.nSkillID = 13;
				if (j==4)		Info.nSkillID = 101;
				if (j==5)		Info.nSkillID = 102;	
	
				Info.nSkillLevel = j;
				Info.nPos = j;

				if (j==6)
				{
					Info.nSkillID = 14;	
					Info.nSkillLevel = 11;
					Info.nPos = 0;
				}
				if (j==7)	
				{
					Info.nSkillID = 15;	
					Info.nSkillLevel = 11;
					Info.nPos = 0;
				}
					
				if (j==8)	
				{
					Info.nSkillID = 103;
					Info.nSkillLevel = 11;
					Info.nPos = 0;
				}
					

				PosInfo.push_back(Info);
			}
			g_pServerDataManager->m_SkillInfoMap[i] = PosInfo;
		}

		//��ʱ����ʼ����ɫѡ����Ϣ
		for (int i=0;i<3;i++)
		{
			tagServerRoleSelectInfo Info;
			Info.nRoleID = i+1;
			Info.fExp = 1;
			Info.fStrength = 1;
			Info.nRoleLevel = 1;
			Info.strRoleName = "naruto";
			if (i==0)		Info.nState = -1;
			if (i==1)		Info.nState = -1;
			if (i==2)		Info.nState = -1;

			g_pServerDataManager->m_RoleSelectInfoVec.push_back(Info);
		}

	}
	return g_pServerDataManager;
}

//�����������б���Ϣ
void CNFServerDataManager::SetServerListInfo( tagServerListInfo ServerListInfo )
{
	m_ServerListInfo = ServerListInfo;
}
void CNFServerDataManager::GetServerListInfo( tagServerListInfo &ServerListInfo )
{
	ServerListInfo = m_ServerListInfo;
}

//��ɫѡ����Ϣ
void CNFServerDataManager::SetRoleSelectInfo( CNFServerRoleSelectInfoVec RoleSelectInfoVec )
{
	m_RoleSelectInfoVec = RoleSelectInfoVec;
}
void CNFServerDataManager::GetRoleSelectInfo( CNFServerRoleSelectInfoVec &RoleSelectInfoVec )
{
	RoleSelectInfoVec = m_RoleSelectInfoVec;
}
tagServerRoleSelectInfo CNFServerDataManager::GetRoleSelectInfo(int nRoleID)
{
	tagServerRoleSelectInfo Info;
	for (CNFServerRoleSelectInfoVec::iterator iter = m_RoleSelectInfoVec.begin();iter != m_RoleSelectInfoVec.end();iter++)
	{
		if (iter->nRoleID==nRoleID)
		{
			Info = *iter;
			break;
		}
	}
	return Info;
}
void CNFServerDataManager::SetRoleSelectInfo(int nRoleID,tagServerRoleSelectInfo Info)
{
	for (CNFServerRoleSelectInfoVec::iterator iter = m_RoleSelectInfoVec.begin();iter != m_RoleSelectInfoVec.end();iter++)
	{
		if (iter->nRoleID==nRoleID)
		{
			*iter = Info;
			break;
		}
	}
	return ;
}
tagServerRoleSelectInfo CNFServerDataManager::GetFightRoleSelectInfo()
{
	tagServerRoleSelectInfo Info;
	for (CNFServerRoleSelectInfoVec::iterator iter = m_RoleSelectInfoVec.begin();iter != m_RoleSelectInfoVec.end();iter++)
	{
		if (iter->nState == 1)		//��ս״̬Ϊ1
		{
			Info = *iter;
			break;
		}
	}
	return Info;
}
CNFServerRoleSelectInfoVec CNFServerDataManager::GetAssistRoleSelectInfo()
{
	CNFServerRoleSelectInfoVec		InfoVec;
	for (CNFServerRoleSelectInfoVec::iterator iter = m_RoleSelectInfoVec.begin();iter != m_RoleSelectInfoVec.end();iter++)
	{
		if (iter->nState == 2)		//����״̬Ϊ2
		{
			InfoVec.push_back(*iter);
		}
	}
	return InfoVec;
}
CNFServerRoleSelectInfoVec CNFServerDataManager::GetFreeRoleSelectInfo()
{
	CNFServerRoleSelectInfoVec		InfoVec;
	for (CNFServerRoleSelectInfoVec::iterator iter = m_RoleSelectInfoVec.begin();iter != m_RoleSelectInfoVec.end();iter++)
	{
		if (iter->nState == 0)		//����״̬Ϊ0
		{
			InfoVec.push_back(*iter);
		}
	}
	return InfoVec;
}

//������Ϣ
void CNFServerDataManager::SetSkillInfo(int nRoleID,CNFServerSkillInfoVec SkillInfoMap)
{
	m_SkillInfoMap[nRoleID] = SkillInfoMap;
}
bool CNFServerDataManager::GetSkillInfo(int nRoleID,CNFServerSkillInfoVec &SkillInfoMap)
{
	CNFServerSkillInfoMap::iterator iFind = m_SkillInfoMap.find(nRoleID);
	if (iFind==m_SkillInfoMap.end())
		return false;

	SkillInfoMap = m_SkillInfoMap[nRoleID];
	return true;
}

//�û���Ϣ
void CNFServerDataManager::SetUserInfo( tagServerUserInfo UserInfo )
{
	m_UserInfo = UserInfo;
	return ;
}
bool CNFServerDataManager::GetUserInfo( tagServerUserInfo &UserInfo )
{
	UserInfo = m_UserInfo;
	return true;
}
















/************************************************************************/
/*						ͨ����											*/
/************************************************************************/
#define _MASK_COLOR_			ccc4(0,0,0,200)					//������ɫ
#define _OUT_TIMER_				10								//�ȴ���ʱ
#define _BUTTON_CANCEL_POS_		ccp(SCREEN_WIDTH-100,100)		//ȡ����ťλ��

bool CNFBasicHttpMsger::init()
{
	do 
	{
		CC_BREAK_IF(CCLayerColor::initWithColor(_MASK_COLOR_)==false);

		//��ռ����ע��ֵС��-128��������Menu(MenuĬ����-128)
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);

		//��Ӳ˵�
		CCMenu *pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(CCPointZero);
		addChild(pMenu,1,enTagMenu);

		//���ȡ����ť
		CCMenuItemFont *pBtnCancel = CCMenuItemFont::create("Cancel",this,menu_selector(CNFBasicHttpMsger::OnBtnClickCancel));
		CC_BREAK_IF(pBtnCancel==NULL);
		pMenu->setPosition(_BUTTON_CANCEL_POS_);
		pMenu->addChild(pBtnCancel);

		//loadingͼƬ
		CCSprite * pHttpLoading = CCSprite::create("gameui/skill_2.png");
		CC_BREAK_IF(pHttpLoading==NULL);
		pHttpLoading->setPosition(SCREEN_CENTER);
		addChild(pHttpLoading,enZOrderFront);

		//ͼƬתȦ
		pHttpLoading->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(1.0f,360.f),NULL)));
	
		//�ӳٻص�
		this->runAction(CCSequence::create(CCDelayTime::create(10.f),CCCallFunc::create(this,callfunc_selector(CNFBasicHttpMsger::OnTimeOut)),NULL));
	
		return true;
	} while (false);
	CCLog("Fun CNFBasicHttpMsger::init Error!");
	return false;
}
bool CNFBasicHttpMsger::ccTouchBegan( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//��ť���
		CCMenu *pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenu==NULL);
		//ȡ��Menu������Button
		CCArray *pAllBtn = pMenu->getChildren();
		//������ť
		CCObject *pItem = NULL;
		CCARRAY_FOREACH(pAllBtn,pItem)
		{
			CCMenuItem *pBtn = dynamic_cast<CCMenuItem*>(pItem);
			CC_BREAK_IF(pBtn==NULL);

			CCPoint local = pBtn->convertToNodeSpace(pTouch->getLocation());
			CCRect rcBtn = pBtn->rect();
			rcBtn.origin = CCPointZero;

			if (rcBtn.containsPoint(local))
			{
				//��ֹ�̳�
				pMenu->ccTouchBegan(pTouch,pEvent);
				m_bTouchOnMenu = true;
			}
		}
		return true;
	} while (false);
	CCLog("Fun CNFBasicHttpMsger::ccTouchBegan Error!");
	return false;

}
void CNFBasicHttpMsger::ccTouchEnded( CCTouch *pTouch, CCEvent *pEvent )
{
	do 
	{
		//�������ť
		if (m_bTouchOnMenu==true)
		{
			CCMenu *pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagMenu));
			CC_BREAK_IF(pMenu==NULL);
			pMenu->ccTouchEnded(pTouch,pEvent);
			m_bTouchOnMenu = false;
		}
		return;
	} while (false);
	CCLog("Fun CNFBasicHttpMsger::ccTouchEnded Error!");
}
void CNFBasicHttpMsger::SendHttpMsg(CCNode* pOwner,void *pSendDataBuffer)
{
	do 
	{
		CMsgReceiver * pReceiver = dynamic_cast<CMsgReceiver *>(pOwner);
		if (pReceiver!=NULL)
		{
			this->SetMsg(pReceiver);
		}

		//����һ��������
		CCHttpRequest* pRequest = new CCHttpRequest();
		CC_BREAK_IF(pRequest==NULL);

		//�����������������URL �ϴ���Ϣ������
		DealRequest(pRequest,pSendDataBuffer);

		pRequest->setResponseCallback(this, callfuncND_selector(CNFBasicHttpMsger::onHttpRequestCallBack));

		//��������
		CCHttpClient::getInstance()->send(pRequest);

		pRequest->release();

		return;
	} while (false);
	CCLog("Fun CNFBasicHttpMsger::SendHttpMsg Error!");
}
void CNFBasicHttpMsger::OnBtnClickCancel( CCObject *pSender )
{
	stopAllActions();	
	removeFromParentAndCleanup(true);
}
void CNFBasicHttpMsger::OnConnectRight()
{
	//CCMessageBox("Connect Right","Connect");
	CCLog("Connect : Connect Right!");
	removeFromParentAndCleanup(true);
}
void CNFBasicHttpMsger::OnConnectError()
{
	//CCMessageBox("Connect Error","Connect");
	CCLog("Connect : Connect Error!");
	removeFromParentAndCleanup(true);
}
void CNFBasicHttpMsger::OnTimeOut()
{
	//CCMessageBox("Out Time","Error");
	CCLog("Connect : Time Out!");
	removeFromParentAndCleanup(true);
}
void CNFBasicHttpMsger::onExit()
{
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCLayer::onExit();
}

/************************************************************************/
/*					ͨ����ʵ�֣��������б�													*/
/************************************************************************/
bool CNFHttpServerList::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//������ַ
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/config/servers?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"pf=%s","ourgame");
		pRequest->setRequestData(postData, strlen(postData)); 

		//�����ַ
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//��������Tag
		pRequest->setTag(_NF_HTTP_TAG_SERVERLIST_);

		return true;
	} while (false);
	CCLog("Fun CNFHttpServerList::DealRequest Error!");
	return false;
}
void CNFHttpServerList::onHttpRequestCallBack( cocos2d::CCNode *sender, void *data )
{
	do 
	{
		//������Ϣ
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//У������Ϊ��
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//У���ǩ����
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//У���ǩ
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_SERVERLIST_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_SERVERLIST_);
			break ;
		}

		//У����Ӧ����
		if (response->getResponseCode() != 200)
		{
			CCLog("ERROR: response code = %d",response->getResponseCode());
			break ;
		}

		if (response->isSucceed()==false)
		{
			CCLog("ERROR: response failed");
			CCLog("ERROR: buffer: %s", response->getErrorBuffer());	
			break ;
		}

		//��ȡ��Ӧ��������
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//ת�����ַ���
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("ServerList = %s",szInput.c_str());
		//{"data":{"s_count":31,"servers":[{"serverId":11,"name":"��11��"},{"serverId":22,"name":"��22��"},{"serverId":17,"name":"��17��"},{"serverId":31,"name":"��31��"},{"serverId":2,"name":"��2��"},{"serverId":28,"name":"��28��"},{"serverId":4,"name":"��4��"},{"serverId":15,"name":"��15��"},{"serverId":25,"name":"��25��"},{"serverId":13,"name":"��13��"},{"serverId":10,"name":"��10��"},{"serverId":7,"name":"��7��"},{"serverId":30,"name":"��30��"},{"serverId":8,"name":"��8��"},{"serverId":19,"name":"��19��"},{"serverId":26,"name":"��26��"},{"serverId":1,"name":"��1��"},{"serverId":20,"name":"��20��"},{"serverId":23,"name":"��23��"},{"serverId":24,"name":"��24��"},{"serverId":14,"name":"��14��"},{"serverId":21,"name":"��21��"},{"serverId":5,"name":"��5��"},{"serverId":16,"name":"��16��"},{"serverId":27,"name":"��27��"},{"serverId":12,"name":"��12��"},{"serverId":18,"name":"��18��"},{"serverId":29,"name":"��29��"},{"serverId":6,"name":"��6��"},{"serverId":9,"name":"��9��"},{"serverId":3,"name":"��3��"}]},"errno":0,"error":"","error_data":"null","server_time":1393409937,"server":""}

		/************************************************************************/
		/*					����Json                                                                     */
		/************************************************************************/
		//����Json
		rapidjson::Document json;

		//�����ַ���
		json.Parse<0>(szInput.c_str());

		//У��Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun ServerList Json Error! %s\n",json.GetParseError());
		}
		
		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//���ӳɹ������ط������б�
		{
			tagServerListInfo ServerListInfo;

			//�õ�data
			const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");

			//����������
			ServerListInfo.nServerNum = DICTOOL->getIntValue_json(DataJson, "s_count");

			//����������
			int nServerArrayCount = DICTOOL->getArrayCount_json(DataJson,"servers");
			for (int j = 0; j < nServerArrayCount; j++)
			{
				//����
				const rapidjson::Value &ServerArray = DICTOOL->getSubDictionary_json(DataJson,"servers", j);

				tagServerInfo Info;
				Info.nServerID = DICTOOL->getIntValue_json(ServerArray, "serverId");
				Info.strServerName = DICTOOL->getStringValue_json(ServerArray, "name");

				ServerListInfo.ServerInfoVec.push_back(Info);
			}

			//���򣺸��ݷ�����ID����������
			sort(ServerListInfo.ServerInfoVec.begin(),ServerListInfo.ServerInfoVec.end(),lessmark);

			//���ط������б�ɹ�������Ϣ���뾲ָ̬��
			CNFServerDataManager::SharedData()->SetServerListInfo(ServerListInfo);
		}
		else						//����ʧ�ܣ���ʾ
		{
			break;
		}
		
		
		OnConnectRight();
		return;
	} while (false);
	CCLog("Fun CNFHttpServerList::onHttpRequestCallBack Error");
	OnConnectError();
}

/************************************************************************/
/*					ͨ����ʵ�֣�ע��													*/
/************************************************************************/
bool CNFHttpRegister::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//ע����Ϣ
		THttpRegister * pInfo = (THttpRegister *)pSendDataBuffer;

		//�õ��������洢��Ϣ
		tagSaveServerInfo SaveServerInfo;
		CNFSaveDataManager::SharedData()->GetSaveServerInfo(SaveServerInfo);
		
		m_strUserName = pInfo->strUserName;
		m_strPassword = pInfo->strPassWord;

		//������ַ
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/center/register?");
		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"username=%s&password=%s&serverid=%d&pf=ourgame",pInfo->strUserName.c_str(),pInfo->strPassWord.c_str(),SaveServerInfo.nServerID);
		pRequest->setRequestData(postData, strlen(postData)); 

		//�����ַ
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//��������Tag
		pRequest->setTag(_NF_HTTP_TAG_REGISTER_);

		return true;
	} while (false);
	CCLog("Fun CTDHttpRegister::DealRequest Error!");
	return false;
}
void CNFHttpRegister::onHttpRequestCallBack( cocos2d::CCNode *sender, void *data )
{
	do 
	{
		//������Ϣ
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//У������Ϊ��
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//У���ǩ����
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//У���ǩ
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_REGISTER_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_REGISTER_);
			break ;
		}

		//У����Ӧ����
		if (response->getResponseCode() != 200)
		{
			CCLog("ERROR: response code = %d",response->getResponseCode());
			break ;
		}

		if (response->isSucceed()==false)
		{
			CCLog("ERROR: response failed");
			CCLog("ERROR: buffer: %s", response->getErrorBuffer());	
			break ;
		}

		//��ȡ��Ӧ��������
		std::vector<char> * pResponsDataBuffer = response->getResponseData();
		
		//ת�����ַ���
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("Register = %s",szInput.c_str());
		//{"data":{"openid":"13","hash":"this is the session key"},"errno":0,"error":"","error_data":"null","server_time":1393410422,"server":""}
		//{"errno":12001,"error":"���ǳ��Ѿ�����","error_data":"null","server_time":1393411413,"server":""}

		/************************************************************************/
		/*					����Json                                                                     */
		/************************************************************************/
		//����Json
		rapidjson::Document json;

		//�����ַ���
		json.Parse<0>(szInput.c_str());

		//У��Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun Register Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//���ӳɹ���ע��ɹ���������ɫ
		{
			//data
			const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");
			//openid
			string strOpenID = DICTOOL->getStringValue_json(DataJson, "openid");


			//openidΪ�û�ΨһID���洢�����أ��洢�û��������롣
			tagSaveUserInfo Info;
			CNFSaveDataManager::SharedData()->GetSaveUserInfo(Info);
			Info.strOpenID = strOpenID;
			Info.strUserName = m_strUserName;
			Info.strPassWord = m_strPassword;
			CNFSaveDataManager::SharedData()->SetSaveUserInfo(Info);

			//����Ϣ��������ɫ
			SendMsg(enHttpMsg_CreateHero,NULL,0);
		}
		else						//����ʧ�ܣ�ע��ʧ��
		{
			break;
		}


		OnConnectRight();
		return;
	} while (false);
	CCLog("Fun CNFHttpRegister::onHttpRequestCallBack Error");
	OnConnectError();
}

/************************************************************************/
/*					ͨ����ʵ�֣���¼													*/
/************************************************************************/
bool CNFHttpLogin::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//��¼��Ϣ
		THttpLogin * pInfo = (THttpLogin *)pSendDataBuffer;

		//������ַ
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/center/login?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"username=%s&password=%s&pf=ourgame",pInfo->strUserName.c_str(),pInfo->strPassWord.c_str());		
		pRequest->setRequestData(postData, strlen(postData)); 

		//�����ַ
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//��������Tag
		pRequest->setTag(_NF_HTTP_TAG_LOGIN_);

		return true;
	} while (false);
	CCLog("Fun CNFHttpLogin::DealRequest Error!");
	return false;
}
void CNFHttpLogin::onHttpRequestCallBack( cocos2d::CCNode *sender, void *data )
{
	do 
	{
		//������Ϣ
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//У������Ϊ��
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//У���ǩ����
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//У���ǩ
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_LOGIN_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_LOGIN_);
			break ;
		}

		//У����Ӧ����
		if (response->getResponseCode() != 200)
		{
			CCLog("ERROR: response code = %d",response->getResponseCode());
			break ;
		}

		if (response->isSucceed()==false)
		{
			CCLog("ERROR: response failed");
			CCLog("ERROR: buffer: %s", response->getErrorBuffer());	
			break ;
		}

		//��ȡ��Ӧ��������
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//ת�����ַ���
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("Login = %s",szInput.c_str());
		//{"data":{"openid":"1","hash":"this is session key"},"errno":0,"error":"","error_data":"null","server_time":1392955798,"server":""}


		///************************************************************************/
		///*					����Json                                                                     */
		///************************************************************************/
		//����Json
		rapidjson::Document json;

		//�����ַ���
		json.Parse<0>(szInput.c_str());

		//У��Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun Login Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//���ӳɹ�����¼�ɹ�
		{
			//����Ϣ������û���ɫ
			SendMsg(enHttpMsg_CheckUserExist,NULL,0);
		}
		else						//����ʧ�ܣ���¼ʧ��
		{
			break;
		}

		OnConnectRight();
		return;
	} while (false);
	CCLog("Fun CNFHttpLogin::onHttpRequestCallBack Error");
	OnConnectError();
}

/************************************************************************/
/*					ͨ����ʵ�֣��û����													*/
/************************************************************************/
bool CNFHttpCheckUserExist::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//�õ���ǰ�洢���û���Ϣ
		tagSaveUserInfo UserInfo;
		CNFSaveDataManager::SharedData()->GetSaveUserInfo(UserInfo);
		//�õ���ǰ�洢�ķ�����ID
		tagSaveServerInfo ServerInfo;
		CNFSaveDataManager::SharedData()->GetSaveServerInfo(ServerInfo);


		//������ַ
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/user/checkUserExist?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"openid=%s&serverid=%d&pf=ourgame",UserInfo.strOpenID.c_str(),ServerInfo.nServerID);		
		pRequest->setRequestData(postData, strlen(postData)); 

		//�����ַ
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//��������Tag
		pRequest->setTag(_NF_HTTP_TAG_CHECK_USER_EXIST_);

		return true;
	} while (false);
	CCLog("Fun CNFHttpCheckUserExist::DealRequest Error!");
	return false;
}
void CNFHttpCheckUserExist::onHttpRequestCallBack( cocos2d::CCNode *sender, void *data )
{
	do 
	{
		//������Ϣ
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//У������Ϊ��
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//У���ǩ����
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//У���ǩ
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_CHECK_USER_EXIST_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_CHECK_USER_EXIST_);
			break ;
		}

		//У����Ӧ����
		if (response->getResponseCode() != 200)
		{
			CCLog("ERROR: response code = %d",response->getResponseCode());
			break ;
		}

		if (response->isSucceed()==false)
		{
			CCLog("ERROR: response failed");
			CCLog("ERROR: buffer: %s", response->getErrorBuffer());	
			break ;
		}

		//��ȡ��Ӧ��������
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//ת�����ַ���
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("check_user_exist = %s",szInput.c_str());
		//{"data":{"existUser":0},"errno":0,"error":"","error_data":"null","server_time":1393467132,"server":""}


		//************************************************************************/
		//*					����Json                                                                     */
		//************************************************************************/
		//����Json
		rapidjson::Document json;

		//�����ַ���
		json.Parse<0>(szInput.c_str());

		//У��Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun Register Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//���ӳɹ����ж��Ƿ��н�ɫ
		{
			tagServerListInfo ServerListInfo;

			//�õ�data
			const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");

			int	nExistUser = DICTOOL->getIntValue_json(DataJson,"existUser",-1);
			if (nExistUser==0)				//��ɫ�����ڣ�����ת����ɫ��������
			{
				//����Ϣ������û���ɫ
				SendMsg(enHttpMsg_CreateHero,NULL,0);
			}
			else if (nExistUser==-1)		//��ɫ���ڣ�����ת����ɫѡ�����
			{
				CNFServerRoleSelectInfoVec RoleSelectVec;

				//��ɫ����
				int nRoleArrayCount = DICTOOL->getArrayCount_json(DataJson,"Hero");
				for (int j = 0; j < nRoleArrayCount; j++)
				{
					const rapidjson::Value &RoleArray = DICTOOL->getSubDictionary_json(DataJson,"Hero", j);

					tagServerRoleSelectInfo Info;
					Info.nRoleID = DICTOOL->getIntValue_json(RoleArray, "Hero_id");
					Info.strRoleName = DICTOOL->getStringValue_json(RoleArray, "Hero_name");
					Info.nRoleLevel = DICTOOL->getIntValue_json(RoleArray, "Hero_level");
					Info.nState = DICTOOL->getIntValue_json(RoleArray, "Hero_state");
					Info.fExp = DICTOOL->getFloatValue_json(RoleArray, "Hero_Exp");
					Info.fStrength = DICTOOL->getFloatValue_json(RoleArray, "Hero_Strength");

					RoleSelectVec.push_back(Info);
				}
				CNFServerDataManager::SharedData()->SetRoleSelectInfo(RoleSelectVec);
			
			}	
		}
		else			//����ʧ��		
		{
			break;
		}

		OnConnectRight();
		return;
	} while (false);
	CCLog("Fun CNFHttpCheckUserExist::onHttpRequestCallBack Error");
	OnConnectError();
}

/************************************************************************/
/*						ͨ����ʵ�֣�������ɫ                                                                    */
/************************************************************************/
bool CNFHttpCreateHero::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//��ɫ������Ϣ
		TTHttpCreateHero * pInfo = (TTHttpCreateHero *)pSendDataBuffer;

		//�û���Ϣ
		tagSaveUserInfo UserInfo;
		CNFSaveDataManager::SharedData()->GetSaveUserInfo(UserInfo);


		//������ַ
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/user/createUserHero?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"openid=%s&heroid=%d&heroname=%s&pf=ourgame",UserInfo.strOpenID.c_str(),pInfo->nHeroID,pInfo->strHeroName.c_str());		
		pRequest->setRequestData(postData, strlen(postData)); 

		//�����ַ
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//��������Tag
		pRequest->setTag(_NF_HTTP_TAG_CREATE_HERO_);

		return true;
	} while (false);
	CCLog("Fun CNFHttpCreateHero::DealRequest Error!");
	return false;
}
void CNFHttpCreateHero::onHttpRequestCallBack( cocos2d::CCNode *sender, void *data )
{
	do 
	{
		//������Ϣ
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//У������Ϊ��
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//У���ǩ����
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//У���ǩ
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_CREATE_HERO_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_CREATE_HERO_);
			break ;
		}

		//У����Ӧ����
		if (response->getResponseCode() != 200)
		{
			CCLog("ERROR: response code = %d",response->getResponseCode());
			break ;
		}

		if (response->isSucceed()==false)
		{
			CCLog("ERROR: response failed");
			CCLog("ERROR: buffer: %s", response->getErrorBuffer());	
			break ;
		}

		//��ȡ��Ӧ��������
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//ת�����ַ���
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("create_hero = %s",szInput.c_str());
		//{"data":{"Hero":[{"Hero_id":1,"Hero_name":"naruto","Hero_level":1,"Hero_E  xp":0,"Hero_Strength":200,"Hero_LastFillStrength":1393837642,"Hero_state":1}]},"errno":0,"error":"","error_data":"  null","server_time":1393837647,"server":""}

		//************************************************************************/
		//*					����Json                                                                     */
		//************************************************************************/
		//����Json
		rapidjson::Document json;

		//�����ַ���
		json.Parse<0>(szInput.c_str());

		//У��Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun create hero Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//���ӳɹ����õ���ɫ��Ϣ
		{
			//�õ�data
			const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");

			CNFServerRoleSelectInfoVec RoleSelectVec;

			//��ɫ����
			int nRoleArrayCount = DICTOOL->getArrayCount_json(DataJson,"Hero");
			for (int j = 0; j < nRoleArrayCount; j++)
			{
				const rapidjson::Value &RoleArray = DICTOOL->getSubDictionary_json(DataJson,"Hero", j);

				tagServerRoleSelectInfo Info;
				Info.nRoleID = DICTOOL->getIntValue_json(RoleArray, "Hero_id");
				Info.strRoleName = DICTOOL->getStringValue_json(RoleArray, "Hero_name");
				Info.nRoleLevel = DICTOOL->getIntValue_json(RoleArray, "Hero_level");
				Info.nState = DICTOOL->getIntValue_json(RoleArray, "Hero_state");
				Info.fExp = DICTOOL->getFloatValue_json(RoleArray, "Hero_Exp");
				Info.fStrength = DICTOOL->getFloatValue_json(RoleArray, "Hero_Strength");

				RoleSelectVec.push_back(Info);
			}
			CNFServerDataManager::SharedData()->SetRoleSelectInfo(RoleSelectVec);
		}
		else				//����ʧ��		
		{
			break;
		}

		OnConnectRight();
		return;
	} while (false);
	CCLog("Fun CNFHttpCreateHero::onHttpRequestCallBack Error");
	OnConnectError();
}

/************************************************************************/
/*						ͨ����ʵ�֣�ѡ���ɫ����ʼ��Ϸ                                                                    */
/************************************************************************/
bool CNFHttpStartGame::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//�û���Ϣ
		tagSaveUserInfo UserInfo;
		CNFSaveDataManager::SharedData()->GetSaveUserInfo(UserInfo);

		//��ɫѡ����Ϣ
		CNFServerRoleSelectInfoVec RoleSelectVec;
		CNFServerDataManager::SharedData()->GetRoleSelectInfo(RoleSelectVec);

		/*********************** ������Json *****************************************/					                                                                    
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		
		//����һ������
		rapidjson::Value array(rapidjson::kArrayType);
		for (CNFServerRoleSelectInfoVec::iterator iter = RoleSelectVec.begin();iter != RoleSelectVec.end();iter++)
		{
			rapidjson::Value object(rapidjson::kObjectType);
			object.AddMember("Hero_id", iter->nRoleID, allocator);
			object.AddMember("Hero_state", iter->nState, allocator);
			array.PushBack(object, allocator);
		}			
		//���һ������
		document.AddMember("Hero", array, allocator);
		
		//���
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);
		//auto out = buffer.GetString();
		//CCLOG("out: %s", out); 
		/***************************** end ***************************************/

		//������ַ
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/user/createUserHero?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"openid=%spf=ourgame&hero=%s",UserInfo.strOpenID.c_str());		
		pRequest->setRequestData(postData, strlen(postData)); 

		//�����ַ
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//��������Tag
		pRequest->setTag(_NF_HTTP_TAG_START_GAME_);

		return true;
	} while (false);
	CCLog("Fun CNFHttpCreateHero::DealRequest Error!");
	return false;
}
void CNFHttpStartGame::onHttpRequestCallBack( cocos2d::CCNode *sender, void *data )
{
	do 
	{
		//������Ϣ
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//У������Ϊ��
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//У���ǩ����
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//У���ǩ
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_START_GAME_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_START_GAME_);
			break ;
		}

		//У����Ӧ����
		if (response->getResponseCode() != 200)
		{
			CCLog("ERROR: response code = %d",response->getResponseCode());
			break ;
		}

		if (response->isSucceed()==false)
		{
			CCLog("ERROR: response failed");
			CCLog("ERROR: buffer: %s", response->getErrorBuffer());	
			break ;
		}

		//��ȡ��Ӧ��������
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//ת�����ַ���
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("start_game = %s",szInput.c_str());
		//

		//************************************************************************/
		//*					����Json                                                                     */
		//************************************************************************/
		//����Json
		rapidjson::Document json;

		//�����ַ���
		json.Parse<0>(szInput.c_str());

		//У��Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun start game Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//���ӳɹ���������Ϸ
		{
			////�õ�data
			//const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");

			//CNFServerRoleSelectInfoVec RoleSelectVec;

			////��ɫ����
			//int nRoleArrayCount = DICTOOL->getArrayCount_json(DataJson,"Hero");
			//for (int j = 0; j < nRoleArrayCount; j++)
			//{
			//	const rapidjson::Value &RoleArray = DICTOOL->getSubDictionary_json(DataJson,"Hero", j);

			//	tagServerRoleSelectInfo Info;
			//	Info.nRoleID = DICTOOL->getIntValue_json(RoleArray, "Hero_id");
			//	Info.strRoleName = DICTOOL->getStringValue_json(RoleArray, "Hero_name");
			//	Info.nRoleLevel = DICTOOL->getIntValue_json(RoleArray, "Hero_level");
			//	Info.nState = DICTOOL->getIntValue_json(RoleArray, "Hero_state");
			//	Info.fExp = DICTOOL->getFloatValue_json(RoleArray, "Hero_Exp");
			//	Info.fStrength = DICTOOL->getFloatValue_json(RoleArray, "Hero_Strength");

			//	RoleSelectVec.push_back(Info);
			//}
			//CNFServerDataManager::SharedData()->SetServerRoleSelectInfo(RoleSelectVec);
		}
		else			//����ʧ��		
		{
			break;
		}

		OnConnectRight();
		return;
	} while (false);
	CCLog("Fun CNFHttpStartGame::onHttpRequestCallBack Error");
	OnConnectError();
}











































