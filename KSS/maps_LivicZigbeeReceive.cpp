/***********************************************
               RTMaps ZigBeeReceive

Note: nouvelle version avec la synchronisation directement sur le composant port série.

Permet de decoder les trames TinyOS

Output:
Integer[5] --> la valeur de l'entier ou du tableau d'entiers
Id_mote    --> L'ID du mote
Infos      --> Dans l'ordre :   -l'adresse de destination 
								-Type de message
								-Group ID
								-longueur du msg  
Input:
La sortie du port com Serie du mote Micaz a CONFIGURER !!!

Baudrate: 57600
Parity: None
Stop bits: 1
Byte size: 8 
Flow control : Xon/Xoff
Xon char : 126
Xoff char : 126
Minimum : 50
Timeout: 20000  

*********TYPE DE MESSAGE***********
(Voir IR.h)
AM_INTMSG		4
S_MSG			5
MSG_CONTROL		6
MSG_LOG			7
MSG_VEHICULE	8
************************************************/

#include "maps_LivicZigBeeReceive.h"

MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicZigbeeReceive)
	//Entrée du port série
    MAPS_INPUT("iStream8",MAPS::FilterStream8,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicZigbeeReceive)
    MAPS_OUTPUT("oDataMote",MAPS::Integer,NULL,NULL,TAILLE_DATA_MOTE)
	MAPS_OUTPUT("oIdMote",MAPS::Integer,NULL,NULL,1)
	MAPS_OUTPUT("Infos",MAPS::Integer,NULL,NULL,4)

MAPS_END_OUTPUTS_DEFINITION

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicZigbeeReceive)
	MAPS_PROPERTY("Check_CRC",true,false,true)
	MAPS_PROPERTY("Compatible_Feu",true,false,true)
	MAPS_PROPERTY("pIntervalleFeu",100,false,true)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicZigbeeReceive)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPSLivicZigbeeReceive,"LivicZigbeeReceive","1.0",128,
			  // Can be thread or sequential ; start as threaded
			  MAPS::Sequential|MAPS::Threaded,MAPS::Sequential,
			  1,3,3,0)

void MAPSLivicZigbeeReceive::Birth()
{	
}

