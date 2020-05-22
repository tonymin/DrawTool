#pragma once

#include "CModelBase.h"
#include "../Graphics/CGraphicsItem.h"
#include <set>

namespace Core {
namespace Visual {

struct CCoord
{
	int xCoord = -1;
	int yCoord = -1;

	CCoord(int x, int y) {
		xCoord = x;
		yCoord = y;
	}
	bool operator<(const CCoord& ob) const
	{
		return xCoord < ob.xCoord || (xCoord == ob.xCoord && yCoord < ob.yCoord);
	}
};


class CMaze : public CModelBase
{
	
public:
	CMaze(Graphics::CGraphicsScene* scene);
	virtual ~CMaze() {}

	void draw() override;
	void run() override;

	

private:
	bool validateMove(const CCoord from, const CCoord to) const;
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();




private:
	//std::map <CCoord,int,std::less<>> m_obstacleMap;
	std::set<CCoord> m_obstacleMap;
	CCoord m_startPosition = CCoord(-1,-1);
	Graphics::CGraphicsItem* m_marker = nullptr;
	std::vector<CCoord> m_goalPositions;

	int m_xMax = 0;
	int m_yMax = 0;

}; // CMaze

}
}