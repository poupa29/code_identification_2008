////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

//////////////////////////////////////////////
//Composant de lecture en mémoire partagée
//////////////////////////////////////////////

#include "maps_LivicMemPartageeRead.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicMemPartageeRead)
//MAPS_INPUT("iName",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicMemPartageeRead)
	MAPS_OUTPUT("sortie",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicMemPartageeRead)
	MAPS_PROPERTY("pSignetMemoire","share",false,false)
	MAPS_PROPERTY("pEchantillonnage",50,false,true)
	MAPS_PROPERTY_ENUM("pMode","Sampling|Event",0,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicMemPartageeRead)
//MAPS_ACTION("aName",MAPSMemPartageeRead::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicMemPartageeRead) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicMemPartageeRead,"LivicMemPartageeRead","1.0",128,
						  MAPS::Threaded,MAPS::Threaded,
						  0, // Nb of inputs
						  1, // Nb of outputs
						  3, // Nb of properties
						  0) // Nb of actions

void MAPSLivicMemPartageeRead::Birth()
{

	MessageBox(NULL,"Avez-vous correctement initialisé la mémoire ?",NULL,MB_OK);

	// Reports this information to the RTMaps console
	Rest(10000);


	hMap= OpenFileMapping(	FILE_MAP_READ | FILE_MAP_WRITE,
		TRUE,
		GetStringProperty("pSignetMemoire"));

	if (hMap == NULL) 
	{
		printf("Impossible de partager la memoire, erreur no : %d\n",GetLastError());
	}


	LPVOID lpMapAddress;
	lpMapAddress = MapViewOfFile(hMap, // Handle to mapping object. 
		FILE_MAP_ALL_ACCESS,               // Read/write permission. 
		0,                                 // Max. object size. 
		0,                                 // Size of hFile. 
		0);                                // Map entire file. 

	if (lpMapAddress == NULL) 
	{ 
		printf("Impossible de partager la memoire, erreur no : %d\n",GetLastError());

	}

	pi_b = (int *) lpMapAddress;
	valeur_precedente = 0;

	printf("Memoire Partage Stereo: OK\n");   

}






void MAPSLivicMemPartageeRead::Core() 
{

	Rest(GetIntegerProperty("pEchantillonnage")*1000);

	int valeur_courante = (int)*pi_b;
	

	if (!strcmp(GetStringProperty("pMode"),"Event"))
	{
		if (valeur_precedente != valeur_courante)
		{
			MAPSIOElt* oEltPistes = StartWriting(Output("sortie"));//Initialise l'écriture sur la sortie du composant			
			oEltPistes->Integer() = valeur_courante;//Ecrit sur la sortie du composant
			StopWriting(oEltPistes);//Fin de l'écriture

			valeur_precedente = valeur_courante;
		}
	}
	else
	{
		MAPSIOElt* oEltPistes = StartWriting(Output("sortie"));//Initialise l'écriture sur la sortie du composant			
		oEltPistes->Integer() = valeur_courante;//Ecrit sur la sortie du composant
		StopWriting(oEltPistes);//Fin de l'écriture
	}
}

void MAPSLivicMemPartageeRead::Death()
{
}
