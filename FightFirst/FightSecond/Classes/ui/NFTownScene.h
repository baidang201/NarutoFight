#ifndef _NF_TOWN_SCENE_H_
#define _NF_TOWN_SCENE_H_

#include "../publicdef/PublicDef.h"


class CNFTownScene : public cocos2d::CCLayer ,public CMsgReceiver
{
protected:
	//==============================================================================��ʱ��Ӧwin32
	bool m_bIsPressW;						//win32�¶�Ӧ�ļ��̰����Ƿ񱻰��£���ʱ��
	bool m_bIsPressA;						//win32�¶�Ӧ�ļ��̰����Ƿ񱻰��£���ʱ��
	bool m_bIsPressS;						//win32�¶�Ӧ�ļ��̰����Ƿ񱻰��£���ʱ��
	bool m_bIsPressD;						//win32�¶�Ӧ�ļ��̰����Ƿ񱻰��£���ʱ��

	bool m_bIsPressJ;						//win32�¶�Ӧ�ļ��̰����Ƿ񱻰��£���ʱ��
	bool m_bIsPressK;						//win32�¶�Ӧ�ļ��̰����Ƿ񱻰��£���ʱ��
	bool m_bIsPressL;						//win32�¶�Ӧ�ļ��̰����Ƿ񱻰��£���ʱ��
	bool m_bIsPressI;						//win32�¶�Ӧ�ļ��̰����Ƿ񱻰��£���ʱ��

	int m_nRunSpace;						//�����������ʹ���ܵ�ʱ����
	float m_fFirectionOld;					//��һ���ƶ�����
	bool m_bIsRun;							//�Ƿ��ڱ���״̬
	//==============================================================================��ʱ��Ӧwin32 end

	bool		m_bBtnListSwitchOpen;				//���·���չ��ť�б���
	bool		m_bBtnListSwitchMoveOver;		//���·���չ��ť�б���������ʶ


	bool		m_bBtnFriendSwitchOpen;				//�ҷ�������չ��ť�б���
	bool		m_bBtnFriendSwitchMoveOver;		//�ҷ����Ѷ���������ʶ

	int			m_nStageID;					//��ǰ����ID
	int			m_nRoleID;					//��ǰ��ɫID

	int			m_nCurrentSkillUpRoleID;	//��ǰ����ҳ��ɫID��ֻ���ڼ�������ҳ�棩
	int			m_nCurrentSelectSkillID;	//��ǰѡ�м���ID��ֻ���ڼ�������ҳ�棩



public:
	//create
	static cocos2d::CCScene* CreateTownScene(int nStageID,int nRoleID);

	static CNFTownScene * CreateLayer(int nStageID,int nRoleID);

protected:
	//init
	virtual bool Init(int nStageID,int nRoleID);     

	//���º���
	void OnCallPerFrame(float dt);

	//����
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	//
	virtual void onEnter();
	virtual void onExit();

	//���պ���
	virtual void RecMsg(int enMsg,void* pData,int nSize);

	//�����ص��������������
	void OnHitResultOverCallBack();
	//��ʾ�������
	void ShowHitResult();

	//��ť�ص�
	void OnBtnCallBack(CCObject *pObj);

	//��UI���湦�ܰ�ť
	void OnExUiBtnListSwitch(CCObject *pSender, TouchEventType type);	//���½ǰ�ť���ػص�
	void OnTouchSwichMoveOver();										//��չ��ť����չ�����ص�

	//����[����]
	virtual void keyBackClicked();					//Android ���ؼ�
	void	OnCloseGame(CCObject *pSender, TouchEventType type);	



	//�л�����ҳ��ť����

	/************************************************************************/
	/* ��UI��չ��ť                                                          */
	/************************************************************************/

	//----------�����Ϣģ�����---------//
	void OnSwitchToPlayerInfoPage(CCObject *pSender, TouchEventType type);				//�л��������Ϣ��
	void OnTouchPlayerInfoPage(CCObject *pSender, TouchEventType type);					//�����Ϣ�㴥���ص�

	void OnClosePlayerInfoPage(CCObject *pSender, TouchEventType type);					//�����Ϣ�ر�
	void OnPlayerInfoItemBagPageViewEvent(CCObject* pSender, PageViewEventType type);	//�����Ϣ�������߻������¼��ص�

	void OnSwitchToBagPage(CCObject *pSender, TouchEventType type);						//�л�����ұ�����
	void OnSwitchToPlayerAtribuePage(CCObject *pSender, TouchEventType type);			//�л���������Բ�
	void OnSwitchToTitlePage(CCObject *pSender, TouchEventType type);					//�л�����ҳƺŲ�
	void OnEquClick(CCObject *pSender, TouchEventType type);							//װ����ť�ص�

	//----------����ҳ��ģ�����---------//
	void OnSwitchToSettingPage(CCObject *pSender, TouchEventType type);					//����ҳ��
	void OnMusicBtnClick(CCObject *pSender, TouchEventType type);						//��������
	void OnSoundBtnClickPage(CCObject *pSender, TouchEventType type);					//������Ч
	void OnSwitchToChangeRoleLayerClick(CCObject *pSender, TouchEventType type);		//��ת��ѡ�˽���
	void OnOnSwitchToLoginLayer(CCObject *pSender, TouchEventType type);				//��ת����¼����
	void OnHideOthersBtnClickPage(CCObject *pSender, TouchEventType type);				//���������������

