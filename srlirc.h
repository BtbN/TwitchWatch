#pragma once


#include <QWidget>
#include <QtSpeech>

class IrcConnection;
class QListWidget;
class IrcPrivateMessage;

class SrlIrc : public QWidget
{
	Q_OBJECT

	public:
	SrlIrc(QWidget *parent = 0);

	private slots:
	void postMsg(const QString &msg, bool read = false);
	void onConnecting();
	void onConnected();
	void onDisconnected();
	void gotMsg(IrcPrivateMessage *msg);

	private:
	IrcConnection *conn;
	QListWidget *lw;
	QtSpeech *speech;
};
