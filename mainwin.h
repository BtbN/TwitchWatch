#pragma once

#include <QMainWindow>

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
	SrlIrc *srlirc;
	TwitchIrc *twitchirc;
	StreamWatch *streamwatch;
};
