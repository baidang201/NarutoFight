#ifndef _NF_TOWN_SCENE_H_
#define _NF_TOWN_SCENE_H_

#include "../publicdef/PublicDef.h"


class CNFTownScene : public cocos2d::CCLayer ,public CMsgReceiver
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

	bool		m_bBtnListSwitchOpen;				//右下方扩展按钮列表开关
	bool		m_bBtnListSwitchMoveOver;		//右下方扩展按钮列表动画结束标识


	bool		m_bBtnFriendSwitchOpen;				//右方好友扩展按钮列表开关
	bool		m_bBtnFriendSwitchMoveOver;		//右方好友动画结束标识

	int			m_nStageID;					//当前城镇ID
	int			m_nRoleID;					//当前角色ID

	int			m_nCurrentSkillUpRoleID;	//当前技能页角色ID（只用于技能升级页面）
	int			m_nCurrentSelectSkillID;	//当前选中技能ID（只用于技能升级页面）



public:
	//create
	static cocos2d::CCScene* CreateTownScene(int nStageID,int nRoleID);

	static CNFTownScene * CreateLayer(int nStageID,int nRoleID);

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

	//按钮回调
	void OnBtnCallBack(CCObject *pObj);

	//主UI界面功能按钮
	void OnExUiBtnListSwitch(CCObject *pSender, TouchEventType type);	//右下角按钮开关回调
	void OnTouchSwichMoveOver();										//扩展按钮栏伸展动画回调

	//重载[返回]
	virtual void keyBackClicked();					//Android 返回键
	void	OnCloseGame(CCObject *pSender, TouchEventType type);	



	//切换功能页按钮函数

	/************************************************************************/
	/* 主UI扩展按钮                                                          */
	/************************************************************************/

	//----------玩家信息模块相关---------//
	void OnSwitchToPlayerInfoPage(CCObject *pSender, TouchEventType type);				//切换到玩家信息层
	void OnTouchPlayerInfoPage(CCObject *pSender, TouchEventType type);					//玩家信息层触碰回调

	void OnClosePlayerInfoPage(CCObject *pSender, TouchEventType type);					//玩家信息关闭
	void OnPlayerInfoItemBagPageViewEvent(CCObject* pSender, PageViewEventType type);	//玩家信息背包道具滑动条事件回调

	void OnSwitchToBagPage(CCObject *pSender, TouchEventType type);						//切换到玩家背包层
	void OnSwitchToPlayerAtribuePage(CCObject *pSender, TouchEventType type);			//切换到玩家属性层
	void OnSwitchToTitlePage(CCObject *pSender, TouchEventType type);					//切换到玩家称号层
	void OnEquClick(CCObject *pSender, TouchEventType type);							//装备按钮回调

	//----------设置页面模块相关---------//
	void OnSwitchToSettingPage(CCObject *pSender, TouchEventType type);					//设置页面
	void OnMusicBtnClick(CCObject *pSender, TouchEventType type);						//设置音乐
	void OnSoundBtnClickPage(CCObject *pSender, TouchEventType type);					//设置音效
	void OnSwitchToChangeRoleLayerClick(CCObject *pSender, TouchEventType type);		//跳转到选人界面
	void OnOnSwitchToLoginLayer(CCObject *pSender, TouchEventType type);				//跳转到登录界面
	void OnHideOthersBtnClickPage(CCObject *pSender, TouchEventType type);				//设置隐藏其他玩家

	//----------技能---------//
	void OnSwitchToSkill(CCObject *pSender, TouchEventType type);					//切换到领奖层
	//----------忍术---------//
	void OnSwitchToNinjaSkill(CCObject *pSender, TouchEventType type);					//切换到技能层
	//----------奥义---------//
	void OnSwitchToBigSkill(CCObject *pSender, TouchEventType type);					//切换到奥义层
	//----------任务---------//
	void OnSwitchToTask(CCObject *pSender, TouchEventType type);						//切换到任务层
	//----------公会---------//
	void OnSwitchToGuild(CCObject *pSender, TouchEventType type);					//切换到公会层 
	//----------排行---------//
	void OnSwitchToRank(CCObject *pSender, TouchEventType type);					//切换到排行层 

	/************************************************************************/
	/* 主UI按钮                                                             */
	/************************************************************************/
	//----------获取奖励---------//
	void OnSwitchToGetReward(CCObject *pSender, TouchEventType type);		//切换到领奖层
	//----------宝藏---------//
	void OnSwitchToGoldReward(CCObject *pSender, TouchEventType type);		//切换到宝箱层
	//----------擂台---------//
	void OnSwitchToPvP(CCObject *pSender, TouchEventType type);				//切换到擂台层
	//----------活动---------//
	void OnSwitchToActive(CCObject *pSender, TouchEventType type);			//切换到活动层
	//----------vip大礼---------//
	void OnSwitchToVip(CCObject *pSender, TouchEventType type);				//切换到vip大礼层
	//----------幸运抽奖---------//
	void OnSwitchToDraw(CCObject *pSender, TouchEventType type);			//切换到幸运抽奖层
	//----------好友副本---------//
	void OnSwitchToFirendFb(CCObject *pSender, TouchEventType type);		//切换到好友副本层
	//----------神秘商人---------//
	void OnSwitchToShop(CCObject *pSender, TouchEventType type);			//切换到神秘商人层
	//----------幻兽竞技场---------//
	void OnSwitchToPiePvP(CCObject *pSender, TouchEventType type);			//切换到幻兽竞技场层
	//----------邮件---------//
	void OnSwitchToEmail(CCObject *pSender, TouchEventType type);			//切换到邮件层




	//----------小助手---------//
	void OnSwitchToHelp(CCObject *pSender, TouchEventType type);			//切换到小助手层
	//----------对话框---------//
	void OnSwitchTotalk(CCObject *pSender, TouchEventType type);			//切换到对话框层
	//----------技能升级---------//
	void OnSwitchToSkillUp(CCObject *pSender, TouchEventType type);			//切换到技能升级层
	void OnChooseSkill(CCObject *pSender, TouchEventType type);				//选择技能
	void OnChooseRole(CCObject *pSender, TouchEventType type);				//选择角色
	void OnCreateChangeSkillLayer(CCObject *pSender, TouchEventType type);	//弹出更换忍术奥义层

	void OnRemoveSkillChangeLayer(CCObject *pSender, TouchEventType type);	//移除更换忍术层
	void OnChangeSkillSelect(CCObject *pSender, TouchEventType type);		//选择忍术	  （选择忍术页）
	void OnChangeSkill(CCObject *pSender, TouchEventType type);				//更换选择忍术（选择忍术页）


	//----------装备改造---------//
	void OnSwitchToEquUp(CCObject *pSender, TouchEventType type);			//切换到装备改造层

	//----------精英副本---------//
	void OnSwitchToExFbPage(CCObject *pSender, TouchEventType type);					//切换到精英副本层
	void OnExFbPageViewEvent(CCObject* pSender, PageViewEventType type);				//精英副本滑动条事件回调
	void OnExFbClick(CCObject* pSender, TouchEventType type);							//副本按钮响应	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 






	//----------普通副本---------//
	void OnSwitchToNormalExFbPage();													//切换到普通副本层
	void OnNormalFbPageViewEvent(CCObject* pSender, PageViewEventType type);			//普通副本滑动条事件回调
	void OnNormalFbClick(CCObject* pSender, TouchEventType type);						//普通副本按钮响应	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
	void OnNormalFbStartBattleClick(CCObject* pSender, TouchEventType type);			//普通副本进入按钮响应	
	void OnNormalFbFastBattleClick(CCObject* pSender, TouchEventType type);				//普通副本扫荡按钮响应
	void OnNormalFbInfoCloseClick(CCObject* pSender, TouchEventType type);				//关闭普通副本详情


	//临时：进入战场按钮
	void OnBtnBattleCallBack(CCObject *pSender, TouchEventType type);


	/************************************************************************/
	/* 好友UI按钮                                                             */
	/************************************************************************/
	//----------好友按钮回调---------//
	void OnBtnFriendSwitchCallBack(CCObject *pSender, TouchEventType type);
	void OnBtnFriendCallBack(CCObject *pSender, TouchEventType type);
	void OnBtnArroundCallBack(CCObject *pSender, TouchEventType type);
	void OnBtnSearchCallBack(CCObject *pSender, TouchEventType type);

public:
	//标签
	enum 
	{
		enTag3D = 100,	
		enTagRocker,
		enTagStudioMainUiLayer,			//主按钮栏层
		enTagStudioCurrentChildUiLayer,	//当前子功能层
		enTagStudioTopUiLayer,			//弹出层
		enTagRole,						//人物
		enTagBtn = 10000,
	};
};


#endif