
//  http://www.9miao.com

#ifndef _NF_DATA_MANAGER_HEAD_
#define _NF_DATA_MANAGER_HEAD_

#include "../publicdef/PublicDef.h"

/************************************************************************/
/*				���ݿ�У��                                                                     */
/************************************************************************/
//У��int��
int GetIntFromTable(char * szItemName, int n_column, char ** column_value, char ** column_name);
//У��float��
float GetFloatFromTable(char * szItemName, int n_column, char ** column_value, char ** column_name);
//У��string��
string GetStringFromTable(char * szItemName, int n_column, char ** column_value, char ** column_name);

/************************************************************************/
/*					��̬���ݽṹ��                                                                     */
/************************************************************************/
//�ؿ���̬��Ϣ�ṹ��
struct tagStageStaticInfo 
{
	float 					fRate;					//������б�Ƕȣ�y�ᣩ
	float					fGravity;				//������z����ٶȣ�
	float					fLength;				//����3D���磩
	float					fWidth;					//��3D���磩
	float					fHeight;				//�ߣ�3D���磩
	float					fProtagonistX;			//����x��ʼ������
	float					fProtagonistY;			//����y��ʼ������
	float					fProtagonistZ;			//����z��ʼ������
	string					strTMXWorldName;		//�����ͼ���������
	CNFTmxStaticInfoMap		TmxStaticInfoMap;		//�����ͼmap
	CNFNPCJsonInfoVec		NPCJsonInfoVec;			//NPC JSON ��Ϣ
	CNFDoorJsonInfoVec		DoorJsonInfoVec;		//Door Json ��Ϣ
};
typedef map<int,tagStageStaticInfo> CNFStageStaticInfoMap;		//�ؿ���̬��Ϣmap��ͨ���ؿ�ID����ùؿ���Ϣ


//���Ǿ�̬��Ϣ�ṹ��
struct tagProtagonistStaticInfo
{
	float			fHP;						//����ֵ
	float			fHp_g;						//����ֵ�ɳ�
	float			fMP;						//����ֵ
	float			fMp_g;						//����ֵ�ɳ�
	float			fDefense;					//������
	float			fDefense_g;					//�������ɳ�
	float			fAttack;					//������
	float			fAttack_g;					//�������ɳ�
	float			fDomination;				//�ƿ���
	float			fMoveSpeed;					//�ƶ��ٶ�
	float			fAttackSpeed;				//�����ٶ�
	int				nCrit;						//������
	float			fCritDamage;				//�����˺�
	int				nCrickTime;					//Ӳֱʱ��
	int				nConverselyTime;			//����ʱ��
	int				nHitTarget;					//����
	int				nAvoid;						//�ر�
	float			fTreatCoefficient;			//����ϵ��

	//hold״̬��3D������������ߣ�
	float			fLength_Hold;				//��
	float			fWidth_Hold;				//��
	float			fHeight_Hold;				//��

	//move״̬��3D���
	float			fLength_Move;				//��
	float			fWidth_Move;				//��
	float			fHeight_Move;				//��

	//conversely״̬��3D���
	float			fLength_Conversely;			//��
	float			fWidth_Conversely;			//��
	float			fHeight_Conversely;			//��

	tagProtagonistAIInfo	tProtagonistAIInfo;	//����ai
	
	string			strBeHitSoundName;			//������Ч����
	string			strDeadSoundName;			//������Ч����

	string			strArmatureName;			//���ǹ�������������
	int				nCommonAttackNum;			//��ͨ��������
};
typedef map<int,tagProtagonistStaticInfo> CNFProtagonistStaticInfoMap;		//���Ǿ�̬��Ϣmap��ͨ������ID�����������Ϣ


