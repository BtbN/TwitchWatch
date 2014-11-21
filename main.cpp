#include <QtDebug>
#include <QTime>
#include <QApplication>
#include "mainwin.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setOrganizationName("btbn.de");
	app.setOrganizationDomain("btbn.de");
	app.setApplicationName("TwitchWatcher");

	qsrand((uint)QTime::currentTime().msec());

	MainWin win;
	win.show();

	return app.exec();
}
