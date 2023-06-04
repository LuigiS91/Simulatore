#include <stdio.h>
#include <stdlib.h>
#include "..\Funzioni\interpola.h"
#include "..\Funzioni\lettura_dati.h"


int main(){
    //Testo la funzione propeller
    printf("Test lettura funzione interpola\n");

    int i;
    double Cx[3]={-1,0,1};
    double ALPHA[3]={-1,0,1};
    double alpha[4]={-1,0.5,1,2};
    double Cx_res[4];
    t_descrizione_aereo desc_aer;
    t_limiti_comando lim_com;
    t_massa_combustibile massa_comb;
    t_dba dba;
    const char* file_dba="Dati\\dba.txt";
    int errore;

    set_dimensione_vettori_interpola(3); //in questo modo si setta una sola volta e non bisogna passarlo ogni volta. Un passaggio quindi inutile di un valore costante è evitabile

    printf("Esempio di database semplice per verificare l'effettiva interpolazione\n");
    printf("ALPHA\tCx(ALPHA)\n");
    for(i=0;i<3;i++)
    {
        printf("%f\t%f\n",ALPHA[i],Cx[i]);
    }

    for(i=0;i<4;i++)
    {
        Cx_res[i]=interpola(ALPHA,Cx,alpha[i]);
        printf("Esempio %d, valore i errore_interpola %d\n",i+1,errore_interpola);
    }

    printf("Esempio numero\talpha\tCx_res(alpha)\n");
    for(i=0;i<4;i++)
    {
        printf("%d\t%f\t%f\n",i+1,alpha[i],Cx_res[i]);
    }

    printf("\n\nTest lettura funzione interpola con database aerodinamico caricato\n");

    errore_interpola=0;//reset dell'errore di interpolazione per il test successivo


    errore=apri_file_lettura(file_dba);
    if(errore){
        printf("Errore apertura file!\nPremere un tato per terminare\n");
        getchar();
        return -1;
    }

    lettura_dati_intestazione_aerodinamica(&desc_aer,&lim_com,&massa_comb);

    alloca_dba(desc_aer.num_inc,&dba);
    set_dimensione_vettori_interpola(desc_aer.num_inc);//Setta la dimensione dei vettori da cui interpolare i dati

    lettura_dati_aerodinamici(&dba);

    printf("Alpha\tCx\tCy\tCz\tCl\tCm\tCn\n");
    for(i=0;i<desc_aer.num_inc;i++){
        printf("%lf\t%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",dba.ssc[I_ALPHA][i]*RAD_TO_DEG,dba.ssc[I_SSC_CX][i],dba.ssc[I_SSC_CY][i],dba.ssc[I_SSC_CZ][i],dba.ssc[I_SSC_CL][i],dba.ssc[I_SSC_CM][i],dba.ssc[I_SSC_CN][i]);
    }

    printf("Test di interpolazione di Cx per alpha = %lf\nCx = %lf\n",3.56,interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CX],3.56*DEG_TO_RAD));
    printf("Errore di interpolazione %d\n\n",errore_interpola);

    printf("Test di interpolazione di Cx per alpha = %lf\nCx = %lf\n",-5.,interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CX],-5*DEG_TO_RAD));
    printf("Errore di interpolazione %d\n\n",errore_interpola);

    printf("Test di interpolazione di Cx per alpha = %lf\nCx = %lf\n",20.,interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CX],20*DEG_TO_RAD));
    printf("Errore di interpolazione %d\n\n",errore_interpola);


    printf("Test di interpolazione di Cx per alpha = %lf\nCx = %lf\n",25.,interpola(dba.ssc[I_ALPHA],dba.ssc[I_SSC_CX],25*DEG_TO_RAD));
    printf("Errore di interpolazione %d\n\n",errore_interpola);

    chiudi_file_lettura();

    libera_dba(&dba);

    printf("Premere un tasto per terminare\n");
    getchar();

return 0;
}
