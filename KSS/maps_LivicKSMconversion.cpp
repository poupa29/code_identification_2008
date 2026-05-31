/////////////////////////////////
//Composant de convertion du KSM
/////////////////////////////////


#include "maps_LivicKSMConversion.h"

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPS_LivicKSMConversion)
    MAPS_INPUT("iSV",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPS_LivicKSMConversion)
    MAPS_OUTPUT("oDataMote",MAPS::Integer,NULL,NULL,2)
	MAPS_OUTPUT("oIdMote",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPS_LivicKSMConversion)
	MAPS_PROPERTY("pIntervalleFeu",100,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPS_LivicKSMConversion)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component behaviour
MAPS_COMPONENT_DEFINITION(MAPS_LivicKSMConversion,"LivicKSMConversion","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  1, // Nb of inputs
			  2, // Nb of outputs
			  1, // Nb of properties
			  0) // Nb of actions

void MAPS_LivicKSMConversion::Birth()
{
	//feu
	xi_TimeLeft = 0;
    xi_ColourCode = 0;
    xi_ColourCode_precedent = -1;
    xi_TimeLeft_precedent = -1;
}

void MAPS_LivicKSMConversion::Core() 
{
	int sv_id;
	MAPSIOElt *iEltSV=NULL;

		iEltSV = StartReading(Input("iSV"));//Entrée du Ksm
		sv_id_brut = iEltSV->Integer();
		sv_id = sv_id_brut % 100000; //Récupère ID du smartdust
		StopReading(Input("iSV"));

	MAPSIOElt* oid_mote = StartWriting(Output("oIdMote"));//Initialise l'écriture sur la sortie du composant
	MAPSIOElt* odata = StartWriting(Output("oDataMote"));//Initialise l'écriture sur la sortie du composant

	
	//*****************************************************************************

	if (sv_id > 256) //1000 //c'est le Feu !!!
	{
		xi_TimeLeft = sv_id / 1000;
		xi_ColourCode = (sv_id % (int)GetIntegerProperty("pIntervalleFeu")) + MSG_TY_FEU_ROUGE - 1;
		odata->Integer(0) = xi_ColourCode;
		odata->Integer(1) = xi_TimeLeft;
		odata->VectorSize()=2;
		sv_id = 20;
	}
	else // ID Panneau
	{	odata->Integer(0) = sv_id;
		odata->VectorSize()= 1;
	}
	
	oid_mote->Integer() = sv_id;
	StopWriting(oid_mote);
	StopWriting(odata);

}

void MAPS_LivicKSMConversion::Death()
{
}
