project(libSimulatore VERSION 0.1.0)

add_library(Sim STATIC
    aeromobile.c
    atmosfera.c
    checking.c
    comando.c
    integratore.c
    interpola.c
    lettura_dati.c
    propeller.c
    routh.c
    trim.c
)

target_include_directories(Sim PUBLIC)
target_link_libraries(Sim m)