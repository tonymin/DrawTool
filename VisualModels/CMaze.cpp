#include "CMaze.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include "CControlDlg.h"
#include <cmath>

// graphics
#include "../Graphics/CGraphicsRect.h"
#include "../Graphics/CGraphicsText.h"
#include "../Graphics/CGraphicsLine.h"

#include <QThread>



using namespace Core::Graphics;
using namespace Core::Visual;

namespace {
const int offset = 10;
}


CMaze::CMaze(Graphics::CGraphicsScene* scene):
	CModelBase(scene)
{
}

void CMaze::draw()
{
	// ask for csv file
	QString fileName = QFileDialog::getOpenFileName(nullptr, "Open CSV Maze Map",
		QDir::homePath(), "CSV (*.csv)");
	if (fileName.isEmpty()) {
		return;
	}

	QFile file(fileName);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(nullptr,"Failed to read file", file.errorString());
		return;
	}
	

	std::vector<std::vector<QString>> grid;
	while (!file.atEnd()) {
		auto dataList = file.readLine().split(','); //QList<QByteArray>
		std::vector<QString> myRow;
		for (auto b: dataList) {
			QString val(b);
			myRow.push_back(val.trimmed());
		}
		grid.push_back(myRow);
	}

	// draw maze elements
	for (int row = 0; row < grid.size(); row++) {
		for (int col = 0; col < grid[row].size(); col++) {
			if (0 == grid[row][col].compare("1")) {
				// obstacle
				auto* rect = new CGraphicsRect();
				m_scene->addItem(rect);
				int xPos = (col + offset) * KCanvasGridSpace;
				int yPos = (row + offset) * KCanvasGridSpace;
				rect->setPos(xPos, yPos);
				//m_obstacleMap[CCoord(col, row)] = 1;
				m_obstacleMap.insert(CCoord(col, row));
			} else if (0 == grid[row][col].compare("0")) {
				// free space
			} else {
				// text
				auto* text = new CGraphicsText(grid[row][col]);
				text->setZValue(2); // we want to paint text on top of everything
				m_scene->addItem(text);
				int xPos = (col + offset) * KCanvasGridSpace;
				int yPos = (row + offset) * KCanvasGridSpace;
				text->setPos(xPos, yPos);
				
				if (grid[row][col].compare("s",Qt::CaseInsensitive) == 0) {
					// starting position
					m_startPosition.xCoord = col;
					m_startPosition.yCoord = row;
					m_marker = text;
				} else {
					// goal positions
					CCoord goal(col, row);
					m_goalPositions.insert(goal);
				}
			}
		}
	}

	// configure boundaries
	m_xMax = grid[0].size();
	m_yMax = grid.size();

	// draw boundaries
	int base = offset * KCanvasGridSpace - KCanvasGridSpace/2;
	int width = grid[0].size() * KCanvasGridSpace;
	int height = grid.size() * KCanvasGridSpace;
	QPointF topLeft(base, base);
	QPointF topRight((base + width), base);
	QPointF botLeft(base, base + height);
	QPointF botRight((base + width), (base + height));
	auto* top = new CGraphicsLine(QPointF(0,0), QPointF(width, 0));
	top->setPos(topLeft);
	auto* bot = new CGraphicsLine(QPointF(0, 0), QPointF(width, 0));
	bot->setPos(botLeft);
	auto* left = new CGraphicsLine(QPointF(0, 0), QPointF(0, height));
	left->setPos(topLeft);
	auto* right = new CGraphicsLine(QPointF(0, 0), QPointF(0, height));
	right->setPos(topRight);
	m_scene->addItem(top);
	m_scene->addItem(bot);
	m_scene->addItem(left);
	m_scene->addItem(right);

	
}

void CMaze::markPosition(const CCoord p, QColor color)
{
	auto* rect = new CGraphicsRect();
	rect->setColor(color);
	m_scene->addItem(rect);
	int xPos = (p.xCoord + offset) * KCanvasGridSpace;
	int yPos = (p.yCoord + offset) * KCanvasGridSpace;
	rect->setPos(xPos, yPos);
}

