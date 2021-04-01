#include "ControlInfoWidget.h"
#include "EDecompiler.h"

extern EDecompilerEngine g_MyDecompiler;

ControlInfoWidget::ControlInfoWidget()
{
	ui.setupUi(this);

	connect(ui.treeWidget, &QTreeWidget::itemClicked, this, [&](QTreeWidgetItem* item, int column) {
		on_controlClicked(item, column);
	});
}

ControlInfoWidget::~ControlInfoWidget()
{

}

void ControlInfoWidget::on_controlClicked(QTreeWidgetItem* item, int column)
{
	QTreeWidgetItem* parentItem = item->parent();
	if (!item->parent()) {
		//����������ϲ�ڵ�
		ui.groupBox->setTitle(QStringLiteral("����"));
	}
	else {
		unsigned int tmpWindowId = parentItem->data(0, Qt::UserRole).toUInt();
		for (unsigned int nWindowIndex = 0; nWindowIndex < g_MyDecompiler.m_eAppInfo.mVec_GuiInfo.size(); ++nWindowIndex) {
			if (g_MyDecompiler.m_eAppInfo.mVec_GuiInfo[nWindowIndex].m_windowId == tmpWindowId) {
				qvector<mid_ControlInfo>& vec_ControlInfo = g_MyDecompiler.m_eAppInfo.mVec_GuiInfo[nWindowIndex].mVec_ControlInfo;
				for (unsigned int nControlIndex = 0; nControlIndex < vec_ControlInfo.size(); ++nControlIndex) {
					if (vec_ControlInfo[nControlIndex].m_controlId == item->data(0, Qt::UserRole).toUInt()) {

						//�ҵ�����Ŀؼ���
						ui.groupBox->setTitle(QString::fromLocal8Bit(vec_ControlInfo[nControlIndex].m_controlTypeName.c_str()));
						int a = 0;
					}
				}
			}
		}
	}
	int a = 0;
}