////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_LivicBaseType2RealObject.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicBaseType2RealObject)
    MAPS_INPUT("iIntegerId",MAPS::FilterInteger,MAPS::FifoReader)
    MAPS_INPUT("iFloatX",MAPS::FilterFloat,MAPS::FifoReader)
    MAPS_INPUT("iFloatY",MAPS::FilterFloat,MAPS::FifoReader)
    MAPS_INPUT("iFloatZ",MAPS::FilterFloat,MAPS::FifoReader)
    MAPS_INPUT("iIntegerEtatPiste",MAPS::FilterInteger,MAPS::FifoReader)
    MAPS_INPUT("iIntegerNumeroPiste",MAPS::FilterInteger,MAPS::FifoReader)
    MAPS_INPUT("iFloatVitesse",MAPS::FilterFloat,MAPS::FifoReader)
    MAPS_INPUT("iFloatHauteur",MAPS::FilterFloat,MAPS::FifoReader)
    MAPS_INPUT("iIntegerVoie",MAPS::FilterInteger,MAPS::FifoReader)
    MAPS_INPUT("iIntegerSignConfirm",MAPS::FilterInteger,MAPS::FifoReader)
    MAPS_INPUT("iVectorSize",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicBaseType2RealObject)
    MAPS_OUTPUT("oRealObject",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicBaseType2RealObject)
    //MAPS_PROPERTY("pName",128,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicBaseType2RealObject)
    //MAPS_ACTION("aName",MAPSLivicBaseType2RealObject::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicBaseType2RealObject) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicBaseType2RealObject,"LivicBaseType2RealObject","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  11, // Nb of inputs
			  1, // Nb of outputs
			  0, // Nb of properties
			  0) // Nb of actions

void MAPSLivicBaseType2RealObject::Birth()
{
}

void MAPSLivicBaseType2RealObject::Core() 
{
    MAPSIOElt *iEltIntegerId = StartReading(Input("iIntegerId"));
    MAPSTimestamp timestamp = iEltIntegerId->Timestamp();
    MAPSIOElt *iEltFloatX = StartReading(Input("iFloatX"));
    MAPSIOElt *iEltFloatY = StartReading(Input("iFloatY"));
    MAPSIOElt *iEltVectorSize = StartReading(Input("iVectorSize"));

    MAPSInteger *iIntegerId = &(iEltIntegerId->Integer());
    MAPSFloat *iFloatX = &(iEltFloatX->Float());
    MAPSFloat *iFloatY = &(iEltFloatY->Float());
    MAPSFloat *iFloatZ;
    MAPSInteger *iIntegerEtatPiste;
    MAPSInteger *iIntegerNumeroPiste;
    MAPSFloat *iFloatVitesse;
    MAPSFloat *iFloatHauteur;
    MAPSInteger *iIntegerVoie;
    MAPSInteger *iIntegerSignConfirm;
    
    if( IsConnected(Input("iFloatZ")) )
    {
        MAPSIOElt *iEltFloatZ = StartReading(Input("iFloatZ"));
        iFloatZ = &(iEltFloatZ->Float());
    }
    if( IsConnected(Input("iIntegerEtatPiste")) )
    {
        MAPSIOElt *iEltIntegerEtatPiste = StartReading(Input("iIntegerEtatPiste"));
        iIntegerEtatPiste = &(iEltIntegerEtatPiste->Integer());
    }
    if( IsConnected(Input("iIntegerNumeroPiste")) )
    {
        MAPSIOElt *iEltIntegerNumeroPiste = StartReading(Input("iIntegerNumeroPiste"));
        iIntegerNumeroPiste = &(iEltIntegerNumeroPiste->Integer());
    }
    if( IsConnected(Input("iFloatVitesse")) )
    {
        MAPSIOElt *iEltFloatVitesse = StartReading(Input("iFloatVitesse"));
        iFloatVitesse = &(iEltFloatVitesse->Float());
    }
    if( IsConnected(Input("iFloatHauteur")) )
    {
        MAPSIOElt *iEltFloatHauteur = StartReading(Input("iFloatHauteur"));
        iFloatHauteur = &(iEltFloatHauteur->Float());
    }
	if( IsConnected(Input("iIntegerVoie")) )
    {
        MAPSIOElt *iEltIntegerVoie = StartReading(Input("iIntegerVoie"));
        iIntegerVoie = &(iEltIntegerVoie->Integer());
    }
	if( IsConnected(Input("iIntegerSignConfirm")) )
    {
        MAPSIOElt *iEltIntegerSignConfirm = StartReading(Input("iIntegerSignConfirm"));
        iIntegerSignConfirm = &(iEltIntegerSignConfirm->Integer());
    }
    MAPSInteger *iVectorSize = &(iEltVectorSize->Integer());
    int vsize = *iVectorSize;
    MAPSIOElt *oEltRealObject = StartWriting(Output("oRealObject"));
    MAPSRealObject *oRealObject = &(oEltRealObject->RealObject());
    oEltRealObject->VectorSize() = vsize;
    oEltRealObject->Timestamp() = timestamp;

    for(int i=0; i<vsize; i++)
    {
        oRealObject[i].id = iIntegerId[i];
        oRealObject[i].x = iFloatX[i];
        oRealObject[i].y = iFloatY[i];
        if( IsConnected(Input("iFloatZ")))
            oRealObject[i].z = iFloatZ[i];
        if( IsConnected(Input("iIntegerEtatPiste")) )
            oRealObject[i].misc2 = iIntegerEtatPiste[i];
        if( IsConnected(Input("iIntegerNumeroPiste")) )
            oRealObject[i].misc1 = iIntegerNumeroPiste[i];
		if( IsConnected(Input("iFloatVitesse")))
			oRealObject[i].vehicle.speed = iFloatVitesse[i];
		if( IsConnected(Input("iFloatHauteur")))
			oRealObject[i].vehicle.height = iFloatHauteur[i];
        if( IsConnected(Input("iIntegerVoie")) )
            oRealObject[i].misc3 = iIntegerVoie[i];
        if( IsConnected(Input("iIntegerSignConfirm")) )
            oRealObject[i].sign.type = iIntegerSignConfirm[i];
    }

    StopReading(Input("iIntegerId"));
    StopReading(Input("iFloatX"));
    StopReading(Input("iFloatY"));
    if( IsConnected(Input("iFloatZ")))
        StopReading(Input("iFloatZ"));
    if( IsConnected(Input("iIntegerEtatPiste")) )
        StopReading(Input("iIntegerEtatPiste"));
    if( IsConnected(Input("iIntegerNumeroPiste")) )
        StopReading(Input("iIntegerNumeroPiste"));
	if( IsConnected(Input("iFloatVitesse")) )
        StopReading(Input("iFloatVitesse"));
	if( IsConnected(Input("iFloatHauteur")) )
        StopReading(Input("iFloatHauteur"));
	if( IsConnected(Input("iIntegerVoie")) )
        StopReading(Input("iIntegerVoie"));
	if( IsConnected(Input("iIntegerSignConfirm")) )
        StopReading(Input("iIntegerSignConfirm"));
    StopReading(Input("iVectorSize"));
    StopWriting(oEltRealObject);
}

void MAPSLivicBaseType2RealObject::Death()
{
}
