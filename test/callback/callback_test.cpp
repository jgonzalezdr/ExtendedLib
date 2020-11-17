/**
 * @file
 * @brief      unit tests for the "callback" class
 * @project    ExtendedLib
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) Jesus Gonzalez. All rights reserved.
 * @license    See LICENSE.txt
 */

/*===========================================================================
 *                              INCLUDES
 *===========================================================================*/

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "Extended/callback.hpp"

/*===========================================================================
 *                      COMMON TEST DEFINES & MACROS
 *===========================================================================*/

class TestClass
{
public:
    void TestCallback( int p )
    {
        mock().actualCall( "TestClass::TestCallback" ).onObject( this ).withParameter( "p", p );
    }
};

/*===========================================================================
 *                          TEST GROUP DEFINITION
 *===========================================================================*/

TEST_GROUP( method_callback )
{
};

/*===========================================================================
 *                    TEST CASES IMPLEMENTATION
 *===========================================================================*/

/*
 * Check that the callback is created properly and the target method is not invoked.
 */
TEST( method_callback, Creation )
{
    // Prepare
    TestClass obj;

    // Exercise
    ext::callback<int>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );

    // Verify
    CHECK_TRUE( callback );
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check proper invocation of the target method
 */
TEST( method_callback, Invocation )
{
    // Prepare
    const int value = 3546743;
    TestClass obj;
    ext::callback<int>::XPtr callback = ext::new_callback( &obj, &TestClass::TestCallback );
    mock().expectOneCall("TestClass::TestCallback").onObject( &obj ).withParameter( "p", value );

    // Exercise
    (*callback)( value );

    // Verify
    mock().checkExpectations();

    // Cleanup
}
