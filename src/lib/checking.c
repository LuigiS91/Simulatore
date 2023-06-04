#include "checking.h"

FILE* out_w_e=0;

int apri_file_w_e(const char* nomefile){
    out_w_e=fopen(nomefile,"w+");
    if(out_w_e==0){
        printf("\n[-] Nessun file presente nella posizione %s\n",nomefile);
        return -1;
    }
    return 0;
}

int chiudi_file_w_e(){
    int errore=0;
    if(out_w_e==0){
        return -1;
    }
    errore=fclose(out_w_e);
    out_w_e=0;
    return errore;
}

int all_check(const t_descrizione_aereo *desc_aer,const t_descrizione_propeller *desc_prop, const t_stato *stato,const  t_dbp *dbp,const  t_atm_sl *atm_sl,
                             double t,double dt,const t_engine *engine ,t_limiti_comando *lim_com,t_comando *com,t_massa_combustibile *mass_comb){
    int errore_grave=0;
    if(check_interpola(t)) errore_grave=1;
    else if(check_prop(t)) errore_grave=1;
    else if(check_h(stato,t)==1)errore_grave=1;
    else if(check_vel(stato,t)==1)errore_grave=1;
    else if(check_potenza_motore(desc_prop,stato,dbp,atm_sl,t,engine,&com->rpm)==1)errore_grave=1;
    else{
        check_comandi(com,lim_com,engine,t);
        check_carburante(desc_aer,stato,atm_sl,engine,mass_comb,t,dt);
    }
return errore_grave;
}

int check_interpola(double t){
    int errore=errore_interpola;
    if(errore){
        printf("\n[-] Errore interpolazione\n");
        fprintf(out_w_e,"t = %lf\n\t[-] Errore interpolazione\n",t);
        errore_interpola=0;
    }
    return errore;
}

int check_comandi(t_comando* comando,const t_limiti_comando *lim_com,const t_engine *engine, double t){

    int errore_comando=0;

    if (comando->de>lim_com->max_elevator){
        comando->de=lim_com->max_elevator;
        fprintf(out_w_e,"t = %lf\n\t[!] Equilibratore a fine corsa massima\n",t);
        errore_comando=1;
    }
    if (comando->de<lim_com->min_elevator){
        comando->de=lim_com->min_elevator;
        fprintf(out_w_e,"t = %lf\n\t[!] Equilibratore a fine corsa minima\n",t);
        errore_comando=1;
    }
    if (comando->da>lim_com->airelon){
        comando->da=lim_com->airelon;
        fprintf(out_w_e,"t = %lf\n\t[!] Alettone a fine corsa massima\n",t);
        errore_comando=1;
    }
    if (comando->da<-lim_com->airelon){
        comando->da=-lim_com->airelon;
        fprintf(out_w_e,"t = %lf\n\t[!] Alettone a fine corsa minima\n",t);
        errore_comando=1;
    }
    if (comando->dr>lim_com->rudder){
        comando->dr=lim_com->rudder;
        fprintf(out_w_e,"t = %lf\n\t[!] Timone a fine corsa massima\n",t);
        errore_comando=1;
    }
    if (comando->dr<-lim_com->rudder){
        comando->dr=-lim_com->rudder;
        fprintf(out_w_e,"t = %lf\n\t[!] Timone a fine corsa minima\n",t);
        errore_comando=1;
    }
    if (comando->rpm>engine->max_rpm){
        comando->rpm=engine->max_rpm;
        fprintf(out_w_e,"t = %lf\n\t[!] Motore al massimo numero di giri\n",t);
        errore_comando=1;
    }
    if (comando->rpm<engine->min_rpm){
        comando->rpm=engine->min_rpm;
        fprintf(out_w_e,"t = %lf\n\t[!] Motore al minimo numero di giri\n",t);
        errore_comando=1;
    }
    return errore_comando;
}

