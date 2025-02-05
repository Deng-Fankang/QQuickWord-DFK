#pragma once
#include "qwidget.h"
#include "baseframe.h"

class BaseModule
{
public:
	BaseModule() :w(nullptr) {};
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual BaseFrame* ShowWidget(QWidget* parent = nullptr) = 0;
	BaseFrame* GetWidget() { return w; };
protected:
	BaseFrame* w;
};
