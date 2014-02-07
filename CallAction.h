#pragma once

#include "Common.h"

class CallAction
{
public:
	std::function<void(bool)> getOnCall();
	virtual void onCall(bool global)  = 0;
};