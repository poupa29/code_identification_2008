////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_LivicVI2CV_H
#define _Maps_LivicVI2CV_H

// Includes maps sdk library header
#include "maps.hpp"
#include "../IR.h"
#define LNG_TRAME_OCTETS 34

#define RAPPORT_RESOLUTIONX (640/320)
#define RAPPORT_RESOLUTIONY (480/120)

#define EVT_NO_EVENT 0
#define EVT_EMERGENCY_BRAKING 1
#define EVT_RIGHT_CURVE 2
#define EVT_LEFT_CURVE 3
#define EVT_ICE_ON_ROAD 4
#define EVT_GENERAL_WARNING 5

// Declares a new MAPSComponent child class
class MAPSLivicVI2CV : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicVI2CV)


	struct TRAME_VI{
		char ID;
		int X;
		int Y;
		int D;
		int ROIX;
		int ROIY;
		int ROIW;
		int ROIH;
		int INFO;
		char EVENT;
	};


private :
	// Place here your specific methods and attributes

	void DecodeEvenement(int *ident, int *event, int *filtre);

	TRAME_VI trames[MAX_CIBLES_IR];
	int temps_core;
	int temps_trame;
	//char trames_stream8[MAX_CIBLES_IR*LNG_TRAME_OCTETS];
	unsigned char *vers_trames_stream8;
	int nb_pistes;

};

#endif
