#include "simulator.hpp"

#include "bfieldbldr.hpp"
#include "geombldr.hpp"
#include "parameters.hpp"
#include "confbldr.hpp"
#include "fileman.hpp"

#include <iostream>
#include "readascii.hpp"
#include "gtkplotter.hpp"
#include <fstream>
 #include "ibsimu.hpp"

using Solids = std::vector<std::pair<Solid*, double>>;

template <class T> T max(const std::vector<T> vec);
std::vector<double> GetEmitParametersFromPDB3D(const Geometry& geom, const ParticleDataBase3D& pdb);

Simulator::Simulator(const FileMan& fm, int n_o_p)
	: _n_o_p(n_o_p), _fm(fm), _gb(fm), 
	_bb(fm), _cb(fm), _par(fm),
	_geom(MODE_3D, _gb.get_meshsize(n_o_p), _gb.get_origin(n_o_p), _gb.get_h(n_o_p)),
	_solver( _geom ),
	_bfield( MODE_3D, fout, _geom.size(), _geom.origo(), _geom.h() ),
	_pdb( _geom ), _scharge( _geom ), _epot( _geom ), _efield( _epot )
	{

		{
			_geom.set_boundary( 1, Bound(BOUND_NEUMANN,     0.0  ) );
			_geom.set_boundary( 2, Bound(BOUND_NEUMANN,     0.0  ) );
			_geom.set_boundary( 3, Bound(BOUND_NEUMANN,     0.0  ) );
			_geom.set_boundary( 4, Bound(BOUND_NEUMANN,     0.0  ) );
			_geom.set_boundary( 5, Bound(BOUND_NEUMANN,     0.0  ) );

			if (n_o_p == _gb.num_of_parts - 1){
				_geom.set_boundary( 6, Bound(BOUND_DIRICHLET,   _par.V.at("puller")  ) );
			} else {
				_geom.set_boundary( 6, Bound(BOUND_NEUMANN,     0.0  ) );
			}
		}

	bool pmirror[6] = { false, false, false, false, false, false };
	_pdb.set_mirror( pmirror );	

	field_extrpl_e efldextrpl[6] = { FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE, 
		FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE, 
		FIELD_EXTRAPOLATE, FIELD_EXTRAPOLATE };
	_efield.set_extrapolation( efldextrpl );

}

void Simulator::set_solids(const Solids& solids){
	for (const auto& solid : solids){
		int solid_number = 7 + _geom.number_of_solids();
		_geom.set_solid(solid_number, solid.first);
		_geom.set_boundary(solid_number, Bound(BOUND_DIRICHLET, solid.second));
	}
	_geom.build_mesh();
	_geom.build_surface();
}

void Simulator::add_beam(){
	if (_n_o_p == 0){
		for (const auto& beam : _par.beam){
			double frac = beam.J/_par.Jtotal;
			_pdb.add_cylindrical_beam_with_energy(
				_cb.N*frac, beam.J, beam.q, beam.m, _cb.Energy, 
				0.0, _cb.Ti, Vec3D(0, 0, _gb.start_of_beam), 
				Vec3D(1, 0, 0), Vec3D(0, 1, 0), _gb.rad_beam);
		}
	} else {
		input_particles();
	}
}

void Simulator::input_particles(){
	ReadAscii din( "particles.txt", 10 );
	std::cout << "Reading " << din.rows() << " particles\n";
	// Go through all read particles
	for( size_t j = 0; j < din.rows(); j++ ) {
		double I  = din[0][j];
		double m  = din[1][j];
		double q  = din[2][j];
		double t  = din[3][j];
		double x  = din[4][j];
		double vx = din[5][j];
		double y  = din[6][j];
		double vy = din[7][j];
		double z  = din[8][j];
		double vz = din[9][j];
		_pdb.add_particle( I, q/CHARGE_E, m/MASS_U, ParticleP3D(t,x,vx,y,vy,z,vz) );
	}
}

void Simulator::output_particles(){
	// Writing output file containing all particles:
		std::ofstream fileOut( "particles.txt" );
		for( size_t k = 0; k < _pdb.size(); k++ ) {

		Particle3D &pp = _pdb.particle( k );

		// Skip electrons
		if( pp.m() < 0.5*MASS_U )
		    continue;

		// Skip ions not at the end
		if( pp(PARTICLE_Z) < (_geom.max(2)-_geom.h()) )
		    continue;

		// Plot particle I, m, coordinates
		// 3D has 7 coordinates
		fileOut << std::setw(12) << pp.IQ() << " ";
		fileOut << std::setw(12) << pp.m() << " ";
		fileOut << std::setw(12) << pp.q() << " ";
		for( size_t j = 0; j < 7; j ++ )
		    fileOut << std::setw(12) << pp(j) << " ";
			fileOut << "\n";
		}
		fileOut.close();
}

void Simulator::set_bfield(){
	for (const auto& item : _par.bf_scale){
		_bb.bfobjs.at(item.first).scale = item.second;
	}
	_bb.add_to_bfield(_bfield, _geom, _n_o_p);
}

