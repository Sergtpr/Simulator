#include "bfieldbldr.hpp"
#include "fileman.hpp"
#include "vec3d.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <utility>

#define os std::cout

BfieldObj::BfieldObj(const FileMan& fm, const std::string& name){
	os << "BfieldObj starts" << std::endl;
	std::string filename(fm.get_main_dir() + "/bfields/" + name + "/" + name + ".txt");
	obj_name = fm.get_main_dir() + "/bfields/" + name + "/" + name;
	std::ifstream file(filename);
	os << "BF_OBJ " << name << ": " << std::endl;
	
	for (std::string line; std::getline(file, line, ';');){
		std::istringstream is(line);
		std::string command;
		is >> command;
		if (command == "LEFT_AND_RIGHT_EDGES"){
			is >> left_edge >> right_edge;
			os << "LEFT_AND_RIGHT_EDGES: " << left_edge << " " << right_edge << std::endl;
		} else if ( command == "XSCALE_AND_SCALE" ){
			is >> xscale >> scale;
			os << "XSCALE_AND_SCALE: " << xscale << " " << scale << std::endl;
		} if ( command == "TRANSLATION" ){
			is >> translation;
			os << "TRANSLATION: " << translation << std::endl;
		} else {
			os << "WARNING: unknown command (BfieldObj): " << command << std::endl;
		}
	}
	file.close();
}

BfieldBldr::BfieldBldr(const FileMan& fm){
	for (const std::string& name : fm.get_bfield_objs()){
		BfieldObj bfobj(fm, name);
		auto p = make_pair(name, bfobj);
		bfobjs.insert(p);
		std::cout << "Add new bfield_obj " << name << std::endl;
	}
}

void BfieldBldr::add_to_bfield(MeshVectorField& bf, const Geometry& geom, int n_o_p) const {
	bool fout[3] = {true, true, true};
	field_extrpl_e bfldextrpl[6] = { FIELD_ZERO, FIELD_ZERO, 
			FIELD_ZERO, FIELD_ZERO, 
			FIELD_ZERO, FIELD_ZERO };
	for (const auto& item : bfobjs){
		if ( item.second.left_edge <= n_o_p && n_o_p <= item.second.right_edge ){
			std::string bf_filename(item.second.obj_name);
						
			MeshVectorField bf_cyl( MODE_CYL, fout, item.second.xscale, item.second.scale, bf_filename + to_string(n_o_p) + ".dat" );
			bf_cyl.translate(Vec3D(item.second.translation, 0));

			MeshVectorField bf_temp( MODE_3D, fout, geom.size(), geom.origo(), geom.h(), bf_cyl);
			bf_temp.set_extrapolation( bfldextrpl );

			bf += bf_temp;
		}
	}
}