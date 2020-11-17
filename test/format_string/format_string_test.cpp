/**
 * @file
 * @brief      unit tests for the "format_string" module
 * @project    ExtendedLib
 * @authors    Jesus Gonzalez <jgonzalez@gdr-sistemas.com>
 * @copyright  Copyright (c) Jesus Gonzalez. All rights reserved.
 * @license    See LICENSE.txt
 */

/*===========================================================================
 *                              INCLUDES
 *===========================================================================*/

#include "Extended/format_string.hpp"

#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

/*===========================================================================
 *                      COMMON TEST DEFINES & MACROS
 *===========================================================================*/

/*===========================================================================
 *                          TEST GROUP DEFINITION
 *===========================================================================*/

TEST_GROUP( format_string )
{
};

/*===========================================================================
 *                    TEST CASES IMPLEMENTATION
 *===========================================================================*/

/*
 * Check that a string is formatted properly
 */
TEST( format_string, format )
{
    // Prepare

    // Exercise
    std::string txt = ext::format( "TEST %d %u %.3f %s %c", -1, 5000, 4.2, "STR", 'X' );

    // Verify
    STRCMP_EQUAL( "TEST -1 5000 4.200 STR X", txt.c_str() );

    // Cleanup
}

/*
 * Check that a large string is formatted properly
 */
TEST( format_string, format_large )
{
    // Prepare
    std::string a( 800, 'A' );
    std::string b( 800, 'B' );

    // Exercise
    std::string txt = ext::format( "%s_X_%s", a.c_str(), b.c_str() );

    // Verify
    CHECK_EQUAL( 1603, txt.length() );
    STRCMP_EQUAL( a.c_str(), txt.substr(0, 800).c_str() );
    STRCMP_EQUAL( "_X_", txt.substr(800, 3).c_str() );
    STRCMP_EQUAL( b.c_str(), txt.substr(803).c_str() );

    // Cleanup
}

/*
 * Check that a hex dump is formatted properly
 */
TEST( format_string, format_hex_1_NoIdent_NoSeparator_Line8 )
{
    // Prepare
    std::vector<uint8_t> data( { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0xFF } );

    // Exercise
    std::string txt = ext::format_hex( data, 0, 0, 8 );

    // Verify
    STRCMP_EQUAL( "0001020304050607\n10203040506070FF", txt.c_str() );

    // Cleanup
}

/*
 * Check that a hex dump is formatted properly
 */
TEST( format_string, format_hex_1_Ident3_Separator2_Line8 )
{
    // Prepare
    std::vector<uint8_t> data( { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0xFF } );

    // Exercise
    std::string txt = ext::format_hex( data, 3, 2, 8 );

    // Verify
    STRCMP_EQUAL( "   00  01  02  03  04  05  06  07\n"
                  "   10  20  30  40  50  60  70  FF", txt.c_str() );

    // Cleanup
}

/*
 * Check that a hex dump is formatted properly
 */
TEST( format_string, format_hex_1_NoIdent_Separator1_Line16 )
{
    // Prepare
    std::vector<uint8_t> data( { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0xFF } );

    // Exercise
    std::string txt = ext::format_hex( data, 0, 1, 16 );

    // Verify
    STRCMP_EQUAL( "00 01 02 03 04 05 06 07 10 20 30 40 50 60 70 FF", txt.c_str() );

    // Cleanup
}

/*
 * Check that a hex dump is formatted properly
 */
TEST( format_string, format_hex_1_Ident4_Separator1_Line7 )
{
    // Prepare
    std::vector<uint8_t> data( { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0xFF } );

    // Exercise
    std::string txt = ext::format_hex( data, 4, 1, 7 );

    // Verify
    STRCMP_EQUAL( "    00 01 02 03 04 05 06\n"
                  "    07 10 20 30 40 50 60\n"
                  "    70 FF", txt.c_str() );

    // Cleanup
}

/*
 * Check that a hex dump is formatted properly
 */
TEST( format_string, format_hex_2_EmptyIdent_EmptySeparator_Line8 )
{
    // Prepare
    std::vector<uint8_t> data( { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0xFF } );

    // Exercise
    std::string txt = ext::format_hex( data, "", "", 8 );

    // Verify
    STRCMP_EQUAL( "0001020304050607\n10203040506070FF", txt.c_str() );

    // Cleanup
}

/*
 * Check that a hex dump is formatted properly
 */
TEST( format_string, format_hex_2_Ident3_Separator1_Line8 )
{
    // Prepare
    std::vector<uint8_t> data( { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0xFF } );

    // Exercise
    std::string txt = ext::format_hex( data, " > ", "-", 8 );

    // Verify
    STRCMP_EQUAL( " > 00-01-02-03-04-05-06-07\n"
                  " > 10-20-30-40-50-60-70-FF", txt.c_str() );

    // Cleanup
}

/*
 * Check that a hex dump is formatted properly
 */
TEST( format_string, format_hex_1_Ident2_Separator1_Line16 )
{
    // Prepare
    std::vector<uint8_t> data( { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0xFF } );

    // Exercise
    std::string txt = ext::format_hex( data, "# ", " ", 16 );

    // Verify
    STRCMP_EQUAL( "# 00 01 02 03 04 05 06 07 10 20 30 40 50 60 70 FF", txt.c_str() );

    // Cleanup
}

/*
 * Check that a hex dump is formatted properly
 */
TEST( format_string, format_hex_2_Ident4_Separator1_Line7 )
{
    // Prepare
    std::vector<uint8_t> data( { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0xFF } );

    // Exercise
    std::string txt = ext::format_hex( data, "*** ", "_", 7 );

    // Verify
    STRCMP_EQUAL( "*** 00_01_02_03_04_05_06\n"
                  "*** 07_10_20_30_40_50_60\n"
                  "*** 70_FF", txt.c_str() );

    // Cleanup
}
