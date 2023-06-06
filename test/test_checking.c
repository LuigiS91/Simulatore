#include <stdio.h>

#include "../src/lib/include/checking.h"
#include "../src/lib/include/comando.h"
#include "../src/lib/include/lettura_dati.h"
#include "../src/lib/include/propeller.h"
#include "../src/lib/include/atmosfera.h"
#include "../src/lib/include/aeromobile.h"
#include "../src/lib/include/trim.h"
#include "../src/lib/include/integratore.h"

int test_checking(){
    u_funz_comando u_com;
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
//    t_res_routh stabilita;


    double h,vel,gamma,t;
    int errore,i;
    char* file_dba="etc/dba.txt";
    char* file_engine="etc/engine.txt";
    char* file_prop="etc/propeller.txt";
    char* file_out_stati="output/stati.txt";
    char* file_out_com="output/comandi.txt";
    char* file_out_w_e="output/warning_errori.txt";
    double dt=0.01;
    FILE *out_stati,*out_com;

    printf("Test dei vari segnali sui comandi\n\n");

    u_com.grad_scontr.tipo=gradino_scontrato;
    u_com.grad_scontr.t_inizio=2;
    u_com.grad_scontr.t_1=3;
    u_com.grad_scontr.t_2=5;
    u_com.grad_scontr.t_finale=6;
    u_com.grad_scontr.ampiezza_iniziale=-2;
    u_com.grad_scontr.ampiezza_1=-1;
    u_com.grad_scontr.ampiezza_2=3;
    u_com.grad_scontr.ampiezza_finale=0;

    set_comando(u_com,c_da);


    u_com.rampa_scontr.tipo=rampa_scontrata;
    u_com.rampa_scontr.t_inizio=1;
    u_com.rampa_scontr.t_1=3;
    u_com.rampa_scontr.t_2=6;
    u_com.rampa_scontr.t_3=9;
    u_com.rampa_scontr.t_4=11;
    u_com.rampa_scontr.t_finale=16;
    u_com.rampa_scontr.ampiezza_iniziale=-1;
    u_com.rampa_scontr.ampiezza_1=0;
    u_com.rampa_scontr.ampiezza_intermedia=-2;
    u_com.rampa_scontr.ampiezza_2=-5;
    u_com.rampa_scontr.ampiezza_finale=3;

    set_comando(u_com,c_de);

    u_com.rampa.tipo=rampa;
    u_com.rampa.ampiezza_iniziale=0;
    u_com.rampa.ampiezza_finale=1;
    u_com.rampa.t_inizio=1;
    u_com.rampa.t_fine=5;

    set_comando(u_com,c_dr);

    u_com.sin.tipo=sinusoidale;
    u_com.sin.t_inizio=0;
    u_com.sin.periodo=10;
    u_com.sin.ampiezza=2;
    u_com.sin.fase=0;
    u_com.sin.valore_medio=1;

    set_comando(u_com,c_rpm);

    printf("i\tda\tde\tdr\trpm\n");
    for(i=0;i<=20;i++){
        com=comando(i);
        printf("%d\t%lf\t%lf\t%lf\t%lf\n",i,com.da,com.de,com.dr,com.rpm);
    }

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
/*
    stabilita=routh(res_trim,desc_aer,atm_sl,dba);

    if(stabilita.is_stat_stab)
        printf("Il velivolo e' stabile  staticamente in condizione di trim\n\n");
    if(stabilita.is_din_stab)
        printf("Il velivolo e' stabile  dinamicamente in condizione di trim\n\n");
*/
    printf("Eseguo l'integrazione\n\n");


    //Setto il comando di trim

    set_comando_trim(res_trim.comando);
/*
    u_com.gradino.tipo=impulso;
    u_com.gradino.t_inizio=20;
    u_com.gradino.t_fine=40;
    u_com.gradino.ampiezza_iniziale=0;
    u_com.gradino.ampiezza_regime=1*DEG_TO_RAD;

    set_comando(u_com,c_da);
    */
/*
    u_com.rampa_scontr.tipo=rampa_scontrata;
    u_com.rampa_scontr.t_inizio=20;
    u_com.rampa_scontr.t_1=30;
    u_com.rampa_scontr.t_2=40;
    u_com.rampa_scontr.t_3=50;
    u_com.rampa_scontr.t_4=60;
    u_com.rampa_scontr.t_finale=70;
    u_com.rampa_scontr.ampiezza_iniziale=res_trim.comando.de;
    u_com.rampa_scontr.ampiezza_1=res_trim.comando.de-1*DEG_TO_RAD;
    u_com.rampa_scontr.ampiezza_intermedia=res_trim.comando.de;
    u_com.rampa_scontr.ampiezza_2=res_trim.comando.de+1*DEG_TO_RAD;
    u_com.rampa_scontr.ampiezza_finale=res_trim.comando.de;

    set_comando(u_com,c_de);
    */

    u_com.rampa.tipo=rampa;
    u_com.rampa.ampiezza_iniziale=res_trim.comando.rpm;
    u_com.rampa.ampiezza_finale=2500;
    u_com.rampa.t_inizio=30;
    u_com.rampa.t_fine=300;

    set_comando(u_com,c_rpm);

    out_stati=fopen(file_out_stati,"w+");
    out_com=fopen(file_out_com,"w+");
    apri_file_w_e(file_out_w_e);

    fprintf(out_stati,"t\tu\tv\tw\tp\tq\tr\tphi\tpsi\ttheta\th\n");
    fprintf(out_com,"t\tde\tda\tdr\trpm\n");


    t=0;
    fprintf(out_stati,"%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\n",  t,
                                                                                                        stato.u,
                                                                                                        stato.v,
                                                                                                        stato.w,
                                                                                                        stato.p,
                                                                                                        stato.q,
                                                                                                        stato.r,
                                                                                                        stato.phi,
                                                                                                        stato.psi,
                                                                                                        stato.theta,
                                                                                                        stato.h);
    t+=dt;

    for(i=0;t<150&&!errore;i++){
        com=comando(t);
        errore=all_check(&desc_aer,&desc_prop,&stato,&dbp,&atm_sl,t,dt,&engine,&limiti_comandi,&com,&massa_combustibile);
        if(!errore){
            fprintf(out_com,"%.16e\t%.16e\t%.16e\t%.16e\t%.16e\n",t,com.de,com.da,com.dr,com.rpm);
            stato=integra(&stato,&com,&dba,&dbp,&desc_aer,&desc_prop,&atm_sl,dt);
            fprintf(out_stati,"%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\t%.16e\n",  t,
                                                                                                                stato.u,
                                                                                                                stato.v,
                                                                                                                stato.w,
                                                                                                                stato.p,
                                                                                                                stato.q,
                                                                                                                stato.r,
                                                                                                                stato.phi,
                                                                                                                stato.psi,
                                                                                                                stato.theta,
                                                                                                                stato.h);
            printf("t=%lf\r",t);
            t+=dt;
        }
    }

    errore=0;

    fclose(out_stati);
    fclose(out_com);
    chiudi_file_w_e();


    libera_dba(&dba);
    libera_dbp(&dbp);


    return 0;
}
