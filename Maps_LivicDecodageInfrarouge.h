////////////////////////////////
// SDK Programmers guide - Sample
////////////////////////////////

#ifndef _Maps_LivicDecodageInfrarouge_H
#define _Maps_LivicDecodageInfrarouge_H

// Includes maps sdk library header
#include "../commun/libTI/LivicTI.h"
//#include "../../general/utils/livicutils.h"
#include "../../general/utils/livicmacros.h"
#include "IR.h"
#include "maps.hpp"


#define D_TAILLE_FENETRE_PISTE 3          // taille des carres verts affiches en overlay
#define FENETRE_FREQ_MOYENNE 5            // nombre de valeurs de freq instantanee pour calcul freq moy
#define T2I_INTERLIGNE 10                 // interligne pour les ecritures overlay
#define T2I_MARGE 10                      // marge au bord de l'image pour les ecritures overlay
//#define TAILLE_MSG_CODE 10                // taille en bits du message code IR : 3 startbits 110 + n codebits
#define FENETRE_ID_MOYEN 10               // nombre de valeurs d'identifiants pour la majorite d'identifiant
#define COEF_COULEUR_REGION_R 100
#define COEF_COULEUR_REGION_V 164
#define COEF_COULEUR_REGION_B 211
#define FENETRE_CALIBRAGE 5               // nombre de cycles de bits (d'images absente/presente) pour calculer nbImagesParBit en mode calibrage
#define HAUTEUR_QUART_PAL 160 //288
#define LARGEUR_QUART_PAL 240 //384
#define SEUIL_REGION_PONCTUELLE_MIN 25    // taille minimale d'une région considérée comme ponctuelle (5x5 pixels)
#define NBPISTES 100

#define NB_REGIONS_STATIQUES 320*120
#define TAILLE_REGIONS_STATIQUES 320

#define TAILLE_VARIABLE_MAX 30
#define LNG_VITESSE 8


struct LDIDoubleVecteur
{
    double x;
    double y;
};

struct LDIPoint
{
    int x;
    int y;
};

struct LDIDoublePoint
{
    double x;
    double y;
};

class LDICalibrage
{
    public:
    LivicFileDouble *nbImagesPresente;
    LivicFileDouble *nbImagesAbsente;
    double nbImagesParBit;
    LDICalibrage()
    {
        nbImagesPresente = new LivicFileDouble(FENETRE_CALIBRAGE);
        nbImagesAbsente = new LivicFileDouble(FENETRE_CALIBRAGE);
        nbImagesParBit = 0.;
    }
    ~LDICalibrage()
    {
        delete nbImagesPresente;
        delete nbImagesAbsente;
        nbImagesParBit = 0.;
    }
};

class LDIRegion
{
    public:
    static bool modeCalibrage;
    static int nextid;
    static int confianceMax;
    static int penaliteConfianceNouvellePiste;
    static int coefAnciennete;
    static bool activeAnciennete;
    int anciennete;
    int id;
    bool traitee;
    bool effacee;
    enum TypePiste { piste, nouvelle, continu, codeInterdit, noStart, parite, reflet, inconnu } faussePiste;
    LivicPilePoint2D *region;
    LDIDoublePoint bar;
    struct LDIDoubleVecteur vitesse;
    struct LDIDoubleVecteur acceleration;
    int nbImagesPresente;
    int nbImagesAbsente;
    LivicFileDouble *frequence_moy;
    LivicFileShort *code;
    LivicPileInt *codeVariable;
    LivicPileInt *codeVariableTemp;
    int vitesseRecu;
    bool codeModifie;
    int confiance;
    bool codeok;
    bool startCodeDebut, startCodeFin;
    LivicFileShort *identifiant;
    double bits1, bits0;
    int ibits1, ibits0;
    int bitsEnfiles;
    LDICalibrage *calibrage;
	MAPSTimestamp timeout_fausse_piste;

