# Simulator
A wrapper for IBSimu (http://ibsimu.sourceforge.net/index.html).    
Encapsulates simulaton source code. 
Simulation space is divided into parts. MODE_3D is used but Geometry is axisymmetric.
Simulator class works with: 
  Solid Objects - Func_solid (built by DXF_Solid) with its potential
  Bfield Objects - bfield (converted from axisymmetric field) with its scales.
  Configure - some unchangeable plasma and computational properties
  Parameters - simulation variables (such as solid potentials, bfield scales, beam structure, etc)
