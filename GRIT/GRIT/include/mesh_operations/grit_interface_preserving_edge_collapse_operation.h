#ifndef GRIT_INTERFACE_PRESERVING_EDGE_COLLAPSE_OPERATION_H
#define GRIT_INTERFACE_PRESERVING_EDGE_COLLAPSE_OPERATION_H

#include <mesh_operations/grit_interface_edge_collapse_operation.h>

namespace grit
{
  namespace details
  {

    ///
    template< typename types>
    class InterfacePreservingEdgeCollapseOperation
    : public InterfaceEdgeCollapseOperation<types>
    {
    protected:

      typedef typename types::real_type               T;
      typedef typename types::vector3_type            V;
      typedef typename types::math_types              MT;
      typedef typename types::attributes_type         AT;
      typedef typename types::param_type              PT;

      typedef typename V::value_traits                VT;

      typedef InterfaceEdgeCollapseOperation<types>   base_class;

      typedef typename base_class::simplex1_lut_type  simplex1_lut_type;
      typedef typename base_class::simplex2_lut_type  simplex2_lut_type;

    protected:

      T            const   m_strength;              ///< The maximum area by which the EdgeCollapseOperation can change the total area
                                                    ///< of any phase.

      T            const   m_area_threshold;        ///< The minimum area of a triangle that EdgeCollapseOperation is allowed to create

    public:

      InterfacePreservingEdgeCollapseOperation(
                                               std::string    const & operation_name
                                               , unsigned int const   label
                                               , PT           const & parameters
                    )
      : base_class( operation_name, label, parameters)
      , m_strength( parameters.get_strength( operation_name, label))
      , m_area_threshold( parameters.get_area_threshold( operation_name, label))
      { }

    public:

      void init(
                InterfaceMesh  const & mesh
                , AT  & attributes
                )
      { }

    public:

      bool update_local_attributes(
                                    Simplex1 const & simplex
                                  , InterfaceMesh const & mesh
                                  , AT & attributes
                                  )
      {
        return false;
      }

    public:

      bool plan_local_connectivity_changes(
                                           Simplex1            const & simplex
                                           , InterfaceMesh           & mesh
                                           , AT                const & attributes
                                           , SimplexSet              & new_simplices
                                           , SimplexSet              & old_simplices
                                           , simplex1_lut_type       & simplex1_lut
                                           , simplex2_lut_type       & simplex2_lut
                                           )
      {
        //--- Finding the subset of vertices in C, which are eligible for collapsing
        SimplexSet const S = mesh.star(simplex);
        SimplexSet const C = mesh.closure(simplex);

        Simplex0 v1, v2;
        extract_simplices(v1, v2, C);

        SimplexSet const W = find_collapsable_vertices( simplex, mesh, base_class::m_parameters, true);

        SimplexSet::simplex0_const_iterator it = W.begin0();

        for( ; it != W.end0(); ++it)
        {
          //--- Try to move v
          Simplex0   const & v  = *it;
          Simplex0   const & vp = (v==v1)?v2:v1;

          SimplexSet const   Sv = mesh.star(v);
          SimplexSet const   T  = difference( Sv, S);

          if( is_any_inverted<V>( v, T, attributes, attributes.get_current_value(vp), m_area_threshold))
          {
            continue;
          }

          if( (IsInterface(mesh)(simplex) || IsBoundary(mesh)(simplex))
             && changes_phase_area<V>( v, vp, Sv, attributes, mesh, m_strength))
          {
            continue;
          }

          base_class::collapse( v, vp, simplex, mesh, attributes, new_simplices, old_simplices, simplex1_lut, simplex2_lut);

          return true;
        }

        return false;
      }

    }; // class InterfacePreservingEdgeCollapseOperation
  } // end namespace details
} // end namespace grit

// GRIT_INTERFACE_PRESERVING_EDGE_COLLAPSE_OPERATION_H
#endif


