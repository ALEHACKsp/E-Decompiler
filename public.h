#pragma once
#include <pro.h>

//ʮ����ʮ
void HexToBin(qstring& HexCode, uchar* BinCode);

//����һΪԭ�ı�,������Ϊ����ı�,������Ϊ�ұ��ı�,������Ϊ��ʼƫ��
qstring GetMidString(qstring& src, const char* left, const char* right, int offset);

//��������������,��0x90������ģ������,����ƫ�ƴ�С
uint32 Search_Bin(unsigned char* pSrc, unsigned char* pTrait, int nSrcLen, int nTraitLen);

qstring �ֽڼ�_�ֽڼ���ʮ������(qvector<unsigned char>& ԭʼ�ֽڼ�);

//��ȡ���е����ݽ�������
qvector<ea_t> GetAllDataRef(ea_t addr);

qstring get_shortstring(int32 addr);

qstring getUTF8String(const char*);

uint32 ReadUInt(unsigned char* pBuf);

int32 ReadInt(unsigned char* pBuf);

qstring ReadStr(unsigned char* pBuf);

