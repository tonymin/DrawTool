#include "CGraphicsScene.h"
#include <QPainter>
#include <QLineF>
#include <QGraphicsSceneMouseEvent>

#include <qdebug.h>

// graphic objects
#include "CGraphicsLine.h"
#include "CGraphicsRect.h"
#include "CGraphicsText.h"

using namespace Core::Graphics;

namespace {
	qreal boardMargin = 80;

	QPointF m_mouseTracking;
	QPointF m_gridSnap;
	QRectF kSceneRect(0,0,1000,1000);
}

CGraphicsScene::CGraphicsScene(QObject* parent):
	QGraphicsScene(parent)
{
	this->setSceneRect(kSceneRect);
}

QPointF CGraphicsScene::snapToGrid(QPointF p)
{
	int x = (int)((p.x()+ KCanvasGridSpace/2) / KCanvasGridSpace) * KCanvasGridSpace;
	int y = (int)((p.y() + KCanvasGridSpace / 2) / KCanvasGridSpace) * KCanvasGridSpace;
	return QPointF(x, y);
}

void CGraphicsScene::drawBackground(QPainter* p, const QRectF& rect)
{
	
	const QRectF rectNorm = rect.normalized();
	p->save();
	p->setPen(QPen(Qt::lightGray, 1));
	int l = int(rectNorm.left());
	l -= (l % KCanvasGridSpace);

	int r = int(rectNorm.right());
	r -= (r % KCanvasGridSpace);
	if (r < int(rectNorm.right()))
		r += KCanvasGridSpace;

	int t = int(rectNorm.top());
	t -= (t % KCanvasGridSpace);

	int b = int(rectNorm.bottom());
	b -= (b % KCanvasGridSpace);
	if (b < int(rectNorm.bottom()))
		b += KCanvasGridSpace;

	for (int x = l; x <= r; x += KCanvasGridSpace)
		for (int y = t; y <= b; y += KCanvasGridSpace)
			p->drawPoint(x, y);

	p->restore();

	// grid snap indicator
	if (!m_gridSnap.isNull()) {
		qreal radius = KCanvasGridSpace / 4;
		p->save();
		p->setPen(Qt::blue);
		p->drawEllipse(m_gridSnap, radius, radius);
		p->restore();
	}
	
}

void CGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
	m_mouseTracking = mouseEvent->scenePos();
	m_gridSnap = this->snapToGrid(m_mouseTracking);


	this->update();
	QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void CGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{

	/*
	CGraphicsLine* line = new CGraphicsLine(QPointF(0, 0), QPointF(0, 100));
	line->setPos(m_gridSnap);
	this->addItem(line);
	*/

	/*
	CGraphicsRect* rect = new CGraphicsRect(10, 10);
	rect->setPos(m_gridSnap);
	this->addItem(rect);
	*/

	/*
	CGraphicsText* text = new CGraphicsText("S");
	text->setPos(m_gridSnap);
	this->addItem(text);
	*/

	QGraphicsScene::mousePressEvent(mouseEvent);
}
