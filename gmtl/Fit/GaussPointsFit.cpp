/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: GaussPointsFit.cpp,v $
 * Date modified: $Date: 2002-01-18 20:43:12 $
 * Version:       $Revision: 1.1.1.1 $
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
// Based on code from:
// Magic Software, Inc.
// http://www.magic-software.com
//

#include <GMTL/Vec3.h>
#include <GMTL/Point3.h>
#include <GMTL/matVecFuncs.h>

#include <GMTL/Numerics/Eigen.h>
#include <GMTL/Fit/GaussPointsFit.h>


namespace gmtl
{

void GaussPointsFit (int iQuantity, const Point3* akPoint,
    Point3& rkCenter, Vec3 akAxis[3], float afExtent[3])
{
    // compute mean of points
    rkCenter = akPoint[0];
    unsigned i;
    for (i = 1; i < iQuantity; i++)
        rkCenter += akPoint[i];
    float fInvQuantity = 1.0f/iQuantity;
    rkCenter *= fInvQuantity;

    // compute covariances of points
    float fSumXX = 0.0, fSumXY = 0.0, fSumXZ = 0.0;
    float fSumYY = 0.0, fSumYZ = 0.0, fSumZZ = 0.0;
    for (i = 0; i < iQuantity; i++)
    {
        Vec3 kDiff = akPoint[i] - rkCenter;
        fSumXX += kDiff[Xelt]*kDiff[Xelt];
        fSumXY += kDiff[Xelt]*kDiff[Yelt];
        fSumXZ += kDiff[Xelt]*kDiff[Zelt];
        fSumYY += kDiff[Yelt]*kDiff[Yelt];
        fSumYZ += kDiff[Yelt]*kDiff[Zelt];
        fSumZZ += kDiff[Zelt]*kDiff[Zelt];
    }
    fSumXX *= fInvQuantity;
    fSumXY *= fInvQuantity;
    fSumXZ *= fInvQuantity;
    fSumYY *= fInvQuantity;
    fSumYZ *= fInvQuantity;
    fSumZZ *= fInvQuantity;

    // compute eigenvectors for covariance matrix
    gmtl::Eigen kES(3);
    kES.Matrix(0,0) = fSumXX;
    kES.Matrix(0,1) = fSumXY;
    kES.Matrix(0,2) = fSumXZ;
    kES.Matrix(1,0) = fSumXY;
    kES.Matrix(1,1) = fSumYY;
    kES.Matrix(1,2) = fSumYZ;
    kES.Matrix(2,0) = fSumXZ;
    kES.Matrix(2,1) = fSumYZ;
    kES.Matrix(2,2) = fSumZZ;
    kES.IncrSortEigenStuff3();

    akAxis[0][Xelt] = kES.GetEigenvector(0,0);
    akAxis[0][Yelt] = kES.GetEigenvector(1,0);
    akAxis[0][Zelt] = kES.GetEigenvector(2,0);

    akAxis[1][Xelt] = kES.GetEigenvector(0,1);
    akAxis[1][Yelt] = kES.GetEigenvector(1,1);
    akAxis[1][Zelt] = kES.GetEigenvector(2,1);

    akAxis[2][Xelt] = kES.GetEigenvector(0,2);
    akAxis[2][Yelt] = kES.GetEigenvector(1,2);
    akAxis[2][Zelt] = kES.GetEigenvector(2,2);

    afExtent[0] = kES.GetEigenvalue(0);
    afExtent[1] = kES.GetEigenvalue(1);
    afExtent[2] = kES.GetEigenvalue(2);
}


//
bool GaussPointsFit (int iQuantity, const Vec3* akPoint,
    const bool* abValid, Vec3& rkCenter, Vec3 akAxis[3],
    float afExtent[3])
{
    // compute mean of points
    rkCenter = Vec3::ZERO;
    int i, iValidQuantity = 0;
    for (i = 0; i < iQuantity; i++)
    {
        if ( abValid[i] )
        {
            rkCenter += akPoint[i];
            iValidQuantity++;
        }
    }
    if ( iValidQuantity == 0 )
        return false;

    float fInvQuantity = 1.0/iValidQuantity;
    rkCenter *= fInvQuantity;

    // compute covariances of points
    float fSumXX = 0.0, fSumXY = 0.0, fSumXZ = 0.0;
    float fSumYY = 0.0, fSumYZ = 0.0, fSumZZ = 0.0;
    for (i = 0; i < iQuantity; i++)
    {
        if ( abValid[i] )
        {
            Vec3 kDiff = akPoint[i] - rkCenter;
            fSumXX += kDiff[Xelt]*kDiff[Xelt];
            fSumXY += kDiff[Xelt]*kDiff[Yelt];
            fSumXZ += kDiff[Xelt]*kDiff[Zelt];
            fSumYY += kDiff[Yelt]*kDiff[Yelt];
            fSumYZ += kDiff[Yelt]*kDiff[Zelt];
            fSumZZ += kDiff[Zelt]*kDiff[Zelt];
        }
    }
    fSumXX *= fInvQuantity;
    fSumXY *= fInvQuantity;
    fSumXZ *= fInvQuantity;
    fSumYY *= fInvQuantity;
    fSumYZ *= fInvQuantity;
    fSumZZ *= fInvQuantity;

    // compute eigenvectors for covariance matrix
    Eigen kES(3);
    kES.Matrix(0,0) = fSumXX;
    kES.Matrix(0,1) = fSumXY;
    kES.Matrix(0,2) = fSumXZ;
    kES.Matrix(1,0) = fSumXY;
    kES.Matrix(1,1) = fSumYY;
    kES.Matrix(1,2) = fSumYZ;
    kES.Matrix(2,0) = fSumXZ;
    kES.Matrix(2,1) = fSumYZ;
    kES.Matrix(2,2) = fSumZZ;
    kES.IncrSortEigenStuff3();

    akAxis[0][Xelt] = kES.GetEigenvector(0,0);
    akAxis[0][Yelt] = kES.GetEigenvector(1,0);
    akAxis[0][Zelt] = kES.GetEigenvector(2,0);

    akAxis[1][Xelt] = kES.GetEigenvector(0,1);
    akAxis[1][Yelt] = kES.GetEigenvector(1,1);
    akAxis[1][Zelt] = kES.GetEigenvector(2,1);

    akAxis[2][Xelt] = kES.GetEigenvector(0,2);
    akAxis[2][Yelt] = kES.GetEigenvector(1,2);
    akAxis[2][Zelt] = kES.GetEigenvector(2,2);

    afExtent[0] = kES.GetEigenvalue(0);
    afExtent[1] = kES.GetEigenvalue(1);
    afExtent[2] = kES.GetEigenvalue(2);

    return true;
}

};

