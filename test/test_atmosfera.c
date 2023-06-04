#include <stdio.h>
#include <stdlib.h>
#include "../src/lib/include/atmosfera.h"


int test_atmosfera(){
    //Testo la funzione propeller
    t_atm prova,prova_std;
    t_atm_sl atm_sl;
    double h;
    printf("Test lettura funzione atmosfera\n\n");
    h=5000;
    atm_sl.dens=1.2;
    atm_sl.press=100000;
    atm_sl.temp=250;
    printf("La tua atmosfera ha:\nDensita' al livello del mare %lf kg/m^3;\nPressione al livello del mare %lf Pa;\nTemperatura al livello del mare %lf\n\n",atm_sl.dens,atm_sl.press,atm_sl.temp);
    prova=atmosfera(h,atm_sl);
    printf("A quota %lf m hai:\nDensita' %lf kg/m^3;\nPressione %lf Pa;\nTemperatura %f K;\nVelocita' del suono %f m/s.",prova.h,prova.dens,prova.press,prova.temp,prova.vel_s);
    printf("\n\n\n\nATMOSFERA STANDARD: \nDensita' al livello del mare %f kg/m^3;\nPressione al livello del mare %f Pa;\nTemperatura al livello del mare %f K\nGradiente termico verticale %f K/m.\n",std_rho0,std_P0,std_T0,std_a);
    prova_std=atmosfera_std(h);
    printf("A quota %f m hai:\nDensita' %f kg/m^3;\nPressione %f Pa;\nTemperatura %f K;\nVelocita' del suono %f m/s.",prova_std.h,prova_std.dens,prova_std.press,prova_std.temp,prova_std.vel_s);

    return 0;
}





