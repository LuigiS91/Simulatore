#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "lib/include/lettura_dati.h"
#include "lib/include/atmosfera.h"
#include "lib/include/propeller.h"
#include "lib/include/aeromobile.h"
#include "lib/include/trim.h"
#include "lib/include/routh.h"
#include "lib/include/comando.h"
#include "lib/include/interpola.h"
#include "lib/include/integratore.h"
#include "lib/include/checking.h"

#define DT 0.01
#define RAD_TO_DEG 180.0/3.14
#define DEG_TO_RAD 3.14/180.0

typedef enum {m_inizio=0,m_trim,m_comando,m_integrazione,m_uscita,m_resoconto,m_atm,m_tipo_comando}t_schermata_menu;
typedef struct {
    int trim;
    int h;
    int vel;
    int gamma;
    int atm_sl;
    int rho;
    int press;
    int temp;
}t_settato;

t_schermata_menu menu_inizio(t_settato settato);
t_schermata_menu menu_trim(t_settato* settato,double *h,double *vel,double *gamma,
                           t_descrizione_aereo desc_aer,t_dba dba,t_engine engine,t_descrizione_propeller desc_prop,t_dbp dbp,t_atm_sl atm_sl,t_res_trim *res_trim);
t_schermata_menu menu_comando(t_res_trim res_trim,t_componente_comando *comp_com);
t_schermata_menu menu_integrazione(char *file_out_stati,char *file_out_com,char *file_out_w_e,t_res_trim res_trim,t_dba dba,t_dbp dbp,
                                   t_descrizione_aereo desc_aer,t_descrizione_propeller desc_prop,t_atm_sl atm_sl,t_engine engine,t_limiti_comando limiti_comandi,
                                   t_massa_combustibile mass_comb);
t_schermata_menu menu_uscita(int *esci);
t_schermata_menu menu_resoconto(t_settato *settato,t_atm_sl *atm_sl,double *h,double *vel,double *gamma,t_res_trim *res_trim);
t_schermata_menu menu_atm(t_settato* settato,t_atm_sl *atm_sl);
t_schermata_menu menu_tipo_comando(t_componente_comando comp_com,t_res_trim res_trim);


int main(int argc,char **argv){
    t_componente_comando comp_com;
    t_descrizione_aereo desc_aer;
    t_limiti_comando limiti_comandi;
    t_massa_combustibile massa_combustibile;
    t_dba dba;
    t_engine engine;
    t_descrizione_propeller desc_prop;
    t_dbp dbp;
    t_atm_sl atm_sl;
    t_res_trim res_trim;
    t_schermata_menu menu;
    t_settato settato={0};

    double h,vel,gamma;
    int errore=0,esci=0;
    char* file_dba="../etc/dba.txt";
    char* file_engine="../etc/engine.txt";
    char* file_prop="../etc/propeller.txt";
    char* file_out_stati="../output/stati.txt";
    char* file_out_com="../output/comandi.txt";
    char* file_out_w_e="../output/warning_errori.txt";


    //Lettura dei dati di input
    errore=apri_file_lettura(file_dba);
    if(errore){
        printf("[-] Errore lettura dati dba\nPremere un tasto per terminare\n");
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
        printf("[-] Errore lettura dati engine\nPremere un tato per terminare\n");
        getchar();
        return -1;
    }
    lettura_engine(&engine);
    chiudi_file_lettura();
    errore=apri_file_lettura(file_prop);
    if(errore){
        printf("[-] Errore lettura dati propeller\nPremere un tato per terminare\n");
        getchar();
        return -1;
    }
    lettura_intestazione_propeller(&desc_prop);
    alloca_dbp(desc_prop.num_stazioni,&dbp);
    lettura_stazioni_propeller(&dbp,desc_prop.diametro_ogiva);
    chiudi_file_lettura();

    //imposto i valori di ingresso
    menu=m_inizio;
    printf("%s\n\n",argv[0]);
    printf("Caricamento dei dati avvenuto con successo\n\n");

    while(!esci){
        switch(menu){
            case m_inizio:
                menu=menu_inizio(settato);
                break;
            case m_trim:
                menu=menu_trim(&settato,&h,&vel,&gamma,desc_aer,dba,engine,desc_prop,dbp,atm_sl,&res_trim);
                break;
            case m_comando:
                menu=menu_comando(res_trim,&comp_com);
                break;
            case m_integrazione:
                menu=menu_integrazione(file_out_stati,file_out_com,file_out_w_e,res_trim,dba,dbp,desc_aer,desc_prop,atm_sl,engine,limiti_comandi,massa_combustibile);
                break;
            case m_uscita:
                menu=menu_uscita(&esci);
                break;
            case m_resoconto:
                menu=menu_resoconto(&settato,&atm_sl,&h,&vel,&gamma,&res_trim);
                break;
            case m_atm:
                menu=menu_atm(&settato,&atm_sl);
                break;
            case m_tipo_comando:
                menu=menu_tipo_comando(comp_com,res_trim);
                break;
        }
    }
    libera_dba(&dba);
    libera_dbp(&dbp);

    return 0;
}

t_schermata_menu menu_inizio(t_settato settato){
    t_schermata_menu menu=m_inizio;
    char scelta,c;
    scelta=0;
    printf("\n\nDigitare il numero per accedere alla sezione voluta\n\n(0) Chiusura del programma\n(1) Settaggio atmosfera al livello del mare\n(2) Settaggio condizione di trim\n");
    printf("(3) Settaggio comando\n(4) Resoconto dei dati impostati\n(5) Lancio della simulazione\n\n");
    while(((c=getchar())!='\n'))scelta=c;
    switch(scelta){
        case '0':
            menu=m_uscita;
            break;
        case '1':
            menu=m_atm;
            break;
        case '2':
            if(!settato.atm_sl){
                printf("\n\n[!] Per poter accedere al menu di trimmaggio e' necessario prima settare le condizioni atmosferiche al suolo\n\n");
                printf("\n\nPremere un tasto per continuare\n");
                getchar();
            }
            else{
                menu=m_trim;
            }
            break;
        case '3':
            if(!settato.trim){
                printf("\n\n[!] Per poter accedere al menu di settaggio dei comandi e' necessario prima settare la condizione di trim valida\n\n");
                printf("\n\nPremere un tasto per continuare\n");
                getchar();
            }
            else{
                menu=m_comando;
            }
            break;
        case '4':
            menu=m_resoconto;
            break;
        case '5':
            if(!settato.atm_sl){
                printf("\n\n[!] Non e' possibile lanciare la simulazione senza aver settato le condizioni atmosferiche al suolo\n\n");
                printf("\n\nPremere un tasto per continuare\n");
                getchar();
            }
            else if(!settato.trim){
                printf("\n\n[!] Non e' possibile lanciare la simulazione senza aver settato una condizione di trim valida\n\n");
                printf("\n\nPremere un tasto per continuare\n");
                getchar();
            }
            else{
                menu=m_integrazione;
            }
            break;
    }
    return menu;
};