//���˾�̬��Ϣ�ṹ��
struct tagEnemyStaticInfo 
{
	float			fHP;						//����ֵ
	float			fHp_g;						//����ֵ�ɳ�
	float			fMP;						//����ֵ
	float			fMp_g;						//����ֵ�ɳ�
	float			fDefense;					//������
	float			fDefense_g;					//�������ɳ�
	float			fAttack;					//������
	float			fAttack_g;					//�������ɳ�
	float			fDomination;				//�ƿ���
	float			fMoveSpeed;					//�ƶ��ٶ�
	float			fAttackSpeed;				//�����ٶ�
	int				nCrit;						//������
	float			fCritDamage;				//�����˺�
	int				nCrickTime;					//Ӳֱʱ��
	int				nConverselyTime;			//����ʱ��
	int				nTurnRoundTime;				//ת��CDʱ�䣨֡��
	int				nHitTarget;					//����
	int				nAvoid;						//�ر�
	float			fTreatCoefficient;			//����ϵ��

	//hold״̬
	float			fLength_Hold;				//��
	float			fWidth_Hold;				//��
	float			fHeight_Hold;				//��

	//move״̬
	float			fLength_Move;				//��
	float			fWidth_Move;				//��
	float			fHeight_Move;				//��

	//conversely״̬
	float			fLength_Conversely;			//��
	float			fWidth_Conversely;			//��
	float			fHeight_Conversely;			//��

	float			fNearDis;					//AI������
	float			fFarDis;					//AIԶ����

	string			strArmatureName;			//���˹�����������
};
typedef map<int,tagEnemyStaticInfo> CNFEnemyStaticInfoMap;		//���˾�̬��Ϣmap��ͨ������ID����õ�����Ϣ


//��Ʒ��̬��Ϣ�ṹ��
struct tagArticleStaticInfo
{
	float			fHp;					//��Ʒ����ֵ
	int				nIsInvincible;			//��Ʒ�Ƿ��޵У�0.���޵�	1.�޵�

	float			fLength_Hold;			//��
	float			fWidth_Hold;			//��
	float			fHeight_Hold;			//��
};
typedef map<int,tagArticleStaticInfo> CNFArticleStaticInfoMap;		//��Ʒ��̬��Ϣmap��ͨ����ƷID�������Ʒ��Ϣ


//�������龲̬��Ϣ�ṹ��
struct tagLevelUpExpStaticInfo
{
	float		fLevelUpExp;		//�������辭��
};
typedef map<int,tagLevelUpExpStaticInfo> CNFLevelUpExpStaticInfoMap;	//�������龲̬��Ϣmap��ͨ���ȼ�ID�������������


//������ϢItem�ṹ�壬һ������
struct tagWaveItemInfo
{
	int				nEnemyID;				//����ID
	int				nEnemyLevel;			//����ȼ�
	int				nStartTime;				//���ﴴ��ʱ��
	float			fCreatePosX;			//����x����ʼλ�ã������3D�����λ�ã�			
	float			fCreatePosY;			//����y����ʼλ�ã������3D�����λ�ã�
};
typedef vector<tagWaveItemInfo>					CNFWaveItemInfoVec;				//����Item��Ϣvec��һ������
typedef map<int,CNFWaveItemInfoVec>				CNFWaveInfoMap;					//������Ϣmap��һ�ص��ˡ�ͨ������ID�����һ��������Ϣ
typedef map<int,CNFWaveInfoMap>					CNFStageWaveInfoMap;			//�ؿ�������Ϣmap��ȫ���ؿ����ˡ�ͨ���ؿ�ID�����һ�ص�����Ϣ


//��ʱ�����ܹ���������Ϣ�ṹ�壬�����õ���ǰ�ؿ��У��õ������м��ܹ�����������Ϣ
struct	tagSkillArmatureInfo
{
	string		strArmatureName;		//������������
};
typedef vector<tagSkillArmatureInfo>		CNFSkillArmatureInfoVec;			//���ܹ���������Ϣ����
typedef	map<int , CNFSkillArmatureInfoVec>	CNFSkillArmatureInfoByStageID;		//ͨ���ؿ�����ñ��ؿ��ã��������м��ܹ�����������Ϣ


