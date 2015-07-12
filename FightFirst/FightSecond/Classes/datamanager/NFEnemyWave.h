
//  http://www.9miao.com

#ifndef _NF_ENEMY_WAVE_HEAD_
#define _NF_ENEMY_WAVE_HEAD_

#include "../publicdef/PublicDef.h"
#include "NFDataManager.h"

/************************************************************************/
/*				���˲���������Ϣ                                                      */
/************************************************************************/
class CNFEnemyWave :public CMsgSender 
{
protected:
	int					m_nStageID;			//�ؿ�ID
	int					m_nWaveID;			//��ǰ����ID
	int					m_nWaveTime;		//��ǰ���ε�ʱ�䣺ÿˢ��һ�����ˣ�ʱ��Ҳˢ��
	int					m_nEnemyNum;		//��ǰ��������������0ʱ˵�������λ��е��ˣ�����0ʱ��˵���������޵�����
	
	CNFWaveItemInfoVec	m_WaveInfo;			//��ǰ������Ϣ
	CCArray				*m_pEnemyArray;		//��������

public:
	static CNFEnemyWave * CreateEnemyWave(int nStageID,CCArray * pEnemyArray);
	
	//���º���
	bool 	update();										

protected:
	//��ʼ��
	bool	InitEnemyWave(int nStageID,CCArray * pEnemyArray);		

};


#endif