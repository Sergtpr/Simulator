#pragma once

#include "fileman.hpp"

#include "vec3d.hpp"

#include <vector>

class GeomBldr {
public:
	int num_of_parts;
	std::vector<double> s_si_list;
	std::vector<double> l_si_list;
	std::vector<double> h_list;
	std::vector<double> rad_simu_list;
	std::vector<double> V;
	double plasma_bound, start_of_beam, rad_beam;
	GeomBldr(const FileMan& fm);
	Int3D get_meshsize(int n_o_p) const;
	Vec3D get_origin(int n_o_p) const;
	double get_h(int n_o_p) const;

private:
	std::vector<Vec3D> origin_list;
	std::vector<Int3D> meshsize_list;
};