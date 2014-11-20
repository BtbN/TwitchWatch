#include <QtDebug>

#include <QApplication>
#include <QtSpeech>


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QtSpeech speech;

	for(QtSpeech::VoiceName name: speech.voices())
	{
		qDebug() << name.name;
	}

	speech.tell("TESTING THE PLAYBACK SOUND SYSTEM");
	qDebug() << speech.name().name;

	return app.exec();
}
