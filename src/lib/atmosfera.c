#include "include/atmosfera.h"



t_atm atmosfera(double h,t_atm_sl atm_sl){ //HO ottimizzato i passaggi per evitare di ricalcolare 1e999 volte le stesse cose inutilmente
    t_atm res;
    res.h=h;
    res.temp=atm_sl.temp-std_a*h;
    res.press=atm_sl.press*pow(res.temp/atm_sl.temp,std_m);
    res.dens=atm_sl.dens*pow(res.temp/atm_sl.temp,std_n);
    res.vel_s=sqrt(K_vels*res.temp);
    return res;
}
t_atm atmosfera_std(double h){  //in realt� questa sarebbe pi� veloce se si evitasse di passare quei parametri standar, il compilatore potrebbe per� ottimizzare (BOH)
    //return atmosfera(h,std_P0,std_T0,std_rho0,std_a);
    t_atm res;
    res.h=h;
    res.temp=std_T0-std_a*h;
    res.press=std_P0*pow(res.temp/std_T0,std_m);
    res.dens=std_rho0*pow(res.temp/std_T0,std_n);
    res.vel_s=sqrt(K_vels*res.temp);
    return res;
}