    LDIRegion()
    {
        id = nextid;
        nextid++;
        traitee = false;
        effacee = false;
        faussePiste = nouvelle;
        region = NULL;
        bar.x = bar.y = 0.;
        vitesse.x = vitesse.y = 0.;
        acceleration.x = acceleration.y = 0.;
        nbImagesPresente = nbImagesAbsente = 0;
        frequence_moy = new LivicFileDouble(FENETRE_FREQ_MOYENNE);
        code = new LivicFileShort();
        startCodeDebut = false;
        startCodeFin = false;
        codeVariable = new LivicPileInt(2*TAILLE_VARIABLE_MAX);
        codeVariableTemp = new LivicPileInt();
        vitesseRecu = -1;
        codeModifie = false;
        confiance = confianceMax-penaliteConfianceNouvellePiste;
        anciennete = 0;
        codeok = false;
        identifiant = new LivicFileShort(FENETRE_ID_MOYEN);
        bits1 = bits0 = 0.;
        ibits1 = ibits0 = 0;
        bitsEnfiles = 0;
        if( modeCalibrage )
            calibrage = new LDICalibrage();
        else
            calibrage = NULL;
		timeout_fausse_piste = 0;
    }
    ~LDIRegion()
    {
        if( region )
        {
            if (LivicPilePoint2D::modeAllocStatique) LivicPilePoint2D::deletePile(region);
            else delete region;
        }
        delete frequence_moy;
        delete code;
        delete codeVariable;
        delete identifiant;
        effacee = true;
        if( calibrage ) delete calibrage;
    }
    int LDIRegion::ibarx()
    {
        return (int)(bar.x+.5);
    }
    int LDIRegion::ibary()
    {
        return (int)(bar.y+.5);
    }
    void LDIRegion::SetNonCalibrante()
    {
        if( calibrage ) delete calibrage;
        calibrage = NULL;
    }
    bool LDIRegion::EstCalibrante()
    {
        return ( (!effacee) && (calibrage != NULL) );
    }
    bool LDIRegion::SetFaussePiste(TypePiste tp = inconnu)
    {
        confiance = max(0, confiance - 1);
		
		//On réinitialise la piste si elle est fausse depuis trop longtemps
		//On récupère donc le temps
		if (timeout_fausse_piste == 0)
			timeout_fausse_piste = MAPS::CurrentTime();

        if( activeAnciennete )
        {
            confiance = min( confianceMax, confiance + anciennete*coefAnciennete/confianceMax);
            anciennete = max(0, anciennete - confianceMax/coefAnciennete);
        }
        if( confiance < confianceMax )
        {
            faussePiste = tp;
            codeok = false;
            return true;
        } else if( activeAnciennete )
        {
            faussePiste = piste;
            codeok = true;
            return false;
        }
    }
    bool LDIRegion::SetCodeok()
    {
		//Piste bonne : remise à zéro du timeout de fausse piste
		timeout_fausse_piste = 0;


        if( confiance == 0 ) return false; // la piste est définitivement une fausse (trop d'erreurs)
        int incr = 1;
		// filtre pour les fausses detections
		// si le nombre d'entrees d'identifiant est trop faible, on ne fait rien (la piste doit se confirmer)
		if( identifiant->nbElements()>2 )
            incr = max(1, identifiant->nbOccurrences(identifiant->majorite()));
        confiance = min(confiance + incr, confianceMax);
        if( activeAnciennete )
        {
            anciennete = min(confianceMax, anciennete + confianceMax/coefAnciennete);
        }
		if( identifiant->nbOccurrences(identifiant->majorite())<3 )
		return false;
        if( confiance == confianceMax )
        {
            faussePiste = piste;
            codeok = true;
            return true;
        } else return false;
    }
    char* LDIRegion::PrintConfiance(char *msg)
    {
        sprintf(msg, "piste %d : confiance=%d, anciennete=%d", id, confiance, anciennete);
        return msg;
    }
};

class LDIIdentifiant
{
public:
    int id;
    int anciennete;
    int nbFoisFaussePiste;
    int nbCodeOk;
    int nbFaussePiste;
    int nbFoisRattrapageAnciennete;
    int confianceLaPlusBasse;
    
    LDIIdentifiant(int _id)
    {
        id = _id;
        anciennete = 0;
        nbCodeOk = 0;
        nbFaussePiste = 0;
        nbFoisFaussePiste = 0;
        nbFoisRattrapageAnciennete = 0;
        confianceLaPlusBasse = LDIRegion::confianceMax;
    }
};

