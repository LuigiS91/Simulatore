#include "include/lettura_dati.h"

FILE* in_lettura=0;
int errore_lettura=0;

int alloca_dba(int num_inc,t_dba* dba){
    int i;
    dba->num_inc=num_inc;
    const char* errore="[-] Impossibile allocare l'area di memoria chiudere qualche programma e riprovare\n";

    for(i=0;i<STEADY_STATE_C_SIZE;i++){
        dba->ssc[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->ssc[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    for(i=0;i<X_DER_SIZE;i++){
        dba->x_der[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->x_der[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    for(i=0;i<Y_DER_SIZE;i++){
        dba->y_der[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->y_der[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    for(i=0;i<Z_DER_SIZE;i++){
        dba->z_der[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->z_der[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    for(i=0;i<L_DER_SIZE;i++){
        dba->l_der[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->l_der[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    for(i=0;i<M_DER_SIZE;i++){
        dba->m_der[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->m_der[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    for(i=0;i<N_DER_SIZE;i++){
        dba->n_der[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->n_der[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    for(i=0;i<COM_FOR_DER_SIZE;i++){
        dba->com_for_der[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->com_for_der[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    for(i=0;i<COM_MOM_DER_SIZE;i++){
        dba->com_mom_der[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->com_mom_der[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    for(i=0;i<ROT_DER_SIZE;i++){
        dba->rot_der[i]=(double*)malloc(sizeof(double)*num_inc);
        if(dba->rot_der[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
    
return 0;
}
void libera_dba(t_dba* dba){
    int i;

    for(i=0;i<STEADY_STATE_C_SIZE;i++){
        free(dba->ssc[i]);
        dba->ssc[i]=NULL;
    }
    for(i=0;i<X_DER_SIZE;i++){
        free(dba->x_der[i]);
        dba->x_der[i]=NULL;
    }
    for(i=0;i<Y_DER_SIZE;i++){
        free(dba->y_der[i]);
        dba->y_der[i]=NULL;
    }
    for(i=0;i<Z_DER_SIZE;i++){
        free(dba->z_der[i]);
        dba->z_der[i]=NULL;
    }
    for(i=0;i<L_DER_SIZE;i++){
        free(dba->l_der[i]);
        dba->l_der[i]=NULL;
    }
    for(i=0;i<M_DER_SIZE;i++){
        free(dba->m_der[i]);
        dba->m_der[i]=NULL;
    }
    for(i=0;i<N_DER_SIZE;i++){
        free(dba->n_der[i]);
        dba->n_der[i]=NULL;
    }
    for(i=0;i<COM_FOR_DER_SIZE;i++){
        free(dba->com_for_der[i]);
        dba->com_for_der[i]=NULL;
    }
    for(i=0;i<COM_MOM_DER_SIZE;i++){
        free(dba->com_mom_der[i]);
        dba->com_mom_der[i]=NULL;
    }
    for(i=0;i<ROT_DER_SIZE;i++){
        free(dba->rot_der[i]);
        dba->rot_der[i]=NULL;
    }
return;
}

int apri_file_lettura(const char* nomefile){
    in_lettura=fopen(nomefile,"r");
    if(in_lettura==0){
        printf("[-] Nessun file presente nella posizione %s\n",nomefile);
        return -1;
    }
    return 0;
}
int chiudi_file_lettura(){
    return fclose(in_lettura);
}

int lettura_dati_intestazione_aerodinamica(t_descrizione_aereo* descrizione_aereo,t_limiti_comando* limiti_comando,t_massa_combustibile* massa_combustibile)
{
    int terminato;
    char buf[NBUF];
    //Leggi la descrizione del velivolo
    terminato=0;
    while(!terminato){
        fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            sscanf(buf,"%lf%*[^\n]\n",&descrizione_aereo->massa);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->apertura_alare);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->superficie_alare);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->corda);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->mach_drag_rise);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->dist_T_x_z);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->dist_T_x_y);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->dist_T_x_x);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->ang_T_x_xy);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->ang_T_x_xz);
            //Converto i valori in radianti
            descrizione_aereo->ang_T_x_xy*=DEG_TO_RAD;
            descrizione_aereo->ang_T_x_xz*=DEG_TO_RAD;

            fscanf(in_lettura,"%d%*[^\n]\n",&descrizione_aereo->num_inc);
            fscanf(in_lettura,"%d%*[^\n]\n",&descrizione_aereo->rotary_der);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->center_cg_ref_loc);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->Jx);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->Jy);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->Jz);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->Jxz);
            fscanf(in_lettura,"%d%*[^\n]\n",&descrizione_aereo->opt_pos_cg);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->new_pos_cg);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->pos_pil_x_cg);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->pos_pil_y_cg);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_aereo->pos_pil_z_cg);
            terminato=1;
        }
    }
    //Leggi la deflection limits
    terminato=0;
    while(!terminato){
        fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {

            sscanf(buf,"%lf%*[^\n]\n",&limiti_comando->max_elevator);
            fscanf(in_lettura,"%lf%*[^\n]\n",&limiti_comando->min_elevator);
            fscanf(in_lettura,"%lf%*[^\n]\n",&limiti_comando->airelon);
            fscanf(in_lettura,"%lf%*[^\n]\n",&limiti_comando->rudder);
            fscanf(in_lettura,"%lf%*[^\n]\n",&limiti_comando->min_flap);
            fscanf(in_lettura,"%lf%*[^\n]\n",&limiti_comando->max_flap);
            //Converto i valori angolare in radianti
            limiti_comando->max_elevator*=DEG_TO_RAD;
            limiti_comando->min_elevator*=DEG_TO_RAD;
            limiti_comando->airelon*=DEG_TO_RAD;
            limiti_comando->rudder*=DEG_TO_RAD;
            limiti_comando->max_flap*=DEG_TO_RAD;
            limiti_comando->min_flap*=DEG_TO_RAD;
            terminato=1;
        }
    }
    //Leggi la massa del combustibile
    terminato=0;
    while(!terminato){
        fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            sscanf(buf,"%d%*[^\n]\n",&massa_combustibile->opt_massa);
            fscanf(in_lettura,"%f%*[^\n]\n",&massa_combustibile->mf_mtow);
            terminato=1;
        }
    }
    return 0;
}


int lettura_dati_aerodinamici(t_dba* dba){

    int terminato;
    char buf[NBUF];
    int i;

    //lettura dati steady state derivative
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf\n",&dba->ssc[I_ALPHA][i],&dba->ssc[I_SSC_CX][i],&dba->ssc[I_SSC_CY][i],&dba->ssc[I_SSC_CZ][i],&dba->ssc[I_SSC_CL][i],&dba->ssc[I_SSC_CM][i],&dba->ssc[I_SSC_CN][i]);
                //Converto i gradi in radianti
                dba->ssc[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    //lettura x derivatives
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf%lf\n",&dba->x_der[I_ALPHA][i],&dba->x_der[I_X_DER_CXA][i],&dba->x_der[I_X_DER_CXAP][i],&dba->x_der[I_X_DER_CXU][i],&dba->x_der[I_X_DER_CXQ][i],&dba->x_der[I_X_DER_CXB][i],&dba->x_der[I_X_DER_CXP][i],&dba->x_der[I_X_DER_CXR][i]);
                //Converto i gradi in radianti
                dba->x_der[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    //lettura y derivatives
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf\n",&dba->y_der[I_ALPHA][i],&dba->y_der[I_Y_DER_CYB][i],&dba->y_der[I_Y_DER_CYBP][i],&dba->y_der[I_Y_DER_CYP][i],&dba->y_der[I_Y_DER_CYR][i],&dba->y_der[I_Y_DER_CYA][i],&dba->y_der[I_Y_DER_CYQ][i]);
                //Converto i gradi in radianti
                dba->y_der[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    //lettura z derivatives
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf%lf\n",&dba->z_der[I_ALPHA][i],&dba->z_der[I_Z_DER_CZA][i],&dba->z_der[I_Z_DER_CZAP][i],&dba->z_der[I_Z_DER_CZU][i],&dba->z_der[I_Z_DER_CZQ][i],&dba->z_der[I_Z_DER_CZB][i],&dba->z_der[I_Z_DER_CZP][i],&dba->z_der[I_Z_DER_CZR][i]);
                //Converto i gradi in radianti
                dba->z_der[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    //lettura l derivatives
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf\n",&dba->l_der[I_ALPHA][i],&dba->l_der[I_L_DER_ClB][i],&dba->l_der[I_L_DER_ClBP][i],&dba->l_der[I_L_DER_ClP][i],&dba->l_der[I_L_DER_ClR][i],&dba->l_der[I_L_DER_ClA][i],&dba->l_der[I_L_DER_ClQ][i]);
                //Converto i gradi in radianti
                dba->l_der[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    //lettura m derivatives
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf%lf\n",&dba->m_der[I_ALPHA][i],&dba->m_der[I_M_DER_CmA][i],&dba->m_der[I_M_DER_CmAP][i],&dba->m_der[I_M_DER_CmU][i],&dba->m_der[I_M_DER_CmQ][i],&dba->m_der[I_M_DER_CmB][i],&dba->m_der[I_M_DER_CmP][i],&dba->m_der[I_M_DER_CmR][i]);
                //Converto i gradi in radianti
                dba->m_der[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    //lettura n derivatives
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf\n",&dba->n_der[I_ALPHA][i],&dba->n_der[I_N_DER_CnB][i],&dba->n_der[I_N_DER_CnBP][i],&dba->n_der[I_N_DER_CnP][i],&dba->n_der[I_N_DER_CnR][i],&dba->n_der[I_N_DER_CnA][i],&dba->n_der[I_N_DER_CnQ][i]);
                //Converto i gradi in radianti
                dba->n_der[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    //lettura control force derivatives
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf\n",&dba->com_for_der[I_ALPHA][i],&dba->com_for_der[I_COM_FOR_DER_CXde][i],&dba->com_for_der[I_COM_FOR_DER_CXdle][i],&dba->com_for_der[I_COM_FOR_DER_CZde][i],&dba->com_for_der[I_COM_FOR_DER_CZdle][i],&dba->com_for_der[I_COM_FOR_DER_CYda][i],&dba->com_for_der[I_COM_FOR_DER_CYdr][i]);
                //Converto i gradi in radianti
                dba->com_for_der[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    //lettura control moment derivatives
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf\n",&dba->com_mom_der[I_ALPHA][i],&dba->com_mom_der[I_COM_MOM_DER_Clda][i],&dba->com_mom_der[I_COM_MOM_DER_Cldr][i],&dba->com_mom_der[I_COM_MOM_DER_Cmde][i],&dba->com_mom_der[I_COM_MOM_DER_Cmdle][i],&dba->com_mom_der[I_COM_MOM_DER_Cnda][i],&dba->com_mom_der[I_COM_MOM_DER_Cndr][i]);
                //Converto i gradi in radianti
                dba->com_mom_der[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    //lettura rotary derivatives
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dba->num_inc;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf%lf%lf%lf\n",&dba->rot_der[I_ALPHA][i],&dba->rot_der[I_ROT_DER_CXom][i],&dba->rot_der[I_ROT_DER_CYom][i],&dba->rot_der[I_ROT_DER_CZom][i],&dba->rot_der[I_ROT_DER_Clom][i],&dba->rot_der[I_ROT_DER_Cmom][i],&dba->rot_der[I_ROT_DER_Cnom][i]);
                //Converto i gradi in radianti
                dba->rot_der[I_ALPHA][i]*=DEG_TO_RAD;
            }
            terminato=1;
        }
    }
    return 0;
}

int lettura_engine(t_engine* engine){
    int terminato;
    char buf[NBUF];

    //Leggi la descrizione del velivolo
    terminato=0;
    while(!terminato){
        fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            sscanf(buf,"%f%*[^\n]\n",&engine->p_max);
            fscanf(in_lettura,"%f%*[^\n]\n",&engine->esp);
            fscanf(in_lettura,"%d%*[^\n]\n",&engine->min_rpm);
            fscanf(in_lettura,"%d%*[^\n]\n",&engine->max_rpm);
            fscanf(in_lettura,"%f%*[^\n]\n",&engine->rapp_rid);
            fscanf(in_lettura,"%f%*[^\n]\n",&engine->rend_mecc_trad);
            fscanf(in_lettura,"%lf%*[^\n]\n",&engine->sfc);
            terminato=1;
        }
    }
return 0;
}

int lettura_intestazione_propeller(t_descrizione_propeller* descrizione_propeller){
    int terminato;
    char buf[NBUF];

    //Leggi la prima parte di intestazione del file propeller
    terminato=0;
    while(!terminato){
        fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            sscanf(buf,"%f%*[^\n]\n",&descrizione_propeller->diametro);
            fscanf(in_lettura,"%f%*[^\n]\n",&descrizione_propeller->diametro_ogiva);
            fscanf(in_lettura,"%d%*[^\n]\n",&descrizione_propeller->n_pale);
            fscanf(in_lettura,"%f%*[^\n]\n",&descrizione_propeller->inerzia);
            fscanf(in_lettura,"%d%*[^\n]\n",&descrizione_propeller->num_stazioni);
            terminato=1;
        }
    }
    //Leggi la seconda parte di intestazione del file propeller
    terminato=0;
    while(!terminato){
        fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            sscanf(buf,"%lf%*[^\n]\n",&descrizione_propeller->Cla);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_propeller->Cl0);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_propeller->a0);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_propeller->Cda2);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_propeller->Cda);
            fscanf(in_lettura,"%lf%*[^\n]\n",&descrizione_propeller->Cd0);
            terminato=1;
        }
    }
return 0;
}

int lettura_stazioni_propeller(t_dbp* dbp,double diam_hub){
    int terminato;
    char buf[NBUF];
    int i;

    //lettura dati steady state derivative
    terminato=0;
    while(!terminato){
            fscanf(in_lettura,"%[^\n]\n",buf);
        if(buf[0]!='*')
        {
            for(i=0;i<dbp->num_staz;i++)
            {
                fscanf(in_lettura,"%lf%lf%lf%lf\n",&dbp->staz[I_STAZ_PROP_CSI][i],&dbp->staz[I_STAZ_PROP_RD][i],&dbp->staz[I_STAZ_PROP_CH_AD][i],&dbp->staz[I_STAZ_PROP_BA][i]);
                if(dbp->staz[I_STAZ_PROP_RD][i]<=diam_hub/2.0)
                    dbp->staz_fuorihub=i+1;
            }
            terminato=1;
        }
    }
return 0;
}

int alloca_dbp(int num_staz,t_dbp* dbp){
    int i;
    dbp->num_staz=num_staz;
    const char* errore="[-] Impossibile allocare l'area di memoria: chiudere qualche programma e riprovare\n";

    for(i=0;i<STAZ_PROP_SIZE;i++){
        dbp->staz[i]=(double*)malloc(sizeof(double)*num_staz);
        if(dbp->staz[i]==NULL){
                printf("%s",errore);
                return -1;
        }
    }
return 0;
}
void libera_dbp(t_dbp* dbp){
    int i;
    for(i=0;i<STAZ_PROP_SIZE;i++){
        free(dbp->staz[i]);
        dbp->staz[i]=NULL;
    }
}
