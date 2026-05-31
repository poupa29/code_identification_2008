/******************************************************************************

  (c) LIVIC 2005
  
	Ce fichier est la propriete du LIVIC. Toute utilisation, copie partielle ou
	totale, modification du fichier sans autorisation du LIVIC est interdite.
	
	  Fichier          : $RCSfile: maps_LivicIHMIdentification.cpp,v $
	  Date de creation : 2005/10/20
	  Auteur           : Mathias PERROLLAZ
	  Modifie le       : $Date: 2006/09/22 16:23:22 $ par $Author: perrollaz $
	  Version          : $Version: 1.0 $
	  
		Cadence:
		Cadence des entrées images
		
		  Entrees:
		  iImageGauche:         image gauche issue du système de stéréovision
		  iImageDroite:         image droite issue du système de stéréovision
		  iHorizon:             position en z de la ligne d'horizon dans les images stéréo
		  iPenteRoute:          pente du profil de la route en v-disparité
		  iParametresCamera:    Parametres du capteur stéréoscopique
		  iDetectionIR:         cibles données par le capteur IR
		  iMessage:             message donné par la radio à associer à une cible IR
          iZoneFiltrageMessages:zone de filtrage des messages
		  
			Sorties:
			Images diverses
			
			  Proprietes:
			  
				Actions:
				
				  Commentaire:
				  A partir des cibles IR, détecte les obstacles, affine un cadre englobant
				  autour et associe les messages radio aux obstacles détectés.
				  
*******************************************************************************/


#include "Maps_LivicIHMIdentification.h"


//  **********************************************************************************************
//                              ---- DEFINITION DU COMPOSANT ----
//  **********************************************************************************************

//  ------------------------------------------------------
//  Définition des entrées
//  ------------------------------------------------------
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicIHMIdentification)
    MAPS_INPUT("iIDmessage", MAPS::FilterInteger, MAPS::FifoReader)
    MAPS_INPUT("iMessage", MAPS::FilterStream8, MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

//  ------------------------------------------------------
//  Définition des sorties
//  ------------------------------------------------------
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicIHMIdentification)
    MAPS_OUTPUT("oIHMIdentification", MAPS::IplImage,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION


//  ------------------------------------------------------
//  Définition des sorties
//  ------------------------------------------------------
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicIHMIdentification)
MAPS_END_PROPERTIES_DEFINITION


//  -------------------------------------------------------
//  Définition des actions
//  -------------------------------------------------------
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicIHMIdentification)
MAPS_END_ACTIONS_DEFINITION


//  -------------------------------------------------------
//  Définition du composant
//  -------------------------------------------------------
MAPS_COMPONENT_DEFINITION(MAPSLivicIHMIdentification,
                          "LivicIHMIdentification",
                          "Version 0.1",
                          128,
                          MAPS::Sequential|MAPS::Threaded,
                          MAPS::Threaded,
                          2,
                          1,
                          0,
                          0)
						  
						  
						  
//**********************************************************************************************
//                              ---- METHODE Birth() ----
//**********************************************************************************************

void MAPSLivicIHMIdentification::Birth()
{
    xb_firstTime            = true;
    pu_ImageVirageDroite    = NULL;
    pu_ImageVirageGauche    = NULL;
    pu_ImageDebutZone30     = NULL;
    pu_ImageFinZone30       = NULL;
    pu_ImageChicane         = NULL;
    pu_ImagePanneauFeu      = NULL;
    pu_ImageFeu             = NULL;
    pu_ImageDanger          = NULL;
    pu_ImageMessage         = NULL;
    pu_ImageIHMIdentification   = NULL;
    pu_ImageStop            = NULL;
}


//**********************************************************************************************
//                              ---- METHODE Core() ----
//**********************************************************************************************

