////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_PartageMemoire_H
#define _Maps_PartageMemoire_H

#include "IRMPClient/MemoireP.h"

// Includes maps sdk library header
#include "maps.hpp"
#include "IR.h"



// Declares a new MAPSComponent child class
class MAPSPartageMemoire : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSPartageMemoire)
private :
	// Place here your specific methods and attributes

	MemoireP *MP;


	int EtatSem;

	bool Start;
	float ObjectMP[4*MAX_CIBLES_IR+1];

	int vectorSize;
	
	MAPSIOElt *iEltPistes;


};

#endif
