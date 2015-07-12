#ifndef _HM_SCROLL_LAYER_HEAD_
#define _HM_SCROLL_LAYER_HEAD_

#include "../publicdef/PublicDef.h"


/************************************************************************/
/*				������ѡ����棬������                                                                     */
/************************************************************************/
class CNFServerChangeScrollLayer : public CCLayerColor ,public CMsgSender
{
protected:
	// ��ǩ
	enum
	{
		enTagMenu = 1,
		enTagBtn = 100,
	};

	float			m_fTouchPassPosX;			//��һ�μ��㴥��ʱ����ָ������λ��
	float			m_fTouchBeginPosX;			//�û��������ʼ��λ��
	bool			m_bTouching;				//�Ƿ�ȡ���˴���(��֤�����ظ�����TouchEnd)
	bool			m_bIsTouchBtn;				//�Ƿ�������ť

	float			m_fPageWidth;				//ÿһҳ�Ŀ��
	float			m_fEndLayerPos;				//�����߽��λ��
	float			m_fStartNodePosX;			//��ʼ�ڵ�x��λ��

	CCRect			m_DisPlayRect;				//��ʾ����

public:
	static CNFServerChangeScrollLayer* CreateLayer(int nItemNum);

	//����
	void SetLeftMove();

	//���һ�
	void SetRightMove();

protected:
	bool InitLayer(int nItemNum);

	//���òü�����
	void visit();

	//��������
	virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
	virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);


	//��ť�ص�
	void OnBtnCallBack(CCObject * pObj);

	virtual void onExit();

};


#endif