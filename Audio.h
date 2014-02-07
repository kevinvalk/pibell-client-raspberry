#pragma once

#include "Common.h"
#include <ao/ao.h>

class Audio
{
public:
	Audio();
	~Audio();

	bool play();
	bool play(std::string file);

	bool setFile(std::string file);

private:
	void play_();


	ao_device *device_;
	ao_sample_format format_;

	boost::interprocess::file_mapping file_;
	boost::interprocess::mapped_region region_;

	std::thread thread_;
	bool isPlaying_;
};
