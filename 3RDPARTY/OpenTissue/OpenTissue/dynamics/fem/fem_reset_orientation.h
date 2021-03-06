#ifndef OPENTISSUE_DYNAMICS_FEM_FEM_RESET_ORIENTATION_H
#define OPENTISSUE_DYNAMICS_FEM_FEM_RESET_ORIENTATION_H
//
// OpenTissue Template Library
// - A generic toolbox for physics-based modeling and simulation.
// Copyright (C) 2008 Department of Computer Science, University of Copenhagen.
//
// OTTL is licensed under zlib: http://opensource.org/licenses/zlib-license.php
//
#include <OpenTissue/configuration.h>

namespace OpenTissue
{
  namespace fem
  {
    namespace detail
    {

      /**
      *
      */
      template<typename tetrahedron_iterator>
      inline void reset_orientation(tetrahedron_iterator const & begin, tetrahedron_iterator const & end)
      {
        for(tetrahedron_iterator T = begin;T!=end;++T)
          T->m_Re = math::diag(1.0);
      }

    } // namespace detail
  } // namespace fem
} // namespace OpenTissue

//OPENTISSUE_DYNAMICS_FEM_FEM_RESET_ORIENTATION_H
#endif
