/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: Generate.h,v $
 * Date modified: $Date: 2002-05-05 19:58:54 $
 * Version:       $Revision: 1.46 $
 * -----------------------------------------------------------------
 *
 *********************************************************** ggt-head end */
/*************************************************************** ggt-cpr beg
*
* GGT: The Generic Graphics Toolkit
* Copyright (C) 2001,2002 Allen Bierbaum
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 2.1 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
 ************************************************************ ggt-cpr end */
#ifndef _GMTL_GENERATE_H_
#define _GMTL_GENERATE_H_

#include <gmtl/Assert.h>

#include <gmtl/Vec.h>    // for Vec
#include <gmtl/VecOps.h> // for lengthSquared
#include <gmtl/Quat.h>
#include <gmtl/QuatOps.h>
#include <gmtl/Coord.h>
#include <gmtl/Matrix.h>
#include <gmtl/Meta.h>
#include <gmtl/Math.h>


/** @defgroup Generate Generators: make( ... ), get( ... ), set( ... ), and convert( ... ).
 *  Make get and set functions for all math types in gmtl.
 */

// @todo Vec& setNormal( Vec&, scalar, scalar, scalar ) (and other dimensions)  (might not need, use this instead - setNormal( Vec( scal, scal, scal ) ))
// @todo getRot euler for quat
// @todo getTrans(mat, vec)  (or is it called vec = setTrans(mat), or is it called convert( vec, mat ), convert( mat, vec ) )
// @todo getScale( mat, vec ) or getScale( mat, scalar )
// @todo getRot( mat, scalar, vec ) getRot( mat, deg, x, y, z )
// @todo getRot(mat, a,b,c ) euler
// @todo getDirCos( mat, axes... )
// @todo getAxes( mat, ... )

namespace gmtl
{
   /** @addtogroup Generate */
   //@{
   
   /** @name Vec Generators */
   //@{
   
   /** create a vector from the vector component of a quaternion
    * @post quat = [v,0] = [v0,v1,v2,0]
    * @todo should this be called convert?
    */
   template <typename DATA_TYPE>
   inline Vec<DATA_TYPE, 3> makeVec( const Quat<DATA_TYPE>& quat )
   {
      return Vec<DATA_TYPE, 3>( quat[Xelt], quat[Yelt], quat[Zelt] );
   }

   /** create a normalized vector from the given vector.
    */
   template <typename DATA_TYPE, unsigned SIZE>
   inline Vec<DATA_TYPE, SIZE> makeNormal( Vec<DATA_TYPE, SIZE> vec )
   {
      normalize( vec );
      return vec;
   }

   //@}
      
   /** @name Quat Generators */
   //@{

   /** Set pure quaternion
   * @todo Write test case for setPure
   */
   template <typename DATA_TYPE>
   inline void setPure( Quat<DATA_TYPE>& quat, const Vec<DATA_TYPE, 3>& vec )
   {
      quat.set(vec[0], vec[1], vec[2], 0);
   }

   /** create a pure quaternion
    * @post quat = [v,0] = [v0,v1,v2,0]
    */
   template <typename DATA_TYPE>
   inline Quat<DATA_TYPE> makePure( const Vec<DATA_TYPE, 3>& vec )
   {
      return Quat<DATA_TYPE>( vec[0], vec[1], vec[2], 0 );
   }

   /** create a pure quaternion
    * @post quat = [v,0] = [v0,v1,v2,0]
    */
   template <typename DATA_TYPE>
   inline Quat<DATA_TYPE> makeNormal( const Quat<DATA_TYPE>& quat )
   {
      Quat<DATA_TYPE> temporary( quat );
      return normalize( temporary );
   }

   /** quaternion complex conjugate.
    *  @post set result to the complex conjugate of result.
    *  @post result'[x,y,z,w] == result[-x,-y,-z,w]
    *  @see Quat
    */
   template <typename DATA_TYPE>
   inline Quat<DATA_TYPE> makeConj( const Quat<DATA_TYPE>& quat )
   {
      Quat<DATA_TYPE> temporary( quat );
      return conj( temporary );
   }

   /** create quaternion from the inverse of another quaternion.
    *  @post returns the multiplicative inverse of quat
    *  @see Quat
    */
   template <typename DATA_TYPE>
   inline Quat<DATA_TYPE> makeInvert( const Quat<DATA_TYPE>& quat )
   {
      Quat<DATA_TYPE> temporary( quat );
      return invert( temporary );
   }

   /** set a rotation quaternion from an angle and an axis (fast).
    * @pre axis must be normalized, or length == 0, prior to calling this.
    * @post q = [ cos(rad/2), sin(rad/2) * [x,y,z] ]
    */
   template <typename DATA_TYPE>
   inline Quat<DATA_TYPE>& setRot( Quat<DATA_TYPE>& result, const DATA_TYPE rad, Vec<DATA_TYPE, 3> axis )
   {
      gmtlASSERT( (Math::isEqual( lengthSquared( axis ), (DATA_TYPE)1.0, (DATA_TYPE)0.001 ) || Math::isEqual( lengthSquared( axis ), (DATA_TYPE)0.0, (DATA_TYPE)0.001 )) && "you must pass in a normalized vector to setRot( quat, rad, vec )" );

      DATA_TYPE half_angle = rad * (DATA_TYPE)0.5;
      DATA_TYPE sin_half_angle = Math::sin( half_angle );

      result[Welt] = Math::cos( half_angle );
      result[Xelt] = sin_half_angle * axis[0];
      result[Yelt] = sin_half_angle * axis[1];
      result[Zelt] = sin_half_angle * axis[2];

      // should automagically be normalized (unit magnitude) now...
      return result;
   }

