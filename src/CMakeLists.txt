set(HEADER_PATH ../include/graphloom)

set(PUBLIC_HEADERS
    ${HEADER_PATH}/common/data_type.h
    ${HEADER_PATH}/common/initializer.h
    ${HEADER_PATH}/common/macros.h
    ${HEADER_PATH}/common/status.h
    
    ${HEADER_PATH}/device/cpu.h
    ${HEADER_PATH}/device/device.h
    ${HEADER_PATH}/device/registration.h

    ${HEADER_PATH}/graph/graph_context.h
    ${HEADER_PATH}/graph/graph_def.h
    ${HEADER_PATH}/graph/node_def_builder.h
    ${HEADER_PATH}/graph/session.h

    ${HEADER_PATH}/op/op.h
    ${HEADER_PATH}/op/registration.h

    ${HEADER_PATH}/tensor/tensor.h

    ${HEADER_PATH}/graphloom.h
)

set(PRIVATE_FILES
    common/status.cpp

    device/cpu.cpp
    device/device.cpp
    device/registration.cpp

    graph/graph_context.cpp
    graph/graph_def.cpp
    graph/graph_factory.cpp
    graph/graph_factory.h
    graph/graph.cpp
    graph/graph.h
    graph/node_def_builder.cpp

    op/op.cpp
    op/registration.cpp
    
    tensor/tensor.cpp
)

add_library(graphloom STATIC
    ${PRIVATE_FILES}
    ${PUBLIC_HEADERS}
)

target_include_directories(graphloom PUBLIC             # public headers
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include/>     # during build
    $<INSTALL_INTERFACE:include/>                       # during install: <prefix>/include/

    PRIVATE ${CMAKE_SOURCE_DIR}/src                     # private headers
)




#============================
# Import OpenMP
#============================
find_package(OpenMP REQUIRED)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
target_link_libraries(graphloom PRIVATE OpenMP::OpenMP_CXX)




#============================
# define install rules
#============================

# define targets
install(TARGETS graphloom EXPORT GraphLoom)

# define header directory
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# define export set
install(EXPORT GraphLoom
        NAMESPACE GraphLoom::
        DESTINATION lib/cmake/GraphLoom
)
