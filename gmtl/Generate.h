/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: Generate.h,v $
 * Date modified: $Date: 2002-02-20 21:39:40 $
 * Version:       $Revision: 1.10 $
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

#include <gmtl/Vec.h>    // for Vec
#include <gmtl/VecOps.h> // for lengthSquared
#include <gmtl/Matrix.h>
#include <gmtl/Meta.h>

namespace gmtl
{
   // VEC
   // TODO: Vec& makeNormalized( Vec&, x, y )
   // TODO: Vec& makeNormalized( Vec&, x, y, z )
   // TODO: Vec& makeNormalized( Vec&, x, y, z, w )

   
   /** Create a translation matrix from vec.
    * @pre if making an n x n matrix, then for
    * <ul>
    *  <li><b>vector is homogeneous:</b> SIZE of vector needs to equal number of Matrix ROWS - 1  
    *  <li><b>vector has scale component:</b> SIZE of vector needs to equal number of Matrix ROWS 
    * </ul>
    * if making an n x n+1 matrix, then for
    * <ul>
    *  <li><b>vector is homogeneous:</b> SIZE of vector needs to equal number of Matrix ROWS 
    *  <li><b>vector has scale component:</b> SIZE of vector needs to equal number of Matrix ROWS + 1
    * </ul>
    * @post if preconditions are not met, then function is undefined (will not compile)
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS, unsigned SIZE >
   inline Matrix<DATA_TYPE, ROWS, COLS>& makeTrans( Matrix<DATA_TYPE, ROWS, COLS>& result, const Vec<DATA_TYPE, SIZE>& trans )
   {
      /* @todo make this a compile time assert... */
      // if n x n   then (homogeneous case) vecsize == rows-1 or (scale component case) vecsize == rows 
      // if n x n+1 then (homogeneous case) vecsize == rows   or (scale component case) vecsize == rows+1
      ggtASSERT( ((ROWS == COLS && (SIZE == (ROWS-1) || SIZE == ROWS)) || 
               (COLS == (ROWS+1) && (SIZE == ROWS || SIZE == (ROWS+1)))) && 
              "preconditions not met for vector size in call to makeTrans.  Read your documentation." );
      result = Matrix<DATA_TYPE, ROWS, COLS>(); // set to ident - this could be faster...
      
      // homogeneous case...
      if ((ROWS == COLS && SIZE == ROWS) || (COLS == (ROWS+1) && SIZE == (ROWS+1)))  
      {
         for (int x = 0; x < COLS - 1; ++x)
            result( x, COLS - 1 ) = trans[x] / trans[SIZE-1];
      }
      
