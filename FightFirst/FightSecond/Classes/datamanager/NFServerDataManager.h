
//  http://www.9miao.com

#ifndef _NF_SERVER_DATAMANAGER_HEAD_
#define _NF_SERVER_DATAMANAGER_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*					 服务器信息结构体                                                     */
/************************************************************************/
//错误数据
struct tagServerErrorInfo
{
	int			nErrno;				//错误ID
	string		srError;			//错误内容
	float		servertime;			//服务器时间
	string		srError_data;		//错误数据
};

//服务器信息
struct tagServerInfo 
{
	int			nServerID;					//服务器id
	string		strServerName;				//服务器名称
};
typedef vector<tagServerInfo>	CNFServerInfoVec;		//服务器信息容器

//服务器分区列表信息
struct tagServerListInfo
{
	int						nServerNum;					//服务器数量
	CNFServerInfoVec		ServerInfoVec;				//服务器信息容器
};

//角色选择信息
struct tagServerRoleSelectInfo 
{
	int			nRoleID;					//角色ID
	string		strRoleName;				//角色名称：昵称
	int			nRoleLevel;					//角色等级
	int			nState;						//角色状态	0.空闲	1.出战	2.助阵	
	float		fExp;						//角色经验
	float		fStrength;					//角色疲劳	
};
typedef vector<tagServerRoleSelectInfo>	CNFServerRoleSelectInfoVec;		//角色选择信息容器

//技能信息
struct tagServerSkillInfo
{
	int				nSkillID;					//技能ID
	int				nSkillLevel;				//技能等级
	int				nPos;						//位置ID		0.未安装	
};
typedef	vector<tagServerSkillInfo>				CNFServerSkillInfoVec;				//每一个角色技能信息容器	
typedef map<int,CNFServerSkillInfoVec>			CNFServerSkillInfoMap;				//所有技能信息的容器		键：角色ID		值：每一个角色技能信息容器

//用户信息
struct tagServerUserInfo
{
	int					nUserID;					//用户编号
	string				strPlatform;				//平台
	string				strOpenID;					//平台编号
	string				strUserName;				//用户名称
	int					nMoney;						//钻（RMB）
	int					nGold;						//金币数
	int					nVipLevel;					//VIP等级
	int					nTotalRecharge;				//总充值人民币
	string				strVIPPackages;				//VIP礼包购买情况
	int					nTutorials;					//新手引导进度
	string				strHeroInfo;				//英雄信息
	int					nWhereID;					//玩家在那个区域
	bool				bIsHaveBgMusic;				//是否有背景音乐
	bool				bIsHaveSound;				//是否有音效
	bool				bIsHide;					//隐藏周围玩家
	string				strTradeAssociationName;	//工会名称
	int					nOutTime;					//退出工会时间
	int					nCreateAt;					//创建时间
	int					nUpdateAt;					//更新时间
};

//角色信息
struct tagServerRoleInfo
{
	int					nRoleID;					//角色ID
	int					nUserID;					//用户编号
	int					nLevel;						//等级
	int					nHp;						//生命值
	int					nHpMax;						//生命值上限
	float				fRegain;					//生命值回复
	int					nMp;						//查克拉
	int					nMpMax;						//查克拉上限
	float				fMpRegain;					//查克拉回复
	int					nAttack;					//攻击力
	int					nDefend;					//防御力
	int					nHitTarget;					//命中
	int					nAvoid;						//闪避
	int					nAttackSpeed;				//攻击速度
	int					nMoveSpeed;					//移动速度
	int					nProficient;				//精通
	int					nWellCoordinated;			//默契
	float				fCrit;						//暴击率
	int					nCritDamage;				//暴击伤害
	int					nWillPower;					//毅力
};
typedef	vector<tagServerRoleInfo>				CNFServerRoleInfoVec;				//角色信息vec
typedef	map<int,tagServerRoleInfo>				CNFServerRoleInfoMap;				//角色信息map：角色ID，角色信息

