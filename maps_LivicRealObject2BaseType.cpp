////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_LivicRealObject2BaseType.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicRealObject2BaseType)
    MAPS_INPUT("iRealObject",MAPS::FilterRealObjects,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicRealObject2BaseType)
    MAPS_OUTPUT("oIntegerId",MAPS::Integer,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oFloatX",MAPS::Float,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oFloatY",MAPS::Float,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oFloatZ",MAPS::Float,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oIntegerEtatPiste",MAPS::Integer,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oIntegerNumeroPiste",MAPS::Integer,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oFloatVitesse",MAPS::Float,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oFloatHauteur",MAPS::Float,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oFloatLargeur",MAPS::Float,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oFloatProfondeur",MAPS::Float,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oIntegerVoie",MAPS::Integer,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oIntegerSignConfirm",MAPS::Integer,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oVectorSize",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicRealObject2BaseType)
    //MAPS_PROPERTY("pName",128,false,false)
    MAPS_PROPERTY("pMaxPistes",20,false,false)
    MAPS_PROPERTY("pFiltrePiste",false,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicRealObject2BaseType)
    //MAPS_ACTION("aName",MAPSLivicRealObject2BaseType::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicRealObject2BaseType) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicRealObject2BaseType,"LivicRealObject2BaseType","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  1, // Nb of inputs
			  13, // Nb of outputs
			  2, // Nb of properties
			  0) // Nb of actions

void MAPSLivicRealObject2BaseType::Birth()
{
}

