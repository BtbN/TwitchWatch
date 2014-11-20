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

	speech.say("TESTING THE PLAYBACK SOUND SYSTEM");
	speech.tell("TESTING THE PLAYBACK SOUND SYSTEM");

	return 0;
	return app.exec();
}
