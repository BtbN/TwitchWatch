#include <QtDebug>

#include <QApplication>
#include "mainwin.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setOrganizationName("btbn.de");
	app.setOrganizationDomain("btbn.de");
	app.setApplicationName("TwitchWatcher");

	MainWin win;
	win.show();

	return app.exec();
}
