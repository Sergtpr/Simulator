#include "parameters.hpp"
#include <iostream>
#include <sstream>
#include <fstream>

#define os std::cout

bool is_out(std::istringstream& is){
	std::string out;
	is >> out;
	return out == "OUT";
}

Parameters::Parameters(const FileMan& fm){
	os << "Parameters start" << std::endl;
	std::string filename(fm.get_path() + "/parameters.txt");
	std::ifstream file(filename);
	Jtotal = 0;
	for (std::string line; std::getline(file, line, ';');){
		std::istringstream is(line);
		std::string command;
		is >> command;
		if (command == "SOLID_POT"){
			std::string solid_name;
			double solid_pot;
			is >> solid_name >> solid_pot;
			V[solid_name] = solid_pot;
			os << "SOLID_POT: " << solid_name << " " << solid_pot;
			V_out[solid_name] = is_out(is);
			if (V_out.at(solid_name)){
				os << " OUT";
			}
			os << std::endl;			
		} else if ( command == "BF_OBJ_SC"){
			std::string bf_name;
			double scale;
			is >> bf_name >> scale;
			bf_scale[bf_name] = scale;
			os << "BF_OBJ_SC: " << bf_name << scale << std::endl;
			bf_scale_out[bf_name] = is_out(is);
		} else if ( command == "SCC" ){
			is >> scc;
			os << "SCC: " << scc;
			scc_out = is_out(is);
			if (scc_out){
				os << " OUT";
			}
			os << std::endl;
		} else if ( command == "BEAM_Q_M_J" ){
			double q, m, J;
			is >> q >> m >> J;
			Jtotal += J;
			beam.push_back({q, m, J});
			os << "BEAM_Q_M_J: " << q << " " << m << " " << J << std::endl;
			beam_out.push_back(is_out(is));
		} else {
			std::cout << "Warning: unknown command" << " (Parameters)" << std::endl;
		}

	}
	
	std::cout << "Jtotal: " << Jtotal << std::endl; 
	
	file.close();
}

std::string Parameters::get_output_name(){
	std::string result;	
	
	for (auto item : V){
		if (V_out.at(item.first) == true){
			std::stringstream ss;
			std::string buf;
			ss << item.first << "=" << item.second << ",";
			ss >> buf;
			result += buf;
		}
	}
	if (!bf_scale.empty()){
		result += "/";
	}
	for (const auto& item : bf_scale){
		if (bf_scale_out.at(item.first) == true){
			std::stringstream ss;
			std::string buf;
			ss << item.first << "=" << item.second << ",";
			ss >> buf;
			result += buf;
		}
	}
	if (!beam.empty()){
		result += "/";
	}
	for (size_t i = 0; i < beam.size(); i++){
		if (beam_out[i] == true){
			std::stringstream ss;
			std::string buf;
			ss << "Q=" << beam[i].q << ",M=" << beam[i].m << ",J=" << beam[i].J << "_";
			ss >> buf;
			result += buf;
		}
	}
	result += "/";
	if (scc_out){
		std::string buf;
		std::stringstream ss;
		ss << "SCC=" << scc;
		ss >> buf;
		result += buf;
	}
	return result;
}