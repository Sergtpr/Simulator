#include "confbldr.hpp"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>


ConfBldr::ConfBldr(const FileMan& fm){
	std::string filename(fm.get_main_dir() + "/" + fm.get_conf_fn() + ".txt");
	std::ifstream file(filename);
	for (std::string line; std::getline(file, line, ';');){
		std::istringstream is(line);
		std::string command;
		is >> command;
		if (command == "AMOUNT_OF_PARTICLES"){
			is >> N;
			std::cout << command << " " << N << std::endl;
		} else if ( command == "PLASMA_POT" ){
			is >> Vplasma;
			std::cout << command << " " << Vplasma << std::endl;
		} else if ( command == "INITIAL_ENERGY" ){
			is >> Energy;
			std::cout << command << " " << Energy << std::endl;
		} else if ( command == "ION_TEMP" ){
			is >> Ti;
			std::cout << command << " " << Ti << std::endl;
		} else if ( command == "ELECTRON_TEMP" ){
			is >> Te;
			std::cout << command << " " << Te << std::endl;
		} else if ( command == "N_CYCLES_MAX" ){
			is >> Ncycles;
			std::cout << command << " " << Ncycles << std::endl;
		} else {
			std::cout << "Warning: unknown command (ConfBldr)" << std::endl;
		}
	}
	file.close();
}