class LDIIdentifiantsStats
{
private:
    LivicPilePointeur identifiants;
    LDIIdentifiant* GetIdentifiant(int id)
    {
        int nbElements = identifiants.nbElements();
        LDIIdentifiant **tidentifiants = (LDIIdentifiant**)identifiants.tableau();
        for(int i=0; i<nbElements; i++)
        {
            if( tidentifiants[i]->id == id ) return tidentifiants[i];
        }
        return NULL;
    }
public:
    void vide()
    {
        int nbElements = identifiants.nbElements();
        LDIIdentifiant **tidentifiants = (LDIIdentifiant**)identifiants.tableau();
        for(int i=0; i<nbElements; i++)
            delete tidentifiants[i];
        identifiants.vide();
    }
    void EcritStats()
    {
        FILE *fstats = fopen("C:\\id_stats.txt", "w");
        int nbElements = identifiants.nbElements();
        LDIIdentifiant **tidentifiants = (LDIIdentifiant**)identifiants.tableau();
        for(int i=0; i<nbElements; i++)
        {
            fprintf(fstats, "id : %d\n", tidentifiants[i]->id);
            fprintf(fstats, "  anciennete : %d\n", tidentifiants[i]->anciennete);
            fprintf(fstats, "  nbCodeOk : %d\n", tidentifiants[i]->nbCodeOk);
            fprintf(fstats, "  nbFaussePiste : %d\n", tidentifiants[i]->nbFaussePiste);
            fprintf(fstats, "  nbFoisFaussePiste : %d\n", tidentifiants[i]->nbFoisFaussePiste);
            fprintf(fstats, "  nbFoisRattrapageAnciennete : %d\n", tidentifiants[i]->nbFoisRattrapageAnciennete);
            fprintf(fstats, "  confianceLaPlusBasse : %d\n", tidentifiants[i]->confianceLaPlusBasse);
            fprintf(fstats, "  taux FaussePiste : %.2f\n",
                tidentifiants[i]->anciennete?
                tidentifiants[i]->nbFaussePiste*100./tidentifiants[i]->anciennete:
                0.);
            fprintf(fstats, "  taux CodeOk : %.2f\n",
                tidentifiants[i]->anciennete?
                tidentifiants[i]->nbCodeOk*100./tidentifiants[i]->anciennete:
                0.);
        }
        fclose(fstats);
    }
    void AjouteAnciennete(int id)
    {
        LDIIdentifiant *identifiant = GetIdentifiant(id);
        if( identifiant == NULL )
        {
            identifiant = new LDIIdentifiant(id);
            identifiants.empile(identifiant);
        }
        identifiant->anciennete++;
    }
    void AjouteCodeOk(int id)
    {
        LDIIdentifiant *identifiant = GetIdentifiant(id);
        if( identifiant == NULL )
        {
            identifiant = new LDIIdentifiant(id);
            identifiants.empile(identifiant);
        }
        identifiant->nbCodeOk++;
    }
    void AjouteFaussePiste(int id)
    {
        LDIIdentifiant *identifiant = GetIdentifiant(id);
        if( identifiant == NULL )
        {
            identifiant = new LDIIdentifiant(id);
            identifiants.empile(identifiant);
        }
        identifiant->nbFaussePiste++;
    }
    void AjouteFoisRattrapageAnciennete(int id)
    {
        LDIIdentifiant *identifiant = GetIdentifiant(id);
        if( identifiant == NULL )
        {
            identifiant = new LDIIdentifiant(id);
            identifiants.empile(identifiant);
        }
        identifiant->nbFoisRattrapageAnciennete++;
    }
    void AjouteFoisFaussePiste(int id)
    {
        LDIIdentifiant *identifiant = GetIdentifiant(id);
        if( identifiant == NULL )
        {
            identifiant = new LDIIdentifiant(id);
            identifiants.empile(identifiant);
        }
        identifiant->nbFoisFaussePiste++;
    }
    void SetConfianceBasse(int id, int confiance)
    {
        LDIIdentifiant *identifiant = GetIdentifiant(id);
        if( identifiant == NULL )
        {
            identifiant = new LDIIdentifiant(id);
            identifiants.empile(identifiant);
        }
        if( confiance < identifiant->confianceLaPlusBasse )
            identifiant->confianceLaPlusBasse = confiance;
    }
};

//! The RTMaps input filter for the structure StructureParametresCameras
const MAPSTypeFilterBase MAPSFilterStructureParametresCameras = MAPSFilterUserStructure(StructureParametresCameras);

// Declares a new MAPSComponent child class
class MAPSLivicDecodageInfrarouge : public MAPSComponent 
{
    // Use standard header definition macro
    MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicDecodageInfrarouge)
public:
    bool                              derniereFois;
