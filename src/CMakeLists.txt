add_subdirectory(lib)

add_executable(Simulatore main.c)
target_link_libraries(Simulatore Sim)