#ifndef INTERPOLA_H_INCLUDED
#define INTERPOLA_H_INCLUDED

#include <stdio.h>
#include <math.h>

#define DEG_TO_RAD 3.14/180.0
#define RAD_TO_DEG 180.0/3.14

extern int N_dba;
extern int errore_interpola;

void set_dimensione_vettori_interpola(int N_dba);

double interpola(double *v_alpha,double *v_coeff,double alpha);
//La funzione restituisce il valore interpolato dei coefficienti aerodinamici,
//gli angoli sono espressi in radianti

#endif // INTERPOLA_H_INCLUDED
