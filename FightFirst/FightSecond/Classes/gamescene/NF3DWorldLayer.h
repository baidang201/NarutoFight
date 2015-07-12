#ifndef _NF_3D_WORLD_LAYER_HEAD_
#define	_NF_3D_WORLD_LAYER_HEAD_

#include "../publicdef/PublicDef.h"
#include "../gameunit/NFWorld3D.h"
#include "../datamanager/NFEnemyWave.h"

class CNF3DWorldLayer : public CNFWorld3DLayer ,public CMsgReceiver,public CMsgSender
{
public:
	//��ǩ
	enum
	{
		enTagMidTMXMap = 10000,
		enTagTMXMap1,
		enTagTMXMap2,
		enTagTMXMap3,
	};

	CCArray		* m_pArticleArray;			//��������
	CCArray		*m_pFriendArray;			//�ѷ�
	CCArray		*m_pEnemyArray;				//�з�

	int				m_nStageID;					//�ؿ�ID
	int				m_nRoleID;					//��ɫID
	CNFEnemyWave	* m_pEnemyWave;				//���˲���

public:
	static CNF3DWorldLayer * CreateLayer(int nStageID,int nRoleID);

	//���º���
	void update(float delta);

	//���NPC������NPC��ID�� 0Ϊû�е����NPC
	int ClickNPC(CCTouch *pTouch);

	//���պ���
	virtual void RecMsg(int enMsg,void* pData,int nSize);

protected:
	virtual bool InitLayer(int nStageID,int nRoleID);  

	//��Ϣ����:�Ƴ�����
	bool OnSubMsgEnemyRemove(void *pInfo,int nSize);
	//��Ϣ����:�Ƴ�����
	bool OnSubMsgProtagonistRemove(void *pInfo,int nSize);
	//��Ϣ����:������ͨ����
	bool OnSubMsgEnemyCommonAttack(void *pInfo,int nSize);
	//��Ϣ����:����
	bool OnSubMsgProtagonistSkill(void *pInfo,int nSize);
	//��Ϣ����:�л�����
	bool OnSubMsgTransfer(void *pInfo,int nSize);
	//��Ϣ����:��������
	bool OnSubMsgCreateEnemy(void *pInfo,int nSize);
	//��Ϣ����:��Ϸʤ��
	bool OnSubMsgGameWin(void *pInfo,int nSize);

	void onExit();
};













#endif