void MAPSLivicZigbeeReceive::Core() 
{
	unsigned char *receivedData;
	int i,z,src,Dscr,lsrc,length,start;
	unsigned int val,id_mote,msg_type;

	// Wait for new data from the sensor
	MAPSIOElt *ioEltInput=StartReading(Input("iStream8"));
	//il faut au moins 8 chars dans le vecteur :
	if (ioEltInput==NULL  || ioEltInput->VectorSize()<8)
		{ReportInfo("size error");return;}
	//Données recu :
	receivedData=ioEltInput->Stream8();
	StopReading(Input("iStream8"));
   
	//*********************Recompose le(s) trame(s) ******************
	for(i=0;i < ioEltInput->VectorSize();i++)
		{if ((receivedData[i]==0X7D) && (receivedData[i+1]==0X5E)) //recompose 7E
				{receivedData[i]=0x7E;
				 for(z=i+1;z < ioEltInput->VectorSize()-1;z++)//decale le reste de la trame
					receivedData[z]=receivedData[z+1];
				 ioEltInput->VectorSize()--; //La trame diminue
				}	
		 else if ((receivedData[i]==0X7D) && (receivedData[i+1]==0X5D)) //recompose 7D
				{for(z=i+1;z < ioEltInput->VectorSize()-1;z++)//decale le reste de la trame
					receivedData[z]=receivedData[z+1];
				 ioEltInput->VectorSize()--; //La trame diminue
				}
		}
	//**********************************************************

	//Boucle de traitement de toutes les trames ...
	for(start=0 ; start < (ioEltInput->VectorSize() - 1) ; start = start)
	{
		 //Gestion des type de msg :
		if (receivedData[0 + start] != 0x42) //On ne gere pas les autres types de packet pour l'instant
				{ReportInfo("Trame_type error");return;}	

		//***********************************************************
		//**********************CRC verification ********************
		if(GetBoolProperty("Check_CRC"))
		{
			Dscr = receivedData[5 + start] + 6 + start; //debut du code scr
			src = receivedData[Dscr] | (receivedData[Dscr+1]<<8); //Check Sum de la trame

			lsrc = 6 + receivedData[5 + start]; //longueur de la trame a verifier
			if( calc(receivedData , start , lsrc) != src) //verification ...
				{ReportInfo("Crc error");return;}
		}
		//***********************************************************

			// Gets the output buffer element
			MAPSIOElt *oData=StartWriting(Output("oDataMote"));
			MAPSIOElt *Infos=StartWriting(Output("Infos"));
			MAPSIOElt *Id_mote=StartWriting(Output("oIdMote"));
			// Check the ioElt returned
			if (oData==NULL || Infos==NULL || Id_mote==NULL)
				{ReportInfo("ioelt error");return;}
			// Set the vector size :
			Infos->VectorSize()=4;
			Id_mote->VectorSize()=1;

		//***************** Decripte la trame **********************
			//INFOS EN-TETE :
			//l'adresse de destination 
			Infos->Integer(0) = receivedData[1 + start] | (receivedData[2 + start]<<8);
			//Type de message :
			msg_type = receivedData[3 + start];
			Infos->Integer(1) = msg_type;
			//Group ID
			Infos->Integer(2) = receivedData[4 + start];
			//Taile du TOSmsg
			length = receivedData[5 + start];
			Infos->Integer(3) = length;

			//Gestion du type de messages ...
			switch(msg_type)
			{
				case S_MSG :  
				case AM_INTMSG :
					{  oData->VectorSize()=(length/2) - 1;
						//Recomposition des Ints du tableau :
						for(i=0;i<(length-2);i=i+2)
							{val = receivedData[i+6 + start] | (receivedData[i+7 + start]<<8);
							 oData->Integer(i/2) = val;
							}
						//Id du mote :
						id_mote = receivedData[i+6 + start] | (receivedData[i+7 + start]<<8);
						Id_mote->Integer() = id_mote;
					  break;
					}
				case MSG_LOG :
					{  oData->VectorSize() = 6;
					   oData->Integer(0) = receivedData[6 + start] | (receivedData[7 + start]<<8); //id vehicule
					   oData->Integer(1) = receivedData[8 + start]; //adverage speed
					   oData->Integer(2) = receivedData[9 + start]; //type vehicule
					   oData->Integer(3) = receivedData[10 + start]; //min
					   oData->Integer(4) = receivedData[11 + start]; //hour
					   oData->Integer(5) = receivedData[12 + start]; //day
					   id_mote = receivedData[13 + start] | (receivedData[14 + start]<<8); //id source
					   Id_mote->Integer() = id_mote;
					  break;
					}
				case MSG_VEHICULE :
					{  oData->VectorSize() = 2;
					   id_mote = receivedData[6 + start] | (receivedData[7 + start]<<8); //id vehicule
					   oData->Integer(0) = receivedData[8 + start]; //average speed
					   oData->Integer(1) = receivedData[9 + start]; //type vehicule
					   Id_mote->Integer() = id_mote;
					  break;
					}

				default:
					{oData->Integer() = 0;
					 id_mote=0;
					 Id_mote->Integer() = id_mote;
					 break;
					}
			}
			

		//***************** Fin Decrypte la trame *******************

		//**********Le cas du Feu tricolore ***********************
		if ((Id_mote->Integer() == ID_FEU) && (GetBoolProperty("Compatible_Feu")))
		{	oData->VectorSize() = 2;
			oData->Integer(0) = (val % (int)GetIntegerProperty("pIntervalleFeu")) + MSG_TY_FEU_ROUGE - 1; //xi_ColourCode 
			oData->Integer(1) = val / 1000; //xi_TimeLeft
		}
		//*********************************************************
	
		//gestion des trames suivantes : Bug 30/06/08
		start = start + receivedData[5 + start] + 8; //start = longueur de la trame

		// Send the outputs
		StopWriting(oData);
		StopWriting(Id_mote);
		StopWriting(Infos);

	}//Fin du For
	
		
}

void MAPSLivicZigbeeReceive::Death()
{
}

 int MAPSLivicZigbeeReceive::calc(unsigned char packet[], int index, int count)
{
	int crc = 0;
	while (count > 0)
		{
		crc = calcByte(crc, packet[index++]);
		count--;
		}
	return crc;
}

 int MAPSLivicZigbeeReceive::calcByte(int crc, int b)
{
	crc = crc ^ (int)b << 8;
	for (int i = 0; i < 8; i++)
		{
			if ((crc & 0x8000) == 0x8000)
			crc = crc << 1 ^ 0x1021;
			else
			crc = crc << 1;
		}
	return crc & 0xffff;
}

