// ReyBox.cpp : Defines the entry point for the console application.
//

#include <functional>

#include "ConfigFileReader.hpp"
#include "common.hpp"
#include "RayBox.hpp"
using namespace RayBox;



int main(int argc, char** argv)
{
	if (argc != 3) {
		std::cout << "Usage: <RayBox> <ConfigFileName> <RayInputFile>" << std::endl;
		return 1;
	}

	std::shared_ptr<Raybox> rayBox;
	std::string config(argv[1]);

	///Reading config file
	try
	{
		ConfigReader::fileReader(config,
			std::bind(ConfigReader::parseConfigFile, std::placeholders::_1, std::ref(rayBox)));
	}
	catch (const std::exception& ex)
	{
		std::cout << "Error parseConfigFile : " << ex.what() << std::endl;
		return 1;
	}
	
	if (rayBox.get() == nullptr)
	{
		std::cout << "Raybox initialization failed" << std::endl;
		return 1;
	}

	/// Covering book keeping information, which helps in reducing processing time
	rayBox->initReferences();

	//// Reading data file with ray direction and co-ordinates on each line
	std::string rayInputFile(argv[2]);
	try {
		TIMER_START(Total);
		ConfigReader::fileReader(rayInputFile,
			std::bind(ConfigReader::parseRayInputFile, std::placeholders::_1, std::ref(rayBox)));
		TIMER_STOP(Total)
	}
	catch (std::exception& ex) {
		std::cout << "Error parseConfigFile : " << ex.what() << std::endl;
		return 1;
	}

	//getchar();

    return 0;
}

