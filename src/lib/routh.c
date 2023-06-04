#include "routh.h"


t_res_routh routh(t_res_trim trim, t_descrizione_aereo desc_aer, t_atm_sl atm_sl,t_dba dba){
    t_res_routh res_routh;
    double C_We,C_Le,C_De,C_Lal,C_Dal,C_mq,C_mAP,C_mal,mi,I_y_cap,V,alpha;
    double A,B,C,D,E,DELTA_ROUTH;
    t_atm atm;

    atm=atmosfera(trim.stato.h,atm_sl);
    res_routh.omega_ph=0;
    res_routh.omega_sp=0;
    res_routh.smorz_ph=0;
    res_routh.smorz_sp=0;
    res_routh.periodo_sp=0;
    res_routh.periodo_ph=0;

    /*Calcolo coefficienti aerodinamici e caratteristiche aeromobile*/
    V=sqrt(pow(trim.stato.u,2.0)+pow(trim.stato.w,2.0));
    alpha=atan2(trim.stato.w,trim.stato.u);



    mi=(desc_aer.massa)/(atm.dens*desc_aer.corda*desc_aer.superficie_alare/2.0); //Calcolo coefficiente mi
    I_y_cap=desc_aer.Jy/(atm.dens*desc_aer.superficie_alare*pow(desc_aer.corda/2.0,3.0)); //Calcolo I_y cappelletto

    C_We=(desc_aer.massa*const_g)/(0.5*atm.dens*desc_aer.superficie_alare*pow(V,2.0)); //Calcolo coefficiente Cwe
    C_Le=C_We; //Calcolo coefficiente C_Le (assunzione trascurando theta)
    C_De=C_D0+TRIM_k*pow(C_Le,2.0); //Calcolo coefficiente C_De
    C_Lal=interpola(dba.x_der[I_ALPHA],dba.x_der[I_X_DER_CXA],alpha)*sin(alpha)-interpola(dba.z_der[I_ALPHA],dba.z_der[I_Z_DER_CZA],alpha)*cos(alpha); //Calcolo coefficiente C_Lalpha
    C_Dal=2*TRIM_k*C_Lal*C_Le;//Calcolo coefficiente C_Dalpha
    C_mq=interpola(dba.m_der[I_ALPHA],dba.m_der[I_M_DER_CmQ],alpha), //Calcolo coefficiente C_mq
    C_mAP=interpola(dba.m_der[I_ALPHA],dba.m_der[I_M_DER_CmAP],alpha); //Calcolo coefficiente C_mAP
    C_mal=interpola(dba.m_der[I_ALPHA],dba.m_der[I_M_DER_CmA],alpha);//Calcolo coefficiente C_malpha

    /*Calcolo coefficienti di Routh*/
    A=2*mi*I_y_cap*(2*mi+C_LAP);
    B=2*mi*I_y_cap*(C_Lal+C_De-C_Tu)-I_y_cap*C_Tu*C_LAP-2*mi*C_mq*C_LAP-4*pow(mi,2.0)*(C_mq+C_mAP);
    C=2*mi*(C_mq*(C_Tu-C_Lal-C_De)-2*mi*C_mAP+C_mAP*C_Tu)+I_y_cap*(2*C_We*(C_We-C_Dal)+C_Tu*C_Lal+C_De*C_Lal)+C_mq*C_LAP*C_Tu;
    D=-2*pow(C_We,2.0)*C_mAP+2*mi*C_Tu*C_mal+C_Tu*C_mq*C_Lal-2*C_We*C_mq*(C_Le-C_Dal)+2*C_De*C_mq*C_Tu;
    E=-2*pow(C_We,2.0)*C_mal;

    DELTA_ROUTH=B*C*D-A*pow(D,2.0)-pow(B,2.0)*E;

    /*Controllo stabilità statica e dinamica*/
    if (E>0){
        res_routh.is_stat_stab=1;
    }else{
        res_routh.is_stat_stab=0;
    }

    if (DELTA_ROUTH>0){
        res_routh.is_din_stab=1;
        /*Calcolo caratteristiche corto periodo e fugoide, ha senso solo se velivolo è stabile*/
        res_routh.smorz_ph=1.0/sqrt(2.0)*C_De/C_We;
        res_routh.smorz_sp=(I_y_cap*C_Lal-2*mi*(C_mq+C_mAP))/(2.0*sqrt(-2.0*mi*I_y_cap*(2.0*mi*C_mal+C_mq*C_Lal)));

        res_routh.omega_ph=C_We/(sqrt(2*pow(mi,2.0)))*2.0*V/desc_aer.corda;
        res_routh.omega_sp=sqrt(-C_mal/I_y_cap)*2.0*V/desc_aer.corda;

        res_routh.periodo_sp=2.0*M_PI/res_routh.omega_sp/(sqrt(1-pow(res_routh.smorz_sp,2.0)));
        res_routh.periodo_ph=2.0*M_PI/res_routh.omega_ph/(sqrt(1-pow(res_routh.smorz_ph,2.0)));

        res_routh.dimezzamento_sp=log(2)/fabs(res_routh.smorz_sp*res_routh.omega_sp);
        res_routh.dimezzamento_ph=log(2)/fabs(res_routh.smorz_ph*res_routh.omega_ph);
    }else{
        res_routh.is_din_stab=0;
    }

    return res_routh;
}








