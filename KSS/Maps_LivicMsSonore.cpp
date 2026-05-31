/******************************************************************************
 
 (c) LIVIC 2004
 
 Ce fichier est la propriete du LIVIC. Toute utilisation, copie partielle ou
 totale, modification du fichier sans autorisation du LIVIC est interdite.
 
 Gestion des messages sonore du viewer !

*******************************************************************************/

#include "MAPS_LivicMsgSonore.h"


// Use the macros to declare the inputs (here, no inputs are present)
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicMsgSonore)
	MAPS_INPUT("iMsgSonore",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs (here, no outputs are present)
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicMsgSonore)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties (here, no properties are present)
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicMsgSonore)
	MAPS_PROPERTY("pPauseSonMin",200, false,true)
	MAPS_PROPERTY("pPauseSonMax",500, false,true)
	MAPS_PROPERTY("pDistanceMinPanneau",20, false,true)
	MAPS_PROPERTY("pSeuilDebutDeclenchement",1.5,false,true)
	MAPS_PROPERTY("pSeuilFinDeclenchement",1.0,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions (here, no actions are present)
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicMsgSonore)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicTopoModem) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicMsgSonore,"LivicMsgSonore",
						  "$Revision: 1.2 $",
						  128,
						  MAPS::Threaded,
						  MAPS::Threaded,
						  1,
						  0,
						  5,
						  0)



void MAPSLivicMsgSonore::Birth()
{
	LectureEnCours.Reset();
	//CreateThread((MAPSThreadFunction)LectureSon);
	CreateThread((MAPSThreadFunction)&MAPSLivicMsgSonore::LectureSon);

}

void MAPSLivicMsgSonore::Core() 
{
		MAPSIOElt *IOEls = StartReading(Input("iMsgSonore"));
		mTempsPause.Lock ();
		TempsPause = ((int)GetIntegerProperty("pPauseSonMin")+(int)GetIntegerProperty("pPauseSonMax"))/2;
		mTempsPause.Release();	

		                                                   //filtrage sur la distance min
		if((IOEls->Integer(0) == TYPE_ID_INFRA) && (IOEls->Integer(2)<(int)GetIntegerProperty("pDistanceMinPanneau")))
		{
			switch (IOEls->Integer(1)){
				case ID_PANNEAU_DEBUT_30:
					{
						strcpy(sons,MSG_DEBUT_30);
						 break;
					 }
				case ID_PANNEAU_FIN_30:
					{
	                    strcpy(sons,MSG_FIN_30);
						break;
					}
				case ID_PANNEAU_VIRAGE_GAUCHE:
					{   
						strcpy(sons,MSG_VIRAGE_GAUCHE);
						break;
					}
				case ID_PANNEAU_CHICANE:
					{
	                    strcpy(sons,MSG_CHICANE);
						break;
					}
				case ID_STOP:
					{
	                    strcpy(sons,MSG_STOP);
						break;
					}
				case ID_DANGER:
					{
	                    strcpy(sons,MSG_DANGER);
						break;
					}
				case ID_FEU:
					{
	                    strcpy(sons,MSG_FEU);
						break;
					}
				case ID_PANNEAU_VITESSE_90:
					{
	                    strcpy(sons,MSG_VITESSE_90);
						break;
					}
				case ID_PANNEAU_VITESSE_50:
					{
	                    strcpy(sons,MSG_VITESSE_50);
						break;
					}
				case ID_PANNEAU_FIN_VITESSE_50:
					{
	                    strcpy(sons,MSG_FIN_VITESSE_50);
						break;
					}
				default:
					{strcpy(sons,WARNING);
					 break;
					}

			}
			
		}
		//----------------------------------------------------------------------------------------------------
		else if(IOEls->Integer(0) == TYPE_ID_VEHICULE)
		{
			switch (IOEls->Integer(1)){
				case MSG_TY_FREINAGE:
					{
						strcpy(sons,ALERTE_FREINAGE);
						 break;
					 }
				case MSG_TY_VIRAGE_DROITE:
					{
						strcpy(sons,ALERTE_VIRAGE_DROITE);
						 break;
					 }
				case MSG_TY_VIRAGE_GAUCHE: //a Ameliorer
					{
						strcpy(sons,ALERTE_VIRAGE_GAUCHE);
						 break;
					 }
				case MSG_TY_ADHERENCE:
					{
						strcpy(sons,ALERTE_ADHERENCE);
						 break;
					 }
				case MSG_TY_OBSTACLE:
					{
						strcpy(sons,ALERTE_OBSTACLE);
						 break;
					 }
				case MSG_TY_FEU_VERT:
					{
						strcpy(sons,ALERTE_FEU_VERT);
						 break;
					 }
				default:
					{strcpy(sons,WARNING);
					 break;
					}
			}//Fin switch
		}
		
		if(strcmp(sonsprecedant,sons))
				{mTempsPause.Lock ();
				 TempsPause = ((int)GetIntegerProperty("pPauseSonMin")+(int)GetIntegerProperty("pPauseSonMax"))/2;
				 mTempsPause.Release();	
				 LectureEnCours.Set();
				}
		else
			LectureEnCours.Reset();
		
		strcpy(sonsprecedant,sons);

		StopReading(Input("iMsgSonore")); 
}



void MAPSLivicMsgSonore::Death()
{
}

void MAPSLivicMsgSonore::LectureSon()
{
	int tempsPause;
	while(!IsDying())
	{
		Wait4Event(&LectureEnCours,MAPS::Infinite);
		PlaySound(sons,NULL,SND_FILENAME | SND_ASYNC);
		mTempsPause.Lock ();
		tempsPause = TempsPause;
		mTempsPause.Release();
		MAPS::Sleep(tempsPause*1000);
	}
}

void MAPSLivicMsgSonore::Set(MAPSProperty &p, MAPSInt64 value) 
{
	if (&p==&Property("pPauseSonMin"))
	{
		if ( value >= GetIntegerProperty("pPauseSonMax") || value <= 30)
			value = GetIntegerProperty("pPauseSonMax");
		if(value <= 30)
			value = 30;

		MAPSComponent::Set(p,value);
    }
	else if (&p==&Property("pPauseSonMax"))
	{
		if ( value <= GetIntegerProperty("pPauseSonMin") || value <= 30)
			value = GetIntegerProperty("pPauseSonMin");
		if(value <= 30)
			value = 30;

		MAPSComponent::Set(p,value);
    }
}

void MAPSLivicMsgSonore::Set(MAPSProperty &p, MAPSFloat value) 
{
    if (&p==&Property("pSeuilDebutDeclenchement"))
	{
		if(value <= GetFloatProperty("pSeuilFinDeclenchement"))
			value = GetFloatProperty("pSeuilFinDeclenchement") + 0.1;
		if(value <= 0.0)
			value = 1.5;

		MAPSComponent::Set(p,value);
	}
	else if (&p==&Property("pSeuilFinDeclenchement"))
	{
		if(value >= GetFloatProperty("pSeuilDebutDeclenchement"))
			value = GetFloatProperty("pSeuilDebutDeclenchement")-0.1;
		if(value <= 0.0)
			value = 0.0;

		MAPSComponent::Set(p,value);
	}
}