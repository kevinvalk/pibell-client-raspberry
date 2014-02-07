#pragma once

#include "../Common.h"
#include "../Action.h"
#include <ao/ao.h>

class Audio : public Action
{
public:
	Audio();
	~Audio();

	bool play();
	bool play(std::string file);

	bool setFile(std::string file);

	void onCall(bool global);

private:
	void play_();


	ao_device *device_;
	ao_sample_format format_;

	boost::interprocess::mapped_region region_;

	std::thread thread_;
	bool isPlaying_;
};
