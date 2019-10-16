#include "analyser.hpp"
#include "ibsimu.hpp"
#include <iostream>
#include <fstream>

std::vector<double> GetEmitParametersFromPDB3D(const Geometry& geom, const ParticleDataBase3D& pdb);

double Analyser::get_beam_angle(double z, double dz) const {
	std::cout << "count beam_angle at z= " << z << std::endl;
	double r1 = get_r_rms(z - dz);
	double r2 = get_r_rms(z);
	return std::atan((r2 - r1)/dz);
}

double Analyser::get_current(double z) const {
	std::cout << "count I at z= " << z << std::endl;

	double result = 0;
	TrajectoryDiagnosticData tdata;
		std::vector<trajectory_diagnostic_e> diag;
		diag.push_back( DIAG_CURR );
	_pdb.trajectories_at_plane( tdata, AXIS_Z, z, diag );
	for (size_t j = 0; j < tdata.traj_size(); j++){
		result += tdata(j, 0);
	}
	return result;
}

double Analyser::get_normalized_emit(double z) const {
	std::cout << "count eps_rms_n at z= " << z << std::endl;
	std::vector<double> twiss = GetEmitParametersFromPDB3D(_geom, _pdb);
	TrajectoryDiagnosticData tdata;
	std::vector<trajectory_diagnostic_e> diag;
		diag.push_back( DIAG_VZ );
	_pdb.trajectories_at_plane( tdata, AXIS_Z, z, diag );
	size_t amount = tdata.traj_size();
	double v_ave = 0;	
	for (size_t j = 0; j < amount; j++){
		v_ave += double (tdata(j, 0) / amount);
	}
	double res = twiss[3]*v_ave/LIGHT_VEL;
	return res;
}

double Analyser::get_r_rms(double z) const {
	std::cout << "count r_rms at z= " << z << std::endl;

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

double Analyser::get_r_max(double z) const {
	std::cout << "count r_max at z= " << z << std::endl;

	double r_max = 0;
	TrajectoryDiagnosticData tdata;
		std::vector<trajectory_diagnostic_e> diag;
		diag.push_back( DIAG_X );
		diag.push_back( DIAG_Y );

	_pdb.trajectories_at_plane( tdata, AXIS_Z, z, diag );
	size_t trajcount = tdata.traj_size();
	for (size_t j = 0; j < trajcount; j++){
		double r_temp = sqrt( tdata(j, 0)*tdata(j, 0) + tdata(j, 1)*tdata(j, 1) );
		r_max = r_max > r_temp ? r_max : r_temp;
	}
	return r_max;
}

void Analyser::save(const std::string& path, double z) const {
	std::cout << "saving analysis" << std::endl;
	std::ofstream file(path + "/analysis.txt");
	file << "beam_angle= " << get_beam_angle(z, _geom.h()) << "(rad)" << std::endl;
	file << "I= " << get_current(z) << "(A)" << std::endl;
	file << "eps_rms_n= " << get_normalized_emit(z)*1e6 / M_PI << "(pi mm mrad)" << std::endl;
	file << "r_rms=" << get_r_rms(z)*1e3 << "(mm)" << std::endl;
	file << "r_max=" << get_r_max(z)*1e3 << "(mm)" << std::endl;
	file.close();
}

void Analyser::plot_field(field_type_e fte, view_e v){
	GeomPlotter gp(_geom);
	gp.set_size(1000, 1000);
	gp.set_view(v);
	gp.set_font_size(14);
	gp.set_epot( &_epot);

	switch (fte){
		case FIELD_EPOT:
			gp.set_fieldgraph_plot(fte);
			gp.plot_png(path + "/epot_" + to_string(_n_o_p) + "_.png");
		break;
		case FIELD_EFIELD:
			gp.set_efield( &_efield);
			gp.set_fieldgraph_plot(fte);
			gp.plot_png(path + "/mod_efield_" + to_string(_n_o_p) + "_.png");
		break;
		case FIELD_EFIELD_Z:
			gp.set_efield( &_efield);
			gp.set_fieldgraph_plot(fte);	
			gp.plot_png(path + "/efieldz_" + to_string(_n_o_p) + "_.png");
		break;
		case FIELD_SCHARGE:
			gp.set_scharge( &_scharge);
			gp.set_fieldgraph_plot(fte);
			gp.plot_png(path + "/scharge_" + to_string(_n_o_p) + "_.png");
		break;
		case FIELD_TRAJDENS:
			gp.set_scharge( &_tdens);
			gp.set_fieldgraph_plot(fte);
			gp.plot_png(path + "/tdens_" + to_string(_n_o_p) + "_.png");
		break;
		default:
		break;
	}
}

void Analyser::plot_pdb(view_e v){
	GeomPlotter gp(_geom);
	gp.set_size(1000, 1000);
	gp.set_view(v);
	gp.set_font_size(14);
	gp.set_epot( &_epot );
	gp.set_fieldgraph_plot(FIELD_EPOT);
	gp.set_particledatabase( &_pdb);
	gp.plot_png(path + "/pdb_pic_" + to_string(_n_o_p) + "_.png");
}
