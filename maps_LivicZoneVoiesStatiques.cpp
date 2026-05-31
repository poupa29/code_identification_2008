////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_LivicZoneVoiesStatiques.h"	// Includes the header of this component
#include "../commun/tarel/image.h"
#include "../commun/tarel/tools.h"

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicZoneVoiesStatiques)
    MAPS_INPUT("iImage",MAPS::FilterIplImage,MAPS::SamplingReader)
    MAPS_INPUT("iClicSouris",MAPS::FilterInteger,MAPS::SamplingReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicZoneVoiesStatiques)
    MAPS_OUTPUT("oImageSortie",MAPS::IplImage,NULL,NULL,1)
    MAPS_OUTPUT("oSegments",MAPS::Integer,NULL,NULL,16*4)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicZoneVoiesStatiques)
    MAPS_PROPERTY("pFichier","C:\\LivicZoneVoiesStatiques.conf",false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicZoneVoiesStatiques)
    MAPS_ACTION("aSAISIE",MAPSLivicZoneVoiesStatiques::Saisie)
    MAPS_ACTION("aEFFACER",MAPSLivicZoneVoiesStatiques::Effacer)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicZoneVoiesStatiques) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicZoneVoiesStatiques,"LivicZoneVoiesStatiques","$Revision: 1.3 $",128,
			  MAPS::Threaded,MAPS::Threaded,
			  2, // Nb of inputs
			  2, // Nb of outputs
			  1, // Nb of properties
			  2) // Nb of actions

bool MAPSLivicZoneVoiesStatiques::actionSaisie = false;
bool MAPSLivicZoneVoiesStatiques::actionEffacer = false;

void MAPSLivicZoneVoiesStatiques::Birth()
{
    lastIndice = 2;
    lastX=0;
    lastY=0;
    actionSaisie = false;
    finSaisie = true;
    enSaisie = false;
    xi_dimx = 0;
    xi_dimy = 0;
    firstTime = true;

    FILE *fic = fopen(GetStringProperty("pFichier"), "r");
    if( fic!= NULL )
    {
        x1.vide();
        y1.vide();
        x2.vide();
        y2.vide();
        int nb = 0;
        int *tx1, *ty1, *tx2, *ty2;
        fread(&nb, 1, sizeof(int), fic);
        tx1 = new int[nb];
        ty1 = new int[nb];
        tx2 = new int[nb];
        ty2 = new int[nb];
        fread(tx1, nb, sizeof(int), fic);
        fread(ty1, nb, sizeof(int), fic);
        fread(tx2, nb, sizeof(int), fic);
        fread(ty2, nb, sizeof(int), fic);
        fclose(fic);
        for(int i=0; i<nb; i++)
        {
            x1.empile(tx1[i]);
            y1.empile(ty1[i]);
            x2.empile(tx2[i]);
            y2.empile(ty2[i]);
        }
        delete tx1; delete ty1; delete tx2; delete ty2;
    }
    FinSaisie();
}

void MAPSLivicZoneVoiesStatiques::Saisie(MAPSModule *module, int nbAction)
{
	MAPSLivicZoneVoiesStatiques *a = (MAPSLivicZoneVoiesStatiques*)module;
    a->actionSaisie = true;
}

void MAPSLivicZoneVoiesStatiques::Effacer(MAPSModule *module, int nbAction)
{
	MAPSLivicZoneVoiesStatiques *a = (MAPSLivicZoneVoiesStatiques*)module;
    a->actionEffacer = true;
}

void MAPSLivicZoneVoiesStatiques::DebutSaisie()
{
    if( !IsConnected(Input("iImage")) || !IsConnected(Input("iClicSouris")) )
    {
        ReportWarning("Image ou ClicSouris non connecté");
        return;
    }
    enSaisie = true;
    finSaisie = false;
    ReportWarning("DEBUT DE SAISIE DES VOIES");
    MAPSIOElt *iEltClic;
    iEltClic = StartReading(Input("iClicSouris"));
    MAPSInteger *iClick = &iEltClic->Integer();
    lastX = iClick[0];
    lastY = iClick[1];
    StopReading(Input("iClicSouris"));
    Rest(100000);
    char msg[48];

    while( !finSaisie )
    {
        iEltClic = StartReading(Input("iClicSouris"));
        MAPSInteger *iClick = &iEltClic->Integer();
        if( (iClick[0]!=lastX) && (iClick[1]!=lastY) )
        {
            if( lastIndice==2 )
            {
                x1.empile(iClick[0]);
                y1.empile(iClick[1]);
                lastIndice = 1;
                sprintf(msg, "P1=[%d;%d]",iClick[0],iClick[1]);
            } else
            {
                x2.empile(iClick[0]);
                y2.empile(iClick[1]);
                lastIndice = 2;
                sprintf(msg, "P2=[%d;%d]",iClick[0],iClick[1]);
            }
            ReportWarning(msg);
            lastX = iClick[0];
            lastY = iClick[1];
        }
        Rest(100000);
    }
    enSaisie = false;
}

