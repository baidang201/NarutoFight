#ifndef _NF_BASICUNIT_HEAD_
#define _NF_BASICUNIT_HEAD_
#include "../publicdef/PublicDef.h"
#include "NFWorld3D.h"

/************************************************************************/
/*			��λ���ࣺ��λ���������е�λ��ĸ��࣬���е�λ�̳���	    */
/************************************************************************/
//displayHPLable������
enum
{
	enDisplayHPLableCommon=1,		//��ͨHP��ʾ

	enDisplayHPLableCrit=2,			//����HP��ʾ

	enDisplayHPLableTreat=3,		//����HP��ʾ

	enDisplayHPLableMiss=4,			//miss

	enDisplayMPLableRecover=5,		//MP�ָ�
};

//��λ����:������������������
class CNFBasicUnit :public CNFUnit3D
{
private:
	
protected:
	
public:
	//ÿ���ڸ��º���(��ս�������ں����������)
	virtual void CallPerFrame(float fT);
	//����Ѫ������
	void displayHPLable(int sh,int type);
	//���Ŵ������
	void displayHitLable(int type);
protected:
	//��ʼ����fRate����б�ȡ�szSize�����
	virtual bool init(float fRate,tagSize3D szSize);
protected:
	enum
	{
		enTagDisplayHPLable=700,
	};
};
#endif