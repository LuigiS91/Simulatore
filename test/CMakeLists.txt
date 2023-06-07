set(TestName Simulatore)

create_test_sourcelist (Test${TestName}
  test_${TestName}.c
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
  add_executable(exec_Test${TestName} ${Test${TestName}})
  target_link_libraries(exec_Test${TestName} Sim)
  
  
  set(TestsToRun ${Test${TestName}})
  remove(TestsToRun test_${TestName}.c) 
  
  foreach(test ${TestsToRun})
    get_filename_component(TName ${test} NAME_WE)
    add_test(NAME ${TName} COMMAND exec_Test${TestName} ${TName} WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  endforeach()