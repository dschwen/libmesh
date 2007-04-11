// $Id: uniform_refinement_estimator.h,v 1.5 2007-04-11 23:06:42 roystgnr Exp $

// The libMesh Finite Element Library.
// Copyright (C) 2002-2005  Benjamin S. Kirk, John W. Peterson
  
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
  
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
  
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA



#ifndef __uniform_refinement_estimator_h__
#define __uniform_refinement_estimator_h__

// C++ includes
#include <vector>

// Local Includes
#include "error_estimator.h"





/**
 * This class implements a ``brute force'' error estimator
 * which integrates differences between the current solution
 * and the solution on a uniformly refined (in h and/or p,
 * for an arbitrary number of levels) grid.
 *
 * @author Roy H. Stogner, 2006.
 */
class UniformRefinementEstimator : public ErrorEstimator
{
public:

  /**
   * Constructor.  Sets the most common default parameter values.
   */
  UniformRefinementEstimator() : number_h_refinements(1),
                                 number_p_refinements(0),
                                 _sobolev_order(1) {}
  
  /**
   * Destructor.  
   */
  ~UniformRefinementEstimator() {}


  /**
   * This function does uniform refinements and a
   * solve to get an improved solution on each cell,
   * then estimates the error by integrating differences
   * between the coarse and fine solutions.
   *
   * system.solve() must be called, and so should
   * have no side effects.
   *
   * Only the provided system is solved on the refined mesh;
   * for problems decoupled into multiple systems, use of
   * estimate_errors() should be more reliable.
   *
   * The estimated error is output in the vector
   * \p error_per_cell
   */
  virtual void estimate_error (const System& system,
			       ErrorVector& error_per_cell,
			       bool estimate_parent_error = false);

  /**
   * Currently this function ignores the component_scale member variable,
   * and uses the function argument component_scales instead.
   *
   * This function is named estimate_errors instead of estimate_error
   * because otherwise C++ can get confused.
   */
  virtual void estimate_errors (const EquationSystems& equation_systems,
				ErrorVector& error_per_cell,
				std::map<const System*, std::vector<float> >& component_scales,
				bool estimate_parent_error = false);

  /**
   * Currently this function ignores the component_scale member variable,
   * because it calculates each error individually, unscaled.
   * 
   * The user selects which errors get confused by filling a map with error
   * vectors: If errors_per_cell[&system][v] exists, it will be filled with the
   * error values in variable \p v of \p system
   */
  virtual void estimate_errors (const EquationSystems& equation_systems,
			        ErrorMap& errors_per_cell,
			        bool estimate_parent_error = false);

  /**
   * Returns or allows you to set the Sobolev order for error computations
   * e.g. 0 for H^0/L_2 error, 1 for H^1, 2 for H^2
   */
  unsigned int & sobolev_order (void)
    { return _sobolev_order; }

  /**
   * How many h refinements to perform to get the fine grid
   */
  unsigned char number_h_refinements;
  
  /**
   * How many p refinements to perform to get the fine grid
   */
  unsigned char number_p_refinements;

protected:
  /**
   * The code for estimate_error and both estimate_errors versions is very
   * similar, so we use the same function for all three
   */
  virtual void _estimate_error (const EquationSystems *equation_systems,
                                const System* system,
				ErrorVector* error_per_cell,
			        std::map<std::pair<const System*, unsigned int>, ErrorVector*>* errors_per_cell,
				std::map<const System*, std::vector<float> >* component_scales,
				bool estimate_parent_error = false);

private:
  /**
   * Sobolev order - e.g. 0 for H^0/L_2 error, 1 for H^1, 2 for H^2
   */
  unsigned int _sobolev_order;

};


#endif

