#pragma once

#include "meshvectorfield.hpp"
#include "epot_efield.hpp"
#include "particledatabase.hpp"
#include "trajectorydiagnostics.hpp"

class ConvergenceTools {
public:
	const Geometry& _geom;
	const EpotField& _epot;
	MeshScalarField& _scharge;
	const ParticleDataBase3D& _pdb;
};