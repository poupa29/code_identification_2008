//permet de d'envoyer le code message en fonction de
//la touche pressée

#include "maps_LivicV2VClavier.h"	

MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicV2VClavier)
    MAPS_INPUT("input",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicV2VClavier)
    MAPS_OUTPUT("output",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicV2VClavier)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicV2VClavier)

MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPSLivicV2VClavier,"LivicV2VClavier","1.0",128,
			MAPS::Threaded|MAPS::Sequential,MAPS::Threaded,
			  1, // Nb of inputs
			  1, // Nb of outputs
			  0, // Nb of properties
			  0) // Nb of actions

void MAPSLivicV2VClavier::Birth()
{
}

void MAPSLivicV2VClavier::Core() 
{
	int valeurtouche,code;

	MAPSIOElt* ioEltIn = StartReading(Input("input"));
	if (ioEltIn == NULL)
		return;
	valeurtouche = ioEltIn->Integer(0);
	StopReading(Input("input"));

	if(valeurtouche==70 )
		{code = MSG_TY_FREINAGE;
		 ReportInfo("Freinage !");
		}
	else if(valeurtouche==68 )
		{code = MSG_TY_VIRAGE_DROITE;
		 ReportInfo("Virage");
		}
	else if(valeurtouche==71 )
		{code = MSG_TY_VIRAGE_GAUCHE;
		 ReportInfo("Virage");
		}
	else if(valeurtouche==65 )
		{code = MSG_TY_ADHERENCE;
		 ReportInfo("Adherance");
		}
	else if(valeurtouche==79 )
		{code = MSG_TY_OBSTACLE;
		 ReportInfo("Obstacle !");
		}
	else if(valeurtouche==76 )
		{code = MSG_TY_LOVE;
		 ReportInfo("Love ...");
		}
	else //par defaut:
			return;

	

	MAPSIOElt* ioEltOut = StartWriting(Output("output"));
	ioEltOut->Integer()= code;
	//Specify the number of valid elements in the output vector (less or equal than 32).
	ioEltOut->VectorSize() =1;

	//Writing in the ioEltOut object is complete.
	StopWriting(ioEltOut);
}

void MAPSLivicV2VClavier::Death()
{
}
