#include <stdio.h>
#include <stdlib.h>
#include "../src/lib/include/propeller.h"
#include "../src/lib/include/atmosfera.h"



int test_propeller(){
    t_descrizione_propeller desc_prop;
    t_dbp dbp;
    t_res_prop prop;
    const char* file_propeller="../../etc/propeller.txt";
    t_atm atm;
    int errore;

    atm=atmosfera_std(800);
    double vel=52;
    double rpm=1656;

    printf("Parametri atmosferici settati\nQuota %lf m\nDensita' %lf kg m^-3\nTemperatura %lf K\nPressione %lf Pa\n",atm.h,atm.dens,atm.temp,atm.press);

    printf("Giri motore impostati %lf\n",rpm);
    printf("Velocit� di volo %lf m s^-1\n",vel);

    errore=apri_file_lettura(file_propeller);
    if(errore){
        printf("Errore lettura dati propeller!\nPremere un tato per terminare\n");
        getchar();
        return -1;
    }
    lettura_intestazione_propeller(&desc_prop);

    alloca_dbp(desc_prop.num_stazioni,&dbp);

    lettura_stazioni_propeller(&dbp,desc_prop.diametro_ogiva);



    prop=propeller(&desc_prop,&dbp,&atm,vel,rpm,RESIDUO_PROPELLER_BASE);

    printf("Spinta: %lf\n",prop.thrust);
    printf("Coppia: %lf\n",prop.torque);

    printf("Indice della stazione di fine HUB: %d\n",dbp.staz_fuorihub);


    chiudi_file_lettura();
    libera_dbp(&dbp);

return 0;
}
