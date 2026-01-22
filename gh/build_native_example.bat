@echo off
setlocal EnableDelayedExpansion

set MSBPLAT=%~1
set RPATH=%~dp0\..\

echo.
echo MSBuild platform: %MSBPLAT%
echo.

:: Check if we are running locally (GITHUB_ACTIONS variable is missing)
if not defined GITHUB_ACTIONS (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
)

set VS_DIR=%RPATH%\native\examples\NurApiExample
set VS_SOLUTION=%VS_DIR%\NurApiExample.sln
set VS_PROJ=NurApiExample

echo Executing : msbuild %VS_SOLUTION% /v:d /t:Clean;Rebuild /t:%VS_PROJ% /p:Configuration="Release" /p:Platform="%MSBPLAT%" /p:BuildProjectReferences=false
"msbuild.exe" %VS_SOLUTION% /v:d /t:Clean;Rebuild /t:%VS_PROJ% /p:Configuration="Release" /p:Platform="%MSBPLAT%" /p:BuildProjectReferences=false

IF NOT EXIST "%VS_DIR%\Release\NurApiExample.exe" (
 echo FAILED
 EXIT /B 1
)

echo DONE
EXIT /B 0
