#ifndef _CM_GAME_MAP_H_
#define _CM_GAME_MAP_H_

#include "../publicdef/PublicDef.h"
#include "../gamescene/NF3DWorldLayer.h"

class CNFMidTMXMap : public cocos2d::CCTMXTiledMap 
{ 
public:
	static CNFMidTMXMap* CreateGameMap(const char* pFileName,CNF3DWorldLayer *p3DWorld);
	
	//更新函数
	void update();

protected:
	virtual bool Init(CNF3DWorldLayer *p3DWorld);
	
	void onExit();
public:
	enum
	{
		enTagProtagonist = 1000,
	};

};

/************************************************************************/
/*				上下的Map                                                                     */
/************************************************************************/
class CMGameMapLayer : public CNFMidTMXMap 
{
protected:
	float			m_fMultipleSpeedX;
	//float			m_fDisPositionY;

public:
	static CMGameMapLayer* CreateGameMap(const char* pFileName);

	//x轴位移
	void SetSpeedMultipleX(float fMultipleSpeedX){m_fMultipleSpeedX = fMultipleSpeedX;}
	float GetSpeedMultipleX(){return m_fMultipleSpeedX;}

	////y轴
	//void SetDisPositionY(float fDisPositionY){m_fDisPositionY = fDisPositionY;} 
	//float GetDisPositionY(){return m_fDisPositionY;}

	virtual void OnCallPerFrame(float dt);
};

#endif