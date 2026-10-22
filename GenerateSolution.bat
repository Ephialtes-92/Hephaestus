@echo off
REM GenerateSolution.bat - Automatically generate the Visual Studio solution using CMake

setlocal enabledelayedexpansion

echo.
echo ========================================
echo Hephaestus Solution Generator
echo ========================================
echo.

REM Check if CMake is installed
cmake --version >nul 2>&1
if errorlevel 1 (
	echo [ERROR] CMake is not installed or not in PATH
	echo Please install CMake from: https://cmake.org/download/
	pause
	exit /b 1
)

REM Get the directory where this batch file is located
set SCRIPT_DIR=%~dp0
cd /d "%SCRIPT_DIR%"

echo [INFO] Project directory: %SCRIPT_DIR%

REM Create build directory if it doesn't exist
if not exist "build" (
	echo [INFO] Creating build directory...
	mkdir build
) else (
	echo [INFO] Build directory already exists
)

REM Navigate to build directory
cd /d "%SCRIPT_DIR%build"

REM Generate the Visual Studio solution
echo [INFO] Generating Visual Studio solution using CMake...
echo.

cmake .. -G "Visual Studio 18 2026"

if errorlevel 1 (
	echo.
	echo [ERROR] CMake configuration failed!
	pause
	exit /b 1
)

echo.
echo ========================================
echo [SUCCESS] Solution generated successfully!
echo ========================================
echo.
echo Solution file location: %SCRIPT_DIR%build\Hephaestus.slnx
echo.

REM Ask if user wants to open the solution
set /p OPEN_SOLUTION="Do you want to open the solution in Visual Studio? (y/n): "
if /i "%OPEN_SOLUTION%"=="y" (
	echo [INFO] Opening solution in Visual Studio...
	start "" "%SCRIPT_DIR%build\Hephaestus.slnx"
)

echo.
pause
