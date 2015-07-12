#ifndef __NFWORLD3D_H__
#define __NFWORLD3D_H__

#include "cocos2d.h"
USING_NS_CC;
#include <vector>
using namespace std;


//��ײλ��
enum enumIntersects
{
	enTagIntersects_NO = 0,
	enTagIntersects_LEFT = 1,
	enTagIntersects_RIGHT = 2,
	enTagIntersects_TOP = 3,
	enTagIntersects_DOWN = 4,
	enTagIntersects_UP = 5,
	enTagIntersects_BACK = 6,
};

//3D�����
struct tagPosition3D
{
	float	fX;				//x������
	float	fY;				//y������
	float	fZ;				//z������
};

//3Dʸ���������һ�µģ�
typedef tagPosition3D tagVector3D;

//3D���
struct tagSize3D
{
	float	fLength;		//�� ��Ӧ x
	float	fWidth;			//�� ��Ӧ y
	float	fHeight;		//�� ��Ӧ z
};

//�ռ�λ��״̬ ���� λ�� �����
struct tagCube3D
{
	tagPosition3D		ptPos3D;	//λ��
	tagSize3D			szCube;		//���
};

class CNFUnit3D;

//��ά��ײ�������
class CNFWorld3DLayer : public cocos2d::CCLayer 
{
protected:
	//��ǩ
	enum 
	{
		enTagEffectLayer = 100,
	};

private:
	bool				m_bIsLensFeatureOver;
protected:
	tagSize3D			m_ImpactSize;		//��ײ��� ��ΪWorld��Ψһ�ռ� ����������(0,0,0)

	float				m_fRate;			//α3D��б����,1Ϊ������ 0Ϊ������

	tagCube3D			m_ShotCube;			//��ͷ��Χ(�����Χ�ᾡ���������ǵ�λ��)
	
	CNFUnit3D*			m_pProtagonist;		//����ָ��

	bool				m_bIsBorderVisible;	//�Ƿ���Ƶ��Ա���
public:
	//create
	static CNFWorld3DLayer * CreateLayer(float rate,tagSize3D sz3DWorld);

	//��������
	void SetProtagonist(CNFUnit3D* pProtagonist);
	
	//��Ч����Ļ��(fDurTime:����ʱ�䡣 nDegree���𶯷��ȡ� nTimes���𶯴���)
	void WorldEffectTremble(float fDurTime,int nDegree,int nTimes);

	//��Ч�������ӽǡ�fPauseTime:��������ʱ�䡣pFocusOn���۽����ĵ�λ���ϡ�Ĭ��Ϊ�۽�����������
	void LensFeature(float fPauseTime,CNFUnit3D *pFocusOn = NULL);

	//�������Ĵ�С
	const tagSize3D* Get3DWorldSize(){return &m_ImpactSize;}

	//��Ч��ı���ɫ
	void SetScreenColor(int nColorID,float fTime);			//��ɫID����ɫʱ��


protected:
	//init
	virtual bool init(float rate,tagSize3D sz3DWorld);  
	
	//draw,ͬʱ��һЩ���²���
	virtual void draw();
private:
	//
	void OnLensFeatureCallBack();

	//�����ص�����Ч����ɫ�ı����
	void OnScreenChangeColorOver();			

};

/************************************************************************/
/*					��Ч��                                                                     */
/************************************************************************/
class CNFEffectLayer : public CCLayerColor
{
public:
	static CNFEffectLayer * CreateLayer();

protected:
	virtual bool InitLayer();  



};


/************************************************************************/
/* 3D������ײ��λ����                                                   */
/************************************************************************/
//action�Ľṹ��
struct tagActionInfo
{
	long			lActionTime;			//ʱ��
	tagVector3D		vec3dSpeed;				//�ٶ�ʸ��
};
typedef  vector<tagActionInfo> VecActionInfo;

struct tagScaleActionInfo
{
	long			lActionTime;			//ʱ��
	tagVector3D		vec3dScale;				//����ʸ��
};

typedef  vector<tagScaleActionInfo> VecScaleActionInfo;

//3D��λ������Ϣ
struct tagUnit3DBasicInfo
{
	tagCube3D	baseCube;
	float		fRate;			//��б�Ƕ���
};

//��ά��ײ����
class CNFUnit3D :public cocos2d::CCNode
{
private:
	float					m_fShadowW;				//Ӱ�ӿ�
	float					m_fShadowH;				//Ӱ�Ӹ�

