
//  http://www.9miao.com

#ifndef _NF_ROCKER_HEAD_
#define _NF_ROCKER_HEAD_

#include "../publicdef/PublicDef.h"
#include "NF3DWorldLayer.h"

/************************************************************************/
/*					ҡ��                                                                    */
/************************************************************************/
class CNFRockerLayer : public CCLayerColor
{
protected:
	//��ǩ
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
	CNF3DWorldLayer	  * m_p3DLayer;					//3D��ָ��
	CCPoint				m_ptTouching;				//��ǰ������
	float				m_fRockerSpeed;				//ҡ���ٶ�
	float				m_fRockerMaxDis;			//ҡ�����ƫ����
	int					m_nStageID;					//������ID

	bool				m_bIsTouching;				//�Ƿ����ڴ�����Ļ
	bool				m_bIsPublicCD;				//�Ƿ�����ִ�й���C

	int					m_nSkill_1_ID;				//����1ID
	int					m_nSkill_2_ID;				//����2ID
	int					m_nSkill_3_ID;				//����3ID
	int					m_nSkill_4_ID;				//����4ID
	int					m_nSkill_5_ID;				//����5ID

public:
	static CNFRockerLayer * CreateLayer(CNF3DWorldLayer * pLayer,int nStageID);

	//�õ��Ƿ�Ϊ���ڴ�����Ļ����Ϣ
	bool GetIsTouching(){return m_bIsTouching;};

protected:
	virtual bool InitLayer(CNF3DWorldLayer * pLayer,int nStageID); 

	//����
	bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	//���º���
	void update(float delta);

	//��ť�ص�
	void OnBtnCallBack(CCObject * pObj);

	//����CD�ص�
	void OnSkillPublicCallBack();

	void onExit();
};

#endif