//װ����̬��Ϣ�ṹ��
struct tagEquipStaticInfo
{
	string		strEquipName;				//װ������				������ ���� �ؼס�
	int			nEquipPositionID;			//װ����λID			1.ͷ  2.����  3.�ز�  4.�ֲ�  5.�Ȳ�  6.�Ų�  7.����  8.����  9.��ָ  10.��Ʒ
	int			nEquipQuality;				//װ��Ʒ��				1.��  2.��    3.��    4.��    5.��
	int			nEquipMustAID;				//װ����������ID		1.�����ظ����ٷֱȣ�  2.���������  3.������  4.����  5.����  6.�ƶ��ٶ�  7.��������  8.������  9.��������ޣ��ٷֱȣ�  10.�����ٶ�  11.����  12.��ͨ  13.Ĭ��  14.����  15.�����˺�
	float		fEquipMustABasicPoint;		//װ���������Ի���ֵ
	float		fEquipMustAGroupPoint;		//װ���������Գɳ�ֵ
};
typedef	map<int , tagEquipStaticInfo>	CNFEquipStaticInfoMap;				//װ����̬��Ϣmap��ͨ��װ��ID���õ�װ����Ϣ��



/************************************************************************/
/*						��̬���ݿ���                                                                     */
/************************************************************************/
class CNFStaticDataManager
{
	CNFStaticDataManager();

protected:
	CNFStageStaticInfoMap					m_StageStaticInfoMap;			//�ؿ���̬��Ϣ
	CNFProtagonistStaticInfoMap				m_ProtagonistStaticInfoMap;		//���Ǿ�̬��Ϣ
	CNFEnemyStaticInfoMap					m_EnemyStaticInfoMap;			//���˾�̬��Ϣ
	CNFArticleStaticInfoMap					m_ArticleStaticInfoMap;			//��Ʒ��̬��Ϣ
	CEnemyAIStaticInfoMap					m_EnemyAIStaticInfoMap;			//����AI��̬��Ϣ
	CNFLevelUpExpStaticInfoMap				m_LevelUpexpStaticInfoMap;		//�������辭�龲̬��Ϣ
	CNFSkillStaticInfoMapByProtagonistID	m_SkillStaticInfoMap;			//���ܾ�̬��Ϣ��
	CNFStageWaveInfoMap						m_StageWaveStaticInfoMap;		//���ξ�̬��Ϣ
	CNFSkillArmatureInfoVec					m_SkillArmatureInfoVec;			//��ʱ�����ܹ���������Ϣ
	CNFEquipStaticInfoMap					m_EquipStaticInfoMap;			//װ����̬��Ϣ
	CNFNPCStaticInfoMap						m_NPCStaticInfoMap;				//NPC��̬��Ϣ
	CNFDoorStaticInfoMap					m_DoorStaticInfoMap;			//Door��̬��Ϣ

public:
	static CNFStaticDataManager *SharedData();
	void initBasicData();			//��ʼ������
	static void ReleaseData();		//�������

public:
	//��ùؿ���̬��Ϣ�����ݹؿ�ID��
	bool GetStageStaticInfo(const int nStageID,tagStageStaticInfo &Info);
	//��ȡ�ؿ���̬��Ϣ
	bool ReadStageStaticFromDB();
	static int LoadStageStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//������Ǿ�̬��Ϣ����������ID��
	bool GetProtagonistStaticInfo(const int nProtagonistID,tagProtagonistStaticInfo &Info);
	//��ȡ���Ǿ�̬��Ϣ
	bool ReadProtagonistStaticFromDB();
	static int LoadProtagonistStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//��õ��˾�̬��Ϣ�����ݽ�ɫID��
	bool GetEnemyStaticInfo(const int nRoleID,tagEnemyStaticInfo &Info);
	//��ȡ���˾�̬��Ϣ
	bool ReadEnemyStaticFromDB();
	static int LoadEnemyStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//�����Ʒ��Ϣ��������ƷID��
	bool GetArticleStaticInfo(const int nArticleID,tagArticleStaticInfo &Info);
	//��ȡ��Ʒ��̬��Ϣ
	bool ReadArticleStaticFromDB();
	static int LoadArticleStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//��õ���AI��Ϣ�����ݽ�ɫID��
	bool GetEnemyAIStaticInfo(const int nRoleID,CMapAI &Info);
	//��õ���AI��̬��Ϣ
	bool ReadEnemyAIStaticFromDB();
	static int LoadEnemyAIStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//����������辭����Ϣ�����ݵȼ�ID��
	bool GetLevelUpExpStaticInfo(const int nLevelID,tagLevelUpExpStaticInfo &Info);
	//��ȡ�������辭�龲̬��Ϣ
	bool ReadLevelUpExpStaticFromDB();
	static int LoadLevelUpExpStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//��ü��ܾ�̬��Ϣ�����ݽ�ɫID������ID��
	bool GetSkillStaticInfo(const int nRoleID,const int nSkillID,tagSkillStaticInfo &Info);
	//��ȡ���ܾ�̬��Ϣ
	bool ReadSkillStaticFromDB();
	static int LoadSkillStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);
	
	//���ָ���ļ��ܾ�̬��Ϣ������nRoleIDΪ��ɫID��nIsCommonSkillΪ0ʱ���������м��ܣ�nIsCommonSkillΪ1ʱ������������ͨ������
	bool GetDesignSkillStaticInfoVec(const int nRoleID,const int nIsCommonSkill,CNFSkillStaticInfoVec &Info);

	//���ݹؿ�ID������ID��ò�����Ϣ��һ�����ˣ�
	bool GetWaveStaticInfo(const int nStageID,const int nWaveID,CNFWaveItemInfoVec &Info);
	//��ȡ���˲��ξ�̬��Ϣ
	bool ReadWaveStaticFromDB();
	static int LoadWaveStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);

	//���װ����̬��Ϣ������װ��ID��
	bool GetEquipStaticInfo(const int nEquip,tagEquipStaticInfo &Info);
	//��ȡװ����̬��Ϣ
	bool ReadEquipStaticFromDB();
	static int LoadEquipStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);

	//���npc��̬��Ϣ������npcID��
	bool GetNPCStaticInfo(const int nNPCID,tagNPCStaticInfo &Info);
	//��ȡncp��̬��Ϣ
	bool ReadNPCStaticFromDB();
	static int LoadNPCStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);

	//���door��̬��Ϣ������Door ID��
	bool GetDoorStaticInfo(const int nDoorID,tagDoorStaticInfo &Info);
	//��ȡdoor��̬��Ϣ
	bool ReadDoorStaticFromDB();
	static int LoadDoorStaticCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//��ʱ���õ����أ����õ��ģ����м��ܵģ�������������
	bool GetSkillArmatureInfo(const int nStageID,const int nWaveID,CNFWaveItemInfoVec &Info);
	//��ʱ����ȡ���˼��ܹ���������Ϣ
	bool ReadSkillArmatureInfo();
};

















