list(APPEND TIMING_SIM_SOURCES
  timing.cpp
  ${LUT_SOURCES}
  ${DS1_SOURCES}
)

list(APPEND TIMING_SIM_SOURCE_DIRS
  bossds1.cpp
  "../LUT"
  "../CircuitSim"
)


# DEBUG CONFIG
add_executable(timingSim ${TIMING_SIM_SOURCES})

target_include_directories(timingSim PUBLIC ${TIMING_SIM_SOURCE_DIRS})

target_link_libraries(timingSim PRIVATE
  project_warnings --coverage
)