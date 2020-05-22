#pragma once

#include "CModelBase.h"
#include "../Graphics/CGraphicsItem.h"
#include <set>

namespace Core {
namespace Visual {

struct CCoord
{
	// position data
	int xCoord = -1;
	int yCoord = -1;
	// back-tracking data
	int parentX = -1;
	int parentY = -1;

	// for A*, g(n) cost, h(n) cost
	int gn = 0;
	int hn = 0;


	CCoord(int x, int y) {
		xCoord = x;
		yCoord = y;
	}
	
	void setParent(int x, int y) { 
		parentX = x;
		parentY = y;
	}

	bool operator<(const CCoord& ob) const
	{
		return xCoord < ob.xCoord || (xCoord == ob.xCoord && yCoord < ob.yCoord);
	}

	bool operator==(const CCoord& ob) const
	{
		return xCoord == ob.xCoord && yCoord == ob.yCoord;
	}

	bool operator!=(const CCoord& ob) const
	{
		return xCoord != ob.xCoord || yCoord != ob.yCoord;
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

	void BFS();
	void DFS();
	void AS();


	bool validateMove(const CCoord from, const CCoord to) const;
	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void markPosition(const CCoord p, QColor color);

	//A* heuristic function
	int h(CCoord n, CCoord target);

private:
	//std::map <CCoord,int,std::less<>> m_obstacleMap;
	std::set<CCoord> m_obstacleMap;
	CCoord m_startPosition = CCoord(-1,-1);
	Graphics::CGraphicsItem* m_marker = nullptr;
	std::set<CCoord> m_goalPositions;

	int m_xMax = 0;
	int m_yMax = 0;

}; // CMaze

}
}