#include "confbldr.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

#define os std::cout

ConfBldr::ConfBldr(const FileMan& fm){
	os << "ConfBldr starts" << std::endl;
	std::string filename(fm.get_main_dir() + "/" + fm.get_conf_fn() + ".txt");
	std::ifstream file(filename);
	for (std::string line; std::getline(file, line, ';');){
		std::istringstream is(line);
		std::string command;
		is >> command;
		if (command == "AMOUNT_OF_PARTICLES"){
			is >> N;
			os << "AMOUNT_OF_PARTICLES: " << N << std::endl;
		} else if ( command == "PLASMA_POT" ){
			is >> Vplasma;
			os << "PLASMA_POT: " << Vplasma << std::endl;
		} else if ( command == "INITIAL_ENERGY" ){
			is >> Energy;
			os << "INITIAL_ENERGY: " << Energy << std::endl;
		} else if ( command == "ION_TEMP" ){
			is >> Ti;
			os << "ION_TEMP: " << Ti << std::endl;
		} else if ( command == "ELECTRON_TEMP" ){
			is >> Te;
			os << "ELECTRON_TEMP: " << Te << std::endl;
		} else if ( command == "N_CYCLES_MAX" ){
			is >> Ncycles;
			os << "N_CYCLES_MAX: " << Ncycles << std::endl;
		} else {
			os << "WARNING: unknown command (ConfBldr): " << command << std::endl;
		}
	}
	file.close();
}