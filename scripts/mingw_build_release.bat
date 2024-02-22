@echo off

SET "BUILD_PATH=mingw_build"
SET "GENERATE=cmake -G "MinGW Makefiles" . -B build/%BUILD_PATH%"
SET "BUILD=cmake --build build/%BUILD_PATH% --config Release"

%GENERATE% && %BUILD%%