void MAPSLivicIHMIdentification::Core() 
{
    try
    {
		//  =============================================================================
		//                      ----- LECTURE SUR LES ENTREES -----
		//  =============================================================================

        MAPSIOElt *IOEltTypeImage = StartReading(Input("iIDmessage"));
        xi_IDmessage = IOEltTypeImage->Integer();
        MAPSTimestamp iTimestamp = IOEltTypeImage->Timestamp();
        StopReading(Input("iIDmessage"));



        //  =============================================================================
        //                  ----------  INITIALISATIONS ---------------
        //  =============================================================================
        if(xb_firstTime)
        {
            //  Allocation de l'image de sortie:
            IplImage ModeleIHM_BGR  = MAPS::IplImageModel(DIM_IMAGES_PANNEAUX, DIM_IMAGES_PANNEAUX+80, MAPS_CHANNELSEQ_BGR);
            Output(0).AllocOutputBufferIplImage(ModeleIHM_BGR);

            //chargement des imagettes IHM:
            pu_ImageVirageDroite    = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageVirageGauche    = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageDebutZone30     = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageFinZone30       = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageChicane         = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImagePanneauFeu      = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageFeu             = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageDanger          = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageMessage         = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageStop            = new unsigned char [DIM_IMAGES_PANNEAUX*DIM_IMAGES_PANNEAUX*3];
            pu_ImageIHMIdentification   = pu_ImageVirageDroite;

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
            bmp = ImageBMP("R:/maps/perception/identification/images/Feu.bmp");
            statut = bmp.LectureImage(pu_ImageFeu,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/Danger.bmp");
            statut = bmp.LectureImage(pu_ImageDanger,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/Message.bmp");
            statut = bmp.LectureImage(pu_ImageMessage,true);
            bmp = ImageBMP("R:/maps/perception/identification/images/Stop.bmp");
            statut = bmp.LectureImage(pu_ImageStop,true);


            //  --- fin des initialisations: ---
            xb_firstTime = false;
        }

	
        //  ----------------------------------------------------
        //  Recuperation du message à ecrire
        //  ----------------------------------------------------
        MAPSIOElt *iIOEltMessage;

		if( DataAvailableInFIFO(Input("iMessage")) )
		{
			iIOEltMessage = StartReading(Input("iMessage"));
			char *data=(char*)iIOEltMessage->Stream8();
			strcpy(pc_message, data);
			StopReading(Input("iMessages"));
        }
        else
        {
            strcpy(pc_message, "");
        }



        //  ----------------------------------------------------------
        //  Choix de l'image à afficher
        //  ----------------------------------------------------------
        int typeMessage = typeIdentifiant(xi_IDmessage);
        if (typeMessage == TYPE_ID_VEHICULE)
        {
            pu_ImageIHMIdentification = pu_ImageMessage;
            xo_couleur = cvScalar(250,100,10);
        }
        else if (typeMessage == TYPE_ID_INFRA)
        {
            switch (xi_IDmessage)
            {
            case ID_PANNEAU_DEBUT_30:
                {
                    pu_ImageIHMIdentification = pu_ImageDebutZone30;
                    break;
                }
            case ID_PANNEAU_FIN_30:
                {
                    pu_ImageIHMIdentification = pu_ImageFinZone30;
                    break;          
                }
            case ID_PANNEAU_VIRAGE_GAUCHE:
                {
                    pu_ImageIHMIdentification = pu_ImageVirageGauche;
                    break;          
                }
            case ID_PANNEAU_CHICANE:
                {
                    pu_ImageIHMIdentification = pu_ImageChicane;
                    break;
                }
            case ID_STOP:
                {
                    pu_ImageIHMIdentification = pu_ImageStop;
                    break;          
                }
            default:
                {
                    pu_ImageIHMIdentification = pu_ImageStop;
                    break;          
                }
            }
            xo_couleur = cvScalar(10,30,255);
        }
        else
            pu_ImageIHMIdentification = NULL;
		
		
        //  =============================================================================
        //                  -------- ECRITURE DES IMAGES DE SORTIE: --------
        //  =============================================================================
		
            MAPSIOElt *oIOEltIHMIdentification          = StartWriting(Output("oIHMIdentification"));
            IplImage &oIPLImageIHMIdentification        = oIOEltIHMIdentification->IplImage();
            oIOEltIHMIdentification->Timestamp()        = iTimestamp;

            if (pu_ImageIHMIdentification!=NULL)
                LvCopieRectBufferUC(&oIPLImageIHMIdentification, pu_ImageIHMIdentification, DIM_IMAGES_PANNEAUX, 3, cvRect(0,0,DIM_IMAGES_PANNEAUX,DIM_IMAGES_PANNEAUX), cvPoint(0,80));
            else
                cvSet( &oIPLImageIHMIdentification, cvScalar(255,255,255), NULL);

            CvFont Font;
            cvInitFont(&Font, CV_FONT_HERSHEY_PLAIN, 0.7, 0.7, 0, 1);
            cvPutText( &oIPLImageIHMIdentification, pc_message, cvPoint(2,15), &Font, xo_couleur );		
    }
	
	
	
    //  =============================================================================
    //                  -------- GESTION DES EXCEPTIONS: --------
    //  =============================================================================
    catch(LivicStereoErreur es)
    {
        ReportError(es.MessageErreur().data());
        es.SauvegardeContexte(std::string("debugIHMIdentification.log"));
        CommitSuicide();
    }
    catch(LivicErreurDivisionParZero edz)
    {
        ReportError(edz.MessageErreur().data());
        edz.SauvegardeContexte(std::string("debugIHMIdentification.log"));
        CommitSuicide();
    }
    catch(LivicErreur e)
    {
        ReportError(e.MessageErreur().data());
        e.SauvegardeContexte(std::string("debugIHMIdentification.log"));
    }
}



//  **********************************************************************************************
//                                  ---- METHODE Death() ----
//  **********************************************************************************************

void MAPSLivicIHMIdentification::Death()
{
    if (pu_ImageVirageDroite!=NULL)
		delete[]pu_ImageVirageDroite;	
    if (pu_ImageVirageGauche == NULL)
        delete[]pu_ImageVirageGauche;
    if (pu_ImageDebutZone30 == NULL)
        delete[]pu_ImageDebutZone30;
    if (pu_ImageFinZone30 == NULL)
        delete[]pu_ImageFinZone30;
    if (pu_ImageChicane == NULL)
        delete[]pu_ImageChicane;
    if (pu_ImagePanneauFeu == NULL)
        delete[]pu_ImagePanneauFeu;
    if (pu_ImageFeu == NULL)
        delete[]pu_ImageFeu;
    if (pu_ImageDanger == NULL)
        delete[]pu_ImageDanger;
    if (pu_ImageMessage == NULL)
        delete[]pu_ImageMessage;

}

