
//  http://www.9miao.com

#ifndef _NF_SERVER_DATAMANAGER_HEAD_
#define _NF_SERVER_DATAMANAGER_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*					 ��������Ϣ�ṹ��                                                     */
/************************************************************************/
//��������
struct tagServerErrorInfo
{
	int			nErrno;				//����ID
	string		srError;			//��������
	float		servertime;			//������ʱ��
	string		srError_data;		//��������
};

//��������Ϣ
struct tagServerInfo 
{
	int			nServerID;					//������id
	string		strServerName;				//����������
};
typedef vector<tagServerInfo>	CNFServerInfoVec;		//��������Ϣ����

//�����������б���Ϣ
struct tagServerListInfo
{
	int						nServerNum;					//����������
	CNFServerInfoVec		ServerInfoVec;				//��������Ϣ����
};

//��ɫѡ����Ϣ
struct tagServerRoleSelectInfo 
{
	int			nRoleID;					//��ɫID
	string		strRoleName;				//��ɫ���ƣ��ǳ�
	int			nRoleLevel;					//��ɫ�ȼ�
	int			nState;						//��ɫ״̬	0.����	1.��ս	2.����	
	float		fExp;						//��ɫ����
	float		fStrength;					//��ɫƣ��	
};
typedef vector<tagServerRoleSelectInfo>	CNFServerRoleSelectInfoVec;		//��ɫѡ����Ϣ����

//������Ϣ
struct tagServerSkillInfo
{
	int				nSkillID;					//����ID
	int				nSkillLevel;				//���ܵȼ�
	int				nPos;						//λ��ID		0.δ��װ	
};
typedef	vector<tagServerSkillInfo>				CNFServerSkillInfoVec;				//ÿһ����ɫ������Ϣ����	
typedef map<int,CNFServerSkillInfoVec>			CNFServerSkillInfoMap;				//���м�����Ϣ������		������ɫID		ֵ��ÿһ����ɫ������Ϣ����

//�û���Ϣ
struct tagServerUserInfo
{
	int					nUserID;					//�û����
	string				strPlatform;				//ƽ̨
	string				strOpenID;					//ƽ̨���
	string				strUserName;				//�û�����
	int					nMoney;						//�꣨RMB��
	int					nGold;						//�����
	int					nVipLevel;					//VIP�ȼ�
	int					nTotalRecharge;				//�ܳ�ֵ�����
	string				strVIPPackages;				//VIP����������
	int					nTutorials;					//������������
	string				strHeroInfo;				//Ӣ����Ϣ
	int					nWhereID;					//������Ǹ�����
	bool				bIsHaveBgMusic;				//�Ƿ��б�������
	bool				bIsHaveSound;				//�Ƿ�����Ч
	bool				bIsHide;					//������Χ���
	string				strTradeAssociationName;	//��������
	int					nOutTime;					//�˳�����ʱ��
	int					nCreateAt;					//����ʱ��
	int					nUpdateAt;					//����ʱ��
};

//��ɫ��Ϣ
struct tagServerRoleInfo
{
	int					nRoleID;					//��ɫID
	int					nUserID;					//�û����
	int					nLevel;						//�ȼ�
	int					nHp;						//����ֵ
	int					nHpMax;						//����ֵ����
	float				fRegain;					//����ֵ�ظ�
	int					nMp;						//�����
	int					nMpMax;						//���������
	float				fMpRegain;					//������ظ�
	int					nAttack;					//������
	int					nDefend;					//������
	int					nHitTarget;					//����
	int					nAvoid;						//����
	int					nAttackSpeed;				//�����ٶ�
	int					nMoveSpeed;					//�ƶ��ٶ�
	int					nProficient;				//��ͨ
	int					nWellCoordinated;			//Ĭ��
	float				fCrit;						//������
	int					nCritDamage;				//�����˺�
	int					nWillPower;					//����
};
typedef	vector<tagServerRoleInfo>				CNFServerRoleInfoVec;				//��ɫ��Ϣvec
typedef	map<int,tagServerRoleInfo>				CNFServerRoleInfoMap;				//��ɫ��Ϣmap����ɫID����ɫ��Ϣ

//��ɫװ����
struct tagServerRoleEquipInfo
{
	int					nUserEquopID;				//�û�װ�����
	int					nUserID;					//�û����
	int					nEquipID;					//װ��ID
	int					nEquipQualityID;			//װ��Ʒ��
	int					nEquipLevel;				//װ���ȼ�
	int					nEquipMustAttributID;		//װ���ı�������
	int					nEquipMustAttributNum;		//װ���������Ե�ֵ
	int					nEquipRandomAttributID;		//װ�����������
	int					nEquipRandomAttributNum;	//װ��������Ե�ֵ
};
typedef vector<tagServerRoleEquipInfo>			CNFServerRoleEquipInfoVec;			//��ɫװ����ϢVec
typedef map<int,tagServerRoleEquipInfo>			CNFServerRoleEquipInfoMap;			//��ɫװ����ϢMap����ɫID����ɫװ����Ϣ





