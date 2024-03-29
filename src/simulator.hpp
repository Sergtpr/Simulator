#pragma once

#include <utility>
#include <vector>

#include "bfieldbldr.hpp"
#include "geombldr.hpp"
#include "parameters.hpp"
#include "confbldr.hpp"
#include "fileman.hpp"

#include "particledatabase.hpp"
#include "geometry.hpp"
#include "epot_efield.hpp"
#include "meshvectorfield.hpp"
#include "epot_bicgstabsolver.hpp"
#include "func_solid.hpp"

using Solids = std::vector<std::pair<Solid*, double>>;

class Simulator {
public:
	
	int _n_o_p;
	FileMan _fm;
	GeomBldr _gb;
	BfieldBldr _bb;
	ConfBldr _cb;
	Parameters _par;

	Geometry _geom;
	EpotBiCGSTABSolver _solver;
	MeshVectorField _bfield;
	MeshScalarField _tdens;
	ParticleDataBase3D _pdb;
	MeshScalarField _scharge;
	EpotField _epot;
	EpotEfield _efield;
	
	Simulator(const FileMan& fm, int n_o_p);
	void add_beam();
	void set_solids(const Solids& solids);

	void set_geom();
	void set_pdb();
	void set_epot();
	void set_bfield();
	
	void compute();
	void input_particles();
	void output_particles();
	void interactive_plot(int* argc, char*** argv);
	bool emit_convergence(const ParticleDataBase3D& pdb, double max_error = 1.0e-4) const;
	bool scharge_convergence(const MeshScalarField& scharge, double max_error = 1.0e-4) const;
	void save() const;

private:
	bool fout[3] = {true, true, true};
};