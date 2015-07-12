#ifndef _NF_ROLE_SELECT_SCENE_HEAD_
#define _NF_ROLE_SELECT_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*����ѡ���															*/
/************************************************************************/
class CNFRoleSelectLayer : public CCLayer
{
protected:
	//��ǩ
	enum 
	{
		enTagStudioMainUiLayer,
		enTagRole1,
		enTagRole2,
		enTagRole3,
	};

	int				m_nRoleID;			//��ɫID	1.����	2.Сӣ	3.����
	int				m_nRelaseRoleID;	//������ɫID	1.����	2.Сӣ	3.����
public:
	static CNFRoleSelectLayer * CreateLayer();
	static CCScene * scene();

	void OnGameStartPage(CCObject *pSender, TouchEventType type);				//��ʼ��Ϸ

	void OnRoleSelect(CCObject *pSender, TouchEventType type);					//ѡ������

	void OnFollowSelect(CCObject *pSender, TouchEventType type);				//ѡ�����

	void OnCancelFollowSelect(CCObject *pSender, TouchEventType type);			//ȡ�����

	void OnBack(CCObject *pSender, TouchEventType type);						//���ص�¼

	void OnShowReleaseDlg(CCObject *pSender, TouchEventType type);				//��ʾ�����Ի���

	void OnHideReleaseDlg(CCObject *pSender, TouchEventType type);				//���ؽ����Ի���

	void OnBtnSureRelease(CCObject *pSender, TouchEventType type);				//ȷ������



protected:
	virtual bool init();     
};

#endif