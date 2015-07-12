#include "NFEnemyWave.h"

#include "../gameunit/person/NFBasicPerson.h"

CNFEnemyWave * CNFEnemyWave::CreateEnemyWave( int nStageID,CCArray * pEnemyArray )
{
	do
	{
		CNFEnemyWave * pSpr = new  CNFEnemyWave();
		CC_BREAK_IF(pSpr==NULL);
		CC_BREAK_IF(pSpr->InitEnemyWave(nStageID,pEnemyArray)==false);

		return pSpr;
	} while (false);
	CCLog("Fun CNFEnemyWave::CreateEnemyWave Error!");
	return NULL;
}


bool CNFEnemyWave::InitEnemyWave(int nStageID,CCArray * pEnemyArray)
{
	do 
	{
		m_nStageID = nStageID;
		m_nWaveID = 1;
		m_nWaveTime = 0;	
		m_nEnemyNum = 0;
		m_WaveInfo.clear();

		//得到敌人数组
		m_pEnemyArray = pEnemyArray;
		
		//若敌人数组为空，读取下一波敌人
		if (m_pEnemyArray->count() <= 0)
		{
			//判定：若无波次信息，游戏结束
			if ( CNFStaticDataManager::SharedData()->GetWaveStaticInfo(m_nStageID,m_nWaveID,m_WaveInfo)==false )
				break;
		}

		m_nEnemyNum = m_WaveInfo.size();

		return true;
	} while (false);
	CCLog("Fun CNFEnemyWave::InitEnemyWave Error!");
	return false;
}

bool CNFEnemyWave::update()
{
	//若本波次敌人全部创建结束，则判断敌人是否都死亡
	if (m_nEnemyNum <= 0)
	{
		bool	bIsEnemyAllDead = true;

		//遍历敌人，判断敌人是否都死亡
		CCObject * pObj = NULL;
		CCARRAY_FOREACH(m_pEnemyArray,pObj)
		{
			CNFBasicPerson *pEnemy = dynamic_cast<CNFBasicPerson *>(pObj);
			if ( pEnemy!=NULL )
			{
				//若还有敌人没死亡
				if (pEnemy->IsDeath()==false)
				{
					bIsEnemyAllDead = false;
					break;
				}
			}
		}

		//若敌人全部死亡，读取下一波敌人
		if (bIsEnemyAllDead==true)
		{
			m_nWaveID++;
			m_nWaveTime = 0;
			m_WaveInfo.clear();

			//判定：无下一波次信息，则游戏结束
			if ( CNFStaticDataManager::SharedData()->GetWaveStaticInfo(m_nStageID,m_nWaveID,m_WaveInfo)==false )
			{
				//发消息，结束游戏
				SendMsg(enMsgGameWin,NULL,0);
				return false;
			}
			//判定：若有下一波信息，则继续游戏
			else
			{
				m_nEnemyNum = m_WaveInfo.size();
			}
		}
	}

	//若本波次敌人未创建结束，则遍历本波次信息，检查是否有敌人创建
	for (CNFWaveItemInfoVec::iterator iter = m_WaveInfo.begin(); iter != m_WaveInfo.end();iter++)
	{	
		//若达到敌人创建时间，则创建敌人
		if (iter->nStartTime == m_nWaveTime)
		{    
			//敌人创建信息
			Cmd_CreateEnemyInfo Info;
			Info.nEnemyID = iter->nEnemyID;
			Info.nEnemyLevel = iter->nEnemyLevel;
			Info.fPosX = iter->fCreatePosX;
			Info.fPosY = iter->fCreatePosY;

			//发消息，创建敌人
			SendMsg(enMsgCreateEnemy,&Info,sizeof(Info));
			m_nEnemyNum--;
		};
	}

	m_nWaveTime++;

	return true;
}


