@echo off

SET "BUILD_PATH=msvc_build"
SET "GENERATE=cmake -DTESTING=ON -G "Visual Studio 17 2022" . -B build/%BUILD_PATH%"
SET "BUILD=cmake --build build/%BUILD_PATH% --config Debug"
SET "TEST=ctest --test-dir build/%BUILD_PATH%/tests --output-on-failure -C Debug"

%GENERATE% && %BUILD% && %TEST%