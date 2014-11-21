#include <QListWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QSettings>

#include <Irc>
#include <IrcConnection>
#include <IrcMessage>
#include <IrcCommand>

#include "twitchirc.h"


TwitchIrc::TwitchIrc(QWidget *parent)
	:QWidget(parent)
{
	speech = new QtSpeech(this);

	conn = nullptr;

	lw = new QListWidget(this);

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

	if(read)
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
	postMsg(QString("Connected!"));
	conn->sendCommand(IrcCommand::createJoin("#runnerguy2489"));
}

void TwitchIrc::onDisconnected()
{
	postMsg("Disconnected from Twitch IRC!", true);
}

void TwitchIrc::gotMsg(IrcPrivateMessage *msg)
{
	if(msg->isPrivate())
		return;

	QString str = QString("%1 says: %2").arg(msg->nick()).arg(msg->content());

	postMsg(str);
}
