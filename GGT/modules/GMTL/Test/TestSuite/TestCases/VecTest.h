/************************************************************** ggt-head beg
 *
 * GGT: Generic Graphics Toolkit
 *
 * Original Authors:
 *   Allen Bierbaum
 *
 * -----------------------------------------------------------------
 * File:          $RCSfile: VecTest.h,v $
 * Date modified: $Date: 2002-02-11 23:53:04 $
 * Version:       $Revision: 1.6 $
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
#include <gmtl/gmtlConfig.h>
#include <iostream>

#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include <gmtl/Vec.h>
#include <gmtl/Compare.h>
#include <gmtl/VecOps.h>

namespace gmtlTest
{

class VecTest : public CppUnit::TestCase
{
public:
   VecTest( std::string name = "VecTest" )
   : TestCase (name)
   {;}

   virtual ~VecTest()
   {}

   virtual void setUp()
   {;}

   virtual void tearDown()
   {
   }

   void testCreation()
   {
      gmtl::Vec<double, 3> vec;

      CPPUNIT_ASSERT( vec[0] == 0.0f);
      CPPUNIT_ASSERT( vec[1] == 0.0f);
      CPPUNIT_ASSERT( vec[2] == 0.0f);

      // Test overhead of creation
      const long iters(400000);
      CPPUNIT_METRIC_START_TIMING();

      for( long iter=0;iter<iters; ++iter)
      {
         gmtl::Vec<float, 2> test_vec2;
         test_vec2[0] = 1.0f;
         gmtl::Vec<float, 3> test_vec3;
         test_vec3[0] = 2.0f;
         gmtl::Vec<float, 4> test_vec4;
         test_vec4[0] = 3.0f;
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/VecCreationOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testCopyConstruct()
   {
      gmtl::Vec<double, 3> test_vec;

      test_vec[0] = 2.0f;
      test_vec[1] = 4.0f;
      test_vec[2] = 8.0f;

      gmtl::Vec<double, 3> test_vec_copy(test_vec);

      CPPUNIT_ASSERT( test_vec_copy[0] == 2.0f);
      CPPUNIT_ASSERT( test_vec_copy[1] == 4.0f);
      CPPUNIT_ASSERT( test_vec_copy[2] == 8.0f);

      // Test copy construction overhead
      const long iters(400000);
      gmtl::Vec<float, 2> test_vec2;
      test_vec2[0] = 2.0f;
      gmtl::Vec<float, 3> test_vec3;
      test_vec2[0] = 3.0f;
      gmtl::Vec<float, 4> test_vec4;
      test_vec2[0] = 4.0f;
      float use_value;     // A temp just here to use the objs so the copiler (hopefully) does not opt them out

      CPPUNIT_METRIC_START_TIMING();

      for( long iter=0;iter<iters; ++iter)
      {
         gmtl::Vec<float, 2> test_vec2_copy(test_vec2);
         use_value += test_vec2_copy[0];
         gmtl::Vec<float, 3> test_vec3_copy(test_vec3);
         use_value += test_vec3_copy[0];
         gmtl::Vec<float, 4> test_vec4_copy(test_vec4);
         use_value += test_vec4_copy[0];
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/CopyConstructOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }


   void testConstructors()
   {
      gmtl::Vec<float, 4> test_vec4(1.0f, 2.0f, 3.0f, 4.0f);
      CPPUNIT_ASSERT( test_vec4[0] == 1.0f);
      CPPUNIT_ASSERT( test_vec4[1] == 2.0f);
      CPPUNIT_ASSERT( test_vec4[2] == 3.0f);
      CPPUNIT_ASSERT( test_vec4[3] == 4.0f);

      gmtl::Vec<float, 3> test_vec3(1.0f, 2.0f, 3.0f);
      CPPUNIT_ASSERT( test_vec3[0] == 1.0f);
      CPPUNIT_ASSERT( test_vec3[1] == 2.0f);
      CPPUNIT_ASSERT( test_vec3[2] == 3.0f);

      gmtl::Vec<float, 2> test_vec2(1.0f, 2.0f);
      CPPUNIT_ASSERT( test_vec2[0] == 1.0f);
      CPPUNIT_ASSERT( test_vec2[1] == 2.0f);

      gmtl::Vec<float, 1> test_vec1(1.0f);
      CPPUNIT_ASSERT( test_vec1[0] == 1.0f);

      // Test constructor
      const long iters(400000);
      float use_value(0.0f);     // A temp just here to use the objs so the copiler (hopefully) does not opt them out

      CPPUNIT_METRIC_START_TIMING();

      for( long iter=0;iter<iters; ++iter)
      {
         gmtl::Vec<float, 4> test_vec4(1.0f, 2.0f, 3.0f, 4.0f);
         gmtl::Vec<float, 3> test_vec3(1.0f, 2.0f, 3.0f);
         gmtl::Vec<float, 2> test_vec2(1.0f, 2.0f);
         gmtl::Vec<float, 1> test_vec1(1.0f);

         use_value = use_value + test_vec4[3] + test_vec3[2] + test_vec2[1] + test_vec1[0];
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/ConstructorsOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testSet()
   {
      gmtl::Vec<float, 4> test_vec4;
      test_vec4.set(1.0f, 2.0f, 3.0f, 4.0f);
      CPPUNIT_ASSERT( test_vec4[0] == 1.0f);
      CPPUNIT_ASSERT( test_vec4[1] == 2.0f);
      CPPUNIT_ASSERT( test_vec4[2] == 3.0f);
      CPPUNIT_ASSERT( test_vec4[3] == 4.0f);

      gmtl::Vec<float, 3> test_vec3;
      test_vec3.set(1.0f, 2.0f, 3.0f);
      CPPUNIT_ASSERT( test_vec3[0] == 1.0f);
      CPPUNIT_ASSERT( test_vec3[1] == 2.0f);
      CPPUNIT_ASSERT( test_vec3[2] == 3.0f);

      gmtl::Vec<float, 2> test_vec2;
      test_vec2.set(1.0f, 2.0f);
      CPPUNIT_ASSERT( test_vec2[0] == 1.0f);
      CPPUNIT_ASSERT( test_vec2[1] == 2.0f);

      gmtl::Vec<float, 1> test_vec1;
      test_vec1.set(1.0f);
      CPPUNIT_ASSERT( test_vec1[0] == 1.0f);

      // Test constructor
      const float iters(400000);
      float use_value(0.0f);     // A temp just here to use the objs so the copiler (hopefully) does not opt them out

      CPPUNIT_METRIC_START_TIMING();

      for( float iter=0;iter<iters; ++iter)
      {
         test_vec4.set(iters+0, iters+1, iters+2, iters+3);
         test_vec3.set(iters+0, iters+1, iters+2);
         test_vec2.set(iters+0, iters+1);
         test_vec1.set(iters+0);

         use_value = use_value + test_vec4[3] + test_vec3[2] + test_vec2[1] + test_vec1[0];
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/SetOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testSetPtr()
   {
      float data[4] = {1.0f, 2.0f, 3.0f, 4.0f};

      gmtl::Vec<float, 4> test_vec4;
      test_vec4.set(data);
      CPPUNIT_ASSERT( test_vec4[0] == 1.0f);
      CPPUNIT_ASSERT( test_vec4[1] == 2.0f);
      CPPUNIT_ASSERT( test_vec4[2] == 3.0f);
      CPPUNIT_ASSERT( test_vec4[3] == 4.0f);

      gmtl::Vec<float, 3> test_vec3;
      test_vec3.set(data);
      CPPUNIT_ASSERT( test_vec3[0] == 1.0f);
      CPPUNIT_ASSERT( test_vec3[1] == 2.0f);
      CPPUNIT_ASSERT( test_vec3[2] == 3.0f);

      gmtl::Vec<float, 2> test_vec2;
      test_vec2.set(data);
      CPPUNIT_ASSERT( test_vec2[0] == 1.0f);
      CPPUNIT_ASSERT( test_vec2[1] == 2.0f);

      gmtl::Vec<float, 1> test_vec1;
      test_vec1.set(data);
      CPPUNIT_ASSERT( test_vec1[0] == 1.0f);

      // Test constructor
      const float iters(400000);
      float use_value(0.0f);     // A temp just here to use the objs so the copiler (hopefully) does not opt them out

      CPPUNIT_METRIC_START_TIMING();

      for( float iter=0;iter<iters; ++iter)
      {
         data[0] += 1.0f; data[1] += 2.0f; data[2] += 4.0f; data[3] += 8.0f;    // Just make sure data changes
         test_vec4.set(data);
         test_vec3.set(data);
         test_vec2.set(data);
         test_vec1.set(data);

         use_value = use_value + test_vec4[3] + test_vec3[2] + test_vec2[1] + test_vec1[0];
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/SetPtrOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }


   void testGetData()
   {
      float* data;
      gmtl::Vec<float, 4> test_vec4(1.0f, 2.0f, 3.0f, 4.0f);
      data = test_vec4.getData();
      CPPUNIT_ASSERT( data[0] == 1.0f);
      CPPUNIT_ASSERT( data[1] == 2.0f);
      CPPUNIT_ASSERT( data[2] == 3.0f);
      CPPUNIT_ASSERT( data[3] == 4.0f);

      gmtl::Vec<float, 3> test_vec3(1.0f, 2.0f, 3.0f);
      data = test_vec3.getData();
      CPPUNIT_ASSERT( data[0] == 1.0f);
      CPPUNIT_ASSERT( data[1] == 2.0f);
      CPPUNIT_ASSERT( data[2] == 3.0f);

      gmtl::Vec<float, 2> test_vec2(1.0f, 2.0f);
      data = test_vec2.getData();
      CPPUNIT_ASSERT( data[0] == 1.0f);
      CPPUNIT_ASSERT( data[1] == 2.0f);
   }


   // -- Test comparison -- //
   void testEqualityCompare()
   {
      gmtl::Vec<float, 4> test_vec1(1.0f, 2.0f, 3.0f, 4.0f);
      gmtl::Vec<float, 4> test_vec2(test_vec1);

      CPPUNIT_ASSERT( test_vec1 == test_vec2);
      CPPUNIT_ASSERT(! (test_vec1 != test_vec2));

      test_vec2 = test_vec1;     // Set equal, vary elt 0
      test_vec2[0] = 21.10f;
      CPPUNIT_ASSERT( test_vec1 != test_vec2);
      CPPUNIT_ASSERT(! (test_vec1 == test_vec2));

      test_vec2 = test_vec1;     // Set equal, vary elt 0
      test_vec2[1] = 21.10f;
      CPPUNIT_ASSERT( test_vec1 != test_vec2);
      CPPUNIT_ASSERT(! (test_vec1 == test_vec2));

      test_vec2 = test_vec1;     // Set equal, vary elt 0
      test_vec2[2] = 21.10f;
      CPPUNIT_ASSERT( test_vec1 != test_vec2);
      CPPUNIT_ASSERT(! (test_vec1 == test_vec2));

      test_vec2 = test_vec1;     // Set equal, vary elt 0
      test_vec2[3] = 21.10f;
      CPPUNIT_ASSERT( test_vec1 != test_vec2);
      CPPUNIT_ASSERT(! (test_vec1 == test_vec2));

      // Test comparison performance
      // Test constructor
      const float iters(400000);
      unsigned true_count(0);
      unsigned false_count(0);
      
      // -- Equality
      CPPUNIT_METRIC_START_TIMING();
      test_vec1.set(0.0f, 0.0f, 0.0f, 2000.0f);
      test_vec2.set(0.0f, 0.0f, 0.0f, 1000.0f);
      
      for( float iter=0;iter<iters; ++iter)
      {
         test_vec1[3] += 1.0f;
         test_vec2[3] += 2.0f;
         if(test_vec1 == test_vec2)
            true_count++;         
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/EqualityCompareOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

      // -- Inequality
      CPPUNIT_METRIC_START_TIMING();
      test_vec1.set(0.0f, 0.0f, 0.0f, 2000.0f);
      test_vec2.set(0.0f, 0.0f, 0.0f, 1000.0f);
      
      for( float iter=0;iter<iters; ++iter)
      {
         test_vec1[3] += 1.0f;
         test_vec2[3] += 2.0f;
         if(test_vec1 != test_vec2)
            false_count++;
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/InequalityCompareOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

   }

   // -- Test comparison -- //
   void testIsEqual()
   {
      gmtl::Vec<float, 4> test_vec1(1.0f, 2.0f, 3.0f, 4.0f);
      gmtl::Vec<float, 4> test_vec2(test_vec1);
      float eps(0.0f);

      for(eps=0.0f;eps<10.0f;eps += 0.05f)
      {
         CPPUNIT_ASSERT( gmtl::isEqual(test_vec1, test_vec2, eps) );                     
      }

      test_vec1.set(1.0f, 1.0f, 1.0f, 1.0f);
      for(unsigned elt=0; elt<4; elt++)
      {
         test_vec2 = test_vec1;     // Set equal, vary elt 0
         test_vec2[elt] = 21.0f;
         CPPUNIT_ASSERT( !gmtl::isEqual(test_vec1, test_vec2, 10.0f) );
         CPPUNIT_ASSERT( !gmtl::isEqual(test_vec1, test_vec2, 19.9f) );
         CPPUNIT_ASSERT( gmtl::isEqual(test_vec1, test_vec2, 20.1f) );
         CPPUNIT_ASSERT( gmtl::isEqual(test_vec1, test_vec2, 22.0f) );
      }

      // Test comparison performance
      // Test constructor
      const float iters(400000);
      unsigned true_count(0);
      unsigned false_count(0);
      
      // -- Equality
      CPPUNIT_METRIC_START_TIMING();
      test_vec1.set(0.0f, 0.0f, 0.0f, 2000.0f);
      test_vec2.set(0.0f, 0.0f, 0.0f, 1000.0f);
      
      for( float iter=0;iter<iters; ++iter)
      {
         test_vec1[3] += 1.0f;
         test_vec2[3] += 2.0f;
         if(gmtl::isEqual(test_vec1, test_vec2, 1.0f) )
            true_count++;
         if(gmtl::isEqual(test_vec1, test_vec2, 0.1f) )
            true_count++;
         if(gmtl::isEqual(test_vec1, test_vec2, 100000.0f) )
            true_count++;
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/isEqualOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
      
   }

   void testOpPlusEq()
   {
      gmtl::Vec<float,3> test_vec1(1.0, 2.0, 3.0);
      gmtl::Vec<float,3> test_vec2(2.0, 2.0, 2.0);

      test_vec1 += test_vec2;
      CPPUNIT_ASSERT( test_vec1[0] == 3.0f &&
                      test_vec1[1] == 4.0f &&
                      test_vec1[2] == 5.0f );

      // -- test op+= performance
      const float iters(400000);
      CPPUNIT_METRIC_START_TIMING();
      gmtl::Vec<float,3> test_vec3(5.0, 7.0, 9.0);

      for( float iter=0;iter<iters; ++iter)
      {
         test_vec3.set(iter, iter+1, iter+2);
         test_vec1 += test_vec3;         
      }

      test_vec2 = test_vec1;

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/OpPlusEqOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testOpPlus()
   {
      gmtl::Vec<float,3> test_vec1(1.0, 2.0, 3.0);
      gmtl::Vec<float,3> test_vec2(2.0, 2.0, 2.0);
      gmtl::Vec<float,3> test_vec3(1.0, 2.0, 3.0);

      test_vec1 = test_vec3 + test_vec2;
      CPPUNIT_ASSERT( test_vec1[0] == 3.0f &&
                      test_vec1[1] == 4.0f &&
                      test_vec1[2] == 5.0f );

      // -- test op+ performance
      const float iters(400000);
      CPPUNIT_METRIC_START_TIMING();
      test_vec3.set(5.0, 7.0, 9.0);

      for( float iter=0;iter<iters; ++iter)
      {
         test_vec3.set(iter, iter+1, iter+2);
         test_vec1 = (test_vec3 + test_vec2);         
      }

      test_vec2 = test_vec1;

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/OpPlusOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testOpMinusEq()
   {
      gmtl::Vec<float,3> test_vec1(1.0, 2.0, 3.0);
      gmtl::Vec<float,3> test_vec2(2.0, 2.0, 2.0);

      test_vec1 -= test_vec2;
      CPPUNIT_ASSERT( test_vec1[0] == -1.0f &&
                      test_vec1[1] == 0.0f &&
                      test_vec1[2] == 1.0f );

      // -- test op-= performance
      const float iters(400000);
      CPPUNIT_METRIC_START_TIMING();
      gmtl::Vec<float,3> test_vec3(5.0, 7.0, 9.0);

      for( float iter=0;iter<iters; ++iter)
      {
         test_vec3.set(iter, iter+1, iter+2);
         test_vec1 -= test_vec3;         
      }

      test_vec2 = test_vec1;

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/OpMinusEqOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testOpMinus()
   {
      gmtl::Vec<float,3> test_vec1(1.0, 2.0, 3.0);
      gmtl::Vec<float,3> test_vec2(2.0, 2.0, 2.0);
      gmtl::Vec<float,3> test_vec3(1.0, 2.0, 3.0);

      test_vec1 = test_vec3 - test_vec2;
      CPPUNIT_ASSERT( test_vec1[0] == -1.0f &&
                      test_vec1[1] == 0.0f &&
                      test_vec1[2] == 1.0f );

      // -- test op- performance
      const float iters(400000);
      CPPUNIT_METRIC_START_TIMING();
      test_vec3.set(5.0, 7.0, 9.0);

      for( float iter=0;iter<iters; ++iter)
      {
         test_vec3.set(iter, iter+1, iter+2);
         test_vec1 = (test_vec3 - test_vec2);         
      }

      test_vec2 = test_vec1;

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/OpMinusOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testOpMultScalarEq()
   {
      gmtl::Vec<float,3> test_vec1(1.0, 2.0, 3.0);
         
      test_vec1 *= 4.0f;
      CPPUNIT_ASSERT( test_vec1[0] == 4.0f &&
                      test_vec1[1] == 8.0f &&
                      test_vec1[2] == 12.0f );

      // -- test op-= performance
      const float iters(400000);
      CPPUNIT_METRIC_START_TIMING();
            
      for( float iter=0;iter<iters; ++iter)
      {
         test_vec1 *= 1.05f;         
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/OpMultScalarEqOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testOpMultScalar()
   {
      gmtl::Vec<float,3> test_vec1(1.0, 2.0, 3.0);
      gmtl::Vec<float,3> test_vec2(2.0, 2.0, 2.0);
      gmtl::Vec<float,3> test_vec3(1.0, 2.0, 3.0);

      test_vec1 = test_vec3 * 4.0f;
      CPPUNIT_ASSERT( test_vec1[0] == 4.0f &&
                      test_vec1[1] == 8.0f &&
                      test_vec1[2] == 12.0f );

      // -- test op- performance
      const float iters(400000);
      CPPUNIT_METRIC_START_TIMING();
      test_vec3.set(5.0, 7.0, 9.0);

      for( float iter=0;iter<iters; ++iter)
      {
         test_vec1 = test_vec3 * 1.05f;
         test_vec3 = test_vec1;         
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/OpMultScalarOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testOpDivScalarEq()
   {
      gmtl::Vec<float,3> test_vec1(12.0, 8.0, 4.0);
         
      test_vec1 /= 4.0f;
      CPPUNIT_ASSERT( test_vec1[0] == 3.0f &&
                      test_vec1[1] == 2.0f &&
                      test_vec1[2] == 1.0f );

      // -- test op-= performance
      const float iters(400000);
      CPPUNIT_METRIC_START_TIMING();
            
      for( float iter=0;iter<iters; ++iter)
      {
         test_vec1 /= 0.95f;         
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/OpDivScalarEqOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testOpDivScalar()
   {
      gmtl::Vec<float,3> test_vec1(1.0, 2.0, 3.0);
      gmtl::Vec<float,3> test_vec2(2.0, 2.0, 2.0);
      gmtl::Vec<float,3> test_vec3(12.0, 8.0, 4.0);

      test_vec1 = test_vec3 / 4.0f;
      CPPUNIT_ASSERT( test_vec1[0] == 3.0f &&
                      test_vec1[1] == 2.0f &&
                      test_vec1[2] == 1.0f );

      // -- test op- performance
      const float iters(400000);
      CPPUNIT_METRIC_START_TIMING();
      test_vec3.set(5.0, 7.0, 9.0);

      for( float iter=0;iter<iters; ++iter)
      {
         test_vec1 = test_vec3 / 0.95f;
         test_vec3 = test_vec1;         
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/OpDivScalarOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }


   void testDot()
   {
      gmtl::Vec<float,3> v1(1,0,0);
      gmtl::Vec<float,3> v2(0,1,0);
      gmtl::Vec<float,3> v3(0,0,1);
      float dot, ans;

      // Base Vectors
      dot = gmtl::dot(v1,v2);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0f, gmtl::dot(v1,v2), 0.05);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0f, gmtl::dot(v1,v3), 0.05);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0f, gmtl::dot(v2,v3), 0.05);

      // Other Vectors
      v1.set(13.45, -7.8, 0.056);
      v2.set(0.777, 5.333,12.21);
      v3.set(3.4, -1.6, 0.23);

      ans = -30.463;
      dot = gmtl::dot(v1,v2);
      CPPUNIT_ASSERT(GMTL_NEAR(dot,ans,0.01));
      dot = gmtl::dot(v2,v1);
      CPPUNIT_ASSERT(GMTL_NEAR(dot,ans,0.01));
      ans = -3.0827;
      dot = gmtl::dot(v2,v3);
      CPPUNIT_ASSERT(GMTL_NEAR(dot,ans,0.01));
      dot = gmtl::dot(v3,v2);
      CPPUNIT_ASSERT(GMTL_NEAR(dot,ans,0.01));

      // -- test op- performance
      const float iters(100000);
      CPPUNIT_METRIC_START_TIMING();
      float val;      
      
      for( float iter=0;iter<iters; ++iter)
      {
         val += gmtl::dot(v1,v2);
         v1 *= 1.0025;                 // Make it grow a little         
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/DotOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }

   void testLength()
   {
      gmtl::Vec<float,3> v1(1,0,0);
      gmtl::Vec<float,3> v2(0,1,0);
      gmtl::Vec<float,3> v3(0,0,1);
      float len, ans;

      // Base Vectors
      CPPUNIT_ASSERT_DOUBLES_EQUAL(gmtl::length(v1),1.0f, 0.05f);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(gmtl::length(v2),1.0f, 0.05f);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(gmtl::length(v3),1.0f, 0.05f);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(gmtl::lengthSquared(v1), 1.0f, 0.05f);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(gmtl::lengthSquared(v2), 1.0f, 0.05f);
      CPPUNIT_ASSERT_DOUBLES_EQUAL(gmtl::lengthSquared(v3), 1.0f, 0.05f);

      // Other Vectors
      v1.set(2.0f, 4.0f, 5.0f);
      v2.set(12.0f, -2.0f, -4.0f);
            
      ans = 4.0f + 16.0f + 25.0f;
      len = gmtl::lengthSquared(v1);
      CPPUNIT_ASSERT(GMTL_NEAR(len,ans,0.01));

      ans = gmtl::Math::sqrt(ans);
      len = gmtl::length(v1);
      CPPUNIT_ASSERT(GMTL_NEAR(len,ans,0.01));
      
      ans = 144.0f + 4.0f + 16.0f;
      len = gmtl::lengthSquared(v2);
      CPPUNIT_ASSERT(GMTL_NEAR(len,ans,0.01));

      ans = gmtl::Math::sqrt(ans);
      len = gmtl::length(v2);
      CPPUNIT_ASSERT(GMTL_NEAR(len,ans,0.01));
            
      // -- test length performance
      const float iters(100000);
      float val(0.0f);      

      CPPUNIT_METRIC_START_TIMING();
      
      for( float iter=0;iter<iters; ++iter)
      {
         val += gmtl::length(v1);
         v1 *= 1.0025f;                 // Make it grow a little         
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/LengthOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%

      // -- test lengthSquared performance
      CPPUNIT_METRIC_START_TIMING();
      val = 0.0f;
      v1.set(1.0f, 2.0f, 4.0f);
      
      for( float iter=0;iter<iters; ++iter)
      {
         val += gmtl::lengthSquared(v1);
         v1 *= 1.0025f;                 // Make it grow a little         
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/LengthSquaredOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%
   }


   void testNormalize()
   {
      gmtl::Vec<float,3> v1(1,0,0);
      gmtl::Vec<float,3> v2(0,1,0);
      gmtl::Vec<float,3> v3(0,0,1);
            
      // Other Vectors
      v1.set(2.0f, 4.0f, 5.0f);
      v2 = v1;
      gmtl::normalize(v1);
      gmtl::Vec<float,3> temp = (v1*gmtl::length(v2));
      CPPUNIT_ASSERT( gmtl::isEqual(v2, temp, 0.01f ) );

      v1.set(12.0f, -2.0f, -4.0f);
      v2 = v1;
      gmtl::normalize(v1);
      CPPUNIT_ASSERT( gmtl::isEqual(v2, (v1*gmtl::length(v2)),0.01f ) );
            
      // -- test performance
      const float iters(100000);
      CPPUNIT_METRIC_START_TIMING();
      float val(0.0f);      
      v1.set(12.0f, 21.0f, 75.0f);

      for( float iter=0;iter<iters; ++iter)
      {
         v1 *= 1.0025f;                 // Make it grow a little
         v2 = v1;
         val += gmtl::normalize(v2);
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/NormalizeOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%      
   }


   void testCross()
   {
      gmtl::Vec<float,3> v1(1,0,0);
      gmtl::Vec<float,3> v2(0,1,0);
      gmtl::Vec<float,3> v3(0,0,1);
      gmtl::Vec<float,3> cross;

      // Base Vectors
      cross = gmtl::cross(v1,v2);
      CPPUNIT_ASSERT( gmtl::isEqual(cross, v3, 0.01f) );
      cross = gmtl::cross(v2,v1);
      CPPUNIT_ASSERT( gmtl::isEqual(cross, (v3 * -1.0f), 0.01f) );

      v1.set(13.45, -7.8, 0.056);
      v2.set(0.777, 5.333,12.21);
      v3.set(-95.537, -164.181, 77.789);

      cross = gmtl::cross(v1,v2);
      CPPUNIT_ASSERT( gmtl::isEqual(cross, v3, 0.01f));
      cross = gmtl::cross(v2, v1);
      CPPUNIT_ASSERT(gmtl::isEqual(cross, (v3 * -1.0f), 0.01f));

      // -- test performance
      const float iters(100000);
      CPPUNIT_METRIC_START_TIMING();
      
      v1.set(12.0f, 21.0f, 75.0f);

      for( float iter=0;iter<iters; ++iter)
      {
         cross = gmtl::cross(v2,v1);
         v1 *= 1.0025f;
         v3 += cross;         
      }

      CPPUNIT_METRIC_STOP_TIMING();
      CPPUNIT_ASSERT_METRIC_TIMING_LE("VecTest/CrossOverhead", iters, 0.075f, 0.1f);  // warn at 7.5%, error at 10%      
   }


   static Test* suite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("VecTest");
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testCreation", &VecTest::testCreation));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testCopyConstruct", &VecTest::testCopyConstruct));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testConstructors", &VecTest::testConstructors));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testSet", &VecTest::testSet));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testSetPtr", &VecTest::testSetPtr));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testGetData", &VecTest::testGetData));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testEqualityCompare", &VecTest::testEqualityCompare));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testIsEqual", &VecTest::testIsEqual));

      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testOpPlusEq", &VecTest::testOpPlusEq));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testOpPlus", &VecTest::testOpPlus));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testOpMinusEq", &VecTest::testOpMinusEq));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testOpMinus", &VecTest::testOpMinus));
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testOpMultScalarEq", &VecTest::testOpMultScalarEq)); 

      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testOpMultScalar", &VecTest::testOpMultScalar)); 
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testOpDivScalarEq", &VecTest::testOpDivScalarEq)); 
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testOpDivScalar", &VecTest::testOpDivScalar)); 
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testDot", &VecTest::testDot)); 
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testLength", &VecTest::testLength)); 
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testNormalize", &VecTest::testNormalize)); 
      test_suite->addTest( new CppUnit::TestCaller<VecTest>("testCross", &VecTest::testCross)); 

      
      return test_suite;
   }

   static Test* interactiveSuite()
   {
      CppUnit::TestSuite* test_suite = new CppUnit::TestSuite ("InteractiveVecTest");
      //test_suite->addTest( new CppUnit::TestCaller<ThreadTest>("interactiveCPUGrind", &ThreadTest::interactiveTestCPUGrind));
      return test_suite;
   }

protected:

};

};
