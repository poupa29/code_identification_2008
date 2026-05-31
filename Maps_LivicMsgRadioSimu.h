
#ifndef _Maps_LivicMsgRadioSimu_H
#define _Maps_LivicMsgRadioSimu_H

// Includes maps sdk library header
#include <stdlib.h>
#include "IR.h"
#include "maps.hpp"


// Declares a new MAPSComponent child class
class MAPSLivicMsgRadioSimu : public MAPSComponent 
{
    // Use standard header definition macro
    MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicMsgRadioSimu)
private :
    int pNombreDeMessages;
    char **messages;
    int *synchros;
	int pa1;
	char messageFeuTri[TAILLE_MSG+1];
	MAPSTimestamp tPrec;
    void Dynamic();
};

#endif
