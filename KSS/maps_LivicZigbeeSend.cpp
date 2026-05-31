/***********************************************
               RTMaps ZigBeeSend

Permet d'envoyer des messages dans une trame TinyOS

*********TYPE DE MESSAGE***********
(Voir IR.h)
AM_INTMSG		4
S_MSG			5
MSG_CONTROL		6
MSG_LOG			7
MSG_VEHICULE	8
************************************************/

#include "maps_LivicZigBeeSend.h"

MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicZigBeeSend)
	 MAPS_INPUT("iInteger",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicZigBeeSend)
	MAPS_OUTPUT("oStream8",MAPS::Stream8,NULL,NULL,TAILLEMAXPACKET)
MAPS_END_OUTPUTS_DEFINITION

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicZigBeeSend)
	MAPS_PROPERTY("Id_mote",10,false,true)
	MAPS_PROPERTY("type_message",4,false,true)
	MAPS_PROPERTY("bit_synchro",true,false,true)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicZigBeeSend)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPSLivicZigBeeSend,"LivicZigBeeSend","1.0",128,
			  // Can be thread or sequential ; start as threaded
			  MAPS::Sequential|MAPS::Threaded,MAPS::Sequential,
			  1,1,3,0)

void MAPSLivicZigBeeSend::Birth()
{	
}

void MAPSLivicZigBeeSend::Core() 
{
    int valeur,id_mote;
	int i;
	char message_type;
	int longueur_trame,longueur_message;
	unsigned char packet[TAILLEMAXPACKET];

	// Gets the intput buffer element
    MAPSIOElt* ioEltIn = StartReading(Input("iInteger"));
	if (ioEltIn == NULL)
		return;
	valeur = ioEltIn->Integer(0);
	StopReading(Input("iInteger"));

	// Gets the output buffer element
	MAPSIOElt *oStream = StartWriting(Output("oStream8"));
	// Check the ioElt returned
	if (oStream==NULL)
		return;
    
	//Vide le tableau
	for(i=0;i<TAILLEMAXPACKET;i++)
			packet[i]=0;

	// 0  1  2  3  4  5  6  7  8  9 10 11 12 13
    //7E 42 FF FF 04 7D 04 C9 00 06 00 88 1D 7E
	//***************** GENERE LA TRAME **********************
	message_type=(char)GetIntegerProperty("type_message");
	id_mote=(int)GetIntegerProperty("Id_mote");

	packet[1]=TYPETRAME;      //Type de trame
	packet[2]=0XFF;packet[3]=0XFF;	//Destination
	packet[4]=message_type;
	packet[5]=GROUPID;

	switch(message_type)
		{		case MSG_CONTROL:
				case AM_INTMSG :
					{  //Integre l'ID dans la trame	
						packet[9]=id_mote; 
						packet[10]=id_mote<<8;
						//Integre le Int dans la trame
						packet[7]=valeur; 
						packet[8]=valeur<<8; 

						longueur_message=4;
						break;
					}
				case MSG_VEHICULE :
					{  
						//Integre l'ID dans la trame	
						packet[8]=id_mote; 
						packet[9]=id_mote<<8;
						//Integre le Int dans la trame
						packet[7]=valeur; //AdverageSpeed 
						longueur_message=3;
						break;
					}

				case S_MSG :
					{  	
						//Integre le tableau de Int dans la trame :
						for(i=0; i < (ioEltIn->VectorSize())*2 ;i=i+2)
							{   packet[i+7]=ioEltIn->Integer(i/2);
								packet[i+8]=ioEltIn->Integer(i/2)<<8;
							}
						//Integre l'ID dans la trame	
						packet[i+7]=id_mote; 
						packet[i+8]=id_mote<<8;

						longueur_message = ioEltIn->VectorSize()*2 + 2 ;
						break;
					}

				default:
					{ReportInfo("Erreur message_type inconnu");
					 return;
					}

		}
	longueur_trame = longueur_message + 10;
	packet[6]=longueur_message;


	//*********  Compose la trame  **********
	for(i=0;i < longueur_trame ;i++)
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

void MAPSLivicZigBeeSend::Death()
{ 
}

