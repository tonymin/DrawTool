#include "CDrawTool.h"
#include <QtWidgets/QApplication>

using namespace Core;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CDrawTool w;
	w.show();
	return a.exec();
}
