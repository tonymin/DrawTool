#pragma once

#include "../Graphics/CGraphicsScene.h"
#include <QObject>

namespace Core{
namespace Visual{

class CModelBase : public QObject
{
	Q_OBJECT
public:
	virtual ~CModelBase() {}

	virtual void draw() = 0;
	virtual void run() = 0;

protected:
	CModelBase(Graphics::CGraphicsScene* scene);

protected:
	Graphics::CGraphicsScene* m_scene = nullptr;

};


}
}



