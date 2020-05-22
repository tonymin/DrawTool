#include "CMaze.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QDialog>
#include <QPushButton>
#include <QHBoxLayout>
#include "CControlDlg.h"

// graphics
#include "../Graphics/CGraphicsRect.h"
#include "../Graphics/CGraphicsText.h"
#include "../Graphics/CGraphicsLine.h"



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
				m_scene->addItem(text);
				int xPos = (col + offset) * KCanvasGridSpace;
				int yPos = (row + offset) * KCanvasGridSpace;
				text->setPos(xPos, yPos);
				
				if (grid[row][col].compare("s",Qt::CaseInsensitive) == 0) {
					// starting position
					m_startPosition.xCoord = col;
					m_startPosition.yCoord = row;
					m_marker = text;
				} else if (grid[row][col].compare("s", Qt::CaseInsensitive) == 0) {
					// goal positions
					CCoord goal(col, row);
					m_goalPositions.push_back(goal);
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

void CMaze::run()
{
	// just for debug
	CControlDlg dlg;
	connect(&dlg, &CControlDlg::moveUp, this, &CMaze::moveUp);
	connect(&dlg, &CControlDlg::moveDown, this, &CMaze::moveDown);
	connect(&dlg, &CControlDlg::moveLeft, this, &CMaze::moveLeft);
	connect(&dlg, &CControlDlg::moveRight, this, &CMaze::moveRight);
	dlg.exec();

}