t_schermata_menu menu_atm(t_settato* settato,t_atm_sl *atm_sl){
    t_schermata_menu menu=m_atm;
    int scelta,c;
    scelta=0;
    printf("\n\nDigitare il numero per accedere alla sezione voluta\n\n(0) Tornare al menu precedente\n(1) Settaggio dell'atmosfera standard al livello del mare\n(2) Settaggio temperatura al livello del mare\n");
    printf("(3) Settaggio densita' al livello del mare\n(4) Settaggio della pressione al livello del mare\n(5) Resoconto dei dati impostati\n\n");
    while((c=getchar())!='\n')scelta=c;
    switch(scelta){
        case '0':
            menu=m_inizio;
            if(settato->rho&&settato->press&&settato->temp)
                settato->atm_sl=1;
            break;
        case '1':
            atm_sl->dens=std_rho0;
            atm_sl->press=std_P0;
            atm_sl->temp=std_T0;
            printf("\n\nAtmosfera al livello del mare :\nT : %lf K\nRho : %lf kg/m^3\nP : %lf Pa\n\n",atm_sl->temp,atm_sl->dens,atm_sl->press);
            settato->atm_sl=1;
            settato->rho=1;
            settato->press=1;
            settato->temp=1;
            printf("\n\nPremere un tasto per continuare\n");
            getchar();
            break;
        case '2':
            printf("\nInserire la temperatura al livello del mare [K] : ");
            scanf("%lf",&atm_sl->temp);
            settato->temp=1;
            break;
        case '3':
            printf("\nInserire la densita' al livello del mare [kg/m^3] : ");
            scanf("%lf",&atm_sl->dens);
            settato->rho=1;
            break;
        case '4':
            printf("\nInserire la pressione al livello del mare [Pa] : ");
            scanf("%lf",&atm_sl->press);
            settato->press=1;
            break;
        case '5':
            printf("\n\nAtmosfera al livello del mare :\n");
            if(settato->temp)
                printf("T : %lf K\n",atm_sl->temp);
            if(settato->rho)
                printf("Rho : %lf kg/m^3\n",atm_sl->dens);
            if(settato->press)
                printf("P : %lf Pa\n",atm_sl->press);
            printf("\nPremere un tasto per continuare\n");
            getchar();
            break;
    }
    return menu;
};

t_schermata_menu menu_trim(t_settato* settato,double *h,double *vel,double *gamma,t_descrizione_aereo desc_aer,t_dba dba,t_engine engine,t_descrizione_propeller desc_prop,t_dbp dbp,t_atm_sl atm_sl,t_res_trim *res_trim){
    t_schermata_menu menu=m_trim;
    t_stato dstato;
    t_res_routh stabilita;
    int scelta,c;
    scelta=0;
    printf("\n\nDigitare il numero per accedere alla sezione voluta\n\n(0) Tornare al menu precedente\n(1) Impostare la quota\n(2) Impostare la velocita'\n");
    printf("(3) Impostare l'angolo gamma\n(4) Resoconto dei dati inseriti\n(5) Determinare la condizione di trim\n\n");
    while((c=getchar())!='\n')scelta=c;
    switch(scelta){
        case '0':
            menu=m_inizio;
            break;
        case '1':
            printf("\nInserire la quota desiderata [m] : ");
            scanf("%lf",h);
            getchar();
            settato->h=1;
            break;
        case '2':
            printf("\nInserire la velocita' [m/s] : ");
            scanf("%lf",vel);
            getchar();
            settato->vel=1;
            break;
        case '3':
            printf("\nInserire l'angolo gamma [deg] : ");
            scanf("%lf",gamma);
            getchar();
            *gamma*=DEG_TO_RAD;
            settato->gamma=1;
            break;
        case '4':
            printf("\n\nParametri settati per il trim :\n");
            if(settato->h)
                printf("h : %lf m\n",*h);
            if(settato->vel)
                printf("V : %lf m/s\n",*vel);
            if(settato->gamma)
                printf("gamma : %lf deg\n",*gamma*RAD_TO_DEG);
            printf("\nPremere un tasto per continuare\n");
            getchar();
            break;
        case '5':
            settato->trim=0;
            if(settato->h&&settato->gamma&&settato->vel){
                *res_trim=trim(desc_aer,dba,engine,desc_prop,dbp,atm_sl,*h,*vel,*gamma);
                stabilita=routh(*res_trim,desc_aer,atm_sl,dba);
                set_comando_trim(res_trim->comando);
                if(!check_trim()&&!check_stab(&stabilita)){
                    printf("\nVettore di stato nella condizione di trim :\n");
                    printf("u=%lf m/s\nv=%lf m/s\nw=%lf m/s\np=%lf deg/s\nq=%lf deg/s\nr=%lf deg/s\nphi=%lf deg\npsi=%lf deg\ntheta=%lf deg \nh=%lf h\n\n",  res_trim->stato.u,
                                                                                                                res_trim->stato.v,
                                                                                                                res_trim->stato.w,
                                                                                                                res_trim->stato.p*RAD_TO_DEG,
                                                                                                                res_trim->stato.q*RAD_TO_DEG,
                                                                                                                res_trim->stato.r*RAD_TO_DEG,
                                                                                                                res_trim->stato.phi*RAD_TO_DEG,
                                                                                                                res_trim->stato.psi*RAD_TO_DEG,
                                                                                                                res_trim->stato.theta*RAD_TO_DEG,
                                                                                                                res_trim->stato.h);
                    printf("Premere un tasto per continuare\n");
                    getchar();


                    printf("\nVettore di comando nella condizione di trim :\n");
                    printf("da=%lf deg\nde=%lf deg\ndr=%lf deg\nrpm=%lf rpm\n\n",res_trim->comando.da*RAD_TO_DEG,res_trim->comando.de*RAD_TO_DEG,res_trim->comando.dr*RAD_TO_DEG,res_trim->comando.rpm);


                    printf("Premere un tasto per continuare\n");
                    getchar();

                    printf("\nVettore derivata dello stato dell'aeromobile nella condizione di trim\n");
                    dstato=aeromobile(&res_trim->stato,&res_trim->comando,&dba,&dbp,&desc_aer,&desc_prop,&atm_sl);

                    printf("du=%lf m/s^2\ndv=%lf m/s^2\ndw=%lf m/s^2\ndp=%lf deg/s^2\ndq=%lf deg/s^2\ndr=%lf deg/s^2\ndphi=%lf deg/s\ndpsi=%lf deg/s\ndtheta=%lf deg/s\ndh=%lf m/s\n\n",dstato.u,dstato.v,dstato.w,dstato.p,dstato.q,dstato.r,dstato.phi,dstato.psi,dstato.theta,dstato.h);

                    printf("Premere un tasto per continuare\n");
                    getchar();

                    if(stabilita.is_stat_stab)
                        printf("Il velivolo e' stabile  staticamente in condizione di trim\n\n");

                    if(stabilita.is_din_stab){
                        printf("Il velivolo e' stabile  dinamicamente in condizione di trim\n\n");
                        printf("Corto periodo\nOmega :%lf\nPeriodo :%lf\nSmorzamento :%lf\nTempo di dimezzamento :%lf\n\n",stabilita.omega_sp,stabilita.periodo_sp,stabilita.smorz_sp,stabilita.dimezzamento_sp);
                        printf("Fugoide\nOmega :%lf\nPeriodo :%lf\nSmorzamento :%lf\nTempo di dimezzamento :%lf\n\n",stabilita.omega_ph,stabilita.periodo_ph,stabilita.smorz_ph,stabilita.dimezzamento_ph);
                    }

                    if(stabilita.is_stat_stab&&stabilita.is_stat_stab)
                        settato->trim=1;
                }
                printf("Premere un tasto per continuare\n");
                getchar();
            }
            else{
                if(!settato->h){
                    printf("\n\n[!] Per determinare la condizione di trim e' necessario impostare h\n\n");
                }
                if(!settato->vel){
                    printf("\n\n[!] Per determinare la condizione di trim e' necessario impostare la velocita'\n\n");
                }
                if(!settato->gamma){
                    printf("\n\n[!] Per determinare la condizione di trim e' necessario impostare gamma\n\n");
                }
            }
            break;
    }
    return menu;
};

