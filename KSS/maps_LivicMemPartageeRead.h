////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

//////////////////////////////////////////////
//Composant de lecture en mémoire partagée
//////////////////////////////////////////////

#ifndef _Maps_LivicMemPartageeRead_H
#define _Maps_LivicMemPartageeRead_H
//#define UNICODE

// Includes maps sdk library header
#include "maps.hpp"

// Declares a new MAPSComponent child class
class MAPSLivicMemPartageeRead : public MAPSComponent 
{

	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicMemPartageeRead)
	

private :
	// Place here your specific methods and attributes
	int *pi_b, valeur_precedente;
	MAPSIOElt *oEltPistes;
	HANDLE hMap;
};

#endif
