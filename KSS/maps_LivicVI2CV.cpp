////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

#include "maps_LivicVI2CV.h"	// Includes the header of this component
#include <stdlib.h>
#include <stdio.h>

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicVI2CV)
	MAPS_INPUT("iPistesCamEntree",MAPS::FilterRealObjects,MAPS::SamplingReader)
	MAPS_INPUT("iPistesVehicule",MAPS::FilterRealObjects,MAPS::SamplingReader)
	MAPS_INPUT("iMessages",MAPS::FilterStream8,MAPS::FifoReader)
	MAPS_INPUT("iMessagesFiltres",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicVI2CV)
	MAPS_OUTPUT("oTrame",MAPS::Stream8,NULL,NULL,LNG_TRAME_OCTETS)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicVI2CV)
	MAPS_PROPERTY("pTrame",5000,false,true)
	MAPS_PROPERTY("pCore",40000,false,true)
	MAPS_PROPERTY("pLargeur_voiture",200,false,false)
	MAPS_PROPERTY("pHauteur_voiture",200,false,false)
	MAPS_PROPERTY("pCoefZoomDepart",1.0,false,true) //pour le zoom :
	MAPS_PROPERTY("pCoefZoomFin",0.5,false,true)
	MAPS_PROPERTY("pDistanceZoomMax",50,false,true)
	MAPS_PROPERTY("pDistanceZoomMin",10,false,true)
	MAPS_PROPERTY("pOffsetYCam",2,false,true) //decalage de la camera
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicVI2CV)
//MAPS_ACTION("aName",MAPSLivicVI2CV::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicVI2CV) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicVI2CV,"LivicVI2CV","1.0",128,
						  MAPS::Threaded,MAPS::Threaded,
						  4, // Nb of inputs
						  1, // Nb of outputs
						  9, // Nb of properties
						  0) // Nb of actions


void MAPSLivicVI2CV::Birth()
{
}

void MAPSLivicVI2CV::Core() 
{

	temps_core = MAPS::CurrentTime();
	MAPSIOElt *iEltRealObject = StartReading(Input("iPistesCamEntree"));
	MAPSRealObject *iRealObject = &(iEltRealObject->RealObject());
	//Pour recuperer la distance ...
	MAPSIOElt *iEltRealObject2 = StartReading(Input("iPistesVehicule"));
	MAPSRealObject *iRealObject2 = &(iEltRealObject2->RealObject());


	nb_pistes = min( iEltRealObject->VectorSize(), MAX_CIBLES_IR-1 );
	int filtre = 0;
	int idevent = 0;
	int event = 0;
	bool eventTraite = false;
	float coef;

	//Recupere les evenements:
	DecodeEvenement(&idevent, &event, &filtre);
	
	for(int i=0; i<nb_pistes; i++)
	{
		trames[i].ID = (char)iRealObject[i].id;
		trames[i].X = (int)iRealObject[i].x * RAPPORT_RESOLUTIONX;
		trames[i].Y = (int)(iRealObject[i].y + GetIntegerProperty("pOffsetYCam")) * RAPPORT_RESOLUTIONY;
		//On recupere la distance sur la piste oPisteIrVehicule
		trames[i].D = (int)iRealObject2[i].z;
		//Gestion distance Max
		if(trames[i].D>GetIntegerProperty("pDistanceZoomMax")) 
				trames[i].D = GetIntegerProperty("pDistanceZoomMax");

		//Filtre sur la distance
		//FiltreDistance( &trames[i].D ); //A TESTER !!!

		//Calcul du Coef de Zoom
		coef=      (GetFloatProperty("pCoefZoomFin")-GetFloatProperty("pCoefZoomDepart"))/
			(GetIntegerProperty("pDistanceZoomMax")-GetIntegerProperty("pDistanceZoomMin"))*trames[i].D 
			 + GetFloatProperty("pCoefZoomDepart") +             GetFloatProperty("pCoefZoomDepart")/
												 (GetIntegerProperty("pDistanceZoomMax")-GetIntegerProperty("pDistanceZoomMin"));
		
		//Zone de cadrage + coordonnées
		trames[i].ROIW = (int)GetIntegerProperty("pLargeur_voiture")*coef;
		trames[i].ROIH = (int)GetIntegerProperty("pHauteur_voiture")*coef;
		trames[i].ROIX = (int)trames[i].X - trames[i].ROIW / 2;
		trames[i].ROIY = (int)trames[i].Y - trames[i].ROIH / 2;

		//Filtrage en fonction de L'id pour les evenements
		if( trames[i].ID == idevent ) {
			trames[i].INFO = filtre;
			trames[i].EVENT = (char)event;
			eventTraite = true;
		}
		else
		{
			trames[i].INFO = 0;
			trames[i].EVENT = 0;
		}
	}

	// traitement du message venant d'un id non visible
	if( (!eventTraite) && ( event!=0 ) )
	{
		trames[nb_pistes].ID = 0;
		trames[nb_pistes].X = 0;
		trames[nb_pistes].Y = 0;
		trames[nb_pistes].D = 0;
		trames[nb_pistes].ROIW = 0;
		trames[nb_pistes].ROIH = 0;
		trames[nb_pistes].ROIX = 0;
		trames[nb_pistes].ROIY = 0;
		trames[nb_pistes].INFO = 1; // event forcement filtre (piste non visible)
		trames[nb_pistes].EVENT = event;
		nb_pistes++;
	}
	
	//Ecriture des trames :
	for (int i = 0; i < nb_pistes; i++)
	{
		temps_trame = MAPS::CurrentTime();

		//Envoi des trames sur la sortie, format chaine de caracteres
		MAPSIOElt *oEltTrame = StartWriting(Output("oTrame"));
		oEltTrame->VectorSize()= LNG_TRAME_OCTETS;

		vers_trames_stream8 = oEltTrame->Stream8();

		memcpy(vers_trames_stream8,&trames[i].ID,sizeof(trames[i].ID));
		vers_trames_stream8 += sizeof(trames[i].ID);
		memcpy(vers_trames_stream8,&trames[i].X,sizeof(trames[i].X));
		vers_trames_stream8 += sizeof(trames[i].X);		
		memcpy(vers_trames_stream8,&trames[i].Y,sizeof(trames[i].Y));
		vers_trames_stream8 += sizeof(trames[i].Y);
		memcpy(vers_trames_stream8,&trames[i].D,sizeof(trames[i].D));
		vers_trames_stream8 += sizeof(trames[i].D);
		memcpy(vers_trames_stream8,&trames[i].ROIX,sizeof(trames[i].ROIX));
		vers_trames_stream8 += sizeof(trames[i].ROIX);
		memcpy(vers_trames_stream8,&trames[i].ROIY,sizeof(trames[i].ROIY));
		vers_trames_stream8 += sizeof(trames[i].ROIY);
		memcpy(vers_trames_stream8,&trames[i].ROIH,sizeof(trames[i].ROIH));
		vers_trames_stream8 += sizeof(trames[i].ROIH);
		memcpy(vers_trames_stream8,&trames[i].ROIW,sizeof(trames[i].ROIW));
		vers_trames_stream8 += sizeof(trames[i].ROIW);
		memcpy(vers_trames_stream8,&trames[i].INFO,sizeof(trames[i].INFO));
		vers_trames_stream8 += sizeof(trames[i].INFO);
		memcpy(vers_trames_stream8,&trames[i].EVENT,sizeof(trames[i].EVENT));
		vers_trames_stream8 += sizeof(trames[i].EVENT);

		StopWriting(oEltTrame);

		temps_trame = MAPS::CurrentTime() - temps_trame;
		Rest(GetIntegerProperty("pTrame")- temps_trame);
	}


	StopReading(Input("iPistesCamEntree"));
	StopReading(Input("iPistesVehicule"));

	temps_core = MAPS::CurrentTime() - temps_core;
	Rest(GetIntegerProperty("pCore")-temps_core);
}