t_schermata_menu menu_comando(t_res_trim res_trim,t_componente_comando *comp_com){
    t_schermata_menu menu=m_comando;
    char *comando[4]={"da","de","dr","rpm"};
    char *tipo[10]={"base","impulso","gradino","rampa","costante","sinusoidale","gradino scontrato","rampa scontrata","trapezio","trapezio scontrato"};
    int scelta,i,c;
    scelta=0;
    printf("\n\nDigitare il numero per accedere alla sezione voluta\n\n(0) Tornare al menu precedente\n(1) Impostare il comando equilibratore\n(2) Impostare comando alettone\n");
    printf("(3) Impostare comando di timone\n(4) Impostare il comando di manetta\n(5) Resocondo dei comandi impostati\n\n");
    while((c=getchar())!='\n')scelta=c;
    switch(scelta){
        case '0':
            menu=m_inizio;
            break;
        case '1':
            *comp_com=c_de;
            menu=menu_tipo_comando(*comp_com,res_trim);
            break;
        case '2':
            *comp_com=c_da;
            menu=menu_tipo_comando(*comp_com,res_trim);
            break;
        case '3':
            *comp_com=c_dr;
            menu=menu_tipo_comando(*comp_com,res_trim);
            break;
        case '4':
            *comp_com=c_rpm;
            menu=menu_tipo_comando(*comp_com,res_trim);
            break;
        case '5':
            printf("\n\nComandi impostati\n\n");
            for(i=0;i<4;i++){
                printf("\n\n%s :\n\tTipo : %s\n",comando[i],tipo[set_comandi[i].base.tipo]);
                switch(set_comandi[i].base.tipo){
                    case base:
                        break;
                    case gradino:
                        printf("\tT inizio : %lf\n\tT fine : %lf\n",set_comandi[i].gradino.t_inizio,set_comandi[i].gradino.t_fine);
                        if(i==c_rpm){
                            printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza a regime [rpm] : %lf\n",set_comandi[i].gradino.ampiezza_iniziale,set_comandi[i].gradino.ampiezza_regime);
                        }
                        else{
                            printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza a regime [deg] : %lf\n",set_comandi[i].gradino.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].gradino.ampiezza_regime*RAD_TO_DEG);
                        }
                        break;
                    case rampa:
                        printf("\tT inizio : %lf\n\tT fine : %lf\n",set_comandi[i].rampa.t_inizio,set_comandi[i].rampa.t_fine);
                        if(i==c_rpm){
                            printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza finale [rpm] : %lf\n",set_comandi[i].rampa.ampiezza_iniziale,set_comandi[i].rampa.ampiezza_finale);
                        }
                        else{
                            printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza finale [deg] : %lf\n",set_comandi[i].rampa.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].rampa.ampiezza_finale*RAD_TO_DEG);
                        }
                        break;
                    case impulso:
                        printf("\tT inizio : %lf\n\tdT : %lf\n",set_comandi[i].impulso.t_impulso,set_comandi[i].impulso.dt);
                        if(i==c_rpm){
                            printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza impulso [rpm] : %lf\n",set_comandi[i].impulso.ampiezza_iniziale,set_comandi[i].impulso.ampiezza_impulso);
                        }
                        else{
                            printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza impulso [deg] : %lf\n",set_comandi[i].impulso.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].impulso.ampiezza_impulso*RAD_TO_DEG);
                        }
                        break;
                    case costante:
                        if(i==c_rpm){
                                printf("\tAmpiezza [rpm] : %lf\n",set_comandi[i].costante.ampiezza);
                        }
                        else{
                                printf("\tAmpiezza [deg] : %lf\n",set_comandi[i].costante.ampiezza*RAD_TO_DEG);
                        }
                        break;
                    case gradino_scontrato:
                        printf("\tT inizio : %lf\n\tT 1 : %lf\n\tT 2 : %lf\n\tT finale : %lf\n",set_comandi[i].grad_scontr.t_inizio,set_comandi[i].grad_scontr.t_1,set_comandi[i].grad_scontr.t_2,set_comandi[i].grad_scontr.t_finale);
                        if(i==c_rpm){
                                printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza 1 [rpm] : %lf\n\tAmpiezza 2 [rpm] : %lf\n\tAmpiezza finale [rpm] : %lf\n",
                                                                            set_comandi[i].grad_scontr.ampiezza_iniziale,set_comandi[i].grad_scontr.ampiezza_1,
                                                                            set_comandi[i].grad_scontr.ampiezza_2,set_comandi[i].grad_scontr.ampiezza_finale);
                        }
                        else{
                                printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza 1 [deg] : %lf\n\tAmpiezza 2 [deg] : %lf\n\tAmpiezza finale [deg] : %lf\n",
                                                                            set_comandi[i].grad_scontr.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].grad_scontr.ampiezza_1*RAD_TO_DEG,
                                                                            set_comandi[i].grad_scontr.ampiezza_2*RAD_TO_DEG,set_comandi[i].grad_scontr.ampiezza_finale*RAD_TO_DEG);
                        }
                        break;
                    case sinusoidale:
                        printf("\tT inizio : %lf\n\tPeriodo: %lf\n",set_comandi[i].sin.t_inizio,set_comandi[i].sin.periodo);
                        if(i==c_rpm){
                                printf("\tValore medio [rpm] : %lf\n\tFase [deg] : %lf\n\tAmpiezza [rpm] : %lf\n",
                                                                            set_comandi[i].sin.valore_medio,set_comandi[i].sin.fase*RAD_TO_DEG,
                                                                            set_comandi[i].sin.ampiezza);
                        }
                        else{
                                printf("\tValore medio [deg] : %lf\n\tFase [deg] : %lf\n\tAmpiezza [deg] : %lf\n",
                                                                            set_comandi[i].sin.valore_medio,set_comandi[i].sin.fase*RAD_TO_DEG,
                                                                            set_comandi[i].sin.ampiezza);
                        }
                        break;
                    case rampa_scontrata:
                        printf("\tT inizio : %lf\n\tT 1 : %lf\n\tT 2 : %lf\n\tT 3 : %lf\n\tT 4 : %lf\n\tT finale : %lf\n",
                               set_comandi[i].rampa_scontr.t_inizio,set_comandi[i].rampa_scontr.t_1,set_comandi[i].rampa_scontr.t_2,set_comandi[i].rampa_scontr.t_3,
                               set_comandi[i].rampa_scontr.t_4,set_comandi[i].rampa_scontr.t_finale);
                        if(i==c_rpm){
                                printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza 1 [rpm] : %lf\n\tAmpiezza 2 [rpm] : %lf\n\tAmpiezza finale [rpm] : %lf\n",
                                                                            set_comandi[i].rampa_scontr.ampiezza_iniziale,set_comandi[i].rampa_scontr.ampiezza_1,
                                                                            set_comandi[i].rampa_scontr.ampiezza_2,set_comandi[i].rampa_scontr.ampiezza_finale);
                        }
                        else{
                                printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza 1 [deg] : %lf\n\tAmpiezza 2 [deg] : %lf\n\tAmpiezza finale [deg] : %lf\n",
                                                                            set_comandi[i].rampa_scontr.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].rampa_scontr.ampiezza_1*RAD_TO_DEG,
                                                                            set_comandi[i].rampa_scontr.ampiezza_2*RAD_TO_DEG,set_comandi[i].rampa_scontr.ampiezza_finale*RAD_TO_DEG);
                        }
                        break;
                    case trapezio:
                        printf("\tT inizio : %lf\n\tT 1 : %lf\n\tT 2 : %lf\n\tT fine : %lf\n",
                               set_comandi[i].rampa_scontr.t_inizio,set_comandi[i].rampa_scontr.t_1,set_comandi[i].rampa_scontr.t_4,set_comandi[i].rampa_scontr.t_finale);
                        if(i==c_rpm){
                                printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza regime [rpm] : %lf\n\tAmpiezza finale [rpm] : %lf\n",
                                                                            set_comandi[i].rampa_scontr.ampiezza_iniziale,set_comandi[i].rampa_scontr.ampiezza_1,
                                                                            set_comandi[i].rampa_scontr.ampiezza_finale);
                        }
                        else{
                                printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza regime [deg] : %lf\n\tAmpiezza finale [deg] : %lf\n",
                                                                            set_comandi[i].rampa_scontr.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].rampa_scontr.ampiezza_1*RAD_TO_DEG,
                                                                            set_comandi[i].rampa_scontr.ampiezza_finale*RAD_TO_DEG);
                        }
                        break;
                    case trapezio_scontrato:
                        printf("\tT inizio : %lf\n\tDelta T rampa : %lf\n\tDelta T regime : %lf\n\tDelta T ritardo dello scontro : %lf\n",
                               set_comandi[i].trap_scontr.t_inizio,set_comandi[i].trap_scontr.Dt_rampa,set_comandi[i].trap_scontr.Dt_regime,set_comandi[i].trap_scontr.Dt_ritardo);
                        if(i==c_rpm){
                                printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza regime [rpm] : %lf\n",
                                                                            set_comandi[i].trap_scontr.ampiezza_iniziale,set_comandi[i].trap_scontr.ampiezza_regime);
                        }
                        else{
                                printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza regime [deg] : %lf\n",
                                                                            set_comandi[i].trap_scontr.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].trap_scontr.ampiezza_regime*RAD_TO_DEG);
                        }
                        break;
                    }
                        }
                        printf("\nPremere un tasto per continuare\n");
                        getchar();
            break;
    }
    return menu;
};

