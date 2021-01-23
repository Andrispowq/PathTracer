#ifndef PREHISTORIC_APP_H
#define PREHISTORIC_APP_H

#include "Prehistoric.h"

class TesterApp : public Prehistoric::Application
{
public:
	TesterApp();
	~TesterApp();
};

Prehistoric::Application* Prehistoric::CreateApplication()
{
	return new TesterApp();
}

#endif