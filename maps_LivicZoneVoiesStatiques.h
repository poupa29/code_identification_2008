////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_LivicZoneVoiesStatiques_H
#define _Maps_LivicZoneVoiesStatiques_H

// Includes maps sdk library header
#include "maps.hpp"
#include "IR.h"

// Declares a new MAPSComponent child class
class MAPSLivicZoneVoiesStatiques : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicZoneVoiesStatiques)
public :
    bool enSaisie;
    static bool actionSaisie, actionEffacer;
private :
    int lastIndice, lastX, lastY;
    LivicPileInt x1, y1, x2, y2;
    bool finSaisie, firstTime;
    int xi_dimx, xi_dimy;
	static void Saisie(MAPSModule *module, int nbAction);
	static void Effacer(MAPSModule *module, int nbAction);
    void AfficheImageSortie();
    void DebutSaisie();
    void FinSaisie();
    void Effacer();
};

#endif
