////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////
// This is the simplest rt-maps component : it has no inputs, outputs, properties 
// nor actions (as you can see below). The CORE of the module just prints some
// information to the console and makes a pause (it sleeps during 500 ms)

#include "MAPS_LivicIRViewer.h"   // Includes the header of this component
//#include "../commun/tarel/image.h"
//#include "../commun/tarel/tools.h"

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicIRViewer)
    MAPS_INPUT("iImage",MAPS::FilterIplImage,MAPS::SamplingReader)
    MAPS_INPUT("iPistesIRStereo",MAPS::FilterRealObjects,MAPS::SamplingReader)
    MAPS_INPUT("iMessages",MAPS::FilterStream8,MAPS::FifoReader)
    MAPS_INPUT("iMessagesFiltres",MAPS::FilterInteger,MAPS::FifoReader)
    MAPS_INPUT("iPistesIRVehicule",MAPS::FilterRealObjects,MAPS::SamplingReader)
//    MAPS_INPUT("iParametresCameras",MAPSFilterStructureParametresCameras, MAPS::SamplingReader)
	MAPS_INPUT("iTangageVehicule", MAPS::FilterFloat, MAPS::SamplingReader)
	MAPS_INPUT("iZoneFiltrageMessages",MAPS::FilterFloats, MAPS::SamplingReader)
    MAPS_INPUT("iVoiesCirculation",MAPS::FilterIntegers, MAPS::SamplingReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicIRViewer)
    MAPS_OUTPUT("oImageSortie",MAPS::IplImage,NULL,NULL,1)
    MAPS_OUTPUT("oIHMConducteur",MAPS::IplImage,NULL,NULL,1)
	MAPS_OUTPUT("oMsgSonore",MAPS::Integer,NULL,NULL,3)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicIRViewer)
    MAPS_PROPERTY("pPeriode", 100, false, true)
    MAPS_PROPERTY("pTempsAffichageMessages", 3000, false, true)
    MAPS_PROPERTY("pTempsAffichagePanneaux", 500, false, true)
    MAPS_PROPERTY("pAffichageIHMConducteur", false, false, false)
    MAPS_PROPERTY("pAffichagePanneaux", true, false, true)
    MAPS_PROPERTY("pAffichageTimestamp", false, false, true)
    MAPS_PROPERTY("pAffichageDistances", false, false, true)
    MAPS_PROPERTY("pAffichageVitesses", false, false, true)
    MAPS_PROPERTY_ENUM("pTypeImage","N&B|Couleur", 0, false,false)
    MAPS_PROPERTY("pAffichageZonesFiltrage", false,false,true)
    MAPS_PROPERTY("pAffichageVoiesCirculation",false, false, true)
    MAPS_PROPERTY("pZoomPanneaux",false, false, true)
    MAPS_PROPERTY("pZoomMin",8, false, true)
    MAPS_PROPERTY("pZoomMax",25, false, true)
    MAPS_PROPERTY("pConfirmationPanneauxParSV",false, false, true)
    MAPS_PROPERTY("pPrecision",10, false, true)
	MAPS_PROPERTY("pDistanceMax",60, false, true)

    MAPS_PROPERTY_READ_ONLY("----- Parametres Camera : -----------",":")
    MAPS_PROPERTY("pDecalageCameraX", 0.0, false, true)
    MAPS_PROPERTY("pDecalageCameraY", -1.3, false, true)
    MAPS_PROPERTY("pDecalageCameraZ", 0.0, false, true)
    MAPS_PROPERTY("pTangageCamera", 2.0, false, true)
    MAPS_PROPERTY("pUo", 160.0, false, true)
    MAPS_PROPERTY("pVo", 60.0, false, true)
    MAPS_PROPERTY("pAlphaU", 810.0, false, true)
    MAPS_PROPERTY("pAlphaV", 405.0, false, true)

    /*MAPS_PROPERTY("pDemieTailleFenetreU", 2, false, true)
    MAPS_PROPERTY("pDemieTailleFenetreV", 1, false, true)
    MAPS_PROPERTY("pLargeurVoiture_m", 3.0, false, true)
    MAPS_PROPERTY("pHauteurVoiture_m", 1.8, false, true)
    MAPS_PROPERTY("pProfondeurVoiture_m", 3.0, false, true)
    MAPS_PROPERTY("pDistanceMinAssociationPistes_m", 1.0, false, true)
    MAPS_PROPERTY("pSeuilScoreCorrelationObstacle", 20.0, false, true)*/
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicIRViewer)
    //MAPS_ACTION("aName",MAPSLivicIRViewer::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicIRViewer) behaviour
MAPS_COMPONENT_DEFINITION(
    MAPSLivicIRViewer,
    "LivicIRViewer",
    "$Revision: 1.34 $",
    128,
    MAPS::Threaded,
    MAPS::Threaded,
    8,
    3,
   26,
    0)



//  **********************************************************************************************
//      ---- METHODE SynchroStartReading(int nb, MAPSInput **inputs, MAPSIOElt **ioElts) ----
//  **********************************************************************************************
//MAPSTimestamp MAPSLivicIRViewer::QuasiSynchroStartReading(int nb, MAPSInput **inputs, MAPSIOElt **ioElts, MAPSTimestamp *iTimestamp, MAPSTimestamp toleranceTimestamp)
//{
//    //Lecture des donnees
//    do
//    {
//        ioElts[0]=StartReading(*inputs[0]);
//    }while(DataAvailableInFIFO(*inputs[0]));
//    iTimestamp[0] = ioElts[0]->Timestamp();
//	
//    for(int i=1 ; i < nb ; i++)
//    {
//        if (IsConnected(*inputs[i]))
//        {
//            do
//            {
//                ioElts[i]=StartReading(*inputs[i]);
//            }while(DataAvailableInFIFO(*inputs[i]));
//            iTimestamp[i] = ioElts[i]->Timestamp();
//        }
//        else
//            iTimestamp[i] = iTimestamp[0];
//    }
//    //On recherche si les donnees sont synchrones.
//    bool sTempsEgaux = true;
//    for(i=1 ; i < nb ; i++)
//    {
//        if( abs(long(iTimestamp[i] - iTimestamp[0]))>toleranceTimestamp/2 )
//
//        {
//            sTempsEgaux = false;
//            break;
//        }
//    }
//	
//
//    return iTimestamp[0];
//}


void MAPSLivicIRViewer::Birth()
{
    xb_firstTime = true;
    pu_ImageVirageDroite    = NULL;
    pu_ImageVirageGauche    = NULL;
    pu_ImageDebutZone30     = NULL;
    pu_ImageFinZone30       = NULL;
    pu_ImageChicane         = NULL;
    pu_ImagePanneauFeu      = NULL;
    pu_ImageFeu             = NULL;
    pu_ImageFeuRouge        = NULL;
    pu_ImageFeuOrange       = NULL;
    pu_ImageFeuVert         = NULL;
    pu_ImageDanger          = NULL;
    pu_ImageMessage         = NULL;
    pu_ImageIHMConducteur   = NULL;
    pu_ImageStop            = NULL;
    pu_ImageVitesse90       = NULL;
    pu_ImageVitesse50       = NULL;
    pu_ImageVitesse50       = NULL;
	//Meteo
	pu_ImageMeteoNA			 = NULL;
	pu_ImageMeteojour		 = NULL;
	pu_ImageMeteonuit		 = NULL;
	pu_ImageMeteochaud		 = NULL;
	pu_ImageMeteopluiejour	 = NULL;
	pu_ImageMeteopluienuit	 = NULL;
	pu_ImageMeteoverglas	 = NULL;




    pe_TypeImage = GetEnumProperty("pTypeImage");

    xb_firstTime = true;
    x1 = (int*) malloc(nbVoies * sizeof(int));
    x2 = (int*) malloc(nbVoies * sizeof(int));
    y1 = (int*) malloc(nbVoies * sizeof(int));
    y2 = (int*) malloc(nbVoies * sizeof(int));

    /*xb_firstTimeAffichageZoneFiltrage = true;*/

    //if(GetBoolProperty("pAffichageZonesFiltrage"))
    //{
    //    //po_stereoIR             = new LivicStereoDetecteur();
    //    xb_firstTime            = true;
    //}

    xo_messages.vide();
    xo_messagesTimestamp.vide();
    xo_messagesID.vide();
    xo_messagesTY.vide();
    xo_messagesPA1.vide();
	xo_messagesPA2.vide();
    xo_messageATraiter.vide();

	pu_lastPanneauImage = NULL;
	xt_lastPanneauTime = 0;
}

