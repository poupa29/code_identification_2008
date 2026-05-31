////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

//////////////////////////////////////////////
//Composant d'écriture en mémoire partagée
//////////////////////////////////////////////

#include "maps_LivicMemPartageeWrite.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPS_LivicMemPartageeWrite)
    MAPS_INPUT("entree",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPS_LivicMemPartageeWrite)
    //MAPS_OUTPUT("oName",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPS_LivicMemPartageeWrite)
	MAPS_PROPERTY("pSignetMemoire","share",false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPS_LivicMemPartageeWrite)
    //MAPS_ACTION("aName",MAPSmaps_MemPartageeWrite::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component behaviour
MAPS_COMPONENT_DEFINITION(MAPS_LivicMemPartageeWrite,"LivicMemPartageeWrite","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  1, // Nb of inputs
			  0, // Nb of outputs
			  1, // Nb of properties
			  0) // Nb of actions

void MAPS_LivicMemPartageeWrite::Birth()
{
    // Initialisation de la memoire partage 
	hMap = CreateFileMapping(	(HANDLE)0xFFFFFFFF,
								NULL ,
								PAGE_READWRITE 
								,0
								,4096
								,GetStringProperty("pSignetMemoire") );
	if (hMap == NULL) 
	{
		printf("Impossible de partager la memoire, erreur no : %d\n",GetLastError());
		
	}
	

	LPVOID lpMapAddress;
	lpMapAddress = MapViewOfFile(hMap, // Handle to mapping object. 
		FILE_MAP_ALL_ACCESS,               // Read/write permission 
		0,                                 // Max. object size. 
		0,                                 // Size of hFile. 
		0);                                // Map entire file. 
 
	if (lpMapAddress == NULL) 
	{ 
		printf("Impossible de partager la memoire, erreur no : %d\n",GetLastError());		
	} 

pi_a =(int *) lpMapAddress;

//*pi_a = 0;	

	printf("Memoire Partage Stereo: OK\n");
}

void MAPS_LivicMemPartageeWrite::Core() 
{
	if((iEltPistes=StartReading(Input("entree")))==NULL)
				ReportInfo("Erreur Input");
	*pi_a = iEltPistes->Integer();
	
	StopReading(Input("entree"));
}

void MAPS_LivicMemPartageeWrite::Death()
{
}
