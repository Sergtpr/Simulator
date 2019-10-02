#include "fileman.hpp"
#include <iostream>
#include <sstream>
#include <fstream>


FileMan::FileMan(const std::string& filename, const std::string& main){
	std::ifstream file(filename);
	for (std::string line; std::getline(file, line, ';');){
		std::istringstream is(line);
		std::string command;
		is >> command;
		main_dir = main;
		if ( command == "GEOM_FN" ){
			is >> geom_fn;
		} else if ( command == "BFIELD_FN" ){
			while ( !is.eof() ){
				std::string bfield_fn;
				is >> bfield_fn;
				bfield_objs.insert(bfield_fn);
			}
		} else if ( command == "CONF_FN" ){
			is >> conf_fn;
		} else {
			std::cout << "Warning: unknown command (fileman) " << std::endl;
		}
	}
}

std::string FileMan::get_main_dir() const { return main_dir; };
std::string FileMan::get_geom_fn() const { return geom_fn; };
std::string FileMan::get_conf_fn() const { return conf_fn; };
std::set<std::string> FileMan::get_bfield_objs() const { return bfield_objs; };