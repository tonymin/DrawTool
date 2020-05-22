#pragma once

#include "CGraphicsItem.h"

namespace Core {
namespace Graphics {


class CGraphicsText : public CGraphicsItem
{
public:
	CGraphicsText(const QString str);
	virtual ~CGraphicsText() {}

	QRectF boundingRect() const override;

protected:
	void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

private:

	QRectF m_textBox;
	QString m_text;
};


}
}
