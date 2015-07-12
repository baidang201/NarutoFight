#ifndef _NF_TEAM_CREATE_SCENE_HEAD_
#define _NF_TEAM_CREATE_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*�����̶Ӳ�															*/
/************************************************************************/
class CNFTeamCreateLayer : public CCLayer
{
protected:
	//��ǩ
	enum 
	{
		enTagStudioMainUiLayer,
		enTagRole,
	};

	int				m_nRoleID;			//��ɫID	1.����	2.Сӣ	3.����

public:
	static CNFTeamCreateLayer * CreateLayer();
	static CCScene * scene();

	void OnGameStartPage(CCObject *pSender, TouchEventType type);				//��������

    void textFieldEvent(CCObject* pSender, TextFiledEventType type);

	void OnBackToLoginPage(CCObject *pSender, TouchEventType type);				//���ص�¼����


	void OnRoleSelect(CCObject *pSender, TouchEventType type);					//��ɫѡ�����

protected:
	virtual bool init();     
};

#endif