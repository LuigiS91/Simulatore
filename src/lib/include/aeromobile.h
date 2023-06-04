#ifndef AEROMOBILE_H_INCLUDED
#define AEROMOBILE_H_INCLUDED

#include "lettura_dati.h"
#include "propeller.h"
#include "atmosfera.h"
#include "interpola.h"

#define const_g 9.81
#define DEG_TO_RAD 3.14/180.0
#define RAD_TO_DEG 180.0/3.14
#define RESIDUO_AEROMOBILE_PROP 1e-5


typedef struct{
    double u;
    double v;
    double w;
    double p;
    double q;
    double r;
    double phi;
    double theta;
    double psi;
    double h;
}t_stato;

typedef struct{
    double da;
    double de;
    double dr;
    double rpm;
}t_comando;

t_stato aeromobile(const t_stato *stato,const t_comando *com,const t_dba *dba,const t_dbp *dbp,const t_descrizione_aereo *desc_aer,const t_descrizione_propeller *desc_prop,const t_atm_sl *atm_sl);//La funzione restituisce la derivata dello stato dato lo stato attuale in funzione delle equazioni del moto



#endif // AEROMOBILE_H_INCLUDED
