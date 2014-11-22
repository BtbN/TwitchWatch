#pragma once

#include <QMainWindow>
#include <QtSpeech>

class SrlIrc;
class TwitchIrc;
class StreamWatch;


class MainWin : public QMainWindow
{
	Q_OBJECT

	public:
	MainWin(QWidget *parent = 0);
	~MainWin();

	private:
	QtSpeech *speech;

	SrlIrc *srlirc;
	TwitchIrc *twitchirc;
	StreamWatch *streamwatch;
};
