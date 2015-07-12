#include "NFTMXTiledMap.h"
#include "../gameunit/article/NFBox.h"
//#include "NF3DWorldLayer.h"

CNFMidTMXMap* CNFMidTMXMap::CreateGameMap(const char* pFileName,CNF3DWorldLayer *p3DWorld)
{
	do 
	{
		CNFMidTMXMap* pPlayer = new CNFMidTMXMap;
		
		if (pPlayer && pPlayer->initWithTMXFile(pFileName))
		{
			
			pPlayer->Init(p3DWorld);
			
			pPlayer->autorelease();
			return pPlayer;
		}
		
		delete pPlayer;
	} while (false);
	CCLog("Fun CNFMidTMXMap::CreateGameMap Error!");
	return NULL;
}

bool CNFMidTMXMap::Init(CNF3DWorldLayer *p3DWorld)
{
	do 
	{

		return true;
	} while (false);
	CCLog("Fun CMGameMap::Init Error!");
	return false;
}

void CNFMidTMXMap::update()
{
	do 
	{

		return;
	} while (false);
	CCLog("fun CMGameMap::OnCallPerFrame Error!");
}

void CNFMidTMXMap::onExit()
{
	CCTMXTiledMap::onExit();
}

/////************************************************************************/
/////*				上下的map                                               */
/////************************************************************************/
CMGameMapLayer* CMGameMapLayer::CreateGameMap( const char* pFileName )
{
	do 
	{
		CMGameMapLayer* pPlayer = new CMGameMapLayer;
		if (pPlayer && pPlayer->initWithTMXFile(pFileName))
		{
			//pPlayer->Init();
			pPlayer->autorelease();
			return pPlayer;
		}
		delete pPlayer;
	} while (false);
	CCLog("Fun CMGameMap::CreateGameMap Error!");
	return NULL;
}

void CMGameMapLayer::OnCallPerFrame( float dt )
{

}