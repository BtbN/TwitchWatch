#include <QListWidget>
#include <QVBoxLayout>
#include <QTimer>

#include <Irc>
#include <IrcConnection>
#include <IrcMessage>
#include <IrcCommand>

#include "srlirc.h"



SrlIrc::SrlIrc(QWidget *parent)
	:QWidget(parent)
{
	speech = new QtSpeech(this);

	conn = new IrcConnection("irc.speedrunslive.com", this);
	conn->setUserName("watcher");
	conn->setRealName("The Watcher");
	conn->setNickName(QString("Watcher%1").arg(qrand() % 9999));

	connect(conn, SIGNAL(connected()), this, SLOT(onConnected()));
	connect(conn, SIGNAL(connecting()), this, SLOT(onConnecting()));
	connect(conn, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

	connect(conn, SIGNAL(privateMessageReceived(IrcPrivateMessage*)), this, SLOT(gotMsg(IrcPrivateMessage*)));

	conn->open();

	lw = new QListWidget(this);

	QVBoxLayout *lay = new QVBoxLayout(this);
	lay->addWidget(lw);
}

void SrlIrc::postMsg(const QString &msg, bool read)
{
	lw->addItem(msg);

	while(lw->count() > 20)
		delete lw->takeItem(0);

	if(read)
	{
		speech->tell(msg);
	}
}

void SrlIrc::onConnecting()
{
	postMsg(QString("Connecting to irc.speedrunslive.com as %1...").arg(conn->nickName()));
}

void SrlIrc::onConnected()
{
	postMsg("Connected to SRL IRC!", true);
	conn->sendCommand(IrcCommand::createJoin("#watch"));
}

void SrlIrc::onDisconnected()
{
	postMsg("Disconnected from SRL IRC!", true);
	QTimer::singleShot(10000, conn, SLOT(open()));
}

void SrlIrc::gotMsg(IrcPrivateMessage *msg)
{
	if(msg->isPrivate())
		return;

	QString str = QString("%1 says: %2").arg(msg->nick()).arg(msg->content());

	postMsg(str, true);
}
