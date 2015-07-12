#ifndef _NF_LOADING_SCENE_HEAD_
#define _NF_LOADING_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*			Loading层                                                                    */
/************************************************************************/
class CNFLoadingLayer : public CCLayerColor
{
protected:
	//标签
	enum 
	{
		enTagPro = 1,
		enTagLight,
		enTagLabel,
	};

public:
	static CNFLoadingLayer * CreateLayer();

	//设置进度条
	void SetPro(float fRate);

protected:
	virtual bool init();     

	//触摸
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	virtual void onExit();

};


#endif