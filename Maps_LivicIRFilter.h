////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

#ifndef _Maps_LivicIRFilter_H
#define _Maps_LivicIRFilter_H

// Includes maps sdk library header

#include "../commun/LibTI/LivicTI.h"

#include "../../general/utils/chargementBMP.h"
//#include "../../general/utils/livicutils.h"
#include "IR.h"
#include "../commun/libStereo/LivicImageIHM.h"
#include "maps.hpp"


#define	MAX_MESSAGES_INFRA	50
#define DIM_IMAGES_PANNEAUX 600
#define MAX_ROADMARKERS		100


// Declares a new MAPSComponent child class
class MAPSLivicIRFilter : public MAPSComponent 
{
    // Use standard header definition macro
    MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicIRFilter)
private :
    // données
    bool                xb_firstTime;
    char                xs_msg[512];
    int                 xi_nbPistesIR;
    MAPSRealObject*     po_pistesIR;
    unsigned char*      pu_ib;
    MAPSTimestamp       xt_timestampImage;
    MAPSTimestamp       xt_timestampPistesIR;
    MAPSTimestamp       xt_timestampMessages;
    MAPSTimestamp       TempsAffichageMessage;
   

    /*int                 xi_dimx;
    int                 xi_dimy;*/
    /*LivicPilePointeur   xo_messages;
    LivicPileInt64      xo_synchros;*/
	/*int                 xo_messageTimestamp;*/
	int                 xo_messageID;
    char                ps_MessagesInfrastructure[MAX_MESSAGES_INFRA][10*TAILLE_MSG];
    int                 xi_nbMessagesInfrastructure;
    char*               message;
    
    int					xi_NbPtDroiteZoneFiltrage;
	int					xi_NbPtGaucheZoneFiltrage;
	double				pd_PtsDroiteZoneFiltrage[2*MAX_ROADMARKERS];
	double				pd_PtsGaucheZoneFiltrage[2*MAX_ROADMARKERS];

    /*MAPSInteger         pb_sortieAffichageZone*/

    MAPSInteger         FUPrecedent;
	MAPSInteger			FU;
	MAPSTimestamp		timestamp;
    /*LivicImageIHM       IHMConducteur;*/

    // méthodes
    MAPSTimestamp       mtsp_PistesIRSuivant();
    void                mv_MessageSuivant();
    void                mv_RecupZoneFiltrageMessages();
    bool                mv_FiltrageMessages();

};

#endif
