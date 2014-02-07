#include "Glow.h"

Glow::Glow(int totalTime)
{
	piGlowSetup(true);
	noGlow_ = isGlowing_ = false;
	totalTime_ = totalTime;
}

Glow::~Glow()
{
	noGlow_ = true;
	fullGlow(0);
}

bool Glow::notify(bool global)
{
	// Only play once and do this non blocking
	if ( ! isGlowing_)
	{
		thread_ = std::thread(&Glow::notify_, this, global);
		thread_.detach();
		return true;
	}
	return false;
}

void Glow::notify_(bool global)
{
	isGlowing_ = true;

	// Do pattern
	int ms = global ? 610 : 255;
	std::chrono::milliseconds speed(ms);

	for(int i = 0; i < std::round(totalTime_ / 2 / ms); i++)
	{
		fadeIn(speed);
		fadeOut(speed);
	}

	isGlowing_ = false;
}


void Glow::onCall(bool global)
{
	notify(global);
}

// Private functions
void Glow::fullGlow(int percentage)
{
	if (noGlow_)
		return;

	for (int ring = 0; ring < 6; ++ring)
	{
		piGlowRing(ring, percentage);
	}

	if (noGlow_)
	{
		for (int ring = 0; ring < 6; ++ring)
		{
			piGlowRing(ring, 0);
		}
	}
}

/**
 * Fades the ring in a given direction in the given ms
 *
 * Because of the 255 steps you will lose precision the smaller ms you go.
 * I recommend to not go lower the 100ms.
 * 
 * @param ms     Amount of time for a fadein
 * @param fadeIn direction of the fade
 */
void Glow::fade(std::chrono::milliseconds ms, bool fadeIn)
{
	int stepSize = std::round(GLOW_MS_PER_PASS / ms.count());
	int glow = fadeIn ? 0 : 255;
	if (fadeIn)
		for (int glow = 0; glow < 255; glow += stepSize)
			fullGlow(glow);
	else
		for (int glow = 255; glow > 0; glow -= stepSize)
			fullGlow(glow);
}

void Glow::fadeIn(std::chrono::milliseconds ms)
{
	fade(ms, true);
	fullGlow(255);
}

void Glow::fadeOut(std::chrono::milliseconds ms)
{
	fade(ms, false);
	fullGlow(0);
}
