#include "CGraphicsView.h"
#include "CGraphicsScene.h"
#include "CGraphicsDef.h"
#include <QWheelEvent>
#include "CGraphicsDef.h"
#include <QScrollBar>

#include "../VisualModels/CMaze.h"

using namespace Core::Graphics;
using namespace Core::Visual;

CGraphicsView::CGraphicsView(QWidget* parent):
	QGraphicsView(parent)
{
	init();
}

void CGraphicsView::init()
{
	m_scene = new CGraphicsScene();
	this->setScene(m_scene);
	this->setVisible(true);

	this->setDragMode(QGraphicsView::NoDrag);

	// this allows us to track mouse movements
	this->setMouseTracking(true); 

	// default canvas area
	this->setSceneRect(KCanvasViewX, KCanvasViewY, KCanvasViewDefaultWidth, KCanvasViewDefaultHeight);

	// setup visual model
	m_visualModel = new CMaze(m_scene);
}

void CGraphicsView::generateVisual()
{
	//TODO: could have a reset feature, just delete and create new scene and visual model
	m_visualModel->draw();
}

void CGraphicsView::runAlgorithm()
{
	// TODO: check if model is drawn first
	m_visualModel->run();
}

void CGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
	if (m_isPanning) {
		horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->x() - m_panStartX));
		verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->y() - m_panStartY));
		m_panStartX = event->x();
		m_panStartY = event->y();
		event->accept();
		return;
	}

	QGraphicsView::mouseMoveEvent(event);
}

void CGraphicsView::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton) {
		m_isPanning = true;
		m_panStartX = event->x();
		m_panStartY = event->y();
		setCursor(Qt::ClosedHandCursor);
		event->accept();
		return;
	}

	QGraphicsView::mousePressEvent(event);
}

void CGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::MiddleButton) {
		m_isPanning = false;
		setCursor(Qt::ArrowCursor);
		event->accept();
		return;
	}

	QGraphicsView::mouseReleaseEvent(event);
}

void CGraphicsView::wheelEvent(QWheelEvent* event)
{
	if (event->modifiers() & Qt::ControlModifier) {
		// cntr + scroll for zooming
		if (event->delta() > 0) {
			scale(KZoomScale, KZoomScale);
		} else {
			scale(1 / KZoomScale, 1 / KZoomScale);
		}
	}

	QGraphicsView::wheelEvent(event);
}