/************************************************************************/
/*					�洢���ݽṹ��                                                       */
/************************************************************************/
//�û�����������
struct tagSaveUserInfo 
{
	string		strOpenID;					//�û�ID��Ψһ��ʶ
	string		strUserName;				//�û�����
	string		strPassWord;				//�û�����
};

//��������Ϣ
struct tagSaveServerInfo
{
	int			nServerID;					//������ID
	string		strServerName;				//����������
};

/************************************************************************/
/*					�洢���ݹ�����                                                                   */
/************************************************************************/
class CNFSaveDataManager
{
	CNFSaveDataManager();

protected:
	tagSaveUserInfo				m_SaveUserInfo;				//�û���Ϣ
	tagSaveServerInfo			m_SaveServerInfo;			//��������Ϣ

public:
	static CNFSaveDataManager *SharedData();
	void initBasicData();			
	static void ReleaseData();		

public:
	//���ñ����û���Ϣ
	bool SetSaveUserInfo(tagSaveUserInfo UserInfo);
	//��ñ����û���Ϣ
	bool GetSaveUserInfo(tagSaveUserInfo &UserInfo);
	//��ȡ�����û���Ϣ
	bool ReadSaveUserFromDB();
	static int LoadSaveUserCallBack(void * para, int n_column, char ** column_value, char ** column_name);


	//���÷�������Ϣ
	bool SetSaveServerInfo(tagSaveServerInfo ServerInfo);
	//��÷�������Ϣ
	bool GetSaveServerInfo(tagSaveServerInfo &ServerInfo);
	//��ȡ��������Ϣ
	bool ReadSaveServerFromDB();
	static int LoadSaveServerCallBack(void * para, int n_column, char ** column_value, char ** column_name);


};











#endif