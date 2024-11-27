@echo off

set BUILD_FOLDER=build
set BUILD_EXECUTABLE=editor.exe
set OPTION=%1

set BUILD_SOURCE_FILES=src/*.c src/string/*.c src/structures/*.c src/log/*.c src/file/*.c src/settings/*.c src/opengl/glad/*.c src/opengl/*.c src/text_rendering/*.c src/shaders/*.c src/editor/*.c
set BUILD_LINK_PATHS=-Isrc -Isrc/opengl -Isrc/opengl/glfw -Isrc/text_rendering/freetype -Isrc/text_rendering/freetype/config

if "" == "%OPTION%" goto :help
if "clean" == "%OPTION%" goto :clean
if "gcc" == "%OPTION%" goto :gcc
if "clang" == "%OPTION%" goto :clang

:help
  echo Usage:
  echo.
  echo .\%~nx0 gcc      - Build the editor (gcc)
  echo .\%~nx0 clang    - Build the editor (clang)
  echo .\%~nx0 clean    - Clean any temporary files and executables
  exit /b 1

:clean
    RD /S /Q %BUILD_FOLDER%
    MD %BUILD_FOLDER%
    exit /b

:gcc
    set BUILD_COMPILER_FLAGS=-std=c17 -O0 -DLOG -DGL_DEBUG -g -Wall -Wextra -Wpedantic
    set BUILD_LINKER_FLAGS=-D_CRT_SECURE_NO_DEPRECATE src/opengl/glfw/libglfw3.a -lopengl32 -lgdi32 -lfreetype

    gcc %BUILD_SOURCE_FILES% %BUILD_COMPILER_FLAGS%^
        %BUILD_LINK_PATHS% %BUILD_LINKER_FLAGS%^
        -o %BUILD_FOLDER%/%BUILD_EXECUTABLE%
    exit /b

:clang
    set BUILD_COMPILER_FLAGS=-std=c17 -O0 -DLOG -DGL_DEBUG -g -gcodeview -Wall -Wextra -Wpedantic
    set BUILD_LINKER_FLAGS=-D_CRT_SECURE_NO_DEPRECATE -static -lopengl32 -lgdi32 -luser32 -lshell32 src/opengl/glfw/glfw3.lib -Xlinker /ignore:4099 src/text_rendering/freetype/freetype.lib -Xlinker /DEBUG:FULL -Xlinker /subsystem:console -fsanitize=address

    clang %BUILD_SOURCE_FILES% %BUILD_COMPILER_FLAGS%^
        %BUILD_LINK_PATHS% %BUILD_LINKER_FLAGS%^
        -o %BUILD_FOLDER%/%BUILD_EXECUTABLE%
    exit /b