/*
void MgcGaussPointsFit (int iQuantity, const MgcVector2* akPoint,
    MgcVector2& rkCenter, MgcVector2 akAxis[2], float afExtent[2])
{
    // compute mean of points
    rkCenter = akPoint[0];
    int i;
    for (i = 1; i < iQuantity; i++)
        rkCenter += akPoint[i];
    float fInvQuantity = 1.0/iQuantity;
    rkCenter *= fInvQuantity;

    // compute covariances of points
    float fSumXX = 0.0, fSumXY = 0.0, fSumYY = 0.0;
    for (i = 0; i < iQuantity; i++)
    {
        MgcVector2 kDiff = akPoint[i] - rkCenter;
        fSumXX += kDiff.x*kDiff.x;
        fSumXY += kDiff.x*kDiff.y;
        fSumYY += kDiff.y*kDiff.y;
    }
    fSumXX *= fInvQuantity;
    fSumXY *= fInvQuantity;
    fSumYY *= fInvQuantity;

    // solve eigensystem of covariance matrix
    MgcEigen kES(2);
    kES.Matrix(0,0) = fSumXX;
    kES.Matrix(0,1) = fSumXY;
    kES.Matrix(1,0) = fSumXY;
    kES.Matrix(1,1) = fSumYY;
    kES.IncrSortEigenStuff2();

    akAxis[0].x = kES.GetEigenvector(0,0);
    akAxis[0].y = kES.GetEigenvector(1,0);
    akAxis[1].x = kES.GetEigenvector(0,1);
    akAxis[1].y = kES.GetEigenvector(1,1);

    afExtent[0] = kES.GetEigenvalue(0);
    afExtent[1] = kES.GetEigenvalue(1);
}
*/


/*
bool MgcGaussPointsFit (int iQuantity, const MgcVector2* akPoint,
    const bool* abValid, MgcVector2& rkCenter, MgcVector2 akAxis[2],
    float afExtent[2])
{
    // compute mean of points
    rkCenter = MgcVector2::ZERO;
    int i, iValidQuantity = 0;
    for (i = 0; i < iQuantity; i++)
    {
        if ( abValid[i] )
        {
            rkCenter += akPoint[i];
            iValidQuantity++;
        }
    }
    if ( iValidQuantity == 0 )
        return false;

    float fInvQuantity = 1.0/iValidQuantity;
    rkCenter *= fInvQuantity;

    // compute covariances of points
    float fSumXX = 0.0, fSumXY = 0.0, fSumYY = 0.0;
    for (i = 0; i < iQuantity; i++)
    {
        if ( abValid[i] )
        {
            MgcVector2 kDiff = akPoint[i] - rkCenter;
            fSumXX += kDiff.x*kDiff.x;
            fSumXY += kDiff.x*kDiff.y;
            fSumYY += kDiff.y*kDiff.y;
        }
    }
    fSumXX *= fInvQuantity;
    fSumXY *= fInvQuantity;
    fSumYY *= fInvQuantity;

    // solve eigensystem of covariance matrix
    MgcEigen kES(2);
    kES.Matrix(0,0) = fSumXX;
    kES.Matrix(0,1) = fSumXY;
    kES.Matrix(1,0) = fSumXY;
    kES.Matrix(1,1) = fSumYY;
    kES.IncrSortEigenStuff2();

    akAxis[0].x = kES.GetEigenvector(0,0);
    akAxis[0].y = kES.GetEigenvector(1,0);
    akAxis[1].x = kES.GetEigenvector(0,1);
    akAxis[1].y = kES.GetEigenvector(1,1);

    afExtent[0] = kES.GetEigenvalue(0);
    afExtent[1] = kES.GetEigenvalue(1);

    return true;
}
*/

