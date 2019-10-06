#include "fileman.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

#include <sys/stat.h>
#include <sys/types.h>

#define os std::cout

FileMan::FileMan(const std::string& filename, const std::string& main){
	os << "FileMan starts" << std::endl;
	std::ifstream file(filename);
	for (std::string line; std::getline(file, line, ';');){
		std::istringstream is(line);
		std::string command;
		is >> command;
		main_dir = main;

		if ( command == "GEOM_FN" ){
			is >> geom_fn;
			std::cout << "GEOM_FN: " << geom_fn << std::endl;
		} else if ( command == "BFIELD_FN" ){
			while ( !is.eof() ){
				std::string bfield_fn;
				is >> bfield_fn;
				bfield_objs.insert(bfield_fn);
				os << "BFIELD_FN: " << bfield_fn << std::endl; 
			}
		} else if ( command == "CONF_FN" ){
			is >> conf_fn;
			os << "CONF_FN: " << conf_fn << std::endl;
		} else if ( command == "PATH" ) {
			is >> path;
			os << "PATH: " << path << std::endl;
		} else {
			os << "WARNING: unknown command (in fileman): " << command << std::endl;
		}
	}
}

void FileMan::set_output_path(const std::string& output){
	std::string dir_loc = "data/geometry=" + get_geom_fn() + "/" + output;
	std::string line;
	std::stringstream ss(dir_loc);
	std::string full_path_loc = get_path();
	std::istringstream is(line);
	while (std::getline(ss, line, '/')){
		full_path_loc = full_path_loc + "/" + line;
		mkdir(full_path_loc.c_str(), 0777);
	} 
	
	output_path = get_path() + "/data/geometry=" + get_geom_fn() + "/" + output;
}

std::string FileMan::get_main_dir() const { return main_dir; };
std::string FileMan::get_geom_fn() const { return geom_fn; };
std::string FileMan::get_conf_fn() const { return conf_fn; };
std::string FileMan::get_path() const { return path; };
std::string FileMan::get_output_path() const { return output_path; };
std::set<std::string> FileMan::get_bfield_objs() const { return bfield_objs; };