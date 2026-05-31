/***********************************************
               RTMaps ZigBeeReceiveString


Permet de decoder les trames String Zigbee

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

************************************************/

#include "maps_LivicZigBeeReceiveString.h"

MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicZigbeeReceiveString)
    MAPS_INPUT("iStream8",MAPS::FilterStream8,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicZigbeeReceiveString)
    MAPS_OUTPUT("oMessage",MAPS::Stream8,NULL,NULL,TAILLE_MSG_MAX)
	MAPS_OUTPUT("Infos",MAPS::Integer,NULL,NULL,4)

MAPS_END_OUTPUTS_DEFINITION

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicZigbeeReceiveString)
	MAPS_PROPERTY("Check_CRC",true,false,true)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicZigbeeReceiveString)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(MAPSLivicZigbeeReceiveString,"LivicZigbeeReceiveString","1.0",128,
			  // Can be thread or sequential ; start as threaded
			  MAPS::Sequential|MAPS::Threaded,MAPS::Sequential,
			  1,2,1,0)

void MAPSLivicZigbeeReceiveString::Birth()
{	
}

void MAPSLivicZigbeeReceiveString::Core() 
{
	unsigned char *receivedData;
	int i,z,src,Dscr,lsrc,length,start;
	unsigned int msg_type;

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
			MAPSIOElt *oData=StartWriting(Output("oMessage"));
			MAPSIOElt *Infos=StartWriting(Output("Infos"));
			// Check the ioElt returned
			if (oData==NULL || Infos==NULL)
				{ReportInfo("ioelt error");return;}
			// Set the vector size :
			Infos->VectorSize()=4;

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

			//Gestion du message ...
			 oData->VectorSize()=length;
			 
            //Recomposition des char du tableau :
		    if(msg_type==MSG_STRING)
				memcpy(oData->Stream8(), receivedData + 6 + start,length );
			else
				ReportError("ce n'est pas un message string !!!");

		//***************** Fin Decrypte la trame *******************

		//gestion des trames suivantes : Bug 30/06/08
		start = start + receivedData[5 + start] + 8; //start = longueur de la trame

		// Send the outputs
		StopWriting(oData);
		StopWriting(Infos);

	}//Fin du For
	
		
}

void MAPSLivicZigbeeReceiveString::Death()
{
}

 int MAPSLivicZigbeeReceiveString::calc(unsigned char packet[], int index, int count)
{
	int crc = 0;
	while (count > 0)
		{
		crc = calcByte(crc, packet[index++]);
		count--;
		}
	return crc;
}

 int MAPSLivicZigbeeReceiveString::calcByte(int crc, int b)
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

