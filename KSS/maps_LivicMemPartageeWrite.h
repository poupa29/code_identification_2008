////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

//////////////////////////////////////////////
//Composant d'écriture en mémoire partagée
//////////////////////////////////////////////

#ifndef _Maps_LivicMemPartageeWrite_H
#define _Maps_LivicMemPartageeWrite_H


// Includes maps sdk library header
#include "maps.hpp"

// Declares a new MAPSComponent child class
class MAPS_LivicMemPartageeWrite : public MAPSComponent 
{
	// Use standard header definition macro

	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPS_LivicMemPartageeWrite)
private :
	// Place here your specific methods and attributes
	HANDLE hMap;
	int *pi_a;
	MAPSIOElt *iEltPistes;
};

#endif
