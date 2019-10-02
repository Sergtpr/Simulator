#pragma once

#include "func_solid.hpp"

#include "parameters.hpp"
#include "fileman.hpp"

#include <utility>
#include <string>
#include <map>

using Solids = std::vector<std::pair<Solid*, double>>;

class SolidBldr {
public:
	std::string name; //filename
	std::map<std::string, Solid*> solids;
	SolidBldr(const FileMan& fm);
	Solids get_solids_from_parameters(const Parameters& _par) const;
};