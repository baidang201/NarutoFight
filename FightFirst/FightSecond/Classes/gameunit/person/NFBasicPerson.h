#ifndef _NF_BASICPERSON_HEAD_
#define _NF_BASICPERSON_HEAD_
#include "../NFBasicUnit.h"


////攻击类型
//enum
//{
//	enAttackBlunt=1,			//物理顿
//	enAttackSharp=2,			//物理锐
//	enAttackThunder=3,			//雷电
//	enAttackFlame=4,			//火焰
//	enAttackCryo=5,				//冰霜
//};

//方向
enum
{
	enTagDirectionR = 1,		//右
	enTagDirectionL = -1,		//左
};

//人物基类:不可以用来创建对象
class CNFBasicPerson :public CNFBasicUnit
{
private:
	int m_nIRGB1;		//临时
	int m_nSpeedRGB1;	//临时
protected:

	unsigned int m_nExistTime;					//存在时间（帧数）
	bool		m_bIsDeath;						//是否死亡
	bool		m_bIsInvincible;				//是否无敌

	int			m_nLV;							//等级
	//float		m_fPower;						//力量
	//float		m_fWit;							//智力
	//float		m_fAgility;						//敏捷
	float		m_fHP;							//生命值
	float		m_fMP;							//能量值
	float		m_fDefense;						//防御力
	float		m_fRefillHP;					//生命恢复
	float		m_fRefillMP;					//能量恢复
	float		m_fAttack;						//攻击力
	float		m_fDomination;					//掌控力
	float		m_fMoveSpeed;					//移动速度
	float		m_fMoveRunSpeed;				//奔跑速度
	float		m_fAttackSpeed;					//攻击速度	100是基础值（1倍速）
	int			m_nCrit;						//暴击率
	float		m_fCritDamage;					//暴击伤害
	int			m_nCrickTime;					//硬直时间
	int			m_nConverselyTime;				//起身时间
	int			m_nHitTarget;					//命中
	int			m_nAvoid;						//回避

	float		m_fMadter;						//精通
	float		m_fTacitUnderstanding;			//默契
	float		m_fPerseverance;				//毅力


	float		m_fTreatCoefficient;			//治疗系数
	float		m_fAttackUpCoefficient;			//攻击力增强系数

	int			m_nSuperArmorTime;				//霸体时间
	int			m_nAstheniaTime;				//衰弱时间
	int			m_nAttackSpeedCutTime;			//缓慢时间
	float		m_fAttackSpeedCut_Quantity;		//缓慢量
	int			m_nMoveSpeedCutTime;			//减速时间
	float		m_fMoveSpeedCut_Quantity;		//减速量
	int			m_nPoisoningTime;				//中毒时间
	float		m_fPoisoning_Quantity;			//中毒量
	int			m_nAttackUpTime;				//攻击力增强时间
	float		m_fAttackUp_Quantity;			//攻击力增强量
	int			m_nAttackUpSTime;				//攻击力增强2时间
	float		m_fAttackUpS_Quantity;			//攻击力增强2量



	float		m_fAttackSpeedTemp;				//当前攻速
	int			m_nConverselyTimeTemp;			//当前起身时间
	int			m_nCrickTimeTemp;				//当前硬直时间
	float		m_fHPTemp;						//当前HP
	float		m_fHP2Temp;						//当前二层血条HP
	float		m_fMPTemp;						//当前MP
	float		m_fMoveSpeedTemp;				//当前移动速度
	float		m_fMoveRate;					//移动角度
	int			m_nDirection;					//方向 
	CCArray		*m_pArticleArray;				//物体数组
	bool		m_bIsFixationMoveRateStatus;	//定方向移动状态


	tagSize3D	m_HoldImpactSize;				//待机时的体积
	tagSize3D	m_MoveImpactSize;				//移动时的体积
	tagSize3D	m_RunImpactSize;				//奔跑时的体积
	tagSize3D	m_ConverselyImpactSize;			//倒地时的体积
public:
	//是否死亡
	bool IsDeath();
	//更新函数
	virtual void CallPerFrame(float fT);
	//伤害
	virtual float HarmFormula(tagTakeABeating tTakeABeating);
	//治疗
	virtual float TreatFormula(float fTreatPoint);
	//百分比治疗
	virtual float BFBTreatFormula(float fBFBTreatPoint);
	//百分比恢复MP
	virtual float BFBRecoverMP(float fBFBMPPoint);
	//获取当前hp百分比
	float GetHPPercent();
	//获得人物当前方向
	int GetDirection();
	//设置人物当前方向
	void SetDirection(int nDirection);
	void SetDirectionEnemy(int nDirection);
	//获得人物的CCArmature
	CCArmature* GetCCArmature();
	//添加可碰撞物体数组
	void SetArticleArray(CCArray *pArticleArray);
	//获得人物的当前移动方向
	float	GetMoveRate();
	//获得人物当前是否处于定方向移动状态
	bool	GetIsFixationMoveRateStatus();
	//获得人物当前是否在地面上
	bool	GetIsOnGroundStatus();
	//获得当前攻击力
	float GetAttack(){return m_fAttack*m_fAttackUpCoefficient;}
	//获得当前暴击
	int GetCrit(){return m_nCrit;}
	//获得当前暴击伤害
	float GetCritDamage(){return m_fCritDamage;}
	//获得当前命中
	int	GetHitTarget(){return m_nHitTarget;}
	//获得当前掌控力
	float	GetDomination(){return m_fDomination;}
protected:
	//人物基类的构造方法
	virtual bool init(const char* pArmatureName,float fRate,tagSize3D szSize);
	//onExit
	virtual void onExit();
public:
	//添加霸体buff
	void AddSuperArmorBuff(int nTime);
	//添加衰弱buff  处于衰弱状态，治疗量减半
	void AddAstheniaBuff(int nTime);
	//添加缓慢buff
	void AddAttackSpeedCutBuff(int nTime,float fQuantity);
	//添加减速buff
	void AddMoveSpeedCutBuff(int nTime,float fQuantityBFB);
	//添加中毒buff
	void AddPoisoningBuff(int nTime,float fQuantity);
	//添加攻击力增强buff.时间重置伤害叠加
	void AddAttackUpBuff(int nTime,float fQuantity);
	//添加攻击力增强2buff.时间重置伤害重置
	void AddAttackUpSBuff(int nTime,float fQuantity);
protected:
	enum
	{
		enTagBody=20000,
	};
};
#endif