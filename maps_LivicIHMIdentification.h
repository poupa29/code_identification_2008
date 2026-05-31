/******************************************************************************
 
 (c) LIVIC 2005
 
 Ce fichier est la propriete du LIVIC. Toute utilisation, copie partielle ou
 totale, modification du fichier sans autorisation du LIVIC est interdite.
 
 Fichier          : $RCSfile: maps_LivicIHMIdentification.h,v $
 Date de creation : 2005/01/20
 Auteur           : MathiasPERROLLAZ
 Modifie le       : $Date: 2007/06/12 12:56:30 $ par $Author: bertrand $
 Version          : $Version: 1.0 $
 
 Inclus dans:
   Maps_LivicIHMIdentification.cpp
 MSdk:
   MSdkLivicIdentification
 Commentaire:
   Fichier d'en-tete.

*******************************************************************************/


#ifndef _Maps_LivicIHMIdentification_H
#define _Maps_LivicIHMIdentification_H

#include "../commun/LibTI/LivicTI.h"
#include "../../general/utils/livicutils.h"
#include "../../general/utils/chargementBMP.h"
#include "IR.h"
#include "maps.hpp"

#define DIM_IMAGES_PANNEAUX 600


// Declares a new MAPSComponent child class
class MAPSLivicIHMIdentification : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicIHMIdentification)

private :
    bool xb_firstTime;
	char                pc_message[10*TAILLE_MSG];

    unsigned char       *pu_ImageVirageDroite;
    unsigned char       *pu_ImageDebutZone30;
    unsigned char       *pu_ImageFinZone30;
    unsigned char       *pu_ImageVirageGauche;
    unsigned char       *pu_ImageChicane;
    unsigned char       *pu_ImagePanneauFeu;
    unsigned char       *pu_ImageFeu;
    unsigned char       *pu_ImageDanger;
    unsigned char       *pu_ImageMessage;
    unsigned char       *pu_ImageStop;

    unsigned char       *pu_ImageIHMIdentification;

    int xi_IDmessage;
    CvScalar xo_couleur;

public:

};

#endif



