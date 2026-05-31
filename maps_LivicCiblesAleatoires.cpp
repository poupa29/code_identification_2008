////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_LivicCiblesAleatoires.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicCiblesAleatoires)
    //MAPS_INPUT("iName",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicCiblesAleatoires)
    MAPS_OUTPUT("oImageSortie",MAPS::IplImage,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicCiblesAleatoires)
    //MAPS_PROPERTY("pName",128,false,false)
    MAPS_PROPERTY("pFrequenceCamera",514,false,false)
    MAPS_PROPERTY("pFrequenceLampe",210,false,false)
    MAPS_PROPERTY("pLongueurTrame",16,false,false)
    MAPS_PROPERTY("pNbSpotsMax",4,false,false)
    MAPS_PROPERTY("pNbSpots",4,false,true)
    MAPS_PROPERTY("pWidth",240,false,false)
    MAPS_PROPERTY("pHeight",160,false,false)
    MAPS_PROPERTY("pTrueSignalAdd",false,false,false)
    MAPS_PROPERTY("pAddVitesse",false,false,false)
    MAPS_PROPERTY("pCalibrage",false,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicCiblesAleatoires)
    //MAPS_ACTION("aName",MAPSLivicCiblesAleatoires::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicCiblesAleatoires) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicCiblesAleatoires,"LivicCiblesAleatoires","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  0, // Nb of inputs
			  1, // Nb of outputs
			  10, // Nb of properties
			  0) // Nb of actions

void MAPSLivicCiblesAleatoires::Birth()
{
    // Reports this information to the RTMaps console
    ReportInfo("LivicCiblesAleatoires: Passing through Birth() method");
    
    
    xi_frequenceCamera = GetIntegerProperty("pFrequenceCamera");
    xi_frequenceLampe = GetIntegerProperty("pFrequenceLampe");
    xi_longueurTrame = GetIntegerProperty("pLongueurTrame");
    xi_nbSpotsMax = GetIntegerProperty("pNbSpotsMax");
    xi_width = GetIntegerProperty("pWidth");
    xi_height = GetIntegerProperty("pHeight");
    xb_trueSignalAdd = GetBoolProperty("pTrueSignalAdd");
    xb_AddVitesse = GetBoolProperty("pAddVitesse");
    xb_calibrage = GetBoolProperty("pCalibrage");
    //fic = fopen("c:\\caca.log","w");
    
    //*po_spot = (Spot*)malloc(xi_nbSpots*sizeof(Spot));
    if (xb_trueSignalAdd) xi_nbSpotsMax++;

    if (xb_calibrage) xi_nbSpotsMax = 1;

    po_spot = new Spot*[xi_nbSpotsMax];


    srand((unsigned)time(NULL));


    for( int i = 0; i < xi_nbSpotsMax; i++)
    {
        //srand( (unsigned)time( NULL ) );
        po_spot[i] = new Spot((int)((((3*xi_width/4)-(xi_width/4))*rand())/RAND_MAX+(xi_width/4)),(int)((((3*xi_height/4)-(xi_height/4))*rand())/RAND_MAX+(xi_height/4)));
        po_spot[i]->SetParam(xi_longueurTrame,xi_frequenceLampe,xb_calibrage);
        if((xb_trueSignalAdd||xb_calibrage) && (i == xi_nbSpotsMax-1))
            po_spot[i]->MAJSignal(xb_AddVitesse);
        else
            po_spot[i]->ChangeEtat();
    }

    pu_imgBytes = (unsigned char*)malloc(xi_width*xi_height*sizeof(unsigned char));
    // initialisation de l'image
    memset(pu_imgBytes, 0, xi_width * xi_height);

    xb_firstTime = true;

    xt_timestampDebut = MAPS::CurrentTime();

}

void MAPSLivicCiblesAleatoires::Core() 
{
    xi_nbSpots = GetIntegerProperty("pNbSpots");
    if(xi_nbSpots>xi_nbSpotsMax) xi_nbSpots=xi_nbSpotsMax;
    // ------------------------------------------------------------------------------------------------
    // le first time
    if (xb_firstTime)
    {
        // Reports this information to the RTMaps console
        ReportInfo("LivicCiblesAleatoires: Passing through Core() method");

        IplImage imgSortie;
        imgSortie = MAPS::IplImageModel(
            xi_width,
            xi_height,
            MAPS_CHANNELSEQ_GRAY);
        Output(0).AllocOutputBufferIplImage(imgSortie);
        xb_firstTime=false;


    }
    // le first time
    // ------------------------------------------------------------------------------------------------

    
    for( int i = 0; i < xi_nbSpots; i++)
    {
        if((xb_trueSignalAdd||xb_calibrage) && (i == xi_nbSpotsMax-1))
            po_spot[i]->MAJSignal(xb_AddVitesse);
        else
            po_spot[i]->ChangeEtat();
        pu_imgBytes[po_spot[i]->GetX() + xi_width * po_spot[i]->GetY()] = (int)(po_spot[i]->SpotEtat()) * 255;     
    }
    for(int i = xi_nbSpots; i<xi_nbSpotsMax; i++)
        pu_imgBytes[po_spot[i]->GetX() + xi_width * po_spot[i]->GetY()] = 0;

    MAPSIOElt* oEltImgSortie=StartWriting(Output("oImageSortie"));  
    IplImage &oImgSortie=oEltImgSortie->IplImage();
    memcpy(oImgSortie.imageData, pu_imgBytes, xi_width*xi_height);
    oEltImgSortie->Timestamp() = xt_timestampDebut;
    StopWriting(oEltImgSortie);
    
    MAPSTimestamp timestamp = xt_timestampFin;

    xt_timestampFin = MAPS::CurrentTime();

    //fprintf(fic,"temps camera : %f \n",1./(float)(xt_timestampFin - timestamp));
    Rest(max(0,(1000000/xi_frequenceCamera)-(xt_timestampFin-xt_timestampDebut)));

    //fprintf(fic,"temps camera : %f \n",(1000000/xi_frequenceCamera)-(xt_timestampFin-xt_timestampDebut));
    //xt_timestampFin = MAPS::CurrentTime();
    //fprintf(fic,"temps camera : %f \n",(xt_timestampFin - xt_timestampDebut));
    
    xt_timestampDebut += 1000000/xi_frequenceCamera;

    //delete oEltImgSortie;
    
}

void MAPSLivicCiblesAleatoires::Death()
{
    // Reports this information to the RTMaps console
    ReportInfo("LivicCiblesAleatoires: Passing through Death() method");

    delete pu_imgBytes;
    
    for( int i = 0; i < xi_nbSpotsMax; i++)
    {
        delete po_spot[i];
        po_spot[i]=NULL;
    }
    delete[] po_spot;

//    fclose(fic);
}
