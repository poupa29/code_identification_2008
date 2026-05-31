/***********************************************
               RTMaps ZigBeeSend

Permet d'envoyer des messages dans une trame TinyOS

************************************************/

#include "maps_LivicZigBeeSendString.h"

MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicZigBeeSendString)
	 MAPS_INPUT("iMessages",MAPS::FilterStream8,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicZigBeeSendString)
	MAPS_OUTPUT("oStream8",MAPS::Stream8,NULL,NULL,TAILLEMAXPACKET)
MAPS_END_OUTPUTS_DEFINITION

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicZigBeeSendString)
	MAPS_PROPERTY("bit_synchro",true,false,true)
	MAPS_PROPERTY("Destination",0XFFFF,false,true)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicZigBeeSendString)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPSLivicZigBeeSendString,"LivicZigBeeSendString","1.0",128,
			  // Can be thread or sequential ; start as threaded
			  MAPS::Sequential|MAPS::Threaded,MAPS::Sequential,
			  1,1,2,0)

void MAPSLivicZigBeeSendString::Birth()
{	
}

void MAPSLivicZigBeeSendString::Core() 
{   
	unsigned int destination;
    int longueur_trame,longueur_message,i;
	unsigned char packet[TAILLEMAXPACKET];

	// Gets the intput buffer element
	 MAPSIOElt *iEltMessages = StartReading(Input("iMessages"));
    if(iEltMessages->VectorSize()>255)
		ReportError("Chaine trop longue ! 255 max");

	StopReading(Input("iMessages"));

	// Gets the output buffer element
	MAPSIOElt *oStream = StartWriting(Output("oStream8"));
	// Check the ioElt returned
	if (oStream==NULL)
		return;
    
	//Vide le tableau
	for(i=0;i<TAILLEMAXPACKET;i++)
			packet[i]=0;

	//***************** GENERE LA TRAME **********************

	packet[1]=TYPETRAME;      //Type de trame
	destination=(unsigned int)GetIntegerProperty("Destination");
	packet[2]=destination; packet[3]=destination<<8;	//Destination
	packet[4]=MSG_STRING;
	packet[5]=GROUPID;

    //Integre le tableau de char dans la trame :
	memcpy(packet + 7 ,iEltMessages->Stream8() , iEltMessages->VectorSize());    

    longueur_message = iEltMessages->VectorSize() ;
    packet[6]=longueur_message;
	longueur_trame = longueur_message + 10;
	

	//*********  Compose la trame  **********
	for(i=0;i < longueur_trame;i++)
		{if (packet[i]==0x7E) //compose 7E   
				{memcpy(packet + i + 1,packet + i,longueur_trame - i);//decale le reste de la trame
				 packet[i]=0X7D;
				 packet[i+1]=0X5E;
				 longueur_trame++;
				}	
		else if (packet[i]==0X7D) //compose 7D
				{memcpy(packet + i + 1,packet + i,longueur_trame - i);//decale le reste de la trame
				 packet[i+1]=0X5D;
				 longueur_trame++;
				}
		}

	//****************************************

	//******AJOUT DES BITS DE SYNCHRO*********
	if(GetBoolProperty("bit_synchro"))
		{packet[0]=BITSYNCHRO;
		 packet[longueur_trame - 1]=BITSYNCHRO;
		}
	else
		{longueur_trame = longueur_trame-2;
		 memcpy(packet,packet + 1,longueur_trame);
		}

	//****************************************

	// Send the outputs
	oStream->VectorSize()=longueur_trame;
	memcpy(oStream->Stream8(),packet,longueur_trame);
	StopWriting(oStream);		
}

void MAPSLivicZigBeeSendString::Death()
{ 
}

