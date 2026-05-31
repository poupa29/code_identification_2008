#ifndef _Maps_LivicMsgSonore_H
#define _Maps_LivicMsgSonore_H


// Includes maps sdk library header
#include "maps.hpp"
#include "../IR.h"

//Chemin des fichiers audio :
#define ALERTE_FREINAGE			"R:/maps/perception/identification/Sons_IHM/freinage.WAV"
#define ALERTE_VIRAGE_DROITE	"R:/maps/perception/identification/Sons_IHM/virage_droite.WAV"
#define ALERTE_VIRAGE_GAUCHE	"R:/maps/perception/identification/Sons_IHM/virage_gauche.WAV"
#define ALERTE_OBSTACLE			"R:/maps/perception/identification/Sons_IHM/obstacle.WAV"
#define ALERTE_FEU_VERT			"R:/maps/perception/identification/Sons_IHM/feuvert.WAV"
#define ALERTE_ADHERENCE		"R:/maps/perception/identification/Sons_IHM/danger.WAV"
#define WARNING					"R:/maps/perception/identification/Sons_IHM/danger.WAV"

#define MSG_DEBUT_30			"R:/maps/perception/identification/Sons_IHM/30km.WAV"
#define MSG_FIN_30				"R:/maps/perception/identification/Sons_IHM/finlimitation.WAV"
#define MSG_VIRAGE_GAUCHE		"R:/maps/perception/identification/Sons_IHM/Pvirage.WAV"
#define MSG_CHICANE				"R:/maps/perception/identification/Sons_IHM/chicane.WAV"
#define MSG_DANGER				"R:/maps/perception/identification/Sons_IHM/danger.WAV"
#define MSG_STOP				"R:/maps/perception/identification/Sons_IHM/stop.WAV"
#define MSG_FEU					"R:/maps/perception/identification/Sons_IHM/feu.WAV"
#define MSG_VITESSE_90			"R:/maps/perception/identification/Sons_IHM/90km.WAV"
#define MSG_VITESSE_50			"R:/maps/perception/identification/Sons_IHM/50km.WAV"
#define MSG_FIN_VITESSE_50		"R:/maps/perception/identification/Sons_IHM/finlimitation.WAV"


// Declares a new MAPSComponent child class
class MAPSLivicMsgSonore : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicMsgSonore)
private :
	//Tache de lecture du son
	void LectureSon();
	MAPSEvent LectureEnCours;
	MAPSMutex mTempsPause;
	int TempsPause;		//en ms 
	char sons[100],sonsprecedant[100];

	//Methodes virtuelles
	void Set(MAPSProperty &p, MAPSInt64 value);
	void Set(MAPSProperty &p, MAPSFloat value);


public:

};

#endif
