////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

//////////////////////////////////////////////
//Composant d'écriture en mémoire partagée
//////////////////////////////////////////////

#ifndef _Maps_LivicKSMConversion_H
#define _Maps_LivicKSMConversion_H


// Includes maps sdk library header
#include "maps.hpp"
#include "R:/maps/perception/identification/IR.h"

// Declares a new MAPSComponent child class
class MAPS_LivicKSMConversion : public MAPSComponent 
{
	// Use standard header definition macro

	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPS_LivicKSMConversion)
private :
	// Place here your specific methods and attributes
    int sv_id,sv_id_brut;

	//Feu
  /*  MAPSInteger xi_TimeLeft;
    MAPSInteger xi_ColourCode;
    MAPSInteger xi_ColourCode_precedent;
    MAPSInteger xi_TimeLeft_precedent;*/
	int xi_TimeLeft,xi_ColourCode,xi_ColourCode_precedent,xi_TimeLeft_precedent;

    char xs_message[50];
};

#endif
