#include "Audio.h"

Audio::Audio()
{
	ao_initialize();
	memset(&format_, 0, sizeof(format_));
	format_.bits = 16;
	format_.channels = 2;
	format_.rate = 22050;
	format_.byte_format = AO_FMT_LITTLE;

	// Open the device
	device_ = ao_open_live(ao_default_driver_id(), &format_, NULL);
	if (device_ == NULL)
		throw 1;

	isPlaying_ = false;
}

Audio::~Audio()
{
	ao_close(device_);
	ao_shutdown();
}

/**
 * Sets the file to play
 * 
 * @param  file Absolute path to wav/raw/ao file
 * @return      Returns true if succeeded, false if previous file is getting played
 */
bool Audio::setFile(std::string file)
{
	// Only play once and do this non blocking
	if ( ! isPlaying_)
	{
		boost::interprocess::file_mapping fileMapping(file.c_str(), boost::interprocess::read_only);
		region_ = boost::interprocess::mapped_region(fileMapping, boost::interprocess::read_only);
		return true;
	}
	return false;
}

void Audio::onCall(bool global)
{
	play();
}

/**
 * Plays the audio
 * 
 * NOTE: This is non blocking
 * 
 * @return      false if already playing, true else
 */
bool Audio::play()
{
	// Only play once and do this non blocking
	if ( ! isPlaying_)
	{
		thread_ = std::thread(&Audio::play_, this);
		thread_.detach();
		return true;
	}
	return false;
}

/**
 * Plays the audio file
 *
 * Sets the audio file as the default audio and starts playing that file.
 * 
 * NOTE: This is non blocking
 * 
 * @param  file Absolute path to file
 * @return      false if already playing, true else
 */
bool Audio::play(std::string file)
{
	// Set the memory map
	if ( ! setFile(file))
		return false;

	// Play it
	return play();
}

/**
 * Play the currently set audio file
 *
 * NOTE: This function blocks
 * 
 */
void Audio::play_()
{
	isPlaying_ = true;

	// Play it
	ao_play(device_, (char*)region_.get_address(), region_.get_size());

	// Cygwin bug, ao_play returns 1.8s (or so) to soon
	#ifdef CYGWIN
		std::this_thread::sleep_for(std::chrono::milliseconds(1800));
	#endif

	// Avoid pops and clicks
	#ifdef RASPBERRY
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	#endif

	isPlaying_ = false;
}
