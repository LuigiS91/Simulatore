#include <stdio.h>
#include <stdlib.h>
#include "../src/lib/include/lettura_dati.h"

int test_lettura()
{
    t_descrizione_aereo descrizione_aereo;
    t_limiti_comando limiti_comando;
    t_massa_combustibile massa_combustibile;
    t_dba dba;
    t_engine engine;
    t_descrizione_propeller desc_prop;
    t_dbp dbp;

    int i;
    int errore;

    const char* file_dba="../../etc/dba.txt";
    const char* file_engine="../../etc/engine.txt";
    const char* file_propeller="../../etc/propeller.txt";


    errore=apri_file_lettura(file_dba); //apro il file dba
    if(errore){
        printf("Errore apertura file dba!\nPremere un tato per terminare\n");
        getchar();
        return -1;
    }

    printf("Test lettura dati!\n");
    errore=lettura_dati_intestazione_aerodinamica(&descrizione_aereo,&limiti_comando,&massa_combustibile);
    if(errore){
        printf("Errore lettura dati intestazione!\nPremere un tato per terminare\n");
        getchar();
        return -1;
    }

    printf("Massa velivolo: %lf\n",descrizione_aereo.massa);
    printf("Apertura alare: %lf\n",descrizione_aereo.apertura_alare);
    printf("Superficie alare: %lf\n",descrizione_aereo.superficie_alare);
    printf("Corda: %lf\n",descrizione_aereo.corda);
    printf("Mach drag rise: %lf\n",descrizione_aereo.mach_drag_rise);
    printf("Distanza T x z: %lf\n",descrizione_aereo.dist_T_x_z);
    printf("Distanza T x y: %lf\n",descrizione_aereo.dist_T_x_y);
    printf("Distanza T x x: %lf\n",descrizione_aereo.dist_T_x_x);
    printf("Numero di incidenze: %d\n",descrizione_aereo.num_inc);
    printf("Rotary derivatives: %d\n",descrizione_aereo.rotary_der);
    printf("Angolo tra T e x xy: %lf\n",descrizione_aereo.ang_T_x_xy*RAD_TO_DEG);
    printf("Angolo tra T e x xz: %lf\n",descrizione_aereo.ang_T_x_xz*RAD_TO_DEG);
    printf("Jx: %lf\n",descrizione_aereo.Jx);
    printf("Jy: %lf\n",descrizione_aereo.Jy);
    printf("Jz: %lf\n",descrizione_aereo.Jz);
    printf("Jxz: %lf\n",descrizione_aereo.Jxz);
    printf("Opt pos baricentro: %d\n",descrizione_aereo.opt_pos_cg);
    printf("Nuova pos baricentro: %lf\n",descrizione_aereo.new_pos_cg);
    printf("Pos pilota x: %lf\n",descrizione_aereo.pos_pil_x_cg);
    printf("Pos pilota y: %lf\n",descrizione_aereo.pos_pil_y_cg);
    printf("Pos pilota z: %lf\n",descrizione_aereo.pos_pil_z_cg);

    printf("Massimo angolo di elevatore: %lf\n",limiti_comando.max_elevator*RAD_TO_DEG);
    printf("Minimo angolo di elevatore: %lf\n",limiti_comando.min_elevator*RAD_TO_DEG);
    printf("Massimo angolo di airelon: %lf\n",limiti_comando.airelon*RAD_TO_DEG);
    printf("Massimo angolo di rudder: %lf\n",limiti_comando.rudder*RAD_TO_DEG);
    printf("Massimo angolo di flap: %lf\n",limiti_comando.max_flap*RAD_TO_DEG);
    printf("Minimo angolo di flap: %lf\n",limiti_comando.min_flap*RAD_TO_DEG);



    printf("Opzione massa: %d\n",massa_combustibile.opt_massa);
    printf("Percentuale di massa fuel su massa totale: %lf\n",massa_combustibile.mf_mtow);

    alloca_dba(descrizione_aereo.num_inc,&dba);

    printf("\n\nAllocati %d elementi per ogni tabella del database\n\n",dba.num_inc);

    lettura_dati_aerodinamici(&dba);

    printf("Alpha\tCx\tCy\tCz\tCl\tCm\tCn\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.ssc[I_ALPHA][i]*RAD_TO_DEG,dba.ssc[I_SSC_CX][i],dba.ssc[I_SSC_CY][i],dba.ssc[I_SSC_CZ][i],dba.ssc[I_SSC_CL][i],dba.ssc[I_SSC_CM][i],dba.ssc[I_SSC_CN][i]);
    }

    printf("Alpha\tCxa\tCxap\tCxu\tCxq\tCxb\tCxp\tCxr\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.x_der[I_ALPHA][i]*RAD_TO_DEG,dba.x_der[I_X_DER_CXA][i],dba.x_der[I_X_DER_CXAP][i],dba.x_der[I_X_DER_CXU][i],dba.x_der[I_X_DER_CXQ][i],dba.x_der[I_X_DER_CXB][i],dba.x_der[I_X_DER_CXP][i],dba.x_der[I_X_DER_CXR][i]);
    }

    printf("Alpha\tCyb\tCybp\tCyp\tCyr\tCya\tCyq\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.y_der[I_ALPHA][i]*RAD_TO_DEG,dba.y_der[I_Y_DER_CYB][i],dba.y_der[I_Y_DER_CYBP][i],dba.y_der[I_Y_DER_CYP][i],dba.y_der[I_Y_DER_CYR][i],dba.y_der[I_Y_DER_CYA][i],dba.y_der[I_Y_DER_CYQ][i]);
    }

    printf("Alpha\tCza\tCzap\tCzu\tCzq\tCzb\tCzp\tCzr\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.z_der[I_ALPHA][i]*RAD_TO_DEG,dba.z_der[I_Z_DER_CZA][i],dba.z_der[I_Z_DER_CZAP][i],dba.z_der[I_Z_DER_CZU][i],dba.z_der[I_Z_DER_CZQ][i],dba.z_der[I_Z_DER_CZB][i],dba.z_der[I_Z_DER_CZP][i],dba.z_der[I_Z_DER_CZR][i]);
    }

    printf("Alpha\tClb\tClbp\tClp\tClr\tCla\tClq\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.l_der[I_ALPHA][i]*RAD_TO_DEG,dba.l_der[I_L_DER_ClB][i],dba.l_der[I_L_DER_ClBP][i],dba.l_der[I_L_DER_ClP][i],dba.l_der[I_L_DER_ClR][i],dba.l_der[I_L_DER_ClA][i],dba.l_der[I_L_DER_ClQ][i]);
    }

    printf("Alpha\tCma\tCmap\tCmu\tCmq\tCmb\tCmp\tCmr\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.m_der[I_ALPHA][i]*RAD_TO_DEG,dba.m_der[I_M_DER_CmA][i],dba.m_der[I_M_DER_CmAP][i],dba.m_der[I_M_DER_CmU][i],dba.m_der[I_M_DER_CmQ][i],dba.m_der[I_M_DER_CmB][i],dba.m_der[I_M_DER_CmP][i],dba.m_der[I_M_DER_CmR][i]);
    }

    printf("Alpha\tCnb\tCnbp\tCnp\tCnr\tCna\tCnq\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.n_der[I_ALPHA][i]*RAD_TO_DEG,dba.n_der[I_N_DER_CnB][i],dba.n_der[I_N_DER_CnBP][i],dba.n_der[I_N_DER_CnP][i],dba.n_der[I_N_DER_CnR][i],dba.n_der[I_N_DER_CnA][i],dba.n_der[I_N_DER_CnQ][i]);
    }

    printf("Alpha\tCXde\tCXdle\tCZde\tCZdle\tCYda\tCYdr\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.com_for_der[I_ALPHA][i]*RAD_TO_DEG,dba.com_for_der[I_COM_FOR_DER_CXde][i],dba.com_for_der[I_COM_FOR_DER_CXdle][i],dba.com_for_der[I_COM_FOR_DER_CZde][i],dba.com_for_der[I_COM_FOR_DER_CZdle][i],dba.com_for_der[I_COM_FOR_DER_CYda][i],dba.com_for_der[I_COM_FOR_DER_CYdr][i]);
    }

    printf("Alpha\tClda\tCldr\tCmde\tCmdle\tCnda\tCndr\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.com_mom_der[I_ALPHA][i]*RAD_TO_DEG,dba.com_mom_der[I_COM_MOM_DER_Clda][i],dba.com_mom_der[I_COM_MOM_DER_Cldr][i],dba.com_mom_der[I_COM_MOM_DER_Cmde][i],dba.com_mom_der[I_COM_MOM_DER_Cmdle][i],dba.com_mom_der[I_COM_MOM_DER_Cnda][i],dba.com_mom_der[I_COM_MOM_DER_Cndr][i]);
    }

    printf("Alpha\tCXom\tCYom\tCZom\tClom\tCmom\tCnom\n");
    for(i=0;i<descrizione_aereo.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.rot_der[I_ALPHA][i]*RAD_TO_DEG,dba.rot_der[I_ROT_DER_CXom][i],dba.rot_der[I_ROT_DER_CYom][i],dba.rot_der[I_ROT_DER_CZom][i],dba.rot_der[I_ROT_DER_Clom][i],dba.rot_der[I_ROT_DER_Cmom][i],dba.rot_der[I_ROT_DER_Cnom][i]);
    }

    chiudi_file_lettura();
    errore=apri_file_lettura(file_engine); // apro il file engine
    if(errore){
        printf("Errore lettura dati engine!\nPremere un tasto per terminare\n");
        getchar();
        return -1;
    }

    lettura_engine(&engine);

    printf("Potenza massima a sea level: %f\n",engine.p_max);
    printf("Esponente della legge di variazione del motore con la quota: %f\n",engine.esp);
    printf("Minimo numero di giri motore: %d\n",engine.min_rpm);
    printf("Massimo numero di giri motore: %d\n",engine.max_rpm);
    printf("Rapporto di riduzione/moltiplicazione: %f\n",engine.rapp_rid);
    printf("Rendimento meccanico della trasmissione: %f\n",engine.rend_mecc_trad);
    printf("Consumo specifico di combustibile: %e\n",engine.sfc);

    chiudi_file_lettura();
    errore=apri_file_lettura(file_propeller); // apro il file engine
    if(errore){
        printf("Errore lettura dati propeller!\nPremere un tato per terminare\n");
        getchar();
        return -1;
    }
    lettura_intestazione_propeller(&desc_prop);

    printf("Diametro: %f\n",desc_prop.diametro);
    printf("Diametro hub: %f\n",desc_prop.diametro_ogiva);
    printf("Numero pale: %d\n",desc_prop.n_pale);
    printf("Inerzia: %f\n",desc_prop.inerzia);
    printf("Numero stazioni: %d\n",desc_prop.num_stazioni);

    printf("Cla: %lf\n",desc_prop.Cla);
    printf("Cl0: %lf\n",desc_prop.Cl0);
    printf("a0: %lf\n",desc_prop.a0);
    printf("Cda2: %lf\n",desc_prop.Cda2);
    printf("Cda: %lf\n",desc_prop.Cda);
    printf("Cd0: %lf\n",desc_prop.Cd0);

    alloca_dbp(desc_prop.num_stazioni,&dbp);

    lettura_stazioni_propeller(&dbp,desc_prop.diametro_ogiva);

    printf("CSI\tRD [m]\tCH AD\tBA [deg]\n");
    for(i=0;i<desc_prop.num_stazioni;i++){
        printf("%lf\t%lf\t%lf\t%lf\n",dbp.staz[I_STAZ_PROP_CSI][i],dbp.staz[I_STAZ_PROP_RD][i],dbp.staz[I_STAZ_PROP_CH_AD][i],dbp.staz[I_STAZ_PROP_BA][i]);
    }


    chiudi_file_lettura();

    libera_dba(&dba);
    libera_dbp(&dbp);


    return 0;
}
