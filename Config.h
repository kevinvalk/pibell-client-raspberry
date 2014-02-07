#pragma once

#define BOOST_FILESYSTEM_NO_DEPRECATED

#include "common.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/filesystem.hpp>

class Config
{
public:
	static ClientSettings getSettings(const char* filename);
	static std::string getAbsoluteAudioPath(std::string audio);
};
