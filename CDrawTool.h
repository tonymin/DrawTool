#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_DrawTool.h"
#include <QPointer>

namespace Core {
namespace Graphics {
	class CGraphicsView;
}
}

namespace Core {

class CDrawTool : public QMainWindow
{
	Q_OBJECT

public:
	CDrawTool(QWidget *parent = Q_NULLPTR);

private:
	void createToolBar();

private slots:
	void onDebug();
	void onGenerate();
	void onRun();

private:
	Ui::DrawToolClass ui;
	QPointer<Graphics::CGraphicsView> m_view;

	QAction* m_debugAct = nullptr;

};

}