bool CMaze::validateMove(const CCoord from, const CCoord to) const
{
	if (to.xCoord >= m_xMax || to.xCoord < 0 ||
		to.yCoord >= m_yMax || to.yCoord < 0) {
		return false;
	}

	if (m_obstacleMap.find(to) != m_obstacleMap.end()) {
		return false;
	}

	return true;
}

void CMaze::moveUp()
{
	CCoord target(m_startPosition.xCoord, m_startPosition.yCoord-1);

	if (validateMove(m_startPosition, target)) {
		m_startPosition = target;
		auto pos = m_marker->pos();
		pos.setY(pos.y() - KCanvasGridSpace);
		m_marker->setPos(pos);
	}
}

void CMaze::moveDown()
{
	CCoord target(m_startPosition.xCoord, m_startPosition.yCoord+1);

	if (validateMove(m_startPosition, target)) {
		m_startPosition = target;
		auto pos = m_marker->pos();
		pos.setY(pos.y() + KCanvasGridSpace);
		m_marker->setPos(pos);
	}
}

void CMaze::moveLeft()
{
	CCoord target(m_startPosition.xCoord - 1, m_startPosition.yCoord);

	if (validateMove(m_startPosition, target)) {
		m_startPosition = target;
		auto pos = m_marker->pos();
		pos.setX(pos.x() - KCanvasGridSpace);
		m_marker->setPos(pos);
	}
}

void CMaze::moveRight()
{
	CCoord target(m_startPosition.xCoord + 1, m_startPosition.yCoord);

	if (validateMove(m_startPosition, target)) {
		m_startPosition = target;
		auto pos = m_marker->pos();
		pos.setX(pos.x() + KCanvasGridSpace);
		m_marker->setPos(pos);
	}
}

void CMaze::BFS()
{
	// FIFO open list
	std::list<CCoord> openList;
	std::set<CCoord> openSet; // for checking repeated nodes

	// closed set
	std::set<CCoord> closedSet; // unordered, used for checking reptitions
	std::list<CCoord> closedList; // ordered, used for output

	openList.push_back(m_startPosition);
	openSet.insert(m_startPosition);
	bool found = false;
	CCoord goalNode(-1,-1);
	while (!openList.empty()) {

		// expand first node. Remove from open list/set, update closed list/set
		auto expand = openList.front();
		openList.pop_front();
		openSet.erase(openSet.find(expand)); // note: This line will crash if openList is out of sync with openSet
		closedList.push_back(expand);
		closedSet.insert(expand);
		
		markPosition(expand, Qt::yellow);
		qApp->processEvents();
		QThread::msleep(20);

		// check goal state
		if (m_goalPositions.find(expand) != m_goalPositions.end()) {
			// found goal
			found = true;
			goalNode = expand;
			break;
		}

		// generate children and add to list. Branching factor = 4
		int x = expand.xCoord;
		int y = expand.yCoord;

		CCoord top(x, y - 1); // (y-axis grows downwards in Qt cartesian system)
		top.setParent(x, y);
		// add children if not repeated, and move is valid
		if (closedSet.find(top) == closedSet.end() &&
			openSet.find(top) == openSet.end() &&
			validateMove(expand, top)) {
			openList.push_back(top);
			openSet.insert(top);
		}

		CCoord down(x, y + 1);
		down.setParent(x, y);
		if (closedSet.find(down) == closedSet.end() &&
			openSet.find(down) == openSet.end() &&
			validateMove(expand, down)) {
			openList.push_back(down);
			openSet.insert(down);
		}

		CCoord left(x-1, y);
		left.setParent(x, y);
		if (closedSet.find(left) == closedSet.end() &&
			openSet.find(left) == openSet.end() &&
			validateMove(expand, left)) {
			openList.push_back(left);
			openSet.insert(left);
		}

		CCoord right(x+1, y);
		right.setParent(x, y);
		if (closedSet.find(right) == closedSet.end() &&
			openSet.find(right) == openSet.end() &&
			validateMove(expand, right)) {
			openList.push_back(right);
			openSet.insert(right);
		}

	}

	if (found) {
		QMessageBox::information(nullptr, "Result", "Found goal!");
		//back track
		auto current = goalNode;
		while (current != m_startPosition) {
			markPosition(current, Qt::green);
			CCoord parent(current.parentX, current.parentY);
			auto parentIt = closedSet.find(parent);
			if (parentIt != closedSet.end()) {
				current = *parentIt;
			} else {
				// eh, bug!
				QMessageBox::information(nullptr, "Back-track", "Bugged out!");
				break;
			}
		}
		markPosition(m_startPosition, Qt::green);

	} else {
		QMessageBox::information(nullptr, "Result", "Did not find goal...");
	}

}