void Simulator::compute(){
	InitialPlasma init_plasma( AXIS_Z, _gb.plasma_bound );
	_solver.set_initial_plasma( _cb.Vplasma, &init_plasma );	
	MeshScalarField scharge_ave( _geom );
	MeshScalarField scharge1( _geom );
	ParticleDataBase3D pdb1( _geom );

	bool convergence = false;
	size_t i = 0;
	while ( !convergence && i < _cb.Ncycles ){

		ibsimu.message(1) << "Major cycle " << i << "\n";
		ibsimu.message(1) << "-----------------------\n";

		if (_n_o_p == 0){
			if( i == 1 ) {
				double rhoe = _pdb.get_rhosum();
				_solver.set_pexp_plasma( -rhoe, _cb.Te, _cb.Vplasma );
			}
		}
		
		_solver.solve( _epot, scharge_ave );
		_efield.recalculate();
		
		_pdb.clear();

		add_beam();

		_pdb.iterate_trajectories( _scharge, _efield, _bfield );

		// Space charge averaging and compensation
			uint32_t nodecount = _scharge.nodecount();
			double scc_ = 1 - _par.scc;
			for( size_t b = 0; b < nodecount; b++) {
				_scharge(b) = scc_*_scharge(b);
			}
			if (i == 0){
				scharge_ave = _scharge;
			} else {
				double sc_alpha = 1.0;
				double sc_beta = 1 - sc_alpha;
				for (size_t b = 0; b < nodecount; b++){
					scharge_ave(b) = sc_alpha*_scharge(b) + sc_beta*scharge_ave(b);
				}
			}
		if ( i == 0){
			scharge1 = _scharge;
			pdb1 = _pdb;
		} else {
			bool emit_conv = emit_convergence(pdb1);
			bool scharge_conv = scharge_convergence(scharge1);
			convergence = emit_conv || scharge_conv;
			scharge1 = _scharge;
			pdb1 = _pdb;
		}
		i++;
	}
	output_particles();
}

void Simulator::interactive_plot(int* argc, char*** argv){
	GTKPlotter plotter( argc, argv );
	plotter.set_geometry( &_geom );
	plotter.set_epot( &_epot );
	plotter.set_scharge( &_scharge );
	plotter.set_particledatabase( &_pdb );
	plotter.set_bfield( &_bfield );
	plotter.set_efield( &_efield );
	plotter.new_geometry_plot_window();
	plotter.run();
}

bool Simulator::emit_convergence(const ParticleDataBase3D& pdb, double max_error) const {
	std::vector<double> params = GetEmitParametersFromPDB3D( _geom, _pdb );
	std::vector<double> params1 = GetEmitParametersFromPDB3D( _geom, pdb );
	std::vector<double> error;
	for (size_t i = 0; i < params.size(); i++){
		error.push_back( abs((params1[i] - params[i])/params[i]) );
	}
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "simulation error(emittance method): " << max(error) << std::endl;


	if ( max(error) < max_error ){
		std::cout << "Simulation converged (emittance method)." << std::endl;
		std::cout << "--------------------------------------" << std::endl;
		return true;
	} else {
		std::cout << "--------------------------------------" << std::endl;
		return false;
	}
}

bool Simulator::scharge_convergence(const MeshScalarField& scharge, double max_error) const {
	uint32_t nodecount = _scharge.nodecount();
	double d_norm = 0, norm = 0, error_sc;
	for (uint32_t b = 0; b < nodecount; b++){
		d_norm += (_scharge(b) - scharge(b))*(_scharge(b) - scharge(b));
		norm += _scharge(b)*_scharge(b);
	}
	error_sc = sqrt(d_norm/norm);
	std::cout << "--------------------------------------" << std::endl;
	std::cout << "simulation error(space charge method): "  << error_sc << std::endl;

	if (error_sc < max_error){
		std::cout << "Simulation converged (space charge method)." << std::endl;
		std::cout << "--------------------------------------" << std::endl;	
		return true;
	} else {
		std::cout << "--------------------------------------" << std::endl;	
		return false;			
	}
}

template <class T> T max(const std::vector<T> vec){
		T temp = vec[0];
		for (size_t i = 1; i < vec.size(); i++)
			temp = temp > vec[i] ? temp : vec[i];
		return temp;
	}

std::vector<double> GetEmitParametersFromPDB3D(const Geometry& geom, const ParticleDataBase3D& pdb){
	std::vector<double> result;
	TrajectoryDiagnosticData tdata;
	std::vector<trajectory_diagnostic_e> diag;
		diag.push_back( DIAG_X );
		diag.push_back( DIAG_XP );
		diag.push_back( DIAG_CURR );
	
	pdb.trajectories_at_plane( tdata, AXIS_Z, geom.max(2), diag );
		Emittance emit( tdata(0).data(), tdata(1).data(), tdata(2).data() );
		double alpha = emit.alpha();
		result.push_back(alpha);
		double beta = emit.beta();
		result.push_back(beta);
		double gamma = emit.gamma();
		result.push_back(gamma);
		double epsilon = emit.epsilon();
		result.push_back(epsilon);
		return result;
}