t_schermata_menu menu_integrazione(char *file_out_stati,char *file_out_com,char *file_out_w_e,t_res_trim res_trim,t_dba dba,t_dbp dbp,
                                   t_descrizione_aereo desc_aer,t_descrizione_propeller desc_prop,t_atm_sl atm_sl,t_engine engine,t_limiti_comando limiti_comandi,
                                   t_massa_combustibile mass_comb){
    FILE *out_stati,*out_com;
    t_schermata_menu menu=m_inizio;
    t_stato stato=res_trim.stato;
    t_comando com;
    double t,T;
    int errore=0;

    out_stati=fopen(file_out_stati,"w");
    out_com=fopen(file_out_com,"w");
    apri_file_w_e(file_out_w_e);
    printf("\n\nT fine simulazione [s] : ");
    scanf("%lf",&T);
    getchar();
    fprintf(out_stati,"t\tu\tv\tw\tp\tq\tr\tphi\tpsi\ttheta\th\n");
    fprintf(out_com,"t\tde\tda\tdr\trpm\n");

    t=0;
    com=comando(t);
    fprintf(out_com,"%.16e\t%.16e\t%.16e\t%.16e\t%.16e\n",t,com.de,com.da,com.dr,com.rpm);
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
    t+=DT;

    while(t<=T&&!errore){
        com=comando(t);
        errore=all_check(&desc_aer,&desc_prop,&stato,&dbp,&atm_sl,t,DT,&engine,&limiti_comandi,&com,&mass_comb);
        if(errore!=1){
            fprintf(out_com,"%.16e\t%.16e\t%.16e\t%.16e\t%.16e\n",t,com.de,com.da,com.dr,com.rpm);
            stato=integra(&stato,&com,&dba,&dbp,&desc_aer,&desc_prop,&atm_sl,DT);
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
            if((int)(t*100.0)%100<1){
                printf("\rt=%lf",t);
            }
            t+=DT;
        }
    }
    fclose(out_stati);
    fclose(out_com);
    chiudi_file_w_e();

    printf("\n\nFine della simulazione\nEseguire post_process.m nella cartella \\Dati per visualizzare i risultati\n\nPremere un tasto per continuare");
    getchar();
    return menu;
};

