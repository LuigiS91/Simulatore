#ifndef INTEGRATORE_H_INCLUDED
#define INTEGRATORE_H_INCLUDED

#include "aeromobile.h"

t_stato integra(const t_stato *stato0,const t_comando *com,const t_dba *dba,const t_dbp *dbp,const t_descrizione_aereo *desc_aer,const t_descrizione_propeller *desc_prop,const t_atm_sl *atm_sl,double dt);


#endif // INTEGRATORE_H_INCLUDED
