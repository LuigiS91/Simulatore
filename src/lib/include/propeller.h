#ifndef PROPELLER_H_INCLUDED
#define PROPELLER_H_INCLUDED

#include <math.h>
#include "lettura_dati.h"
#include "atmosfera.h"


#define RPM_TO_ANGVEL 2.0*M_PI/60.0
// #define DEG_TO_RAD M_PI/180.0
// #define RAD_TO_DEG 180.0/M_PI
#define RESIDUO_PROPELLER_BASE 1e-5

extern int errore_propeller;

typedef struct{
    double thrust;
    double torque;
}t_res_prop;

/*Prototipo funzione propeller*/
t_res_prop propeller(const t_descrizione_propeller *desc_prop,const t_dbp *dbp,const t_atm *atm,double vel, double rpm,double residuo); //WARNING Dovrebbe ricevere la manetta e restituire copia e spinta disponibile


#endif // PROPELLER_H_INCLUDED
