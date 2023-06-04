#include "include/trim.h"

int errore_trim=0;

t_res_trim trim(t_descrizione_aereo desc_aer,t_dba dba,t_engine engine,t_descrizione_propeller desc_prop,t_dbp dbp,t_atm_sl atm_sl,double h, double vel,double gamma){
    t_res_trim res_trim;
    t_atm atm;
    double alpha,alpha_m1,alpha_p1,Cz_tot,Cz_tot_m1,de,de_m1,T_trim,Cx_tot,rpm,rpm_m1,rpm_p1,T,T_m1,F,F_m1;
    int finito,i,rinizia;

    errore_trim=0;

    atm=atmosfera(h,atm_sl);
    finito=0;
/*
    alpha=dba.ssc[I_ALPHA][0];
    while((alpha<dba.ssc[I_ALPHA][dba.num_inc-1])&&(!finito)){
            de= -(interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CM],alpha)+interpola(dba.m_der[I_ALPHA],dba.m_der[I_M_DER_CmA],alpha)*alpha)
                    /
                (interpola(dba.com_mom_der[I_ALPHA],dba.com_mom_der[I_COM_MOM_DER_Cmde],alpha));
            Cz_tot= interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CZ],alpha)+interpola(dba.z_der[I_ALPHA],dba.z_der[I_Z_DER_CZA],alpha)*alpha
                   +interpola(dba.com_for_der[I_ALPHA],dba.com_for_der[I_COM_FOR_DER_CZde],alpha)*de;
        if(fabs(desc_aer.massa*const_g*cos((alpha+gamma))+0.5*Cz_tot*atm.dens*desc_aer.superficie_alare*pow(vel,2.0))<1)
            finito=1;
        else{
            alpha+=0.000001;
        }
    }

*/

    if(vel<=0||h<=0){
        errore_trim=1;
    }
    for(i=0;i<dba.num_inc&&!finito&&!errore_trim;i++){
        rinizia=0;
        alpha=dba.ssc[I_ALPHA][i];
        alpha_m1=dba.ssc[I_ALPHA][i+1];
        while(!finito&&!rinizia){
            de= -(interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CM],alpha)+interpola(dba.m_der[I_ALPHA],dba.m_der[I_M_DER_CmA],alpha)*alpha)
                        /
                    (interpola(dba.com_mom_der[I_ALPHA],dba.com_mom_der[I_COM_MOM_DER_Cmde],alpha));
            Cz_tot= interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CZ],alpha)+interpola(dba.z_der[I_ALPHA],dba.z_der[I_Z_DER_CZA],alpha)*alpha
                       +interpola(dba.com_for_der[I_ALPHA],dba.com_for_der[I_COM_FOR_DER_CZde],alpha)*de;
            F=desc_aer.massa*const_g*cos((alpha+gamma))+0.5*Cz_tot*atm.dens*desc_aer.superficie_alare*pow(vel,2.0);
            if(fabs(F)<RESIDUO_ALPHA)
                finito=1;
            else{
                    de_m1= -(interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CM],alpha_m1)+interpola(dba.m_der[I_ALPHA],dba.m_der[I_M_DER_CmA],alpha_m1)*alpha_m1)
                        /
                    (interpola(dba.com_mom_der[I_ALPHA],dba.com_mom_der[I_COM_MOM_DER_Cmde],alpha_m1));
                    Cz_tot_m1= interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CZ],alpha_m1)+interpola(dba.z_der[I_ALPHA],dba.z_der[I_Z_DER_CZA],alpha_m1)*alpha_m1
                       +interpola(dba.com_for_der[I_ALPHA],dba.com_for_der[I_COM_FOR_DER_CZde],alpha_m1)*de_m1;

                    F_m1=desc_aer.massa*const_g*cos((alpha_m1+gamma))+0.5*Cz_tot_m1*atm.dens*desc_aer.superficie_alare*pow(vel,2.0);

                    alpha_p1=alpha-(alpha-alpha_m1)/(F-F_m1)*F;
                    alpha_m1=alpha;
                    alpha=alpha_p1;
                }
            if(alpha<=dba.ssc[I_ALPHA][0]||alpha>=dba.ssc[I_ALPHA][dba.num_inc-1])
                rinizia=1;
        }
        if(i>dba.num_inc-1){
            //printf("Non si pu� trimmare il velivolo")
            finito=1;
            errore_trim=1;
        }
    }


    finito=0;
    Cx_tot= interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CX],alpha)
            +interpola(dba.x_der[I_ALPHA],dba.x_der[I_X_DER_CXA],alpha)*alpha
            +interpola(dba.com_for_der[I_ALPHA],dba.com_for_der[I_COM_FOR_DER_CXde],alpha)*de;
    T_trim=desc_aer.massa*const_g*sin((alpha+gamma))-0.5*atm.dens*desc_aer.superficie_alare*pow(vel,2.0)*Cx_tot;
    /*while(rpm<engine.max_rpm&&!finito){
            prop=propeller(desc_prop,dbp,atm,vel,rpm);
            if(fabs(T_trim-prop.thrust)<RESIDUO_RPM){
                finito=1;
            }
            else{
                rpm+=DELTA_RPM;
            }
    } */
    rpm_m1=engine.min_rpm;
    rpm=engine.max_rpm;

    i=0;
    while(!finito&&!errore_trim&&!errore_propeller){
        T=propeller(&desc_prop,&dbp,&atm,vel,rpm,RESIDUO_AEROMOBILE_PROP).thrust;
        if(fabs(T-T_trim)<RESIDUO_RPM){
            finito=1;
        }
        else{
            T_m1=propeller(&desc_prop,&dbp,&atm,vel,rpm_m1,RESIDUO_TRIM_PROP).thrust;
            rpm_p1=rpm-(rpm-rpm_m1)/(T-T_m1)*(T-T_trim);
            rpm_m1=rpm;
            rpm=rpm_p1;
        }
        i++;
        if(i>500)
            errore_trim=1;
    }
    if(rpm>engine.max_rpm||rpm<engine.min_rpm||errore_propeller) errore_trim=1;

    res_trim.stato.u=vel*cos(alpha);
    res_trim.stato.v=0;
    res_trim.stato.w=vel*sin(alpha);
    res_trim.stato.p=0;
    res_trim.stato.q=0;
    res_trim.stato.r=0;
    res_trim.stato.psi=0;
    res_trim.stato.phi=0;
    res_trim.stato.theta=(alpha+gamma);
    res_trim.stato.h=h;

    res_trim.comando.de=de;
    res_trim.comando.da=0;
    res_trim.comando.dr=0;
    res_trim.comando.rpm=rpm;

return res_trim;
}
