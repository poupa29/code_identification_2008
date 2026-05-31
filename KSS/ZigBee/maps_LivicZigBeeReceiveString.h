////////////////////////////////
// SDK Programmer samples
////////////////////////////////

#ifndef _MAPSLivicZigbeeReceiveString_H
#define _MAPSLivicZigbeeReceiveString_H

#include "maps.hpp"
#include "../../IR.h"

#define TAILLE_MSG_MAX 255

class MAPSLivicZigbeeReceiveString : public MAPSComponent 
{
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicZigbeeReceiveString)

	private :

		 int calcByte(int crc, int b);
		 int calc(unsigned char packet[], int index, int count);
};

#endif