void MAPSLivicZoneVoiesStatiques::AfficheImageSortie()
{
    MAPSIOElt *iEltImage = StartReading(Input("iImage"));
    IplImage &iImage=iEltImage->IplImage();
    unsigned char* ib = (unsigned char*)iImage.imageData;

    MAPSIOElt* oEltImgSortie=StartWriting(Output("oImageSortie"));    
    IplImage &oImgSortie=oEltImgSortie->IplImage();
    unsigned char *obS=(unsigned char*)oImgSortie.imageData; 
    long taille = xi_dimx*xi_dimy;
    for(int p=0; p<taille; p++)
    {
        memset(obS+3*p, ib[p], 3);
    }

    for(int i=0; i<x2.nbElements(); i++)
    {
        TraceRGBLigne(
                obS,
                x1[i], y1[i], x2[i], y2[i],
                255,
                255,
                100,
                xi_dimx);
    }

    StopReading(Input("iImage"));
    StopWriting(oEltImgSortie);
}

void MAPSLivicZoneVoiesStatiques::FinSaisie()
{
    ReportWarning("FIN DE SAISIE DES VOIES");
    finSaisie = true;
    while( enSaisie )
    {
        Rest(100000);
    }
    MAPSIOElt *oEltSegments = StartWriting(Output("oSegments"));
    MAPSInteger *oSegments = &oEltSegments->Integer();
    int nb = x2.nbElements();
    oEltSegments->VectorSize() = nb*4;

    memcpy(oSegments+0*nb, x1.tableau(), nb*sizeof(int));
    memcpy(oSegments+1*nb, y1.tableau(), nb*sizeof(int));
    memcpy(oSegments+2*nb, x2.tableau(), nb*sizeof(int));
    memcpy(oSegments+3*nb, y2.tableau(), nb*sizeof(int));

    FILE *fic = fopen(GetStringProperty("pFichier"), "w");
    fwrite(&nb, 1, sizeof(int), fic);
    fwrite(x1.tableau(), nb, sizeof(int), fic);
    fwrite(y1.tableau(), nb, sizeof(int), fic);
    fwrite(x2.tableau(), nb, sizeof(int), fic);
    fwrite(y2.tableau(), nb, sizeof(int), fic);
    fclose(fic);

    StopWriting(oEltSegments);
}

void MAPSLivicZoneVoiesStatiques::Effacer()
{
    x1.vide();
    y1.vide();
    x2.vide();
    y2.vide();
    FinSaisie();
}

void MAPSLivicZoneVoiesStatiques::Core() 
{
    if( IsConnected(Input("iImage")) && IsConnected(Input("iClicSouris")) )
    {
        if (firstTime)
        {
            MAPSIOElt *iEltImage = StartReading(Input("iImage"));
            IplImage &iImage=iEltImage->IplImage();
            IplImage imgSortie;
            xi_dimx = iImage.width;
            xi_dimy = iImage.height;
            imgSortie = MAPS::IplImageModel(
                xi_dimx,
                xi_dimy,
                MAPS_CHANNELSEQ_RGB);
            Output(0).AllocOutputBufferIplImage(imgSortie);
            StopReading(Input("iImage"));
            firstTime = false;
        }
    }
    if( actionEffacer )
    {
        if( enSaisie ) FinSaisie();
        Effacer();
        actionEffacer = false;
    }
    if( actionSaisie )
    {
        if( enSaisie ) FinSaisie();
		else CreateThread((MAPSThreadFunction)&MAPSLivicZoneVoiesStatiques::DebutSaisie);
        actionSaisie = false;
    }
    AfficheImageSortie();
    Rest(100000);
}

void MAPSLivicZoneVoiesStatiques::Death()
{
}
