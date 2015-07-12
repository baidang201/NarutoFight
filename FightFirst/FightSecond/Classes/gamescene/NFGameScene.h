#ifndef _CM_GAME_SCENE_H_
#define _CM_GAME_SCENE_H_

#include "../publicdef/PublicDef.h"
#include "../gameunit/person/NFEnemy.h"
using namespace gui;

class CNFGameScene : public cocos2d::CCLayer ,public CMsgReceiver
{
protected:
	//==============================================================================临时对应win32
	bool m_bIsPressW;						//win32下对应的键盘按键是否被按下（临时）
	bool m_bIsPressA;						//win32下对应的键盘按键是否被按下（临时）
	bool m_bIsPressS;						//win32下对应的键盘按键是否被按下（临时）
	bool m_bIsPressD;						//win32下对应的键盘按键是否被按下（临时）

	bool m_bIsPressJ;						//win32下对应的键盘按键是否被按下（临时）
	bool m_bIsPressK;						//win32下对应的键盘按键是否被按下（临时）
	bool m_bIsPressL;						//win32下对应的键盘按键是否被按下（临时）
	bool m_bIsPressI;						//win32下对应的键盘按键是否被按下（临时）

	int m_nRunSpace;						//连续按方向键使奔跑的时间间隔
	float m_fFirectionOld;					//上一次移动方向
	bool m_bIsRun;							//是否处于奔跑状态
	//==============================================================================临时对应win32 end


	float		m_fNowHPProgress2;			//当前Hp2的百分比
	float		m_fNowEnemyHpProgress2;		//当前敌人Hp2的百分比
	CNFEnemy	* m_pTargetEnemy;			//被击中的目标敌人
	int			m_nEnemyBloodVisibleTime;	//敌人血条显示时间（帧）
	int			m_nEnemyBloodVisibleTimeTemp;//当前敌人血条显示时间（帧）

	int			m_nHitNum;					//连击数
	int			m_nHitTime;					//连击间隔时间
	int			m_nHitTimeTemp;				//当前连击时间

	int			m_nProgonistLevel;			//角色等级
	float		m_fProgonistExp;			//角色经验
	float		m_fProgonistExpLevel;		//角色升级所需经验

	bool		m_bBtnListSwitch;				//下方按钮列表开关
	bool		m_bBtnListSwitchMoveOver;		//下方按钮列表动画结束标识

	int			m_nStageID;					//战场ID
	int			m_nRoleID;					//角色ID

public:
	//create
	static cocos2d::CCScene* CreateGameScene(int nStageID,int nRoleID);

	static CNFGameScene * CreateLayer(int nStageID,int nRoleID);

protected:
	//init
    virtual bool Init(int nStageID,int nRoleID);     

	//更新函数
	void OnCallPerFrame(float dt);

	//触摸
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

	//
	virtual void onEnter();
    virtual void onExit();

	//接收函数
	virtual void RecMsg(int enMsg,void* pData,int nSize);

	//动作回调：连击结果结束
	void OnHitResultOverCallBack();
	//显示连击结果
	void ShowHitResult();


	//消息处理:游戏胜利
	bool OnSubMsgGameWin(void *pInfo,int nSize);
	//消息处理:游戏失败
	bool OnSubMsgGameOver(void *pInfo,int nSize);

	//按钮回调
	void OnBtnCallBack(CCObject *pObj);

	//回城按钮回调	
	void OnReturnToTownClick(CCObject* pSender, TouchEventType type);			


protected:
	//标签
	enum 
	{
		enTag3D = 100,	
		enTagRocker,
		enTagHPProgress2,
		enTagHPProgress1,
		enTagMPProgressChakela2,
		enTagMPProgressChakela1,
		enTagHPEnemyProgress2,
		enTagHPEnemyProgress1,
		enTagHPEnemyProgressBg,
		enTagHitNum,
		enTagHitResult,
		enTagMenu,
		enTagBtnProAICtrl,
		enTagBtnBackToTown,
		enTagLevel,
		enTagExpPro,
		enTagGameWinLayer,
		enMsgGameOverLayer,
	};
};


#endif