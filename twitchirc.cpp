#include <QListWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSettings>
#include <QCheckBox>
#include <QTimer>

#include <Irc>
#include <IrcConnection>
#include <IrcMessage>
#include <IrcCommand>

#include "twitchirc.h"


TwitchIrc::TwitchIrc(QtSpeech*& speech, QWidget *parent)
	:QWidget(parent)
	,speech(speech)
	,conn(nullptr)
{
	lw = new QListWidget(this);
	shutUpBox = new QCheckBox(this);

	shutUpBox->setText("Shut up");

	QLineEdit *ue = new QLineEdit(this);
	QLineEdit *le = new QLineEdit(this);

	ue->setText("runnerguy2489");
	le->setText("http://www.twitchapps.com/tmi/ token here");

	connect(ue, &QLineEdit::returnPressed, this, [this, le, ue]()
	{
		QSettings sett;
		sett.setValue("twuname", ue->text());

		connectIrc(ue->text(), sett.value("tmi", le->text()).toString());
	});

	connect(le, &QLineEdit::returnPressed, this, [this, le, ue]()
	{
		QString txt = le->text();
		le->setText("hidden");

		QSettings sett;
		sett.setValue("tmi", txt);

		connectIrc(ue->text(), txt);
	});

	QSettings sett;

	if(sett.contains("twuname"))
		ue->setText(sett.value("twuname").toString());

	if(sett.contains("tmi"))
	{
		connectIrc(ue->text(), sett.value("tmi").toString());
		le->setText("hidden");
	}

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addWidget(ue);
	lay->addWidget(le);
	lay->addWidget(lw);
	lay->addWidget(shutUpBox);

	msgtimer = new QTimer(this);
	msgtimer->setSingleShot(true);
	msgtimer->setInterval(120000);

	QTimer *checkTimer = new QTimer(this);
	checkTimer->setSingleShot(false);
	checkTimer->setInterval(10000);
	checkTimer->start();

	connect(checkTimer, SIGNAL(timeout()), this, SLOT(checkConn()));

	connect(msgtimer, SIGNAL(timeout()), this, SLOT(nomsg()));
}

void TwitchIrc::connectIrc(const QString &username, const QString &token)
{
	if(conn)
		delete conn;

	conn = new IrcConnection("irc.twitch.tv", this);

	conn->setUserName(username);
	conn->setRealName(username);
	conn->setNickName(username);
	conn->setPassword(token);

	connect(conn, SIGNAL(connected()), this, SLOT(onConnected()));
	connect(conn, SIGNAL(connecting()), this, SLOT(onConnecting()));
	connect(conn, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

	connect(conn, SIGNAL(privateMessageReceived(IrcPrivateMessage*)), this, SLOT(gotMsg(IrcPrivateMessage*)));

	conn->open();
}

void TwitchIrc::postMsg(const QString &msg, bool read)
{
	lw->addItem(msg);

	while(lw->count() > 20)
		delete lw->takeItem(0);

	if(read && !shutUpBox->isChecked())
	{
		speech->tell(msg);
	}
}

void TwitchIrc::onConnecting()
{
	postMsg(QString("Connecting to irc.twitch.tv..."));
}

void TwitchIrc::onConnected()
{
	postMsg("Connected to Twitch IRC!", true);
	conn->sendCommand(IrcCommand::createJoin("#runnerguy2489"));

	msgtimer->start();
}

void TwitchIrc::onDisconnected()
{
	postMsg("Disconnected from Twitch IRC!", true);
}

void TwitchIrc::gotMsg(IrcPrivateMessage *msg)
{
	if(msg->isPrivate())
		return;

	msgtimer->start();

	QString str = QString("%1 says: %2").arg(msg->nick()).arg(msg->content());

	postMsg(str);
}

void TwitchIrc::nomsg()
{
	postMsg("No Twitch chat message in 2 minutes!", true);
}

void TwitchIrc::checkConn()
{
	if(!conn)
		return;

	if(conn->isConnected())
		return;

	conn->open();
}