void CMaze::DFS()
{
	// LIFO open list
	std::list<CCoord> openList;
	std::set<CCoord> openSet; // for checking repeated nodes

	// closed set
	std::set<CCoord> closedSet; // unordered, used for checking reptitions
	std::list<CCoord> closedList; // ordered, used for output

	openList.push_back(m_startPosition);
	openSet.insert(m_startPosition);
	bool found = false;
	CCoord goalNode(-1, -1);
	while (!openList.empty()) {

		// expand first node. Remove from open list/set, update closed list/set
		auto expand = openList.back(); // NOTE: back instead of front
		openList.pop_back(); // NOTE: pop_back instead of pop_front
		openSet.erase(openSet.find(expand)); // note: This line will crash if openList is out of sync with openSet
		closedList.push_back(expand);
		closedSet.insert(expand);

		// visuals
		markPosition(expand, Qt::yellow);
		qApp->processEvents();
		QThread::msleep(20);

		// check goal state
		if (m_goalPositions.find(expand) != m_goalPositions.end()) {
			// found goal
			found = true;
			goalNode = expand;
			break;
		}

		// generate children and add to list. Branching factor = 4
		int x = expand.xCoord;
		int y = expand.yCoord;

		CCoord top(x, y - 1); // (y-axis grows downwards in Qt cartesian system)
		top.setParent(x, y);
		// add children if not repeated, and move is valid
		if (closedSet.find(top) == closedSet.end() &&
			openSet.find(top) == openSet.end() &&
			validateMove(expand, top)) {
			openList.push_back(top);
			openSet.insert(top);
		}

		CCoord down(x, y + 1);
		down.setParent(x, y);
		if (closedSet.find(down) == closedSet.end() &&
			openSet.find(down) == openSet.end() &&
			validateMove(expand, down)) {
			openList.push_back(down);
			openSet.insert(down);
		}

		CCoord left(x - 1, y);
		left.setParent(x, y);
		if (closedSet.find(left) == closedSet.end() &&
			openSet.find(left) == openSet.end() &&
			validateMove(expand, left)) {
			openList.push_back(left);
			openSet.insert(left);
		}

		CCoord right(x + 1, y);
		right.setParent(x, y);
		if (closedSet.find(right) == closedSet.end() &&
			openSet.find(right) == openSet.end() &&
			validateMove(expand, right)) {
			openList.push_back(right);
			openSet.insert(right);
		}

	}

	if (found) {
		QMessageBox::information(nullptr, "Result", "Found goal!");
		//back track
		auto current = goalNode;
		while (current != m_startPosition) {
			markPosition(current, Qt::green);
			CCoord parent(current.parentX, current.parentY);
			auto parentIt = closedSet.find(parent);
			if (parentIt != closedSet.end()) {
				current = *parentIt;
			} else {
				// eh, bug!
				QMessageBox::information(nullptr, "Back-track", "Bugged out!");
				break;
			}
		}
		markPosition(m_startPosition, Qt::green);

	} else {
		QMessageBox::information(nullptr, "Result", "Did not find goal...");
	}
}


int CMaze::h(CCoord n, CCoord target)
{
	// function that represent distance to target (not direct, but sum of x and y distance)
	int xDist = std::abs(target.xCoord - n.xCoord);
	int yDist = std::abs(target.yCoord - n.yCoord);
	return xDist + yDist;
}

