#include "include/propeller.h"

int errore_propeller=0;

t_res_prop propeller(const t_descrizione_propeller *desc_prop,const t_dbp *dbp,const t_atm *atm, double vel, double rpm,double residuo){
    t_res_prop res_prop;
    double omega=rpm*RPM_TO_ANGVEL;
    int i, finito, somma;
    double a, b, theta, V0, V2, phi, alpha, cl, cd, Vlocal, CT, DtDr, CQ, DqDr, tem1, tem2, anew, bnew;
    res_prop.thrust=0;
    res_prop.torque=0;
    errore_propeller=0;

    for(i=dbp->staz_fuorihub;i<dbp->num_staz;i++){
        theta=dbp->staz[I_STAZ_PROP_BA][i]*DEG_TO_RAD;
        a=0.1;
        b=0.01;
        somma=1;
        finito=0;
        while(!finito){
            V0=vel*(1+a); // componente del flusso all'incirca uguale alla velocit� di avanzamento del velivolo (Vinf), aumentata tramite l'axial inflow factor
            V2=omega*dbp->staz[I_STAZ_PROP_CSI][i]*(1-b); // componente del flusso all'incirca uguale alla velocit� angolare della sezione della pala (omega*rad), ridotta tramite l'angular inflow factor
            phi=atan2(V0,V2); // angolo tra le due componenti del flusso V0 e V2
            alpha=theta-phi; // angolo di attacco raltivo alla j-esima sezione della pala
            cl=desc_prop->Cl0+desc_prop->Cla*alpha; //CL coefficiente di portanza
            cd=desc_prop->Cd0+desc_prop->Cda*alpha+desc_prop->Cda2*pow(alpha,2.0); // CD coefficiente di resistenza CD = CD0+CD1*CL+CD2*CL^2 (NB nel nostro case, CD = CD0+CD_alpha*alpha+CD_alpha2*alpha^2 -> slide lezione 2)
            Vlocal=sqrt(pow(V0,2.0)+pow(V2,2.0)); // velocit� locale del flusso
            CT = cl*cos(phi)-cd*sin(phi); // CT coefficiente di spinta adimensionale
            DtDr=0.5*atm->dens*pow(Vlocal,2.0)*desc_prop->n_pale*dbp->staz[I_STAZ_PROP_CH_AD][i]*CT; // contributo di spinta della j-esima sezione
            CQ = cd*cos(phi)+cl*sin(phi); // CQ coefficiente di coppia adimensionale
            DqDr=0.5*atm->dens*pow(Vlocal,2.0)*desc_prop->n_pale*dbp->staz[I_STAZ_PROP_CH_AD][i]*dbp->staz[I_STAZ_PROP_CSI][i]*CQ; // contributo di coppia della j-esima sezione
            tem1=DtDr/(4.0*M_PI*dbp->staz[I_STAZ_PROP_CSI][i]*atm->dens*pow(vel,2.0)*(1+a)); // fattore correttivo del coefficiente "a"
            tem2=DqDr/(4.0*M_PI*pow(dbp->staz[I_STAZ_PROP_CSI][i],3.0)*atm->dens*vel*(1+a)*omega); // fattore correttivo del coefficiente "b"
            anew=0.5*(a+tem1); // nuovo valore coefficiente "a"
            bnew=0.5*(b+tem2); // nuovo valore coefficiente "b"
            if((fabs(anew-a)<residuo)&&(fabs(bnew-b)<residuo))
                finito=1;
            a=anew;
            b=bnew;
            somma++;
            if(somma>500){
                finito=1;
                errore_propeller=1;
            }
        }
        res_prop.thrust+=DtDr*(dbp->staz[I_STAZ_PROP_RD][i]-dbp->staz[I_STAZ_PROP_RD][i-1]);
        res_prop.torque+=DqDr*(dbp->staz[I_STAZ_PROP_RD][i]-dbp->staz[I_STAZ_PROP_RD][i-1]);
    }
    return res_prop;
}
