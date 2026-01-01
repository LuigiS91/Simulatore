# Nome del test principale
set(TestName Simulatore)

# Lista dei file presenti in test/
file(GLOB TestList RELATIVE "${PROJECT_SOURCE_DIR}/test" "test_*.c")

# Lista dei file sorgente dei test
create_test_sourcelist(Test${TestName}
  test_${TestName}.c
  ${TestList}
)

# Output del binario dei test nella cartella test/bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/test/bin)

# Crea il binario principale dei test
add_executable(exec_Test${TestName} ${Test${TestName}})
target_link_libraries(exec_Test${TestName} Sim)
target_compile_options(exec_Test${TestName} PRIVATE -g -O0)

# Aggiungi i test 
foreach(Test ${TestList})
	get_filename_component(TestFunc ${Test} NAME_WE)	
	add_test(
		NAME ${TestFunc}
		COMMAND ${PROJECT_SOURCE_DIR}/test/bin/exec_Test${TestName} ${TestFunc}
		WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}  
	    )
endforeach()

