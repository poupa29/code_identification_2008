////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

#ifndef _Maps_LivicPistesIRViewer_H
#define _Maps_LivicPistesIRViewer_H

// Includes maps sdk library header
#include "maps.hpp"
#include "../../general/utils/livicutils.h"
#include "IR.h"



// Declares a new MAPSComponent child class
class MAPSLivicPistesIRViewer : public MAPSComponent 
{
    // Use standard header definition macro
    MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicPistesIRViewer)
private :
    // données
    bool                xb_firstTime;
    char                xs_msg[512];
    int                 xi_nbPistesIR;
    MAPSRealObject*     po_pistesIR;
    unsigned char*      pu_ib;
    MAPSTimestamp       xt_timestampImage;
    MAPSTimestamp       xt_timestampPistesIR;
    int                 xi_dimx;
    int                 xi_dimy;
    LivicPilePointeur   xo_messages;
    LivicPileInt64      xo_synchros;

    // méthodes
    MAPSTimestamp       mtsp_ImageSuivant();
    MAPSTimestamp       mtsp_PistesIRSuivant();
    void                mv_MessageSuivant();
};

#endif
