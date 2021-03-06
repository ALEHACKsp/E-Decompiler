#include "EDecompiler.h"
#include <bytes.hpp>
#include <segment.hpp>
#include <kernwin.hpp>
#include <afx.h>
#include "EGuiInfoObject.h"
#include "IDAMenu.h"
#include "public.h"
#include <ua.hpp>
#include <allins.hpp>

EDecompilerEngine g_MyDecompiler;

bool IsFloatInstruction(ea_t addr)
{
	insn_t ins;
	decode_insn(&ins, addr);
	int a = 0;
	return true;
}

struct chooser_UserResource :public chooser_t 
{
protected:
	const int widths_[3] = { 10,10,50 };
	const char* header_[3] = { "Address","Type","Data" };
public:
	chooser_UserResource(const char* title) :chooser_t(0, qnumber(widths_), widths_, header_, title)
	{

	}
private:
	~chooser_UserResource()
	{

	}
	const void* get_obj_id(size_t* len) const
	{
		*len = strlen(title);
		return title;
	}

	void idaapi get_row(qstrvec_t* cols_, int* icon_, chooser_item_attrs_t* attrs, size_t n) const
	{
		qstrvec_t& cols = *cols_;
		qvector<BinSource>& vec_Bin = g_MyDecompiler.m_eAppInfo.m_VecUserResource;

		cols[0].sprnt("%08a", vec_Bin[n].binAddr);

		if (vec_Bin[n].binData.size())
		{
			if (vec_Bin[n].itype == 0) {
				acp_utf8(&cols[1], "�ı���");
				acp_utf8(&cols[2], (char*)&vec_Bin[n].binData[0]);
			}
			else {
				acp_utf8(&cols[1], "�ֽڼ�");
				cols[2] = �ֽڼ�_�ֽڼ���ʮ������(vec_Bin[n].binData);
			}
		}
		else if (vec_Bin[n].itype == 0) {
			acp_utf8(&cols[1], "�հ��ı�");
		}
		else if (vec_Bin[n].itype == 1) {
			acp_utf8(&cols[1], "�հ��ֽڼ�");
		}
		else if (vec_Bin[n].itype == 2) {
			acp_utf8(&cols[1], "������");
			cols[2].sprnt("%lf", vec_Bin[n].floatData);
		}
	}

	cbret_t idaapi enter(size_t n)
	{
		jumpto(g_MyDecompiler.m_eAppInfo.m_VecUserResource[n].binAddr);
		return cbret_t();
	}
	size_t idaapi get_count(void) const
	{
		return g_MyDecompiler.m_eAppInfo.m_VecUserResource.size();
	}
};

int MenuHandle_ShowUserResource()
{
	qstring ����;
	acp_utf8(&����, "�û�������Դ");
	chooser_UserResource* pUserChoose = new chooser_UserResource(����.c_str());
	pUserChoose->choose();
	return 0;
}

bool EDecompilerEngine::ParseGUIResource(ea_t lpGUIStart,uint32 infoSize)
{
	qvector<unsigned char> tmpGuiBuf;
	tmpGuiBuf.resize(infoSize);
	get_bytes(&tmpGuiBuf[0], infoSize, lpGUIStart);

	CMemFile memGuiBuf;
	memGuiBuf.Attach(&tmpGuiBuf[0],infoSize);
	
	CArchive tmpArchive(&memGuiBuf, CArchive::load);
	int windowCount;
	
	EGuiInfoObject GuiObject;
	GuiObject.Serialize(tmpArchive);
	tmpArchive.Close();
	

	int a = 0;
	
	return true;
}

bool EDecompilerEngine::IsArrayHeadValue(ea_t addr)
{
	qvector<ea_t> DataRef = GetAllDataRef(addr);
	if (!DataRef.size()) {
		return false;
	}

	if (DataRef[0] <= m_eAppInfo.m_UserCodeStartAddr || DataRef[0] >= m_eAppInfo.m_UserCodeEndAddr) {
		return false;
	}
	insn_t ins;
	decode_insn(&ins, DataRef[0]);
	if (ins.itype == NN_mov && get_dword(DataRef[0] + 5) == 0xABADABAD) {
		return true;
	}
	return false;
}

bool EDecompilerEngine::IsFloatConstValue(ea_t addr)
{
	qvector<ea_t> DataRef = GetAllDataRef(addr);
	if (!DataRef.size()) {
		return false;
	}

	if (DataRef[0] <= m_eAppInfo.m_UserCodeStartAddr || DataRef[0] >= m_eAppInfo.m_UserCodeEndAddr) {
		return false;
	}
	insn_t ins;
	decode_insn(&ins, DataRef[0]);
	if (ins.itype == NN_fadd || ins.itype == NN_fsub || ins.itype == NN_fmul || ins.itype == NN_fdiv || ins.itype == NN_fcomp || ins.itype == NN_fld)
	{
		return true;
	}
	return false;
}

