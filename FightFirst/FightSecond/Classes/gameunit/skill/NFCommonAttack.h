#ifndef _NF_COMMONATTACK_HEAD_
#define _NF_COMMONATTACK_HEAD_

#include "NFBasicSkill.h"
/************************************************************************/
/*			��ͨ�����ࣺ����������ͨ��������						    */
/************************************************************************/
class CNFCommonAttack :public CNFBasicSkill,public CMsgSender{
private:
	unsigned	int m_nExistTime;			//����ʱ�䣨֡����
	CCArray		*m_pHitOverArray;			//��ײ������
	tagTakeABeating m_tTakeABeatingInfo;	//��������Ϣ
public:
	//create��tCommonAttackInfo�����ܿ����Ϣ��pArrayTarget��Ŀ������
	static CNFCommonAttack* CreateCommonAttack(tagCommonAttack *tCommonAttackInfo,CCArray *pArrayTarget);
	//���º���
	virtual void CallPerFrame(float fT);
protected:
	//init��tCommonAttackInfo�����ܿ����Ϣ��pArrayTarget��Ŀ������
	bool InitCommonAttack(tagCommonAttack *tCommonAttackInfo,CCArray *pArrayTarget);
	//onExit
	virtual void onExit();
};
#endif