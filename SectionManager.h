#pragma once
#include <pro.h>

struct SegmentInfomation
{
	ea_t m_segStart;                   //������ʼ��ַ
	uint32 m_segSize;                  //���δ�С
	qstring m_segName;                 //��������
	qvector<unsigned char> m_segData;  //��������
};

//��ʼ����������
bool InitSectionManager();

//���Ե�ַת��Ϊ�����ַ
uint8* LinearAddrToVirtualAddr(ea_t LinerAddr);

//�����ַת��Ϊ���Ե�ַ
uint32 VirtualAddrToLinearAddr(ea_t dwOaddr);
