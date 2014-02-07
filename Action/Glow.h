#include "../Common.h"
#include "../Action.h"
#include <wiringPi.h>
#include <piGlow.h>

#define GLOW_MS_PER_PASS 3067

class Glow : public Action
{
public:
	Glow(int totalTime);
	~Glow();

	void onCall(bool global);
	bool notify(bool global);

private:
	void notify_(bool global);

	void fullGlow(int percentage);
	void fade(std::chrono::milliseconds ms, bool fadeIn);
	void fadeIn(std::chrono::milliseconds ms);
	void fadeOut(std::chrono::milliseconds ms);

	std::thread thread_;
	bool isGlowing_;
	bool noGlow_;
	int totalTime_;
};
