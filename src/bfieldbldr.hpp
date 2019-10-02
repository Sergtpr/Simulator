#pragma once

#include <map>
#include <string>

#include "fileman.hpp"
#include "meshvectorfield.hpp"
#include "geometry.hpp"

class BfieldObj {
public:
	int left_edge = 0, right_edge = 0;
	double xscale = 1.0e-3, scale = 0;
	std::string obj_name = "null";
	BfieldObj(const FileMan& fm, const std::string& name);
};

class BfieldBldr {
public:
	std::map<std::string, BfieldObj> bfobjs;
	BfieldBldr(const FileMan& fm);
	void add_to_bfield(MeshVectorField& bf, const Geometry& geom, int n_o_p) const; 
};