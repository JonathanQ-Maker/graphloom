@echo off

SET "BUILD_PATH=mingw_build"
SET "GENERATE=cmake -DTESTING=ON -G "MinGW Makefiles" . -B build/%BUILD_PATH%"
SET "BUILD=cmake --build build/%BUILD_PATH% --config Debug"
SET "TEST=ctest --test-dir build/%BUILD_PATH%/tests --output-on-failure -C Debug"

%GENERATE% && %BUILD% && %TEST%