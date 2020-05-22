#include "CGraphicsRect.h"
#include <QPainter> 



using namespace Core::Graphics;


CGraphicsRect::CGraphicsRect(const qreal width, const qreal height):
	m_width(width),
	m_height(height),
	m_topLeft(QPointF(-width/2, -height/2)),
	CGraphicsItem()
{
	
}

QRectF CGraphicsRect::boundingRect() const
{
	return QRectF(m_topLeft.x(), m_topLeft.y(), m_width, m_height);
}

void CGraphicsRect::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	painter->setBrush(Qt::black);
	painter->drawRect(m_topLeft.x(), m_topLeft.y(), m_width, m_height);

	painter->restore();

}
