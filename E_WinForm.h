#pragma once
#include <pro.h>

struct WinFrom_UnitData
{
	unsigned int version;
	unsigned int �߿�;
	unsigned int ��ͼ��ʽ;
	unsigned int ��ɫ;
	unsigned int ��󻯰�ť;
	unsigned int ��С����ť;
	unsigned int ���ư�ť;
	unsigned int λ��;
	unsigned int �ɷ��ƶ�;
	unsigned int ���Ŵ���;
	unsigned int �س����ƽ���;
	unsigned int Esc���ر�;
	unsigned int F1���򿪰���;
	unsigned int field_34;
	unsigned int field_38;
	unsigned int field_3C;
	unsigned int field_40;
	unsigned int ������ǰ;
	unsigned int ���ֱ���������;
	qstring ��������;
	qvector<unsigned char> ��ͼ;
	qvector<unsigned char> ͼ��;
	qvector<unsigned char> ��������;
	qstring ����;
	qstring �����ļ���;
};

void Unserialize_WinFormData(unsigned char* lpControlData, WinFrom_UnitData* out_Data);