MAPSTimestamp MAPSLivicIRViewer::mtsp_ImageSuivant()
{
    MAPSIOElt *iEltImage;
    iEltImage = StartReading(Input("iImage"));
    IplImage &iImage=iEltImage->IplImage();
    pu_ib = (unsigned char*)iImage.imageData;
    xi_dimx = iImage.width;
    xi_dimy = iImage.height;
	MAPSTimestamp t = iEltImage->Timestamp();
	return t;
}

MAPSTimestamp MAPSLivicIRViewer::mtsp_PistesIRSuivant()
{
    MAPSIOElt *iEltPistesIR = StartReading(Input("iPistesIRStereo"));
    po_pistesIR = &iEltPistesIR->RealObject();
    xi_nbPistesIR = iEltPistesIR->VectorSize();
    MAPSTimestamp t = iEltPistesIR->Timestamp();

	MAPSIOElt *iEltPistesIRVehicule = StartReading(Input("iPistesIRVehicule"));
	po_pistesIRVehicule = &iEltPistesIRVehicule->RealObject();

	return t;
}

void MAPSLivicIRViewer::mv_MessageSuivant()
{
    void *p;
    LivicInt64 val;
	int vali;
    int filtre;
	char msg[TAILLE_MSG+1];
		
    TempsAffichageMessage = GetIntegerProperty("pTempsAffichageMessages")*1000;

    if( DataAvailableInFIFO(Input("iMessages")) )
		{
			MAPSIOElt *iEltMessages = StartReading(Input("iMessages"));
			char *data=(char*)iEltMessages->Stream8();
			memcpy( msg, data, TAILLE_MSG);
			msg[TAILLE_MSG] = 0;
			xt_timestampMessages = MAPS::CurrentTime();
			StopReading(Input("iMessages"));
            

            if(IsConnected(Input("iMessagesFiltres")) && DataAvailableInFIFO(Input("iMessagesFiltres")))
			{
                MAPSIOElt *iEltMessagesFiltre = StartReading(Input("iMessagesFiltres"));
                filtre = iEltMessagesFiltre->Integer();
			    StopReading(Input("iMessagesFiltres"));
            }
            else 
                filtre = 0;

			// id:num:ty:pa1:pa2
			char sid[2+1], snum[3+1], sty[2+1], spa1[3+1], spa2[3+1];
			int id, num, ty, pa1, pa2;
			int pos = 0;
			strncpy(sid, msg+pos, 2);
			pos += 2+1;
			strncpy(snum, msg+pos, 3);
			pos += 3+1;
			strncpy(sty, msg+pos, 2);
			pos += 2+1;
			strncpy(spa1, msg+pos, 3);
			pos += 3+1;
			strncpy(spa2, msg+pos, 3);
			id = atoi(sid);
			num = atoi(snum);
			ty = atoi(sty);
			pa1 = atoi(spa1);
			pa2 = atoi(spa2);

			char *message = (char*)malloc(512*sizeof(char));

			switch( ty )
			{
            case MSG_TY_FREINAGE :		 sprintf(message, "%d : Freinage d'urgence", id); break;
			case MSG_TY_VIRAGE_DROITE :  sprintf(message, "%d : Abord d un virage a droite", id); break;
			case MSG_TY_VIRAGE_GAUCHE :  sprintf(message, "%d : Abord d un virage a gauche", id); break;
			case MSG_TY_ADHERENCE :		 sprintf(message, "%d : Perte d'adherence", id); break;
			case MSG_TY_OBSTACLE :		 sprintf(message, "%d : Obstacle detecte", id); break;
            case MSG_TY_LOVE :			 sprintf(message, "%d : Salut beau brun, appelle moi au 06 11 43 37", id); break;
			case MSG_TY_FEU_ROUGE :		 sprintf(message, "%d : Feu rouge : %d", id, pa1); break;
			case MSG_TY_FEU_ORANGE :	 sprintf(message, "%d : Feu orange : %d", id, pa1); break;
			case MSG_TY_FEU_VERT :		 sprintf(message, "%d : Feu vert : %d", id, pa1); break;
			case MSG_TY_INFO  :			 sprintf(message, "%d : Consigne vit: %d code: %d ", id, pa1, pa2); break;
			default :					 sprintf(message, "%d : Message inconnu", id); break;
			}
			
            bool msg_repete = false;
            
			for (int i = 0; i < xo_messages.nbElements(); i++)
			{
                if ((xo_messagesID[i] == id)  && (xo_messagesTY[i] == ty) && (xo_messagesPA1[i] == pa1) && ((!xo_messageATraiter[i]) == filtre))
				{				
					msg_repete = true;
                    ReportWarning("message repete");
					break;
				}
			}
            if(!msg_repete)
            {
                xo_messages.empile((void*)message);
			    xo_messagesTimestamp.empile(xt_timestampMessages);
			    xo_messagesID.empile(id);
                xo_messagesTY.empile(ty);
                xo_messagesPA1.empile(pa1);
				xo_messagesPA2.empile(pa2);
                xo_messageATraiter.empile(!filtre);
            }
		}
		
		while((xo_messages.nbElements()>0) && ((MAPS::CurrentTime() - xo_messagesTimestamp[0]) > TempsAffichageMessage) )
		{
			//  Suppression des messages trop vieux:
			xo_messages.depileBase(&p);
			free(p);
			xo_messagesTimestamp.depileBase(&val);
			xo_messagesID.depileBase(&vali);
            xo_messagesTY.depileBase(&vali);
            xo_messagesPA1.depileBase(&vali);
			xo_messagesPA2.depileBase(&vali);
            xo_messageATraiter.depileBase(&vali);
        }
}

unsigned char* MAPSLivicIRViewer::mv_AffichageMeteo(int code)
{
	 switch (code)
	  { case 0:
				 return pu_ImageMeteoNA;
		case 1:
				 return pu_ImageMeteojour;
		case 2:
				 return pu_ImageMeteonuit;
		case 3:
				 return pu_ImageMeteochaud;
		case 4:
				 return pu_ImageMeteopluiejour;
		case 5:
				 return pu_ImageMeteopluienuit;
		case 6:
		case 7:
				 return pu_ImageMeteoverglas;
		default:
				 return NULL;
		}

}
//Gestion des messages sonore
void MAPSLivicIRViewer::MsgSonore()
{	
	bool traite = false;
	int MsgType, Msg, distance, j, i;
	int nbMessages = xo_messages.nbElements();

	//Gere les messages panneaux
	for(i=0 ; i < xi_nbPistesIR ; i++)
       if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
	   {
		   if(typeIdentifiant(po_pistesIR[i].id)==TYPE_ID_INFRA)
		   {MsgType=TYPE_ID_INFRA;
			Msg=po_pistesIR[i].id; //Id du panneau
			distance = (int)min(GetIntegerProperty("pDistanceMax"), po_pistesIRVehicule[i].z);
			traite = true;
		   }
	   }

	//Gere les messages vehicule "prioritaire"
	for(i=nbMessages-1; i>=0;i--)
    {
		for(j= 0; j<xi_nbPistesIR;j++)
			if( xo_messagesID[i]==po_pistesIR[j].id)
			{
				MsgType=TYPE_ID_VEHICULE;
				Msg = xo_messagesTY[i]; //le message d'alerte !
				distance = (int)min(GetIntegerProperty("pDistanceMax"), po_pistesIRVehicule[i].z);
				traite = true;
			}
	}

	//Ecriture du message
	if(traite)
	{	MAPSIOElt* ioEltMsgSonore = StartWriting(Output("oMsgSonore"));
		ioEltMsgSonore->VectorSize() = 3;
		ioEltMsgSonore->Integer(0)= MsgType;
		ioEltMsgSonore->Integer(1)= Msg;
		ioEltMsgSonore->Integer(2)= distance;
		StopWriting(ioEltMsgSonore);
	}
}

