#include "solidbldr.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include "dxf_solid.hpp"
#include "mydxffile.hpp"

using Solids = std::vector<std::pair<Solid*, double>>;

#define os std::cout

SolidBldr::SolidBldr(const FileMan& fm){
	os << "SolidBldr starts" << std::endl;
	std::string filename(fm.get_path() + "/geometry/" + fm.get_geom_fn() + "/" + fm.get_geom_fn());
	name = filename + ".dxf";
	filename = filename + ".txt";

	std::ifstream file(filename);
	for (std::string line; std::getline(file, line, ';');){
		std::istringstream is(line);
		std::string command;
		is >> command;
		if ( command == "SOLID_NAMES" ){
			MyDXFFile *dxffile = new MyDXFFile( name );
			while( !is.eof() ){
				std::string s_name;
				is >> s_name; 
				DXFSolid *s = new DXFSolid(dxffile, s_name);
				s->scale(1.0e-3);
				s->define_2x3_mapping( DXFSolid::rotz );
				solids[s_name] = s;
			}
		}
	}
	file.close();
}

Solids SolidBldr::get_solids_from_parameters(const Parameters& _par) const {
	Solids result;
	for (const auto& item : _par.V){
		if ( solids.count(item.first) == 0 ){
			if (item.first != "gnd")
				std::cout << "Parameters use unknown solid_name " << item.first << std::endl;
		} else {
			result.push_back(std::make_pair(solids.at(item.first), item.second));
		}
	}
	return result;
}
