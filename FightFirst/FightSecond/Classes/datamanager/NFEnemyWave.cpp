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

		//�õ���������
		m_pEnemyArray = pEnemyArray;
		
		//����������Ϊ�գ���ȡ��һ������
		if (m_pEnemyArray->count() <= 0)
		{
			//�ж������޲�����Ϣ����Ϸ����
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
	//�������ε���ȫ���������������жϵ����Ƿ�����
	if (m_nEnemyNum <= 0)
	{
		bool	bIsEnemyAllDead = true;

		//�������ˣ��жϵ����Ƿ�����
		CCObject * pObj = NULL;
		CCARRAY_FOREACH(m_pEnemyArray,pObj)
		{
			CNFBasicPerson *pEnemy = dynamic_cast<CNFBasicPerson *>(pObj);
			if ( pEnemy!=NULL )
			{
				//�����е���û����
				if (pEnemy->IsDeath()==false)
				{
					bIsEnemyAllDead = false;
					break;
				}
			}
		}

		//������ȫ����������ȡ��һ������
		if (bIsEnemyAllDead==true)
		{
			m_nWaveID++;
			m_nWaveTime = 0;
			m_WaveInfo.clear();

			//�ж�������һ������Ϣ������Ϸ����
			if ( CNFStaticDataManager::SharedData()->GetWaveStaticInfo(m_nStageID,m_nWaveID,m_WaveInfo)==false )
			{
				//����Ϣ��������Ϸ
				SendMsg(enMsgGameWin,NULL,0);
				return false;
			}
			//�ж���������һ����Ϣ���������Ϸ
			else
			{
				m_nEnemyNum = m_WaveInfo.size();
			}
		}
	}

	//�������ε���δ�����������������������Ϣ������Ƿ��е��˴���
	for (CNFWaveItemInfoVec::iterator iter = m_WaveInfo.begin(); iter != m_WaveInfo.end();iter++)
	{	
		//���ﵽ���˴���ʱ�䣬�򴴽�����
		if (iter->nStartTime == m_nWaveTime)
		{    
			//���˴�����Ϣ
			Cmd_CreateEnemyInfo Info;
			Info.nEnemyID = iter->nEnemyID;
			Info.nEnemyLevel = iter->nEnemyLevel;
			Info.fPosX = iter->fCreatePosX;
			Info.fPosY = iter->fCreatePosY;

			//����Ϣ����������
			SendMsg(enMsgCreateEnemy,&Info,sizeof(Info));
			m_nEnemyNum--;
		};
	}

	m_nWaveTime++;

	return true;
}


