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
	Parameters(const FileMan& fm);
};