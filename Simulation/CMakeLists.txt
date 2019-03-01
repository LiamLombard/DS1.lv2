list(APPEND SIMULATION_SOURCES
  bossds1.cpp
  ${LUT_SOURCES}
  ${DS1_SOURCES}
)

list(APPEND SIMULATION_SOURCE_DIRS
  bossds1.cpp
  "../LUT"
  "../CircuitSim"
)


# DEBUG CONFIG
add_executable(simulation ${SIMULATION_SOURCES})

target_include_directories(simulation PUBLIC ${SIMULATION_SOURCE_DIRS})

target_link_libraries(simulation PRIVATE
  project_warnings --coverage
)