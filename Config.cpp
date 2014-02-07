#include "Config.h"

ClientSettings Config::getSettings(const char* filename)
{
	boost::property_tree::ptree pt;
	boost::property_tree::ini_parser::read_ini(filename, pt);

	ClientSettings settings;
	settings.hostname = pt.get<std::string>("settings.hostname", std::string());
	settings.port = pt.get("settings.port", std::string());
	settings.username = pt.get("settings.username", std::string());
	settings.password = pt.get("settings.password", std::string());
	settings.ringtone = Config::getAbsoluteAudioPath(pt.get("settings.ringtone", "audio/bigdrum.wav"));

	return settings;
}

std::string Config::getAbsoluteAudioPath(std::string audio)
{
	boost::filesystem::path audioPath(audio);
	if ( ! audioPath.is_absolute())
		return boost::filesystem::absolute(audioPath).generic_string();
	else
		return audio;
}