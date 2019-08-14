# Define project
cmake_minimum_required(VERSION 3.3)
project(iDeform VERSION 2.0.0)

# Set project version macro
add_definitions(-DPROJECT_VERSION="${PROJECT_VERSION}")

# Set default Maya version
if(NOT DEFINED MAYA_VERSION)
    set(MAYA_VERSION 2018)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(Maya REQUIRED)

# Set compile flags
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    add_compile_options(/wd4068 /EHsc)
endif()

# Create target
add_library(iDeform SHARED
    source/iCollide.h
    source/iCollide.cpp
    source/iDisplace.h
    source/iDisplace.cpp
    source/iSkinDeform.h
    source/iSkinDeform.cpp
    source/pluginMain.cpp)

include_directories(${MAYA_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR})

target_link_libraries(iDeform ${MAYA_LIBRARIES})
set_target_properties(iDeform PROPERTIES
    COMPILE_DEFINITIONS "${MAYA_COMPILE_DEFINITIONS}"
    PREFIX ""
    SUFFIX ${MAYA_PLUGIN_EXT})

if(WIN32)
    set_target_properties(iDeform PROPERTIES LINK_FLAGS "/export:initializePlugin /export:uninitializePlugin")
endif()
