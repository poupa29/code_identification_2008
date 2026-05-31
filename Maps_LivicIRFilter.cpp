////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////
// This is the simplest rt-maps component : it has no inputs, outputs, properties 
// nor actions (as you can see below). The CORE of the module just prints some
// information to the console and makes a pause (it sleeps during 500 ms)

#include "MAPS_LivicIRFilter.h"   // Includes the header of this component
//#include "../commun/tarel/image.h"
//#include "../commun/tarel/tools.h"

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicIRFilter)
    //MAPS_INPUT("iImage",MAPS::FilterIplImage,MAPS::FifoReader)
    /*MAPS_INPUT("iImageDroite", MAPS::FilterIplImage, MAPS::FifoReader)
    MAPS_INPUT("iImageGauche", MAPS::FilterIplImage, MAPS::FifoReader)
    MAPS_INPUT("iParametresCameras",MAPSFilterStructureParametresCameras, MAPS::SamplingReader)*/
    /*MAPS_INPUT("iTangageVehicule", MAPS::FilterFloat, MAPS::FifoReader)
    MAPS_INPUT("iVariationHauteurVehicule", MAPS::FilterFloat, MAPS::FifoReader)*/
    MAPS_INPUT("iPistesIR",MAPS::FilterRealObjects,MAPS::FifoReader)
    MAPS_INPUT("iMessages",MAPS::FilterStream8,MAPS::FifoReader)
    MAPS_INPUT("iZoneFiltrageMessages",MAPS::FilterFloats, MAPS::SamplingReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicIRFilter)
    MAPS_OUTPUT("oMessage",MAPS::Stream8,NULL,NULL,TAILLE_MSG)
    MAPS_OUTPUT("oFU", MAPS::Integer, NULL, NULL, 1)
    MAPS_OUTPUT("oMessageFiltre", MAPS::Integer, NULL, NULL, 1)
    MAPS_OUTPUT("oTY", MAPS::Integer, NULL, NULL, 1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicIRFilter)
    MAPS_PROPERTY("pTempsEnvoiFu", 1000, false, true)
    /*MAPS_PROPERTY("pDemieTailleFenetreV", 1, false, true)
    MAPS_PROPERTY("pSeuilGradient", 8, false, true)
    MAPS_PROPERTY("pSeuilScoreCorrelationRoute", 20.0, false, true)
    MAPS_PROPERTY("pSeuilScoreCorrelationObstacle", 20.0, false, true)*/
    /*MAPS_PROPERTY("pSeuilAlignementVertical", 12, false, true)
    MAPS_PROPERTY("pLargeurVoiture_m", 3.0, false, true)
    MAPS_PROPERTY("pHauteurVoiture_m", 1.8, false, true)
    MAPS_PROPERTY("pProfondeurVoiture_m", 3.0, false, true)
    MAPS_PROPERTY("pConfirmerPistesIR", false, false, true)
    MAPS_PROPERTY("pAffinerDetectionsStereo", false, false, true)
    MAPS_PROPERTY("pDistanceMinAssociationPistes_m", 1.0, false, true)
    MAPS_PROPERTY("pFiltrerMessages", false, false, true)*/
    //MAPS_PROPERTY_ENUM("pTypeImage","N&B|Couleur", 0, false,false)
    //MAPS_PROPERTY("pName",128,false,false)
    MAPS_PROPERTY("pFiltrage", true, false, true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicIRFilter)
    //MAPS_ACTION("aName",MAPSLivicIRFilter::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicIRFilter) behaviour
MAPS_COMPONENT_DEFINITION(
    MAPSLivicIRFilter,
    "LivicIRFilter",
    "$Revision: 1.9 $",
    128,
    MAPS::Threaded,
    MAPS::Threaded,
    3,
    4,
    2,
    0)