//************************************************************************/
/*					���������ݹ����ࣺ����洢���ӷ��������õ�����Ϣ 	                               */
//************************************************************************/
class CNFServerDataManager
{
protected:

protected:
	tagServerListInfo					m_ServerListInfo;					//�������б���Ϣ
	CNFServerRoleSelectInfoVec			m_RoleSelectInfoVec;				//��ɫѡ����Ϣ����
	CNFServerSkillInfoMap				m_SkillInfoMap;						//������Ϣ����
	tagServerUserInfo					m_UserInfo;							//�������洢���û���Ϣ


public:
	static CNFServerDataManager *SharedData();							

public:
	void SetServerListInfo(tagServerListInfo ServerListInfo);							//���÷����������б���Ϣ
	void GetServerListInfo(tagServerListInfo &ServerListInfo);							//��÷����������б���Ϣ

	void SetRoleSelectInfo(CNFServerRoleSelectInfoVec RoleSelectInfoVec);				//���ý�ɫѡ����Ϣ
	void GetRoleSelectInfo(CNFServerRoleSelectInfoVec &RoleSelectInfoVec);				//��ý�ɫѡ����Ϣ
	tagServerRoleSelectInfo GetFightRoleSelectInfo();									//��õ�ǰ��ս�Ľ�ɫѡ����Ϣ
	CNFServerRoleSelectInfoVec GetAssistRoleSelectInfo();								//��õ�ǰ�����Ľ�ɫѡ����ϢVec
	CNFServerRoleSelectInfoVec GetFreeRoleSelectInfo();									//��õ�ǰ���еĽ�ɫѡ����ϢVec
	void SetRoleSelectInfo(int nRoleID,tagServerRoleSelectInfo Info);					//ͨ����ɫID�����ý�ɫѡ����Ϣ
	tagServerRoleSelectInfo GetRoleSelectInfo(int nRoleID);								//ͨ����ɫID����ý�ɫѡ����Ϣ


	void SetSkillInfo(int nRoleID,CNFServerSkillInfoVec SkillInfoMap);					//ͨ����ɫID�����ü�����Ϣ
	bool GetSkillInfo(int nRoleID,CNFServerSkillInfoVec &SkillInfoMap);					//ͨ����ɫID����ü�����Ϣ

	void SetUserInfo(tagServerUserInfo UserInfo);										//�����û���Ϣ
	bool GetUserInfo(tagServerUserInfo &UserInfo);										//�õ��û���Ϣ


};







/************************************************************************/
/*					ͨ�ű�ǩ�궨��                                                                     */
/************************************************************************/
#define				_NF_HTTP_TAG_REGISTER_				"http_register"				//ע��
#define				_NF_HTTP_TAG_LOGIN_					"http_login"				//��¼
#define				_NF_HTTP_TAG_SERVERLIST_			"http_serverlist"			//�������б�
#define				_NF_HTTP_TAG_CHECK_USER_EXIST_		"http_check_user_exist"		//�û����
#define				_NF_HTTP_TAG_CREATE_HERO_			"http_create_hero"			//������ɫ
#define				_NF_HTTP_TAG_START_GAME_			"http_start_game"			//��ʼ��Ϸ

/************************************************************************/
/*				ͨѶ�ࣺ
				�������ӷ������õ���Ϣ								*/
/************************************************************************/
//���幫�ù��캯��
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

//ͨ�Ż���
class CNFBasicHttpMsger:public CCLayerColor ,public CMsgSender
{
protected:
	enum enHttpStatus		//״̬��ǩ����ʱû�ã�
	{
		enHttpStatusFree,		//״̬:����
		enHttpStatusSending,	//״̬:���ڷ���
		enHttpStatusNoNet,		//״̬:����������
	};
	enum					//��ǩ
	{
		enTagMenu,
	};

protected:
	enHttpStatus					m_enHttpStatus;					//��Ϣ״̬ 0���� 1���� 2���� ����ʱû�ã�
	bool							m_bTouchOnMenu;					//�ж��Ƿ����ڲ˵���

public:
	//������Ϣ
	void SendHttpMsg(CCNode* pOwner,void *pSendDataBuffer);

protected:
	bool init();//���������ö�ռ ��ʼ��UI

	void onExit();//����������ռ

	//��ťȡ��
	void OnBtnClickCancel(CCObject *pSender);

	//���ӳ�ʱ
	void OnTimeOut();

	//������ȷ
	void OnConnectRight();

	//���Ӵ���
	void OnConnectError();

	virtual bool DealRequest(CCHttpRequest *pRequest,void *pSendDataBuffer){return true;}
	//Http Response��Ϣ�ص�
	virtual void onHttpRequestCallBack(cocos2d::CCNode *sender, void *data){;}

protected:
	//��������(������Ϣ�ַ�)
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
};


/************************************************************************/
/*						ͨ����ʵ�֣��������б�                                                                     */
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
/*						ͨ����ʵ�֣�ע��                                                                    */
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
/*						ͨ����ʵ�֣���¼                                                                   */
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
/*						ͨ����ʵ�֣��û����                                                                     */
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
/*						ͨ����ʵ�֣�������ɫ                                                                    */
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
/*						ͨ����ʵ�֣�ѡ���ɫ����ʼ��Ϸ                                                                    */
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




