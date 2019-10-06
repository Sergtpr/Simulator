#pragma once

#include "vec3d.hpp"
#include "meshscalarfield.hpp"
#include "geometry.hpp"
#include "particledatabase.hpp"
#include "epot_efield.hpp"

#include "simulator.hpp"

#define LIGHT_VEL 299792458

class Analyser {
public:

	double get_current() const;
	double get_normalized_emit() const;
	double get_beam_angle(double z, double dz) const;
	double get_r_rms(double z) const;
	void save(const std::string& path) const;

	Analyser(Simulator& sim) 
	: _geom(sim._geom), _epot(sim._epot), _pdb(sim._pdb) {}
	//Analyser(const Geometry& geom, const MeshScalarField& epot, const ParticleDataBase3D& pdb)
	//	: _geom(geom), _epot(epot), _pdb(pdb) {} 
private:
	const Geometry& _geom;
	const MeshScalarField& _epot;
	const ParticleDataBase3D& _pdb;
};