t_schermata_menu menu_uscita(int *esci){
    t_schermata_menu menu=m_uscita;
    char selezione,c;
    printf("\n\nSi desidera terminare il programma S/N : ");
    while((c=getchar())!='\n')selezione=c;
    switch(selezione){
        case 's':
        case 'S':
            *esci=1;
            break;
        case 'n':
        case 'N':
            menu=m_inizio;
            break;
    }
    return menu;
};

t_schermata_menu menu_resoconto(t_settato *settato,t_atm_sl *atm_sl,double *h,double *vel,double *gamma,t_res_trim *res_trim){
    t_schermata_menu menu=m_inizio;
    int i;
    char *comando[4]={"da","de","dr","rpm"};
    char *tipo[10]={"base","impulso","gradino","rampa","costante","sinusoidale","gradino scontrato","rampa scontrata","trapezio","trapezio scontrato"};
    //stampare l'atmosfera
    if(!settato->atm_sl&&!settato->gamma&&!settato->h&&!settato->press&&!settato->rho&&!settato->temp&&!settato->trim&&!settato->vel){
        printf("\n\nNessun parametro settato\n\n");
        printf("Premere un tasto per continuare\n");
        getchar();
    }
    else{
        if(settato->h||settato->press||settato->rho){
            printf("\n\nAtmosfera al livello del mare :\n");
            if(settato->temp)
                printf("T : %lf K\n",atm_sl->temp);
            if(settato->rho)
                printf("Rho : %lf kg/m^3\n",atm_sl->dens);
            if(settato->press)
                printf("P : %lf Pa\n",atm_sl->press);
            printf("\nPremere un tasto per continuare\n");
            getchar();
        }
        //stampare la condizione di trim imposta
        if(settato->h||settato->vel||settato->gamma){
            printf("\n\nParametri settati per il trim :\n");
            if(settato->h)
                printf("h : %lf m\n",*h);
            if(settato->vel)
                printf("V : %lf m/s\n",*vel);
            if(settato->gamma)
                printf("gamma : %lf deg\n",*gamma*RAD_TO_DEG);
            printf("\nPremere un tasto per continuare\n");
            getchar();
        }

        if(settato->trim){
            printf("\nVettore di stato nella condizione di trim :\n");
                printf("u=%lf m/s\nv=%lf m/s\nw=%lf m/s\np=%lf deg/s\nq=%lf deg/s\nr=%lf deg/s\nphi=%lf deg\npsi=%lf deg\ntheta=%lf deg \nh=%lf h\n\n",  res_trim->stato.u,
                                                                                                            res_trim->stato.v,
                                                                                                            res_trim->stato.w,
                                                                                                            res_trim->stato.p*RAD_TO_DEG,
                                                                                                            res_trim->stato.q*RAD_TO_DEG,
                                                                                                            res_trim->stato.r*RAD_TO_DEG,
                                                                                                            res_trim->stato.phi*RAD_TO_DEG,
                                                                                                            res_trim->stato.psi*RAD_TO_DEG,
                                                                                                            res_trim->stato.theta*RAD_TO_DEG,
                                                                                                            res_trim->stato.h);
                printf("Premere un tasto per continuare\n");
                getchar();


                printf("\nVettore di comando nella condizione di trim :\n");
                printf("da=%lf deg\nde=%lf deg\ndr=%lf deg\nrpm=%lf rpm\n\n",res_trim->comando.da*RAD_TO_DEG,res_trim->comando.de*RAD_TO_DEG,res_trim->comando.dr*RAD_TO_DEG,res_trim->comando.rpm);


                printf("Premere un tasto per continuare\n");
                getchar();


                printf("\n\nComandi impostati\n\n");
                for(i=0;i<4;i++){
                    printf("\n\n%s :\n\tTipo : %s\n",comando[i],tipo[set_comandi[i].base.tipo]);
                    switch(set_comandi[i].base.tipo){
                        case base:
                            break;
                        case gradino:
                            printf("\tT inizio : %lf\n\tT fine : %lf\n",set_comandi[i].gradino.t_inizio,set_comandi[i].gradino.t_fine);
                            if(i==c_rpm){
                                printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza a regime [rpm] : %lf\n",set_comandi[i].gradino.ampiezza_iniziale,set_comandi[i].gradino.ampiezza_regime);
                            }
                            else{
                                printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza a regime [deg] : %lf\n",set_comandi[i].gradino.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].gradino.ampiezza_regime*RAD_TO_DEG);
                            }
                            break;
                        case rampa:
                            printf("\tT inizio : %lf\n\tT fine : %lf\n",set_comandi[i].rampa.t_inizio,set_comandi[i].rampa.t_fine);
                            if(i==c_rpm){
                                printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza finale [rpm] : %lf\n",set_comandi[i].rampa.ampiezza_iniziale,set_comandi[i].rampa.ampiezza_finale);
                            }
                            else{
                                printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza finale [deg] : %lf\n",set_comandi[i].rampa.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].rampa.ampiezza_finale*RAD_TO_DEG);
                            }
                            break;
                        case impulso:
                            printf("\tT inizio : %lf\n\tdT : %lf\n",set_comandi[i].impulso.t_impulso,set_comandi[i].impulso.dt);
                            if(i==c_rpm){
                                printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza impulso [rpm] : %lf\n",set_comandi[i].impulso.ampiezza_iniziale,set_comandi[i].impulso.ampiezza_impulso);
                            }
                            else{
                                printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza impulso [deg] : %lf\n",set_comandi[i].impulso.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].impulso.ampiezza_impulso*RAD_TO_DEG);
                            }
                            break;
                        case costante:
                            if(i==c_rpm){
                                    printf("\tAmpiezza [rpm] : %lf\n",set_comandi[i].costante.ampiezza);
                            }
                            else{
                                    printf("\tAmpiezza [deg] : %lf\n",set_comandi[i].costante.ampiezza*RAD_TO_DEG);
                            }
                            break;
                        case gradino_scontrato:
                            printf("\tT inizio : %lf\n\tT 1 : %lf\n\tT 2 : %lf\n\tT finale : %lf\n",set_comandi[i].grad_scontr.t_inizio,set_comandi[i].grad_scontr.t_1,set_comandi[i].grad_scontr.t_2,set_comandi[i].grad_scontr.t_finale);
                            if(i==c_rpm){
                                    printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza 1 [rpm] : %lf\n\tAmpiezza 2 [rpm] : %lf\n\tAmpiezza finale [rpm] : %lf\n",
                                                                                set_comandi[i].grad_scontr.ampiezza_iniziale,set_comandi[i].grad_scontr.ampiezza_1,
                                                                                set_comandi[i].grad_scontr.ampiezza_2,set_comandi[i].grad_scontr.ampiezza_finale);
                            }
                            else{
                                    printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza 1 [deg] : %lf\n\tAmpiezza 2 [deg] : %lf\n\tAmpiezza finale [deg] : %lf\n",
                                                                                set_comandi[i].grad_scontr.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].grad_scontr.ampiezza_1*RAD_TO_DEG,
                                                                                set_comandi[i].grad_scontr.ampiezza_2*RAD_TO_DEG,set_comandi[i].grad_scontr.ampiezza_finale*RAD_TO_DEG);
                            }
                            break;
                        case sinusoidale:
                            printf("\tT inizio : %lf\n\tPeriodo: %lf\n",set_comandi[i].sin.t_inizio,set_comandi[i].sin.periodo);
                            if(i==c_rpm){
                                    printf("\tValore medio [rpm] : %lf\n\tFase [deg] : %lf\n\tAmpiezza [rpm] : %lf\n",
                                                                                set_comandi[i].sin.valore_medio,set_comandi[i].sin.fase*RAD_TO_DEG,
                                                                                set_comandi[i].sin.ampiezza);
                            }
                            else{
                                    printf("\tValore medio [deg] : %lf\n\tFase [deg] : %lf\n\tAmpiezza [deg] : %lf\n",
                                                                                set_comandi[i].sin.valore_medio,set_comandi[i].sin.fase*RAD_TO_DEG,
                                                                                set_comandi[i].sin.ampiezza);
                            }
                            break;
                        case rampa_scontrata:
                            printf("\tT inizio : %lf\n\tT 1 : %lf\n\tT 2 : %lf\n\tT 3 : %lf\n\tT 4 : %lf\n\tT finale : %lf\n",
                                   set_comandi[i].rampa_scontr.t_inizio,set_comandi[i].rampa_scontr.t_1,set_comandi[i].rampa_scontr.t_2,set_comandi[i].rampa_scontr.t_3,
                                   set_comandi[i].rampa_scontr.t_4,set_comandi[i].rampa_scontr.t_finale);
                            if(i==c_rpm){
                                    printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza 1 [rpm] : %lf\n\tAmpiezza 2 [rpm] : %lf\n\tAmpiezza finale [rpm] : %lf\n",
                                                                                set_comandi[i].rampa_scontr.ampiezza_iniziale,set_comandi[i].rampa_scontr.ampiezza_1,
                                                                                set_comandi[i].rampa_scontr.ampiezza_2,set_comandi[i].rampa_scontr.ampiezza_finale);
                            }
                            else{
                                    printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza 1 [deg] : %lf\n\tAmpiezza 2 [deg] : %lf\n\tAmpiezza finale [deg] : %lf\n",
                                                                                set_comandi[i].rampa_scontr.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].rampa_scontr.ampiezza_1*RAD_TO_DEG,
                                                                                set_comandi[i].rampa_scontr.ampiezza_2*RAD_TO_DEG,set_comandi[i].rampa_scontr.ampiezza_finale*RAD_TO_DEG);
                            }
                            break;
                        case trapezio:
                            printf("\tT inizio : %lf\n\tT 1 : %lf\n\tT 2 : %lf\n\tT fine : %lf\n",
                                   set_comandi[i].rampa_scontr.t_inizio,set_comandi[i].rampa_scontr.t_1,set_comandi[i].rampa_scontr.t_4,set_comandi[i].rampa_scontr.t_finale);
                            if(i==c_rpm){
                                    printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza regime [rpm] : %lf\n\tAmpiezza finale [rpm] : %lf\n",
                                                                                set_comandi[i].rampa_scontr.ampiezza_iniziale,set_comandi[i].rampa_scontr.ampiezza_1,
                                                                                set_comandi[i].rampa_scontr.ampiezza_finale);
                            }
                            else{
                                    printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza regime [deg] : %lf\n\tAmpiezza finale [deg] : %lf\n",
                                                                                set_comandi[i].rampa_scontr.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].rampa_scontr.ampiezza_1*RAD_TO_DEG,
                                                                                set_comandi[i].rampa_scontr.ampiezza_finale*RAD_TO_DEG);
                            }
                            break;
                        case trapezio_scontrato:
                            printf("\tT inizio : %lf\n\tDelta T rampa : %lf\n\tDelta T regime : %lf\n\tDelta T ritardo dello scontro : %lf\n",
                                   set_comandi[i].trap_scontr.t_inizio,set_comandi[i].trap_scontr.Dt_rampa,set_comandi[i].trap_scontr.Dt_regime,set_comandi[i].trap_scontr.Dt_ritardo);
                            if(i==c_rpm){
                                    printf("\tAmpiezza iniziale [rpm] : %lf\n\tAmpiezza regime [rpm] : %lf\n",
                                                                                set_comandi[i].trap_scontr.ampiezza_iniziale,set_comandi[i].trap_scontr.ampiezza_regime);
                            }
                            else{
                                    printf("\tAmpiezza iniziale [deg] : %lf\n\tAmpiezza regime [deg] : %lf\n",
                                                                                set_comandi[i].trap_scontr.ampiezza_iniziale*RAD_TO_DEG,set_comandi[i].trap_scontr.ampiezza_regime*RAD_TO_DEG);
                            }
                            break;
                        }
                }
                printf("\nPremere un tasto per continuare\n");
                getchar();
        }
    }
    return menu;
};

