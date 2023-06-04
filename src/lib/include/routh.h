#ifndef ROUTH_H_INCLUDED
#define ROUTH_H_INCLUDED

#include "trim.h"
#include "interpola.h"
#include "atmosfera.h"

#define const_g 9.81
#define TRIM_k 0.047
#define C_D0 0.0235
#define C_LAP 1.56 //rad^-1
#define C_Tu -0.0841 //rad^-1
#define C_Du 0
#define C_mu 0
#define C_Lu 0


typedef struct{
    int is_stat_stab; //Pari a 1 se velivolo è staticamente stabile, pari a 0 se velivolo non è staticamente stabile
    int is_din_stab; //Pari a 1 se velivolo è dinamicamente stabile, pari a 0 se velivolo non è dinamicamente stabile
    double omega_sp;
    double smorz_sp;
    double periodo_sp;
    double dimezzamento_sp;
    double omega_ph;
    double smorz_ph;
    double periodo_ph;
    double dimezzamento_ph;
}t_res_routh;

t_res_routh routh(t_res_trim trim,t_descrizione_aereo desc_aer,t_atm_sl atm_sl,t_dba dba);

#endif // ROUTH_H_INCLUDED