void MAPSLivicVI2CV::Death()
{
}

void MAPSLivicVI2CV::DecodeEvenement(int *ident, int *event, int *filtre)
{
	*filtre = 0;
	*event = 0;
	char msg[TAILLE_MSG+1];
	if( DataAvailableInFIFO(Input("iMessages")) )
	{
		MAPSIOElt *iEltMessages = StartReading(Input("iMessages"));
		char *data=(char*)iEltMessages->Stream8();
		memcpy( msg, data, TAILLE_MSG);
		msg[TAILLE_MSG] = 0;
		StopReading(Input("iMessages"));
        
        if( IsConnected(Input("iMessagesFiltres")) && DataAvailableInFIFO(Input("iMessagesFiltres")) )
        {
            MAPSIOElt *iEltMessagesFiltre = StartReading(Input("iMessagesFiltres"));
            *filtre = iEltMessagesFiltre->Integer();
			StopReading(Input("iMessagesFiltres"));
        }

		// id:num:ty:pa1:pa2
		char sid[2+1], snum[3+1], sty[2+1], spa1[3+1], spa2[3+1];
		int id, num, ty, pa1, pa2;
		int pos = 0;
		strncpy(sid, msg+pos, 2);
		pos += 2+1;
		strncpy(snum, msg+pos, 3);
		pos += 3+1;
		strncpy(sty, msg+pos, 2);
		pos += 2+1;
		strncpy(spa1, msg+pos, 3);
		pos += 3+1;
		strncpy(spa2, msg+pos, 3);
		id = atoi(sid);
		num = atoi(snum);
		ty = atoi(sty);
		pa1 = atoi(spa1);
		pa2 = atoi(spa2);

		*ident = id;

		switch( ty )
		{
			case MSG_TY_FREINAGE: *event = EVT_EMERGENCY_BRAKING; break;
			case MSG_TY_VIRAGE_DROITE: *event = EVT_RIGHT_CURVE; break;
			case MSG_TY_VIRAGE_GAUCHE: *event = EVT_LEFT_CURVE; break;
			case MSG_TY_ADHERENCE: *event = EVT_ICE_ON_ROAD; break;
			default: *event = EVT_GENERAL_WARNING; break;
		}
	}
}
