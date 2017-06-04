#ifndef CONFIG_FILE_READER_HPP
#define CONFIG_FILE_READER_HPP

#include <fstream>
#include <sstream>

#include "common.hpp"
#include "RayBox.hpp"

namespace RayBox {

	/**
	 * @brief      Class for configuration reader.
	 * 			   Provides
	 * 			   1) Simple file reader with binding call on every line read
	 * 			   2) Configuration reading function
	 * 			   3) Input data file reading function
	 * 			   
	 */	
	class ConfigReader {
	public:
		ConfigReader()				= default;
		~ConfigReader()				= default;


		/**
		 * @brief      { Simple file reader with binding call on every line read }
		 *
		 * @param[in]  fileName    The file name
		 * @param[in]  readerFunc  The reader function
		 */
		static void fileReader(const std::string& fileName, std::function<void(std::string&)> readerFunc) {
			try {
				std::fstream configFile(fileName.c_str());
				if (configFile.is_open()) {
					std::string line;
					while (std::getline(configFile, line)) {
						if (line[0] == '#' || line.length() == 0)
							continue;
						readerFunc(line);
					}
				}
				configFile.close();
			}
			catch (std::exception& ex) {
				std::cout << "error while reading " << fileName.c_str() << ": " 
					<< ex.what() << std::endl;
			}
		}

		/**
		 * @brief     { Configuration reading function }
		 *
		 * @param[in]  line    The config file line
		 * 						line format: 1st line: Side of Sqaure (RayBox)
		 * 									 2nd line onwards: RowNumber ColNumber Strength (Mirror Details) 
		 * @param[ref] rayBox  Instance of raybox
		 */
		static void parseConfigFile(const std::string& line, std::shared_ptr<RayBox::Raybox>& rayBox) throw(std::logic_error) {
			static int lineNo = 0;

			if (0 == lineNo) {
				int capacity = std::stoi(line);
				if (capacity < 1)
					throw std::logic_error("invalid input column size");
				rayBox = std::make_shared<Raybox>(capacity);
			}

			if (lineNo > 0) {
				std::istringstream iss = std::istringstream(line);
				std::string row; std::string column; std::string strength;
				iss >> row;
				iss >> column;

				int r = std::stoi(row) - 1;
				if (r < 0 || r > rayBox->getSize())
					std::logic_error("Invalid row in ray Input file");

				int c = std::stoi(column) - 1;
				if (c < 0 || c > rayBox->getSize())
					std::logic_error("Invalid column in ray Input file");

				std::shared_ptr<Mirror> mirror;
				if (iss >> strength) {
					int s = std::stoi(strength);
					if (s < 0)
						std::logic_error("Invalid strength in ray Input file");
					mirror = std::make_shared<Mirror>(r, c, s);
				}
				else
					mirror = std::make_shared<Mirror>(r, c);

				rayBox->AddMirror(mirror);
			}

			lineNo++;
		}

		/**
		 * @brief      { Input data file reading function }
		 *
		 * @param[in]  line    Data line mentioning direction of ray and co-ordinates of entering
		 * 						line format: C/R Number +/-
		 * @param[ref] rayBox  Instance of raybox
		 * 
		 * 
		 */
		static void parseRayInputFile(const std::string& line, std::shared_ptr<RayBox::Raybox>& rayBox) throw(std::logic_error) {
			static int lineNo = 0;

			switch (line[0])
			{
			case 'C': {
				Ray ray;
				ray._row = 0;
				if (line[line.length() - 1] == '-') {
					ray._direction = Ray::Direction::BottomToTop;
					ray._row = rayBox->getSize() - 1;
				}
				else if (line[line.length() - 1] == '+')
					ray._direction = Ray::Direction::TopToBottom;
				else
					throw std::logic_error("Invalid input direction ");
				ray._column = atoi(line.substr(1, line.length() - 2).c_str()) - 1;
				if (ray._column > rayBox->getSize())
					throw std::logic_error("Invalid Input");
				std::cout << line.c_str() << " -> ";
				rayBox->PassTheRay(ray);
			}
			break;
			case 'R': {
				Ray ray;
				ray._column = 0;
				if (line[line.length() - 1] == '-') {
					ray._direction = Ray::Direction::RightToLeft;
					ray._column = rayBox->getSize() - 1;
				}
				else if (line[line.length() - 1] == '+')
					ray._direction = Ray::Direction::LeftToRight;
				else
					throw std::logic_error("Invalid Input");
				ray._row = atoi(line.substr(1, line.length() - 2).c_str()) - 1;
				if (ray._row > rayBox->getSize())
					throw std::logic_error("Invalid Input");
				std::cout << line.c_str() << " -> ";
				rayBox->PassTheRay(ray);
			}
			break;
			}
			lineNo++;
		}
	};
}

#endif //CONFIG_FILE_READER_HPP
