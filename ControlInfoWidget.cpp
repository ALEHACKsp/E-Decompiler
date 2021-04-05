#include "ControlInfoWidget.h"
#include "EDecompiler.h"
#include <bytes.hpp>
#include "public.h"
#include "E_WinForm.h"
#include "PropertyDelegate.h"

extern EDecompilerEngine g_MyDecompiler;

#define COLUMN_PropertyName  0
#define COLUMN_PropertyValue 1



void ControlInfoWidget::InitKernelControl_Window(mid_ControlInfo* pControl)
{
	qvector<unsigned char> tmpBuf;
	tmpBuf.resize(pControl->m_propertySize);
	get_bytes(&tmpBuf[0], pControl->m_propertySize, pControl->m_propertyAddr);
	unsigned char* lpControlInfo = &tmpBuf[0];

	ui.ControlTable->setRowCount(33);

	//������ʵ��û�����ֵ�
	//qstring controlName;
	//controlName.sprnt("����_%08a", pControl->m_controlId);
	ui.ControlTable->setItem(0, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	ui.ControlTable->setItem(0, COLUMN_PropertyValue, new QTableWidgetItem(ui_LineEditor_Disabled));

	{
		uint32 dwControlTypeId = ReadUInt(lpControlInfo);
		lpControlInfo += 4;
		//�̶���20�����ֽ�,����ʹ��?
		lpControlInfo += 20;
		//�����ַ���1?
		ReadStr(lpControlInfo);
		lpControlInfo += qstrlen(lpControlInfo) + 1;
		//�����ַ���2?
		ReadStr(lpControlInfo);
		lpControlInfo += qstrlen(lpControlInfo) + 1;
		//�洢����?
		ReadUInt(lpControlInfo);
		lpControlInfo += 4;
	}

	int left = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(1, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���")));
	ui.ControlTable->setItem(1, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(left)));


	int top = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(2, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	ui.ControlTable->setItem(2, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(top)));

	int width = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(3, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���")));
	ui.ControlTable->setItem(3, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(width)));

	int height = ReadInt(lpControlInfo);
	lpControlInfo += 4;
	ui.ControlTable->setItem(4, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("�߶�")));
	ui.ControlTable->setItem(4, COLUMN_PropertyValue, new QTableWidgetItem(QString::number(height)));

	//δֵ֪
	lpControlInfo += 8;

	//һ������?
	unsigned int index1 = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	//���ָ��
	unsigned int cursorFlag = ReadUInt(lpControlInfo + 4);
	QString str_Cursor;
	switch (cursorFlag)
	{
	case 0x0:
		str_Cursor = QStringLiteral("Ĭ����");
		break;
	case 0x00007F00:
		str_Cursor = QStringLiteral("��׼��ͷ��");
		break;
	case 0x00007F03:
		str_Cursor = QStringLiteral("ʮ����");
		break;
	case 0x00007F01:
		str_Cursor = QStringLiteral("�ı��༭��");
		break;
	case 0x00007F02:
		str_Cursor = QStringLiteral("ɳ©��");
		break;
	case 0x00007F8B:
		str_Cursor = QStringLiteral("��ͷ���ʺ���");
		break;
	case 0x00007F8A:
		str_Cursor = QStringLiteral("��ͷ��ɳ©��");
		break;
	case 0x00007F88:
		str_Cursor = QStringLiteral("��ֹ����");
		break;
	case 0x00007F86:
		str_Cursor = QStringLiteral("�����ͷ��");
		break;
	case 0x00007F83:
		str_Cursor = QStringLiteral("��<->����ͷ��");
		break;
	case 0x00007F85:
		str_Cursor = QStringLiteral("��<->�ϼ�ͷ��");
		break;
	case 0x00007F82:
		str_Cursor = QStringLiteral("��<->����ͷ��");
		break;
	case 0x00007F84:
		str_Cursor = QStringLiteral("��<->����ͷ��");
		break;
	case 0x00007F04:
		str_Cursor = QStringLiteral("���ϼ�ͷ��");
		break;
	case 0x00007F89:
		str_Cursor = QStringLiteral("����");
		break;
	case 0xFFFFFFFF:
		str_Cursor = QStringLiteral("�Զ�����");
		break;
	default:
		str_Cursor = QStringLiteral("��������");
		break;
	}

	ui.ControlTable->setItem(8, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���ָ��")));
	ui.ControlTable->setItem(8, COLUMN_PropertyValue, new QTableWidgetItem(str_Cursor));

	//����ƫ��?
	unsigned int offset2 = ReadUInt(lpControlInfo + (index1 * 4));
	lpControlInfo += index1 * 4;
	lpControlInfo += offset2 + 4;
	
	//���
	qstring m_tag = ReadStr(lpControlInfo);
	lpControlInfo += qstrlen(lpControlInfo) + 1;
	ui.ControlTable->setItem(5, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("���")));
	ui.ControlTable->setItem(5, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(m_tag.c_str())));

	unsigned int unKnowValueA = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	unsigned int flags = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	ui.ControlTable->setItem(6, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	if (flags & 0x1) {
		ui.ControlTable->setItem(6, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}
	else {
		ui.ControlTable->setItem(6, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}

	ui.ControlTable->setItem(7, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("��ֹ")));
	if (flags & 0x2) {
		ui.ControlTable->setItem(7, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}
	else {
		ui.ControlTable->setItem(7, COLUMN_PropertyValue, new QTableWidgetItem(QStringLiteral("��")));
	}

	unsigned int unKnowValueB = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	unsigned int index2 = ReadUInt(lpControlInfo);
	lpControlInfo += 4;

	lpControlInfo += (index2 * 8) + 0x14;

	//����������������������������ʣ�µ�����ΪUnitData����������������������������
	int UnitDataSize = (&tmpBuf[0] - lpControlInfo) + pControl->m_propertySize;

	WinFrom_UnitData WinData = {};
	Unserialize_WinFormData(lpControlInfo, &WinData);

	ui.ControlTable->setItem(9, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	if (WinData.����.size()) {
		ui.ControlTable->setItem(9, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(WinData.����.c_str())));
	}
	return;
}

void ControlInfoWidget::InitKernelControl_Label(mid_ControlInfo* pControl)
{
	ui.ControlTable->insertRow(0);
	ui.ControlTable->setItem(0, COLUMN_PropertyName, new QTableWidgetItem(QStringLiteral("����")));
	ui.ControlTable->setItem(0, COLUMN_PropertyValue, new QTableWidgetItem(QString::fromLocal8Bit(pControl->m_controlName.c_str())));
}



ControlInfoWidget::ControlInfoWidget()
{
	ui.setupUi(this);

	ui.ControlTable->setColumnCount(2);
	ui.ControlTable->setSelectionMode(QAbstractItemView::SingleSelection);
	ui.ControlTable->setSelectionBehavior(QAbstractItemView::SelectItems);
	ui.ControlTable->setItemDelegateForColumn(COLUMN_PropertyValue, new PropertyDelegate(ui.ControlTable));

	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, [&](QTreeWidgetItem* item, int column) {
		on_controlClicked(item, column);
	});
}

ControlInfoWidget::~ControlInfoWidget()
{

}

void ControlInfoWidget::on_controlClicked(QTreeWidgetItem* item, int column)
{
	ui.ControlTable->setRowCount(0);
	ui.ControlTable->clearContents();

	QTreeWidgetItem* parentItem = item->parent();
	if (!item->parent()) {
		//����������ϲ�ڵ�
		ui.groupBox->setTitle(QStringLiteral("����"));
	}
	else {
		unsigned int controlId = item->data(0, Qt::UserRole).toUInt();

		mid_ControlInfo currentControl;
		if (EDecompilerEngine::GetControlInfo(controlId, currentControl)) {

			ui.groupBox->setTitle(QString::fromLocal8Bit(currentControl.m_controlTypeName.c_str()));

			ControlType_t controlType = EDecompilerEngine::GetControlType(currentControl.m_controlTypeId);
			switch (controlType)
			{
			case EC_UnknownControl:
				break;
			case EC_Window:
				InitKernelControl_Window(&currentControl);
				break;
			case EC_Label:
				InitKernelControl_Label(&currentControl);
				break;
			default:
				break;
			}
		}

	}

}