void MAPSLivicRealObject2BaseType::Core() 
{

    int MaxPistes = GetIntegerProperty("pMaxPistes");
    bool FiltrePiste = GetBoolProperty("pFiltrePiste");
    //MAPSStreamedString toto("i: ");

    MAPSIOElt *iEltRealObject = StartReading(Input("iRealObject"));
    MAPSRealObject *iRealObject = &(iEltRealObject->RealObject());
    MAPSTimestamp timestamp = iEltRealObject->Timestamp();

    if((!FiltrePiste )||(FiltrePiste && (iEltRealObject->VectorSize())) )
    {

        MAPSIOElt *oEltIntegerId = StartWriting(Output("oIntegerId"));
        MAPSIOElt *oEltFloatX = StartWriting(Output("oFloatX"));
        MAPSIOElt *oEltFloatY = StartWriting(Output("oFloatY"));
        MAPSIOElt *oEltFloatZ = StartWriting(Output("oFloatZ"));
        MAPSIOElt *oEltIntegerNumeroPiste = StartWriting(Output("oIntegerNumeroPiste"));
        MAPSIOElt *oEltIntegerEtatPiste = StartWriting(Output("oIntegerEtatPiste"));   
        MAPSIOElt *oEltVectorSize = StartWriting(Output("oVectorSize"));
        MAPSIOElt *oEltFloatVitesse = StartWriting(Output("oFloatVitesse"));
        MAPSIOElt *oEltFloatHauteur = StartWriting(Output("oFloatHauteur"));
        MAPSIOElt *oEltFloatLargeur = StartWriting(Output("oFloatLargeur"));
        MAPSIOElt *oEltFloatProfondeur = StartWriting(Output("oFloatProfondeur"));
        MAPSIOElt *oEltIntegerVoie = StartWriting(Output("oIntegerVoie"));
        MAPSIOElt *oEltIntegerSignConfirm = StartWriting(Output("oIntegerSignConfirm"));

        MAPSInteger *oIntegerId = &(oEltIntegerId->Integer());
        MAPSFloat *oFloatX = &(oEltFloatX->Float());
        MAPSFloat *oFloatY = &(oEltFloatY->Float());
        MAPSFloat *oFloatZ = &(oEltFloatZ->Float());
        MAPSInteger *oIntegerNumeroPiste = &(oEltIntegerNumeroPiste->Integer());
        MAPSInteger *oIntegerEtatPiste = &(oEltIntegerEtatPiste->Integer());
        MAPSInteger *oVectorSize = &(oEltVectorSize->Integer());
        MAPSFloat *oFloatVitesse = &(oEltFloatVitesse->Float());
        MAPSFloat *oFloatHauteur = &(oEltFloatHauteur->Float());
        MAPSFloat *oFloatLargeur = &(oEltFloatLargeur->Float());
        MAPSFloat *oFloatProfondeur = &(oEltFloatProfondeur->Float());
        MAPSInteger *oIntegerVoie = &(oEltIntegerVoie->Integer());
        MAPSInteger *oIntegerSignConfirm = &(oEltIntegerSignConfirm->Integer());

        int vsize = iEltRealObject->VectorSize();
        oEltIntegerId->VectorSize() = MaxPistes;
        oEltIntegerId->Timestamp() = timestamp;
        oEltFloatX->VectorSize() = MaxPistes;
        oEltFloatX->Timestamp() = timestamp;
        oEltFloatY->VectorSize() = MaxPistes;
        oEltFloatY->Timestamp() = timestamp;
        oEltFloatZ->VectorSize() = MaxPistes;
        oEltFloatZ->Timestamp() = timestamp;
        oEltIntegerNumeroPiste->VectorSize() = MaxPistes;
        oEltIntegerNumeroPiste->Timestamp() = timestamp;
        oEltIntegerEtatPiste->VectorSize() = MaxPistes;
        oEltIntegerEtatPiste->Timestamp() = timestamp;
        oEltFloatVitesse->VectorSize() = MaxPistes;
        oEltFloatVitesse->Timestamp() = timestamp;
        oEltFloatHauteur->VectorSize() = MaxPistes;
        oEltFloatHauteur->Timestamp() = timestamp;
        oEltFloatProfondeur->VectorSize() = MaxPistes;
        oEltFloatProfondeur->Timestamp() = timestamp;
        oEltFloatLargeur->VectorSize() = MaxPistes;
        oEltFloatLargeur->Timestamp() = timestamp;
        oEltIntegerVoie->VectorSize() = MaxPistes;
        oEltIntegerVoie->Timestamp() = timestamp;
        oEltIntegerSignConfirm->VectorSize() = MaxPistes;
        oEltIntegerSignConfirm->Timestamp() = timestamp;
        oEltVectorSize->Timestamp() = timestamp;
        *oVectorSize = vsize;

        for(int i=0; i<MaxPistes; i++)
        {
                if(i<vsize)
                {  
                    oIntegerId[i] = iRealObject[i].id;
                    oFloatX[i] = iRealObject[i].x;
                    oFloatY[i] = iRealObject[i].y;
                    oFloatZ[i] = iRealObject[i].z;
                    oIntegerNumeroPiste[i] = iRealObject[i].misc1;
                    oIntegerEtatPiste[i] = iRealObject[i].misc2;
                    oFloatVitesse[i] = iRealObject[i].vehicle.speed;
                    oFloatHauteur[i] = iRealObject[i].vehicle.height;
                    oFloatLargeur[i] = iRealObject[i].vehicle.width;
                    oFloatProfondeur[i] = iRealObject[i].vehicle.length;
                    oIntegerVoie[i] = iRealObject[i].misc3;
                    oIntegerSignConfirm[i] = iRealObject[i].sign.type;
                }
                else
                {
                    oIntegerId[i] = -1;
                    oFloatX[i] = -1;
                    oFloatY[i] = -1;
                    oFloatZ[i] = -1;
                    oIntegerNumeroPiste[i] = -1;
                    oIntegerEtatPiste[i] = -1;
                    oFloatVitesse[i] = -1;
                    oFloatHauteur[i] = -1;
                    oFloatLargeur[i] = -1;
                    oFloatProfondeur[i] = -1;
                    oIntegerVoie[i] = -1;
                    oIntegerSignConfirm[i] = -1;
                }
        }

        //ReportInfo(toto);

        StopWriting(oEltIntegerId);
        StopWriting(oEltFloatX);
        StopWriting(oEltFloatY);
        StopWriting(oEltFloatZ);
        StopWriting(oEltIntegerNumeroPiste);
        StopWriting(oEltIntegerEtatPiste);
        StopWriting(oEltVectorSize);
        StopWriting(oEltFloatVitesse);
        StopWriting(oEltFloatHauteur);
        StopWriting(oEltFloatLargeur);
        StopWriting(oEltFloatProfondeur);
        StopWriting(oEltIntegerVoie);
        StopWriting(oEltIntegerSignConfirm);
    }
    StopReading(Input("iRealObject"));
}

void MAPSLivicRealObject2BaseType::Death()
{
}
