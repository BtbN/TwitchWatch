#include <QtDebug>

#include <QApplication>
#include <QMessageBox>
#include <QTime>

#include "mainwin.h"


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	app.setOrganizationName("btbn.de");
	app.setOrganizationDomain("btbn.de");
	app.setApplicationName("TwitchWatcher");

	qsrand((uint)QTime::currentTime().msec());

	try
	{
		MainWin win;
		win.show();

		return app.exec();
	}
	catch(const std::exception &e)
	{
		QMessageBox::critical(nullptr, "Exception occured!", e.what());
	}

	return -1;
}
