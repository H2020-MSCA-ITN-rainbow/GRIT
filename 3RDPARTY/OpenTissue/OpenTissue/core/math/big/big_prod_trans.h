#ifndef OPENTISSUE_CORE_MATH_BIG_PROD_TRANS_H
#define OPENTISSUE_CORE_MATH_BIG_PROD_TRANS_H
//
// OpenTissue Template Library
// - A generic toolbox for physics-based modeling and simulation.
// Copyright (C) 2008 Department of Computer Science, University of Copenhagen.
//
// OTTL is licensed under zlib: http://opensource.org/licenses/zlib-license.php
//
#include <OpenTissue/configuration.h>

#include <OpenTissue/core/math/big/big_types.h>

#include <cassert>

namespace OpenTissue
{
  namespace math
  {
    namespace big
    {

      /**
       * Compute y = prod(trans(A),x)
       *
       * @param A       The matrix.
       * @param x       A vector to be multiplied with the transpose of the specified matrix.
       * @param y       Upon return this argument holds the result of the computations.
       */
      template<typename T>
      inline void prod_trans(
          boost::numeric::ublas::compressed_matrix<T> const & A
        , boost::numeric::ublas::vector<T> const & x
        , boost::numeric::ublas::vector<T>       & y
        )
      {
        typedef boost::numeric::ublas::vector<T> vector_type;
        typedef typename vector_type::size_type  size_type;
        typedef typename vector_type::value_type real_type;

        assert(A.size1()>0            || !"prod_trans(): A was empty"            );
        assert(A.size2()>0            || !"prod_trans(): A was empty"            );
        assert(A.size1() ==  x.size() || !"prod_trans(): incompatible dimensions");
        assert(A.size2() ==  y.size() || !"prod_trans(): incompatible dimensions");

        //
        //  Example of compressed matrix format:
        //      
        //    0 x y 0
        //    0 0 z 0
        //    w 0 0 0
        //
        //   value_data: [ x y z w ]
        //
        //    Note this is simply all non-zero elements of the sparse matrix stored in a row-by-row wise manner.
        //
        //   index1_data: [ 0 2 3 ]     
        //
        //    Note that the number of elements is equal to the number of rows in the
        //    sparse matrix. Each element points to the index in value_data that
        //    corresponds to the starting element of the corresponding row.
        //
        //   index2_data: [ 1 2 2 0 ]     
        //
        //    Note this array have the same dimension as value_data. Each element in index2_data
        //    stores the corresponind column index of the matching element in value_data.
        //
        y.clear();

        size_type row_end = A.filled1 () - 1;
        for (size_type i = 0; i < row_end; ++ i) 
        {
          size_type begin = A.index1_data()[i];
          size_type end   = A.index1_data()[i + 1];
          for (size_type j = begin; j < end; ++ j)
            y( A.index2_data()[j] ) += A.value_data()[j] * x( i );
        }
      }


      /**
       * Compute y = prod(trans(A),x) + b
       *
       * @param A       The matrix.
       * @param x       A vector to be multiplied with the transpose of the specified matrix.
       * @param b       A vector to be added to the matrix vector product
       * @param y       Upon return this argument holds the result of the computations.
       */
      template<typename T>
      inline void prod_trans(
          boost::numeric::ublas::compressed_matrix<T> const & A
        , boost::numeric::ublas::vector<T> const & x
        , boost::numeric::ublas::vector<T> const & b
        , boost::numeric::ublas::vector<T>       & y
        )
      {
        typedef boost::numeric::ublas::vector<T> vector_type;
        typedef typename vector_type::size_type  size_type;
        typedef typename vector_type::value_type real_type;

        assert(A.size1()>0            || !"prod_trans(): A was empty"            );
        assert(A.size2()>0            || !"prod_trans(): A was empty"            );
        assert(A.size1() ==  x.size() || !"prod_trans(): incompatible dimensions");
        assert(A.size2() ==  y.size() || !"prod_trans(): incompatible dimensions");
        assert(b.size()  ==  y.size() || !"prod_trans(): incompatible dimensions");

        //
        //  Example of compressed matrix format:
        //      
        //    0 x y 0
        //    0 0 z 0
        //    w 0 0 0
        //
        //   value_data: [ x y z w ]
        //
        //    Note this is simply all non-zero elements of the sparse matrix stored in a row-by-row wise manner.
        //
        //   index1_data: [ 0 2 3 ]     
        //
        //    Note that the number of elements is equal to the number of rows in the
        //    sparse matrix. Each element points to the index in value_data that
        //    corresponds to the starting element of the corresponding row.
        //
        //   index2_data: [ 1 2 2 0 ]     
        //
        //    Note this array have the same dimension as value_data. Each element in index2_data
        //    stores the corresponind column index of the matching element in value_data.
        //
        y.assign( b );

        size_type row_end = A.filled1 () - 1;
        for (size_type i = 0; i < row_end; ++ i) 
        {
          size_type begin = A.index1_data()[i];
          size_type end   = A.index1_data()[i + 1];
          for (size_type j = begin; j < end; ++ j)
            y( A.index2_data()[j] ) += A.value_data()[j] * x( i );
        }
      }

    } // end  namespace big
  } // end  namespace math
} // end namespace OpenTissue
// OPENTISSUE_CORE_MATH_BIG_PROD_TRANS_H
#endif
