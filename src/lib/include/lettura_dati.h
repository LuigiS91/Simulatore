#ifndef LETTURA_DATI_H_INCLUDED
#define LETTURA_DATI_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEG_TO_RAD 3.14/180.0
#define RAD_TO_DEG 180.0/3.14

// indici e dimensione vettore coefficienti steady state
#define NBUF 100
#define I_ALPHA 0
#define I_SSC_CX 1
#define I_SSC_CY 2
#define I_SSC_CZ 3
#define I_SSC_CL 4
#define I_SSC_CM 5
#define I_SSC_CN 6
#define STEADY_STATE_C_SIZE 7

// indici e dimensione vettore coefficenti derivati forze lungo x
#define I_X_DER_CXA   1
#define I_X_DER_CXAP  2
#define I_X_DER_CXU   3
#define I_X_DER_CXQ	  4
#define I_X_DER_CXB	  5
#define I_X_DER_CXP	  6
#define I_X_DER_CXR   7
#define X_DER_SIZE    8

// indici e dimensione vettore coefficenti derivati forze lungo y
#define I_Y_DER_CYB   1
#define I_Y_DER_CYBP  2
#define I_Y_DER_CYP   3
#define I_Y_DER_CYR	4
#define I_Y_DER_CYA	5
#define I_Y_DER_CYQ	6
#define Y_DER_SIZE 7

// indici e dimensione vettore coefficenti derivati forze lungo z
#define I_Z_DER_CZA   1
#define I_Z_DER_CZAP  2
#define I_Z_DER_CZU   3
#define I_Z_DER_CZQ	4
#define I_Z_DER_CZB	5
#define I_Z_DER_CZP	6
#define I_Z_DER_CZR   7
#define Z_DER_SIZE 8

// indici e dimensione vettore coefficenti derivati momenti lungo x
#define I_L_DER_ClB   1
#define I_L_DER_ClBP  2
#define I_L_DER_ClP   3
#define I_L_DER_ClR	4
#define I_L_DER_ClA	5
#define I_L_DER_ClQ	6
#define L_DER_SIZE 7

// indici e dimensione vettore coefficenti derivati momenti lungo y
#define I_M_DER_CmA   1
#define I_M_DER_CmAP  2
#define I_M_DER_CmU   3
#define I_M_DER_CmQ	4
#define I_M_DER_CmB	5
#define I_M_DER_CmP	6
#define I_M_DER_CmR	7
#define M_DER_SIZE 8

// indici e dimensione vettore coefficenti derivati momenti lungo z
#define I_N_DER_CnB   1
#define I_N_DER_CnBP  2
#define I_N_DER_CnP   3
#define I_N_DER_CnR	4
#define I_N_DER_CnA	5
#define I_N_DER_CnQ	6
#define N_DER_SIZE 7

// indici e dimensione vettore derivate delle forze di controllo
#define I_COM_FOR_DER_CXde   1
#define I_COM_FOR_DER_CXdle  2
#define I_COM_FOR_DER_CZde   3
#define I_COM_FOR_DER_CZdle	 4
#define I_COM_FOR_DER_CYda	 5
#define I_COM_FOR_DER_CYdr	 6
#define COM_FOR_DER_SIZE 7

//  indici e dimensione vettore derivate dei momenti di controllo
#define I_COM_MOM_DER_Clda   1
#define I_COM_MOM_DER_Cldr   2
#define I_COM_MOM_DER_Cmde   3
#define I_COM_MOM_DER_Cmdle	 4
#define I_COM_MOM_DER_Cnda	 5
#define I_COM_MOM_DER_Cndr	 6
#define COM_MOM_DER_SIZE 7

//  indici e dimensione vettore derivate di rotazione
#define I_ROT_DER_CXom   1
#define I_ROT_DER_CYom   2
#define I_ROT_DER_CZom   3
#define I_ROT_DER_Clom	 4
#define I_ROT_DER_Cmom	 5
#define I_ROT_DER_Cnom	 6
#define ROT_DER_SIZE 7

//  indici e dimensione vettore stazioni propeller
#define I_STAZ_PROP_CSI           0
#define I_STAZ_PROP_RD            1
#define I_STAZ_PROP_CH_AD         2
#define I_STAZ_PROP_BA	        3
#define STAZ_PROP_SIZE  4



typedef struct  //la struct mantiene in memoria tutti i dati descrittivi del velivolo
{
    double massa; //64bit
    double apertura_alare;
    double superficie_alare;
    double corda;
    double mach_drag_rise;
    double dist_T_x_z;
    double dist_T_x_y;
    double dist_T_x_x;
    double ang_T_x_xy;
    double ang_T_x_xz;
    int num_inc; //32bit
    int rotary_der;
    double center_cg_ref_loc;
    double Jx;
    double Jy;
    double Jz;
    double Jxz;
    int opt_pos_cg;
    double new_pos_cg;
    double pos_pil_x_cg;
    double pos_pil_y_cg;
    double pos_pil_z_cg;
} t_descrizione_aereo;