private:
    // proprietes
    MAPSFloat                         pf_nbImagesParBit;
    MAPSInteger                       pi_tailleMessageCode;
    MAPSInteger                       pi_seuil;
    MAPSInteger                       pi_tailleDilatation;
    MAPSEnumStruct                    pe_formeSpot;
    MAPSFloat                         pf_rapportRectangle;
    MAPSFloat                         pf_seuilRegionForme;
    MAPSInteger                       pi_seuilRegionGrande;
    MAPSInteger                       pi_seuilRegionPonctuelle;
    MAPSFloat                         pf_rapportBinning;
    MAPSFloat                         pf_voisinagex;
    MAPSFloat                         pf_voisinagey;
    MAPSFloat                         pf_echelleAffichageVitesse;
    MAPSFloat                         pf_echelleAffichageAcceleration;
    MAPSInteger                       pi_idPisteCalibrante;
    MAPSInteger                       pi_hauteurHorizon;
    MAPSFloat                         pf_coefTangageHorizon;
    MAPSInteger                       pi_SeuilTempsDetectionRegions;
    bool                              pb_sivic;
    bool                              pb_debug;
    bool                              pb_verbeux;
    bool                              pb_affichage;
    bool                              pb_bitParite;
    bool                              pb_stats;
    bool                              pb_sortieToutesPisteIR;
    bool                              pb_sortieTempsExecution;
    bool                              pb_affichageDetaille;
    bool                              pb_decodageVitesse;
    bool                              pb_filtrageReflets;
    //PIDSeuil bool                              pb_PIDSeuil;
    //PIDSeuil MAPSFloat                         pd_PIDSeuil_Kp;
    //PIDSeuil MAPSFloat                         pd_PIDSeuil_Ki;
    //PIDSeuil MAPSFloat                         pd_PIDSeuil_Kd;
    //PIDSeuil MAPSInteger                       pi_PIDSeuilConsigne;
	

    // données
    char                              xs_msg[MSG_TAILLE];
    int                               xi_largeur, xi_hauteur, xi_taille;
    int                               xi_logdbgi;
    char                              xs_logdbgmsg[MSG_TAILLE];
    bool                              xb_firstTime;
    LivicPilePointeur*                po_pistes;
    LDIIdentifiantsStats              xo_identifiantsStats;
    LivicFileShort                    xo_startcode;
    MAPSTimestamp                     xt_timestamp;
    MAPSTimestamp                     xt_timestampPrec;
	//PIDSeuil double							  xd_E_PIDSeuilPrec;
	//PIDSeuil double							  xd_I_PIDSeuilPrec;
	//PIDSeuil double							  xd_PIDSeuil;
	//PIDSeuil int  							  xi_aff_PID_P;
	//PIDSeuil int			 			    	  xi_aff_PID_I;
	//PIDSeuil int			     				  xi_aff_PID_D;
	//PIDSeuil int			     				  xi_aff_PID_Seuil;
	//PIDSeuil int     						  xi_aff_PID_intensiteMoyenne;
	int                               xi_nbPistesRattrapeesAnciennete;
    LivicFileDouble*                  po_freqCamera;
    bool                              xb_modeAllocStatique;
  
  // méthodes
    void                              mv_LOGDEBUG(const char* logmsg);
    void                              mv_LOGDEBUG(int);
    void                              mv_LitProprietesStatiques();
    void                              mv_LitProprietesDynamiques();
	//PIDSeuil void							  mv_PIDSeuil(unsigned char* imgIn);
    void                              mv_Seuillage(unsigned char* imgIn, unsigned char* imgOut, int seuil, int horizon);
    void                              mv_EliminationsPointsBlancsSivic(unsigned char*imgTmp1);
    void                              mv_EliminationRegions(unsigned char **imgRegions1, LivicPilePointeur *pregions, int *nbRegions);
    void                              mv_TraitementCiblesVisibles(int *nbPistes, LivicPilePointeur *pregions, int *nbRegions);
    LDIRegion*                        mo_RecherchePisteDansVoisinage(int nbPistes, double barx, double bary, double barxerr, double baryerr);
    void                              mv_TraitementPistesInvisibles(int nbPistes);
    void                              mv_DecodageCode(int nbPistes);
    void                              mv_DecodageCodeVariable(int nbPistes);
    /*void                              mv_sortieVitesses(int nbPistes);*/
    void                              mv_EffacementPistesEffacees(int *nbPistes);
    void                              mv_FiltrageReflets(int nbPistes);
    void                              mv_SortiePistesIR(int nbPistes);
    void                              mv_RecopieImageSortie(unsigned char *obS,unsigned char *ib);
    void                              mv_RecopieImagesRegions(unsigned char *obR,unsigned char *obR1,unsigned char *imgRegions,unsigned char *imgRegions1);
    void                              mv_AffichageImagesRegions(unsigned char *obR, unsigned char *obR1);
    void                              mv_AffichageImageSortie(unsigned char *obS, int nbRegions, int nbPistes);
    void                              mv_CalculFinalCalibrage();
    void                              ReportError(const char *string, int importance=0);
    void                              ReportWarning(const char *string, int importance=0);
    void                              ReportInfo(const char *string, int importance=0);
    void                              Report(const char *string, int type=MAPS::Info, int importance=0);
	static void                       Arret(MAPSModule *module, int nbAction);
};

#endif
