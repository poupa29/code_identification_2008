

#ifndef _Maps_LivicChangementRepereIR_H
#define _Maps_LivicChangementRepereIR_H


#include "IR.h"
#include "../../General/utils/livicutils.h"
#include "../../General/utils/BasicDefs.h"
#include <math.h>
#include "maps.hpp"

#define AUCUN_FILTRE    0
#define FILTRE_PASSEBAS 1
#define FILTRE_MEDIAN   2


// Declares a new MAPSComponent child class
class MAPSLivicChangementRepereIR : public MAPSComponent 
{
    // Use standard header definition macro
    MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicChangementRepereIR)
private :
    // données
    bool                xb_firstTime;
    int                 xi_nbPistesIR;
    MAPSRealObject*     xo_pistesIRin;

    bool    xb_UtiliserIdentificationStereo;

    int                 xi_nbPistesIRGauche;
    MAPSRealObject*     xo_pistesIRinGauche;

    MAPSTimestamp       xtsp_timestampPistesIR;

    MAPSRealObject*     xo_pistesIRGauchein;

    double              xd_VariationHauteurVehicule;
    double              xd_TangageVehicule;

    int                 xi_NbAnciennePistes;
    int                 xi_NbPlusAnciennePistes;
    double              pd_XAnciennesPistesEntree1[MAX_CIBLES_IR];
    double              pd_ZAnciennesPistesEntree1[MAX_CIBLES_IR];
    double              pd_XAnciennesPistesEntree2[MAX_CIBLES_IR];
    double              pd_ZAnciennesPistesEntree2[MAX_CIBLES_IR];
    double              pd_XAnciennesPistesEntree3[MAX_CIBLES_IR];
    double              pd_ZAnciennesPistesEntree3[MAX_CIBLES_IR];
    double              pd_XAnciennesPistesEntree4[MAX_CIBLES_IR];
    double              pd_ZAnciennesPistesEntree4[MAX_CIBLES_IR];
    double              pd_XPlusAnciennesPistesEntree1[MAX_CIBLES_IR];
    double              pd_ZPlusAnciennesPistesEntree1[MAX_CIBLES_IR];
    double              pd_XPlusAnciennesPistesEntree2[MAX_CIBLES_IR];
    double              pd_ZPlusAnciennesPistesEntree2[MAX_CIBLES_IR];
    double              pd_XPlusAnciennesPistesEntree3[MAX_CIBLES_IR];
    double              pd_ZPlusAnciennesPistesEntree3[MAX_CIBLES_IR];
    double              pd_XPlusAnciennesPistesEntree4[MAX_CIBLES_IR];
    double              pd_ZPlusAnciennesPistesEntree4[MAX_CIBLES_IR];    
    int                 pi_IdPlusAnciennesPistes[MAX_CIBLES_IR];
    double              pd_XAnciennesPistesSortie1[MAX_CIBLES_IR];
    double              pd_ZAnciennesPistesSortie1[MAX_CIBLES_IR];
    double              pd_XAnciennesPistesSortie2[MAX_CIBLES_IR];
    double              pd_ZAnciennesPistesSortie2[MAX_CIBLES_IR];
    double              pd_XAnciennesPistesSortie3[MAX_CIBLES_IR];
    double              pd_ZAnciennesPistesSortie3[MAX_CIBLES_IR];
    double              pd_XAnciennesPistesSortie4[MAX_CIBLES_IR];
    double              pd_ZAnciennesPistesSortie4[MAX_CIBLES_IR];
    int                 pi_IdAnciennesPistes[MAX_CIBLES_IR];
    double              pd_SauvegardePistesFiltrees[4*4*MAX_CIBLES_IR];

    MAPSTimestamp MAPSLivicChangementRepereIR::mtsp_PistesIRSuivant();
};

#endif