   /** set a rotation quaternion from an angle and an axis (slow).
    * @pre axis [xyz] will be normalized for you, no need to worry about it.
    * @post q = [ cos(rad/2), sin(rad/2) * [x,y,z] ]
    */
   template <typename DATA_TYPE>
   inline Quat<DATA_TYPE>& setRot( Quat<DATA_TYPE>& result, const DATA_TYPE rad, const DATA_TYPE x, const DATA_TYPE y, const DATA_TYPE z )
   {
      return setRot( result, rad, makeNormal( Vec<DATA_TYPE, 3>( x, y, z ) ) );
   }

   /** set a rotation quaternion that will xform first vector to the second.
    *  @post generate rotation quaternion that is the rotation between the vectors.
    */
   template <typename DATA_TYPE>
   inline Quat<DATA_TYPE>& setRot( Quat<DATA_TYPE>& result, const Vec<DATA_TYPE, 3>& from, const Vec<DATA_TYPE, 3>& to )
   {
      const DATA_TYPE epsilon = (DATA_TYPE)0.00001;
      DATA_TYPE cosangle = dot( from, to );

      // if cosangle is close to 1, so the vectors are close to being coincident
      // Need to generate an angle of zero with any vector we like
      // We'll choose identity (no rotation)
      if ( Math::isEqual( cosangle, (DATA_TYPE)1.0, epsilon ) )
      {
         return result = Quat<DATA_TYPE>();
      }

      // vectors are close to being opposite, so rotate one a little...
      else if ( Math::isEqual( cosangle, (DATA_TYPE)-1.0, epsilon ) )
      {
         Vec<DATA_TYPE, 3> to_rot( to[0] + (DATA_TYPE)0.3, to[1] - (DATA_TYPE)0.15, to[2] - (DATA_TYPE)0.15 ), axis;
         cross( axis, from, to_rot );
         DATA_TYPE angle = Math::aCos( cosangle );
         return setRot( result, angle, axis );
      }

      // This is the usual situation - take a cross-product of vec1 and vec2
      // and that is the axis around which to rotate.
      else
      {
         Vec<DATA_TYPE, 3> axis;
         cross( axis, from, to );
         DATA_TYPE angle = Math::aCos( cosangle );
         return setRot( result, angle, axis );
      }
   }

   /** generate a rotation quaternion that will xform first vector to the second. */
   template< typename QUAT_TYPE >
   inline QUAT_TYPE makeRot( const Vec<typename QUAT_TYPE::DataType, 3>& from, const Vec<typename QUAT_TYPE::DataType, 3>& to )
   {
      QUAT_TYPE temporary;
      return setRot( temporary, from, to );
   }

   /** get the angle/axis from the rotation quaternion.
    *
    * @post returns an angle in radians, and a normalized axis equivelent to the quaternion's rotation.
    * @post returns rad and xyz such that
    * <ul>
    * <li>  rad = acos( w ) * 2.0
    * <li>  vec = v / (asin( w ) * 2.0)   [where v is the xyz or vector component of the quat]
    * </ul>
    */
   template <typename DATA_TYPE>
   inline void getRot( Quat<DATA_TYPE> quat, DATA_TYPE& rad, DATA_TYPE& x, DATA_TYPE& y, DATA_TYPE& z )
   {
      // set sure we don't get a NaN result from acos...
      if (Math::abs( quat[Welt] ) > (DATA_TYPE)1.0)
      {
         normalize( quat );
      }
      gmtlASSERT( Math::abs( quat[Welt] ) <= (DATA_TYPE)1.0 && "acos returns NaN when quat[Welt] > 1, try normalizing your quat." );

      // [acos( w ) * 2.0, v / (asin( w ) * 2.0)]

      // get the angle:
      rad = Math::aCos( quat[Welt] ) * (DATA_TYPE)2.0;

      // get the axis: (use sin(rad) instead of asin(w))
      DATA_TYPE sin_half_angle = Math::sin( rad * (DATA_TYPE)0.5 );
      if (sin_half_angle >= (DATA_TYPE)0.0001)  // some arbitrary epsillon close to zero
      {
         DATA_TYPE sin_half_angle_inv = DATA_TYPE(1.0) / sin_half_angle;
         x = quat[Xelt] * sin_half_angle_inv;
         y = quat[Yelt] * sin_half_angle_inv;
         z = quat[Zelt] * sin_half_angle_inv;
      }

      // avoid NAN
      else
      {
         x = DATA_TYPE( 1.0 ) - quat[Welt]; // one of the terms should be a 1,
         y = (DATA_TYPE)0.0;              // so we can maintain unit-ness
         z = (DATA_TYPE)0.0;              // in case w is 0 (which here w is 0)
      }
   }

