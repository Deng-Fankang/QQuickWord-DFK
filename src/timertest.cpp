#include "timertest.h"
#include "qdebug.h"
#include "qdatetime.h"

TimerTest::TimerTest(QObject* parent):
	QThread(parent)
{
	timer_ = new QElapsedTimer();
}

TimerTest::~TimerTest()
{
	delete timer_;
}

void TimerTest::run()
{
	timer_->start();
	int i = 1;
	quint64 m = QDateTime::currentMSecsSinceEpoch();
	while (true)
	{
		if (timer_->nsecsElapsed() >= 56.5 * i * 1000000) {
			quint64 n = QDateTime::currentMSecsSinceEpoch();
			qDebug() << "elapsed time ms:" << i << ',' << n - m << ',' << timer_->nsecsElapsed() / 1000000.0 << endl;
			//timer_->restart();
			i++;
			if (QThread::currentThread()->isInterruptionRequested()) {
				break;
			}
			m = n;
		}
	}
}
