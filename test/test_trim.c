#include <stdio.h>
#include "../src/lib/include/trim.h"

int test_trim(){
    t_descrizione_aereo desc_aer;
    t_limiti_comando limiti_comandi;
    t_massa_combustibile massa_combustibile;
    t_dba dba;
    t_engine engine;
    t_descrizione_propeller desc_prop;
    t_dbp dbp;
    t_atm_sl atm_sl;
    t_res_trim res_trim;
    double h,vel,gamma;
    int errore;
    char* file_dba="etc/dba.txt";
    char* file_engine="etc/engine.txt";
    char* file_prop="etc/propeller.txt";



    //Lettura dei dati di input
    printf("Lettura dei file\n");
    errore=apri_file_lettura(file_dba);
    if(errore){
        printf("Errore lettura dati dba!\n");
        return -1;
    }
    lettura_dati_intestazione_aerodinamica(&desc_aer,&limiti_comandi,&massa_combustibile);
    alloca_dba(desc_aer.num_inc,&dba);
    lettura_dati_aerodinamici(&dba);
    set_dimensione_vettori_interpola(desc_aer.num_inc);
    chiudi_file_lettura();
    errore=apri_file_lettura(file_engine);
    if(errore){
        printf("Errore lettura dati engine!\n");
        return -1;
    }
    lettura_engine(&engine);
    chiudi_file_lettura();
    errore=apri_file_lettura(file_prop);
    if(errore){
        printf("Errore lettura dati prop!\n");
        return -1;
    }
    lettura_intestazione_propeller(&desc_prop);
    alloca_dbp(desc_prop.num_stazioni,&dbp);
    lettura_stazioni_propeller(&dbp,desc_prop.diametro_ogiva);
    chiudi_file_lettura();

    //imposto i valori di ingresso

    atm_sl.dens=std_rho0;
    atm_sl.press=std_P0;
    atm_sl.temp=std_T0;
    h=1000;
    vel=52;
    gamma=0*DEG_TO_RAD;

    printf("\nSettaggio parametri di volo\nAtmosfera std al suolo\nQuota %lf m\nVelocita' %lf m s^-1\nGamma %lf [deg]\n",h,vel,gamma*RAD_TO_DEG);

    printf("\nTest funzione trim\n");

    res_trim=trim(desc_aer,dba,engine,desc_prop,dbp,atm_sl,h,vel,gamma);

    printf("u=%lf\nv=%lf\nw=%lf\np=%lf\nq=%lf\nr=%lf\nphi=%lf\npsi=%lf\ntheta=%lf\nh=%lf\n\n",  res_trim.stato.u,
                                                                                                res_trim.stato.v,
                                                                                                res_trim.stato.w,
                                                                                                res_trim.stato.p*RAD_TO_DEG,
                                                                                                res_trim.stato.q*RAD_TO_DEG,
                                                                                                res_trim.stato.r*RAD_TO_DEG,
                                                                                                res_trim.stato.phi*RAD_TO_DEG,
                                                                                                res_trim.stato.psi*RAD_TO_DEG,
                                                                                                res_trim.stato.theta*RAD_TO_DEG,
                                                                                                res_trim.stato.h);
    printf("da=%lf\nde=%lf\ndr=%lf\nrpm=%lf\n",res_trim.comando.da*RAD_TO_DEG,res_trim.comando.de*RAD_TO_DEG,res_trim.comando.dr*RAD_TO_DEG,res_trim.comando.rpm);

    printf("Errore trim: %d\n",errore_trim);

    libera_dba(&dba);
    libera_dbp(&dbp);

    return 0;
}
