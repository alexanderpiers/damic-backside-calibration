# CMakeLists.txt for aSe0vBB analysis

#----------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
project(backside_calibration_analysis)
set(CMAKE_BUILD_TYPE Debug)

#----------------------------------------------------------
# Location of root fies
list(APPEND CMAKE_PREFIX_PATH /home/apiers/rootbuilds/root6-18/cmake)
find_package(ROOT REQUIRED COMPONENTS Minuit2)

# openmp 
find_package(OpenMP)

#----------------------------------------------------------
# Include root and project directories
include_directories(${ROOT_INCLUDE_DIRS})
link_directories(${ROOT_LIBRARY_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

#----------------------------------------------------------
# Locate source and header files for project
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cxx)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.h)


#----------------------------------------------------------
# Create an executable and link to root libraries
add_executable(backside_calibration_analysis main.cxx ${sources} ${headers})
target_link_libraries(backside_calibration_analysis ${ROOT_LIBRARIES} OpenMP::OpenMP_CXX)
