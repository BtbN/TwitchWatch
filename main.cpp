#include <QApplication>
#include <QtSpeech>


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	QtSpeech speech;
	speech.tell("TESTING");

	return app.exec();
}