typedef struct{
    double max_elevator;
    double min_elevator;
    double airelon;
    double rudder;
    double max_flap;
    double min_flap;
}t_limiti_comando;

typedef struct{
    int opt_massa;
    float mf_mtow;
}t_massa_combustibile;

//Le aree di memoria seguenti vanno inizializzate dinamicamente prima di chiamare le funzioni di lettura
// e liberate prima della chusura del programma

typedef double* t_steady_state_coeff[STEADY_STATE_C_SIZE];//Vettore di vettori dinamici [STEADY_STATE_C_SIZE][Num_inc]
typedef double* t_x_der[X_DER_SIZE];//Vettore di vettori dinamici [X/Y/Z/L/M/N][Num_inc] con le derivate aerodinamiche
typedef double* t_y_der[Y_DER_SIZE];
typedef double* t_z_der[Z_DER_SIZE];
typedef double* t_l_der[L_DER_SIZE];
typedef double* t_m_der[M_DER_SIZE];
typedef double* t_n_der[N_DER_SIZE];
typedef double* t_com_for_der[COM_FOR_DER_SIZE];//Vettore di vettori dinamici dei comandi
typedef double* t_com_mom_der[COM_MOM_DER_SIZE];
typedef double* t_rot_der[ROT_DER_SIZE];//Vettore di vettori dinamici delle derivate rotary

//Struttura complet del database
typedef struct{
    t_steady_state_coeff ssc;
    t_x_der x_der;
    t_y_der y_der;
    t_z_der z_der;
    t_l_der l_der;
    t_m_der m_der;
    t_n_der n_der;
    t_com_for_der com_for_der;
    t_com_mom_der com_mom_der;
    t_rot_der rot_der;
    int num_inc;
}t_dba;

typedef struct{
    float p_max;
    float esp;
    int max_rpm;
    int min_rpm;
    float rapp_rid;
    float rend_mecc_trad;
    double sfc;
}t_engine;

typedef struct{
    float diametro;
    float diametro_ogiva;
    int n_pale;
    float inerzia;
    int num_stazioni;
    double Cla;
    double Cl0;
    double a0;
    double Cda2;
    double Cda;
    double Cd0;
}t_descrizione_propeller;

//Vettore dinamico stazioni propeller
typedef double* t_stazioni_propeller[STAZ_PROP_SIZE];

//Database stazioni propeller
typedef struct{
    t_stazioni_propeller staz;
    int num_staz;
    int staz_fuorihub;
}t_dbp;


/* il buffer del file viene mantenuto esteernamente in memoria in modo da dover aprire e chiudere il file una sola volta
    Chiamare in ordine :
                      -apri_file_lettura()
                      -lettura_dati_intestrazione(...)
                      -alloca_dba(...)
                      -lettura_dati_aerodinamici(...)
                      -chiudi_file_lettura()
                      ... CODICE INTERNO
                      -libera_dba(...)
                      CHIUDI PROGRAMMA
    La stessa logica � applicata per il database del propeller
    Nel caso si utilizzi ila funzione interpola � necessario inizializzare anche la dimensione dei vettore con set_dimensioni_vettori_interpola(...)
*/
extern FILE* in_lettura;
extern int errore_lettura;

//alloca la memoria in base alla lunghezza del database (Num_inc)
int alloca_dba(int num_inc,t_dba* dba);
//libera la memoria in base alla lunghezza del database (Num_inc)
void libera_dba(t_dba* dba);
//verifica la presenza del file e imposta il buffer
int apri_file_lettura(const char* nome_file);
//Chiude il file al termine della lettura
int chiudi_file_lettura();
//Legge la parte di intestazione del file database aerodinamico (i valori angolari sono salvati in radianti)
int lettura_dati_intestazione_aerodinamica(t_descrizione_aereo* descrizione_aereo,t_limiti_comando* limiti_comando,t_massa_combustibile* massa_combustibile);
//Legge le tabelle aerodinamiche seguenti la intestazione (i valori degli angoli sono salvati in radianti)
int lettura_dati_aerodinamici(t_dba* dba);
//Legge il file engine
int lettura_engine(t_engine* engine);
//Legge il file intestazione propeller
int lettura_intestazione_propeller(t_descrizione_propeller* descrizione_propeller);
//Legge il file stazioni propeller
int lettura_stazioni_propeller(t_dbp* dbp,double diam_hub);
//alloca la memoria in base alla lunghezza del database (Num_sazio)
int alloca_dbp(int num_staz,t_dbp* dbp);
//alloca la memoria in base alla lunghezza del database (Num_staz)
void libera_dbp(t_dbp* dbp);

#endif // LETTURA_DATI_H_INCLUDED
