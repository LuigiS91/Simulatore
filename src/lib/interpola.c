#include "interpola.h"

int N=0;
int errore_interpola=0;

double interpola(double *v_alpha,double *v_coeff,double alpha)
{
    errore_interpola=0;
    int i=N/2,a=0,b=N,intervallo_trovato=0;
    double res=0, m;

    if (alpha>v_alpha[N-1]||alpha<v_alpha[0]){
        //printf("ERRORE di Interpola(): Il valore da interpolare si trova al di fuori del database\n");
        errore_interpola=1;
    }
    //Cerco l'intervallo dove è contenuto il risultato
    while ((!intervallo_trovato)&&!errore_interpola){
        if ((v_alpha[i]<=alpha)&&(alpha<=v_alpha[i+1]))
        {
            m=(v_coeff[i+1]-v_coeff[i])/(v_alpha[i+1]-v_alpha[i]);
            res=v_coeff[i]+m*(alpha-v_alpha[i]);
            intervallo_trovato=1;
        }
        else if(alpha<v_alpha[i]){
            b=i;
            i=(a+i)/2;
        }
        else{
            a=i;
            i=(i+b)/2;
            }
    }
    return res;
}

void set_dimensione_vettori_interpola(int N_dba){
    N=N_dba;
    return;
}
