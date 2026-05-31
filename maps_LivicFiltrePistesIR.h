////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_LivicFiltrePistesIR_H
#define _Maps_LivicFiltrePistesIR_H

// Includes maps sdk library header
#include "IR.h"
#include "../../general/utils/livicutils.h"
#include "maps.hpp"

// Declares a new MAPSComponent child class
class MAPSLivicFiltrePistesIR : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicFiltrePistesIR)
private :
	// Place here your specific methods and attributes
    MAPSTimestamp       xt_oldTimestamp;
    LivicPilePointeur   xo_oldEntree;
    LivicPilePointeur   xo_oldSortie;

    MAPSFloat filtre(MAPSFloat x, MAPSFloat oldx, MAPSFloat oldxfiltre, MAPSFloat tempsBoucle);

};

#endif
