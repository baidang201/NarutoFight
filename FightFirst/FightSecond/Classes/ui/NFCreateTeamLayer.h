#ifndef _NF_TEAM_CREATE_SCENE_HEAD_
#define _NF_TEAM_CREATE_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*创建忍队层															*/
/************************************************************************/
class CNFTeamCreateLayer : public CCLayer
{
protected:
	//标签
	enum 
	{
		enTagStudioMainUiLayer,
		enTagRole,
	};

	int				m_nRoleID;			//角色ID	1.鸣人	2.小樱	3.佐助

public:
	static CNFTeamCreateLayer * CreateLayer();
	static CCScene * scene();

	void OnGameStartPage(CCObject *pSender, TouchEventType type);				//创建人物

    void textFieldEvent(CCObject* pSender, TextFiledEventType type);

	void OnBackToLoginPage(CCObject *pSender, TouchEventType type);				//返回登录界面


	void OnRoleSelect(CCObject *pSender, TouchEventType type);					//角色选择界面

protected:
	virtual bool init();     
};

#endif