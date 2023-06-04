#ifndef CHECKING_H_INCLUDED
#define CHECKING_H_INCLUDED

#include <stdio.h>
#include "aeromobile.h"
#include "interpola.h"
#include "trim.h"
#include "lettura_dati.h"
#include "routh.h"
#include <math.h>


#define V_STALLO 30 //[m/s]
#define V_NE 75 //[m/s]
#define RESIDUO_POTENZA 5 //[W]
#define RESIDUO_CHECKING_PROP 1e-3
#define KW_TO_W 1000
#define QUOTA_TANGENZA 4116

extern FILE* out_w_e;
extern int errore_interpola; //nel file interpola.c
extern int errore_lettura;//nel file lettura.c
extern int errore_propeller;//nel file propeller.c
extern int errore_trim;//nel file trim.c

int apri_file_w_e(const char* nomefile);
int chiudi_file_w_e();


int all_check(const t_descrizione_aereo *desc_aer,const t_descrizione_propeller *desc_prop, const t_stato *stato,const  t_dbp *dbp,const  t_atm_sl *atm_sl,
                             double t,double dt,const t_engine *engine ,t_limiti_comando *lim_com,t_comando *com,t_massa_combustibile *mass_comb);
int check_interpola(double t);
int check_comandi(t_comando* comando,const t_limiti_comando *lim_com,const t_engine *engine, double t);
int check_potenza_motore(const t_descrizione_propeller *desc_prop, const t_stato *stato,const  t_dbp *dbp,const  t_atm_sl *atm_sl, const double t,const t_engine *engine , double *rpm_com);
int check_vel(const t_stato *stato, double t);
int check_h(const t_stato *stato, double t);
int check_carburante(const t_descrizione_aereo* desc_aer,const t_stato *stato,const t_atm_sl *atm_sl,const t_engine *engine,t_massa_combustibile *mass_comb, double t, double dt);
int check_trim();
int check_prop(double t);
int check_stab(const t_res_routh *res_routh);



#endif // CHECKING_H_INCLUDED
