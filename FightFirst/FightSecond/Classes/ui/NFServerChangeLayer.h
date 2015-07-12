#ifndef _NF_SERVER_CHANGE_LAYER_HEAD_
#define _NF_SERVER_CHANGE_LAYER_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*			服务器切换层                                                                    */
/************************************************************************/
class CNFServerChangeLayer : public CCLayer , public CMsgReceiver ,public CMsgSender
{
protected:
	//标签
	enum 
	{
		enTagScrollLayer = 1,
	};

	int m_nCurrentServerID;

public:
	static CNFServerChangeLayer * CreateLayer(int nCurrentServerID,int nServerTotalNum);	//当前服务器ID，服务器总数量

	//消息接收
	virtual void RecMsg(int nMsgID,void* pInfo,int nSize);

protected:
	virtual bool InitLayer(int nCurrentServerID,int nServerTotalNum);     

	//按钮回调：返回
	void OnBtnCallBack(CCObject * pObj);
	//按钮回调：向左滑动
	void OnBtnLeftMove(CCObject * pObj);
	//按钮回调：向右滑动
	void OnBtnRightMove(CCObject * pObj);
};


#endif