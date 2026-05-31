////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_LivicFiltrePistesIR.h"	// Includes the header of this component
#include <stdlib.h>
#include <math.h>

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicFiltrePistesIR)
    MAPS_INPUT("iPistesIR",MAPS::FilterRealObjects,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicFiltrePistesIR)
    MAPS_OUTPUT("oPistesIR",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicFiltrePistesIR)
	MAPS_PROPERTY("pFrequenceCoupure",5.,false,true)
	MAPS_PROPERTY("pFiltreX",true,false,true)
	MAPS_PROPERTY("pFiltreY",true,false,true)
	MAPS_PROPERTY("pFiltreZ",true,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicFiltrePistesIR)
    //MAPS_ACTION("aName",MAPSLivicFiltrePistesIR::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicFiltrePistesIR) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicFiltrePistesIR,"LivicFiltrePistesIR","$Revision: 1.4 $",128,
			  MAPS::Threaded,MAPS::Threaded,
			  1, // Nb of inputs
			  1, // Nb of outputs
			  4, // Nb of properties
			  0) // Nb of actions

void MAPSLivicFiltrePistesIR::Birth()
{
    xt_oldTimestamp = MAPS::CurrentTime();
}

void MAPSLivicFiltrePistesIR::Core() 
{
    // initialisation variables locales
    MAPSTimestamp timestamp = MAPS::CurrentTime();
    MAPSRealObject *iPistesIR = NULL, *oPisteIR = NULL;
    int nbPistesIR = 0;
    int nbOldEntree = xo_oldEntree.nbElements();
    bool *traitee = NULL;
    char msg[256];
    
    // lecture des pistes en entree
    if( IsConnected(Input("iPistesIR")) )
    {
        MAPSIOElt *iEltPistesIR = StartReading(Input("iPistesIR"));
        iPistesIR = &iEltPistesIR->RealObject();
        nbPistesIR = iEltPistesIR->VectorSize();
        timestamp = iEltPistesIR->Timestamp();
    }

    // calcul du temps de boucle
    MAPSFloat tempsBoucle = (MAPSFloat)(timestamp - xt_oldTimestamp) / 1000000.;

    // ecriture des pistes en sortie
    MAPSIOElt *oEltPisteIR = StartWriting(Output("oPistesIR"));
    MAPSRealObject *oPistesIR = &oEltPisteIR->RealObject();
    oEltPisteIR->VectorSize() = nbPistesIR;
    oEltPisteIR->Timestamp() = timestamp;

    // recopie des pistes entrees dans les pistes sorties
    for(int i=0; i<nbPistesIR; i++)
    {
        LivicMapsUtils::copieRealObject( &iPistesIR[i], &oPistesIR[i] );
    }
    
    // initialisation d'un tableau disant si une piste des sauvegardes interne est morte ou pas
    traitee = (bool*)malloc(xo_oldEntree.nbElements()*sizeof(bool));
    for(int i=0; i<nbOldEntree; i++) traitee[i] = false;

    bool trouve = false;
    // parcours des pistes entrees
    for(int i=0; i<nbPistesIR; i++)
    {
        // recherche dans les pistes oldEntree
        for(int oi=0; oi<nbOldEntree; oi++)
        {
            MAPSRealObject *oldEntree = (MAPSRealObject*)(xo_oldEntree[oi]);
            MAPSRealObject *oldSortie = (MAPSRealObject*)(xo_oldSortie[oi]);
            // si trouvee, filtrage
            if( oldEntree->misc1 == iPistesIR[i].misc1 )
            {
                //sprintf(msg, "trouvé %d", iPistesIR[i].misc1);
                //ReportInfo(msg);

                if( GetBoolProperty("pFiltreX") )
                    oPistesIR[i].x = filtre( iPistesIR[i].x, oldEntree->x, oldSortie->x, tempsBoucle );
                oldEntree->x = iPistesIR[i].x;
                oldSortie->x = oPistesIR[i].x;

                if( GetBoolProperty("pFiltreY") )
                    oPistesIR[i].y = filtre( iPistesIR[i].y, oldEntree->y, oldSortie->y, tempsBoucle );
                oldEntree->y = iPistesIR[i].y;
                oldSortie->y = oPistesIR[i].y;

                if( GetBoolProperty("pFiltreZ") )
                    oPistesIR[i].z = filtre( iPistesIR[i].z, oldEntree->z, oldSortie->z, tempsBoucle );
                oldEntree->z = iPistesIR[i].z;
                oldSortie->z = oPistesIR[i].z;

                traitee[oi] = true;
                trouve = true;
                break;
            }
        }
        if( !trouve )
        {
            sprintf(msg, "nouvelle %d", iPistesIR[i].misc1);
            ReportInfo(msg);

            MAPSRealObject *roEntree = (MAPSRealObject*)malloc(sizeof(MAPSRealObject));
            LivicMapsUtils::copieRealObject(&iPistesIR[i], roEntree);
            xo_oldEntree.empile(roEntree);
            MAPSRealObject *roSortie = (MAPSRealObject*)malloc(sizeof(MAPSRealObject));
            LivicMapsUtils::copieRealObject(&oPistesIR[i], roSortie);
            xo_oldSortie.empile(roSortie);
        }
    }

    // elimination des pistes mortes
    for(int oi=0; oi<nbOldEntree; oi++)
    {
        if( !traitee[oi] )
        {
            MAPSRealObject *oldEntree = (MAPSRealObject*)(xo_oldEntree[oi]);
            MAPSRealObject *oldSortie = (MAPSRealObject*)(xo_oldSortie[oi]);

            sprintf(msg, "efface %d", oldEntree->misc1);
            ReportInfo(msg);

            free(oldEntree);
            free(oldSortie);
            xo_oldEntree.effaceIndice(oi);
            xo_oldSortie.effaceIndice(oi);
        }
    }

    // mise a jour oldTimestamp
    xt_oldTimestamp = timestamp;

    if( IsConnected(Input("iPistesIR")) ) StopReading(Input("iPistesIR"));
    StopWriting(oEltPisteIR);

    free(traitee);
}

void MAPSLivicFiltrePistesIR::Death()
{
}

MAPSFloat MAPSLivicFiltrePistesIR::filtre(MAPSFloat x, MAPSFloat oldx, MAPSFloat oldxfiltre, MAPSFloat tempsBoucle)
{
    MAPSFloat Wac = tan( PI * tempsBoucle * GetFloatProperty("pFrequenceCoupure") );
	return (Wac * (x+oldx) - ((Wac-1)*oldxfiltre))/(Wac+1);
}
