#include "include/aeromobile.h"


t_stato aeromobile(const t_stato *stato,const t_comando *com,const t_dba *dba,const t_dbp *dbp,const t_descrizione_aereo *desc_aer,const t_descrizione_propeller *desc_prop,const t_atm_sl *atm_sl){
    t_stato res_stato;
    t_atm atm;
    double press_din,X,Y,T,Z,L,M,N,V,alpha,beta;

    double Cxss,Cxa,Cxde;
    double Cyb,Cyp,Cyr,Cydr;
    double Czss,Cza,Czde,Czq;
    double Clb,Cldr,Clda,Clp,Clr;
    double Cmss,Cma,Cmde,Cmq;
    double Cnb,Cnda,Cndr,Cnp,Cnr;

    atm=atmosfera(stato->h,*atm_sl);

    V=sqrt(pow(stato->u,2.0)+pow(stato->v,2.0)+pow(stato->w,2.0));
    press_din=0.5*atm.dens*desc_aer->superficie_alare*pow(V,2.0);
    alpha=atan(stato->w/stato->u);
    beta=atan(stato->v/V);

    Cxss=interpola(dba->ssc[I_ALPHA],dba->ssc[I_SSC_CX],alpha);
    Cxa=interpola(dba->x_der[I_ALPHA],dba->x_der[I_X_DER_CXA],alpha);
    Cxde=interpola(dba->com_for_der[I_ALPHA],dba->com_for_der[I_COM_FOR_DER_CXde],alpha);

    //aggiungere staady state
    Cyb=interpola(dba->y_der[I_ALPHA],dba->y_der[I_Y_DER_CYB],alpha);
    Cydr=interpola(dba->com_for_der[I_ALPHA],dba->com_for_der[I_COM_FOR_DER_CYdr],alpha);
    Cyp=interpola(dba->y_der[I_ALPHA],dba->y_der[I_Y_DER_CYP],alpha);
    Cyr=interpola(dba->y_der[I_ALPHA],dba->y_der[I_Y_DER_CYR],alpha);

    Czss=interpola(dba->ssc[I_ALPHA],dba->ssc[I_SSC_CZ],alpha);
    Cza=interpola(dba->z_der[I_ALPHA],dba->z_der[I_Z_DER_CZA],alpha);
    Czde=interpola(dba->com_for_der[I_ALPHA],dba->com_for_der[I_COM_FOR_DER_CZde],alpha);
    Czq=interpola(dba->z_der[I_ALPHA],dba->z_der[I_Z_DER_CZQ],alpha);

    Clb=interpola(dba->l_der[I_ALPHA],dba->l_der[I_L_DER_ClB],alpha);
    Clda=interpola(dba->com_mom_der[I_ALPHA],dba->com_mom_der[I_COM_MOM_DER_Clda],alpha);
    Cldr=interpola(dba->com_mom_der[I_ALPHA],dba->com_mom_der[I_COM_MOM_DER_Cldr],alpha);
    Clp=interpola(dba->l_der[I_ALPHA],dba->l_der[I_L_DER_ClP],alpha);
    Clr=interpola(dba->l_der[I_ALPHA],dba->l_der[I_L_DER_ClR],alpha);

    Cmss=interpola(dba->ssc[I_ALPHA],dba->ssc[I_SSC_CM],alpha);
    Cma=interpola(dba->m_der[I_ALPHA],dba->m_der[I_M_DER_CmA],alpha);
    Cmde=interpola(dba->com_mom_der[I_ALPHA],dba->com_mom_der[I_COM_MOM_DER_Cmde],alpha);
    Cmq=interpola(dba->m_der[I_ALPHA],dba->m_der[I_M_DER_CmQ],alpha);

    Cnb=interpola(dba->n_der[I_ALPHA],dba->n_der[I_N_DER_CnB],alpha);
    Cnda=interpola(dba->com_mom_der[I_ALPHA],dba->com_mom_der[I_COM_MOM_DER_Cnda],alpha);
    Cndr=interpola(dba->com_mom_der[I_ALPHA],dba->com_mom_der[I_COM_MOM_DER_Cndr],alpha);
    Cnp=interpola(dba->n_der[I_ALPHA],dba->n_der[I_N_DER_CnP],alpha);
    Cnr=interpola(dba->n_der[I_ALPHA],dba->n_der[I_N_DER_CnR],alpha);


    /*Calcolo delle forze*/
    X=press_din*(Cxss+Cxa*alpha+Cxde*com->de);
    T=propeller(desc_prop,dbp,&atm,V,com->rpm,RESIDUO_AEROMOBILE_PROP).thrust;
    Y=press_din*(Cyb*beta+Cydr*com->dr+Cyp*stato->p*desc_aer->apertura_alare/(2.0*V)+Cyr*stato->r*desc_aer->apertura_alare/(2.0*V));
    Z=press_din*(Czss+Cza*alpha+Czde*com->de+Czq*stato->q*desc_aer->corda/(2.0*V));
    L=press_din*desc_aer->apertura_alare*(Clb*beta+Clda*com->da+Cldr*com->dr+Clp*stato->p*desc_aer->apertura_alare/(2.0*V)+Clr*stato->r*desc_aer->apertura_alare/(2.0*V));
    M=press_din*desc_aer->corda*(Cmss+Cma*alpha+Cmde*com->de+Cmq*stato->q*desc_aer->corda/(2.0*V));
    N=press_din*desc_aer->apertura_alare*(Cnb*beta+Cnda*com->da+Cndr*com->dr+Cnp*stato->p*desc_aer->apertura_alare/(2.0*V)+Cnr*stato->r*desc_aer->apertura_alare/(2.0*V));


    /*Equazioni*/
    // A sinistra � calcolato il valore di derivata nella struttura di stato
    res_stato.u=(stato->r*stato->v-stato->q*stato->w) - const_g*sin(stato->theta) + X/desc_aer->massa + T/desc_aer->massa;
    res_stato.v=(stato->p*stato->w-stato->r*stato->u) + const_g*sin(stato->phi)*cos(stato->theta) + Y/desc_aer->massa;
    res_stato.w=(stato->q*stato->u-stato->p*stato->v) + const_g*cos(stato->phi)*cos(stato->theta) + Z/desc_aer->massa;
    res_stato.p=(-(desc_aer->Jz-desc_aer->Jy)*stato->q*stato->r/desc_aer->Jx + (stato->p*stato->q)*desc_aer->Jxz/desc_aer->Jx + L/desc_aer->Jx
                 +desc_aer->Jxz/desc_aer->Jx*
                 (-(desc_aer->Jy-desc_aer->Jx)*stato->p*stato->q/desc_aer->Jz - (stato->q*stato->r)*desc_aer->Jxz/desc_aer->Jz + N/desc_aer->Jz)
                 )/
                 (1-pow(desc_aer->Jxz,2.0)/(desc_aer->Jx*desc_aer->Jz));// r_punto sostituito
    res_stato.q=-(desc_aer->Jx-desc_aer->Jz)*stato->p*stato->r/desc_aer->Jy - (stato->p*stato->p-stato->r*stato->r)*desc_aer->Jxz/desc_aer->Jy + M/desc_aer->Jy;
    res_stato.r=-(desc_aer->Jy-desc_aer->Jx)*stato->p*stato->q/desc_aer->Jz - (stato->q*stato->r-res_stato.p)*desc_aer->Jxz/desc_aer->Jz + N/desc_aer->Jz;//sostituito il valore gia calcolato di p_punto
    res_stato.phi=stato->p + stato->q*sin(stato->phi)*tan(stato->theta) + stato->r*cos(stato->phi)*tan(stato->theta);
    res_stato.theta=stato->q*cos(stato->phi) - stato->r*sin(stato->phi);
    res_stato.psi=stato->q*sin(stato->phi)/cos(stato->theta) + stato->r*cos(stato->phi)/cos(stato->theta);
    res_stato.h=+stato->u*sin(stato->theta) - stato->v*cos(stato->theta)*sin(stato->phi) - stato->w*cos(stato->theta)*cos(stato->phi);

return res_stato;
}
