#ifndef _NF_LOGO_SCENE_HEAD_
#define _NF_LOGO_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*			Logo场景                                                                    */
/************************************************************************/
class CNFLogoScene : public CCLayer
{
public:
	static CCScene * scene();

	static CNFLogoScene * CreateLayer();

protected:
	virtual bool init();     

	//临时：切换场景
	void OnChangeScene();
	
	////临时：提示层
	//void OnAddTipLayer();

};


#endif