#include <gtest/gtest.h>

#include "Config.hpp"
#include "ConfigParser.hpp"

TEST(ConfigParser, ParseConfigFile) {
	{

		testing::internal::CaptureStderr();

		ConfigParser config_parser;
		std::string output;
		std::string file_path = "./config/duplicated_port.conf";
		config_parser.parseConfigFile(file_path);
		output = testing::internal::GetCapturedStderr();
		ASSERT_EQ("duplicated port\n", output);
	}
}
