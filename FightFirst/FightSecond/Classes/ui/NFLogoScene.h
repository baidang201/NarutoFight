#ifndef _NF_LOGO_SCENE_HEAD_
#define _NF_LOGO_SCENE_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*			Logo����                                                                    */
/************************************************************************/
class CNFLogoScene : public CCLayer
{
public:
	static CCScene * scene();

	static CNFLogoScene * CreateLayer();

protected:
	virtual bool init();     

	//��ʱ���л�����
	void OnChangeScene();
	
	////��ʱ����ʾ��
	//void OnAddTipLayer();

};


#endif