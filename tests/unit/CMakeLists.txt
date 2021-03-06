set(GTEST_ROOT /usr/src/gtest)
add_subdirectory(${GTEST_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/gtest)

add_definitions ( -DCMAKE_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

add_executable(
  test_container_config
  ContainerConfigTests.cpp
  ContainerConfigListTests.cpp
)
target_link_libraries(
  test_container_config
  libertine-common
  gtest gtest_main
  Qt5::Core
)

add_test(test_container_config
  test_container_config
)

add_test(test_libertine_public_gir
  "/usr/bin/python3" "-m" "testtools.run" "${CMAKE_CURRENT_SOURCE_DIR}/libertine_public_gir_tests.py"
)
set_tests_properties(test_libertine_public_gir
                     PROPERTIES
                     ENVIRONMENT "GI_TYPELIB_PATH=${CMAKE_BINARY_DIR}/liblibertine;LD_LIBRARY_PATH=${CMAKE_BINARY_DIR}/liblibertine:${LD_LIBRARY_PATH};CMAKE_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}")
set_tests_properties(test_libertine_public_gir
                     PROPERTIES
                     WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
