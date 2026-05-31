////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////
// This is the simplest rt-maps component : it has no inputs, outputs, properties 
// nor actions (as you can see below). The CORE of the module just prints some
// information to the console and makes a pause (it sleeps during 500 ms)

#include "MAPS_LivicPistesIRViewer.h"   // Includes the header of this component
#include "../commun/tarel/image.h"
#include "../commun/tarel/tools.h"

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicPistesIRViewer)
    MAPS_INPUT("iImage",MAPS::FilterIplImage,MAPS::SamplingReader)
    MAPS_INPUT("iPistesIR",MAPS::FilterRealObjects,MAPS::FifoReader)
    MAPS_INPUT("iMessages",MAPS::FilterStream8,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicPistesIRViewer)
    MAPS_OUTPUT("oImageSortie",MAPS::IplImage,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicPistesIRViewer)
    MAPS_PROPERTY("pAfficherIDNuls", true, false, true)
	MAPS_PROPERTY("pAfficherRectangles", true, false, true)
    MAPS_PROPERTY("pAfficherTimestamps", true, false, true)
    //MAPS_PROPERTY("pName",128,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicPistesIRViewer)
    //MAPS_ACTION("aName",MAPSLivicPistesIRViewer::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicPistesIRViewer) behaviour
MAPS_COMPONENT_DEFINITION(
    MAPSLivicPistesIRViewer,
    "LivicPistesIRViewer",
    "$Revision: 1.9 $",
    128,
    MAPS::Threaded,
    MAPS::Threaded,
    3,
    1,
    3,
    0)

void MAPSLivicPistesIRViewer::Birth()
{
    xb_firstTime = true;
}

MAPSTimestamp MAPSLivicPistesIRViewer::mtsp_ImageSuivant()
{
    if( pu_ib ) StopReading(Input("iImage"));
    MAPSIOElt *iEltImage;
    iEltImage = StartReading(Input("iImage"));
    IplImage &iImage=iEltImage->IplImage();
    pu_ib = (unsigned char*)iImage.imageData;
    xi_dimx = iImage.width;
    xi_dimy = iImage.height;
    return iEltImage->Timestamp();
}

MAPSTimestamp MAPSLivicPistesIRViewer::mtsp_PistesIRSuivant()
{
    if( po_pistesIR ) StopReading(Input("iPistesIR"));
    MAPSIOElt *iEltPistesIR = StartReading(Input("iPistesIR"));
    po_pistesIR = &iEltPistesIR->RealObject();
    xi_nbPistesIR = iEltPistesIR->VectorSize();
    return iEltPistesIR->Timestamp();
}

void MAPSLivicPistesIRViewer::mv_MessageSuivant()
{
    if( DataAvailableInFIFO(Input("iMessages")) )
    {
        char *message = (char*)malloc(TAILLE_MSG*sizeof(char));
        MAPSIOElt *iEltMessages = StartReading(Input("iMessages"));
        char *data=(char*)iEltMessages->Stream8();
        strcpy(message, data);
        StopReading(Input("iMessages"));
        LivicInt64 x;
        void *p;
        if( xo_messages.nbElements() >2 )
        {
            xo_messages.depileBase(&p);
            free(p);
            xo_synchros.depileBase(&x);
        }
        xo_messages.empile((void*)message);
        xo_synchros.empile(xt_timestampImage);
    }
}

void MAPSLivicPistesIRViewer::Core() 
{
    xi_nbPistesIR = 0;
    po_pistesIR = NULL;
    pu_ib = NULL;
    xt_timestampImage = 0;
    xt_timestampPistesIR = 0;
    xi_dimx = 0;
    xi_dimy = 0;


    // ------------------------------------------------------------------------------------------------
    // lecture des pistes IR
    if( IsConnected(Input("iPistesIR")) )
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
    }
    // lecture du message radio
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
        xb_firstTime=false;
    }
    // le first time
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // startwriting image sortie
    MAPSIOElt* oEltImgSortie=StartWriting(Output("oImageSortie"));    
    IplImage &oImgSortie=oEltImgSortie->IplImage();
    oEltImgSortie->Timestamp()=xt_timestampImage;
    unsigned char *obS=(unsigned char*)oImgSortie.imageData;
    memset(obS, 0, xi_dimx*xi_dimy*3);
    // startwriting image sortie
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // recopie image entree dans image sortie
    if( pu_ib )
    {
        long taille = xi_dimx*xi_dimy;
        for(int p=0; p<taille; p++)
        {
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
            if ( ((int)po_pistesIR[i].id != 0) || GetBoolProperty("pAfficherIDNuls") )
            {
				if (GetBoolProperty("pAfficherRectangles"))
				{
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
					sprintf(xs_msg, "%d", po_pistesIR[i].id);
					printT2I(
						obS,
						xs_msg,
						(int)po_pistesIR[i].x,
						(int)po_pistesIR[i].y-5-10,
						CYAN,
						xi_dimx,
						xi_dimy
					);
				}
				else
				{
					
					DrawRGBLine (obS, (int)po_pistesIR[i].x-5, (int)po_pistesIR[i].y, (int)po_pistesIR[i].x+5, (int)po_pistesIR[i].y, RED, xi_dimx, xi_dimy);
					DrawRGBLine (obS, (int)po_pistesIR[i].x, (int)po_pistesIR[i].y-5, (int)po_pistesIR[i].x, (int)po_pistesIR[i].y+5, RED, xi_dimx, xi_dimy);

					DrawRGBLine (obS, (int)po_pistesIR[i].x-5, (int)po_pistesIR[i].y+1, (int)po_pistesIR[i].x+5, (int)po_pistesIR[i].y+1, RED, xi_dimx, xi_dimy);
					DrawRGBLine (obS, (int)po_pistesIR[i].x+1, (int)po_pistesIR[i].y-5, (int)po_pistesIR[i].x+1, (int)po_pistesIR[i].y+5, RED, xi_dimx, xi_dimy);


					sprintf(xs_msg, "%d", po_pistesIR[i].id);
					printT2I(
						obS,
						xs_msg,
						2,
						(int)po_pistesIR[i].x-5,
						(int)po_pistesIR[i].y-22,
						RED,
						xi_dimx,
						xi_dimy
					);
				}
            }
        }
    }
    // affichage des pistes en overlay de l'image
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // affichage des timestamps
    if (GetBoolProperty("pAfficherTimestamps"))
    {
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
    // affichage des messages
    for(int i=0; i<xo_messages.nbElements(); i++)
    {
        sprintf(xs_msg, "%I64d : %s", xo_synchros[i], (char*)xo_messages[i]);
        printT2I(
            obS,
            xs_msg,
            10,
            10*(i+1),
            ORANGE,
            xi_dimx,
            xi_dimy);
    }
    // affichage des messages
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // Stopreadings et stopwritings
    StopWriting(oEltImgSortie);
    StopReading(Input("iImage"));
    if( IsConnected(Input("iPistesIR")) ) StopReading(Input("iPistesIR"));
    // Stopreadings et stopwritings
    // ------------------------------------------------------------------------------------------------


}

void MAPSLivicPistesIRViewer::Death()
{
    int n = xo_messages.nbElements();
    for(int i=0; i<n; i++) free((char*)xo_messages[i]);
    xo_messages.vide();
    xo_synchros.vide();
}
