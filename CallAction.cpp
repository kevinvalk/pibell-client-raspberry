#include "CallAction.h"

std::function<void(bool)> CallAction::getOnCall()
{
	return std::bind(&CallAction::onCall, this, std::placeholders::_1);
}