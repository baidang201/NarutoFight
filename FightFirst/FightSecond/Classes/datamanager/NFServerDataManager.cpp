#include "NFServerDataManager.h"
#include <algorithm>				//vector排序
#include "NFDataManager.h"

//输出头文件
#include "CocoStudio/Json/rapidjson/stringbuffer.h"
#include "CocoStudio/Json/rapidjson/writer.h"


/************************************************************************/
/*					容器排序                                                                     */
/************************************************************************/
//服务器列表：以服务器ID排序
bool lessmark(const tagServerInfo & s1,const tagServerInfo & s2)	//升序
{
	return s1.nServerID	< s2.nServerID;  
}
bool greatermark(const tagServerInfo & s1,const tagServerInfo & s2)  //降序
{  
	return s1.nServerID > s2.nServerID;  
}



/************************************************************************/
/*					服务器数据管理类                                                                     */
/************************************************************************/
static CNFServerDataManager * g_pServerDataManager = NULL;
CNFServerDataManager * CNFServerDataManager::SharedData()
{
	if(g_pServerDataManager==NULL)
	{
		g_pServerDataManager = new CNFServerDataManager();

		//临时：初始化技能信息
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

		//临时：初始化角色选择信息
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

//服务器分区列表信息
void CNFServerDataManager::SetServerListInfo( tagServerListInfo ServerListInfo )
{
	m_ServerListInfo = ServerListInfo;
}
void CNFServerDataManager::GetServerListInfo( tagServerListInfo &ServerListInfo )
{
	ServerListInfo = m_ServerListInfo;
}

//角色选择信息
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
		if (iter->nState == 1)		//参战状态为1
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
		if (iter->nState == 2)		//助攻状态为2
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
		if (iter->nState == 0)		//空闲状态为0
		{
			InfoVec.push_back(*iter);
		}
	}
	return InfoVec;
}

//技能信息
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

//用户信息
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
/*						通信类											*/
/************************************************************************/
#define _MASK_COLOR_			ccc4(0,0,0,200)					//遮罩颜色
#define _OUT_TIMER_				10								//等待计时
#define _BUTTON_CANCEL_POS_		ccp(SCREEN_WIDTH-100,100)		//取消按钮位置

