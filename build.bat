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

set BUILD_SOURCE_FILES=src/*.c src/string/*.c src/log/*.c src/file/*.c src/settings/*.c src/opengl/glad/*.c src/opengl/*.c
set BUILD_LINK_PATHS=-Isrc -Isrc/opengl -Isrc/opengl/glfw

set BUILD_COMPILER_FLAGS=-std=c17 -O0 -DLOG -DGL_DEBUG -g -Wall -Wextra -Wpedantic
set BUILD_LINKER_FLAGS=-D_CRT_SECURE_NO_DEPRECATE src/opengl/glfw/libglfw3.a -lopengl32 -lgdi32

if "editor" == "%OPTION%" (
    gcc %BUILD_SOURCE_FILES% %BUILD_COMPILER_FLAGS%^
        %BUILD_LINK_PATHS% %BUILD_LINKER_FLAGS%^
        -o %BUILD_FOLDER%/%BUILD_EXECUTABLE%
    exit /b
)