void MAPSLivicIRFilter::Birth()
{
     FUPrecedent=0;   
     MAPSIOElt *oEltFU = StartWriting(Output("oFU"));
     oEltFU->Integer()=0;
     StopWriting(oEltFU);

	 FU = 0;
	 timestamp = -1;

}

MAPSTimestamp MAPSLivicIRFilter::mtsp_PistesIRSuivant()
{
    //if( po_pistesIR ) StopReading(Input("iPistesIR"));
    MAPSIOElt *iEltPistesIR = StartReading(Input("iPistesIR"));
    po_pistesIR = &iEltPistesIR->RealObject();
    xi_nbPistesIR = iEltPistesIR->VectorSize();
    return iEltPistesIR->Timestamp();
}


void MAPSLivicIRFilter::mv_RecupZoneFiltrageMessages()
{
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

				pd_PtsGaucheZoneFiltrage[2*xi_NbPtDroiteZoneFiltrage] = Z;
				pd_PtsGaucheZoneFiltrage[2*xi_NbPtDroiteZoneFiltrage+1] = X;
				xi_NbPtGaucheZoneFiltrage ++;


				//	Extraction des points de gauche:
				Z = RoadMarkersVector[Number++];
				X = RoadMarkersVector[Number++];

				pd_PtsDroiteZoneFiltrage[2*xi_NbPtGaucheZoneFiltrage] = Z;
				pd_PtsDroiteZoneFiltrage[2*xi_NbPtGaucheZoneFiltrage+1] = X;
				xi_NbPtDroiteZoneFiltrage ++;
			}

			StopReading(Input("iZoneFiltrageMessages"));
        }


}


bool MAPSLivicIRFilter::mv_FiltrageMessages()
{
	bool estFiltre= true;
    int i;
	for(i=0; i<xi_nbPistesIR; i++)
    {
        if( po_pistesIR[i].id == xo_messageID )
		{
			estFiltre = false;
            break;
		}
        else
            estFiltre = true;
    }
    if (estFiltre) return true;
    
    
    //	On regarde si la piste IR est situee dans la zone de filtrage:
	//	a droite...
	int j=0;
	double xbord, dxbord, dzbord;
	do
	{
		j++;
		if (j>xi_NbPtDroiteZoneFiltrage-2)
			break;
	}
	while (po_pistesIR[i].z>pd_PtsDroiteZoneFiltrage[2*j]);

	//	si la piste est trop lointaine, on la rejette:
	if (j<xi_NbPtDroiteZoneFiltrage-2)
	{
		dxbord = pd_PtsDroiteZoneFiltrage[2*j + 1] - pd_PtsDroiteZoneFiltrage[2*(j-1) + 1];
		dzbord = pd_PtsDroiteZoneFiltrage[2*j] - pd_PtsDroiteZoneFiltrage[2*(j-1)];
		if (dzbord<EPSILON)
			xbord = pd_PtsDroiteZoneFiltrage[2*(j-1) + 1];
		else
			xbord = pd_PtsDroiteZoneFiltrage[2*(j-1) + 1] + dxbord*((po_pistesIR[i].z-pd_PtsDroiteZoneFiltrage[2*(j-1)])/dzbord);

		
		//	si la piste est à gauche du bord droit, on teste le bord gauche
		if (po_pistesIR[i].x<xbord)
		{
			j=0;
			do
			{
				j++;
				if (j>xi_NbPtGaucheZoneFiltrage-2)
					break;
			}
			while (po_pistesIR[i].z>pd_PtsDroiteZoneFiltrage[2*j]);

			//	si la piste est trop lointaine, on la rejette:
			if (j<xi_NbPtGaucheZoneFiltrage-2)
			{
				dxbord = pd_PtsGaucheZoneFiltrage[2*j + 1] - pd_PtsGaucheZoneFiltrage[2*(j-1) + 1];
				dzbord = pd_PtsGaucheZoneFiltrage[2*j] - pd_PtsGaucheZoneFiltrage[2*(j-1)];
				if (dzbord<EPSILON)
					xbord = pd_PtsGaucheZoneFiltrage[2*(j-1) + 1];
				else
					xbord = pd_PtsGaucheZoneFiltrage[2*(j-1) + 1] + dxbord*((po_pistesIR[i].z-pd_PtsGaucheZoneFiltrage[2*(j-1)])/dzbord);
			
				//	si la piste est a droite du bord gauche, c'est bon:
				if (po_pistesIR[i].x>xbord)
				{
					return false;							
				}
                else
                    return true;
			}
            else 
                return true;
		}
		//	Sinon, on filtre la piste
		else
		    return true;
	}
    else
        return true;

}
		