void CMaze::AS()
{
	// open list/set
	std::list<CCoord> openList;
	std::set<CCoord> openSet; // for checking repeated nodes

	// closed list/set
	std::set<CCoord> closedSet; // unordered, used for checking reptitions
	std::list<CCoord> closedList; // ordered, used for output

	openList.push_back(m_startPosition);
	openSet.insert(m_startPosition);
	bool found = false;
	CCoord goalNode(-1, -1);
	while (!openList.empty()) {

		// expand first node. Remove from open list/set, update closed list/set
		auto expand = openList.front();
		openList.pop_front();
		openSet.erase(openSet.find(expand)); // note: This line will crash if openList is out of sync with openSet
		closedList.push_back(expand);
		closedSet.insert(expand);

		// visuals
		markPosition(expand, Qt::yellow);
		qApp->processEvents();
		QThread::msleep(20);

		// check goal state
		if (m_goalPositions.find(expand) != m_goalPositions.end()) {
			// found goal
			found = true;
			goalNode = expand;
			break;
		}

		// generate children and add to list. Branching factor = 4
		int x = expand.xCoord;
		int y = expand.yCoord;
		int cost = expand.gn++; // cost to travel to neighbouring nodes
		CCoord targetNode = *m_goalPositions.begin(); // assuming graph has a goal

		CCoord top(x, y - 1); // (y-axis grows downwards in Qt cartesian system)
		top.setParent(x, y);
		top.gn = cost;
		top.hn = h(top, targetNode);
		// add children if not repeated, and move is valid
		if (closedSet.find(top) == closedSet.end() &&
			openSet.find(top) == openSet.end() &&
			validateMove(expand, top)) {
			openList.push_back(top);
			openSet.insert(top);
		}

		CCoord down(x, y + 1);
		down.setParent(x, y);
		down.gn = cost;
		down.hn = h(down, targetNode);
		if (closedSet.find(down) == closedSet.end() &&
			openSet.find(down) == openSet.end() &&
			validateMove(expand, down)) {
			openList.push_back(down);
			openSet.insert(down);
		}

		CCoord left(x - 1, y);
		left.setParent(x, y);
		left.gn = cost;
		left.hn = h(left, targetNode);
		if (closedSet.find(left) == closedSet.end() &&
			openSet.find(left) == openSet.end() &&
			validateMove(expand, left)) {
			openList.push_back(left);
			openSet.insert(left);
		}

		CCoord right(x + 1, y);
		right.setParent(x, y);
		right.gn = cost;
		right.hn = h(right, targetNode);
		if (closedSet.find(right) == closedSet.end() &&
			openSet.find(right) == openSet.end() &&
			validateMove(expand, right)) {
			openList.push_back(right);
			openSet.insert(right);
		}

		// sort frontier list by increase value of gn + hn

		openList.sort([](const CCoord& A, const CCoord& B) {
			return (A.gn + A.hn) < (B.gn + B.hn);
			});
		/*
		std::sort(openList.begin(), openList.end(), [](const CCoord& A, const CCoord& B) {
			return (A.gn + A.hn) < (B.gn + B.hn);
		});*/

	}

	if (found) {
		QMessageBox::information(nullptr, "Result", "Found goal!");
		//back track
		auto current = goalNode;
		while (current != m_startPosition) {
			markPosition(current, Qt::green);
			CCoord parent(current.parentX, current.parentY);
			auto parentIt = closedSet.find(parent);
			if (parentIt != closedSet.end()) {
				current = *parentIt;
			} else {
				// eh, bug!
				QMessageBox::information(nullptr, "Back-track", "Bugged out!");
				break;
			}
		}
		markPosition(m_startPosition, Qt::green);

	} else {
		QMessageBox::information(nullptr, "Result", "Did not find goal...");
	}
}

void CMaze::run()
{
	//BFS();
	//DFS();
	AS();


	// just for debug
	/*
	CControlDlg dlg;
	connect(&dlg, &CControlDlg::moveUp, this, &CMaze::moveUp);
	connect(&dlg, &CControlDlg::moveDown, this, &CMaze::moveDown);
	connect(&dlg, &CControlDlg::moveLeft, this, &CMaze::moveLeft);
	connect(&dlg, &CControlDlg::moveRight, this, &CMaze::moveRight);
	dlg.exec();
	*/

}