bool CNFBasicHttpMsger::init()
{
	do 
	{
		CC_BREAK_IF(CCLayerColor::initWithColor(_MASK_COLOR_)==false);

		//独占触屏注册值小于-128才能屏蔽Menu(Menu默认是-128)
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -129, true);

		//添加菜单
		CCMenu *pMenu = CCMenu::create();
		CC_BREAK_IF(pMenu==NULL);
		pMenu->setPosition(CCPointZero);
		addChild(pMenu,1,enTagMenu);

		//添加取消按钮
		CCMenuItemFont *pBtnCancel = CCMenuItemFont::create("Cancel",this,menu_selector(CNFBasicHttpMsger::OnBtnClickCancel));
		CC_BREAK_IF(pBtnCancel==NULL);
		pMenu->setPosition(_BUTTON_CANCEL_POS_);
		pMenu->addChild(pBtnCancel);

		//loading图片
		CCSprite * pHttpLoading = CCSprite::create("gameui/skill_2.png");
		CC_BREAK_IF(pHttpLoading==NULL);
		pHttpLoading->setPosition(SCREEN_CENTER);
		addChild(pHttpLoading,enZOrderFront);

		//图片转圈
		pHttpLoading->runAction(CCRepeatForever::create(CCSequence::create(CCRotateBy::create(1.0f,360.f),NULL)));
	
		//延迟回调
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
		//按钮点击
		CCMenu *pMenu = dynamic_cast<CCMenu*>(getChildByTag(enTagMenu));
		CC_BREAK_IF(pMenu==NULL);
		//取到Menu中所用Button
		CCArray *pAllBtn = pMenu->getChildren();
		//遍历按钮
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
				//防止吞吃
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
		//点击到按钮
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

		//创建一个新链接
		CCHttpRequest* pRequest = new CCHttpRequest();
		CC_BREAK_IF(pRequest==NULL);

		//重载这个函数，处理URL 上传信息等问题
		DealRequest(pRequest,pSendDataBuffer);

		pRequest->setResponseCallback(this, callfuncND_selector(CNFBasicHttpMsger::onHttpRequestCallBack));

		//发送请求
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
/*					通信类实现：服务器列表													*/
/************************************************************************/
bool CNFHttpServerList::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//设置网址
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/config/servers?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"pf=%s","ourgame");
		pRequest->setRequestData(postData, strlen(postData)); 

		//输出网址
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//创建请求Tag
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
		//接收信息
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//校验数据为空
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//校验标签长度
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//校验标签
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_SERVERLIST_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_SERVERLIST_);
			break ;
		}

		//校验相应代码
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

		//获取相应返回数据
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//转化成字符串
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("ServerList = %s",szInput.c_str());
		//{"data":{"s_count":31,"servers":[{"serverId":11,"name":"格斗11区"},{"serverId":22,"name":"格斗22区"},{"serverId":17,"name":"格斗17区"},{"serverId":31,"name":"格斗31区"},{"serverId":2,"name":"格斗2区"},{"serverId":28,"name":"格斗28区"},{"serverId":4,"name":"格斗4区"},{"serverId":15,"name":"格斗15区"},{"serverId":25,"name":"格斗25区"},{"serverId":13,"name":"格斗13区"},{"serverId":10,"name":"格斗10区"},{"serverId":7,"name":"格斗7区"},{"serverId":30,"name":"格斗30区"},{"serverId":8,"name":"格斗8区"},{"serverId":19,"name":"格斗19区"},{"serverId":26,"name":"格斗26区"},{"serverId":1,"name":"格斗1区"},{"serverId":20,"name":"格斗20区"},{"serverId":23,"name":"格斗23区"},{"serverId":24,"name":"格斗24区"},{"serverId":14,"name":"格斗14区"},{"serverId":21,"name":"格斗21区"},{"serverId":5,"name":"格斗5区"},{"serverId":16,"name":"格斗16区"},{"serverId":27,"name":"格斗27区"},{"serverId":12,"name":"格斗12区"},{"serverId":18,"name":"格斗18区"},{"serverId":29,"name":"格斗29区"},{"serverId":6,"name":"格斗6区"},{"serverId":9,"name":"格斗9区"},{"serverId":3,"name":"格斗3区"}]},"errno":0,"error":"","error_data":"null","server_time":1393409937,"server":""}

		/************************************************************************/
		/*					解析Json                                                                     */
		/************************************************************************/
		//创建Json
		rapidjson::Document json;

		//复制字符串
		json.Parse<0>(szInput.c_str());

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun ServerList Json Error! %s\n",json.GetParseError());
		}
		
		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//连接成功：下载服务器列表
		{
			tagServerListInfo ServerListInfo;

			//得到data
			const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");

			//服务器数量
			ServerListInfo.nServerNum = DICTOOL->getIntValue_json(DataJson, "s_count");

			//服务器数组
			int nServerArrayCount = DICTOOL->getArrayCount_json(DataJson,"servers");
			for (int j = 0; j < nServerArrayCount; j++)
			{
				//数组
				const rapidjson::Value &ServerArray = DICTOOL->getSubDictionary_json(DataJson,"servers", j);

				tagServerInfo Info;
				Info.nServerID = DICTOOL->getIntValue_json(ServerArray, "serverId");
				Info.strServerName = DICTOOL->getStringValue_json(ServerArray, "name");

				ServerListInfo.ServerInfoVec.push_back(Info);
			}

			//排序：根据服务器ID，升序排列
			sort(ServerListInfo.ServerInfoVec.begin(),ServerListInfo.ServerInfoVec.end(),lessmark);

			//下载服务器列表成功，则将信息存入静态指针
			CNFServerDataManager::SharedData()->SetServerListInfo(ServerListInfo);
		}
		else						//连接失败：提示
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
/*					通信类实现：注册													*/
/************************************************************************/
bool CNFHttpRegister::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//注册信息
		THttpRegister * pInfo = (THttpRegister *)pSendDataBuffer;

		//得到服务器存储信息
		tagSaveServerInfo SaveServerInfo;
		CNFSaveDataManager::SharedData()->GetSaveServerInfo(SaveServerInfo);
		
		m_strUserName = pInfo->strUserName;
		m_strPassword = pInfo->strPassWord;

		//设置网址
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/center/register?");
		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"username=%s&password=%s&serverid=%d&pf=ourgame",pInfo->strUserName.c_str(),pInfo->strPassWord.c_str(),SaveServerInfo.nServerID);
		pRequest->setRequestData(postData, strlen(postData)); 

		//输出网址
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//创建请求Tag
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
		//接收信息
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//校验数据为空
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//校验标签长度
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//校验标签
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_REGISTER_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_REGISTER_);
			break ;
		}

		//校验相应代码
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

		//获取相应返回数据
		std::vector<char> * pResponsDataBuffer = response->getResponseData();
		
		//转化成字符串
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("Register = %s",szInput.c_str());
		//{"data":{"openid":"13","hash":"this is the session key"},"errno":0,"error":"","error_data":"null","server_time":1393410422,"server":""}
		//{"errno":12001,"error":"此昵称已经存在","error_data":"null","server_time":1393411413,"server":""}

		/************************************************************************/
		/*					解析Json                                                                     */
		/************************************************************************/
		//创建Json
		rapidjson::Document json;

		//复制字符串
		json.Parse<0>(szInput.c_str());

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun Register Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//连接成功：注册成功，创建角色
		{
			//data
			const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");
			//openid
			string strOpenID = DICTOOL->getStringValue_json(DataJson, "openid");


			//openid为用户唯一ID，存储到本地，存储用户名和密码。
			tagSaveUserInfo Info;
			CNFSaveDataManager::SharedData()->GetSaveUserInfo(Info);
			Info.strOpenID = strOpenID;
			Info.strUserName = m_strUserName;
			Info.strPassWord = m_strPassword;
			CNFSaveDataManager::SharedData()->SetSaveUserInfo(Info);

			//发消息，创建角色
			SendMsg(enHttpMsg_CreateHero,NULL,0);
		}
		else						//连接失败：注册失败
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
/*					通信类实现：登录													*/
/************************************************************************/
bool CNFHttpLogin::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//登录信息
		THttpLogin * pInfo = (THttpLogin *)pSendDataBuffer;

		//设置网址
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/center/login?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"username=%s&password=%s&pf=ourgame",pInfo->strUserName.c_str(),pInfo->strPassWord.c_str());		
		pRequest->setRequestData(postData, strlen(postData)); 

		//输出网址
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//创建请求Tag
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
		//接收信息
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//校验数据为空
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//校验标签长度
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//校验标签
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_LOGIN_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_LOGIN_);
			break ;
		}

		//校验相应代码
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

		//获取相应返回数据
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//转化成字符串
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("Login = %s",szInput.c_str());
		//{"data":{"openid":"1","hash":"this is session key"},"errno":0,"error":"","error_data":"null","server_time":1392955798,"server":""}


		///************************************************************************/
		///*					解析Json                                                                     */
		///************************************************************************/
		//创建Json
		rapidjson::Document json;

		//复制字符串
		json.Parse<0>(szInput.c_str());

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun Login Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//连接成功：登录成功
		{
			//发消息，检测用户角色
			SendMsg(enHttpMsg_CheckUserExist,NULL,0);
		}
		else						//连接失败：登录失败
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
/*					通信类实现：用户检测													*/
/************************************************************************/
bool CNFHttpCheckUserExist::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//得到当前存储的用户信息
		tagSaveUserInfo UserInfo;
		CNFSaveDataManager::SharedData()->GetSaveUserInfo(UserInfo);
		//得到当前存储的服务器ID
		tagSaveServerInfo ServerInfo;
		CNFSaveDataManager::SharedData()->GetSaveServerInfo(ServerInfo);


		//设置网址
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/user/checkUserExist?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"openid=%s&serverid=%d&pf=ourgame",UserInfo.strOpenID.c_str(),ServerInfo.nServerID);		
		pRequest->setRequestData(postData, strlen(postData)); 

		//输出网址
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//创建请求Tag
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
		//接收信息
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//校验数据为空
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//校验标签长度
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//校验标签
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_CHECK_USER_EXIST_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_CHECK_USER_EXIST_);
			break ;
		}

		//校验相应代码
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

		//获取相应返回数据
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//转化成字符串
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("check_user_exist = %s",szInput.c_str());
		//{"data":{"existUser":0},"errno":0,"error":"","error_data":"null","server_time":1393467132,"server":""}


		//************************************************************************/
		//*					解析Json                                                                     */
		//************************************************************************/
		//创建Json
		rapidjson::Document json;

		//复制字符串
		json.Parse<0>(szInput.c_str());

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun Register Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//连接成功：判断是否有角色
		{
			tagServerListInfo ServerListInfo;

			//得到data
			const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");

			int	nExistUser = DICTOOL->getIntValue_json(DataJson,"existUser",-1);
			if (nExistUser==0)				//角色不存在，则跳转到角色创建界面
			{
				//发消息，检测用户角色
				SendMsg(enHttpMsg_CreateHero,NULL,0);
			}
			else if (nExistUser==-1)		//角色存在，则跳转到角色选择界面
			{
				CNFServerRoleSelectInfoVec RoleSelectVec;

				//角色数组
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
		else			//连接失败		
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
/*						通信类实现：创建角色                                                                    */
/************************************************************************/
bool CNFHttpCreateHero::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//角色创建信息
		TTHttpCreateHero * pInfo = (TTHttpCreateHero *)pSendDataBuffer;

		//用户信息
		tagSaveUserInfo UserInfo;
		CNFSaveDataManager::SharedData()->GetSaveUserInfo(UserInfo);


		//设置网址
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/user/createUserHero?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"openid=%s&heroid=%d&heroname=%s&pf=ourgame",UserInfo.strOpenID.c_str(),pInfo->nHeroID,pInfo->strHeroName.c_str());		
		pRequest->setRequestData(postData, strlen(postData)); 

		//输出网址
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//创建请求Tag
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
		//接收信息
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//校验数据为空
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//校验标签长度
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//校验标签
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_CREATE_HERO_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_CREATE_HERO_);
			break ;
		}

		//校验相应代码
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

		//获取相应返回数据
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//转化成字符串
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("create_hero = %s",szInput.c_str());
		//{"data":{"Hero":[{"Hero_id":1,"Hero_name":"naruto","Hero_level":1,"Hero_E  xp":0,"Hero_Strength":200,"Hero_LastFillStrength":1393837642,"Hero_state":1}]},"errno":0,"error":"","error_data":"  null","server_time":1393837647,"server":""}

		//************************************************************************/
		//*					解析Json                                                                     */
		//************************************************************************/
		//创建Json
		rapidjson::Document json;

		//复制字符串
		json.Parse<0>(szInput.c_str());

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun create hero Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//连接成功：得到角色信息
		{
			//得到data
			const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");

			CNFServerRoleSelectInfoVec RoleSelectVec;

			//角色数组
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
		else				//连接失败		
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
/*						通信类实现：选择角色，开始游戏                                                                    */
/************************************************************************/
bool CNFHttpStartGame::DealRequest( CCHttpRequest *pRequest,void *pSendDataBuffer )
{
	do 
	{
		//用户信息
		tagSaveUserInfo UserInfo;
		CNFSaveDataManager::SharedData()->GetSaveUserInfo(UserInfo);

		//角色选择信息
		CNFServerRoleSelectInfoVec RoleSelectVec;
		CNFServerDataManager::SharedData()->GetRoleSelectInfo(RoleSelectVec);

		/*********************** 反解析Json *****************************************/					                                                                    
		rapidjson::Document document;
		document.SetObject();
		rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
		
		//创建一个数组
		rapidjson::Value array(rapidjson::kArrayType);
		for (CNFServerRoleSelectInfoVec::iterator iter = RoleSelectVec.begin();iter != RoleSelectVec.end();iter++)
		{
			rapidjson::Value object(rapidjson::kObjectType);
			object.AddMember("Hero_id", iter->nRoleID, allocator);
			object.AddMember("Hero_state", iter->nState, allocator);
			array.PushBack(object, allocator);
		}			
		//添加一个数组
		document.AddMember("Hero", array, allocator);
		
		//输出
		rapidjson::StringBuffer buffer;
		rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
		document.Accept(writer);
		//auto out = buffer.GetString();
		//CCLOG("out: %s", out); 
		/***************************** end ***************************************/

		//设置网址
		char szServerUrl[1024] = {0};
		sprintf(szServerUrl,"192.168.0.200:8080/Naruto_Fight/api/user/createUserHero?");

		pRequest->setUrl(szServerUrl);
		pRequest->setRequestType(CCHttpRequest::kHttpPost);

		char postData[1024]={0};
		sprintf(postData,"openid=%spf=ourgame&hero=%s",UserInfo.strOpenID.c_str());		
		pRequest->setRequestData(postData, strlen(postData)); 

		//输出网址
		string srPost = szServerUrl;
		srPost += postData;
		CCLog("srPost = %s",srPost.c_str());

		//创建请求Tag
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
		//接收信息
		CCHttpResponse *response = (CCHttpResponse*)data;
		if (!response)		//校验数据为空
		{
			CCLog("ERROR: response error!");
			break;
		}
		if (0 != strlen(response->getHttpRequest()->getTag()))		//校验标签长度
		{
			//CCLog("%s completed", response->getHttpRequest()->getTag());
		}

		//校验标签
		if(strcmp( response->getHttpRequest()->getTag(),_NF_HTTP_TAG_START_GAME_)!=0)
		{
			CCLog("ERROR: %s Connect Error!",_NF_HTTP_TAG_START_GAME_);
			break ;
		}

		//校验相应代码
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

		//获取相应返回数据
		std::vector<char> * pResponsDataBuffer = response->getResponseData();

		//转化成字符串
		string szInput = "";
		for (unsigned int i = 0; i < pResponsDataBuffer->size(); i++)
		{
			szInput += (*pResponsDataBuffer)[i];
		}
		CCLog("start_game = %s",szInput.c_str());
		//

		//************************************************************************/
		//*					解析Json                                                                     */
		//************************************************************************/
		//创建Json
		rapidjson::Document json;

		//复制字符串
		json.Parse<0>(szInput.c_str());

		//校验Json
		if (json.HasParseError()) 
		{
			CCLOG("Fun start game Json Error! %s\n",json.GetParseError());
		}

		//ErrorNo
		int nErrorNo = DICTOOL->getIntValue_json(json,"errno");
		if (nErrorNo==0)			//连接成功：进入游戏
		{
			////得到data
			//const rapidjson::Value &DataJson = DICTOOL->getSubDictionary_json(json,"data");

			//CNFServerRoleSelectInfoVec RoleSelectVec;

			////角色数组
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
		else			//连接失败		
		{
			break;
		}

		OnConnectRight();
		return;
	} while (false);
	CCLog("Fun CNFHttpStartGame::onHttpRequestCallBack Error");
	OnConnectError();
}











































