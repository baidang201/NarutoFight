#ifndef _NF_SERVER_SELECT_SCENE_HEAD_
#define _NF_SERVER_SELECT_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*			登录场景													*/
/************************************************************************/
class CNFLoginScene : public CCLayer ,public CMsgReceiver
{
protected:
	//标签
	enum 
	{
		enTagLoading = 1,
		enTagServerNameLabel,
		enTagStudioLoginLayer,		//登陆层
		enTagStudioRegisterLayer,	//注册层
	};

	int			m_nLoadRecNum;				//资源加载数
	int			m_nLoadRecTotalNum;			//资源加载总数

	int			m_nCurrentServerID;			//当前服务器ID
	int			m_nServerTotalNum;			//服务器的总数量

public:
	static CCScene * scene();

	static CNFLoginScene * CreateLayer();

	//消息接收
	virtual void RecMsg(int nMsgID,void* pInfo,int nSize);

protected:
	virtual void onEnter();

	//资源预加载
	void LoadRec();			

	void OnLoadRecCallBack(CCObject *pObj);		//回调：资源加载
//	void OnLoadLastRecCallBack(CCObject *pObj);	//回调：最后一个资源加载

	//异步加载骨骼资源的回调
	void OnLoadArmatureData(float percent);

	virtual void InitSceneUI();					//在这里加载该图层专用的资源

	//---------------登录页按钮回调----------------
	void OnBtnEnterCallBack(CCObject *pSender, TouchEventType type);			//开始游戏
	void OnBtnServerSelectCallBack(CCObject *pSender, TouchEventType type);		//选择服务器
	void OnBtnSwitchToRegisterCallBack(CCObject *pSender, TouchEventType type);	//跳转到注册页
	void OnBtnLoginCallBack(CCObject *pSender, TouchEventType type);			//登录
	void OnAccountTextFieldEvent(CCObject *pSender, TextFiledEventType type);




	//---------------注册页按钮回调----------------
	void OnBackToLoginCallBack(CCObject *pSender, TouchEventType type);			//跳转到登录页
	void OnBtnRegisterCallBack(CCObject *pSender, TouchEventType type);			//注册
};


#endif