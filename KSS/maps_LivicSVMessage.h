////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

//////////////////////////////////////////////////
//Composant de synthèse de message "feu tricolore"
//////////////////////////////////////////////////


#ifndef _Maps_LivicSVMessage_H
#define _Maps_LivicSVMessage_H



// Includes maps sdk library header
#include "maps.hpp"
#include <stdlib.h>
#include "../IR.h"

// Declares a new MAPSComponent child class
class MAPSLivicSVMessage : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicSVMessage)

private :
	// Place here your specific methods and attributes
	int tolerance,debut_tolerance;
	char xs_message_precedent[TAILLE_MSG+1];
	int num;
};

#endif
