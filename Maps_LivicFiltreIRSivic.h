////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

#ifndef _Maps_LivicFiltreIRSivic_H
#define _Maps_LivicFiltreIRSivic_H

// Includes maps sdk library header

#include "IR.h"
#include "maps.hpp"

// Declares a new MAPSComponent child class
class MAPSLivicFiltreIRSivic : public MAPSComponent 
{
    // Use standard header definition macro
    MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicFiltreIRSivic)
private :
    // Place here your specific methods and attributes
    bool firstTime;
};

#endif