void MAPSLivicIRFilter::Core() 
{
    xi_nbPistesIR = 0;
    po_pistesIR = NULL;
    xt_timestampPistesIR = 0;
    xi_nbMessagesInfrastructure = 0;
	xo_messageID = -2;

    // ------------------------------------------------------------------------------------------------
    // lecture des pistes IR

    xt_timestampPistesIR = mtsp_PistesIRSuivant();

    // lecture des pistes IR
    // ------------------------------------------------------------------------------------------------
	/*char txt[200];
	sprintf(txt,"%d",xi_nbPistesIR);
	ReportInfo((const char*)txt);*/
    
    
    /*void *p;
    LivicInt64 val;*/
    
	
    /*bool messageFiltre = false;*/
		
    if( DataAvailableInFIFO(Input("iMessages")) )
		{           
            // ------------------------------------------------------------------------------------------------
            // lecture du message radio
            message = (char*)malloc((TAILLE_MSG+1)*sizeof(char));
			MAPSIOElt *iEltMessages = StartReading(Input("iMessages"));
			char *data=(char*)iEltMessages->Stream8();
			memcpy(message, data, TAILLE_MSG);
			message[TAILLE_MSG] = 0;
			/*xt_timestampMessages = MAPS::CurrentTime();*/
			StopReading(Input("iMessages"));
			
			// id:num:ty:pa1
			char sid[2+1], snum[3+1], sty[2+1], spa1[3+1];
			int id, num, ty, pa1;
			int pos = 0;
			strncpy(sid, message+pos, 2);
			pos += 2+1;
			strncpy(snum, message+pos, 3);
			pos += 3+1;
			strncpy(sty, message+pos, 2);
			pos += 2+1;
			strncpy(spa1, message+pos, 3);
			id = atoi(sid);
			num = atoi(snum);
			ty = atoi(sty);
			pa1 = atoi(spa1);
			
            

           /* if (ty == MSG_TY_FREINAGE)
            {
                FU = 1;                
            }
            else
            {
                FU = 0;
            }*/
        

            xo_messageID = id;

            /*MAPSIOElt *oEltFU = StartWriting(Output("oFU"));
            oEltFU->Integer()=FU;
            StopWriting(oEltFU);*/
			//switch( ty )
			//{
   //         case MSG_TY_FREINAGE : sprintf(message, "%d : Emergency Braking", id); break;
	  //      //case MSG_TY_FREINAGE : sprintf(message, "%d : Freinage a %d", id, pa1); break;
			//case MSG_TY_VIRAGE :  sprintf(message, "%d : Abord d un virage dangereux", id); break;
			//case MSG_TY_ADHERENCE :  sprintf(message, "%d : Perte d'adherence", id); break;
			//case MSG_TY_OBSTACLE :  sprintf(message, "%d : Obstacle detecte", id); break;
   //         case MSG_TY_LOVE :  sprintf(message, "%d : Salut beau brun, appelles moi au 06 11 55 XX", id); break;
			//default : sprintf(message, "%d : Message inconnu", id); break;
			//}
            // lecture du message radio
            // ------------------------------------------------------------------------------------------------


            bool messageFiltre = false;
            // ------------------------------------------------------------------------------------------------
            // recuperation zone filtrage des messages et filtrage du message
            
            if(GetBoolProperty("pFiltrage"))
            {
                if(IsConnected(Input("iZoneFiltrageMessages")))
                {
                    mv_RecupZoneFiltrageMessages();
                    messageFiltre = mv_FiltrageMessages();
                }
            }
               
            // recuperation zone filtrage des messages et filtrage du message
            // ------------------------------------------------------------------------------------------------
            if( !messageFiltre )
            {
                
                if (ty == MSG_TY_FREINAGE)
                {
                    FU = 1;
					timestamp=MAPS::CurrentTime();

                }
                else
                {
                    FU = 0;
                }
                
                /*MAPSIOElt *oEltFU = StartWriting(Output("oFU"));
                oEltFU->Integer()=FU;
                StopWriting(oEltFU);*/

                MAPSIOElt *oEltMessageFiltre = StartWriting(Output("oMessageFiltre"));
                oEltMessageFiltre->Integer()=0;
                StopWriting(oEltMessageFiltre);
            }
            // mise a zero de la sortie du FU si le message est filtré
            else
            {
                /*MAPSIOElt *oEltFU = StartWriting(Output("oFU"));
                oEltFU->Integer()=0;
                StopWriting(oEltFU);*/

                MAPSIOElt *oEltMessageFiltre = StartWriting(Output("oMessageFiltre"));
                oEltMessageFiltre->Integer()=1;
                StopWriting(oEltMessageFiltre);

            }
            MAPSIOElt *oEltMsg = StartWriting(Output("oMessage"));
            memcpy(oEltMsg->Stream8(), message, TAILLE_MSG);
            oEltMsg->VectorSize()=TAILLE_MSG;
            StopWriting(oEltMsg);
			MAPSIOElt *oEltTY = StartWriting(Output("oTY"));
			oEltTY->Integer() = ty;
			StopWriting(oEltTY);
			delete message;
		}
        
		MAPSTimestamp tempsEcoule=MAPS::CurrentTime();

        if( timestamp != -1)
        {
			FU = 1;
		}
            /*MAPSIOElt *oEltFU = StartWriting(Output("oFU"));
            oEltFU->Integer()=FU;
            StopWriting(oEltFU);*/

            /*char* mess;
            mess = (char*) malloc(sizeof(char*));
            sprintf(mess, "FU : %i", FU);
            ReportInfo(( const char*)mess);
            delete mess;*/
		
		if((FU==1)&&(tempsEcoule-timestamp>GetIntegerProperty("pTempsEnvoiFu")*1000))
		{
			//FUPrecedent = FU;
			
			FU=0;
			timestamp = -1;
		}
       
		//if ( (FU==1)&&(timestamp>GetIntegerProperty("pTempsEnvoiFu")*1000)) FU=0;

		
		/*elseif( FU != FUPrecedent )
		{
			MAPSIOElt *oEltFU = StartWriting(Output("oFU"));
			oEltFU->Integer()=FU;
			StopWriting(oEltFU);

			char* mess;
            mess = (char*) malloc(sizeof(char*));
            sprintf(mess, "FU : %i", FU);
            ReportInfo(( const char*)mess);
            delete mess;
			
			FUPrecedent=FU;
		}*/
		if( FU != FUPrecedent )
		{
			MAPSIOElt *oEltFU = StartWriting(Output("oFU"));
			oEltFU->Integer()=FU;
			StopWriting(oEltFU);
			FUPrecedent = FU;
		}
	    

    
    


    // ------------------------------------------------------------------------------------------------
    // Stopreadings et stopwritings
    
    if( IsConnected(Input("iPistesIR")) ) StopReading(Input("iPistesIR"));

    // Stopreadings et stopwritings
    // ------------------------------------------------------------------------------------------------


}

void MAPSLivicIRFilter::Death()
{
      
}
