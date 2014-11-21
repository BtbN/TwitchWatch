#include <QTabWidget>

#include "mainwin.h"
#include "srlirc.h"
#include "twitchirc.h"
#include "streamwatch.h"

MainWin::MainWin(QWidget *parent)
	:QMainWindow(parent)
{
	resize(800, 600);

	srlirc = new SrlIrc(this);
	twitchirc = new TwitchIrc(this);
	streamwatch = new StreamWatch(this);

	QTabWidget *tabber = new QTabWidget(this);

	tabber->addTab(srlirc, "SRL IRC");
	tabber->addTab(twitchirc, "Twitch IRC");
	tabber->addTab(streamwatch, "Stream Watch");

	setCentralWidget(tabber);
}


MainWin::~MainWin()
{

}
