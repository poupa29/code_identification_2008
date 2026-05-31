
//////////////////////////////////////////////////////
//Composant de synthèse de message provenant des Motes
//////////////////////////////////////////////////////

#include "maps_LivicSVMessage.h"


// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicSVMessage)
	MAPS_INPUT("iDataMote",MAPS::FilterInteger,MAPS::FifoReader)
	MAPS_INPUT("iIdMote",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicSVMessage)
    MAPS_OUTPUT("oMessage",MAPS::Stream8,NULL,NULL,TAILLE_MSG)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicSVMessage)
	MAPS_PROPERTY("pToleranceRepeat",5000,false,true) //en Ms
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicSVMessage)
    //MAPS_ACTION("aName",MAPSLivicSVMessage::ActionName)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPSLivicSVMessage,"LivicSVMessage","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  2, // Nb of inputs
			  1, // Nb of outputs
			  1, // Nb of properties
			  0) // Nb of actions

void MAPSLivicSVMessage::Birth()
{
	num=0;
	debut_tolerance = (int)MAPS::CurrentTime();
}

void MAPSLivicSVMessage::Core() 
{
	int i,id_mote,data[TAILLE_DATA_MOTE];
    char xs_message[TAILLE_MSG+1];
	tolerance = (int)GetIntegerProperty("pToleranceRepeat")*1000;

    MAPSIOElt *iEltData=StartReading(Input("iDataMote"));//Data du smartdust
	MAPSIOElt *iEltID=StartReading(Input("iIdMote"));//Id du mote
	//Recupere les valeurs
	for(i=0;i<iEltData->VectorSize();i++)
		data[i] = iEltData->Integer(i); 
	id_mote = iEltID->Integer(); 

	StopReading(Input("iDataMote"));
	StopReading(Input("iIdMote"));
	
	//Trame msg: id:num:ty:pa1:pa2

	if(id_mote==ID_FEU)//Message du feu
		sprintf(xs_message,"%02d:%03d:%02d:%03d:000",id_mote,num,data[0],data[1]);
	else if(id_mote==ID_INFOS && data[0]!=ID_INFOS)//Message d'information
		sprintf(xs_message,"%02d:%03d:%02d:%03d:%03d",id_mote,num,MSG_TY_INFO,data[0],data[1]);
	//else if(id_mote==ID_VOITURE_MEGANE || id_mote==ID_VOITURE_ZX || id_mote==ID_VOITURE_3) //Gestion V2V
	else if (typeIdentifiant(id_mote)== TYPE_ID_VEHICULE) //Gestion V2V
		sprintf(xs_message,"%02d:%03d:%02d:000:000",id_mote,num,data[0]);
	else	//Pas de message c'est un panneau
		return;


	//Pas de repetition de message
	if (!strcmp(xs_message_precedent,xs_message))
	   if (!((MAPS::CurrentTime() - debut_tolerance)> tolerance)) //sauf sur tempo de 5s ... bug V2V
			return;

	//num++; //incremente le num de msg

	debut_tolerance = (int)MAPS::CurrentTime();
	strcpy(xs_message_precedent, xs_message);

	//Ecrit le message :
	MAPSIOElt *oEltMsg = StartWriting(Output("oMessage"));
	memcpy(oEltMsg->Stream8(), xs_message, TAILLE_MSG+1);
	oEltMsg->VectorSize()=TAILLE_MSG+1;
	StopWriting(oEltMsg);
}

void MAPSLivicSVMessage::Death()
{

}
