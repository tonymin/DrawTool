#pragma once
#include "CGraphicsItem.h"

namespace Core {
namespace Graphics {

class CGraphicsRect :  public CGraphicsItem
{

public:
	CGraphicsRect(const qreal width = KCanvasGridSpace, const qreal height = KCanvasGridSpace);
	virtual ~CGraphicsRect() {}

	QRectF boundingRect() const override;

protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:
	qreal m_width, m_height;
	QPointF m_topLeft;

};


}
}

