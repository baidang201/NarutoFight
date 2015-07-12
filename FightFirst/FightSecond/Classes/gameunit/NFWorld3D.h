#ifndef __NFWORLD3D_H__
#define __NFWORLD3D_H__

#include "cocos2d.h"
USING_NS_CC;
#include <vector>
using namespace std;


//碰撞位置
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

//3D坐标点
struct tagPosition3D
{
	float	fX;				//x轴坐标
	float	fY;				//y轴坐标
	float	fZ;				//z轴坐标
};

//3D矢量（与点是一致的）
typedef tagPosition3D tagVector3D;

//3D体积
struct tagSize3D
{
	float	fLength;		//长 对应 x
	float	fWidth;			//宽 对应 y
	float	fHeight;		//高 对应 z
};

//空间位置状态 包含 位置 和体积
struct tagCube3D
{
	tagPosition3D		ptPos3D;	//位置
	tagSize3D			szCube;		//体积
};

class CNFUnit3D;

//三维碰撞世界基类
class CNFWorld3DLayer : public cocos2d::CCLayer 
{
protected:
	//标签
	enum 
	{
		enTagEffectLayer = 100,
	};

private:
	bool				m_bIsLensFeatureOver;
protected:
	tagSize3D			m_ImpactSize;		//碰撞体积 因为World是唯一空间 它的坐标是(0,0,0)

	float				m_fRate;			//伪3D倾斜度数,1为纯俯视 0为纯侧视

	tagCube3D			m_ShotCube;			//镜头范围(这个范围会尽量锁定主角的位置)
	
	CNFUnit3D*			m_pProtagonist;		//主角指针

	bool				m_bIsBorderVisible;	//是否绘制调试边线
public:
	//create
	static CNFWorld3DLayer * CreateLayer(float rate,tagSize3D sz3DWorld);

	//设置主角
	void SetProtagonist(CNFUnit3D* pProtagonist);
	
	//特效：屏幕震动(fDurTime:持续时间。 nDegree：震动幅度。 nTimes：震动次数)
	void WorldEffectTremble(float fDurTime,int nDegree,int nTimes);

	//特效：拉近视角。fPauseTime:拉近所用时间。pFocusOn：聚焦到的单位身上。默认为聚焦到主角身上
	void LensFeature(float fPauseTime,CNFUnit3D *pFocusOn = NULL);

	//获得世界的大小
	const tagSize3D* Get3DWorldSize(){return &m_ImpactSize;}

	//特效层改变颜色
	void SetScreenColor(int nColorID,float fTime);			//颜色ID，变色时间


protected:
	//init
	virtual bool init(float rate,tagSize3D sz3DWorld);  
	
	//draw,同时做一些更新操作
	virtual void draw();
private:
	//
	void OnLensFeatureCallBack();

	//动作回调：特效层颜色改变结束
	void OnScreenChangeColorOver();			

};

/************************************************************************/
/*					特效层                                                                     */
/************************************************************************/
class CNFEffectLayer : public CCLayerColor
{
public:
	static CNFEffectLayer * CreateLayer();

protected:
	virtual bool InitLayer();  



};


/************************************************************************/
/* 3D基本碰撞单位声明                                                   */
/************************************************************************/
//action的结构体
struct tagActionInfo
{
	long			lActionTime;			//时间
	tagVector3D		vec3dSpeed;				//速度矢量
};
typedef  vector<tagActionInfo> VecActionInfo;

struct tagScaleActionInfo
{
	long			lActionTime;			//时间
	tagVector3D		vec3dScale;				//缩放矢量
};

typedef  vector<tagScaleActionInfo> VecScaleActionInfo;

//3D单位基础信息
struct tagUnit3DBasicInfo
{
	tagCube3D	baseCube;
	float		fRate;			//倾斜角度数
};

//三维碰撞基类
class CNFUnit3D :public cocos2d::CCNode
{
private:
	float					m_fShadowW;				//影子宽
	float					m_fShadowH;				//影子高

	VecActionInfo			m_ActionInfoVec;		//动作的序列
	VecScaleActionInfo		m_ScaleActionInfoVec;	//缩放动作的序列

