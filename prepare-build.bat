@echo off
setlocal ENABLEEXTENSIONS ENABLEDELAYEDEXPANSION

::
:: Toolchain selection
::

echo.
echo Select toolchain:
echo 1^) MinGW Makefiles
echo 2^) Eclipse + MinGW Makefiles
echo 3^) Visual Studio 10 ^(2010^)
echo 4^) Visual Studio 11 ^(2012^)
echo 5^) Visual Studio 12 ^(2013^)
echo 6^) Visual Studio 14 ^(2015^)
choice /c 123456 /n /m ">"

set TOOLCHAIN=%ERRORLEVEL%
 
if "%TOOLCHAIN%" == "1" (
    set CMAKE_GEN="MinGW Makefiles"
    set BUILD_DIR="MinGW"
) else if "%TOOLCHAIN%" == "2" (
    set CMAKE_GEN="Eclipse CDT4 - MinGW Makefiles"
    set BUILD_DIR="Eclipse-MinGW"
) else if "%TOOLCHAIN%" == "3" (
    set CMAKE_GEN="Visual Studio 10"
    set BUILD_DIR="VS10"
) else if "%TOOLCHAIN%" == "4" (
    set CMAKE_GEN="Visual Studio 11"
    set BUILD_DIR="VS11"
) else if "%TOOLCHAIN%" == "5" (
    set CMAKE_GEN="Visual Studio 12"
    set BUILD_DIR="VS12"
) else if "%TOOLCHAIN%" == "6" (
    set CMAKE_GEN="Visual Studio 14"
    set BUILD_DIR="VS14"
) else (
    echo ERROR: Invalid toolchain!
    exit /b 1
)

if %TOOLCHAIN% leq 2 (
    set CMAKE_OPTS=!CMAKE_OPTS! -DCMAKE_CXX_COMPILER_ARG1=-std=c++11
)

::
:: Build type selection
::

echo.
echo Select build type:
echo 1^) Shared library ^(DLL^)
echo 2^) Static library
choice /c 12 /n /m ">"

set LIBRARY_TYPE=%ERRORLEVEL%
    
if "%LIBRARY_TYPE%" == "1" (
    set BUILD_DIR_EXT="dll"
    set CMAKE_OPTS=!CMAKE_OPTS! -DBUILD_SHARED_LIBS=YES
) else if "%LIBRARY_TYPE%" == "2" (
    set BUILD_DIR_EXT="static"
    set CMAKE_OPTS=!CMAKE_OPTS! -DBUILD_SHARED_LIBS=NO
) else (
    echo ERROR: Invalid build type!
    exit /b 1
)

::
:: Release type selection
::

echo.
echo Select release type:
echo 1^) Release
echo 2^) Debug

if %TOOLCHAIN% leq 2 (
    echo 3^) Coverage
    choice /c 123 /n /m ">"

    set RELEASE_TYPE=!ERRORLEVEL!
) else (
    choice /c 12 /n /m ">"

    set RELEASE_TYPE=!ERRORLEVEL!
)

if "%RELEASE_TYPE%" == "1" (
    set CMAKE_OPTS=!CMAKE_OPTS! -DCOVERAGE=OFF -DCMAKE_BUILD_TYPE=Release
    set BUILD_DIR_EXT=Release_!BUILD_DIR_EXT!
) else if "%RELEASE_TYPE%" == "2" (
    set CMAKE_OPTS=!CMAKE_OPTS! -DCOVERAGE=OFF -DCMAKE_BUILD_TYPE=Debug
    set BUILD_DIR_EXT=Debug_!BUILD_DIR_EXT!
) else if "%RELEASE_TYPE%" == "3" (
    set CMAKE_OPTS=!CMAKE_OPTS! -DCOVERAGE=ON -DCMAKE_BUILD_TYPE=Coverage
    set BUILD_DIR_EXT=Coverage_!BUILD_DIR_EXT!
) else (
    echo ERROR: Invalid release type!
    exit /b 1
)

::
:: Eclipse source project generation selection
::

if "%TOOLCHAIN%" == "2" (
    echo.
    echo Generate Eclipse source project?
    echo 1^) Yes
    echo 2^) No
    choice /c 12 /n /m ">"

    set ECLIPSE_SRC_PRJ=!ERRORLEVEL!

    if "!ECLIPSE_SRC_PRJ!" == "1" (
        set CMAKE_OPTS=!CMAKE_OPTS! -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=YES
    ) else (
        set CMAKE_OPTS=!CMAKE_OPTS! -DCMAKE_ECLIPSE_GENERATE_SOURCE_PROJECT=NO
    )
)

::
:: Documentation generation selection
::

echo.
echo Generate documentation?
echo 1^) Yes
echo 2^) No
choice /c 12 /n /m ">"

set GENERATE_DOC=%ERRORLEVEL%

if "%GENERATE_DOC%" == "1" (
    echo.
    echo Generate PDF documentation?
    echo 1^) Yes
    echo 2^) No
    choice /c 12 /n /m ">"

    set GENERATE_PDF=!ERRORLEVEL!

    if "!GENERATE_PDF!" == "1" (
        set CMAKE_OPTS=!CMAKE_OPTS! -DPDF_DOCS=ON
    ) else (
        set CMAKE_OPTS=!CMAKE_OPTS! -DPDF_DOCS=OFF
    )

    echo.
    echo Generate CHM documentation?
    echo 1^) Yes
    echo 2^) No
    choice /c 12 /n /m ">"

    set GENERATE_CHM=!ERRORLEVEL!

    if "!GENERATE_CHM!" == "1" (
        set CMAKE_OPTS=!CMAKE_OPTS! -DCHM_DOCS=ON
    ) else (
        set CMAKE_OPTS=!CMAKE_OPTS! -DCHM_DOCS=OFF
    )

    echo.
    echo Generate HTML documentation?
    echo 1^) Yes
    echo 2^) No
    choice /c 12 /n /m ">"

    set GENERATE_HTML=!ERRORLEVEL!

    if "!GENERATE_HTML!" == "1" (
        set CMAKE_OPTS=!CMAKE_OPTS! -DHTML_DOCS=ON
    ) else (
        set CMAKE_OPTS=!CMAKE_OPTS! -DHTML_DOCS=OFF
    )
)

::
:: Create directory and execute cmake
::

set BUILD_DIR=build_%BUILD_DIR%_%BUILD_DIR_EXT%

echo.

md %BUILD_DIR% 2>NUL
cd %BUILD_DIR%

cmake .. -G %CMAKE_GEN% -Wno-dev %CMAKE_OPTS%

pause