//角色装备表
struct tagServerRoleEquipInfo
{
	int					nUserEquopID;				//用户装备编号
	int					nUserID;					//用户编号
	int					nEquipID;					//装备ID
	int					nEquipQualityID;			//装备品质
	int					nEquipLevel;				//装备等级
	int					nEquipMustAttributID;		//装备的必有属性
	int					nEquipMustAttributNum;		//装备必有属性的值
	int					nEquipRandomAttributID;		//装备的随机属性
	int					nEquipRandomAttributNum;	//装备随机属性的值
};
typedef vector<tagServerRoleEquipInfo>			CNFServerRoleEquipInfoVec;			//角色装备信息Vec
typedef map<int,tagServerRoleEquipInfo>			CNFServerRoleEquipInfoMap;			//角色装备信息Map：角色ID，角色装备信息





//************************************************************************/
/*					服务器数据管理类：负责存储连接服务器所得到的信息 	                               */
//************************************************************************/
class CNFServerDataManager
{
protected:

protected:
	tagServerListInfo					m_ServerListInfo;					//服务器列表信息
	CNFServerRoleSelectInfoVec			m_RoleSelectInfoVec;				//角色选择信息容器
	CNFServerSkillInfoMap				m_SkillInfoMap;						//技能信息容器
	tagServerUserInfo					m_UserInfo;							//服务器存储的用户信息


public:
	static CNFServerDataManager *SharedData();							

public:
	void SetServerListInfo(tagServerListInfo ServerListInfo);							//设置服务器分区列表信息
	void GetServerListInfo(tagServerListInfo &ServerListInfo);							//获得服务器分区列表信息

	void SetRoleSelectInfo(CNFServerRoleSelectInfoVec RoleSelectInfoVec);				//设置角色选择信息
	void GetRoleSelectInfo(CNFServerRoleSelectInfoVec &RoleSelectInfoVec);				//获得角色选择信息
	tagServerRoleSelectInfo GetFightRoleSelectInfo();									//获得当前参战的角色选择信息
	CNFServerRoleSelectInfoVec GetAssistRoleSelectInfo();								//获得当前助攻的角色选择信息Vec
	CNFServerRoleSelectInfoVec GetFreeRoleSelectInfo();									//获得当前空闲的角色选择信息Vec
	void SetRoleSelectInfo(int nRoleID,tagServerRoleSelectInfo Info);					//通过角色ID，设置角色选择信息
	tagServerRoleSelectInfo GetRoleSelectInfo(int nRoleID);								//通过角色ID，获得角色选择信息


	void SetSkillInfo(int nRoleID,CNFServerSkillInfoVec SkillInfoMap);					//通过角色ID，设置技能信息
	bool GetSkillInfo(int nRoleID,CNFServerSkillInfoVec &SkillInfoMap);					//通过角色ID，获得技能信息

	void SetUserInfo(tagServerUserInfo UserInfo);										//设置用户信息
	bool GetUserInfo(tagServerUserInfo &UserInfo);										//得到用户信息


};







/************************************************************************/
/*					通信标签宏定义                                                                     */
/************************************************************************/
#define				_NF_HTTP_TAG_REGISTER_				"http_register"				//注册
#define				_NF_HTTP_TAG_LOGIN_					"http_login"				//登录
#define				_NF_HTTP_TAG_SERVERLIST_			"http_serverlist"			//服务器列表
#define				_NF_HTTP_TAG_CHECK_USER_EXIST_		"http_check_user_exist"		//用户检测
#define				_NF_HTTP_TAG_CREATE_HERO_			"http_create_hero"			//创建角色
#define				_NF_HTTP_TAG_START_GAME_			"http_start_game"			//开始游戏

/************************************************************************/
/*				通讯类：
				负责连接服务器得到信息								*/
/************************************************************************/
//定义公用构造函数
#define HTTP_CREATE(__TYPE__) \
	static bool SendMsgToServer(CCNode* pOwner,void *pDataBuffer) \
{ \
	__TYPE__ *pRet = new __TYPE__(); \
	if (pRet!=NULL && pRet->init()) \
{ \
	pRet->autorelease(); \
	pOwner->addChild(pRet,enZOrderFront+100000); \
	pRet->SendHttpMsg(pOwner,pDataBuffer); \
	return true; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return false; \
} \
}

