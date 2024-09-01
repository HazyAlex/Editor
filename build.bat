@echo off

set BUILD_FOLDER=build
set BUILD_EXECUTABLE=editor.exe
set OPTION=%1

if "" == "%OPTION%" (
  echo Usage:
  echo.
  echo .\%~nx0 editor     - Build the editor
  echo .\%~nx0 clean      - Clean any temporary files and executables
  exit /b 1
)

if "clean" == "%OPTION%" (
    RD /S /Q %BUILD_FOLDER%
    MD %BUILD_FOLDER%
    exit /b
)

if "editor" == "%OPTION%" (
    gcc src/*.c src/string/*.c src/log/*.c src/file/*.c^
        -std=c17 -O0 -DLOG -DDEBUG -g -Wall -Wextra -Wpedantic^
        -Isrc -D_CRT_SECURE_NO_DEPRECATE^
        -o %BUILD_FOLDER%/%BUILD_EXECUTABLE%
    exit /b
)
