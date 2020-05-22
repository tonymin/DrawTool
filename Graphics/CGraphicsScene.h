#pragma once

#include <QGraphicsScene>
#include "CGraphicsDef.h"

namespace Core {
namespace Graphics {

class CGraphicsScene : public QGraphicsScene
{
	Q_OBJECT
public:
	CGraphicsScene(QObject* parent = Q_NULLPTR);
	~CGraphicsScene() {}

protected:
	void drawBackground(QPainter* painter, const QRectF& rect) override;
	void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
	void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

private:
	QPointF snapToGrid(QPointF p);
	EDrawMode m_drawMode = EDrawMode::None;
	
};

}}
