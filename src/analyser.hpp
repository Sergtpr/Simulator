#pragma once

#include "vec3d.hpp"
#include "meshscalarfield.hpp"
#include "geometry.hpp"
#include "particledatabase.hpp"
#include "epot_efield.hpp"

#include "simulator.hpp"
#include "geomplotter.hpp"

#define LIGHT_VEL 299792458

class Analyser {
public:

	double get_current(double z) const;
	double get_normalized_emit(double z) const;
	double get_beam_angle(double z, double dz) const;
	double get_r_rms(double z) const;
	double get_r_max(double z) const;
	void save(const std::string& path, double z) const;

	Analyser(Simulator& sim) 
	: _geom(sim._geom), _bfield(sim._bfield), _tdens(sim._tdens),
	_pdb(sim._pdb), _scharge(sim._scharge), _epot(sim._epot),
	_efield(sim._efield), path(sim._fm.get_output_path() + "/pictures") {
		_n_o_p = sim._n_o_p;
	}

	void plot_field(field_type_e fte, view_e v); //EPOT, SCHARGE, TDENS, EFIELD,(Z)
	void plot_pdb(view_e v);

private:
	int _n_o_p;
	Geometry& _geom;
	MeshVectorField& _bfield;
	MeshScalarField& _tdens;
	ParticleDataBase3D& _pdb;
	MeshScalarField& _scharge;
	EpotField& _epot;
	EpotEfield& _efield;
	std::string path;
};
