#pragma once

#include <string>
#include <map>
#include <vector>

#include "fileman.hpp"

struct Beam {
	double q;
	double m;
	double J;
};

class Parameters {
public:
	std::map<std::string, double> V;
	std::map<std::string, double> bf_scale;
	double scc, Jtotal;
	std::vector<Beam> beam;
		//OUT flags:
	std::map<std::string, bool> V_out;
	std::map<std::string, bool> bf_scale_out;
	bool scc_out;
	std::vector<bool> beam_out;
	std::string get_output_name();
	Parameters(const FileMan& fm);
};