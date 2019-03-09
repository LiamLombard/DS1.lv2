list(APPEND BossDS1_SOURCES
  main.cpp
  ${LUT_SOURCES}
  ${DS1_SOURCES}
)

list(APPEND BossDS1_SOURCE_DIRS
  bossds1.cpp
  "../LUT"
  "../CircuitSim"
)


# DEBUG CONFIG
add_library(bossds1 SHARED ${BossDS1_SOURCES})

target_include_directories(bossds1 PUBLIC ${BossDS1_SOURCE_DIRS})

target_link_libraries(bossds1 PRIVATE
  project_warnings --coverage
)

add_custom_command(TARGET bossds1
  DEPENDS ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E rename libbossds1.so DS1.so
  COMMAND ${CMAKE_COMMAND} -E copy DS1.so ../..
)
