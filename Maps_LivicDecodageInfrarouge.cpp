////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////
// This is the simplest rt-maps component : it has no inputs, outputs, properties 
// nor actions (as you can see below). The CORE of the module just prints some
// information to the console and makes a pause (it sleeps during 500 ms)

#include <stdio.h>
FILE *flog;
//FILE *fcounter;


#include "MAPS_LivicDecodageInfrarouge.h"   // Includes the header of this component
#include "../../general/utils/livicglobal.h"
#include "../commun/tarel/image.h"
#include "../commun/tarel/tools.h"

// allocation des statics
int LDIRegion::nextid = 0;
bool LDIRegion::modeCalibrage = false;
int LDIRegion::confianceMax = 0;
int LDIRegion::penaliteConfianceNouvellePiste = 0;
int LDIRegion::coefAnciennete = 2;
bool LDIRegion::activeAnciennete = false;



// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicDecodageInfrarouge)
    MAPS_INPUT("iImage",MAPS::FilterIplImage,MAPS::FifoReader)
	MAPS_INPUT("iTangage",MAPS::FilterFloat,MAPS::SamplingReader)
	ARRET_INPUTS
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicDecodageInfrarouge)
    MAPS_OUTPUT("oImageSortie",MAPS::IplImage,NULL,NULL,1)
    MAPS_OUTPUT("oImageRegionsAvantElimination",MAPS::IplImage,NULL,NULL,1)
    MAPS_OUTPUT("oImageRegions",MAPS::IplImage,NULL,NULL,1)
    MAPS_OUTPUT("oPistesIR",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
    MAPS_OUTPUT("oSortieTempsExecution",MAPS::Integer,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicDecodageInfrarouge)
    MAPS_PROPERTY("pModeCalibrage",false,false,false)
    MAPS_PROPERTY("pIdPisteCalibrante",0,false,true)
    MAPS_PROPERTY("pNbImagesParBit",5.,false,false)
    MAPS_PROPERTY("pTailleMessageCode",10,false,false)
    MAPS_PROPERTY("pStartCode","1101",false,false)
    MAPS_PROPERTY("pHauteurHorizon",50,false,true)
    MAPS_PROPERTY("pCoefTangageHorizon",4.,false,true)
    MAPS_PROPERTY("pSeuil",150,false,true)
//PIDSeuil    MAPS_PROPERTY("pPIDSeuil",false,false,true)
//PIDSeuil    MAPS_PROPERTY("pPIDSeuilConsigne",30,false,true)
//PIDSeuil    MAPS_PROPERTY("pPIDSeuilKp",0.05,false,true)
//PIDSeuil    MAPS_PROPERTY("pPIDSeuilKd",0.,false,true)
//PIDSeuil    MAPS_PROPERTY("pPIDSeuilKi",0.,false,true)
    MAPS_PROPERTY("pTailleDilatation",5,false,true)
    MAPS_PROPERTY("pVoisinageX",125.,false,true)
    MAPS_PROPERTY("pVoisinageY",25.,false,true)
    MAPS_PROPERTY("pConfianceMax",100,false,true)
    MAPS_PROPERTY("pPenaliteConfianceNouvellePiste",3,false,true)
    MAPS_PROPERTY("pActiveAnciennete",false,false,true)
    MAPS_PROPERTY("pCoefAnciennete",2,false,true)
    MAPS_PROPERTY_ENUM("pFormeSpot","Circulaire|Rectiligne", 0, false,true)
    MAPS_PROPERTY("pRapportRectangle",10.,false,true)
    MAPS_PROPERTY("pSeuilRegionForme",0.55,false,true)
    MAPS_PROPERTY("pSeuilRegionGrande",200,false,true)
    MAPS_PROPERTY("pSeuilRegionPonctuelle",25,false,true)
    MAPS_PROPERTY("pRapportBinningXsurY",1.,false,false)
    MAPS_PROPERTY("pModeAllocStatique", false, false, false)
    MAPS_PROPERTY("pEchelleAffichageVitesse",5.,false,true)
    MAPS_PROPERTY("pEchelleAffichageAcceleration",5.,false,true)
    MAPS_PROPERTY("pBitParite",true,false,false)
    MAPS_PROPERTY("pDecodageVitesse",false,false,false)
    MAPS_PROPERTY("pSivic",false,false,false)
    MAPS_PROPERTY("pStats",false,false,false)
    MAPS_PROPERTY("pVerbeux",false,false,true)
    MAPS_PROPERTY("pAffichage",true,false,true)
    MAPS_PROPERTY("pAffichageDetaille",true,false,true)
    MAPS_PROPERTY("pSortieToutesPisteIR",false,false,true)
    MAPS_PROPERTY("pSortieTempsExecution",false,false,true)
    MAPS_PROPERTY("pFiltrageReflets",false,false,true)
    MAPS_PROPERTY("pDebug",false,false,false)
    MAPS_PROPERTY("pDebugCheminFichierLog","C:\\identification.log",false,false)
	MAPS_PROPERTY("pTimoutFaussesPistes",500,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicDecodageInfrarouge)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (LivicDecodageInfrarouge) behaviour
MAPS_COMPONENT_DEFINITION(
                  MAPSLivicDecodageInfrarouge,
                  "LivicDecodageInfrarouge",
                  "$Revision: 1.75 $",
                  128,
                  MAPS::Threaded,
                  MAPS::Threaded,
                  2+ARRET_INPUTS_NB,
                  5,
                  37,
                  0)


void MAPSLivicDecodageInfrarouge::Birth()
{
    livicTermineAppli = false;
    derniereFois = false;
    sprintf(xs_msg, "");
    xi_largeur = xi_hauteur = xi_taille = 0;
    xi_logdbgi = 0;
    sprintf(xs_logdbgmsg, "");
    xb_firstTime = true;

    if (xb_modeAllocStatique) LivicPilePoint2D::setModeAllocationStatique( NB_REGIONS_STATIQUES, TAILLE_REGIONS_STATIQUES);//640*480, 640*480); 
    po_pistes = new LivicPilePointeur(NBPISTES);
    po_pistes->vide();
    LDIRegion::modeCalibrage = false;
    xi_nbPistesRattrapeesAnciennete = 0;
    po_freqCamera = new LivicFileDouble(4);
    xt_timestampPrec = 0;
//PIDSeuil    xd_E_PIDSeuilPrec = 0;
//PIDSeuil    xd_I_PIDSeuilPrec = 0;

    //xo_codeVariable = new LivicFileShort(TAILLE_VARIABLE_MAX);

    // ------------------------------------------------------------------------------------------------
    // lecture des proprietes
    mv_LitProprietesStatiques();
    // lecture des proprietes
    // ------------------------------------------------------------------------------------------------
    

	if( LDIRegion::modeCalibrage )
    {
        strcpy(xs_msg, "\n  Vous etes en mode calibrage.");
        strcat(xs_msg, "\n  Avant de lancer le diagramme, mettez la propriete pNbImagesParBit a une valeur elevee (10000)");
        strcat(xs_msg, "\n  Procedure:");
        strcat(xs_msg, "\n    - attendez que la bonne source clignotante de signal 10101010... soit detectee comme piste calibrante");
        strcat(xs_msg, "\n    - ouvrez les proprietes et indiquez le numero (id) de cette piste dans pIdPisteCalibrante");
        strcat(xs_msg, "\n    - attendez ensuite le premier resultat de nbImagesParBit pour cette piste");
        strcat(xs_msg, "\n    - attendez eventuellement d'autres approximations de nbImagesParBit (qui s'affine avec le temps)");
        strcat(xs_msg, "\n    - quand vous estimez que le calcul est correct, stoppez le diagramme");
        strcat(xs_msg, "\n    - le calcul pour la piste choisie est alors valide et auto-inscrit dans la propriete pNbImagesParBit");
        strcat(xs_msg, "\n    - enregistrez le diagramme pour prendre en compte la nouvelle valeur");
        strcat(xs_msg, "\n  Fin de la procedure.");
        ReportWarning(xs_msg);
    }

    LDIRegion::nextid=0;

    if( pb_debug ) flog = fopen(GetStringProperty("pDebugCheminFichierLog"), "w");
    mv_LOGDEBUG("Birth fin");

}

void MAPSLivicDecodageInfrarouge::Core() 
{
    
    MAPSTimestamp tempsDebut = MAPS::CurrentTime();
    
    if( derniereFois ) {
        livicTermineAppli = true;
    }

    if (livicTermineAppli) {
        Rest(livicArretRest);
        ReportWarning("appli terminee");
        return;
    }

    ARRET_CORE
    
    xi_logdbgi=0;
    mv_LOGDEBUG("----------------------------------------------------------------------------------------------------");


    // ------------------------------------------------------------------------------------------------
    // lecture de l'image d'entree
    MAPSIOElt *iEltImage;
    iEltImage = StartReading(Input("iImage"));
    IplImage &iImage=iEltImage->IplImage();
    xt_timestamp = iEltImage->Timestamp();
    po_freqCamera->enfile(1000000./(xt_timestamp - xt_timestampPrec));
    xi_largeur = ((640%iImage.width)||(iImage.width%640))?iImage.width-8:iImage.width;
    xi_hauteur = iImage.height;
    xi_taille = xi_largeur*xi_hauteur;
    mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Core debut : timestamp %ld", xt_timestamp));
    xi_logdbgi++;
    // lecture de l'image d'entree
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // le first time
    if (xb_firstTime)
    {
        IplImage imgSortie;
        imgSortie = MAPS::IplImageModel(
            iImage.width,
            iImage.height,
            MAPS_CHANNELSEQ_RGB);
        Output(0).AllocOutputBufferIplImage(imgSortie);
        Output(1).AllocOutputBufferIplImage(imgSortie);
        Output(2).AllocOutputBufferIplImage(imgSortie);
        xb_firstTime=false;
    }
    // le first time
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // lecture des proprietes
    mv_LitProprietesDynamiques();
    // lecture des proprietes
    // ------------------------------------------------------------------------------------------------

    
    // ------------------------------------------------------------------------------------------------
    // le gros core
    unsigned char *ib=(unsigned char*)iImage.imageData;
    if(ib != NULL)
    {

        // ------------------------------------------------------------------------------------------------
        // Allocation images sorties
        MAPSIOElt* oEltImgSortie=StartWriting(Output("oImageSortie"));  
        IplImage &oImgSortie=oEltImgSortie->IplImage();
        unsigned char *obS=(unsigned char*)oImgSortie.imageData;
        MAPSIOElt* oEltImgRegions=NULL;
        IplImage *oImgRegions=NULL;
        MAPSIOElt* oEltImgRegions1=NULL;
        IplImage *oImgRegions1=NULL;
        unsigned char *obR=NULL;
        unsigned char *obR1=NULL;
        if( pb_debug )
        {
            oEltImgRegions=StartWriting(Output("oImageRegionsAvantElimination")); 
            oImgRegions=&(oEltImgRegions->IplImage());
            oEltImgRegions1=StartWriting(Output("oImageRegions"));    
            oImgRegions1=&(oEltImgRegions1->IplImage());
            obR=(unsigned char*)oImgRegions->imageData;
            obR1=(unsigned char*)oImgRegions1->imageData;
        }
        mv_LOGDEBUG("Img read et malloc OK");
        // Allocation images sorties
        // ------------------------------------------------------------------------------------------------


		// ------------------------------------------------------------------------------------------------
		// lecture du tangage et calcul horizon
		MAPSFloat tangage = 0;
		if( IsConnected(Input("iTangage")) )
		{
			MAPSIOElt *iEltTangage;
			iEltTangage = StartReading(Input("iTangage"));
			tangage = iEltTangage->Float();
			StopReading(Input("iTangage"));
		}
		pi_hauteurHorizon -= tangage*pf_coefTangageHorizon;
		pi_hauteurHorizon = max(pi_hauteurHorizon,0);
		// lecture du tangage et calcul horizon
		// ------------------------------------------------------------------------------------------------


		// ------------------------------------------------------------------------------------------------
        // PID seuil sur l'intensite de l'image d'entree
		//PIDSeuil if( pb_PIDSeuil )
		//PIDSeuil {
		//PIDSeuil 	mv_PIDSeuil(ib);
		//PIDSeuil 	mv_LOGDEBUG(sprintf(xs_logdbgmsg, "PIDSeuil OK"));
		//PIDSeuil }
        // PID seuil sur l'intensite de l'image d'entree
        // ------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------------------
        // seuillage
        unsigned char *imgTmp1 = (unsigned char*)malloc(xi_taille*sizeof(unsigned char));
		memset(imgTmp1, 0, xi_taille*sizeof(unsigned char));
        mv_Seuillage(ib, imgTmp1, pi_seuil, pi_hauteurHorizon);
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Seuillage OK"));
        // seuillage
        // ------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------------------
        // pour Sivic : elimination pixels blancs isolés
        if( pb_sivic )
        {
            mv_EliminationsPointsBlancsSivic(imgTmp1);
			mv_LOGDEBUG(sprintf(xs_logdbgmsg, "EliminationsPointsBlancsSivic OK"));
        }
        // pour Sivic : elimination pixels blancs isolés
        // ------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------------------
        // dilatation
        // La taille de la dilatation peut etre calculee en fonction de l'interdistance.
        // (plus on est près, plus elle est grande, pour bien lisser les diodes)
        LvDilatation(imgTmp1, xi_largeur, xi_hauteur, pi_tailleDilatation);
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Dilatation OK"));
        // dilatation
        // ------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------------------
        // etiquetage des regions
        unsigned char *imgRegions;
        LivicPilePointeur pregions(100);
        int nbRegions = LvDetection_regions(TI_REGIONS_LINEAIRE, imgTmp1, &imgRegions, &pregions, xi_largeur, xi_hauteur);
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Detection %d regions", nbRegions));
        // etiquetage des regions
        // ------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------------------
        // elimination des grandes ou non circulaires regions
        // (aucune chance qu'elles soient des po_pistes IR)
        // et recopie des regions triees dans un nouveau buffer
        unsigned char *imgRegions1;
        mv_EliminationRegions(&imgRegions1, &pregions, &nbRegions);
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Elimination regions -> reste %d", nbRegions));
        // elimination des grandes ou non circulaires regions
        // ------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------------------
        // TraitementCiblesVisibles
        ///// §§§ REMPLACER LA PILE DE PISTES PAR UNE LISTE OU TABLE!
        int nbPistes = po_pistes->nbElements();
        //double freqreel = pf_frequence * RAPPORT_FREQ_REELLE_FREQ_VUE;
        // 0. CREER ET ENVOYER LES CIBLES SORTIES MAPS REALOBJECT
        // 1. COMPARER ET METTRE A JOUR LES PISTES EXISTANTES
        // 2. AJOUTER LES NOUVELLES PISTES
        // 3. METTRE A JOUR OU SUPPRIMER LES PISTES INVISIBLES TROP VIEILLES
        xi_logdbgi++;
        
        mv_TraitementCiblesVisibles(&nbPistes, &pregions, &nbRegions);
        xi_logdbgi--;

        // fin de traitement des cibles de l'image courante. On detruit la pile de regions
        //if (xb_modeAllocStatique) LivicPilePoint2D::deleteToutesPiles();
        pregions.vide();
        mv_LOGDEBUG("Regions videes OK");
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Regions traitees"));
        // TraitementCiblesVisibles
        // ------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------------------
        // 3. Mise a jour des pistes invisibles sur l'image courante
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Pistes invisibles (sur %d)", nbPistes));
        xi_logdbgi++;
        mv_TraitementPistesInvisibles(nbPistes);
        xi_logdbgi--;
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Pistes traitees"));
        // 3. Mise a jour des pistes invisibles sur l'image courante
        // ------------------------------------------------------------------------------------------------


        if(!GetBoolProperty("pDecodageVitesse"))
        {
            // ------------------------------------------------------------------------------------------------
            // décodage des codes
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Decodage codes (sur %d)", nbPistes));
            xi_logdbgi++;
            mv_DecodageCode(nbPistes);
            xi_logdbgi--;
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Pistes traitees"));
            // décodage des codes
            // ------------------------------------------------------------------------------------------------
        }
        else
        {
            // ------------------------------------------------------------------------------------------------
            // décodage des codes
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Decodage codes (sur %d)", nbPistes));
            xi_logdbgi++;
            mv_DecodageCodeVariable(nbPistes);
            xi_logdbgi--;
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Pistes traitees"));
            // décodage des codes
            // ------------------------------------------------------------------------------------------------
        
            //mv_sortieVitesses(nbPistes);
        
        }

        if( pb_filtrageReflets)
        {
            // ------------------------------------------------------------------------------------------------
            // Filtrage des reflets sur la route
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Filtrage des reflets"));
            xi_logdbgi++;
            mv_FiltrageReflets(nbPistes);
            xi_logdbgi--;
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Sortie Filtrage des reflets"));
            // Filtrage des reflets sur la route
            // ------------------------------------------------------------------------------------------------
        }
        
        // ------------------------------------------------------------------------------------------------
        // sortie des pistes IR pour l'appli perception
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Sortie des pistes IR pour la perception"));
        xi_logdbgi++;
        mv_SortiePistesIR(nbPistes);
        xi_logdbgi--;
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Sortie pistes IR"));
        // sortie des pistes IR pour l'appli perception
        // ------------------------------------------------------------------------------------------------


		if( pb_affichage )
		{
			// ------------------------------------------------------------------------------------------------
			// recopie dans les images de sortie
			mv_RecopieImageSortie(obS, ib);
			mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Recopie image sortie"));
			if( pb_debug )
			{
				mv_RecopieImagesRegions(obR, obR1, imgRegions, imgRegions1);
				mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Recopie images regions"));
			}
			// recopie dans les images de sortie
			// ------------------------------------------------------------------------------------------------


			// ------------------------------------------------------------------------------------------------
			// Affichages
			mv_AffichageImageSortie(obS, nbRegions, nbPistes);
			mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Affichages"));
			if( pb_debug )
			{
				mv_AffichageImagesRegions(obR, obR1);
				mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Affichage image regions"));
			}
			// Affichages
			// ------------------------------------------------------------------------------------------------
        }

        // ------------------------------------------------------------------------------------------------
        // effacement effectitf des pistes effacees
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Pistes Effacees"));
        xi_logdbgi++;
        mv_EffacementPistesEffacees(&nbPistes);
        xi_logdbgi--;
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Pistes Effacees"));
        // effacement effectitf des pistes effacees
        // ------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------------------
        // StopWritings et frees
        oEltImgSortie->Timestamp() = xt_timestamp;
        StopWriting(oEltImgSortie);
        if( pb_debug )
        {
            oEltImgRegions1->Timestamp() = xt_timestamp;
            StopWriting(oEltImgRegions1);
            oEltImgRegions->Timestamp() = xt_timestamp;
            StopWriting(oEltImgRegions);
        }
        free(imgTmp1);
        free(imgRegions);
        free(imgRegions1);
        // StopWritings et frees
        // ------------------------------------------------------------------------------------------------
    }
    else
    {
        ReportError("Pas de donnees sur l'image d'entree");
    }
    // le gros core
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // stop readings
    StopReading(Input("iImage"));
    // stop readings
    // ------------------------------------------------------------------------------------------------


    xi_logdbgi--;
    mv_LOGDEBUG("Core fin\n");

    if(pb_sortieTempsExecution)
    {
        MAPSIOElt* oEltSortieTempsExecution=StartWriting(Output("oSortieTempsExecution"));  
        
        oEltSortieTempsExecution->Integer() = (MAPSInteger)((MAPS::CurrentTime()-tempsDebut)/10);
        oEltSortieTempsExecution->Timestamp() = xt_timestamp;
        StopWriting(oEltSortieTempsExecution);
    }
    xt_timestampPrec = xt_timestamp;
}


void MAPSLivicDecodageInfrarouge::Death()
{

    // ------------------------------------------------------------------------------------------------
    // calcul du calibrage
    if( LDIRegion::modeCalibrage )
    {
        mv_CalculFinalCalibrage();
    }
    // calcul du calibrage
    // ------------------------------------------------------------------------------------------------

    sprintf(xs_msg, "Pistes rattrapees par l'anciennete : %d", xi_nbPistesRattrapeesAnciennete);
    ReportWarning(xs_msg);
    po_pistes->vide();
    xo_startcode.vide();
    delete po_freqCamera;
    delete po_pistes;
    if (LivicPilePoint2D::modeAllocStatique) LivicPilePoint2D::removeModeAllocationStatique();
    if( pb_stats )
    {
        xo_identifiantsStats.EcritStats();
        xo_identifiantsStats.vide();
    }
    if( pb_debug ) fclose(flog);
}


void MAPSLivicDecodageInfrarouge::mv_LOGDEBUG(const char* logmsg)
{
    if( !pb_debug ) return;
    int indent = xi_logdbgi;
    if( xi_logdbgi>256 )
    {
        indent = 256;
        fprintf(flog, ">>%d", xi_logdbgi);
    }
    for(int i=0; i<indent; i++) fprintf(flog, "  ");
    fprintf(flog, logmsg);
    fprintf(flog, "\n");
    fflush(flog);
}


void MAPSLivicDecodageInfrarouge::mv_LOGDEBUG(int)
{
    if( !pb_debug ) return;
    mv_LOGDEBUG(xs_logdbgmsg);
}

void MAPSLivicDecodageInfrarouge::ReportError(const char *string, int importance)
{
    char ts[MSG_TAILLE];
    sprintf(ts, "%ld", (long)xt_timestamp);
    strcat(ts, " : ");
    strcat(ts, string);
    MAPSComponent::ReportError(ts, importance);
}

void MAPSLivicDecodageInfrarouge::ReportWarning(const char *string, int importance)
{
    char ts[MSG_TAILLE];
    sprintf(ts, "%ld", (long)xt_timestamp);
    strcat(ts, " : ");
    strcat(ts, string);
    MAPSComponent::ReportWarning(ts, importance);
}

void MAPSLivicDecodageInfrarouge::ReportInfo(const char *string, int importance)
{
    char ts[MSG_TAILLE];
    sprintf(ts, "%ld", (long)xt_timestamp);
    strcat(ts, " : ");
    strcat(ts, string);
    MAPSComponent::ReportInfo(ts, importance);
}

void MAPSLivicDecodageInfrarouge::Report(const char *string, int type, int importance)
{
    char ts[MSG_TAILLE];
    sprintf(ts, "%ld", (long)xt_timestamp);
    strcat(ts, " : ");
    strcat(ts, string);
    MAPSComponent::Report(ts, importance);
}
