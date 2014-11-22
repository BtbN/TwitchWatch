#pragma once


#include <QWidget>
#include <QtSpeech>

class IrcConnection;
class QListWidget;
class IrcPrivateMessage;
class QTimer;

class SrlIrc : public QWidget
{
	Q_OBJECT

	public:
	SrlIrc(QtSpeech*& speech, QWidget *parent = 0);

	private slots:
	void newConn();
	void postMsg(const QString &msg, bool read = false);
	void onConnecting();
	void onConnected();
	void onDisconnected();
	void gotMsg(IrcPrivateMessage *msg);
	void checkConn();

	private:
	IrcConnection *conn;
	QListWidget *lw;
	QtSpeech*& speech;
};