      // non-homogeneous case...
      else
      {
         for (int x = 0; x < COLS - 1; ++x)
            result( x, COLS - 1 ) = trans[x];
      }
      return result;
   }

   /** Create a scale matrix.
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS, unsigned SIZE >
   inline Matrix<DATA_TYPE, ROWS, COLS>& makeScale( Matrix<DATA_TYPE, ROWS, COLS>& result, const Vec<DATA_TYPE, SIZE>& scale )
   {
      ggtASSERT( ((SIZE == (ROWS-1) && SIZE == (COLS-1)) || (SIZE == (ROWS-1) && SIZE == COLS) || (SIZE == (COLS-1) && SIZE == ROWS)) && "the scale params must fit within the matrix, check your sizes." );
      result = Matrix<DATA_TYPE, ROWS, COLS>(); // set to ident - this could be faster...
      for (int x = 0; x < SIZE; ++x)
         result( x, x ) = scale[x];
      return result;
   }

   /** Create a scale matrix.
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& makeScale( Matrix<DATA_TYPE, ROWS, COLS>& result, const DATA_TYPE scale )
   {
      result = Matrix<DATA_TYPE, ROWS, COLS>();
      for (int x = 0; x < Math::Min( ROWS, COLS, Math::Max( ROWS, COLS ) - 1 ); ++x) // account for 2x4 or other weird sizes...
         result( x, x ) = scale;
      return result;
   }
   
   /** Create a rotation matrix using an axis and an angle (in radians).
    *  to a rotation matrix defined by the rotation part of M
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& makeRot( Matrix<DATA_TYPE, ROWS, COLS>& result, const DATA_TYPE radians, const Vec<DATA_TYPE, 3>& vec )
   {
      /* @todo make this a compile time assert... */
      ggtASSERT( ROWS >= 3 && COLS >= 3 && ROWS <= 4 && COLS <= 4 && "this func is undefined for Matrix smaller than 3x3 or bigger than 4x4" );
      ggtASSERT( Math::isEqual( lengthSquared( vec ), (DATA_TYPE)1.0, (DATA_TYPE)0.001 ) && "you must pass in a normalized vector to makeRot( rad, vec )" );
      
      // GGI: pg 466
      DATA_TYPE s = Math::sin( radians );
      DATA_TYPE c = Math::cos( radians );
      DATA_TYPE t = 1.0f - c;
      DATA_TYPE x = vec[0];
      DATA_TYPE y = vec[1];
      DATA_TYPE z = vec[2];

      /* From: Introduction to robotic.  Craig.  Pg. 52 */
      result( 0, 0 ) = (t*x*x)+c;     result( 0, 1 ) = (t*x*y)-(s*z); result( 0, 2 ) = (t*x*z)+(s*y); 
      result( 1, 0 ) = (t*x*y)+(s*z); result( 1, 1 ) = (t*y*y)+c;     result( 1, 2 ) = (t*y*z)-(s*x); 
      result( 2, 0 ) = (t*x*z)-(s*y); result( 2, 1 ) = (t*y*z)+(s*x); result( 2, 2 ) = (t*z*z)+c;     

      // if 4x3, 3x4, or 4x4, then fill in the rest with ident...
      if (ROWS > 3)
      {
         result( 3, 0 ) = (DATA_TYPE)0.0; result( 3, 1 ) = (DATA_TYPE)0.0; result( 3, 2 ) = (DATA_TYPE)0.0;
      }
                  
      if (COLS > 3)           
      {
         result( 0, 3 ) = (DATA_TYPE)0.0;
         result( 1, 3 ) = (DATA_TYPE)0.0;
         result( 2, 3 ) = (DATA_TYPE)0.0;
      }
      
      if (ROWS > 3 && COLS > 3)
      {
         result( 3, 3 ) = (DATA_TYPE)1.0;
      }

      return result;
   }
   
   /** Create a rotation matrix using an axis and an angle (in radians).   (static version)
    *  to a rotation matrix defined by the rotation part of M
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    */
   /*
   template< typename MATRIX_CLASS >
   inline MATRIX_CLASS makeRot( MATRIX_CLASS::datatype radians, MATRIX_CLASS::datatype x, MATRIX_CLASS::datatype y, MATRIX_CLASS::datatype z, Type2Type<MATRIX_CLASS> t = Type2Type<MATRIX_CLASS>() )
   {
      // (slow -> uses a temporary)
      MATRIX_CLASS temporary;
      return makeRot( temporary, radians, x, y, z );
   }
   */
         
   enum RotationOrder
   {
      XYZ, ZYX, ZXY
   };
   
   /** Create a rotation matrix using euler angles (in radians)
    * @pre pass in your args in the same order as the RotationOrder you specify
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& makeRot( Matrix<DATA_TYPE, ROWS, COLS>& result, const DATA_TYPE param0, 
                                                 const DATA_TYPE param1, const DATA_TYPE param2, const RotationOrder order, 
                                                 Type2Type<Matrix<DATA_TYPE, ROWS, COLS> > t = Type2Type<Matrix<DATA_TYPE, ROWS, COLS> >() )
   {
      // @todo make this a compile time assert...
      ggtASSERT( ROWS >= 3 && COLS >= 3 && ROWS <= 4 && COLS <= 4 && "this is undefined for Matrix smaller than 3x3 or bigger than 4x4" );

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
         ggtASSERT( false && "unknown rotation order passed to makeRot" );
         break;
      }
         
      // if 4x3, 3x4, or 4x4, then fill in the rest with ident...
      if (ROWS > 3)
      {
         result( 3, 0 ) = (DATA_TYPE)0.0; result( 3, 1 ) = (DATA_TYPE)0.0; result( 3, 2 ) = (DATA_TYPE)0.0;
      }
                  
      if (COLS > 3)           
      {
         result( 0, 3 ) = (DATA_TYPE)0.0;
         result( 1, 3 ) = (DATA_TYPE)0.0;
         result( 2, 3 ) = (DATA_TYPE)0.0;
      }
      
      if (ROWS > 3 && COLS > 3)
      {
         result( 3, 3 ) = (DATA_TYPE)1.0;
      }

      return result;
   }
   
   /** Create a rotation matrix using euler angles (in radians) (static version)
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    */
   /*
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS> makeRot( DATA_TYPE rotx, DATA_TYPE roty, DATA_TYPE rotz, RotationOrder order, Type2Type<Matrix<DATA_TYPE, ROWS, COLS> > t = Type2Type<Matrix<DATA_TYPE, ROWS, COLS> >() )
   {
      Matrix<DATA_TYPE, ROWS, COLS> temporary;
      return makeRot( temporary, rotx, roty, rotz, order );
   }
   */
         
   /** create a rotation matrix that will rotate from SrcAxis to DestAxis.
    *  xSrcAxis, ySrcAxis, zSrcAxis is the base rotation to go from and defaults to xSrcAxis(1,0,0), ySrcAxis(0,1,0), zSrcAxis(0,0,1) if you only pass in 3 axes.
    *  @pre pass in 3 axes, and makeDirCos will give you the rotation from MATRIX_IDENTITY to DestAxis
    *  @pre pass in 6 axes, and makeDirCos will give you the rotation from your 3-axis rotation to your second 3-axis rotation
    *  @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& makeDirCos( Matrix<DATA_TYPE, ROWS, COLS>& result, const Vec<DATA_TYPE, 3>& xDestAxis, const Vec<DATA_TYPE, 3>& yDestAxis, const Vec<DATA_TYPE, 3>& zDestAxis,
         const Vec<DATA_TYPE, 3>& xSrcAxis = Vec<DATA_TYPE, 3>(1,0,0), const Vec<DATA_TYPE, 3>& ySrcAxis = Vec<DATA_TYPE, 3>(0,1,0), const Vec<DATA_TYPE, 3>& zSrcAxis = Vec<DATA_TYPE, 3>(0,0,1) )
   {
      // @todo make this a compile time assert...
      ggtASSERT( ROWS >= 3 && COLS >= 3 && ROWS <= 4 && COLS <= 4 && "this is undefined for Matrix smaller than 3x3 or bigger than 4x4" );

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
      
      // if 4x3, 3x4, or 4x4, then fill in the rest with ident...
      if (ROWS > 3)
      {
         result( 3, 0 ) = (DATA_TYPE)0.0; result( 3, 1 ) = (DATA_TYPE)0.0; result( 3, 2 ) = (DATA_TYPE)0.0;
      }
                  
      if (COLS > 3)           
      {
         result( 0, 3 ) = (DATA_TYPE)0.0;
         result( 1, 3 ) = (DATA_TYPE)0.0;
         result( 2, 3 ) = (DATA_TYPE)0.0;
      }
      
      if (ROWS > 3 && COLS > 3)
      {
         result( 3, 3 ) = (DATA_TYPE)1.0;
      }
      
      return result;
   }
   
   /** set the matrix 3x3 to the direction cosines (static version)
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices
    */
   /*
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS> makeDirCos( const Vec<DATA_TYPE, 3>& xDestAxis, const Vec<DATA_TYPE, 3>& yDestAxis, const Vec<DATA_TYPE, 3>& zDestAxis, Type2Type<Matrix<DATA_TYPE, ROWS, COLS> > t = Type2Type<Matrix<DATA_TYPE, ROWS, COLS> >() )
   {
      Matrix<DATA_TYPE, ROWS, COLS> temporary;
      return makeAxes( temporary, xDestAxis, yDestAxis, zDestAxis );
   }
   */

   /** make the matrix given the raw coordinate axes.
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    * @post these axes are copied direct to the 3x3 in the matrix
    */
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS>& makeAxes( Matrix<DATA_TYPE, ROWS, COLS>& result, const Vec<DATA_TYPE, 3>& xAxis, const Vec<DATA_TYPE, 3>& yAxis, const Vec<DATA_TYPE, 3>& zAxis )
   {
      // @todo make this a compile time assert...
      ggtASSERT( ROWS >= 3 && COLS >= 3 && ROWS <= 4 && COLS <= 4 && "this is undefined for Matrix smaller than 3x3 or bigger than 4x4" );

      result( 0, 0 ) = xAxis[0];
      result( 1, 0 ) = xAxis[1];
      result( 2, 0 ) = xAxis[2];

      result( 0, 1 ) = yAxis[0];
      result( 1, 1 ) = yAxis[1];
      result( 2, 1 ) = yAxis[2];

      result( 0, 2 ) = zAxis[0];
      result( 1, 2 ) = zAxis[1];
      result( 2, 2 ) = zAxis[2];

      // if 4x3, 3x4, or 4x4, then fill in the rest with ident...
      if (ROWS > 3)
      {
         result( 3, 0 ) = (DATA_TYPE)0.0; result( 3, 1 ) = (DATA_TYPE)0.0; result( 3, 2 ) = (DATA_TYPE)0.0;
      }

      if (COLS > 3)           
      {
         result( 0, 3 ) = (DATA_TYPE)0.0;
         result( 1, 3 ) = (DATA_TYPE)0.0;
         result( 2, 3 ) = (DATA_TYPE)0.0;
      }

      if (ROWS > 3 && COLS > 3)
      {
         result( 3, 3 ) = (DATA_TYPE)1.0;
      }

      return result;
   }
   
   /** make the matrix given the raw coordinate axes. (static version)
    * @post this function only produces 3x3, 3x4, 4x3, and 4x4 matrices, and is undefined otherwise
    * @post these axes are copied direct to the 3x3 in the matrix
    */
   /*
   template< typename DATA_TYPE, unsigned ROWS, unsigned COLS >
   inline Matrix<DATA_TYPE, ROWS, COLS> makeAxes( const Vec<DATA_TYPE, 3>& xAxis, const Vec<DATA_TYPE, 3>& yAxis, const Vec<DATA_TYPE, 3>& zAxis, Type2Type<Matrix<DATA_TYPE, ROWS, COLS> > t = Type2Type<Matrix<DATA_TYPE, ROWS, COLS> >() )
   {
      Matrix<DATA_TYPE, ROWS, COLS> temporary;
      return makeAxes( temporary, xAxis, yAxis, zAxis );
   }
   */
      
} // end gmtl namespace.

#endif