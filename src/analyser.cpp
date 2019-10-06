#include "analyser.hpp"
#include "ibsimu.hpp"
#include <iostream>
#include <fstream>

std::vector<double> GetEmitParametersFromPDB3D(const Geometry& geom, const ParticleDataBase3D& pdb);

double Analyser::get_beam_angle(double z, double dz) const {
	double r1 = get_r_rms(z - dz);
	double r2 = get_r_rms(z);
	return std::atan((r2 - r1)/dz);
}

double Analyser::get_current() const {
	double result = 0;
	TrajectoryDiagnosticData tdata;
		std::vector<trajectory_diagnostic_e> diag;
		diag.push_back( DIAG_CURR );
	_pdb.trajectories_at_plane( tdata, AXIS_Z, _geom.max(2), diag );
	for (size_t j = 0; j < tdata.traj_size(); j++){
		result += tdata(j, 0);
	}
	return result;
}

double Analyser::get_normalized_emit() const {
	std::vector<double> twiss = GetEmitParametersFromPDB3D(_geom, _pdb);
	TrajectoryDiagnosticData tdata;
	std::vector<trajectory_diagnostic_e> diag;
		diag.push_back( DIAG_VZ );
	_pdb.trajectories_at_plane( tdata, AXIS_Z, _geom.max(2), diag );
	size_t amount = tdata.traj_size();
	double v_ave = 0;	
	for (size_t j = 0; j < amount; j++){
		v_ave += double (tdata(j, 0) / amount);
	}
	double res = twiss[3]*v_ave/LIGHT_VEL;
	return res;
}

double Analyser::get_r_rms(double z) const {
	double r_rms = 0;
	TrajectoryDiagnosticData tdata;
		std::vector<trajectory_diagnostic_e> diag;
		diag.push_back( DIAG_X );
		diag.push_back( DIAG_Y );

	_pdb.trajectories_at_plane( tdata, AXIS_Z, z, diag );
	size_t trajcount = tdata.traj_size();
	for (size_t j = 0; j < trajcount; j++){
		double r_temp = sqrt( tdata(j, 0)*tdata(j, 0) + tdata(j, 1)*tdata(j, 1) );
		r_rms = r_rms + (r_temp*r_temp);
	}

	r_rms = sqrt(r_rms/(double) trajcount);
	return r_rms;
}

void Analyser::save(const std::string& path) const {
	std::ofstream file(path + "/analysis.txt");
	file << "beam_angle= " << get_beam_angle(_geom.max(2), _geom.h()) << "(rad)" << std::endl;
	file << "I= " << get_current() << "(A)" << std::endl;
	file << "eps_rms_n= " << get_normalized_emit() << "(pi mm mrad)" << std::endl;
	file << "r_rms=" << get_r_rms(_geom.max(2)) << std::endl;
	file.close();
}