#ifndef UTIL_BARYCENTRIC_INSIDE_H
#define UTIL_BARYCENTRIC_INSIDE_H

#include <util_is_number.h>
#include <util_is_finite.h>

#include <OpenTissue/core/math/math_value_traits.h>


#include <cmath>
#include <cassert>

namespace util
{

  /**
   * Test of barycentric coordinates of an edge corresponds to a point inside the edge.
   */
  template<typename T>
  inline bool barycentric_inside(T const & w1, T const & w2)
  {
    typedef typename OpenTissue::math::ValueTraits<T> VT;

    assert( is_number(w1) || !"barycentric_inside(): NaN encountered");
    assert( is_number(w2) || !"barycentric_inside(): NaN encountered");
    assert( is_finite(w1) || !"barycentric_inside(): Inf encountered");
    assert( is_finite(w2) || !"barycentric_inside(): Inf encountered");

    if(w1 < VT::zero())
      return false;
    if(w2<VT::zero())
      return false;

    if(w1>VT::one())
      return false;
    if(w2>VT::one())
      return false;

    if((w1+w2)>VT::one())
      return false;

    if((w1+w2)<VT::zero())
      return false;

    return true;
  }

  /**
   * Test of barycentric coordinates of a triangle corresponds to a point inside the triangle.
   */
  template<typename T>
  inline bool barycentric_inside(T const & w1, T const & w2, T const & w3)
  {
    typedef typename OpenTissue::math::ValueTraits<T> VT;

    assert( is_number(w1) || !"barycentric_inside(): NaN encountered");
    assert( is_number(w2) || !"barycentric_inside(): NaN encountered");
    assert( is_number(w3) || !"barycentric_inside(): NaN encountered");
    assert( is_finite(w1) || !"barycentric_inside(): Inf encountered");
    assert( is_finite(w2) || !"barycentric_inside(): Inf encountered");
    assert( is_finite(w3) || !"barycentric_inside(): Inf encountered");


    if(w1<VT::zero())
      return false;
    if(w2<VT::zero())
      return false;
    if(w3<VT::zero())
      return false;

    if(w1>VT::one())
      return false;
    if(w2>VT::one())
      return false;
    if(w3>VT::one())
      return false;

    if((w1+w2+w3)>VT::one())
      return false;

    if((w1+w2+w3)<VT::zero())
      return false;


    return true;
  }

  /**
   * Test of barycentric coordinates of a tetrahedron corresponds to a point inside the tetrahedron.
   */
  template<typename T>
  inline bool barycentric_inside(T const & w1, T const & w2, T const & w3, T const & w4)
  {
    typedef typename OpenTissue::math::ValueTraits<T> VT;

    assert( is_number(w1) || !"barycentric_inside(): NaN encountered");
    assert( is_number(w2) || !"barycentric_inside(): NaN encountered");
    assert( is_number(w3) || !"barycentric_inside(): NaN encountered");
    assert( is_number(w4) || !"barycentric_inside(): NaN encountered");
    assert( is_finite(w1) || !"barycentric_inside(): Inf encountered");
    assert( is_finite(w2) || !"barycentric_inside(): Inf encountered");
    assert( is_finite(w3) || !"barycentric_inside(): Inf encountered");
    assert( is_finite(w4) || !"barycentric_inside(): Inf encountered");

    if(w1<VT::zero())
      return false;
    if(w2<VT::zero())
      return false;
    if(w3<VT::zero())
      return false;
    if(w4<VT::zero())
      return false;

    if(w1>VT::one())
      return false;
    if(w2>VT::one())
      return false;
    if(w3>VT::one())
      return false;
    if(w4>VT::one())
      return false;

    if((w1+w2+w3+w4)>VT::one())
      return false;

    if((w1+w2+w3+w4)<VT::zero())
      return false;

    return true;
  }

}//namespace util

// UTIL_BARYCENTRIC_INSIDE_H
#endif
