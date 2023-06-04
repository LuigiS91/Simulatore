#ifndef TRIM_H_INCLUDED
#define TRIM_H_INCLUDED


#include "aeromobile.h" //serve per avere la definizione del vettore di stato
#include "lettura_dati.h"//serve per utilizzare le strutture dati di descrizione dei limiti del comando e di velocit�
#include "propeller.h"//serve per poter determinare la spinta del gruppo propulsivo


#define RESIDUO_ALPHA 0.001  //Residuo nel calcol odell'equiliprio lungo z [N]
#define RESIDUO_RPM 0.001 //Residuo dell'equilibrio lungo x [N]
// #define DEG_TO_RAD M_PI/180.0
// #define RAD_TO_DEG 180.0/M_PI
#define RESIDUO_TRIM_PROP 1e-5

extern int errore_trim;

typedef struct{
    t_stato stato;
    t_comando comando;
}t_res_trim; //Struttura restituita dalla funzione trim

t_res_trim trim(t_descrizione_aereo desc_aer,
                t_dba dba,t_engine engine,
                t_descrizione_propeller desc_prop,
                t_dbp dbp,t_atm_sl atm_sl,double h, double vel,double gamma);
//La funzione restituisce i valori di stato e dei comandi nella condizione di trim
//Il valore di gamma deve essere espresso in radianti
//I valori di alpha e di rpm vengono determinati attraverso il metodo delle secanti

#endif // TRIM_H_INCLUDED
