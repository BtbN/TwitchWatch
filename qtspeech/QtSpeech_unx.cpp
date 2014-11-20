/*  QtSpeech -- a small cross-platform library to use TTS
    Copyright (C) 2010-2011 LynxLine.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 3 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General
    Public License along with this library; if not, write to the
    Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301 USA */

#include <QtCore>
#include <QProcess>
#include <QtSpeech.h>

namespace QtSpeech_v1
{

// some defines for throwing exceptions
#define Where QString("%1:%2:").arg(__FILE__).arg(__LINE__)

// internal data
class QtSpeech::Private
{
	public:
	VoiceName name;
	static const QString VoiceId;
};

const QString QtSpeech::Private::VoiceId = QString("festival:%1");

// implementation
QtSpeech::QtSpeech(QObject* parent)
	: QObject(parent), d(new Private)
{
	VoiceName n = {Private::VoiceId.arg("english"), "English"};

	if (n.id.isEmpty())
	{
		throw InitError(Where + "No default voice in system");
	}

	d->name = n;
}

QtSpeech::QtSpeech(VoiceName n, QObject* parent)
	: QObject(parent), d(new Private)
{
	if (n.id.isEmpty())
	{
		VoiceName def = {Private::VoiceId.arg("english"), "English"};
		n = def;
	}

	if (n.id.isEmpty())
	{
		throw InitError(Where + "No default voice in system");
	}

	d->name = n;
}

QtSpeech::~QtSpeech()
{
	//if ()
	delete d;
}

const QtSpeech::VoiceName& QtSpeech::name() const
{
	return d->name;
}

QtSpeech::VoiceNames QtSpeech::voices()
{
	VoiceNames vs;
	VoiceName n = {Private::VoiceId.arg("english"), "English"};
	vs << n;
	return vs;
}

void QtSpeech::tell(const QString &text)
{
	tell(text, nullptr, nullptr);
}

void QtSpeech::tell(const QString &text, QObject* obj, const char* slot)
{
	QProcess *proc = new QProcess(this);

	QString prog = "festival";
	QStringList args = {"--tts"};

	proc->start(prog, args);

	connect(proc, &QProcess::started, this, [proc, text]() {
		proc->write(text.toUtf8());
		proc->closeWriteChannel();
	});

	if(obj && slot)
		connect(proc, SIGNAL(finished(int)), obj, slot);

	connect(proc, SIGNAL(finished(int)), proc, SLOT(deleteLater()));
}

void QtSpeech::say(const QString &text)
{
	QEventLoop el;
	tell(text, &el, SLOT(quit()));
	el.exec();
}

void QtSpeech::timerEvent(QTimerEvent* te)
{
	QObject::timerEvent(te);
}

} // namespace QtSpeech_v1
