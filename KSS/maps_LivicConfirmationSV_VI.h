////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

//////////////////////////////////////////////
//Composant de couplage entre KSS VI et KSS SV
//////////////////////////////////////////////

#ifndef _Maps_LivicConfirmationSV_VI_H
#define _Maps_LivicConfirmationSV_VI_H

// Includes maps sdk library header
#include "maps.hpp"
#include "../IR.h"
#include "../IR.cpp"
#include "../../../general/utils/livicutils.cpp" //Sinon bug de compilation

#define TAMPON_MOTES 5  //pour gerer jusqu'a X motes en meme temps

// Declares a new MAPSComponent child class
class MAPSLivicConfirmationSV_VI : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicConfirmationSV_VI)
private :
	int tolerance,sv_timestamp,debut_tolerance;
	unsigned int sv_id_recu, data[TAILLE_DATA_MOTE];
    //rajout pour la gestion de plusieurs motes
	unsigned int sv_id_tab[TAMPON_MOTES];
	unsigned int sv_timestamp_tab[TAMPON_MOTES];

	bool presence_id(unsigned int id);
	void remplir_tableau_id(void);
};

#endif