	VecActionInfo			m_ActionInfoVec;		//����������
	VecScaleActionInfo		m_ScaleActionInfoVec;	//���Ŷ���������

	bool					m_bIsBorderVisible;		//�Ƿ���ʾ��ײ�߿�
	bool					m_bIsGravity;			//�Ƿ�������
	bool					m_bIsOnOutside;			//�Ƿ��������
protected:
	float					m_fRate;				//��б��
	tagCube3D				m_Cube3D;				//��ǰ����

	//����ģ�ⲿ��
	float					m_fZSpeed;				//z���ٶ�
	float					m_fSubtractSpeedl;		//ģ���������ٶ�
	int						m_nGround;				//����߶�
public:
	//create
	static CNFUnit3D* create(tagUnit3DBasicInfo unitInfo);
	//�Ƿ���ʾ��ײ�߿�
	void SetBorderVisible(bool bVisible);
	//�Ƿ�������,���������󽫲����ƶ����������¡�
	void SetGravityOpen(bool bGravityOpen);
	//�Ƿ���ʾӰ��
	void SetShadowVisible(bool bVisible);
	//���������С��λ��
	const tagCube3D* GetCurrentCube();
	//��������λ��
	virtual void setPosition3D(tagPosition3D ptPos3D);
	//������ײ���
	virtual void setSize3D(tagSize3D szNew);
	
	//�Ƿ���ײ������pInitiative��ײ���ĵ�λ��InsersectResult����ǰ��λ����ײ��λ����ײ�����˶����Ʒ���
	bool GetIntersectsVec(const CNFUnit3D* pInitiative,tagVector3D &InsersectResult);
	//�Ƿ���ײ������pInitiative��ײ���ĵ�λ��nBian�����ֵ
	int IntersectsLT_Z(CNFUnit3D* pInitiative,int nBian);
	//�Ƿ���ͶӰ��ײ������pInitiative��ײ���ĵ�λ��nBian�����ֵ
	bool IntersectsLT_TopProjection(CNFUnit3D* pInitiative,int nBian);

	//������б��0~1
	void SetLTRate(float fRate);
	//�����б��
	float GetLTRate();

	//��õ���߶�
	int GetGround();
	//���õ���߶�
	void SetGround(int nHeight);

	//����ģ���������ٶ�
	void SetLTSubtractSpeedl(float fSubtractSpeedl);
	//���ģ���������ٶ�
	float GetLTSubtractSpeedl();

	//����z���ٶ�
	void SetZSpeed(float zSpeed);
	//���z���ٶ�
	float GetZSpeed();

	//�����Ƿ��������
	void SetOnOutside(bool bIsOutside){m_bIsOnOutside = bIsOutside;};
	//����Ƿ��������
	float GetOnOutside(){return m_bIsOnOutside;};

	//��õ�λ���������ָ��
	CNFWorld3DLayer* getLTLayerParent();
	//�ƶ�������fTime��ʱ�䡣vec3D������ı仯����bRemoveSelfAfterAction�������������Ƿ�ɾ���õ�λ
	void MoveBy3D(float fTime,tagVector3D vec3D,bool bRemoveSelfAfterAction = false);
	//�ƶ���������fTime��ʱ�䡣ptTarget��Ŀ������㡣bRemoveSelfAfterAction�������������Ƿ�ɾ���õ�λ
	void MoveTo3D(float fTime,tagPosition3D ptTarget,bool bRemoveSelfAfterAction = false);
	//���ŵ�������fTime��ʱ�䡣szScaleTo��Ŀ�������bRemoveSelfAfterAction�������������Ƿ�ɾ���õ�λ
	void ScaleTo3D(float fTime,tagSize3D szScaleTo,bool bRemoveSelfAfterAction = false);
	//���Ŷ�����fTime��ʱ�䡣vec3D������ı仯����bRemoveSelfAfterAction�������������Ƿ�ɾ���õ�λ
	void ScaleBy3D(float fTime,tagVector3D vec3D,bool bRemoveSelfAfterAction = false);
	//ֹͣ�����ƶ�����
	void StopMoveAction();
	//ֹͣ�������Ŷ���
	void StopScaleAction();
protected:
	//init
	virtual bool init(tagUnit3DBasicInfo unitInfo);
	//onExit
	virtual void onExit();
	//draw,ͬʱ��һЩ���²���
	virtual void draw();
protected:
	enum
	{
		enTagLTBody=20000,
		enTagLTShadow,
	};
};

#endif 