	//----------����---------//
	void OnSwitchToSkill(CCObject *pSender, TouchEventType type);					//�л����콱��
	//----------����---------//
	void OnSwitchToNinjaSkill(CCObject *pSender, TouchEventType type);					//�л������ܲ�
	//----------����---------//
	void OnSwitchToBigSkill(CCObject *pSender, TouchEventType type);					//�л��������
	//----------����---------//
	void OnSwitchToTask(CCObject *pSender, TouchEventType type);						//�л��������
	//----------����---------//
	void OnSwitchToGuild(CCObject *pSender, TouchEventType type);					//�л�������� 
	//----------����---------//
	void OnSwitchToRank(CCObject *pSender, TouchEventType type);					//�л������в� 

	/************************************************************************/
	/* ��UI��ť                                                             */
	/************************************************************************/
	//----------��ȡ����---------//
	void OnSwitchToGetReward(CCObject *pSender, TouchEventType type);		//�л����콱��
	//----------����---------//
	void OnSwitchToGoldReward(CCObject *pSender, TouchEventType type);		//�л��������
	//----------��̨---------//
	void OnSwitchToPvP(CCObject *pSender, TouchEventType type);				//�л�����̨��
	//----------�---------//
	void OnSwitchToActive(CCObject *pSender, TouchEventType type);			//�л������
	//----------vip����---------//
	void OnSwitchToVip(CCObject *pSender, TouchEventType type);				//�л���vip�����
	//----------���˳齱---------//
	void OnSwitchToDraw(CCObject *pSender, TouchEventType type);			//�л������˳齱��
	//----------���Ѹ���---------//
	void OnSwitchToFirendFb(CCObject *pSender, TouchEventType type);		//�л������Ѹ�����
	//----------��������---------//
	void OnSwitchToShop(CCObject *pSender, TouchEventType type);			//�л����������˲�
	//----------���޾�����---------//
	void OnSwitchToPiePvP(CCObject *pSender, TouchEventType type);			//�л������޾�������
	//----------�ʼ�---------//
	void OnSwitchToEmail(CCObject *pSender, TouchEventType type);			//�л����ʼ���




	//----------С����---------//
	void OnSwitchToHelp(CCObject *pSender, TouchEventType type);			//�л���С���ֲ�
	//----------�Ի���---------//
	void OnSwitchTotalk(CCObject *pSender, TouchEventType type);			//�л����Ի����
	//----------��������---------//
	void OnSwitchToSkillUp(CCObject *pSender, TouchEventType type);			//�л�������������
	void OnChooseSkill(CCObject *pSender, TouchEventType type);				//ѡ����
	void OnChooseRole(CCObject *pSender, TouchEventType type);				//ѡ���ɫ
	void OnCreateChangeSkillLayer(CCObject *pSender, TouchEventType type);	//�����������������

	void OnRemoveSkillChangeLayer(CCObject *pSender, TouchEventType type);	//�Ƴ�����������
	void OnChangeSkillSelect(CCObject *pSender, TouchEventType type);		//ѡ������	  ��ѡ������ҳ��
	void OnChangeSkill(CCObject *pSender, TouchEventType type);				//����ѡ��������ѡ������ҳ��


	//----------װ������---------//
	void OnSwitchToEquUp(CCObject *pSender, TouchEventType type);			//�л���װ�������

	//----------��Ӣ����---------//
	void OnSwitchToExFbPage(CCObject *pSender, TouchEventType type);					//�л�����Ӣ������
	void OnExFbPageViewEvent(CCObject* pSender, PageViewEventType type);				//��Ӣ�����������¼��ص�
	void OnExFbClick(CCObject* pSender, TouchEventType type);							//������ť��Ӧ	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 






	//----------��ͨ����---------//
	void OnSwitchToNormalExFbPage();													//�л�����ͨ������
	void OnNormalFbPageViewEvent(CCObject* pSender, PageViewEventType type);			//��ͨ�����������¼��ص�
	void OnNormalFbClick(CCObject* pSender, TouchEventType type);						//��ͨ������ť��Ӧ	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	void OnNormalFbStartBattleClick(CCObject* pSender, TouchEventType type);			//��ͨ�������밴ť��Ӧ	
	void OnNormalFbFastBattleClick(CCObject* pSender, TouchEventType type);				//��ͨ����ɨ����ť��Ӧ
	void OnNormalFbInfoCloseClick(CCObject* pSender, TouchEventType type);				//�ر���ͨ��������


	//��ʱ������ս����ť
	void OnBtnBattleCallBack(CCObject *pSender, TouchEventType type);


	/************************************************************************/
	/* ����UI��ť                                                             */
	/************************************************************************/
	//----------���Ѱ�ť�ص�---------//
	void OnBtnFriendSwitchCallBack(CCObject *pSender, TouchEventType type);
	void OnBtnFriendCallBack(CCObject *pSender, TouchEventType type);
	void OnBtnArroundCallBack(CCObject *pSender, TouchEventType type);
	void OnBtnSearchCallBack(CCObject *pSender, TouchEventType type);

public:
	//��ǩ
	enum 
	{
		enTag3D = 100,	
		enTagRocker,
		enTagStudioMainUiLayer,			//����ť����
		enTagStudioCurrentChildUiLayer,	//��ǰ�ӹ��ܲ�
		enTagStudioTopUiLayer,			//������
		enTagRole,						//����
		enTagBtn = 10000,
	};
};


#endif