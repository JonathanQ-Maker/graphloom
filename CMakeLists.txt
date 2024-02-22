cmake_minimum_required(VERSION 3.27.1)

project(GraphLoom 
    VERSION 0.1.0 
    DESCRIPTION "Threaded Tensor Library"
    LANGUAGES CXX
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)

include(GNUInstallDirs)
add_subdirectory(src)


# enable with "cmake -DTESTING=ON"
if (TESTING)
    add_subdirectory(tests)
endif()