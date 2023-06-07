#ifndef COMANDO_H_INCLUDED
#define COMANDO_H_INCLUDED

#include "aeromobile.h"

#define I_DA        0
#define I_DE        1
#define I_DR        2
#define I_RPM       3
#define SIZE_COM    4


typedef enum {c_da=0,c_de,c_dr,c_rpm}t_componente_comando; //serve per determinare in modo controllato il comando che viene impostato
typedef enum {base=0,impulso,gradino,rampa,costante,sinusoidale,gradino_scontrato,rampa_scontrata,trapezio,trapezio_scontrato}t_tipo_comando; //elenco di comandi disponibili

//TODO passare ad una versione con puntatori di funzioni e gestione ad eventi

typedef struct{
    t_tipo_comando tipo;
    double t_impulso;
    double dt;
    double ampiezza_iniziale;
    double ampiezza_impulso;
}t_com_impulso; //Struttura dati con i parametri minimi dell'impulso.

typedef struct{
    t_tipo_comando tipo;
    double t_inizio;
    double t_fine;
    double ampiezza_iniziale;
    double ampiezza_regime;
}t_com_gradino;//Struttura dati con i parametri minimi del gradino.

typedef struct{
    t_tipo_comando tipo;
    double t_inizio;
    double t_fine;
    double ampiezza_iniziale;
    double ampiezza_finale;
}t_com_rampa;//Struttura dati con i parametri minimi della rampa.

typedef struct{
    t_tipo_comando tipo;
    double ampiezza;
}t_com_costante;//Struttura dati con i parametri minimi del comando costante.

typedef struct{
    t_tipo_comando tipo;
    double t_inizio;
    double periodo;
    double valore_medio;
    double ampiezza;
    double fase;
}t_com_sinusoidale;//Struttura dati con i parametri minimi del comando sinusoidale.

typedef struct{
    t_tipo_comando tipo;
    double t_inizio;
    double t_1;
    double t_2;
    double t_finale;
    double ampiezza_iniziale;
    double ampiezza_1;
    double ampiezza_2;
    double ampiezza_finale;
}t_com_gradino_scontrato;//Struttura dati con i parametri minimi del comando gradino scontrato.

typedef struct{
    t_tipo_comando tipo;
    double t_inizio;
    double t_1;
    double t_2;
    double t_3;
    double t_4;
    double t_finale;
    double ampiezza_iniziale;
    double ampiezza_1;
    double ampiezza_2;
    double ampiezza_finale;
    double ampiezza_intermedia;
}t_com_rampa_scontrata;//Struttura dati con i parametri minimi del comando gradino scontrato.Si pu� usare per il trapezio.

typedef struct{
    t_tipo_comando tipo;
    double t_inizio;
    double Dt_rampa;
    double Dt_regime;
    double Dt_ritardo;
    double ampiezza_iniziale;
    double ampiezza_regime;
}t_com_trapezio_scontrato;

typedef struct{
    t_tipo_comando tipo;
}t_com_base;//Base di appoggio comune per distinguuere, usando la union, qual'� il comando salvato all'interno eil successivo accesso.


typedef union{
    t_com_impulso impulso;
    t_com_gradino gradino;
    t_com_rampa rampa;
    t_com_costante costante;
    t_com_sinusoidale sin;
    t_com_gradino_scontrato grad_scontr;
    t_com_rampa_scontrata rampa_scontr;
    t_com_trapezio_scontrato trap_scontr;
    t_com_base base;
}u_funz_comando;//Union dei vari tipi di comando:
//In questo modo � possibile incapsulare qualsiasi struttura dei comandi in una struttura comune da passare alla funzione.
//Sara poi la funzione a verificare il tipo di comanda e accedere alla memoria nel modo corretto.

//Nell'utilizzare quest libreria � quindi necessario:
//     -inizializzare il vettore di impostazione attraverso la funzione set_comando_trim(...)
//     -instaziare una union di comando
//     -settare il tipo di comando e i valori
//              ad esempio
//                      u_comando u_com;
//                      u_com.gradino.tipo=gradino;
//                      u_com.gradino.t_inizio=30;
//                      u_com.gradino.t_fine=500;
//                      u_com.gradino.ampiezza_iniziale=0*DEG_TO_RAD;
//                      u_com.gradino.ampiezza_regime=2*DEG_TO_RAD;
//     -settare il segnale del comando alla superficie desiderata o altro comando come la manetta
//              ad esempio
//                      set_comando(u_com,c_da);
//
//     -chiamare quando serve la funzione comando(t) che restituir� il vettore di comando in funzione delle impostazione settate
//

typedef double vett_com[SIZE_COM];

typedef union{
    t_comando com;
    vett_com v_com;
}u_com;

extern u_funz_comando set_comandi[SIZE_COM];

t_comando comando(double t);
void set_comando_trim(t_comando com);
void set_comando(u_funz_comando set_comando, t_componente_comando c_com);

#endif // COMANDO_H_INCLUDED
