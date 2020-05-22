#pragma once

#include <QGraphicsItem>
#include "CGraphicsDef.h"

namespace Core {
namespace Graphics {

class CGraphicsItem : public QGraphicsItem
{
public:
	
	virtual ~CGraphicsItem() {}

protected:
	CGraphicsItem();

protected:
	QPointF m_pos;
	

};

}
}

