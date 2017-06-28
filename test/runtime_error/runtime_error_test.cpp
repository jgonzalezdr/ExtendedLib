/**
 * @file
 * @brief      unit tests for the "runtime_error" class
 * @project    ExtendedLib
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) Jesus Gonzalez. All rights reserved.
 * @license    See LICENSE.txt
 */

/*===========================================================================
 *                              INCLUDES
 *===========================================================================*/

#include "ExtendedLib/runtime_error.h"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*===========================================================================
 *                      COMMON TEST DEFINES & MACROS
 *===========================================================================*/

/*===========================================================================
 *                          TEST GROUP DEFINITION
 *===========================================================================*/

TEST_GROUP( runtime_error )
{
};

/*===========================================================================
 *                    TEST CASES IMPLEMENTATION
 *===========================================================================*/

/*
 * Check that a runtime exception is thrown but not logged immediately
 */
TEST( runtime_error, Throw_NoLog )
{
    // Prepare

    try
    {
        // Exercise
        THROW_ERROR( "TEST_ERR" );

        // Verify
        FAIL( "Expected exception not thrown" );
    }
    catch( ext::runtime_error &e )
    {
        // Verify
        STRCMP_EQUAL( "", e.get_category().c_str() );
        STRCMP_EQUAL( __PRETTY_FUNCTION__, e.get_function().c_str() );
        STRCMP_EQUAL( "TEST_ERR", e.get_message().c_str() );
        STRCMP_EQUAL( "TEST_ERR", e.what() );
    }

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that a runtime exception is thrown and logged immediately
 */
TEST( runtime_error, Throw_Log )
{
    // Prepare
    mock().expectOneCall( "::OutputDebugString" ).withParameter( "lpOutputString", "[ERROR] {ExtendedLib.Test.runtime_error.exe} "
                                                                 "<TEST_runtime_error_Throw_Log_Test::testBody> TEST_ERR\n" );

    try
    {
        // Exercise
        THROW_ERROR_LOG( "TEST_ERR" );

        // Verify
        FAIL( "Expected exception not thrown" );
    }
    catch( ext::runtime_error &e )
    {
        // Verify
        STRCMP_EQUAL( "", e.get_category().c_str() );
        STRCMP_EQUAL( __PRETTY_FUNCTION__, e.get_function().c_str() );
        STRCMP_EQUAL( "TEST_ERR", e.get_message().c_str() );
        STRCMP_EQUAL( "TEST_ERR", e.what() );
    }

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check runtime exception construction with delayed logging
 */
TEST( runtime_error, Constructor_DelayedLog )
{
    // Prepare

    // Exercise
    ext::runtime_error e( "TEST_CAT", "TEST_FUN", false, std::string("TEST_ERR") );

    // Verify
    STRCMP_EQUAL( "TEST_CAT", e.get_category().c_str() );
    STRCMP_EQUAL( "TEST_FUN", e.get_function().c_str() );
    STRCMP_EQUAL( "TEST_ERR", e.get_message().c_str() );
    STRCMP_EQUAL( "TEST_ERR", e.what() );
    mock().checkExpectations();
    mock().clear();

    // Prepare
    mock().expectOneCall( "::OutputDebugString" ).withParameter( "lpOutputString", "[ERROR] {ExtendedLib.Test.runtime_error.exe:TEST_CAT} "
                                                                 "<TEST_FUN> TEST_ERR\n" );

    // Exercise
    e.log();

    // Verify
    mock().checkExpectations();
    mock().clear();

    // Exercise
    e.log();

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check runtime exception construction with immediate logging
 */
TEST( runtime_error, Constructor_ImmediateLog )
{
    // Prepare
    mock().expectOneCall( "::OutputDebugString" ).withParameter( "lpOutputString", "[ERROR] {ExtendedLib.Test.runtime_error.exe:TEST_CAT} "
                                                                 "<TEST_FUN> TEST_ERR\n" );

    // Exercise
    ext::runtime_error e( "TEST_CAT", "TEST_FUN", true, std::string("TEST_ERR") );

    // Verify
    STRCMP_EQUAL( "TEST_CAT", e.get_category().c_str() );
    STRCMP_EQUAL( "TEST_FUN", e.get_function().c_str() );
    STRCMP_EQUAL( "TEST_ERR", e.get_message().c_str() );
    STRCMP_EQUAL( "TEST_ERR", e.what() );
    mock().checkExpectations();
    mock().clear();

    // Exercise
    e.log();

    // Verify
    mock().checkExpectations();
    mock().clear();

    // Cleanup
}
