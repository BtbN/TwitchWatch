#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

#include <QVBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QTimer>

#include "streamwatch.h"


StreamWatch::StreamWatch(QWidget *parent)
    :QWidget(parent)
{
	streamLine = new QLineEdit(this);
	textEdit = new QTextEdit(this);

	streamLine->setText("runnerguy2489");

	QVBoxLayout *lay = new QVBoxLayout(this);

	lay->addWidget(streamLine);
	lay->addWidget(textEdit);

	nam = new QNetworkAccessManager(this);

	QTimer *reauthTimer = new QTimer(this);
	reauthTimer->setInterval(30000);
	reauthTimer->setSingleShot(false);
	reauthTimer->start();

	retryTimer = new QTimer(this);
	retryTimer->setInterval(10000);
	retryTimer->setSingleShot(true);

	speech = new QtSpeech(this);

	connect(reauthTimer, SIGNAL(timeout()), this, SLOT(reauth()));
	connect(retryTimer, SIGNAL(timeout()), this, SLOT(reauth()));

	haveToken = false;
	wasDown = false;

	reauth();
}

void StreamWatch::reauth()
{
	QNetworkRequest req;

	QString url = QString("http://api.twitch.tv/api/channels/%1/access_token").arg(streamLine->text());
	req.setUrl(url);

	QNetworkReply *reply = nam->get(req);
	connect(reply, &QNetworkReply::finished, this, [this, reply]()
	{
		tokenReply(reply);
	});

	textEdit->setText("Reauthing:" + url);
}

void StreamWatch::tokenReply(QNetworkReply *reply)
{
	if(reply->error() != QNetworkReply::NoError)
	{
		textEdit->setText("token get error!");
		retryTimer->start();
		return;
	}

	int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	if(code >= 300 && code <= 310)
	{
		qDebug() << "Token redirected to" << reply->attribute(QNetworkRequest::RedirectionTargetAttribute);

		QNetworkRequest req(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString());
		QNetworkReply *reply = nam->get(req);
		connect(reply, &QNetworkReply::finished, this, [this, reply]()
		{
			tokenReply(reply);
		});
		return;
	}

	if(code != 200)
	{
		qDebug() << "Get token failed: Error" << code;
		retryTimer->start();
		return;
	}

	QByteArray data = reply->readAll();

	QJsonParseError err;
	QJsonDocument doc = QJsonDocument::fromJson(data, &err);

	if(err.error != QJsonParseError::NoError)
	{
		qDebug() << "Json parsing failed:" << err.errorString();
		retryTimer->start();
		return;
	}

	QString newToken = doc.object().value("token").toString();
	QString newSig = doc.object().value("sig").toString();

	if(!newToken.isEmpty() && !newSig.isEmpty())
	{
		token = newToken;
		sig = newSig;
		haveToken = true;
	}
	else
	{
		qDebug() << "New info is empty!";
		return;
	}

	QNetworkRequest req;

	QUrl url = QString("http://usher.twitch.tv/select/%1.json").arg(streamLine->text());
	QUrlQuery qry;
	qry.addQueryItem("nauthsig", sig);
	qry.addQueryItem("nauth", token);
	qry.addQueryItem("allow_source", "true");
	url.setQuery(qry);

	req.setUrl(url);

	reply = nam->get(req);

	connect(reply, &QNetworkReply::finished, this, [this, reply]()
	{
		usherReply(reply);
	});

	textEdit->setText("Requested usher:" + url.toString());
}

void StreamWatch::usherReply(QNetworkReply *reply)
{
	if(reply->error() != QNetworkReply::NoError)
	{
		textEdit->setText("usher get error!");
		retryTimer->start();
		return;
	}

	int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

	if(code >= 300 && code <= 310)
	{
		textEdit->setText(QString("Usher redirected to") + reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString());

		QNetworkRequest req(reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toString());
		QNetworkReply *reply = nam->get(req);
		connect(reply, &QNetworkReply::finished, this, [this, reply]()
		{
			usherReply(reply);
		});
		return;
	}

	if(code != 200)
	{
		textEdit->setText("Get usher failed: Error" + code);
		retryTimer->start();
		return;
	}

	QString m3u8 =  QString::fromUtf8(reply->readAll());

	if(m3u8.trimmed() == "[]")
	{
		textEdit->setText("m3u8 is empty, channel likely not live, retrying in 10 seconds");
		speech->tell("Twitch channel appears to be down!");
		retryTimer->start();
		wasDown = true;
		return;
	}

	QStringList lines = m3u8.split('\n');

	for(const QString &line: lines)
	{
		QString tline = line.trimmed();

		if(tline.startsWith("http"))
		{
			textEdit->setText(m3u8);

			if(wasDown)
			{
				wasDown = false;
				speech->tell("Twitch channel is online");
			}

			return;
		}
	}

	textEdit->setText("no channel found in m3u8, retrying in 10 seconds");
	retryTimer->start();
}