   /** Create a rotation quaternion using euler angles (each in radians).
    * @pre pass in your angles in the same order as the RotationOrder you specify
    */
   template <typename DATA_TYPE>
   inline Quat<DATA_TYPE>& setRot( Quat<DATA_TYPE>& result, const DATA_TYPE param0,
                 const DATA_TYPE param1, const DATA_TYPE param2, const RotationOrder order )
   {
      // this might be faster if put into the switch statement... (testme)
      const DATA_TYPE xRot = (order == XYZ) ? param0 : ((order == ZXY) ? param1 : param2);
      const DATA_TYPE yRot = (order == XYZ) ? param1 : ((order == ZXY) ? param2 : param1);
      const DATA_TYPE zRot = (order == XYZ) ? param2 : ((order == ZXY) ? param0 : param0);

      // this could be written better for each rotation order, but this is really general...
      Quat<DATA_TYPE> qx, qy, qz;

      // precompute half angles
      DATA_TYPE xOver2 = xRot * (DATA_TYPE)0.5;
      DATA_TYPE yOver2 = yRot * (DATA_TYPE)0.5;
      DATA_TYPE zOver2 = zRot * (DATA_TYPE)0.5;

      // set the pitch quat
      qx[Xelt] = Math::sin( xOver2 );
      qx[Yelt] = (DATA_TYPE)0.0;
      qx[Zelt] = (DATA_TYPE)0.0;
      qx[Welt] = Math::cos( xOver2 );

      // set the yaw quat
      qy[Xelt] = (DATA_TYPE)0.0;
      qy[Yelt] = Math::sin( yOver2 );
      qy[Zelt] = (DATA_TYPE)0.0;
      qy[Welt] = Math::cos( yOver2 );

      // set the roll quat
      qz[Xelt] = (DATA_TYPE)0.0;
      qz[Yelt] = (DATA_TYPE)0.0;
      qz[Zelt] = Math::sin( zOver2 );
      qz[Welt] = Math::cos( zOver2 );

      // compose the three in pyr order...
      switch (order)
      {
      case XYZ: result = qx * qy * qz; break;
      case ZYX: result = qz * qy * qx; break;
      case ZXY: result = qz * qx * qy; break;
      default:
         gmtlASSERT( false && "unknown rotation order passed to setRot" );
         break;
      }

      // ensure the quaternion is normalized
      normalize( result );
      return result;
   }

   //@}


   /** @name Matrix Generators */
   //@{

   /** Set matrix translation from vec.
    * @pre if making an n x n matrix, then for
    *    - <b>vector is homogeneous:</b> SIZE of vector needs to equal number of Matrix ROWS - 1
    *    - <b>vector has scale component:</b> SIZE of vector needs to equal number of Matrix ROWS
    * if making an n x n+1 matrix, then for
    *    - <b>vector is homogeneous:</b> SIZE of vector needs to equal number of Matrix ROWS
    *    - <b>vector has scale component:</b> SIZE of vector needs to equal number of Matrix ROWS + 1
    * @post if preconditions are not met, then function is undefined (will not compile)
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS, unsigned SIZE >
   inline Matrix<DATA_TYPE, ROWS, COLS>& setTrans( Matrix<DATA_TYPE, ROWS, COLS>& result, const Vec<DATA_TYPE, SIZE>& trans )
   {
      /* @todo make this a compile time assert... */
      // if n x n   then (homogeneous case) vecsize == rows-1 or (scale component case) vecsize == rows
      // if n x n+1 then (homogeneous case) vecsize == rows   or (scale component case) vecsize == rows+1
      gmtlASSERT( ((ROWS == COLS && (SIZE == (ROWS-1) || SIZE == ROWS)) ||
               (COLS == (ROWS+1) && (SIZE == ROWS || SIZE == (ROWS+1)))) &&
              "preconditions not met for vector size in call to makeTrans.  Read your documentation." );

      // homogeneous case...
      if ((ROWS == COLS && SIZE == ROWS) /* Square matrix and vec so assume homogeneous vector. ex. 4x4 with vec 4 */
          || (COLS == (ROWS+1) && SIZE == (ROWS+1)))  /* ex: 3x4 with vec4 */
      {
         for (unsigned x = 0; x < COLS - 1; ++x)
            result( x, COLS - 1 ) = trans[x] / trans[SIZE-1];
      }

