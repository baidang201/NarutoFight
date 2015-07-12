#ifndef _NF_SERVER_CHANGE_LAYER_HEAD_
#define _NF_SERVER_CHANGE_LAYER_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*			�������л���                                                                    */
/************************************************************************/
class CNFServerChangeLayer : public CCLayer , public CMsgReceiver ,public CMsgSender
{
protected:
	//��ǩ
	enum 
	{
		enTagScrollLayer = 1,
	};

	int m_nCurrentServerID;

public:
	static CNFServerChangeLayer * CreateLayer(int nCurrentServerID,int nServerTotalNum);	//��ǰ������ID��������������

	//��Ϣ����
	virtual void RecMsg(int nMsgID,void* pInfo,int nSize);

protected:
	virtual bool InitLayer(int nCurrentServerID,int nServerTotalNum);     

	//��ť�ص�������
	void OnBtnCallBack(CCObject * pObj);
	//��ť�ص������󻬶�
	void OnBtnLeftMove(CCObject * pObj);
	//��ť�ص������һ���
	void OnBtnRightMove(CCObject * pObj);
};


#endif