#include "CControlDlg.h"

CControlDlg::CControlDlg(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	
	connect(ui.m_downBtn, &QPushButton::clicked, this, &CControlDlg::onDown);
	connect(ui.m_upBtn, &QPushButton::clicked, this, &CControlDlg::onUp);
	connect(ui.m_rightBtn, &QPushButton::clicked, this, &CControlDlg::onRight);
	connect(ui.m_leftBtn, &QPushButton::clicked, this, &CControlDlg::onLeft);
}

CControlDlg::~CControlDlg()
{
}

void CControlDlg::onUp()
{
	emit moveUp();
}
void CControlDlg::onDown()
{
	emit moveDown();
}
void CControlDlg::onLeft()
{
	emit moveLeft();
}
void CControlDlg::onRight()
{
	emit moveRight();
}