	bool					m_bIsBorderVisible;		//是否显示碰撞边框
	bool					m_bIsGravity;			//是否开启重力
	bool					m_bIsOnOutside;			//是否允许出界
protected:
	float					m_fRate;				//倾斜度
	tagCube3D				m_Cube3D;				//当前区域

	//重力模拟部分
	float					m_fZSpeed;				//z轴速度
	float					m_fSubtractSpeedl;		//模拟重力减速度
	int						m_nGround;				//地面高度
public:
	//create
	static CNFUnit3D* create(tagUnit3DBasicInfo unitInfo);
	//是否显示碰撞边框
	void SetBorderVisible(bool bVisible);
	//是否开启重力,开启重力后将不能移动到地面以下。
	void SetGravityOpen(bool bGravityOpen);
	//是否显示影子
	void SetShadowVisible(bool bVisible);
	//返回物体大小、位置
	const tagCube3D* GetCurrentCube();
	//设置坐标位置
	virtual void setPosition3D(tagPosition3D ptPos3D);
	//设置碰撞体积
	virtual void setSize3D(tagSize3D szNew);
	
	//是否被碰撞方法。pInitiative：撞击的单位。InsersectResult：当前单位（被撞单位）被撞击后运动趋势方向。
	bool GetIntersectsVec(const CNFUnit3D* pInitiative,tagVector3D &InsersectResult);
	//是否被碰撞方法。pInitiative：撞击的单位。nBian：误差值
	int IntersectsLT_Z(CNFUnit3D* pInitiative,int nBian);
	//是否俯视投影碰撞方法。pInitiative：撞击的单位。nBian：误差值
	bool IntersectsLT_TopProjection(CNFUnit3D* pInitiative,int nBian);

	//设置倾斜度0~1
	void SetLTRate(float fRate);
	//获得倾斜度
	float GetLTRate();

	//获得地面高度
	int GetGround();
	//设置地面高度
	void SetGround(int nHeight);

	//设置模拟重力减速度
	void SetLTSubtractSpeedl(float fSubtractSpeedl);
	//获得模拟重力减速度
	float GetLTSubtractSpeedl();

	//设置z轴速度
	void SetZSpeed(float zSpeed);
	//获得z轴速度
	float GetZSpeed();

	//设置是否允许出界
	void SetOnOutside(bool bIsOutside){m_bIsOnOutside = bIsOutside;};
	//获得是否允许出界
	float GetOnOutside(){return m_bIsOnOutside;};

	//获得单位所在世界的指针
	CNFWorld3DLayer* getLTLayerParent();
	//移动动作。fTime：时间。vec3D：坐标的变化量。bRemoveSelfAfterAction：动作结束后是否删除该单位
	void MoveBy3D(float fTime,tagVector3D vec3D,bool bRemoveSelfAfterAction = false);
	//移动到动作。fTime：时间。ptTarget：目标坐标点。bRemoveSelfAfterAction：动作结束后是否删除该单位
	void MoveTo3D(float fTime,tagPosition3D ptTarget,bool bRemoveSelfAfterAction = false);
	//缩放到动作。fTime：时间。szScaleTo：目标体积。bRemoveSelfAfterAction：动作结束后是否删除该单位
	void ScaleTo3D(float fTime,tagSize3D szScaleTo,bool bRemoveSelfAfterAction = false);
	//缩放动作。fTime：时间。vec3D：体积的变化量。bRemoveSelfAfterAction：动作结束后是否删除该单位
	void ScaleBy3D(float fTime,tagVector3D vec3D,bool bRemoveSelfAfterAction = false);
	//停止所有移动动作
	void StopMoveAction();
	//停止所有缩放动作
	void StopScaleAction();
protected:
	//init
	virtual bool init(tagUnit3DBasicInfo unitInfo);
	//onExit
	virtual void onExit();
	//draw,同时做一些更新操作
	virtual void draw();
protected:
	enum
	{
		enTagLTBody=20000,
		enTagLTShadow,
	};
};

#endif 
