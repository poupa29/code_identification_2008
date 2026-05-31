////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

#ifndef _Maps_LivicIRViewer_H
#define _Maps_LivicIRViewer_H

// Includes maps sdk library header

#include "../commun/LibTI/LivicTI.h"

#include "../../general/utils/chargementBMP.h"
//#include "../../general/utils/livicutils.h"
#include "IR.h"
#include "../commun/libStereo/LivicImageIHM.h"
#include "maps.hpp"


//#include "../stereoPerrollaz/DetectionStereo/LivicStereoDetecteur.h"
//#include "../commun/echangesRealObject/EntreeSortieRtMaps.h"

#define	MAX_MESSAGES_INFRA	50
#define DIM_IMAGES_PANNEAUX 600
#define DIM_IMAGES_METEO 120
//#define DISP_MAX			200
#define MAX_ROADMARKERS		100
#define PIsur180 0.01745329251994

struct StructureParametresCameras
{
	double Alpha;
	double B;
	double H;
	double Theta;
};


//! The RTMaps input filter for the structure MyNewStructure
const MAPSTypeFilterBase MAPSFilterStructureParametresCameras = MAPSFilterUserStructure(StructureParametresCameras);



// Declares a new MAPSComponent child class
class MAPSLivicIRViewer : public MAPSComponent 
{
    // Use standard header definition macro
    MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicIRViewer)
private :
    // données
    bool                xb_firstTime;
    //bool                xb_firstTimeAffichageZoneFiltrage;
    char                xs_msg[512];
    int                 xi_nbPistesIR;
    MAPSRealObject*     po_pistesIR;
    MAPSRealObject*     po_pistesIRVehicule;
    unsigned char*      pu_ib;
    MAPSTimestamp       xt_timestampImage;
    MAPSTimestamp       xt_timestampPistesIR;
    MAPSTimestamp       xt_timestampMessages;
    MAPSTimestamp       TempsAffichageMessage;
   

    //Parametres de la camera:
    double xd_DecalageCameraX;
    double xd_DecalageCameraY;
    double xd_DecalageCameraZ;
    double xd_TangageCamera;  
    double xd_Uo;             
    double xd_Vo;             
    double xd_AlphaU;         
    double xd_AlphaV;         

    int                 xi_dimx;
    int                 xi_dimy;
    //int                 xi_nbPixels;
    LivicPilePointeur   xo_messages;
    /*LivicPileInt64      xo_synchros;*/
	LivicPileInt64      xo_messagesTimestamp;
	LivicPileInt        xo_messagesID;
    LivicPileInt  		xo_messagesTY;
    LivicPileInt  		xo_messagesPA1;
	LivicPileInt  		xo_messagesPA2;
    LivicPileInt        xo_messageATraiter;
    char                ps_MessagesInfrastructure[MAX_MESSAGES_INFRA][10*TAILLE_MSG];
    int                 xi_nbMessagesInfrastructure;
    
    unsigned char*      pu_ImageIHMConducteur;
    bool                xb_AfficherIHMConducteur;
    bool                xb_AfficherTimestamp;
    bool                xb_AffichageZoneFiltrage;
    bool                xb_AffichageDistances;
    bool                xb_AffichageVitesses;
    bool                xb_AffichageVoiesCirculation;

    unsigned char       *pu_ImageVirageDroite;
    unsigned char       *pu_ImageDebutZone30;
    unsigned char       *pu_ImageFinZone30;
    unsigned char       *pu_ImageVirageGauche;
    unsigned char       *pu_ImageChicane;
    unsigned char       *pu_ImagePanneauFeu;
    unsigned char       *pu_ImageFeu;
    unsigned char       *pu_ImageFeuRouge;
    unsigned char       *pu_ImageFeuOrange;
    unsigned char       *pu_ImageFeuVert;
    unsigned char       *pu_ImageDanger;
    unsigned char       *pu_ImageMessage;
    unsigned char       *pu_ImageStop;
    unsigned char       *pu_ImageVitesse90;
    unsigned char       *pu_ImageVitesse50;
    unsigned char       *pu_ImageFinVitesse50;
    MAPSEnumStruct      pe_TypeImage;
	//Meteo
	unsigned char       *pu_ImageMeteoNA;
	unsigned char       *pu_ImageMeteojour;
	unsigned char       *pu_ImageMeteonuit;
	unsigned char       *pu_ImageMeteochaud;
	unsigned char       *pu_ImageMeteopluiejour;
	unsigned char       *pu_ImageMeteopluienuit;
	unsigned char       *pu_ImageMeteoverglas;






	unsigned char		*pu_lastPanneauImage;
	int					xi_lastPanneauX;
	int					xi_lastPanneauY;
	int					xi_lastPanneauDist;
	MAPSTimestamp		xt_lastPanneauTime;
	
	unsigned char *       IHMConducteur;
    unsigned char *       obS;
    //unsigned char       *obS

    // donnees zone filtrage
    int					xi_NbPtDroiteZoneFiltrage;
	int					xi_NbPtGaucheZoneFiltrage;
	double				pd_PtsDroiteZoneFiltrage[2*MAX_ROADMARKERS];
	double				pd_PtsGaucheZoneFiltrage[2*MAX_ROADMARKERS];

    StructureParametresCameras xo_parametresCameras;

    // donnees voies circulation
    int                 nbVoies;
    int                 *x1, *x2, *y1, *y2;
   

    // méthodes
    MAPSTimestamp       mtsp_ImageSuivant();
    MAPSTimestamp       mtsp_PistesIRSuivant();
    void                mv_MessageSuivant();

    int                 mi_XZ2U(double _x, double _z);
    int                 mi_XZ2V(double _x, double _z);
   // void                mv_RecupZoneFiltrageMessages();
   // void                mv_FiltrageMessages();
    void                mv_AffichagePanneaux();
    int                 mv_AffichageMessages(char *ihmMsg, unsigned char** pu_imageMeteo);
	void				mv_AffichePanneau();
	void				mv_SetPanneau(unsigned char* panneau, int x, int y, int dist);
	void				MAPSLivicIRViewer::mv_CopieMiniImage(unsigned char *image, int maxx, int maxy, unsigned char* mini, int x, int y, int dimMiniX, int dimMiniY, int dist, bool inversionRGB, bool centrage);
	unsigned char*		MAPSLivicIRViewer::mv_AffichageMeteo(int code);
	void				MAPSLivicIRViewer::MsgSonore();
};

#endif
