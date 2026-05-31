////////////////////////////////
// SDK Programmer samples
////////////////////////////////

#ifndef _MAPSLivicZigbeeReceive_H
#define _MAPSLivicZigbeeReceive_H

#include "maps.hpp"
#include "../IR.h"

class MAPSLivicZigbeeReceive : public MAPSComponent 
{
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicZigbeeReceive)

	private :

		 int calcByte(int crc, int b);
		 int calc(unsigned char packet[], int index, int count);
};

#endif