//通信基类
class CNFBasicHttpMsger:public CCLayerColor ,public CMsgSender
{
protected:
	enum enHttpStatus		//状态标签（暂时没用）
	{
		enHttpStatusFree,		//状态:空闲
		enHttpStatusSending,	//状态:正在发送
		enHttpStatusNoNet,		//状态:无网络连接
	};
	enum					//标签
	{
		enTagMenu,
	};

protected:
	enHttpStatus					m_enHttpStatus;					//消息状态 0空闲 1发送 2接受 （暂时没用）
	bool							m_bTouchOnMenu;					//判断是否点击在菜单上

public:
	//发送消息
	void SendHttpMsg(CCNode* pOwner,void *pSendDataBuffer);

protected:
	bool init();//在这里设置独占 初始化UI

	void onExit();//在这里解除独占

	//按钮取消
	void OnBtnClickCancel(CCObject *pSender);

	//连接超时
	void OnTimeOut();

	//连接正确
	void OnConnectRight();

	//连接错误
	void OnConnectError();

	virtual bool DealRequest(CCHttpRequest *pRequest,void *pSendDataBuffer){return true;}
	//Http Response消息回调
	virtual void onHttpRequestCallBack(cocos2d::CCNode *sender, void *data){;}

protected:
	//触摸函数(负责消息分发)
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};


/************************************************************************/
/*						通信类实现：服务器列表                                                                     */
/************************************************************************/
class CNFHttpServerList:public CNFBasicHttpMsger
{
public:
	HTTP_CREATE(CNFHttpServerList);
protected:
	virtual bool DealRequest(CCHttpRequest *pRequest,void *pSendDataBuffer);
	virtual void onHttpRequestCallBack(cocos2d::CCNode *sender, void *data);
};

/************************************************************************/
/*						通信类实现：注册                                                                    */
/************************************************************************/
class CNFHttpRegister:public CNFBasicHttpMsger
{
protected:
	string				m_strUserName;
	string				m_strPassword;

public:
	HTTP_CREATE(CNFHttpRegister);
protected:
	virtual bool DealRequest(CCHttpRequest *pRequest,void *pSendDataBuffer);
	virtual void onHttpRequestCallBack(cocos2d::CCNode *sender, void *data);
};

/************************************************************************/
/*						通信类实现：登录                                                                   */
/************************************************************************/
class CNFHttpLogin:public CNFBasicHttpMsger
{
public:
	HTTP_CREATE(CNFHttpLogin);
protected:
	virtual bool DealRequest(CCHttpRequest *pRequest,void *pSendDataBuffer);
	virtual void onHttpRequestCallBack(cocos2d::CCNode *sender, void *data);
};

/************************************************************************/
/*						通信类实现：用户检测                                                                     */
/************************************************************************/
class CNFHttpCheckUserExist:public CNFBasicHttpMsger
{
public:
	HTTP_CREATE(CNFHttpCheckUserExist);
protected:
	virtual bool DealRequest(CCHttpRequest *pRequest,void *pSendDataBuffer);
	virtual void onHttpRequestCallBack(cocos2d::CCNode *sender, void *data);
};

/************************************************************************/
/*						通信类实现：创建角色                                                                    */
/************************************************************************/
class CNFHttpCreateHero:public CNFBasicHttpMsger
{
public:
	HTTP_CREATE(CNFHttpCreateHero);
protected:
	virtual bool DealRequest(CCHttpRequest *pRequest,void *pSendDataBuffer);
	virtual void onHttpRequestCallBack(cocos2d::CCNode *sender, void *data);
};

/************************************************************************/
/*						通信类实现：选择角色，开始游戏                                                                    */
/************************************************************************/
class CNFHttpStartGame:public CNFBasicHttpMsger
{
public:
	HTTP_CREATE(CNFHttpStartGame);
protected:
	virtual bool DealRequest(CCHttpRequest *pRequest,void *pSendDataBuffer);
	virtual void onHttpRequestCallBack(cocos2d::CCNode *sender, void *data);
};












#endif




