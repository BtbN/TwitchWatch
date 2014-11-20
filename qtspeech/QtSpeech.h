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

#ifndef QtSpeech_H
#define QtSpeech_H

#include <QObject>

#if defined(QTSPEECH_STATIC)
# define QTSPEECH_API
#elif defined(QTSPEECH_LIBRARY)
# define QTSPEECH_API Q_DECL_EXPORT
#else
# define QTSPEECH_API Q_DECL_IMPORT
#endif

namespace QtSpeech_v1
{

class QTSPEECH_API QtSpeech : public QObject
{
	Q_OBJECT

	public:
	// exceptions
	struct Error
	{
		QString msg;
		Error(QString s): msg(s) {}
	};
	struct InitError : Error
	{
		InitError(QString s): Error(s) {}
	};
	struct LogicError : Error
	{
		LogicError(QString s): Error(s) {}
	};
	struct CloseError : Error
	{
		CloseError(QString s): Error(s) {}
	};

	// types
	struct VoiceName
	{
		QString id;
		QString name;
	};
	typedef QList<VoiceName> VoiceNames;

	// api
	QtSpeech(QObject* parent);
	QtSpeech(VoiceName n = VoiceName(), QObject* parent = nullptr);
	virtual ~QtSpeech();

	const VoiceName& name() const;  //!< Name of current voice
	static VoiceNames voices();     //!< List of available voices in system

	public slots:
	void say(const QString &msg);                                    //!< Say something, synchronous
	void tell(const QString &msg);                                   //!< Tell something, asynchronous
	void tell(const QString &msg, QObject* obj, const char* slot);   //!< Tell something, invoke slot at end

	signals:
	void finished();

	protected:
	virtual void timerEvent(QTimerEvent*);

	private:
	class Private;
	Private* d;
};

}

#endif // QtSpeech_H

