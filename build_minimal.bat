@echo off
echo Building Minimal Ant Colony Simulator...

REM Set up Visual Studio environment
if exist "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
) else if exist "C:\Program Files(x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" (
    call "C:\Program Files(x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
) else (
    echo Error: Could not find Visual Studio 2022 installation.
    pause
    exit /b 1
)

REM Create output directory
if not exist "bin" mkdir bin

REM Compile only essential files
echo Compiling essential files...
cl /Fe:bin\AntColonySimulator.exe ^
   src\main.c ^
   src\world.c ^
   src\ant_logic.c ^
   src\pheromones.c ^
   src\visualization.c ^
   src\file_io.c ^
   src\algorithms.c ^
   src\utils.c ^
   /I:src ^
   /std:c11 ^
   /link user32.lib ^
   /W3 ^
   /D_DEBUG ^
   /Zi

if %errorlevel% equ 0 (
    echo.
    echo Build successful! Executable created: bin\AntColonySimulator.exe
) else (
    echo.
    echo Build failed! Please check the error messages above.
)

pause
