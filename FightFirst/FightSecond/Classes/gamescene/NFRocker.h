
//  http://www.9miao.com

#ifndef _NF_ROCKER_HEAD_
#define _NF_ROCKER_HEAD_

#include "../publicdef/PublicDef.h"
#include "NF3DWorldLayer.h"

/************************************************************************/
/*					摇杆                                                                    */
/************************************************************************/
class CNFRockerLayer : public CCLayerColor
{
protected:
	//标签
	enum
	{
		enTagBtnCommonAttack = 100,
		enTagBtnSkill1 = 101,
		enTagBtnSkill2,
		enTagBtnSkill3,
		enTagBtnSkill4,
		enTagBtnSkill5,
		enTagRockerBg = 1000,
		enTagRocker,
		enTagMenu,
		enTagCommonAttackCD,
		enTagCommonAttackFont,
	};

protected:
	CNF3DWorldLayer	  * m_p3DLayer;					//3D层指针
	CCPoint				m_ptTouching;				//当前触摸点
	float				m_fRockerSpeed;				//摇杆速度
	float				m_fRockerMaxDis;			//摇杆最大偏移量
	int					m_nStageID;					//场景的ID

	bool				m_bIsTouching;				//是否正在触摸屏幕
	bool				m_bIsPublicCD;				//是否正在执行公共C

	int					m_nSkill_1_ID;				//技能1ID
	int					m_nSkill_2_ID;				//技能2ID
	int					m_nSkill_3_ID;				//技能3ID
	int					m_nSkill_4_ID;				//技能4ID
	int					m_nSkill_5_ID;				//技能5ID

public:
	static CNFRockerLayer * CreateLayer(CNF3DWorldLayer * pLayer,int nStageID);

	//得到是否为正在触摸屏幕的信息
	bool GetIsTouching(){return m_bIsTouching;};

protected:
	virtual bool InitLayer(CNF3DWorldLayer * pLayer,int nStageID); 

	//触摸
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	//更新函数
	void update(float delta);

	//按钮回调
	void OnBtnCallBack(CCObject * pObj);

	//公共CD回调
	void OnSkillPublicCallBack();

	void onExit();
};

#endif