////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////
// This is the simplest rt-maps component : it has no inputs, outputs, properties 
// nor actions (as you can see below). The CORE of the module just prints some
// information to the console and makes a pause (it sleeps during 500 ms)

#include "MAPS_LivicFiltreIRSivic.h"    // Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicFiltreIRSivic)
    MAPS_INPUT("iImage",MAPS::FilterIplImage,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicFiltreIRSivic)
    MAPS_OUTPUT("oImageS",MAPS::IplImage,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicFiltreIRSivic)
    MAPS_PROPERTY("pIntensiteIR",255,false,true)
    MAPS_PROPERTY("pAttenuation",50,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicFiltreIRSivic)
    //MAPS_ACTION("aName",MAPSLivicFiltreIRSivic::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicFiltreIRSivic) behaviour
MAPS_COMPONENT_DEFINITION(
                          MAPSLivicFiltreIRSivic,
                          "LivicFiltreIRSivic",
                          "$Revision: 1.3 $",
                          128,
                          MAPS::Threaded,
                          MAPS::Threaded,
                          1,1,2,0)

void MAPSLivicFiltreIRSivic::Birth()
{
    firstTime = true;
}

void MAPSLivicFiltreIRSivic::Core() 
{
    MAPSIOElt *iEltImage;
    iEltImage = StartReading(Input("iImage"));

    int intensiteIR = GetIntegerProperty("pIntensiteIR");

    IplImage &iImage=iEltImage->IplImage();

    if (firstTime)
    {
        IplImage imgSortie;
        imgSortie = MAPS::IplImageModel(
        iImage.width,
        iImage.height,
        MAPS_CHANNELSEQ_GRAY);

        Output(0).AllocOutputBufferIplImage(imgSortie);
        firstTime=false;
    }

    MAPSInteger attenuation = GetIntegerProperty("pAttenuation");
    unsigned char *ib=(unsigned char*)iImage.imageData;
    if(ib != NULL)
    {
        MAPSIOElt* oEltImgSortie=StartWriting(Output("oImageS"));   
        oEltImgSortie->Timestamp() = iEltImage->Timestamp();
        IplImage &oImgSortie=oEltImgSortie->IplImage();
        unsigned char *obS=(unsigned char*)oImgSortie.imageData;
        for(int i=0; i<iImage.width*iImage.height; i++)
        {
            if( ib[i]!=intensiteIR ) obS[i] = (100-attenuation)*ib[i]/100;
            else obS[i] = ib[i];
        }
        StopWriting(oEltImgSortie);
    }
    StopReading(Input("iImage"));
}

void MAPSLivicFiltreIRSivic::Death()
{
}
