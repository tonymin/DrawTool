#include "CGraphicsLine.h"
#include <QPainter>

using namespace Core;
using namespace Core::Graphics;

CGraphicsLine::CGraphicsLine(QPointF p1, QPointF p2):
	m_p1(p1),
	m_p2(p2),
	CGraphicsItem()
{

}

QRectF CGraphicsLine::boundingRect() const
{
	// TODO: need to add margins, otherwise this line will be unselectable
	return QRectF(m_p1, m_p2);
}

void CGraphicsLine::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	painter->setBrush(Qt::red);
	painter->drawLine(m_p1, m_p2);

	painter->restore();
}
