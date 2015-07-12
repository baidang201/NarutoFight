
//  http://www.9miao.com

#ifndef _NF_ENEMY_WAVE_HEAD_
#define _NF_ENEMY_WAVE_HEAD_

#include "../publicdef/PublicDef.h"
#include "NFDataManager.h"

/************************************************************************/
/*				敌人产生波次信息                                                      */
/************************************************************************/
class CNFEnemyWave :public CMsgSender 
{
protected:
	int					m_nStageID;			//关卡ID
	int					m_nWaveID;			//当前波次ID
	int					m_nWaveTime;		//当前波次的时间：每刷新一波敌人，时间也刷新
	int					m_nEnemyNum;		//当前敌人数量：大于0时说明本波次还有敌人，等于0时，说明本波次无敌人了
	
	CNFWaveItemInfoVec	m_WaveInfo;			//当前波次信息
	CCArray				*m_pEnemyArray;		//敌人数组

public:
	static CNFEnemyWave * CreateEnemyWave(int nStageID,CCArray * pEnemyArray);
	
	//更新函数
	bool 	update();										

protected:
	//初始化
	bool	InitEnemyWave(int nStageID,CCArray * pEnemyArray);		

};


#endif