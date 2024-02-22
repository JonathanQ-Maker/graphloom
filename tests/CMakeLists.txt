



include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()
include(CTest)
include_directories("../src" "../include")


# list of test executables (do not include header files)
set(testFiles
    data_type_test.cpp
    device_cpu_test.cpp
    device_registry_test.cpp
    node_def_builder_test.cpp
    register_op_test.cpp
    status_test.cpp
)

# # Now simply link against gtest or gtest_main as needed. Eg
# add_executable(example example.cpp)
# target_link_libraries(example gtest_main)


# compile an executable for each test file
foreach(testFile ${testFiles})
    get_filename_component(testFilename ${testFile} NAME_WLE)

    add_executable(${testFilename} ${testFile})
    target_link_libraries(${testFilename} PRIVATE gtest_main graphloom)
    add_test(NAME ${testFilename} COMMAND ${testFilename})
endforeach()
