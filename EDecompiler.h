#pragma once
#include <pro.h>
#include "SectionManager.h"


enum EProgramsType_t
{
	E_UNKNOWN = 0,  //δ֪����
	E_DYNAMIC,      //��̬�������
	E_STATIC,       //��̬�������
};

#pragma pack(push,1)

struct EHead
{
	uint32 dwMagic;  //δ֪,ֵ�̶�Ϊ3
	uint32 szNone2;  //δ֪,ֵ�̶�Ϊ0
	uint32 szNone3;  //δ֪,�����Ǹ������,�޸Ĳ�Ӱ�����
	uint32 lpStartCode;   //��ʼ�����ַ,�����޸�
	uint32 lpEString;     //�ַ�����Դ,���û���ַ�����Դ,��Ϊ0
	uint32 dwEStringSize; //�ַ�����Դ��С,���û���ַ�����Դ,��Ϊ0
	uint32 lpEWindow;     //���������Ϣ
	uint32 dwEWindowSize; //���������Ϣ��С
	uint32 dwLibNum;      //֧�ֿ�����
	uint32 lpLibEntry;    //֧�ֿ���Ϣ���
	uint32 dwApiCount;    //Api����
	uint32 lpModuleName;  //ָ��ģ������
	uint32 lpApiName;     //ָ��Api����
};

#pragma pack(pop)

struct BinSource
{
	//-1��ʾ������ͷ
	//0��ʾ���ַ���
    //1��ʾ���ֽڼ�
    //2��ʾ�Ǹ�����
	int itype;

	ea_t binAddr;
	qvector<unsigned char> binData;
	double floatData;
};

struct mid_EAppInfo
{
	ea_t m_UserCodeStartAddr;                          //�û���ʼ��ַ
	ea_t m_UserCodeEndAddr;                            //�û�������ַ,Ŀǰ��ʱ��û��ʲô�ð취��ȡ�����ַ,����кõ��뷨��ӭ��issue
	qvector<BinSource> m_VecUserResource;              //�û���Դ
};

class IDAMenu;
class EDecompilerEngine:public SectionManager
{
public:
	EDecompilerEngine();
	~EDecompilerEngine();

public:
	bool InitDecompilerEngine();
	bool DoDecompile();
private:
	bool DoDecompiler_EStatic();

	//�ж�ָ����ַ�������Ƿ�Ϊ�����Ը��㳣��
	bool IsFloatConstValue(ea_t);
	//�ж�ָ����ַ�������Ƿ�Ϊ����������ͷ
	bool IsArrayHeadValue(ea_t);
	//��������ؼ���Ϣ
	bool ParseGUIResource(ea_t, uint32);
	//����������Դ��Ϣ
	bool ParseStringResource(ea_t, uint32);
	//����֧�ֿ���Ϣ
	bool ParseLibInfomation(ea_t, uint32);
public:
	EProgramsType_t m_ProgramType;
	mid_EAppInfo m_eAppInfo;
private:
	ea_t m_EHeadAddr;
	IDAMenu* gMenu_ShowResource = nullptr;
};

extern EDecompilerEngine g_MyDecompiler;