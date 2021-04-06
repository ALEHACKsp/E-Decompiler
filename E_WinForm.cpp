#include "E_WinForm.h"
#include "public.h"

void Unserialize_WinFormData(unsigned char* lpControlData, WinFrom_UnitData* out_Data)
{
	out_Data->version = ReadUInt(lpControlData);
	lpControlData += 4;

	if (out_Data->version <= 6) {
		out_Data->�߿� = CDR_ReadUInt(lpControlData);
		out_Data->��ͼ��ʽ = CDR_ReadUInt(lpControlData);
		out_Data->��ɫ = CDR_ReadUInt(lpControlData);

		out_Data->��󻯰�ť = CDR_ReadUInt(lpControlData);
		out_Data->��С����ť = CDR_ReadUInt(lpControlData);
		out_Data->���ư�ť = CDR_ReadUInt(lpControlData);
		out_Data->λ�� = CDR_ReadUInt(lpControlData);
		out_Data->�ɷ��ƶ� = CDR_ReadUInt(lpControlData);
		out_Data->���Ŵ��� = CDR_ReadUInt(lpControlData);
		out_Data->�س����ƽ��� = CDR_ReadUInt(lpControlData);
		out_Data->Esc���ر� = CDR_ReadUInt(lpControlData);
		out_Data->F1���򿪰��� = CDR_ReadUInt(lpControlData);
		out_Data->field_34 = CDR_ReadUInt(lpControlData);

		if (out_Data->version >= 2) {
			out_Data->field_38 = CDR_ReadUInt(lpControlData);
		}
		if (out_Data->version >= 3) {
			out_Data->field_3C = CDR_ReadUInt(lpControlData);
			out_Data->field_40 = CDR_ReadUInt(lpControlData);
		}
		if (out_Data->version >= 4) {
			out_Data->������ǰ = CDR_ReadUInt(lpControlData);
		}
		if (out_Data->version >= 5) {
			out_Data->���ֱ��������� = CDR_ReadUInt(lpControlData);
		}
		if (out_Data->version == 6) {
			out_Data->�������� = CDR_ReadCString(lpControlData);
		}

		out_Data->��ͼ = CDR_ReadCFreqMem(lpControlData);
		out_Data->ͼ�� = CDR_ReadCFreqMem(lpControlData);
		out_Data->�������� = CDR_ReadCFreqMem(lpControlData);

		out_Data->���� = CDR_ReadCString(lpControlData);
		out_Data->�����ļ��� = CDR_ReadCString(lpControlData);
	}
	return;
}