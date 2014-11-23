#pragma once

#include <QWidget>
#include <QtSpeech>

class QCheckBox;
class IrcConnection;
class QListWidget;
class IrcPrivateMessage;


class TwitchIrc : public QWidget
{
	Q_OBJECT

	public:
	TwitchIrc(QtSpeech*& speech, QWidget *parent = 0);

	private slots:
	void connectIrc(const QString &username, const QString &token);
	void postMsg(const QString &msg, bool read = false);
	void onConnecting();
	void onConnected();
	void onDisconnected();
	void gotMsg(IrcPrivateMessage *msg);
	void nomsg();
	void checkConn();

	private:
	IrcConnection *conn;
	QListWidget *lw;
	QtSpeech*& speech;
	QTimer *msgtimer;
	QCheckBox *shutUpBox;
	bool haveGotMsg;
};
