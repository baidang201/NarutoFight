#ifndef _NF_SERVER_SELECT_SCENE_HEAD_
#define _NF_SERVER_SELECT_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*			��¼����													*/
/************************************************************************/
class CNFLoginScene : public CCLayer ,public CMsgReceiver
{
protected:
	//��ǩ
	enum 
	{
		enTagLoading = 1,
		enTagServerNameLabel,
		enTagStudioLoginLayer,		//��½��
		enTagStudioRegisterLayer,	//ע���
	};

	int			m_nLoadRecNum;				//��Դ������
	int			m_nLoadRecTotalNum;			//��Դ��������

	int			m_nCurrentServerID;			//��ǰ������ID
	int			m_nServerTotalNum;			//��������������

public:
	static CCScene * scene();

	static CNFLoginScene * CreateLayer();

	//��Ϣ����
	virtual void RecMsg(int nMsgID,void* pInfo,int nSize);

protected:
	virtual void onEnter();

	//��ԴԤ����
	void LoadRec();			

	void OnLoadRecCallBack(CCObject *pObj);		//�ص�����Դ����
//	void OnLoadLastRecCallBack(CCObject *pObj);	//�ص������һ����Դ����

	//�첽���ع�����Դ�Ļص�
	void OnLoadArmatureData(float percent);

	virtual void InitSceneUI();					//��������ظ�ͼ��ר�õ���Դ

	//---------------��¼ҳ��ť�ص�----------------
	void OnBtnEnterCallBack(CCObject *pSender, TouchEventType type);			//��ʼ��Ϸ
	void OnBtnServerSelectCallBack(CCObject *pSender, TouchEventType type);		//ѡ�������
	void OnBtnSwitchToRegisterCallBack(CCObject *pSender, TouchEventType type);	//��ת��ע��ҳ
	void OnBtnLoginCallBack(CCObject *pSender, TouchEventType type);			//��¼
	void OnAccountTextFieldEvent(CCObject *pSender, TextFiledEventType type);




	//---------------ע��ҳ��ť�ص�----------------
	void OnBackToLoginCallBack(CCObject *pSender, TouchEventType type);			//��ת����¼ҳ
	void OnBtnRegisterCallBack(CCObject *pSender, TouchEventType type);			//ע��
};


#endif