#include "include/integratore.h"


t_stato integra(const t_stato *stato0,const t_comando *com,const t_dba *dba,const t_dbp *dbp,const t_descrizione_aereo *desc_aer,const t_descrizione_propeller *desc_prop,const t_atm_sl *atm_sl,double dt){
    t_stato res_stato;
    res_stato=aeromobile(stato0,com,dba,dbp,desc_aer,desc_prop,atm_sl);

    res_stato.u=res_stato.u*dt+stato0->u;
    res_stato.v=res_stato.v*dt+stato0->v;
    res_stato.w=res_stato.w*dt+stato0->w;
    res_stato.p=res_stato.p*dt+stato0->p;
    res_stato.q=res_stato.q*dt+stato0->q;
    res_stato.r=res_stato.r*dt+stato0->r;
    res_stato.psi=res_stato.psi*dt+stato0->psi;
    res_stato.phi=res_stato.phi*dt+stato0->phi;
    res_stato.theta=res_stato.theta*dt+stato0->theta;
    res_stato.h=res_stato.h*dt+stato0->h;
    return res_stato;
}
