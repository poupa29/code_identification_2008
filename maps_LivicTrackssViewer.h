////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////
/******************************************************************************
 
 (c) LIVIC 2005
 
 Ce fichier est la propriete du LIVIC. Toute utilisation, copie partielle ou
 totale, modification du fichier sans autorisation du LIVIC est interdite.
 
 Fichier          : $RCSfile: maps_LivicTrackssViewer.h,v $
 Date de creation : 2005/01/20
 Auteur           : MathiasPERROLLAZ
 Modifie le       : $Date: 2008/07/29 08:59:05 $ par $Author: scenic2 $
 Version          : $Version: 1.0 $
 
 Inclus dans:
   maps_LivicTrackssViewer.cpp
 Commentaire:
   Fichier d'en-tete.

*******************************************************************************/


#ifndef _maps_LivicTrackssViewer_H
#define _maps_LivicTrackssViewer_H

// Multiple de 8 !!!!!!!
//image trackss
#define IMG_X 624
#define IMG_Y 83
//image banniere
#define IMG_BANNER_X 624
#define IMG_BANNER_Y 40

//Ecran de l'IHM
#define IHM_X 304
#define IHM_Y 344

//Ecran de la route
#define CENTRAGE_ROUTE_Y 110
#define ROUTE_X 320
#define ROUTE_Y 120

#define ECRAN_X 624 //IHM_X+ROUTE_X
#define ECRAN_Y 468 //IHM_Y+IMG_Y    ecran 4/3

#ifndef _BMP_H
	#include "../../general/utils/chargementBMP.h"
#endif

#ifndef _ImageIHM_H
	#include "../commun/libStereo/LivicImageIHM.h"
#endif

#include "maps.hpp"


// Declares a new MAPSComponent child class
class MAPSLivicTrackssViewer : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicTrackssViewer)

private :
	//	Proprietes des images:
	int i, j;
	int dimX, dimY, nbPix;
	bool firstTime;

	unsigned char       *pu_Image;
	unsigned char       *pu_Image2;
  
public:
	
};

#endif



