#pragma once

#include <QDialog>
#include "ui_CControlDlg.h"
class CControlDlg : public QDialog
{
	Q_OBJECT

public:
	CControlDlg(QWidget *parent = Q_NULLPTR);
	~CControlDlg();

public slots:
	void onUp();
	void onDown();
	void onLeft();
	void onRight();
signals:
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();
private:
	Ui::CControlDlg ui;
};
