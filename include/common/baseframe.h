#pragma once
#include "qframe.h"
#include "qgraphicseffect.h"

class BaseFrame: public QFrame
{
public:
	explicit BaseFrame(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
	void SetOpacity(float opacity, bool is_enable);

private:
	QGraphicsOpacityEffect* op;
};

