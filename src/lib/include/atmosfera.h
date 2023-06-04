#ifndef ATMOSFERA_H_INCLUDED
#define ATMOSFERA_H_INCLUDED

#include <math.h>

#define K_vels 8314*1.4/29
#define std_P0 101325.
#define std_T0 288.
#define std_rho0 1.225
#define std_a 0.0065
#define std_m 5.2561
#define std_n 4.2561



typedef struct{
    double h;
    double press;
    double dens;
    double temp;
    double vel_s;
}t_atm;

typedef struct{
    double press;
    double dens;
    double temp;
}t_atm_sl;



t_atm atmosfera(double h,t_atm_sl atm_sl);
t_atm atmosfera_std(double h);

#endif // ATMOSFERA_H_INCLUDED