bool EDecompilerEngine::ParseStringResource(ea_t lpStringStart,uint32 StringSize)
{
	m_eAppInfo.m_VecUserResource.clear();

	qvector<unsigned char> tmpResouceBuf;
	tmpResouceBuf.resize(StringSize);
	get_bytes(&tmpResouceBuf[0], StringSize, lpStringStart);

	for (unsigned int index = 0; index < tmpResouceBuf.size(); ++index) {
		BinSource tmpSource = {};

		if (IsArrayHeadValue(lpStringStart + index)) {         //����ͷ
			tmpSource.itype = -1;
			index = index + 8 - 1;
		}
		else if (IsFloatConstValue(lpStringStart + index)) {   //��鸡��������С��
			tmpSource.itype = 2;
			tmpSource.binAddr = lpStringStart + index;
			get_bytes(&tmpSource.floatData, sizeof(double), tmpSource.binAddr);
			m_eAppInfo.m_VecUserResource.push_back(tmpSource);
			index = index + 8 - 1;
		}
		else if (tmpResouceBuf[index] == 0x0) {
			tmpSource.itype = 0;
			tmpSource.binAddr = lpStringStart + index;
			m_eAppInfo.m_VecUserResource.push_back(tmpSource);
		}
		else if (tmpResouceBuf[index] == 0x1) {
			if ((index + 8) > tmpResouceBuf.size()) {
				return false;
			}
			uint32 binHead = *(uint32*)&tmpResouceBuf[index];
			if (binHead != 0x1) {
				return false;
			}
			uint32 binSize = *(uint32*)&tmpResouceBuf[index + 4];
			if (index + 8 + binSize > tmpResouceBuf.size()) {
				return false;
			}
			tmpSource.itype = 1;
			tmpSource.binAddr = lpStringStart + index;
			if (binSize) {
				tmpSource.binData.resize(binSize);
				memcpy(&tmpSource.binData[0], &tmpResouceBuf[index + 8], binSize);
			}
			m_eAppInfo.m_VecUserResource.push_back(tmpSource);
			index = index + 8 + binSize - 1;
		}
		else {
			tmpSource.itype = 0;
			uint32 sLen = qstrlen(&tmpResouceBuf[index]);
			tmpSource.binAddr = lpStringStart + index;
			tmpSource.binData.resize(sLen + 1);
			memcpy(&tmpSource.binData[0], &tmpResouceBuf[index], sLen + 1);
			m_eAppInfo.m_VecUserResource.push_back(tmpSource);
			index = index + sLen;
		}
	}

	return true;
}

bool EDecompilerEngine::ParseLibInfomation(ea_t lpLibStartAddr, uint32 dwLibCount)
{
	return true;
}

EDecompilerEngine::EDecompilerEngine()
{
	m_ProgramType = E_UNKNOWN;
}

EDecompilerEngine::~EDecompilerEngine()
{
	if (gMenu_ShowResource) {
		gMenu_ShowResource->DestroyMenu();
	}
}

bool EDecompilerEngine::InitDecompilerEngine()
{
	if (!InitSectionManager()) {
		return false;
	}

	m_ProgramType = E_UNKNOWN;

	//̽�������Գ�������
	compiled_binpat_vec_t binPat;
	parse_binpat_str(&binPat, 0x0, "50 64 89 25 00 00 00 00 81 EC AC 01 00 00 53 56 57", 16);
	unsigned int segCount = get_segm_qty();
	ea_t searchAddr = BADADDR;
	for (unsigned int n = 0; n < segCount; ++n) {
		segment_t* pSegment = getnseg(n);
		searchAddr = bin_search2(pSegment->start_ea, pSegment->end_ea, binPat, 0x0);
		if (searchAddr != BADADDR) {
			break;
		}
	}

	if (searchAddr != BADADDR) {
		m_ProgramType = E_STATIC;
		m_EHeadAddr = get_dword(searchAddr + 0x26);
	}

	return true;
}

bool EDecompilerEngine::DoDecompile()
{
	//��̬�������
	if (m_ProgramType == E_STATIC) {
		return DoDecompiler_EStatic();
	}

	return true;
}

bool EDecompilerEngine::DoDecompiler_EStatic()
{
	EHead eHead;
	get_bytes(&eHead, sizeof(EHead), m_EHeadAddr);
	if (eHead.dwMagic != 0x3) {
		return false;
	}

	m_eAppInfo.m_UserCodeStartAddr = eHead.lpStartCode;
	m_eAppInfo.m_UserCodeEndAddr = m_EHeadAddr;

	if (eHead.lpEString != 0 && eHead.dwEStringSize != 0) {
		ParseStringResource(eHead.lpEString, eHead.dwEStringSize);
	}

	if (eHead.dwLibNum != 0 && eHead.lpLibEntry != 0) {
		ParseLibInfomation(eHead.lpLibEntry, eHead.dwLibNum);
	}

	if (eHead.lpEWindow != 0 && eHead.dwEWindowSize != 0) {
		ParseGUIResource(eHead.lpEWindow, eHead.dwEWindowSize);
	}

	qstring �˵�_�û���Դ�˵�;
	acp_utf8(&�˵�_�û���Դ�˵�, "������/�û�������Դ");
	gMenu_ShowResource = IDAMenu::CreateMenu(�˵�_�û���Դ�˵�.c_str(), MenuHandle_ShowUserResource);


	return true;
}