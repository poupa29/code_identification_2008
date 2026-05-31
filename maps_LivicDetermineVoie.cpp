////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_LivicDetermineVoie.h"	// Includes the header of this component
#include "IR.h"

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicDetermineVoie)
    MAPS_INPUT("iPistesIRVehicule",MAPS::FilterRealObjects,MAPS::FifoReader)
    MAPS_INPUT("iPistesIRCamEntree",MAPS::FilterRealObjects,MAPS::FifoReader)
    MAPS_INPUT("iPistesIRCamSortie",MAPS::FilterRealObjects,MAPS::FifoReader)
    MAPS_INPUT("iPistesIRStereo",MAPS::FilterRealObjects,MAPS::FifoReader)
    MAPS_INPUT("iZoneVoiesStatiques",MAPS::FilterIntegers,MAPS::SamplingReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicDetermineVoie)
    MAPS_OUTPUT("oPistesIRVehicule",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oPistesIRCamEntree",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oPistesIRCamSortie",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oPistesIRStereo",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicDetermineVoie)
    //MAPS_PROPERTY("pName",128,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicDetermineVoie)
    //MAPS_ACTION("aName",MAPSLivicDetermineVoie::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicDetermineVoie) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicDetermineVoie,"LivicDetermineVoie","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  5, // Nb of inputs
			  4, // Nb of outputs
			  0, // Nb of properties
			  0) // Nb of actions

void MAPSLivicDetermineVoie::Birth()
{
}

void MAPSLivicDetermineVoie::Core() 
{
    MAPSIOElt *iEltPistesIRVehicule = StartReading(Input("iPistesIRVehicule"));
    MAPSRealObject *iPistesIRVehicule = &iEltPistesIRVehicule->RealObject();
    int nbPistesIR = iEltPistesIRVehicule->VectorSize();
    MAPSTimestamp ts = iEltPistesIRVehicule->Timestamp();
    MAPSIOElt *oEltPistesIRVehicule = StartWriting(Output("oPistesIRVehicule"));
    MAPSRealObject *oPistesIRVehicule = &oEltPistesIRVehicule->RealObject();
    oEltPistesIRVehicule->Timestamp() = ts;
    oEltPistesIRVehicule->VectorSize() = nbPistesIR;

    MAPSIOElt *iEltPistesIRCamEntree = StartReading(Input("iPistesIRCamEntree"));
    MAPSRealObject *iPistesIRCamEntree = &iEltPistesIRCamEntree->RealObject();
    MAPSIOElt *oEltPistesIRCamEntree = StartWriting(Output("oPistesIRCamEntree"));
    MAPSRealObject *oPistesIRCamEntree = &oEltPistesIRCamEntree->RealObject();
    oEltPistesIRCamEntree->Timestamp() = ts;
    oEltPistesIRCamEntree->VectorSize() = nbPistesIR;

    MAPSIOElt *iEltPistesIRCamSortie = StartReading(Input("iPistesIRCamSortie"));
    MAPSRealObject *iPistesIRCamSortie = &iEltPistesIRCamSortie->RealObject();
    MAPSIOElt *oEltPistesIRCamSortie = StartWriting(Output("oPistesIRCamSortie"));
    MAPSRealObject *oPistesIRCamSortie = &oEltPistesIRCamSortie->RealObject();
    oEltPistesIRCamSortie->Timestamp() = ts;
    oEltPistesIRCamSortie->VectorSize() = nbPistesIR;

    MAPSIOElt *iEltPistesIRStereo = StartReading(Input("iPistesIRStereo"));
    MAPSRealObject *iPistesIRStereo = &iEltPistesIRStereo->RealObject();
    MAPSIOElt *oEltPistesIRStereo = StartWriting(Output("oPistesIRStereo"));
    MAPSRealObject *oPistesIRStereo = &oEltPistesIRStereo->RealObject();
    oEltPistesIRStereo->Timestamp() = ts;
    oEltPistesIRStereo->VectorSize() = nbPistesIR;

    int nbVoies = 0;
    MAPSInteger *iZone = NULL;
    if( DataAvailableInFIFO(Input("iZoneVoiesStatiques")) )
    {
        MAPSIOElt *iEltZone = StartReading(Input("iZoneVoiesStatiques"));
        nbVoies = iEltZone->VectorSize()/4;
        iZone = &(iEltZone->Integer());
    }

    for(int p=0; p<nbPistesIR; p++)
    {
        LivicMapsUtils::copieRealObject(&iPistesIRVehicule[p], &oPistesIRVehicule[p]);
        LivicMapsUtils::copieRealObject(&iPistesIRCamEntree[p], &oPistesIRCamEntree[p]);
        LivicMapsUtils::copieRealObject(&iPistesIRCamSortie[p], &oPistesIRCamSortie[p]);
        LivicMapsUtils::copieRealObject(&iPistesIRStereo[p], &oPistesIRStereo[p]);

        int fvoie = -1;
        for(int voie=0; voie<nbVoies; voie++)
        {
            MAPSFloat upiste = iPistesIRCamEntree[p].x;
            MAPSFloat vpiste = iPistesIRCamEntree[p].y - iPistesIRCamEntree[p].vehicle.height;
            MAPSFloat vsegment = vpiste;
            MAPSFloat x1 = iZone[0*nbVoies+voie];
            MAPSFloat y1 = iZone[1*nbVoies+voie];
            MAPSFloat x2 = iZone[2*nbVoies+voie];
            MAPSFloat y2 = iZone[3*nbVoies+voie];
            MAPSFloat a = (y1-y2) / (x1-x2);
            MAPSFloat usegment = ((vsegment-y1) / a) + x1;
            if( upiste > usegment ) fvoie = voie;
        }

        oPistesIRVehicule[p].misc3 = fvoie;
        oPistesIRCamEntree[p].misc3 = fvoie;
        oPistesIRCamSortie[p].misc3 = fvoie;
        oPistesIRStereo[p].misc3 = fvoie;
    }

    if( DataAvailableInFIFO(Input("iZoneVoiesStatiques")) ) StopReading(Input("iZoneVoiesStatiques"));
    StopReading(Input("iPistesIRVehicule"));
    StopReading(Input("iPistesIRCamEntree"));
    StopReading(Input("iPistesIRCamSortie"));
    StopReading(Input("iPistesIRStereo"));
    StopWriting(oEltPistesIRVehicule);
    StopWriting(oEltPistesIRCamEntree);
    StopWriting(oEltPistesIRCamSortie);
    StopWriting(oEltPistesIRStereo);
}

void MAPSLivicDetermineVoie::Death()
{
}
