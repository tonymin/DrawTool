#pragma once

#include <QGraphicsView>
#include <QPointer>
#include "../VisualModels/CModelBase.h"

namespace Core {
namespace Graphics{
class CGraphicsScene;


class CGraphicsView : public QGraphicsView
{
	Q_OBJECT
public:
	CGraphicsView(QWidget* parent = Q_NULLPTR);
	~CGraphicsView() {}

	void generateVisual();
	void runAlgorithm();


public slots:
	
protected:
	void mouseMoveEvent(QMouseEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void wheelEvent(QWheelEvent* event) override;

private:
	void init();

private:
	QPointer<CGraphicsScene> m_scene = nullptr;

	bool m_isPanning = false;
	int m_panStartX, m_panStartY;
	Visual::CModelBase* m_visualModel = nullptr;

};

}}