#include "baseframe.h"

BaseFrame::BaseFrame(QWidget* parent, Qt::WindowFlags f)
	:QFrame(parent, f)
{
	op = new QGraphicsOpacityEffect(this);
}

void BaseFrame::SetOpacity(float opacity, bool is_enable)
{
	op->setOpacity(opacity);
	setGraphicsEffect(op);
	setEnabled(is_enable);
}
