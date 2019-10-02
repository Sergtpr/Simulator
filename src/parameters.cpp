#include "parameters.hpp"
#include <iostream>
#include <sstream>
#include <fstream>


Parameters::Parameters(const FileMan& fm){
	std::string filename(fm.get_main_dir() + "/parameters.txt");
	std::ifstream file(filename);
	Jtotal = 0;
	for (std::string line; std::getline(file, line, ';');){
		std::istringstream is(line);
		std::string command;
		is >> command;
		if (command == "SOLID_POT"){
			std::string solid_name;
			double solid_pot;
			while( !is.eof() ){
				is >> solid_name >> solid_pot;
				V[solid_name] = solid_pot;
				std::cout << solid_name << " " << solid_pot << std::endl;
			}
		} else if ( command == "BF_OBJ_SC"){
			std::string bf_name;
			double scale;
			while ( !is.eof() ){
				is >> bf_name >> scale;
				bf_scale[bf_name] = scale;
			}
		} else if ( command == "SCC" ){
			is >> scc;
		} else if ( command == "BEAM_Q_M_J" ){
			while ( !is.eof() ){
				double q, m, J;
			is >> q >> m >> J;
			Jtotal += J;
			beam.push_back({q, m, J});
			}
		} else {
			std::cout << "Warning: unknown command" << " (Parameters)" << std::endl;
		}

	}
	
	if (V.count("puller") == 0){
		std::cout << "Set default V_gnd = -60e3" << std::endl;
		V["gnd"] = -60e3;
	} else {
		V["gnd"] = V["puller"];
	}
	std::cout << "Jtotal: " << Jtotal << std::endl; 
	
	file.close();
}