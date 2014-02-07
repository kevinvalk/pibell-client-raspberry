#include "Action.h"

std::function<void(bool)> Action::getOnCall()
{
	return std::bind(&Action::onCall, this, std::placeholders::_1);
}