@echo off

:: Get an ESC characters
setlocal
call :setESC

:: Build System commands handling
set "command=%~1"
if "%command%"=="gensln" goto :%command% 
if "%command%"=="opensln" goto :%command%
if "%command%"=="clean" goto :%command%
if "%command%"=="" goto :emptyCommand
if "%command%"=="--help" goto :help
goto :notRecognizedCommand

:: Help menu 
:help
echo %ESC%[36m-----------------------------%ESC%[0m
echo %ESC%[36mEON build script.%ESC%[0m
echo %ESC%[36m-----------------------------%ESC%[0m
echo.
echo %ESC%[36m^>%ESC%[0m Usage: eon [command]
echo.
echo COMMANDS
echo.
echo %ESC%[36m-%ESC%[0m gensln           Generates and configures your Visual Studio solution.
echo %ESC%[36m-%ESC%[0m opensln          Opens your Visual Studio solution.
echo %ESC%[36m-%ESC%[0m clean            Cleans compiled data from the build directory.
goto :eof

:: Warning message thrown due an empty command
:emptyCommand
echo %ESC%[33mNo command was entered. Type eon --help for help. %ESC%[0m
goto :eof

:: Error message thrown due an unrecognized command
:notRecognizedCommand
echo %ESC%[31m[ERROR]: Not such an EON command. Type --help for help. %ESC%[0m
goto :eof

:: Generates the solution files
:gensln
pushd ..\
echo.
call vendor\premake\premake5.exe vs2019
echo %ESC%[32m[SUCCESS]: Solution successfully generated! %ESC%[0m
popd
goto :eof

:: Opens your solution
:opensln
pushd ..\
start generated\OpenGL-Course.sln
popd
goto :eof

:: Deteles your build files
:clean
pushd ..\

if not exist build\ goto :buildDirNotFound

setlocal
set count=0
for %%x in (build) do set /a count+=1
if %count% == 0 goto :noBuildFiles

del "build\*." /s /f /q
echo %ESC%[32m[SUCCESS]: Build files succesfully deleted. %ESC%[0m
goto :deleteAutoGenerated

:buildDirNotFound
echo %ESC%[33m[WARNING]: Couldn't find any build directory. No file has been deleted. %ESC%[0m
goto :eof

:noBuildFiles
echo %ESC%[33m[WARNING]: No build files found. Nothing to delete. %ESC%[0m
popd
goto :eof

:: Generates a ESC character used to set line foreground and background colors
:setESC
for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do (
  set ESC=%%b
  exit /B 0
)