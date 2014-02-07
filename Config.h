#pragma once

#include "Common.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>

class Config
{
public:
	static ClientSettings getSettings(const char* filename);
	static std::string getAbsoluteAudioPath(std::string audio);
};