void MAPSLivicIRViewer::mv_AffichagePanneaux()
{
        //  ----------------------------------------------------------
        //  Affichage des panneaux:
        //  ----------------------------------------------------------

		char sdist[32];
		int xPiste, yPiste, distance;

        for(int i=0 ; i < xi_nbPistesIR ; i++)
        {
			xPiste = (int)po_pistesIR[i].x;
			yPiste = (int)po_pistesIR[i].y;
			distance = (int)min(GetIntegerProperty("pDistanceMax"), po_pistesIRVehicule[i].z);
			bool traite = false;
            switch ((int)po_pistesIR[i].id)
            {
            case ID_PANNEAU_DEBUT_30:
                {
					sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Zone 30");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageDebutZone30, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
            case ID_PANNEAU_FIN_30:
                {
                    sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Fin de zone 30");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageFinZone30, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
            case ID_PANNEAU_VIRAGE_GAUCHE:
                {   
                    sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Virage a gauche");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageVirageGauche, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
            case ID_PANNEAU_CHICANE:
                {
                    sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Chicane");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageChicane, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
            case ID_STOP:
                {
                    sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Stop");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageStop, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
            case ID_DANGER:
                {
                    sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Danger");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageDanger, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
            case ID_FEU:
                {
                    sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Feu tricolore");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageFeu, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
            case ID_PANNEAU_VITESSE_90:
                {
                    sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Vitesse limitee");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageVitesse90, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
            case ID_PANNEAU_VITESSE_50:
                {
                    sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Vitesse limitee");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageVitesse50, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
            case ID_PANNEAU_FIN_VITESSE_50:
                {
                    sprintf ( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], "Fin de vitesse limitee a 50");
                    if( (!GetBoolProperty("pConfirmationPanneauxParSV")) || (po_pistesIR[i].sign.type != -1) )
					{
						mv_SetPanneau(pu_ImageFinVitesse50, xPiste, yPiste, distance);
					}
					traite = true;
                    break;
                }
			default:
                {break;}
            }
			if( traite )
			{
				if( xb_AffichageDistances )
				{
					
					int xi_precision = GetIntegerProperty("pPrecision");
					int distance_affichee = (int)distance/xi_precision*xi_precision;

					if (distance_affichee < GetIntegerProperty("pDistanceMax"))
					{
						sprintf( sdist, "      %d m", distance_affichee);
					}
					else
					{
						sprintf( sdist, " over %d m", distance_affichee);
					}
					strcat( ps_MessagesInfrastructure[xi_nbMessagesInfrastructure], sdist);
				}
				xi_nbMessagesInfrastructure++;
			}
        }
	mv_AffichePanneau();
}

void MAPSLivicIRViewer::mv_AffichePanneau()
{
	if( pu_lastPanneauImage == NULL ) return;
	if( (MAPS::CurrentTime() - xt_lastPanneauTime) < (GetIntegerProperty("pTempsAffichagePanneaux")*1000) )
	{
		pu_ImageIHMConducteur = pu_lastPanneauImage;

		mv_CopieMiniImage(obS, xi_dimx, xi_dimy, pu_lastPanneauImage, xi_lastPanneauX, xi_lastPanneauY,DIM_IMAGES_PANNEAUX, DIM_IMAGES_PANNEAUX, xi_lastPanneauDist, true, true);
	
	} else
	{
		pu_lastPanneauImage = NULL;
	}
}

void MAPSLivicIRViewer::mv_SetPanneau(unsigned char* panneau, int x, int y, int dist)
{
	pu_lastPanneauImage = panneau;
	xi_lastPanneauX = x;
	xi_lastPanneauY = y;
	xi_lastPanneauDist = dist;
	xt_lastPanneauTime = MAPS::CurrentTime();
}

void MAPSLivicIRViewer::mv_CopieMiniImage(unsigned char* image, int maxx, int maxy, unsigned char* mini, int x, int y, int dimMiniX, int dimMiniY, int dist, bool inversionRGB, bool centrage)
{
	int zoom = 1;
	if( GetBoolProperty("pZoomPanneaux") && (dist>=0) ) 
	{
		zoom = max(1, 
		GetIntegerProperty("pZoomMin") +
		(GetIntegerProperty("pZoomMax") - GetIntegerProperty("pZoomMin"))*
		((double)dist/(GetIntegerProperty("pDistanceMax"))
		));

		//centrage des images
		if( centrage )
		{
			x = x -(dimMiniX/(zoom))/3;
			y = y -(dimMiniY/(zoom))/3;
		}

	}
	for(int m=0; m<dimMiniY; m++)
	{
		if( m%zoom ) continue;
		for(int n=0; n<dimMiniX; n++)
		{
			if( n%zoom ) continue;
			int pix=0;
			if( zoom!=1 ) pix = 3*(x-(maxx/(2*zoom))+m/zoom+(y-(maxy/(2*zoom))+n/zoom)*maxx);
			else pix = 3*(x+m+(y+n)*maxx);
			if( (pix <0) || (pix>=(3*maxx*maxy)-3) ) continue;
			int mpix = 3*(m+n*dimMiniX);
			if( (mini[mpix]==255) && (mini[mpix+1]==255) && (mini[mpix+2]==255) )
				continue;
			if( inversionRGB )
			{
				image[pix] = mini[mpix+2];
				image[pix+1] = mini[mpix+1];
				image[pix+2] = mini[mpix];
			} else
			{
				image[pix] = mini[mpix];
				image[pix+1] = mini[mpix+1];
				image[pix+2] = mini[mpix+2];
			}
		}
	}
}

int MAPSLivicIRViewer::mv_AffichageMessages(char *ihmMsg, unsigned char** pu_imageMeteo)
{
    //  -------------------------------------------------------
    //  affichage des messages instantanés
    //  -------------------------------------------------------
    
    int j;
    bool elementNonTraite = true;
	bool feuNonTraite = true;
	int nbMessages = xo_messages.nbElements();
	int yMessageSortie, xMessageSortie, yMessageIHM, xMessageIHM, xPiste, yPiste;
	int distance;
	int nbMessagesAffiches = nbMessages;
	int dimMiniImage = DIM_IMAGES_PANNEAUX / (GetIntegerProperty("pZoomMin")-1);

    for(int i=nbMessages-1; i>=0;i--)
    {
		elementNonTraite = true;
		xMessageSortie = 15; //15*(nbMessages-i+1)
		yMessageSortie = 20*(nbMessages-nbMessagesAffiches);
		xMessageIHM = 15;
		yMessageIHM = DIM_IMAGES_PANNEAUX+20*(nbMessages-nbMessagesAffiches);
        for(j= 0; j<xi_nbPistesIR;j++)
		{
			xPiste = (int)po_pistesIR[j].x;
			yPiste = (int)po_pistesIR[j].y;
			distance = (int)min(GetIntegerProperty("pDistanceMax"), po_pistesIRVehicule[i].z);
			if( xo_messagesID[i]==po_pistesIR[j].id)
			{
				switch( xo_messagesTY[i] )
				{
				case MSG_TY_FEU_ROUGE:
				{
					if( feuNonTraite )
					{
						mv_SetPanneau(pu_ImageFeuRouge, xPiste, yPiste, distance);
						sprintf(xs_msg, "%d", xo_messagesPA1[i]);
						printT2I(obS, xs_msg, 2, xPiste, yPiste-dimMiniImage/2-10, RED, xi_dimx, xi_dimy);
						strcpy(ihmMsg, xs_msg);
						feuNonTraite = false;
					}
					elementNonTraite = false;
					break;
				}
				case MSG_TY_FEU_ORANGE:
				{
					if( feuNonTraite )
					{
						mv_SetPanneau(pu_ImageFeuOrange, xPiste, yPiste, distance);
						sprintf(xs_msg, "%d", xo_messagesPA1[i]);
						printT2I(obS, xs_msg, 2, xPiste, yPiste-dimMiniImage/2-10, RED, xi_dimx, xi_dimy);
						strcpy(ihmMsg, xs_msg);
						feuNonTraite = false;
					}
					elementNonTraite = false;
					break;
				}
				case MSG_TY_FEU_VERT:
				{
					if( feuNonTraite )
					{
						mv_SetPanneau(pu_ImageFeuVert, xPiste, yPiste, distance);
						sprintf(xs_msg, "%d", xo_messagesPA1[i]);
						printT2I(obS, xs_msg, 2, xPiste, yPiste-dimMiniImage/2-10, RED, xi_dimx, xi_dimy);
						strcpy(ihmMsg, xs_msg);
						feuNonTraite = false;
					}
					elementNonTraite = false;
					break;
				}
				case MSG_TY_INFO:
						sprintf(xs_msg, "%d Km h", xo_messagesPA1[i]);
						printT2I(obS, xs_msg, 2, xPiste-30, yPiste-dimMiniImage/2-10, RED, xi_dimx, xi_dimy);
						strcpy(ihmMsg, xs_msg);
					    *pu_imageMeteo = mv_AffichageMeteo(xo_messagesPA2[i]);
						elementNonTraite = false;
					break;
				default:
				{
					if (xo_messageATraiter[i])
					{
						if( xo_messagesTY[i]==MSG_TY_FREINAGE )
						{
							mv_SetPanneau(pu_ImageStop, xPiste, yPiste, distance);
						}
						else
						if( xo_messagesTY[i]==MSG_TY_VIRAGE_DROITE )
						{
							mv_SetPanneau(pu_ImageVirageDroite, xPiste, yPiste, distance);
						}
						else
						if( xo_messagesTY[i]==MSG_TY_VIRAGE_GAUCHE )
						{
							mv_SetPanneau(pu_ImageVirageGauche, xPiste, yPiste, distance);
						}
						else
						{
							mv_SetPanneau(pu_ImageDanger, xPiste, yPiste, distance);
						}
						sprintf(xs_msg, "%s", (char*)xo_messages[i]);
						printT2I(
							obS,
							xs_msg,
							2,
							xMessageSortie,
							yMessageSortie,
							RED,
							xi_dimx,
							xi_dimy
							);
						//IHMDetectionIR.EcritTexte (message, 2, 15, 20*(i+1), RED);
						DrawRGBLine (obS, xMessageSortie, yMessageSortie+15, xPiste, yPiste, RED, xi_dimx, xi_dimy);
						DrawRGBLine (obS, xMessageSortie, yMessageSortie+15, xi_dimx-15, yMessageSortie+15, RED, xi_dimx, xi_dimy);
						sprintf(xs_msg, "%s", (char*)xo_messages[i]);
						printT2I(
							IHMConducteur,
							xs_msg,
							4, //2
							xMessageIHM,
							yMessageIHM,
							BLUE, // ca fait rouge en fait a l'ecran
							DIM_IMAGES_PANNEAUX,
							DIM_IMAGES_PANNEAUX+80
							);
						nbMessagesAffiches--;
						elementNonTraite = false;
					}
					else
					{
						sprintf(xs_msg, "%s", (char*)xo_messages[i]);
						printT2I(
							obS,
							xs_msg,
							2,
							xMessageSortie,
							yMessageSortie,
							RED, // ca fait rouge en fait a l'ecran
							xi_dimx,
							xi_dimy
							);
						DrawRGBLine (obS, xMessageSortie, yMessageSortie+15, xPiste, yPiste, BLUE, xi_dimx, xi_dimy);
						DrawRGBLine (obS, xMessageSortie, yMessageSortie+15, xi_dimx-15, yMessageSortie+15, BLUE, xi_dimx, xi_dimy);
						//IHMDetectionIR.EcritTexte (message, 1, 15, 20*(i+1), BLUE);
						printT2I(
							IHMConducteur,
							xs_msg,
							2,
							xMessageIHM,
							yMessageIHM,
							BLUE,
							DIM_IMAGES_PANNEAUX,
							DIM_IMAGES_PANNEAUX+80
							);
						nbMessagesAffiches--;
						elementNonTraite = false;
					}
				}
				} //switch
			}
		}
        if(elementNonTraite)
        {
			ReportInfo("Coucou");
			if( xo_messagesTY[i]!=MSG_TY_FEU_ROUGE &&
				xo_messagesTY[i]!=MSG_TY_FEU_ORANGE &&
				xo_messagesTY[i]!=MSG_TY_FEU_VERT &&
				xo_messagesTY[i]!=MSG_TY_INFO)
			{
				sprintf(xs_msg, "%s", (char*)xo_messages[i]);
				printT2I(
					obS,
					xs_msg,
					2,
					xMessageSortie,
					yMessageSortie,
					BLUE,
					xi_dimx,
					xi_dimy
					);
					printT2I(
						IHMConducteur,
						xs_msg,
						2,
						xMessageIHM,
						yMessageIHM,
						BLUE,
						DIM_IMAGES_PANNEAUX,
						DIM_IMAGES_PANNEAUX+80
						);
				nbMessagesAffiches--;
			}
        }
    }
	return nbMessages-nbMessagesAffiches;
}

int MAPSLivicIRViewer::mi_XZ2U(double _x, double _z)
{
    double u = xd_Uo + xd_AlphaU*(_x-xd_DecalageCameraX)/(-xd_DecalageCameraY*sin(xd_TangageCamera*PIsur180) + (_z-xd_DecalageCameraZ)*cos(xd_TangageCamera*PIsur180));
    return(int(u+0.5));
}

int MAPSLivicIRViewer::mi_XZ2V(double _x, double _z)
{
    double v = xd_Vo + xd_AlphaV*(-xd_DecalageCameraY*cos(xd_TangageCamera*PIsur180) - (_z-xd_DecalageCameraZ)*sin(xd_TangageCamera*PIsur180))/(-xd_DecalageCameraY*sin(xd_TangageCamera*PIsur180) + (_z-xd_DecalageCameraZ)*cos(xd_TangageCamera*PIsur180));
    return(int(v+0.5));
}


void MAPSLivicIRViewer::Core() 
{
	MAPSTimestamp tps_debut = MAPS::CurrentTime();
    xi_nbPistesIR = 0;
    po_pistesIR = NULL;
    pu_ib = NULL;
    xt_timestampImage = 0;
    xt_timestampPistesIR = 0;
    xi_dimx = 0;
    xi_dimy = 0;

    xi_nbMessagesInfrastructure = 0;

    /*unsigned char *ibImageDroite;
    unsigned char *ibImageGauche;*/

    xb_AffichageZoneFiltrage = GetBoolProperty("pAffichageZonesFiltrage");
    xb_AffichageVoiesCirculation = GetBoolProperty("pAffichageVoiesCirculation");
    xb_AffichageVitesses = GetBoolProperty("pAffichageVitesses");
    xb_AffichageDistances = GetBoolProperty("pAffichageDistances");

    // ------------------------------------------------------------------------------------------------
    // lecture des pistes IR
    if( IsConnected(Input("iPistesIRStereo")) && IsConnected(Input("iPistesIRVehicule")) )
    {
        xt_timestampPistesIR = mtsp_PistesIRSuivant();
    }
    // lecture des pistes IR
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // lecture de l'image d'entree
    xt_timestampImage = mtsp_ImageSuivant();
    // lecture de l'image d'entree
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // lecture du message radio
    if( IsConnected(Input("iMessages")) )
    {
        mv_MessageSuivant();
		MsgSonore();
    }


    // lecture du message radio
    // ------------------------------------------------------------------------------------------------
    

    // ------------------------------------------------------------------------------------------------
    // recuperation zone filtrage des messages
    xd_DecalageCameraX = GetFloatProperty("pDecalageCameraX");
    xd_DecalageCameraY = GetFloatProperty("pDecalageCameraY");
    xd_DecalageCameraZ = GetFloatProperty("pDecalageCameraZ");
	xd_TangageCamera   = GetFloatProperty("pTangageCamera");
	if (IsConnected(Input("iTangageVehicule")))
	{
	    MAPSIOElt *ioEltTang = StartReading(Input("iTangageVehicule"));
		xd_TangageCamera += ioEltTang->Float();
		StopReading(Input("iTangageVehicule"));
	}
    xd_Uo              = GetFloatProperty("pUo");
    xd_Vo              = GetFloatProperty("pVo");
    xd_AlphaU          = GetFloatProperty("pAlphaU");
    xd_AlphaV          = GetFloatProperty("pAlphaV");

    // ------------------------------------------------------------------------------------------------
    // recuperation zone filtrage des messages
    if(xb_AffichageZoneFiltrage)
    {
        // //  -----------------------------------------
        ////  Recuperation des parametres de la camera:
        ////  -----------------------------------------
        //if(DataAvailableInFIFO(Input("iParametresCameras")))
        //{
        //    MAPSIOElt *ioElt=StartReading(Input("iParametresCameras"));
        //    if (ioElt==NULL)CommitSuicide();
        //    StructureParametresCameras &Parametres=*static_cast<StructureParametresCameras *>(ioElt->Data());
        //    xo_parametresCameras = Parametres;
        //    StopReading(Input("iParametresCameras"));
        //}
        //else
        //{
        //    xo_parametresCameras.H = 1.335;    //HauteurCameras
        //    xo_parametresCameras.B = 1.04;     //BaseStereoscopique
        //    xo_parametresCameras.Alpha = 525;  //Alpha
        //    xo_parametresCameras.Theta = 5.2;  //TangageCamerasRepos
        //}

        //  -------------------------------------------------
        //  Recuperation de la zone de filtrage des messages:
        //  -------------------------------------------------
        int NbPointsZR = 0;
        double *RoadMarkersVector;
        if(IsConnected(Input("iZoneFiltrageMessages")))
        {
            MAPSIOElt *IOELtZR = StartReading(Input("iZoneFiltrageMessages"));
            RoadMarkersVector=&IOELtZR->Float();
            int xi_NbRoadMarkers = int(*RoadMarkersVector);


			xi_NbPtDroiteZoneFiltrage = 0;
			xi_NbPtGaucheZoneFiltrage = 0;

			if (xi_NbRoadMarkers > MAX_ROADMARKERS)
				xi_NbRoadMarkers = MAX_ROADMARKERS;

			int Number = 1;
			double X,Z;

			//	Recuperation des coordonnées des points limites de la zone de recherche 
			for(int i=0 ; i < xi_NbRoadMarkers ; i++)
			{
				//	Extraction des points de droite:
				Z = RoadMarkersVector[Number++];
				X = RoadMarkersVector[Number++];

				pd_PtsGaucheZoneFiltrage[2*xi_NbPtGaucheZoneFiltrage] = Z;
				pd_PtsGaucheZoneFiltrage[2*xi_NbPtGaucheZoneFiltrage+1] = X;
				xi_NbPtGaucheZoneFiltrage ++;				


				//	Extraction des points de gauche:
				Z = RoadMarkersVector[Number++];
				X = RoadMarkersVector[Number++];

				pd_PtsDroiteZoneFiltrage[2*xi_NbPtDroiteZoneFiltrage] = Z;
				pd_PtsDroiteZoneFiltrage[2*xi_NbPtDroiteZoneFiltrage+1] = X;
				xi_NbPtDroiteZoneFiltrage ++;

			}

			StopReading(Input("iZoneFiltrageMessages"));
        }
    }

    // lecture des coordonnees des voies de circulation
    if(xb_AffichageVoiesCirculation)
    {
        MAPSIOElt *ioEltAffichageVoies=StartReading(Input("iVoiesCirculation"));
        MAPSInteger *AffichageVoies = &ioEltAffichageVoies->Integer();
        int vectorSize = ioEltAffichageVoies->VectorSize();
        nbVoies = vectorSize/4;
        /*int *x1, *x2, *y1, *y2;*/

        for(int i=0; i<nbVoies; i++)
        {
            x1[i] = AffichageVoies[i];
            y1[i] = AffichageVoies[i+nbVoies];
            x2[i] = AffichageVoies[i+2*nbVoies];
            y2[i] = AffichageVoies[i+3*nbVoies];
        }
        StopReading(Input("iVoiesCirculation"));
    }
   
    // recuperation zone filtrage des messages
    // ------------------------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------------------
    // le first time
    if (xb_firstTime)
    {
        IplImage imgSortie;
        imgSortie = MAPS::IplImageModel(
            xi_dimx,
            xi_dimy,
            MAPS_CHANNELSEQ_RGB);
        Output(0).AllocOutputBufferIplImage(imgSortie);

        xb_AfficherIHMConducteur = GetBoolProperty("pAffichageIHMConducteur");
        xb_AfficherTimestamp = GetBoolProperty("pAffichageTimestamp");
        
        if (xb_AfficherIHMConducteur)
        {
			
            IplImage ModeleIHM_BGR  = MAPS::IplImageModel(DIM_IMAGES_PANNEAUX, DIM_IMAGES_PANNEAUX+80, MAPS_CHANNELSEQ_BGR);
            Output(1).AllocOutputBufferIplImage(ModeleIHM_BGR);
            //chargement des imagettes IHM:
            pu_ImageVirageDroite    = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageVirageGauche    = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageDebutZone30     = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageFinZone30       = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageChicane         = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImagePanneauFeu      = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageFeu             = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageFeuRouge        = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageFeuOrange       = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageFeuVert         = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageDanger          = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageMessage         = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageStop            = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageVitesse90       = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageVitesse50       = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageFinVitesse50    = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
			//Meteo :
			pu_ImageMeteoNA			= new unsigned char [DIM_IMAGES_METEO*DIM_IMAGES_METEO*3];
			pu_ImageMeteojour		= new unsigned char [DIM_IMAGES_METEO*DIM_IMAGES_METEO*3];
			pu_ImageMeteonuit		= new unsigned char [DIM_IMAGES_METEO*DIM_IMAGES_METEO*3];
			pu_ImageMeteochaud		= new unsigned char [DIM_IMAGES_METEO*DIM_IMAGES_METEO*3];
			pu_ImageMeteopluiejour  = new unsigned char [DIM_IMAGES_METEO*DIM_IMAGES_METEO*3];
			pu_ImageMeteopluienuit  = new unsigned char [DIM_IMAGES_METEO*DIM_IMAGES_METEO*3];
			pu_ImageMeteoverglas    = new unsigned char [DIM_IMAGES_METEO*DIM_IMAGES_METEO*3];

			
            ImageBMP bmp = ImageBMP("R:/maps/perception/identification/images/VirageDroite.bmp");
            int statut = bmp.LectureImage(pu_ImageVirageDroite,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/VirageGauche.bmp");
            statut = bmp.LectureImage(pu_ImageVirageGauche,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/DebutZone30.bmp");
            statut = bmp.LectureImage(pu_ImageDebutZone30,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/FinZone30.bmp");
            statut = bmp.LectureImage(pu_ImageFinZone30,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/Chicane.bmp");
            statut = bmp.LectureImage(pu_ImageChicane,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/PanneauFeu.bmp");
            statut = bmp.LectureImage(pu_ImagePanneauFeu,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/FeuRouge.bmp");
            statut = bmp.LectureImage(pu_ImageFeu,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/FeuRouge.bmp");
            statut = bmp.LectureImage(pu_ImageFeuRouge,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/FeuOrange.bmp");
            statut = bmp.LectureImage(pu_ImageFeuOrange,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/FeuVert.bmp");
            statut = bmp.LectureImage(pu_ImageFeuVert,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/Danger.bmp");
            statut = bmp.LectureImage(pu_ImageDanger,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/Message.bmp");
            statut = bmp.LectureImage(pu_ImageMessage,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/Stop.bmp");
            statut = bmp.LectureImage(pu_ImageStop,true);
			bmp = ImageBMP("R:/maps/perception/identification/images/Vitesse90.bmp");
            statut = bmp.LectureImage(pu_ImageVitesse90,true);
			bmp = ImageBMP("R:/maps/perception/identification/images/Vitesse50.bmp");
            statut = bmp.LectureImage(pu_ImageVitesse50,true);
			bmp = ImageBMP("R:/maps/perception/identification/images/FinVitesse50.bmp");
            statut = bmp.LectureImage(pu_ImageFinVitesse50,true);
			//Meteo
			bmp = ImageBMP("R:/maps/perception/identification/images/Meteo/0na.bmp");
            statut = bmp.LectureImage(pu_ImageMeteoNA,true);
			bmp = ImageBMP("R:/maps/perception/identification/images/Meteo/1jour.bmp");
            statut = bmp.LectureImage(pu_ImageMeteojour,true);
			bmp = ImageBMP("R:/maps/perception/identification/images/Meteo/2nuit.bmp");
            statut = bmp.LectureImage(pu_ImageMeteonuit,true);
			bmp = ImageBMP("R:/maps/perception/identification/images/Meteo/3chaud.bmp");
            statut = bmp.LectureImage(pu_ImageMeteochaud,true);
			bmp = ImageBMP("R:/maps/perception/identification/images/Meteo/4pluiejour.bmp");
            statut = bmp.LectureImage(pu_ImageMeteopluiejour,true);
			bmp = ImageBMP("R:/maps/perception/identification/images/Meteo/5pluienuit.bmp");
            statut = bmp.LectureImage(pu_ImageMeteopluienuit,true);
			bmp = ImageBMP("R:/maps/perception/identification/images/Meteo/6verglas.bmp");
            statut = bmp.LectureImage(pu_ImageMeteoverglas,true);


        }
        xb_firstTime=false;
    }
        

    // ------------------------------------------------------------------------------------------------
    // startwriting image sortie
    MAPSIOElt* oEltImgSortie=StartWriting(Output("oImageSortie"));    
    IplImage &oImgSortie=oEltImgSortie->IplImage();
    oEltImgSortie->Timestamp()=xt_timestampImage;
    obS=(unsigned char*)oImgSortie.imageData; 
    memset(obS, 0, xi_dimx*xi_dimy*3);

        

    // startwriting image sortie
    // ------------------------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------------------
    // recopie image entree dans image sortie
    if( pu_ib )
    {
        long taille = xi_dimx*xi_dimy;

        if(pe_TypeImage.selectedEnum) taille = taille*3;
        
        for(int p=0; p<taille; p++)
        {
             if(pe_TypeImage.selectedEnum)
                 memset(obS+p, pu_ib[p], 1);
             else 
                 memset(obS+3*p, pu_ib[p], 3);
        }
    }
    // recopie image entree dans image sortie
    // ------------------------------------------------------------------------------------------------
  

    // ------------------------------------------------------------------------------------------------
    // affichage des pistes en overlay de l'image
    if( xi_nbPistesIR>0 )
    {
        for(int i=0; i<xi_nbPistesIR; i++)
        {
			int ligne = 0;
            DrawRGBSquare(
                obS,
                (int)po_pistesIR[i].x-5,
                (int)po_pistesIR[i].y-5,
                (int)po_pistesIR[i].x+5,
                (int)po_pistesIR[i].y+5,
                CYAN,
                xi_dimx,
                xi_dimy
            );
            sprintf(xs_msg, "id:%d", po_pistesIR[i].id);
            printT2I(
                obS,
                xs_msg,
                (int)po_pistesIR[i].x+10,
                (int)po_pistesIR[i].y-15+8*ligne++,
                CYAN,
                xi_dimx,
                xi_dimy
            );
			if(xb_AffichageDistances)
			{
				sprintf(xs_msg, "z:%.0f", po_pistesIRVehicule[i].z);
				printT2I(
					obS,
					xs_msg,
					(int)po_pistesIR[i].x+10,
					(int)po_pistesIR[i].y-15+8*ligne++,
					CYAN,
					xi_dimx,
					xi_dimy
				);
			}
			if(xb_AffichageVoiesCirculation)
			{
				sprintf(xs_msg, "voie:%d", po_pistesIR[i].misc3);
				printT2I(
					obS,
					xs_msg,
					(int)po_pistesIR[i].x+10,
		            (int)po_pistesIR[i].y-15+8*ligne++,
					CYAN,
					xi_dimx,
					xi_dimy
				);
			}
           if(xb_AffichageVitesses && po_pistesIR[i].vehicle.speed >= 0)
            {
                sprintf(xs_msg, "v:%.1f", po_pistesIR[i].vehicle.speed);
                printT2I(
                    obS,
                    xs_msg,
                    (int)po_pistesIR[i].x+10,
	                (int)po_pistesIR[i].y-15+8*ligne++,
                    CYAN,
                    xi_dimx,
                    xi_dimy
                );
            }

			/*ReportWarning((const char*)po_pistesIR[i].vehicle.speed);*/
        }
    }
    // affichage des pistes en overlay de l'image
    // ------------------------------------------------------------------------------------------------

    if(xb_AfficherTimestamp)
    {
        // ------------------------------------------------------------------------------------------------
        // affichage des timestamps
        sprintf(xs_msg, "%I64d", xt_timestampImage);
        printT2I(
            obS,
            xs_msg,
            xi_dimx-60,
            10,
            WHITE,
            xi_dimx,
            xi_dimy);
        sprintf(xs_msg, "%I64d", xt_timestampPistesIR);
        printT2I(
            obS,
            xs_msg,
            xi_dimx-60,
            20,
            CYAN,
            xi_dimx,
            xi_dimy);
        if( xt_timestampImage!=xt_timestampPistesIR )
        {
            printT2I(
                obS,
                "DECALAGE",
                xi_dimx-60,
                30,
                ORANGE,
                xi_dimx,
                xi_dimy);
        }
    }
    // affichage des timestamps
    // ------------------------------------------------------------------------------------------------

    // ------------------------------------------------------------------------------------------------
    // affichage des panneaux et messages
    if(xb_AfficherIHMConducteur)
	{        
		MAPSIOElt *oIOEltIHMConducteur          = StartWriting(Output("oIHMConducteur"));
        IplImage &oIPLImageIHMConducteur        = oIOEltIHMConducteur->IplImage();
        oIOEltIHMConducteur->Timestamp()        = xt_timestampImage;
        
        IHMConducteur=(unsigned char*)oIPLImageIHMConducteur.imageData;
        memset(IHMConducteur, 0, (DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX+80)*3);

		memset(IHMConducteur+3*DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX, 255, 3*80*DIM_IMAGES_PANNEAUX*sizeof(unsigned char));

        // ------------------------------------------------------------------------------------------------
        // affichage des panneaux
        // si les ID correspondent à un panneau, affichage du panneau
        if( GetBoolProperty("pAffichagePanneaux") ) mv_AffichagePanneaux();
        // affichage des panneaux
        // ------------------------------------------------------------------------------------------------

		char ihmMsg[64] = "";
		int nbMessagesAffiches;
		unsigned char *pu_imageMeteo = NULL;

		nbMessagesAffiches = mv_AffichageMessages(ihmMsg, &pu_imageMeteo);

		//	Sinon, affichage de la vignette panneau
        if (pu_ImageIHMConducteur!=NULL)
        {
            //IHMConducteur.CopieRect(pu_ImageVirageDroite, 0, 0, 300, 200, LIVIC_RGB);
            for (int j=0; j<DIM_IMAGES_PANNEAUX; j++)
                memcpy(IHMConducteur+3*DIM_IMAGES_PANNEAUX*j, pu_ImageIHMConducteur+3*DIM_IMAGES_PANNEAUX*j, DIM_IMAGES_PANNEAUX*3*sizeof(unsigned char));
			if( strlen(ihmMsg)>0 )
				printT2I(IHMConducteur, ihmMsg, 4, 20, 20, BLUE, DIM_IMAGES_PANNEAUX, DIM_IMAGES_PANNEAUX);
//                memset(IHMConducteur+3*DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX, 255, 3*80*DIM_IMAGES_PANNEAUX*sizeof(unsigned char));
			if( pu_imageMeteo ) //Gere l'affichage de la meteo
					mv_CopieMiniImage(
					 IHMConducteur, 
					 DIM_IMAGES_PANNEAUX, 
					 DIM_IMAGES_PANNEAUX+80, 
					 pu_imageMeteo, 
					 DIM_IMAGES_PANNEAUX - 20 - DIM_IMAGES_METEO, 
					 20, 
					 DIM_IMAGES_METEO,
					 DIM_IMAGES_METEO,
					 -1,
					 false,
					 false);            
        }
		pu_ImageIHMConducteur = NULL;
		//	ou de rien...
        /*else
        {
			memset(IHMConducteur, 0, DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3*sizeof(unsigned char));
			memset(IHMConducteur+3*DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX, 255, 3*80*DIM_IMAGES_PANNEAUX*sizeof(unsigned char));
        }*/
		
        ////  affichage des messages infrastructure
        for(int i=0; i<xi_nbMessagesInfrastructure; i++)
        {
            printT2I(
                    IHMConducteur,
                    ps_MessagesInfrastructure[i],
                    2,
                    15,
                    DIM_IMAGES_PANNEAUX+20*(nbMessagesAffiches),
                    ORANGE,
                    DIM_IMAGES_PANNEAUX,
                    DIM_IMAGES_PANNEAUX+80
                    );
			nbMessagesAffiches++;
        }

        StopWriting(oIOEltIHMConducteur);
    }

    if(xb_AffichageZoneFiltrage)
    {
  

        //	-----------------------------------------------
		//	Affichage de la zone de filtrage des messages :
		//	-----------------------------------------------
		
		int u1,u2,v1,v2;
        double x1, x2, z1, z2;

		//	a droite:
		for(int i=0 ; i < xi_NbPtDroiteZoneFiltrage-1 ; i++)
		{
			//u1 = (int)(xi_dimx/2 + ( xo_parametresCameras.Alpha *  (pd_PtsDroiteZoneFiltrage[2*i+1] -(xo_parametresCameras.B * 0.5) ) ) / ( (0 + xo_parametresCameras.H) * sin(xo_parametresCameras.Theta*PIsur180) +pd_PtsDroiteZoneFiltrage[2*i] * cos(xo_parametresCameras.Theta*PIsur180) ));
   //         //(int)po_stereoIR->CapteurActif()->XYZ2Ud(pd_PtsDroiteZoneFiltrage[2*i+1], 0, pd_PtsDroiteZoneFiltrage[2*i]);
			//u2 = (int)(xi_dimx/2 + ( xo_parametresCameras.Alpha *  (pd_PtsDroiteZoneFiltrage[2*(i+1)+1] -(xo_parametresCameras.B * 0.5) ) ) / ( (0 + xo_parametresCameras.H) * sin(xo_parametresCameras.Theta*PIsur180) +pd_PtsDroiteZoneFiltrage[2*(i+1)] * cos(xo_parametresCameras.Theta*PIsur180) ));
   //         //(int)po_stereoIR->CapteurActif()->XYZ2Ud(pd_PtsDroiteZoneFiltrage[2*(i+1)+1], 0, pd_PtsDroiteZoneFiltrage[2*(i+1)]);

			//v1 = (int)(( (xi_dimy/2 * sin(xo_parametresCameras.Theta*PIsur180) + xo_parametresCameras.Alpha * cos(xo_parametresCameras.Theta*PIsur180)) * (0 + xo_parametresCameras.H) + (xi_dimy/2 * cos(xo_parametresCameras.Theta*PIsur180) - xo_parametresCameras.Alpha * sin(xo_parametresCameras.Theta*PIsur180))* pd_PtsDroiteZoneFiltrage[2*i]) /
   //                             ( (0+xo_parametresCameras.H) * sin(xo_parametresCameras.Theta*PIsur180) + pd_PtsDroiteZoneFiltrage[2*i] * cos(xo_parametresCameras.Theta*PIsur180) ));
   //         //(int)po_stereoIR->CapteurActif()->YZ2V(0, pd_PtsDroiteZoneFiltrage[2*i]);
			//v2 = (int)(( (xi_dimy/2 * sin(xo_parametresCameras.Theta*PIsur180) + xo_parametresCameras.Alpha * cos(xo_parametresCameras.Theta*PIsur180)) * (0 + xo_parametresCameras.H) + (xi_dimy/2 * cos(xo_parametresCameras.Theta*PIsur180) - xo_parametresCameras.Alpha * sin(xo_parametresCameras.Theta*PIsur180))* pd_PtsDroiteZoneFiltrage[2*(i+1)]) /
   //                             ( (0+xo_parametresCameras.H) * sin(xo_parametresCameras.Theta*PIsur180) + pd_PtsDroiteZoneFiltrage[2*(i+1)] * cos(xo_parametresCameras.Theta*PIsur180) ));
   //         //(int)po_stereoIR->CapteurActif()->YZ2V(0, pd_PtsDroiteZoneFiltrage[2*(i+1)]);

            x1 = pd_PtsDroiteZoneFiltrage[2*i+1];
            x2 = pd_PtsDroiteZoneFiltrage[2*(i+1)+1];
            z1 = pd_PtsDroiteZoneFiltrage[2*i];
            z2 = pd_PtsDroiteZoneFiltrage[2*(i+1)];
            u1 = mi_XZ2U(x1, z1);
            u2 = mi_XZ2U(x2, z2);
            v1 = mi_XZ2V(x1, z1);
            v2 = mi_XZ2V(x2, z2);
			DrawRGBLine(obS, u1, v1, u2, v2, GREEN, xi_dimx, xi_dimy);

		}


		//	a gauche:
		for(int i=0 ; i < xi_NbPtGaucheZoneFiltrage-1 ; i++)
		{
			//u1 = (int)(xi_dimx/2 + ( xo_parametresCameras.Alpha *  (pd_PtsGaucheZoneFiltrage[2*i+1] -(xo_parametresCameras.B * 0.5) ) ) / ( (0 + xo_parametresCameras.H) * sin(xo_parametresCameras.Theta*PIsur180) +pd_PtsGaucheZoneFiltrage[2*i] * cos(xo_parametresCameras.Theta*PIsur180) ));
   //         //(int)po_stereoIR->CapteurActif()->XYZ2Ud(pd_PtsGaucheZoneFiltrage[2*i+1], 0, pd_PtsGaucheZoneFiltrage[2*i]);
			//u2 = (int)(xi_dimx/2 + ( xo_parametresCameras.Alpha *  (pd_PtsGaucheZoneFiltrage[2*(i+1)+1] -(xo_parametresCameras.B * 0.5) ) ) / ( (0 + xo_parametresCameras.H) * sin(xo_parametresCameras.Theta*PIsur180) +pd_PtsGaucheZoneFiltrage[2*(i+1)] * cos(xo_parametresCameras.Theta*PIsur180) ));
   //         //(int)po_stereoIR->CapteurActif()->XYZ2Ud(pd_PtsGaucheZoneFiltrage[2*(i+1)+1], 0, pd_PtsGaucheZoneFiltrage[2*(i+1)]);

			//v1 = (int)(( (xi_dimy/2 * sin(xo_parametresCameras.Theta*PIsur180) + xo_parametresCameras.Alpha * cos(xo_parametresCameras.Theta*PIsur180)) * (0 + xo_parametresCameras.H) + (xi_dimy/2 * cos(xo_parametresCameras.Theta*PIsur180) - xo_parametresCameras.Alpha * sin(xo_parametresCameras.Theta*PIsur180))* pd_PtsGaucheZoneFiltrage[2*i]) /
   //                             ( (0+xo_parametresCameras.H) * sin(xo_parametresCameras.Theta*PIsur180) + pd_PtsGaucheZoneFiltrage[2*i] * cos(xo_parametresCameras.Theta*PIsur180) ));
   //         //(int)po_stereoIR->CapteurActif()->YZ2V(0, pd_PtsGaucheZoneFiltrage[2*i]);
			//v2 = (int)(( (xi_dimy/2 * sin(xo_parametresCameras.Theta*PIsur180) + xo_parametresCameras.Alpha * cos(xo_parametresCameras.Theta*PIsur180)) * (0 + xo_parametresCameras.H) + (xi_dimy/2 * cos(xo_parametresCameras.Theta*PIsur180) - xo_parametresCameras.Alpha * sin(xo_parametresCameras.Theta*PIsur180))* pd_PtsGaucheZoneFiltrage[2*(i+1)]) /
   //                             ( (0+xo_parametresCameras.H) * sin(xo_parametresCameras.Theta*PIsur180) + pd_PtsGaucheZoneFiltrage[2*(i+1)] * cos(xo_parametresCameras.Theta*PIsur180) ));
   //         //(int)po_stereoIR->CapteurActif()->YZ2V(0, pd_PtsGaucheZoneFiltrage[2*(i+1)]);

            x1 = pd_PtsGaucheZoneFiltrage[2*i+1];
            x2 = pd_PtsGaucheZoneFiltrage[2*(i+1)+1];
            z1 = pd_PtsGaucheZoneFiltrage[2*i];
            z2 = pd_PtsGaucheZoneFiltrage[2*(i+1)];
            u1 = mi_XZ2U(x1, z1);
            u2 = mi_XZ2U(x2, z2);
            v1 = mi_XZ2V(x1, z1);
            v2 = mi_XZ2V(x2, z2);
			DrawRGBLine(obS, u1, v1, u2, v2, ORANGE, xi_dimx, xi_dimy);

		}
		

        //  ---------------------------------------------------------
        //  Fin de lecture des entrées et d'ecriture sur les sorties
        //  ---------------------------------------------------------
        
    }

    if(xb_AffichageVoiesCirculation)
    {
        for(int voie=0; voie<nbVoies; voie++)
        {
            DrawRGBLine(obS, x1[voie], y1[voie], x2[voie], y2[voie], BLUE, xi_dimx, xi_dimy);
        }
    }

    // ------------------------------------------------------------------------------------------------
    // Stopreadings et stopwritings
    StopWriting(oEltImgSortie);
    StopReading(Input("iImage"));
    if( IsConnected(Input("iPistesIRStereo")) ) StopReading(Input("iPistesIRStereo"));
    if( IsConnected(Input("iPistesIRVehicule")) ) StopReading(Input("iPistesIRVehicule"));
    // Stopreadings et stopwritings
    // ------------------------------------------------------------------------------------------------

	MAPSTimestamp tps_fin = MAPS::CurrentTime();
	if( (tps_fin-tps_debut)>0 ) Rest(GetIntegerProperty("pPeriode")*1000 - (tps_fin-tps_debut));
}

void MAPSLivicIRViewer::Death()
{
    int n = xo_messages.nbElements();
    for(int i=0; i<n; i++) free((char*)xo_messages[i]);
    xo_messages.vide();
    xo_messagesTimestamp.vide();
    xo_messagesID.vide();
    xo_messagesTY.vide();
    xo_messagesPA1.vide();
	xo_messagesPA2.vide();
    xo_messageATraiter.vide();
    /*xo_synchros.vide();*/

    if (pu_ImageVirageDroite != NULL)
		delete[]pu_ImageVirageDroite;	
    if (pu_ImageVirageGauche != NULL)
        delete[]pu_ImageVirageGauche;
    if (pu_ImageDebutZone30 != NULL)
        delete[]pu_ImageDebutZone30;
    if (pu_ImageFinZone30 != NULL)
        delete[]pu_ImageFinZone30;
    if (pu_ImageChicane != NULL)
        delete[]pu_ImageChicane;
    if (pu_ImagePanneauFeu != NULL)
        delete[]pu_ImagePanneauFeu;
    if (pu_ImageFeu != NULL)
        delete[]pu_ImageFeu;
    if (pu_ImageFeuRouge != NULL)
        delete[]pu_ImageFeuRouge;
    if (pu_ImageFeuOrange != NULL)
        delete[]pu_ImageFeuOrange;
    if (pu_ImageFeuVert != NULL)
        delete[]pu_ImageFeuVert;
    if (pu_ImageDanger != NULL)
        delete[]pu_ImageDanger;
    if (pu_ImageStop != NULL)
        delete[]pu_ImageStop;
    if (pu_ImageVitesse90 != NULL)
        delete[]pu_ImageVitesse90;
    if (pu_ImageVitesse50 != NULL)
        delete[]pu_ImageVitesse50;
    if (pu_ImageFinVitesse50 != NULL)
        delete[]pu_ImageFinVitesse50;
	if (pu_ImageMessage != NULL)
        delete[]pu_ImageMessage;
	//Meteo:
	if (pu_ImageMeteoNA != NULL)
        delete[]pu_ImageMeteoNA;
	if (pu_ImageMeteojour != NULL)
        delete[]pu_ImageMeteojour;
	if (pu_ImageMeteonuit != NULL)
        delete[]pu_ImageMeteonuit;
	if (pu_ImageMeteochaud != NULL)
        delete[]pu_ImageMeteochaud;
	if (pu_ImageMeteopluiejour != NULL)
        delete[]pu_ImageMeteopluiejour;
	if (pu_ImageMeteopluienuit != NULL)
        delete[]pu_ImageMeteopluienuit;
	if (pu_ImageMeteoverglas != NULL)
        delete[]pu_ImageMeteoverglas;



    /*if(GetBoolProperty("pAffichageZonesFiltrage"))
        delete po_stereoIR;*/
    if(x1!=NULL)
        delete x1;
    if(x2!=NULL)
        delete x2;
    if(y1!=NULL)
        delete y1;
    if(y2!=NULL)
        delete y2;
}
