#include "include/comando.h"

u_funz_comando set_comandi[SIZE_COM];
void * funct_ptr[SIZE_COM];

t_comando comando(double t){
    u_com com={0};
    int i;
    for(i=0;i<4;i++){
        switch(set_comandi[i].base.tipo){
        case base:
            //Settare errore , si avvisa che bisogna settare almeno il comando di trim
            break;
        case gradino:
            com.v_com[i]=set_comandi[i].gradino.ampiezza_iniziale;
            if(t>=set_comandi[i].gradino.t_inizio&&t<=set_comandi[i].gradino.t_fine)
                com.v_com[i]=set_comandi[i].gradino.ampiezza_regime;
            break;
        case rampa:
            if(t<=set_comandi[i].rampa.t_inizio)
                com.v_com[i]=set_comandi[i].rampa.ampiezza_iniziale;
            else if(t>=set_comandi[i].rampa.t_fine)
                com.v_com[i]=set_comandi[i].rampa.ampiezza_finale;
            else
                com.v_com[i]=set_comandi[i].rampa.ampiezza_iniziale+
                (set_comandi[i].rampa.ampiezza_finale-set_comandi[i].rampa.ampiezza_iniziale)
                /
                (set_comandi[i].rampa.t_fine-set_comandi[i].rampa.t_inizio)*(t-set_comandi[i].rampa.t_inizio);
            break;
        case impulso:
            com.v_com[i]=set_comandi[i].impulso.ampiezza_iniziale;
            if(t>=set_comandi[i].impulso.t_impulso&&t<(set_comandi[i].impulso.t_impulso+set_comandi[i].impulso.dt))
                com.v_com[i]=set_comandi[i].impulso.ampiezza_impulso;
            break;
        case costante:
            com.v_com[i]=set_comandi[i].costante.ampiezza;
            break;
        case sinusoidale:
            com.v_com[i]=set_comandi[i].sin.valore_medio;
            if(t>=set_comandi[i].sin.t_inizio){
                com.v_com[i]=set_comandi[i].sin.ampiezza*sin(2*3.14/set_comandi[i].sin.periodo*t+set_comandi[i].sin.fase)+set_comandi[i].sin.valore_medio;
            }
            break;
        case gradino_scontrato:
            if(t<set_comandi[i].grad_scontr.t_inizio)
                com.v_com[i]=set_comandi[i].grad_scontr.ampiezza_iniziale;
            else if(t<set_comandi[i].grad_scontr.t_1)
                com.v_com[i]=set_comandi[i].grad_scontr.ampiezza_1;
            else if(t<set_comandi[i].grad_scontr.t_2)
                com.v_com[i]=set_comandi[i].grad_scontr.ampiezza_iniziale;
            else if(t<set_comandi[i].grad_scontr.t_finale)
                com.v_com[i]=set_comandi[i].grad_scontr.ampiezza_2;
            else
                com.v_com[i]=set_comandi[i].grad_scontr.ampiezza_finale;
            break;
        case rampa_scontrata:
        case trapezio://comando equivalente con impostazione settate in modo corretto
            com.v_com[i]=set_comandi[i].rampa_scontr.ampiezza_finale;
            if(t<set_comandi[i].rampa_scontr.t_inizio)
                com.v_com[i]=set_comandi[i].rampa_scontr.ampiezza_iniziale;
            else if(t<set_comandi[i].rampa_scontr.t_1)
                com.v_com[i]=set_comandi[i].rampa_scontr.ampiezza_iniziale+
                (set_comandi[i].rampa_scontr.ampiezza_1-set_comandi[i].rampa_scontr.ampiezza_iniziale)
                /
                (set_comandi[i].rampa_scontr.t_1-set_comandi[i].rampa_scontr.t_inizio)*(t-set_comandi[i].rampa_scontr.t_inizio);
            else if(t<set_comandi[i].rampa_scontr.t_2)
                com.v_com[i]=set_comandi[i].rampa_scontr.ampiezza_1+
                (set_comandi[i].rampa_scontr.ampiezza_intermedia-set_comandi[i].rampa_scontr.ampiezza_1)
                /
                (set_comandi[i].rampa_scontr.t_2-set_comandi[i].rampa_scontr.t_1)*(t-set_comandi[i].rampa_scontr.t_1);
            else if(t<set_comandi[i].rampa_scontr.t_3)
                com.v_com[i]=set_comandi[i].rampa_scontr.ampiezza_intermedia;
            else if(t<set_comandi[i].rampa_scontr.t_4)
                com.v_com[i]=set_comandi[i].rampa_scontr.ampiezza_intermedia+
                (set_comandi[i].rampa_scontr.ampiezza_2-set_comandi[i].rampa_scontr.ampiezza_intermedia)
                /
                (set_comandi[i].rampa_scontr.t_4-set_comandi[i].rampa_scontr.t_3)*(t-set_comandi[i].rampa_scontr.t_3);
            else if(t<set_comandi[i].rampa_scontr.t_finale)
                com.v_com[i]=set_comandi[i].rampa_scontr.ampiezza_2+
                (set_comandi[i].rampa_scontr.ampiezza_finale-set_comandi[i].rampa_scontr.ampiezza_2)
                /
                (set_comandi[i].rampa_scontr.t_finale-set_comandi[i].rampa_scontr.t_4)*(t-set_comandi[i].rampa_scontr.t_4);
            break;
        case trapezio_scontrato:
            com.v_com[i]=set_comandi[i].trap_scontr.ampiezza_iniziale;
            if(t<set_comandi[i].trap_scontr.t_inizio)
                com.v_com[i]=set_comandi[i].trap_scontr.ampiezza_iniziale;
            else if(t<set_comandi[i].trap_scontr.t_inizio+set_comandi[i].trap_scontr.Dt_rampa){
                com.v_com[i]=set_comandi[i].trap_scontr.ampiezza_iniziale+
                (set_comandi[i].trap_scontr.ampiezza_regime-set_comandi[i].trap_scontr.ampiezza_iniziale)
                /
                (set_comandi[i].trap_scontr.Dt_rampa)*(t-set_comandi[i].trap_scontr.t_inizio);
                }
            else if(t<set_comandi[i].trap_scontr.t_inizio+set_comandi[i].trap_scontr.Dt_rampa+set_comandi[i].trap_scontr.Dt_regime)
                com.v_com[i]=set_comandi[i].trap_scontr.ampiezza_regime;
            else if(t<set_comandi[i].trap_scontr.t_inizio+2.0*set_comandi[i].trap_scontr.Dt_rampa+set_comandi[i].trap_scontr.Dt_regime){
                com.v_com[i]=set_comandi[i].trap_scontr.ampiezza_regime-
                (set_comandi[i].trap_scontr.ampiezza_regime-set_comandi[i].trap_scontr.ampiezza_iniziale)
                /
                (set_comandi[i].trap_scontr.Dt_rampa)*(t-set_comandi[i].trap_scontr.t_inizio-set_comandi[i].trap_scontr.Dt_rampa-set_comandi[i].trap_scontr.Dt_regime);
                }
            else if(t<set_comandi[i].trap_scontr.t_inizio+2.0*set_comandi[i].trap_scontr.Dt_rampa+set_comandi[i].trap_scontr.Dt_regime+set_comandi[i].trap_scontr.Dt_ritardo)
                com.v_com[i]=set_comandi[i].trap_scontr.ampiezza_iniziale;
            else if(t<set_comandi[i].trap_scontr.t_inizio+3.0*set_comandi[i].trap_scontr.Dt_rampa+set_comandi[i].trap_scontr.Dt_regime+set_comandi[i].trap_scontr.Dt_ritardo){
                com.v_com[i]=set_comandi[i].trap_scontr.ampiezza_iniziale-
                (set_comandi[i].trap_scontr.ampiezza_regime-set_comandi[i].trap_scontr.ampiezza_iniziale)
                /
                (set_comandi[i].trap_scontr.Dt_rampa)*(t-set_comandi[i].trap_scontr.t_inizio-2.0*set_comandi[i].trap_scontr.Dt_rampa-set_comandi[i].trap_scontr.Dt_regime-set_comandi[i].trap_scontr.Dt_ritardo);
                }
            else if(t<set_comandi[i].trap_scontr.t_inizio+3.0*set_comandi[i].trap_scontr.Dt_rampa+2.0*set_comandi[i].trap_scontr.Dt_regime+set_comandi[i].trap_scontr.Dt_ritardo)
                com.v_com[i]=-set_comandi[i].trap_scontr.ampiezza_regime;
            else if(t<set_comandi[i].trap_scontr.t_inizio+4.0*set_comandi[i].trap_scontr.Dt_rampa+2.0*set_comandi[i].trap_scontr.Dt_regime+set_comandi[i].trap_scontr.Dt_ritardo){
                com.v_com[i]=-set_comandi[i].trap_scontr.ampiezza_regime+
                (set_comandi[i].trap_scontr.ampiezza_regime-set_comandi[i].trap_scontr.ampiezza_iniziale)
                /
                (set_comandi[i].trap_scontr.Dt_rampa)*(t-set_comandi[i].trap_scontr.t_inizio-3.0*set_comandi[i].trap_scontr.Dt_rampa-2.0*set_comandi[i].trap_scontr.Dt_regime-set_comandi[i].trap_scontr.Dt_ritardo);
                }
            break;
        }
    }
    return com.com;
};

void set_comando_trim(t_comando com){
    u_com comando;
    int i;
    comando.com = com;
    for(i=0;i<SIZE_COM;i++){
        set_comandi[i].costante.tipo=costante;
        set_comandi[i].costante.ampiezza=comando.v_com[i];
    }
    return;
}

void set_comando(u_funz_comando set_comando, t_componente_comando c_com){
    set_comandi[c_com]=set_comando;
    return;
};

//TODO incapsulare funzioni dei comandi
//TODO spostare lo switch nel setup e chiamare i puntatori di funzione al posto di cercare il tipo di comando
