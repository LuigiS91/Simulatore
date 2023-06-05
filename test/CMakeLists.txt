create_test_sourcelist (TestSimulatore
  test_simulatore.c
  test_aeromobile.c
  test_atmosfera.c
  test_checking.c
  test_comando.c
  test_integratore.c
  test_interpola.c
  test_lettura.c
  test_propeller.c
  test_routh.c
  test_trim.c
  )

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin)
add_executable(exec_TestSimulatore ${TestSimulatore})
target_link_libraries(exec_TestSimulatore Sim)


set(TestsToRun ${TestSimulatore})
remove(TestsToRun test_simulatore.c) 

foreach(test ${TestsToRun})
  get_filename_component(TName ${test} NAME_WE)
  add_test(NAME ${TName} COMMAND exec_TestSimulatore ${TName} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endforeach()