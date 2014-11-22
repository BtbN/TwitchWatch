#include <QActionGroup>
#include <QTabWidget>
#include <QMenuBar>
#include <QAction>
#include <QMenu>

#include "mainwin.h"
#include "srlirc.h"
#include "twitchirc.h"
#include "streamwatch.h"

MainWin::MainWin(QWidget *parent)
	:QMainWindow(parent)
{
	resize(800, 600);

	speech = new QtSpeech(this);

	srlirc = new SrlIrc(speech, this);
	twitchirc = new TwitchIrc(speech, this);
	streamwatch = new StreamWatch(speech, this);

	QTabWidget *tabber = new QTabWidget(this);

	tabber->addTab(srlirc, "SRL IRC");
	tabber->addTab(twitchirc, "Twitch IRC");
	tabber->addTab(streamwatch, "Stream Watch");

	setCentralWidget(tabber);

	QMenuBar *mbar = menuBar();

	QMenu *filemenu = mbar->addMenu("File");

	filemenu->addAction("Exit", this, SLOT(close()));

	QMenu *voicemenu = mbar->addMenu("Voices");

	QActionGroup *ag = new QActionGroup(voicemenu);
	ag->setExclusive(true);

	for(const QtSpeech::VoiceName &vname: speech->voices())
	{
		QAction *vaction = ag->addAction(vname.name);
		voicemenu->addAction(vaction);

		vaction->setCheckable(true);

		if(speech->name().id == vname.id)
			vaction->setChecked(true);

		connect(vaction, &QAction::triggered, this, [this, vname, vaction]()
		{
			speech->deleteLater();
			speech = new QtSpeech(vname, this);

			vaction->setChecked(true);
		});
	}
}


MainWin::~MainWin()
{

}
