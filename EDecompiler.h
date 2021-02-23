#pragma once
#include <pro.h>

enum EProgramsType_t
{
	E_UNKNOWN = 0,  //δ֪����
	E_DYNAMIC,      //��̬�������
	E_STATIC,       //��̬�������
};

#pragma pack(push,1)

struct EHead
{
	uint32 dwMagic;  //δ֪
	uint32 szNone2;  //δ֪
	uint32 szNone3;  //δ֪
	uint32 lpStartCode;   //��ʼ�����ַ
	uint32 lpEString;     //�ַ�����Դ,���û���ַ�����Դ,��Ϊ0
	uint32 dwEStringSize; //�ַ�����Դ��С,���û���ַ�����Դ,��Ϊ0
	uint32 lpEWindow;     //���������Ϣ
	uint32 dwEWindowSize; //���������Ϣ��С
	uint32 dwLibNum;      //֧�ֿ�����
	uint32 lpLibEntry;    //֧�ֿ���Ϣ���
	uint32 dwApiCount;    //Api����
	uint32 lpLibName;     //ָ��֧�ֿ�����
	uint32 lpApiName;     //ָ��Api����
};

#pragma pack(pop)


struct mid_EHeadInfo
{
	ea_t m_UserCodeStartAddr;                          //�û���ʼ��ַ
	qvector<qstring> m_VecStringResource;              //�ַ�����Դ
	qvector<qvector<unsigned char>> m_VecBinResource;  //�ֽڼ���Դ
};

class EDecompilerEngine
{
public:
	EDecompilerEngine();
	~EDecompilerEngine();
public:
	bool DoDecompiler();
private:
	bool DoDecompiler_EStatic();
	bool ParseStringResource(ea_t, uint32);
public:
	EProgramsType_t m_ProgramType;
private:
	ea_t m_EHeadAddr;
	mid_EHeadInfo m_eHeadInfo;
};