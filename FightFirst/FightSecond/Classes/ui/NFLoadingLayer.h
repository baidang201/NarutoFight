#ifndef _NF_LOADING_SCENE_HEAD_
#define _NF_LOADING_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*			Loading��                                                                    */
/************************************************************************/
class CNFLoadingLayer : public CCLayerColor
{
protected:
	//��ǩ
	enum 
	{
		enTagPro = 1,
		enTagLight,
		enTagLabel,
	};

public:
	static CNFLoadingLayer * CreateLayer();

	//���ý�����
	void SetPro(float fRate);

protected:
	virtual bool init();     

	//����
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);

	virtual void onExit();

};


#endif