int check_potenza_motore(const t_descrizione_propeller *desc_prop, const t_stato *stato,const  t_dbp *dbp,const  t_atm_sl *atm_sl, const double t,const t_engine *engine , double *rpm_com){
    int errore_potenza_motore=0,finito=0,i;
    double potenza_disponibile,rpm_m1,rpm_p1,rpm,pot_nec,pot_nec_m1,vel,potenza_necessaria;
    t_atm atm;
    t_res_prop res_prop,res_prop_m1;

    atm=atmosfera(stato->h,*atm_sl);
    vel=sqrt(pow(stato->u,2.0)+pow(stato->v,2.0)+pow(stato->w,2.0));

    res_prop=propeller(desc_prop,dbp,&atm,vel,*rpm_com,RESIDUO_CHECKING_PROP);

    potenza_disponibile=engine->p_max*KW_TO_W*pow(atm.dens/atm_sl->dens,engine->esp);
    potenza_necessaria=res_prop.torque*(*rpm_com)*RPM_TO_ANGVEL;

    if (potenza_disponibile<potenza_necessaria){
            errore_potenza_motore=2;//warning non grave
            fprintf(out_w_e,"t = %lf\n\t[!] Potenza insufficiente, numero di giri motore ricalcolato\n",t);


        rpm_m1=*rpm_com;
        rpm=engine->min_rpm;

        i=0;
        while(!finito&&(errore_potenza_motore==2)){
            res_prop=propeller(desc_prop,dbp,&atm,vel,rpm,RESIDUO_CHECKING_PROP);
            pot_nec=res_prop.torque*rpm*RPM_TO_ANGVEL;
            if(fabs(pot_nec-potenza_disponibile)<RESIDUO_POTENZA){
                finito=1;
            }
            else{
                res_prop_m1=propeller(desc_prop,dbp,&atm,vel,rpm_m1,RESIDUO_CHECKING_PROP);
                pot_nec_m1=res_prop_m1.torque*rpm_m1*RPM_TO_ANGVEL;
                rpm_p1=rpm-(rpm-rpm_m1)/(pot_nec-pot_nec_m1)*(pot_nec-potenza_disponibile);
                rpm_m1=rpm;
                rpm=rpm_p1;
            }
            i++;
            if(i>500){
                errore_potenza_motore=1;
                printf("\n[-] Impossibile raggiungere la potenza necessaria, mancata convergenza\n");
                fprintf(out_w_e,"t = %lf\n\t[-] Impossibile raggiungere la potenza necessaria, mancata convergenza\n",t);
            }
        }
        if(rpm>engine->max_rpm||rpm<engine->min_rpm){
            errore_potenza_motore=1;
            printf("\n[-] Impossibile raggiungere la potenza necessaria, numero di giri fuori dai limiti\n");
            fprintf(out_w_e,"t = %lf\n\t[-] Impossibile raggiungere la potenza necessaria, numero di giri fuori dai limiti\n",t);
        }
        *rpm_com=rpm;
    }
    return errore_potenza_motore;
}

int check_vel(const t_stato *stato, double t){
    int errore_velocita=0;
    if ((sqrt(pow(stato->u,2.0)+pow(stato->v,2.0)+pow(stato->w,2.0)))<V_STALLO){
        errore_velocita=1; //Errore grave
        printf("t= %lf\n\t[-] Velocità inferiore alla velocità di stallo\n",t);
        fprintf(out_w_e,"t = %lf\n\t[-] Velocita' inferiore alla velocita' di stallo\n",t);
    }
    else if ((sqrt(pow(stato->u,2.0)+pow(stato->v,2.0)+pow(stato->w,2.0)))>V_NE){
        errore_velocita=1; //Errore grave
        printf("t= %lf\n\t[-] Velocità superiore alla Vne\n",t);
        fprintf(out_w_e,"t= %lf\n\t[-] Velocità superiore alla Vne\n",t);
    }

return errore_velocita;
}

int check_h(const t_stato *stato, double t){
    int errore_h=0;
    if (stato->h>QUOTA_TANGENZA){ //4116 è la quota di tangenza, slides
        errore_h=2;//warning non grave
        fprintf(out_w_e,"t = %lf\n\t[!] Velivolo ha superato la quota di tangenza\n",t);
    }
    else if (stato->h<=0){
        errore_h=1;
        printf("\n[-] Velivolo ha toccato terra\n");
        fprintf(out_w_e,"t = %lf\n\t[-] Velivolo ha toccato terra\n",t);
    }
    return errore_h;
}


int check_carburante(const t_descrizione_aereo* desc_aer,const t_stato *stato,const t_atm_sl *atm_sl,const t_engine *engine,t_massa_combustibile *mass_comb, double t, double dt){
    int errore_combustibile=0;
    double carb_cons, cons_spec_h;
    t_atm atm;

    atm=atmosfera(stato->h,*atm_sl);
    cons_spec_h=engine->sfc*pow(atm.dens/atm_sl->dens,engine->esp);
    carb_cons=cons_spec_h*dt;
    mass_comb->mf_mtow=mass_comb->mf_mtow-carb_cons/desc_aer->massa;
    if(mass_comb->mf_mtow<=0){
        errore_combustibile=1;
        fprintf(out_w_e,"t = %lf\n\t[!] Errore carburante, esaurito il carburante disponibile\n",t);
    }
return errore_combustibile;
}

int check_prop(double t){
    int errore=errore_propeller;
    if(errore){
        printf("\n[-] Errore propeller, mancata convergenze\n");
        fprintf(out_w_e,"t = %lf\n\t[-] Errore propeller, mancata convergenze\n",t);
        errore_propeller=0;
    }
    return errore;

};

int check_trim(){
    int errore=errore_trim;
    if(errore){
        printf("\n\n[-] Condizione di trim non trovata\n");
    }
    return errore;
}

int check_stab(const t_res_routh *res_routh){
    int errore=0;
    if (!(res_routh->is_stat_stab)){
        printf("\n\n[-] Il velivolo e' staticamente instabile\n");
        errore=1;
    }
    if (!(res_routh->is_din_stab)){
        printf("\n\n[-] Il velivolo e' dinamicamente instabile\n");
        errore=1;
    }
    return errore;
}

