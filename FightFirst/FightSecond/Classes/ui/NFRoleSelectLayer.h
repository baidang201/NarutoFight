#ifndef _NF_ROLE_SELECT_SCENE_HEAD_
#define _NF_ROLE_SELECT_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*人物选择层															*/
/************************************************************************/
class CNFRoleSelectLayer : public CCLayer
{
protected:
	//标签
	enum 
	{
		enTagStudioMainUiLayer,
		enTagRole1,
		enTagRole2,
		enTagRole3,
	};

	int				m_nRoleID;			//角色ID	1.鸣人	2.小樱	3.佐助
	int				m_nRelaseRoleID;	//解锁角色ID	1.鸣人	2.小樱	3.佐助
public:
	static CNFRoleSelectLayer * CreateLayer();
	static CCScene * scene();

	void OnGameStartPage(CCObject *pSender, TouchEventType type);				//开始游戏

	void OnRoleSelect(CCObject *pSender, TouchEventType type);					//选择人物

	void OnFollowSelect(CCObject *pSender, TouchEventType type);				//选择随从

	void OnCancelFollowSelect(CCObject *pSender, TouchEventType type);			//取消随从

	void OnBack(CCObject *pSender, TouchEventType type);						//返回登录

	void OnShowReleaseDlg(CCObject *pSender, TouchEventType type);				//显示解锁对话框

	void OnHideReleaseDlg(CCObject *pSender, TouchEventType type);				//隐藏解锁对话框

	void OnBtnSureRelease(CCObject *pSender, TouchEventType type);				//确定解锁



protected:
	virtual bool init();     
};

#endif