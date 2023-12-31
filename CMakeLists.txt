cmake_minimum_required(VERSION 3.26)
project(AirBusManagementSystem)

set(CMAKE_CXX_STANDARD 23)

add_executable(AirBusManagementSystem main.cpp
        classes/airline.cpp
        classes/airline.h
        classes/airport.cpp
        classes/airport.h
        network/graph.cpp
        network/graph.h
        network/vertex.cpp
        network/edge.cpp
        classes/Parser.cpp
        classes/Parser.h
        classes/Utils.cpp
        classes/Utils.h
        classes/Minheap.h
        classes/Fibtree.h
        classes/menu.cpp
        classes/menu.h
        network/graph_templates.h
)

target_compile_options(AirBusManagementSystem PRIVATE -msse2)

find_package(Doxygen)
if(DOXYGEN_FOUND)
    set(BUILD_DOC_DIR "${CMAKE_SOURCE_DIR}/docs/output")
    if(NOT EXISTS "${BUILD_DOC_DIR}")
        file(MAKE_DIRECTORY "${BUILD_DOC_DIR}")
    endif()
    set(DOXYGEN_IN "${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile")
    set(DOXYGEN_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
    configure_file("${DOXYGEN_IN}" "${DOXYGEN_OUT}" @ONLY)
    message("Doxygen build started")
    add_custom_target(Doxygen ALL
            COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYGEN_OUT}"
            WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
else(DOXYGEN_FOUND)
    message("Doxygen needs to be installed to generate the documentation.")
endif(DOXYGEN_FOUND)