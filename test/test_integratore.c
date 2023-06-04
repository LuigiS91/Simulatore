#include <stdio.h>

#include "../src/lib/include/lettura_dati.h"
#include "../src/lib/include/propeller.h"
#include "../src/lib/include/atmosfera.h"
#include "../src/lib/include/aeromobile.h"
#include "../src/lib/include/trim.h"
#include "../src/lib/include/routh.h"
#include "../src/lib/include/integratore.h"

int test_integratore(){
    t_descrizione_aereo desc_aer;
    t_limiti_comando limiti_comandi;
    t_massa_combustibile massa_combustibile;
    t_dba dba;
    t_engine engine;
    t_descrizione_propeller desc_prop;
    t_dbp dbp;
    t_atm_sl atm_sl;
    t_res_trim res_trim;
    t_stato stato;
    t_comando com;
    t_stato dstato;
    t_res_routh stabilita;


    double h,vel,gamma,t;
    int errore,i;
    char* file_dba="../../etc/dba.txt";
    char* file_engine="../../etc/engine.txt";
    char* file_prop="../../etc/propeller.txt";
    char* file_out_stati="../../output/stati.txt";
    double dt=0.01;
    FILE* in_lettura;


    //Lettura dei dati di input
    printf("Lettura dei file\n");
    errore=apri_file_lettura(file_dba);
    if(errore){
        printf("Errore lettura dati dba!\nPremere un tato per terminare\n");
        getchar();
        return -1;
    }
    lettura_dati_intestazione_aerodinamica(&desc_aer,&limiti_comandi,&massa_combustibile);
    alloca_dba(desc_aer.num_inc,&dba);
    lettura_dati_aerodinamici(&dba);
    set_dimensione_vettori_interpola(desc_aer.num_inc);
    chiudi_file_lettura();
    errore=apri_file_lettura(file_engine);
    if(errore){
        printf("Errore lettura dati engine!\nPremere un tato per terminare\n");
        getchar();
        return -1;
    }
    lettura_engine(&engine);
    chiudi_file_lettura();
    errore=apri_file_lettura(file_prop);
    if(errore){
        printf("Errore lettura dati prop!\nPremere un tato per terminare\n");
        getchar();
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
    gamma=0;

    printf("\nSettaggio parametri di volo\nAtmosfera std al suolo\nQuota %lf m\nVelocita' %lf m s^-1\nGamma %lf [deg]\n",h,vel,gamma*RAD_TO_DEG);

    printf("\nCalcolo la condizione di trim\n");
    res_trim=trim(desc_aer,dba,engine,desc_prop,dbp,atm_sl,h,vel,gamma);


    stato=res_trim.stato;
    com=res_trim.comando;

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

    printf("\nCalcolo la derivata dello stato dell'aeromobile nella condizione di trim (dovrebbe essere tutto nullo a meno delle approssimazioni)\n");

    dstato=aeromobile(&stato,&com,&dba,&dbp,&desc_aer,&desc_prop,&atm_sl);

    printf("du=%lf\ndv=%lf\ndw=%lf\ndp=%lf\ndq=%lf\ndr=%lf\ndphi=%lf\ndpsi=%lf\ndtheta=%lf\ndh=%lf\n\n",dstato.u,dstato.v,dstato.w,dstato.p,dstato.q,dstato.r,dstato.phi,dstato.psi,dstato.theta,dstato.h);

    stabilita=routh(res_trim,desc_aer,atm_sl,dba);

    if(stabilita.is_stat_stab)
        printf("Il velivolo e' stabile  staticamente in condizione di trim\n\n");
    if(stabilita.is_din_stab)
        printf("Il velivolo e' stabile  dinamicamente in condizione di trim\n\n");

    printf("Eseguo l'integrazione\n\n");

    in_lettura=fopen(file_out_stati,"w+");

    fprintf(in_lettura,"t\tu\tv\tw\tp\tq\tr\tphi\tpsi\ttheta\th\n");


    //Setto il comando
    //com.dr=0.01*DEG_TO_RAD;

    t=0;
    for(i=0;t<500;i++){
        stato=integra(&stato,&com,&dba,&dbp,&desc_aer,&desc_prop,&atm_sl,dt);
        t+=dt;
        printf("t=%lf\r",t);
        fprintf(in_lettura,"%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\n",     t,
                                                                                                        stato.u,
                                                                                                        stato.v,
                                                                                                        stato.w,
                                                                                                        stato.p*RAD_TO_DEG,
                                                                                                        stato.q*RAD_TO_DEG,
                                                                                                        stato.r*RAD_TO_DEG,
                                                                                                        stato.phi*RAD_TO_DEG,
                                                                                                        stato.psi*RAD_TO_DEG,
                                                                                                        stato.theta*RAD_TO_DEG,
                                                                                                        stato.h);
    }

    fclose(in_lettura);


    libera_dba(&dba);
    libera_dbp(&dbp);


    return 0;
}