      // non-homogeneous case...
      else
      {
         for (unsigned x = 0; x < COLS - 1; ++x)
            result( x, COLS - 1 ) = trans[x];
      }
      return result;
   }

   /** Create a translation matrix from a vec
    * @todo in .net vc7 you have to explicitly give SIZE.  This is not what we want.
   */
   template< typename MATRIX_TYPE, unsigned SIZE >
   inline MATRIX_TYPE makeTrans( const Vec<typename MATRIX_TYPE::DataType, SIZE>& trans, Type2Type< MATRIX_TYPE > t = Type2Type< MATRIX_TYPE >() )
   {
      MATRIX_TYPE temporary;
      return setTrans( temporary, trans );
   }

   /** Set vector from matrix translation.
    * @pre if making an n x n matrix, then for
    *    - <b>vector is homogeneous:</b> SIZE of vector needs to equal number of Matrix ROWS - 1
    *    - <b>vector has scale component:</b> SIZE of vector needs to equal number of Matrix ROWS
    * if making an n x n+1 matrix, then for
    *    - <b>vector is homogeneous:</b> SIZE of vector needs to equal number of Matrix ROWS
    *    - <b>vector has scale component:</b> SIZE of vector needs to equal number of Matrix ROWS + 1
    * @post if preconditions are not met, then function is undefined (will not compile)
    */
   template<typename VEC_TYPE, typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline VEC_TYPE& setTrans( VEC_TYPE& ret_trans, const Matrix<DATA_TYPE, ROWS, COLS>& arg )
   {
      // ASSERT: There are as many

      // if n x n   then (homogeneous case) vecsize == rows-1 or (scale component case) vecsize == rows
      // if n x n+1 then (homogeneous case) vecsize == rows   or (scale component case) vecsize == rows+1
      gmtlASSERT( ((ROWS == COLS && ( VEC_TYPE::Size == (ROWS-1) ||  VEC_TYPE::Size == ROWS)) ||
               (COLS == (ROWS+1) && ( VEC_TYPE::Size == ROWS ||  VEC_TYPE::Size == (ROWS+1)))) &&
              "preconditions not met for vector size in call to makeTrans.  Read your documentation." );

      // homogeneous case...
      if ((ROWS == COLS &&  VEC_TYPE::Size == ROWS)              // Square matrix and vec so assume homogeneous vector. ex. 4x4 with vec 4
          || (COLS == (ROWS+1) &&  VEC_TYPE::Size == (ROWS+1)))  // ex: 3x4 with vec4
      {
         ret_trans[VEC_TYPE::Size-1] = 1.0f;
      }

      // non-homogeneous case... (SIZE == ROWS),
      //else
      //{}

      for (unsigned x = 0; x < COLS - 1; ++x)
      {
         ret_trans[x] = arg( x, COLS - 1 );
      }

      return ret_trans;
   }
   
   /** Make vector from matrix translation.
    * @pre if making an n x n matrix, then for
    *    - <b>vector is homogeneous:</b> SIZE of vector needs to equal number of Matrix ROWS - 1
    *    - <b>vector has scale component:</b> SIZE of vector needs to equal number of Matrix ROWS
    * if making an n x n+1 matrix, then for
    *    - <b>vector is homogeneous:</b> SIZE of vector needs to equal number of Matrix ROWS
    *    - <b>vector has scale component:</b> SIZE of vector needs to equal number of Matrix ROWS + 1
    * @post if preconditions are not met, then function is undefined (will not compile)
    */
   template<typename VEC_TYPE, typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline VEC_TYPE makeTrans( const Matrix<DATA_TYPE, ROWS, COLS>& arg,
                             Type2Type< VEC_TYPE > t = Type2Type< VEC_TYPE >())
   {
      VEC_TYPE temporary;
      return setTrans( temporary, arg );
   }

   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline void getTrans( const Matrix<DATA_TYPE, ROWS, COLS>& arg,
                             DATA_TYPE& x, DATA_TYPE& y, DATA_TYPE& z)
   {
      gmtl::Vec<DATA_TYPE,3> trans_vec( makeTrans<gmtl::Vec<DATA_TYPE,3> >(arg) );
      x = trans_vec[0];
      y = trans_vec[1];
      z = trans_vec[2];
   }





   /** Set the scale part of a matrix.
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS, unsigned SIZE >
   inline Matrix<DATA_TYPE, ROWS, COLS>& setScale( Matrix<DATA_TYPE, ROWS, COLS>& result, const Vec<DATA_TYPE, SIZE>& scale )
   {
      gmtlASSERT( ((SIZE == (ROWS-1) && SIZE == (COLS-1)) || (SIZE == (ROWS-1) && SIZE == COLS) || (SIZE == (COLS-1) && SIZE == ROWS)) && "the scale params must fit within the matrix, check your sizes." );
      for (unsigned x = 0; x < SIZE; ++x)
         result( x, x ) = scale[x];
      return result;
   }

   /** Create a scale matrix.
    */
   template <typename MATRIX_TYPE, unsigned SIZE>
   inline MATRIX_TYPE makeScale( const Vec<typename MATRIX_TYPE::DataType, SIZE>& scale,
                               Type2Type< MATRIX_TYPE > t = Type2Type< MATRIX_TYPE >() )
   {
      MATRIX_TYPE temporary;
      return setScale( temporary, scale );
   }



   /** Create a scale matrix.
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& setScale( Matrix<DATA_TYPE, ROWS, COLS>& result, const DATA_TYPE scale )
   {
      for (unsigned x = 0; x < Math::Min( ROWS, COLS, Math::Max( ROWS, COLS ) - 1 ); ++x) // account for 2x4 or other weird sizes...
         result( x, x ) = scale;
      return result;
   }

   /** Create a scale matrix.
    */
   template <typename MATRIX_TYPE>
   inline MATRIX_TYPE makeScale( const typename MATRIX_TYPE::DataType scale,
                               Type2Type< MATRIX_TYPE > t = Type2Type< MATRIX_TYPE >() )
   {
      MATRIX_TYPE temporary;
      return setScale( temporary, scale );
   }



   /** Create a rotation matrix using an axis and an angle (in radians).
    *  to a rotation matrix defined by the rotation part of M
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    * @pre you must pass a normalized vector in for the axis.
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& setRot( Matrix<DATA_TYPE, ROWS, COLS>& result, const DATA_TYPE radians, const Vec<DATA_TYPE, 3>& vec )
   {
      /* @todo set this a compile time assert... */
      gmtlASSERT( ROWS >= 3 && COLS >= 3 && ROWS <= 4 && COLS <= 4 && "this func is undefined for Matrix smaller than 3x3 or bigger than 4x4" );
      gmtlASSERT( Math::isEqual( lengthSquared( vec ), (DATA_TYPE)1.0, (DATA_TYPE)0.001 ) && "you must pass in a normalized vector to setRot( mat, rad, vec )" );

      // GGI: pg 466
      DATA_TYPE s = Math::sin( radians );
      DATA_TYPE c = Math::cos( radians );
      DATA_TYPE t = DATA_TYPE( 1.0 ) - c;
      DATA_TYPE x = vec[0];
      DATA_TYPE y = vec[1];
      DATA_TYPE z = vec[2];

      /* From: Introduction to robotic.  Craig.  Pg. 52 */
      result( 0, 0 ) = (t*x*x)+c;     result( 0, 1 ) = (t*x*y)-(s*z); result( 0, 2 ) = (t*x*z)+(s*y);
      result( 1, 0 ) = (t*x*y)+(s*z); result( 1, 1 ) = (t*y*y)+c;     result( 1, 2 ) = (t*y*z)-(s*x);
      result( 2, 0 ) = (t*x*z)-(s*y); result( 2, 1 ) = (t*y*z)+(s*x); result( 2, 2 ) = (t*z*z)+c;

      return result;
   }

   /** Create a rotation matrix or quaternion (or any other rotation data type) using axis and angle.
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and quaternions, and is undefined otherwise.
    * @todo Increase perf of setRot(val,axis). Make it fast for mp
    */
   template< typename ROTATION_TYPE >
   inline ROTATION_TYPE makeRot( typename ROTATION_TYPE::DataType rad, const Vec<typename ROTATION_TYPE::DataType, 3> axis,
                               Type2Type< ROTATION_TYPE > t = Type2Type< ROTATION_TYPE >() )
   {
      ROTATION_TYPE temporary;
      return setRot( temporary, rad, axis );
   }



   /** set a rotation matrix from an angle and an axis.
    * @pre axis [xyz] will be normalized for you, no need to worry about it.
    * @post q = [ cos(rad/2), sin(rad/2) * [x,y,z] ]
    */
   template <typename DATA_TYPE, unsigned ROWS, unsigned COLS>
   inline Matrix<DATA_TYPE, ROWS, COLS>& setRot( Matrix<DATA_TYPE, ROWS, COLS>& result, const DATA_TYPE rad, const DATA_TYPE x, const DATA_TYPE y, const DATA_TYPE z )
   {
      return setRot( result, rad, makeNormal( Vec<DATA_TYPE, 3>( x, y, z ) ) );
   }

   /** generate a rotation quaternion or matrix (or any other rotation data type) from an angle and axis. */
   template< typename ROTATION_TYPE >
   inline ROTATION_TYPE makeRot( const typename ROTATION_TYPE::DataType rad, const typename ROTATION_TYPE::DataType x,
                               const typename ROTATION_TYPE::DataType y, const typename ROTATION_TYPE::DataType z,
                               Type2Type< ROTATION_TYPE > t = Type2Type< ROTATION_TYPE >() )
   {
      ROTATION_TYPE temporary;
      return setRot( temporary, rad, x, y, z );
   }




   /** Create a rotation matrix using euler angles (in radians)
    * @pre pass in your args in the same order as the RotationOrder you specify
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& setRot( Matrix<DATA_TYPE, ROWS, COLS>& result, const DATA_TYPE param0,
                                                 const DATA_TYPE param1, const DATA_TYPE param2, const RotationOrder order )
   {
      // @todo set this a compile time assert...
      gmtlASSERT( ROWS >= 3 && COLS >= 3 && ROWS <= 4 && COLS <= 4 && "this is undefined for Matrix smaller than 3x3 or bigger than 4x4" );

      // this might be faster if put into the switch statement... (testme)
      const float xRot = (order == XYZ) ? param0 : ((order == ZXY) ? param1 : param2);
      const float yRot = (order == XYZ) ? param1 : ((order == ZXY) ? param2 : param1);
      const float zRot = (order == XYZ) ? param2 : ((order == ZXY) ? param0 : param0);

      float sx = Math::sin( xRot );  float cx = Math::cos( xRot );
      float sy = Math::sin( yRot );  float cy = Math::cos( yRot );
      float sz = Math::sin( zRot );  float cz = Math::cos( zRot );

      // @todo metaprogram this!
      switch (order)
      {
      case XYZ:
         // Derived by simply multiplying out the matrices by hand X * Y * Z
         result( 0, 0 ) = cy*cz;             result( 0, 1 ) = -cy*sz;            result( 0, 2 ) = sy;
         result( 1, 0 ) = sx*sy*cz + cx*sz;  result( 1, 1 ) = -sx*sy*sz + cx*cz; result( 1, 2 ) = -sx*cy;
         result( 2, 0 ) = -cx*sy*cz + sx*sz; result( 2, 1 ) = cx*sy*sz + sx*cz;  result( 2, 2 ) = cx*cy;
         break;
      case ZYX:
         // Derived by simply multiplying out the matrices by hand Z * Y * Z
         result( 0, 0 ) = cy*cz; result( 0, 1 ) = -cx*sz + sx*sy*cz; result( 0, 2 ) = sx*sz + cx*sy*cz;
         result( 1, 0 ) = cy*sz; result( 1, 1 ) = cx*cz + sx*sy*sz;  result( 1, 2 ) = -sx*cz + cx*sy*sz;
         result( 2, 0 ) = -sy;   result( 2, 1 ) = sx*cy;             result( 2, 2 ) = cx*cy;
         break;
      case ZXY:
         // Derived by simply multiplying out the matrices by hand Z * X * Y
         result( 0, 0 ) = cy*cz - sx*sy*sz; result( 0, 1 ) = -cx*sz; result( 0, 2 ) = sy*cz + sx*cy*sz;
         result( 1, 0 ) = cy*sz + sx*sy*cz; result( 1, 1 ) = cx*cz;  result( 1, 2 ) = sy*sz - sx*cy*cz;
         result( 2, 0 ) = -cx*sy;           result( 2, 1 ) = sx;     result( 2, 2 ) = cx*cy;
         break;
      default:
         gmtlASSERT( false && "unknown rotation order passed to setRot" );
         break;
      }

      return result;
   }
   
   

   /** Create a rotation matrix or quaternion (or any other rotation data type) using euler angles (in radians)
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    * @todo Increase perf of setRot(val,val,val, rotMethod). Make it fast for mp
    */
   ///*
   template< typename ROTATION_TYPE >
   inline ROTATION_TYPE makeRot( typename ROTATION_TYPE::DataType rotx, typename ROTATION_TYPE::DataType roty, typename ROTATION_TYPE::DataType rotz,
                               RotationOrder order, Type2Type< ROTATION_TYPE > t = Type2Type< ROTATION_TYPE >() )
   {
      ROTATION_TYPE temporary;
      return setRot( temporary, rotx, roty, rotz, order );
   }
   
   
   /** get euler angles (in radians) from a rotation matrix
    * @pre pass in your args in the same order as the RotationOrder you specify
    * @post this function only reads 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline void getRot(  const Matrix<DATA_TYPE, ROWS, COLS>& mat, 
                        DATA_TYPE& param0,
                        DATA_TYPE& param1, 
                        DATA_TYPE& param2, 
                        const RotationOrder order )
   {
      // @todo set this a compile time assert...
      gmtlASSERT( ROWS >= 3 && COLS >= 3 && ROWS <= 4 && COLS <= 4 && "this is undefined for Matrix smaller than 3x3 or bigger than 4x4" );

      DATA_TYPE sx;
      DATA_TYPE cz;
      
      // @todo metaprogram this!
      switch (order)
      {
      case XYZ:
         {
            param2 = Math::aTan2( -mat(0,1), mat(0,0) );       // -(-cy*sz)/(cy*cz) - cy falls out
            param0 = Math::aTan2( -mat(1,2), mat(2,2) );       // -(sx*cy)/(cx*cy) - cy falls out
            cz = Math::cos( param2 );
            param1 = Math::aTan2( mat(0,2), mat(0,0) / cz );   // (sy)/((cy*cz)/cz)
         }
         break;
      case ZYX:
         {
            param0 = Math::aTan2( mat(1,0), mat(0,0) );        // (cy*sz)/(cy*cz) - cy falls out
            param2 = Math::aTan2( mat(2,1), mat(2,2) );        // (sx*cy)/(cx*cy) - cy falls out
            sx = Math::sin( param2 );
            param1 = Math::aTan2( -mat(2,0), mat(2,1) / sx );  // -(-sy)/((sx*cy)/sx)
         }
         break;
      case ZXY:
         {
            // Extract the rotation directly from the matrix
            DATA_TYPE x_angle;
            DATA_TYPE y_angle;
            DATA_TYPE z_angle;
            DATA_TYPE cos_y, sin_y;
            DATA_TYPE cos_x, sin_x;
            DATA_TYPE cos_z, sin_z;

            sin_x = mat(2,1);
            x_angle = Math::aSin( sin_x );     // Get x angle
            cos_x = Math::cos( x_angle );
            
            // Check if cos_x = Zero
            if (cos_x != 0.0f)     // ASSERT: cos_x != 0
            {
                  // Get y Angle
               cos_y = mat(2,2) / cos_x;
               sin_y = -mat(2,0) / cos_x;
               y_angle = Math::aTan2( cos_y, sin_y );

                  // Get z Angle
               cos_z = mat(1,1) / cos_x;
               sin_z = -mat(0,1) / cos_x;
               z_angle = Math::aTan2( cos_z, sin_z );
            }
            else
            {
               // Arbitrarily set z_angle = 0
               z_angle = 0;

                  // Get y Angle
               cos_y = mat(0,0);
               sin_y = mat(1,0);
               y_angle = Math::aTan2( cos_y, sin_y );
            }

            param1 = x_angle;
            param2 = y_angle;
            param0 = z_angle;
         }
         break;
      default:
         gmtlASSERT( false && "unknown rotation order passed to setRot" );
         break;
      }
   }
   //*/

   /**
    * Extracts the yaw information from the matrix.
    * @post Returned value is from -180 to 180, where 0 is none.
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline float getYRot ( const Matrix<DATA_TYPE, ROWS, COLS>& mat )
   {
      const gmtl::Vec3f forward_point(0.0f, 0.0f, -1.0f);   // -Z
      const gmtl::Vec3f origin_point(0.0f, 0.0f, 0.0f);
      gmtl::Vec3f end_point, start_point;

      gmtl::xform(end_point, mat, forward_point);
      gmtl::xform(start_point, mat, origin_point);
      gmtl::Vec3f direction_vector = end_point - start_point;

      // Constrain the direction to XZ-plane only.
      direction_vector[1] = 0.0f;                  // Eliminate Y value
      gmtl::normalize(direction_vector);
      float y_rot = gmtl::Math::aCos(gmtl::dot(direction_vector,
                                               forward_point));

      gmtl::Vec3f which_side = gmtl::cross(forward_point, direction_vector);

      // If direction vector to "right" (negative) side of forward
      if ( which_side[1] < 0.0f )
      {
         y_rot = -y_rot;
      }

      return y_rot;
   }

/*
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline void getYaw ( const Matrix<DATA_TYPE, ROWS, COLS>& mat )
   {
      return getYRot(mat);
   }
*/

   /**
    * Extracts the pitch information from the matrix.
    * @post Returned value is from -180 to 180, where 0 is none.
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline float getXRot ( const Matrix<DATA_TYPE, ROWS, COLS>& mat )
   {
      const gmtl::Vec3f forward_point(0.0f, 0.0f, -1.0f);   // -Z
      const gmtl::Vec3f origin_point(0.0f, 0.0f, 0.0f);
      gmtl::Vec3f end_point, start_point;

      gmtl::xform(end_point, mat, forward_point);
      gmtl::xform(start_point, mat, origin_point);
      gmtl::Vec3f direction_vector = end_point - start_point;

      // Constrain the direction to YZ-plane only.
      direction_vector[0] = 0.0f;                  // Eliminate X value
      gmtl::normalize(direction_vector);
      float x_rot = gmtl::Math::aCos(gmtl::dot(direction_vector,
                                               forward_point));

      gmtl::Vec3f which_side = gmtl::cross(forward_point, direction_vector);

      // If direction vector to "bottom" (negative) side of forward
      if ( which_side[0] < 0.0f )
      {
         x_rot = -x_rot;
      }

      return x_rot;
   }

/*
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline void getPitch ( const Matrix<DATA_TYPE, ROWS, COLS>& mat )
   {
      return getXRot(mat);
   }
*/

   /**
    * Extracts the roll information from the matrix.
    * @post Returned value is from -180 to 180, where 0 is no roll.
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline float getZRot ( const Matrix<DATA_TYPE, ROWS, COLS>& mat )
   {
      const gmtl::Vec3f forward_point(0.0f, 0.0f, -1.0f);   // -Z
      const gmtl::Vec3f origin_point(0.0f, 0.0f, 0.0f);
      gmtl::Vec3f end_point, start_point;

      gmtl::xform(end_point, mat, forward_point);
      gmtl::xform(start_point, mat, origin_point);
      gmtl::Vec3f direction_vector = end_point - start_point;

      // Constrain the direction to XY-plane only.
      direction_vector[2] = 0.0f;                  // Eliminate Z value
      gmtl::normalize(direction_vector);
      float z_rot = gmtl::Math::aCos(gmtl::dot(direction_vector,
                                               forward_point));

      gmtl::Vec3f which_side = gmtl::cross(forward_point, direction_vector);

      // If direction vector to "right" (negative) side of forward
      if ( which_side[2] < 0.0f )
      {
         z_rot = -z_rot;
      }

      return z_rot;
   }

/*
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline void getRoll ( const Matrix<DATA_TYPE, ROWS, COLS>& mat )
   {
      return getZRot(mat);
   }
*/

   /** create a rotation matrix that will rotate from SrcAxis to DestAxis.
    *  xSrcAxis, ySrcAxis, zSrcAxis is the base rotation to go from and defaults to xSrcAxis(1,0,0), ySrcAxis(0,1,0), zSrcAxis(0,0,1) if you only pass in 3 axes.
    *  @pre pass in 3 axes, and setDirCos will give you the rotation from MATRIX_IDENTITY to DestAxis
    *  @pre pass in 6 axes, and setDirCos will give you the rotation from your 3-axis rotation to your second 3-axis rotation
    *  @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& setDirCos( Matrix<DATA_TYPE, ROWS, COLS>& result,
                                                     const Vec<DATA_TYPE, 3>& xDestAxis,
                                                     const Vec<DATA_TYPE, 3>& yDestAxis, const Vec<DATA_TYPE, 3>& zDestAxis,
                                                     const Vec<DATA_TYPE, 3>& xSrcAxis = Vec<DATA_TYPE, 3>(1,0,0),
                                                     const Vec<DATA_TYPE, 3>& ySrcAxis = Vec<DATA_TYPE, 3>(0,1,0),
                                                     const Vec<DATA_TYPE, 3>& zSrcAxis = Vec<DATA_TYPE, 3>(0,0,1) )
   {
      // @todo set this a compile time assert...
      gmtlASSERT( ROWS >= 3 && COLS >= 3 && ROWS <= 4 && COLS <= 4 && "this is undefined for Matrix smaller than 3x3 or bigger than 4x4" );

      DATA_TYPE Xa, Xb, Xy;    // Direction cosines of the secondary x-axis
      DATA_TYPE Ya, Yb, Yy;    // Direction cosines of the secondary y-axis
      DATA_TYPE Za, Zb, Zy;    // Direction cosines of the secondary z-axis

      Xa = dot(xDestAxis, xSrcAxis);  Xb = dot(xDestAxis, ySrcAxis);  Xy = dot(xDestAxis, zSrcAxis);
      Ya = dot(yDestAxis, xSrcAxis);  Yb = dot(yDestAxis, ySrcAxis);  Yy = dot(yDestAxis, zSrcAxis);
      Za = dot(zDestAxis, xSrcAxis);  Zb = dot(zDestAxis, ySrcAxis);  Zy = dot(zDestAxis, zSrcAxis);

      // Set the matrix correctly
      result( 0, 0 ) = Xa; result( 0, 1 ) = Xb; result( 0, 2 ) = Xy;
      result( 1, 0 ) = Ya; result( 1, 1 ) = Yb; result( 1, 2 ) = Yy;
      result( 2, 0 ) = Za; result( 2, 1 ) = Zb; result( 2, 2 ) = Zy;

      return result;
   }

   /** Create a rotation matrix or quaternion (or any other rotation data type) using euler angles (in radians)
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    * @todo Increase perf of setRot(val,val,val, rotMethod). Make it fast for mp
    */
   ///*
   template< typename ROTATION_TYPE >
   inline ROTATION_TYPE makeDirCos( const Vec<typename ROTATION_TYPE::DataType, 3>& xDestAxis,
                                  const Vec<typename ROTATION_TYPE::DataType, 3>& yDestAxis,
                                  const Vec<typename ROTATION_TYPE::DataType, 3>& zDestAxis,
                                  const Vec<typename ROTATION_TYPE::DataType, 3>& xSrcAxis = Vec<typename ROTATION_TYPE::DataType, 3>(1,0,0),
                                  const Vec<typename ROTATION_TYPE::DataType, 3>& ySrcAxis = Vec<typename ROTATION_TYPE::DataType, 3>(0,1,0),
                                  const Vec<typename ROTATION_TYPE::DataType, 3>& zSrcAxis = Vec<typename ROTATION_TYPE::DataType, 3>(0,0,1),
                               Type2Type< ROTATION_TYPE > t = Type2Type< ROTATION_TYPE >() )
   {
      ROTATION_TYPE temporary;
      return setDirCos( temporary, xDestAxis, yDestAxis, zDestAxis, xSrcAxis, ySrcAxis, zSrcAxis );
   }




   /** set the matrix given the raw coordinate axes.
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    * @post these axes are copied direct to the 3x3 in the matrix
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& setAxes( Matrix<DATA_TYPE, ROWS, COLS>& result, const Vec<DATA_TYPE, 3>& xAxis, const Vec<DATA_TYPE, 3>& yAxis, const Vec<DATA_TYPE, 3>& zAxis )
   {
      // @todo set this a compile time assert...
      gmtlASSERT( ROWS >= 3 && COLS >= 3 && ROWS <= 4 && COLS <= 4 && "this is undefined for Matrix smaller than 3x3 or bigger than 4x4" );

      result( 0, 0 ) = xAxis[0];
      result( 1, 0 ) = xAxis[1];
      result( 2, 0 ) = xAxis[2];

      result( 0, 1 ) = yAxis[0];
      result( 1, 1 ) = yAxis[1];
      result( 2, 1 ) = yAxis[2];

      result( 0, 2 ) = zAxis[0];
      result( 1, 2 ) = zAxis[1];
      result( 2, 2 ) = zAxis[2];

      return result;
   }

   /** create a matrix transposed from the source.
    *  @post returns the transpose of m
    *  @see Quat
    */
   template < typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS> makeTranspose( const Matrix<DATA_TYPE, ROWS, COLS>& m )
   {
      return transpose( Matrix<DATA_TYPE, ROWS, COLS>( m ) );
   }

   /** set the matrix given the raw coordinate axes.
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    * @post these axes are copied direct to the 3x3 in the matrix
    */
   template< typename ROTATION_TYPE >
   inline ROTATION_TYPE makeAxes( const Vec<typename ROTATION_TYPE::DataType, 3>& xAxis,
                                  const Vec<typename ROTATION_TYPE::DataType, 3>& yAxis,
                                  const Vec<typename ROTATION_TYPE::DataType, 3>& zAxis,
                                  Type2Type< ROTATION_TYPE > t = Type2Type< ROTATION_TYPE >() )
   {
      ROTATION_TYPE temporary;
      return setAxes( temporary, xAxis, yAxis, zAxis );
   }

   //@}


   /** @name Coord Generators */
   //@{

   /// @todo redundant set and convert funcs...
   
   template <typename DATATYPE, unsigned POSSIZE, unsigned MATCOLS, unsigned MATROWS >
   inline Coord<DATATYPE, POSSIZE, 3>& setCoord( Coord<DATATYPE, POSSIZE, 3>& eulercoord, const Matrix<DATATYPE, MATROWS, MATCOLS>& mat, RotationOrder order = gmtl::XYZ )
   {
      setTrans( eulercoord.pos(), mat );
      getRot( mat, eulercoord.rot()[0], eulercoord.rot()[1], eulercoord.rot()[2], order );
      return eulercoord;
   }
   
   template <typename DATATYPE, unsigned POSSIZE, unsigned MATCOLS, unsigned MATROWS >
   inline Matrix<DATATYPE, MATROWS, MATCOLS>& setMatrix( Matrix<DATATYPE, MATROWS, MATCOLS>& mat, const Coord<DATATYPE, POSSIZE, 3>& eulercoord, RotationOrder order = gmtl::XYZ )
   {
      if (MATCOLS == 4)
      {
         setTrans( mat, eulercoord.pos() );
      }
      setRot( mat, eulercoord.rot()[0], eulercoord.rot()[1], eulercoord.rot()[2], order );
      return mat;
   }

   template <typename DATATYPE, unsigned POSSIZE, unsigned MATCOLS, unsigned MATROWS >
   inline Coord<DATATYPE, POSSIZE, 3>& convert( Coord<DATATYPE, POSSIZE, 3>& eulercoord, const Matrix<DATATYPE, MATROWS, MATCOLS>& mat, RotationOrder order = gmtl::XYZ )
   {
      return setCoord( eulercoord, mat, order );
   }
   
   template <typename DATATYPE, unsigned POSSIZE, unsigned MATCOLS, unsigned MATROWS >
   inline Matrix<DATATYPE, MATROWS, MATCOLS>& convert( Matrix<DATATYPE, MATROWS, MATCOLS>& mat, const Coord<DATATYPE, POSSIZE, 3>& eulercoord, RotationOrder order = gmtl::XYZ )
   {
      return setMatrix( mat, eulercoord, order );
   }
   
   template <typename COORD_TYPE, unsigned MATCOLS, unsigned MATROWS >
   inline COORD_TYPE makeCoord( const Matrix<typename COORD_TYPE::DataType, MATROWS, MATCOLS>& mat,
                                RotationOrder order,
                                Type2Type< COORD_TYPE > t = Type2Type< COORD_TYPE >() )
   {
      COORD_TYPE temporary;
      return convert( temporary, mat, order );
   }
   
   template <typename MATRIX_TYPE, unsigned POSSIZE, unsigned ROTSIZE >
   inline MATRIX_TYPE makeMatrix( const Coord<typename MATRIX_TYPE::DataType, POSSIZE, ROTSIZE>& coord,
                                RotationOrder order,
                                Type2Type< MATRIX_TYPE > t = Type2Type< MATRIX_TYPE >() )
   {
      MATRIX_TYPE temporary;
      return convert( temporary, coord, order );
   }

   //@}
   
   //@}
   
} // end gmtl namespace.

#endif