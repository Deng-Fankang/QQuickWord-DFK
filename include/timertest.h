#pragma once
#include "qelapsedtimer.h"
#include "qthread.h"

class TimerTest: public QThread
{
public:
	TimerTest(QObject* parent=Q_NULLPTR);
	~TimerTest();

protected:
	void run();

private:
	QElapsedTimer* timer_;
	quint64 elapsed_time_;
};
