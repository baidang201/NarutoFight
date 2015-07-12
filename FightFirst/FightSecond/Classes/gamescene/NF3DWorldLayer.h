#ifndef _NF_3D_WORLD_LAYER_HEAD_
#define	_NF_3D_WORLD_LAYER_HEAD_

#include "../publicdef/PublicDef.h"
#include "../gameunit/NFWorld3D.h"
#include "../datamanager/NFEnemyWave.h"

class CNF3DWorldLayer : public CNFWorld3DLayer ,public CMsgReceiver,public CMsgSender
{
public:
	//标签
	enum
	{
		enTagMidTMXMap = 10000,
		enTagTMXMap1,
		enTagTMXMap2,
		enTagTMXMap3,
	};

	CCArray		* m_pArticleArray;			//物体数组
	CCArray		*m_pFriendArray;			//友方
	CCArray		*m_pEnemyArray;				//敌方

	int				m_nStageID;					//关卡ID
	int				m_nRoleID;					//角色ID
	CNFEnemyWave	* m_pEnemyWave;				//敌人波次

public:
	static CNF3DWorldLayer * CreateLayer(int nStageID,int nRoleID);

	//更新函数
	void update(float delta);

	//点击NPC。返回NPC的ID， 0为没有点击到NPC
	int ClickNPC(CCTouch *pTouch);

	//接收函数
	virtual void RecMsg(int enMsg,void* pData,int nSize);

protected:
	virtual bool InitLayer(int nStageID,int nRoleID);  

	//消息处理:移除敌人
	bool OnSubMsgEnemyRemove(void *pInfo,int nSize);
	//消息处理:移除主角
	bool OnSubMsgProtagonistRemove(void *pInfo,int nSize);
	//消息处理:敌人普通攻击
	bool OnSubMsgEnemyCommonAttack(void *pInfo,int nSize);
	//消息处理:技能
	bool OnSubMsgProtagonistSkill(void *pInfo,int nSize);
	//消息处理:切换场景
	bool OnSubMsgTransfer(void *pInfo,int nSize);
	//消息处理:创建敌人
	bool OnSubMsgCreateEnemy(void *pInfo,int nSize);
	//消息处理:游戏胜利
	bool OnSubMsgGameWin(void *pInfo,int nSize);

	void onExit();
};













#endif