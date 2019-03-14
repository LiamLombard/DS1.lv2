cmake_minimum_required(VERSION 3.7.0)
project(bossds1.lv2 VERSION 0.1.0)


include(LUT/CMakeLists.txt)
include(CircuitSim/CMakeLists.txt)

add_subdirectory(cmake)
add_subdirectory(src)
add_subdirectory(Simulation)
add_subdirectory(TimingSimulation)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