t_schermata_menu menu_tipo_comando(t_componente_comando comp_com,t_res_trim res_trim){
    t_schermata_menu menu=m_tipo_comando;
    u_funz_comando u_f_com;
    u_com com;
    int scelta,delta,c;
    scelta=0;
    delta=0;
    double tmp;
    printf("\n\nDigitare il numero per accedere alla sezione voluta\n\n(0) Tornare al menu precedente\n(1) Settaggio del comando impulso\n(2) Settaggio del comando gradino\n");
    printf("(3) Settaggio del comando rampa\n(4) Reset del comando costante alla condizione di trim\n(5) Settaggio del comando sinusoidale\n");
    printf("(6) Settaggio del comando a gradino scontrato\n(7) Settaggio del comando a rampa scontrata\n(8) Settaggio del comando trapezio\n");
    printf("(9) Settaggio del comando trapezio scontrato\n\n");
    while((c=getchar())!='\n')scelta=c;
    com.com=res_trim.comando;
    switch(scelta){
        case '0':
            menu=m_comando;
            break;
        case '1':
            u_f_com.impulso.tipo=impulso;
            printf("\nT inizio [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.impulso.t_impulso=tmp;
            u_f_com.impulso.ampiezza_iniziale=com.v_com[comp_com];
            u_f_com.impulso.dt=DT;
            while(!(delta=='0'||delta=='1')){
                printf("\n\nDigitare il numero per\n\n(0) Settaggio del comando in modo assoluto\n(1) Settare il comando differenziale rispetto alla condizione di trim\n");
                while((c=getchar())!='\n')delta=c;
            }
            if(comp_com==c_rpm){
                printf("\nAmpiezza [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                if(delta=='1'){
                    u_f_com.impulso.ampiezza_impulso=u_f_com.impulso.ampiezza_iniziale+tmp;
                }
                else if(delta=='0'){
                    u_f_com.impulso.ampiezza_impulso=tmp;
                }
            }
            else{
                printf("\nAmpiezza [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                if(delta=='1'){
                    u_f_com.impulso.ampiezza_impulso=u_f_com.impulso.ampiezza_iniziale+tmp*DEG_TO_RAD;
                }
                else if(delta=='0'){
                    u_f_com.impulso.ampiezza_impulso=tmp*DEG_TO_RAD;
                }
            }
            set_comando(u_f_com,comp_com);
            break;
        case '2':
            u_f_com.gradino.tipo=gradino;
            u_f_com.gradino.ampiezza_iniziale=com.v_com[comp_com];

            printf("\nT inizio [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.gradino.t_inizio=tmp;

            printf("\nT fine [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.gradino.t_fine=tmp;

            while(!(delta=='0'||delta=='1')){
                printf("\n\nDigitare il numero per\n\n(0) Settaggio del comando in modo assoluto\n(1) Settare il comando differenziale rispetto alla condizione di trim\n");
                while((c=getchar())!='\n')delta=c;
            }
            if(comp_com==c_rpm){
                printf("\nAmpiezza a regime [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                if(delta=='1'){
                    u_f_com.gradino.ampiezza_regime=u_f_com.gradino.ampiezza_iniziale+tmp;
                }
                else if(delta=='0'){
                    u_f_com.gradino.ampiezza_regime=tmp;
                }
            }
            else{
                printf("\nAmpiezza [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                if(delta=='1'){
                    u_f_com.gradino.ampiezza_regime=u_f_com.gradino.ampiezza_iniziale+tmp*DEG_TO_RAD;
                }
                else if(delta=='0'){
                    u_f_com.gradino.ampiezza_regime=tmp*DEG_TO_RAD;
                }
            }
            set_comando(u_f_com,comp_com);
            break;
        case '3':
            u_f_com.rampa.tipo=rampa;
            u_f_com.rampa.ampiezza_iniziale=com.v_com[comp_com];

            printf("\nT inizio [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa.t_inizio=tmp;

            printf("\nT fine [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa.t_fine=tmp;

            while(!(delta=='0'||delta=='1')){
                printf("\n\nDigitare il numero per\n\n(0) Settaggio del comando in modo assoluto\n(1) Settare il comando differenziale rispetto alla condizione di trim\n");
                while((c=getchar())!='\n')delta=c;
            }
            if(comp_com==c_rpm){
                printf("\nAmpiezza finale [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                if(delta=='1'){
                    u_f_com.rampa.ampiezza_finale=u_f_com.rampa.ampiezza_iniziale+tmp;
                }
                else if(delta=='0'){
                    u_f_com.rampa.ampiezza_finale=tmp;
                }
            }
            else{
                printf("\nAmpiezza [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                if(delta=='1'){
                    u_f_com.rampa.ampiezza_finale=u_f_com.rampa.ampiezza_iniziale+tmp*DEG_TO_RAD;
                }
                else if(delta=='0'){
                    u_f_com.rampa.ampiezza_finale=tmp*DEG_TO_RAD;
                }
            }
            set_comando(u_f_com,comp_com);
            break;
        case '4':
            u_f_com.costante.tipo=costante;
            u_f_com.costante.ampiezza=com.v_com[comp_com];
            set_comando(u_f_com,comp_com);
            break;
        case '5':
            u_f_com.sin.tipo=sinusoidale;
            u_f_com.sin.valore_medio=com.v_com[comp_com];

            printf("\nT inizio [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.sin.t_inizio=tmp;

            printf("\nPeriodo [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.sin.periodo=tmp;

            printf("\nFase [deg] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.sin.fase=tmp*DEG_TO_RAD;

            if(comp_com==c_rpm){
                printf("\nAmpiezza rispetto al valore di trim [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.sin.ampiezza=tmp;
            }
            else{
                printf("\nAmpiezza rispetto al valore di trim [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.sin.ampiezza=tmp*DEG_TO_RAD;
            }
            set_comando(u_f_com,comp_com);
            break;
        case '6':
            u_f_com.grad_scontr.tipo=gradino_scontrato;
            u_f_com.grad_scontr.ampiezza_iniziale=com.v_com[comp_com];
            u_f_com.grad_scontr.ampiezza_finale=com.v_com[comp_com];

            printf("\nT inizio [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.grad_scontr.t_inizio=tmp;

            printf("\nT 1 [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.grad_scontr.t_1=tmp;

            printf("\nT 2 [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.grad_scontr.t_2=tmp;

            printf("\nT finale [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.grad_scontr.t_finale=tmp;

            if(comp_com==c_rpm){
                printf("\nAmpiezza rispetto al valore di trim, prima parte [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.grad_scontr.ampiezza_1=com.v_com[comp_com]+tmp;
            }
            else{
                printf("\nAmpiezza rispetto al valore di trim, prima parte [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.grad_scontr.ampiezza_1=com.v_com[comp_com]+tmp*DEG_TO_RAD;
            }

            if(comp_com==c_rpm){
                printf("\nAmpiezza rispetto al valore di trim, seconda parte [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.grad_scontr.ampiezza_2=com.v_com[comp_com]+tmp;
            }
            else{
                printf("\nAmpiezza rispetto al valore di trim, seconda parte [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.grad_scontr.ampiezza_2=com.v_com[comp_com]+tmp*DEG_TO_RAD;
            }
            set_comando(u_f_com,comp_com);
            break;
        case '7':
            u_f_com.rampa_scontr.tipo=rampa_scontrata;
            u_f_com.rampa_scontr.ampiezza_iniziale=com.v_com[comp_com];
            u_f_com.rampa_scontr.ampiezza_intermedia=com.v_com[comp_com];
            u_f_com.rampa_scontr.ampiezza_finale=com.v_com[comp_com];

            printf("\nT inizio [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_inizio=tmp;

            printf("\nT valore massimo/minimo nella prima parte [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_1=tmp;

            printf("\nT fine prima parte [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_2=tmp;

            printf("\nT inizio seconda rampa [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_3=tmp;

            printf("\nT valore massimo/minimo nella seconda parte [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_4=tmp;

            printf("\nT finale [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_finale=tmp;

            if(comp_com==c_rpm){
                printf("\nAmpiezza rispetto al valore di trim, prima parte [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.rampa_scontr.ampiezza_1=com.v_com[comp_com]+tmp;
            }
            else{
                printf("\nAmpiezza rispetto al valore di trim, prima parte [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.rampa_scontr.ampiezza_1=com.v_com[comp_com]+tmp*DEG_TO_RAD;
            }

            if(comp_com==c_rpm){
                printf("\nAmpiezza rispetto al valore di trim, seconda parte [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.rampa_scontr.ampiezza_2=com.v_com[comp_com]+tmp;
            }
            else{
                printf("\nAmpiezza rispetto al valore di trim, seconda parte [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.rampa_scontr.ampiezza_2=com.v_com[comp_com]+tmp*DEG_TO_RAD;
            }
            set_comando(u_f_com,comp_com);
            break;
        case '8':
            u_f_com.rampa_scontr.tipo=trapezio;
            u_f_com.rampa_scontr.ampiezza_iniziale=com.v_com[comp_com];


            printf("\nT inizio [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_inizio=tmp;

            printf("\nT 1 [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_1=tmp;

            printf("\nT 2 [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_2=tmp;
            u_f_com.rampa_scontr.t_3=tmp;
            u_f_com.rampa_scontr.t_4=tmp;

            printf("\nT finale [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.rampa_scontr.t_finale=tmp;

            if(comp_com==c_rpm){
                printf("\nAmpiezza regime rispetto al valore di trim [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.rampa_scontr.ampiezza_1=com.v_com[comp_com]+tmp;
                u_f_com.rampa_scontr.ampiezza_intermedia=com.v_com[comp_com]+tmp;
                u_f_com.rampa_scontr.ampiezza_2=com.v_com[comp_com]+tmp;
            }
            else{
                printf("\nAmpiezza regime rispetto al valore di trim [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.rampa_scontr.ampiezza_1=com.v_com[comp_com]+tmp*DEG_TO_RAD;
                u_f_com.rampa_scontr.ampiezza_intermedia=com.v_com[comp_com]+tmp*DEG_TO_RAD;
                u_f_com.rampa_scontr.ampiezza_2=com.v_com[comp_com]+tmp*DEG_TO_RAD;
            }

            while(!(delta=='0'||delta=='1')){
                printf("\n\nDigitare il numero per\n\n(0) Settare l'ampiezza finale uguale a quella di trim iniziale\n(1) Settare l'ampiezza finale diversa da quella di trim iniziale\n");
                while((c=getchar())!='\n')delta=c;
            }

            if(delta=='0'){
                u_f_com.rampa_scontr.ampiezza_finale=com.v_com[comp_com];
            }
            else if(delta=='1'){
                if(comp_com==c_rpm){
                    printf("\nAmpiezza finale [rpm] : ");
                    scanf("%lf",&tmp);
                    getchar();
                    u_f_com.rampa_scontr.ampiezza_finale=tmp;
                }
                else{
                    printf("\nAmpiezza finale [deg] : ");
                    scanf("%lf",&tmp);
                    getchar();
                    u_f_com.rampa_scontr.ampiezza_finale=tmp*DEG_TO_RAD;
                }
            }


            set_comando(u_f_com,comp_com);
            break;
        case '9':
            u_f_com.trap_scontr.tipo=trapezio_scontrato;
            u_f_com.trap_scontr.ampiezza_iniziale=com.v_com[comp_com];


            printf("\nT inizio [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.trap_scontr.t_inizio=tmp;

            printf("\nDelta T rampa [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.trap_scontr.Dt_rampa=tmp;

            printf("\nDelta T regime [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.trap_scontr.Dt_regime=tmp;

            printf("\nDelta T ritardo dello scontro [s] : ");
            scanf("%lf",&tmp);
            getchar();
            u_f_com.trap_scontr.Dt_ritardo=tmp;

            if(comp_com==c_rpm){
                printf("\nAmpiezza regime rispetto al valore di trim [rpm] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.trap_scontr.ampiezza_regime=com.v_com[comp_com]+tmp;
            }
            else{
                printf("\nAmpiezza regime rispetto al valore di trim [deg] : ");
                scanf("%lf",&tmp);
                getchar();
                u_f_com.trap_scontr.ampiezza_regime=com.v_com[comp_com]+tmp*DEG_TO_RAD;
            }
            set_comando(u_f_com,comp_com);
            break;
    }
    return menu;
};
