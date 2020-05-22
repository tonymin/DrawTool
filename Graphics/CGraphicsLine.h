#pragma once
#include "CGraphicsItem.h"

namespace Core {
namespace Graphics {

class CGraphicsLine : public CGraphicsItem
{
public:

	CGraphicsLine(QPointF p1, QPointF p2);
	virtual ~CGraphicsLine() {}

	QRectF boundingRect() const override;

protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

protected:
	QPointF m_p1;
	QPointF m_p2;


};

}
}
