#pragma once

#include "fileman.hpp"

class ConfBldr {
public:
	int N;
	double Vplasma;
	double Energy;
	double Te;
	double Ti;
	double alpha_aver;
	size_t Ncycles;
	ConfBldr(const FileMan& fm);
};