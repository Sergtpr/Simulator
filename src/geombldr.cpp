#include "geombldr.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>


GeomBldr::GeomBldr(const FileMan& fm){
	std::string filename(fm.get_main_dir() + "/geometry/" + fm.get_geom_fn() + "/" + fm.get_geom_fn());
	std::string name(filename + ".dxf");
	filename = filename + ".txt";

	std::ifstream file(filename);
	for (std::string line; std::getline(file, line, ';');){
		std::istringstream is(line);
		std::string command;
		is >> command;
		if (command == "AMOUNT_OF_GEOM_PARTS"){
			is >> num_of_parts;
		} else if ( command == "S_SI_LIST" ){
			for (int i = 0; i <= num_of_parts; i++){
				double s_si_temp;
				is >> s_si_temp;
				s_si_list.push_back(s_si_temp);
				if (i > 0){
					l_si_list.push_back(s_si_list[i] - s_si_list[i - 1]);
				}
			}
		} else if ( command == "H_LIST" ){
			for (int i = 0; i < num_of_parts; i++){
				double h_temp;
				is >> h_temp;
				h_list.push_back(h_temp);
			}
		} else if ( command == "RAD_SIMU_LIST" ){
			for (int i = 0; i < num_of_parts; i++){
				double rad_temp;
				is >> rad_temp;
				rad_simu_list.push_back(rad_temp);
			}
		} else if ( command == "PLASMA_BOUND" ){
			is >> plasma_bound;
		} else if ( command == "BEAM_START" ){
			is >> start_of_beam;
		} else if ( command == "BEAM_RADIUS"){
			is >> rad_beam;
		}
	}
	for (int i = 0; i < num_of_parts; i++){
		double size_x, size_y, size_z, rad_simu, s_si, h;
		size_x = 2*rad_simu_list[i];
		size_y = 2*rad_simu_list[i];
		size_z = l_si_list[i]+h_list[i];
		rad_simu = rad_simu_list[i];
		h = h_list[i];
		s_si = s_si_list[i];

		Int3D meshsize(
			(int) floor(size_x/h)+1,
			(int) floor(size_y/h)+1,
			(int) floor(size_z/h)+1 );
		Vec3D origin(-rad_simu, -rad_simu, s_si);
		meshsize_list.push_back(meshsize);
		origin_list.push_back(origin);
	}
}

Int3D GeomBldr::get_meshsize(int n_o_p) const { return meshsize_list[n_o_p]; }
Vec3D GeomBldr::get_origin(int n_o_p) const { return origin_list[n_o_p]; }
double GeomBldr::get_h(int n_o_p) const { return h_list[n_o_p]; }