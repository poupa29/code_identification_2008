////////////////////////////////
// RTMaps SDK Component
////////////////////////////////
/******************************************************************************
 
 (c) LIVIC 2005
 
 Ce fichier est la propriete du LIVIC. Toute utilisation, copie partielle ou
 totale, modification du fichier sans autorisation du LIVIC est interdite.
 
        ATTENTION l'image d'entrée 1 doit absolument être connectée.

*******************************************************************************/


#include "maps_LivicTrackssViewer.h"


//**********************************************************************************************
//		DEFINITION DU COMPOSANT
//**********************************************************************************************0

// Entrées du composant:
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicTrackssViewer)
	MAPS_INPUT("iImageRoute", MAPS::FilterIplImage, MAPS::FifoReader)
	MAPS_INPUT("iImageIHM", MAPS::FilterIplImage, MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION


// Sorties du composant:
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicTrackssViewer)
	MAPS_OUTPUT("oImageSortie",MAPS::IplImage,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Propriétés du composant:
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicTrackssViewer)
MAPS_END_PROPERTIES_DEFINITION

// Actions du composant:
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicTrackssViewer)
MAPS_END_ACTIONS_DEFINITION



// Déclaration du composant lui-même:
MAPS_COMPONENT_DEFINITION(MAPSLivicTrackssViewer,
						  "LivicTrackssViewer",
						  "Version 0.1",
						  128,
						  MAPS::Sequential|MAPS::Threaded,
						  MAPS::Threaded,
						  2,1,0,0)

void MAPSLivicTrackssViewer::Birth()
{
	firstTime=true;
	ReportInfo("MAPSLivicTrackssViewer : Passing through Birth() method");
}

void MAPSLivicTrackssViewer::Core() 
{
	//	-----------------------
	//	Lecture des entrees:
	//	-----------------------

	MAPSIOElt *ioEltsImageRoute = StartReading(Input("iImageRoute"));	
	MAPSIOElt *ioEltsImageIHM = StartReading(Input("iImageIHM"));	

	//	-------------------------------------------
	//	----------	INITIALISATIONS	---------------
	//---------------------------------------------
	if(firstTime)
	{
		ReportInfo("Initialisation IHM");

		//--- Taille des images: ---
		dimX	= ECRAN_X; 
		dimY	= ECRAN_Y; 
		nbPix	= dimX*dimY;

		//---	Test de la taille des images entrée:	---

		if ( (ioEltsImageIHM->IplImage().width != IHM_X) || (ioEltsImageIHM->IplImage().height != IHM_Y) )
		{
			ReportError("Taille de l'IHM 304*344 a reduire !!!");
			CommitSuicide();
		}

		//	--- Allocation des images de sortie: ---
		IplImage ModeleDoubleRGB	= MAPS::IplImageModel(dimX,dimY,MAPS_CHANNELSEQ_RGB);

		Output("oImageSortie").AllocOutputBufferIplImage(ModeleDoubleRGB);
		
		//Chargement de l'image TRACKSS
		pu_Image  = new unsigned char [IMG_X*IMG_Y*3];
		ImageBMP bmp = ImageBMP("R:/maps/perception/identification/images/Viewer/trackss_icon.bmp");
        int statut = bmp.LectureImage(pu_Image,true);

		pu_Image2  = new unsigned char [IMG_BANNER_X*IMG_BANNER_Y*3];
		bmp = ImageBMP("R:/maps/perception/identification/images/Viewer/trackss_banner.bmp");
        statut = bmp.LectureImage(pu_Image2,true);
		
		firstTime=false;
	}

	//	---	Initialisation des images de sortie:	---
	MAPSIOElt *oIOEltImageSortie = StartWriting(Output("oImageSortie"));
	unsigned char *oSortie = (unsigned char *)(oIOEltImageSortie->IplImage().imageData);

	memset(oSortie, 0, 3*nbPix*sizeof(unsigned char));

	//Affiche l'image trackss

	memcpy(oSortie, pu_Image, IMG_X*IMG_Y*3);

	//Affiche la banniere du bas
	memcpy(oSortie + (ECRAN_Y-IMG_BANNER_Y)*ECRAN_X*3, pu_Image2, IMG_BANNER_X*IMG_BANNER_Y*3);

	//	Recopie de l'image route :
	unsigned char *iBuff = (unsigned char *)(ioEltsImageRoute->IplImage().imageData);
	for(j=0;j<ROUTE_Y;j++)
		memcpy(oSortie + (IMG_X*IMG_Y*3) + j*3*ECRAN_X + CENTRAGE_ROUTE_Y*ECRAN_X*3 , iBuff + (j*3*ROUTE_X), 3*ROUTE_X);
	
	
	//	Recopie de l'image IHM:
	iBuff = (unsigned char *)(ioEltsImageIHM->IplImage().imageData);

	for(j=0;j<IHM_Y;j++)
		memcpy(oSortie + (IMG_X*IMG_Y*3) + ROUTE_X*3 + (j*3*ECRAN_X) , iBuff+(j*3*IHM_X), 3*IHM_X);
	

	//	Fermeture des entrées:
	StopReading(Input("iImageRoute"));
	StopReading(Input("iImageIHM"));

	//	Fermeture des sorties:
	StopWriting(oIOEltImageSortie);

}


void MAPSLivicTrackssViewer::Death()
{
}