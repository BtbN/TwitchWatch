#pragma once


#include <QWidget>
#include <QtSpeech>

class QNetworkAccessManager;
class QNetworkReply;
class QTextEdit;
class QLineEdit;
class QCheckBox;
class QTimer;

class StreamWatch : public QWidget
{
	Q_OBJECT

	public:
	StreamWatch(QtSpeech*& speech, QWidget *parent = 0);

	private slots:
	void reauth();
	void tokenReply(QNetworkReply *reply);
	void usherReply(QNetworkReply *reply);

	void randomError();
	void isOn();
	void isOff();

	private:
	QNetworkAccessManager *nam;
	QtSpeech*& speech;
	QTimer *retryTimer;
	QString token, sig;
	bool haveToken;
	QTextEdit *textEdit;
	QLineEdit *streamLine;
	QCheckBox *shutUpBox;
	bool wasDown;
};
