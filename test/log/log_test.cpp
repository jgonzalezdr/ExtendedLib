/**
 * @file
 * @brief      unit tests for the "log" class
 * @project    ExtendedLib
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) Jesus Gonzalez. All rights reserved.
 * @license    See LICENSE.txt
 */

/*===========================================================================
 *                              INCLUDES
 *===========================================================================*/

#include "Extended/log.hpp"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*===========================================================================
 *                      COMMON TEST DEFINES & MACROS
 *===========================================================================*/

class TestLogHandler : public ext::log_handler
{
public:
    virtual ~TestLogHandler() {}

    virtual bool process( int prio, const char* category, const char* function, const char* msg )
    {
        return mock().actualCall( "TestLogHandler::process" ).onObject( this ).withParameter( "prio", prio )
                     .withParameter( "category", category ).withParameter( "function", function )
                     .withParameter( "msg", msg ).returnBoolValue();
    }
};

/*===========================================================================
 *                          TEST GROUP DEFINITION
 *===========================================================================*/

TEST_GROUP( log )
{
};

/*===========================================================================
 *                    TEST CASES IMPLEMENTATION
 *===========================================================================*/

/*
 * Check that the priority setting is respected.
 */
TEST( log, PrioritySetting )
{
    for( int i = LOG_PRIORITY_ERROR; i <= LOG_PRIORITY_ALLOC; i++ )
    {
        // Prepare

        // Exercise
        ext::log::set_priority_limit( i );

        // Verify
        CHECK_EQUAL( i, ext::log::get_priority_limit() );

        for( int j = LOG_PRIORITY_ERROR; j <= (LOG_PRIORITY_ALLOC + 1); j++ )
        {
            // Prepare
            static const char *prio[] = { "", "[ERROR]", "[WARN]", "[INFO]", "[DEBUG]", "[TRACE]", "[XTDBG]", "[ALLOC]", "[UNKNOWN]" };

            SimpleString expected_msg = StringFromFormat( "%s {ExtendedLib.Test.log.exe} <TEST_FUNC> TEST_MSG\n", prio[j] );

            if( j <= i )
            {
                mock().expectOneCall( "::OutputDebugString" ).withParameter( "lpOutputString", expected_msg.asCharString() );
            }

            // Exercise
            ext::log::log_message( j, NULL, "TEST_FUNC", "TEST_MSG" );

            // Verify
            mock().checkExpectations();

            // Cleanup
            mock().clear();
        }

    }

    // Cleanup
    ext::log::set_priority_limit( LOG_PRIORITY_MAX );
}

#ifdef __GNUC__

/*
 * Check that C++ functions are simplified to only the name.
 */
TEST( log, FunctionSimplification )
{
    // Prepare
    mock().expectOneCall( "::OutputDebugString" ).withParameter( "lpOutputString", "[ERROR] {ExtendedLib.Test.log.exe} <TEST_FUNC> TEST_MSG\n" );

    // Exercise
    ext::log::log_message( LOG_PRIORITY_ERROR, NULL, "void TEST_FUNC(int i)", "TEST_MSG" );

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that C++ constructors are simplified to only the name.
 */
TEST( log, FunctionSimplificationConstructor )
{
    // Prepare
    mock().expectOneCall( "::OutputDebugString" ).withParameter( "lpOutputString", "[ERROR] {ExtendedLib.Test.log.exe} <TEST_CTOR> TEST_MSG\n" );

    // Exercise
    ext::log::log_message( LOG_PRIORITY_ERROR, NULL, "TEST_CTOR(int i)", "TEST_MSG" );

    // Verify
    mock().checkExpectations();

    // Cleanup
}

#endif // __GNUC__

/*
 * Check that category is logged when present.
 */
TEST( log, WithCategory )
{
    // Prepare
    mock().expectOneCall( "::OutputDebugString" ).withParameter( "lpOutputString", "[ERROR] {ExtendedLib.Test.log.exe:TEST_CAT} <TEST_FUNC> TEST_MSG\n" );

    // Exercise
    ext::log::log_message( LOG_PRIORITY_ERROR, "TEST_CAT", "TEST_FUNC", "TEST_MSG" );

    // Verify
    mock().checkExpectations();

    // Cleanup
}

/*
 * Check that log handlers are called properly and that console output is suppressed when requested.
 */
TEST( log, WithLogHandler_NoConsoleOutput )
{
    // Prepare
    std::shared_ptr<TestLogHandler> testLogHandler = std::make_shared<TestLogHandler>();

    mock().expectOneCall( "TestLogHandler::process" ).onObject( testLogHandler.get() ).withParameter( "prio", LOG_PRIORITY_ERROR )
                         .withParameter( "category", "TEST_CAT" ).withParameter( "function", "TEST_FUNC" )
                         .withParameter( "msg", "TEST_MSG" ).andReturnValue( false );

    // Exercise
    ext::log::set_log_handler( testLogHandler );

    // Verify
    CHECK_EQUAL( testLogHandler.get(), ext::log::get_log_handler().get() );

    // Exercise
    ext::log::log_message( LOG_PRIORITY_ERROR, "TEST_CAT", "TEST_FUNC", "TEST_MSG" );

    // Verify
    mock().checkExpectations();

    // Cleanup
    ext::log::set_log_handler( NULL );
    testLogHandler.reset();
}

/*
 * Check that log handlers are called properly and that console output is not suppressed when requested.
 */
TEST( log, WithLogHandler_ConsoleOutput )
{
    // Prepare
    std::shared_ptr<TestLogHandler> testLogHandler = std::make_shared<TestLogHandler>();

    mock().expectOneCall( "TestLogHandler::process" ).onObject( testLogHandler.get() ).withParameter( "prio", LOG_PRIORITY_ERROR )
                         .withParameter( "category", "TEST_CAT" ).withParameter( "function", "TEST_FUNC" )
                         .withParameter( "msg", "TEST_MSG" ).andReturnValue( true );
    mock().expectOneCall( "::OutputDebugString" ).withParameter( "lpOutputString", "[ERROR] {ExtendedLib.Test.log.exe:TEST_CAT} <TEST_FUNC> TEST_MSG\n" );

    // Exercise
    ext::log::set_log_handler( testLogHandler );

    // Verify
    CHECK_EQUAL( testLogHandler.get(), ext::log::get_log_handler().get() );

    // Exercise
    ext::log::log_message( LOG_PRIORITY_ERROR, "TEST_CAT", "TEST_FUNC", "TEST_MSG" );

    // Verify
    mock().checkExpectations();

    // Cleanup
    ext::log::set_log_handler( NULL );
    testLogHandler.reset();
}
