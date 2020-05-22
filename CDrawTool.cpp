#include "CDrawTool.h"
#include "Graphics/CGraphicsView.h"
#include <QToolBar>

using namespace Core;
using namespace Core::Graphics;

CDrawTool::CDrawTool(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_view = new CGraphicsView();
	this->setCentralWidget(m_view);

	createToolBar();
}

void CDrawTool::createToolBar()
{
	QToolBar* toolbar = this->addToolBar("ToolBar");

	m_debugAct = new QAction("Debug", this);
	m_debugAct->setStatusTip("Degug");
	toolbar->addAction(m_debugAct);
	connect(m_debugAct, &QAction::triggered, this, &CDrawTool::onDebug);

	auto* runAct = new QAction("Run Algorithm", this);
	runAct->setStatusTip("Run Algorithm");
	toolbar->addAction(runAct);
	connect(runAct, &QAction::triggered, this, &CDrawTool::onRun);

	auto* generateAct = new QAction("Generate Graph", this);
	generateAct->setStatusTip("Generate Graph");
	toolbar->addAction(generateAct);
	connect(generateAct, &QAction::triggered, this, &CDrawTool::onGenerate);
}

void CDrawTool::onGenerate()
{
	m_view->generateVisual();
}

void CDrawTool::onRun()
{
	m_view->runAlgorithm();
}

void CDrawTool::onDebug()
{
}