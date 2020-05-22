#include "CGraphicsText.h"
#include <QPainter>

using namespace Core::Graphics;

CGraphicsText::CGraphicsText(const QString str) :
	m_text(str),
	CGraphicsItem()
{
	m_textBox = QRectF(-KCanvasGridSpace / 2, -KCanvasGridSpace / 2, KCanvasGridSpace, KCanvasGridSpace);
}

QRectF CGraphicsText::boundingRect() const
{
	return m_textBox;
}

void CGraphicsText::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->save();

	painter->setPen(Qt::blue);
	painter->setFont(QFont("Arial", 10));
	painter->drawText(m_textBox, Qt::AlignCenter, m_text);

	painter->restore();
}
