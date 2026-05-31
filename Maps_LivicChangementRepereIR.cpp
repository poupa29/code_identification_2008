
#include "MAPS_LivicChangementRepereIR.h"


MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicChangementRepereIR)
    MAPS_INPUT("iPistesIR"      ,MAPS::FilterRealObjects,MAPS::FifoReader)
    MAPS_INPUT("iPistesIRGauche"      ,MAPS::FilterRealObjects,MAPS::FifoReader)
	MAPS_INPUT("iTangageVehicule"   , MAPS::FilterFloat, MAPS::SamplingReader)
	MAPS_INPUT("iVariationHauteurVehicule", MAPS::FilterFloat, MAPS::SamplingReader)
MAPS_END_INPUTS_DEFINITION

MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicChangementRepereIR)
	MAPS_OUTPUT("oPistesIRVehicule",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
	MAPS_OUTPUT("oPistesIRCamEntree",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
	MAPS_OUTPUT("oPistesIRCamSortie",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
	MAPS_OUTPUT("oPistesIRStereo",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
MAPS_END_OUTPUTS_DEFINITION

MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicChangementRepereIR)

	MAPS_PROPERTY_READ_ONLY("----- Lampe : -----------",":")
    MAPS_PROPERTY("pHauteurLampeIRVehicule", 0.6, false, true)
    MAPS_PROPERTY("pHauteurLampeIRInfra", 0.6, false, true)

	MAPS_PROPERTY_READ_ONLY("----- Camera ID : -----------",":")
    MAPS_PROPERTY("pDecalageCameraInX", 0.0, false, true)
    MAPS_PROPERTY("pDecalageCameraInY", -1.4, false, true)
    MAPS_PROPERTY("pDecalageCameraInZ", 0.0, false, true)
    MAPS_PROPERTY("pAlphaUCameraIn", 525.0, false, true)
    MAPS_PROPERTY("pAlphaVCameraIn", 525.0, false, true)
    MAPS_PROPERTY("pTangageCameraIn", 5.0, false, true)
    MAPS_PROPERTY("pLacetCameraIn", 0.0, false, true)
    MAPS_PROPERTY("pResolutionHorizontaleIn", 384, false, true)
    MAPS_PROPERTY("pResolutionVerticaleIn", 288, false, true)

    MAPS_PROPERTY_READ_ONLY("----- Camera out : -----------",":")
    MAPS_PROPERTY("pDecalageCameraOutX", 0.0, false, true)
    MAPS_PROPERTY("pDecalageCameraOutY", -1.4, false, true)
    MAPS_PROPERTY("pDecalageCameraOutZ", 0.0, false, true)
    MAPS_PROPERTY("pResolutionHorizontaleOut", 384, false, true)
    MAPS_PROPERTY("pResolutionVerticaleOut", 288, false, true)
    MAPS_PROPERTY("pAlphaCameraOut", 525.0, false, true)
    MAPS_PROPERTY("pTangageCameraOut", 5.0, false, true)
    MAPS_PROPERTY("pBaseStereo", 1.03, false, true)

    MAPS_PROPERTY_READ_ONLY("----- Stereo ID : -----------",":")
    MAPS_PROPERTY("pUtiliserIdentifiationStereo", false, false, true)
    MAPS_PROPERTY("pUtiliserMonoSiPasStereo", false, false, true)
    MAPS_PROPERTY("pDecalageStereoInX", 0.0, false, true)
    MAPS_PROPERTY("pDecalageStereoInY", -1.4, false, true)
    MAPS_PROPERTY("pDecalageStereoInZ", 0.0, false, true)
    MAPS_PROPERTY("pAlphaUStereoIn", 525.0, false, true)
    MAPS_PROPERTY("pAlphaVStereoIn", 525.0, false, true)
    MAPS_PROPERTY("pBaseStereoIn", 0.2, false, true)
    MAPS_PROPERTY("pTangageStereoIn", 5.0, false, true)
    MAPS_PROPERTY("pLacetStereoIn", 0.0, false, true)
    MAPS_PROPERTY("pResolutionHorizontaleStereoIn", 384, false, true)
    MAPS_PROPERTY("pResolutionVerticaleStereoIn", 288, false, true)

   MAPS_PROPERTY_READ_ONLY("----- Objets Observes : -----------",":")
   MAPS_PROPERTY("pLargeurVehicules", 3.0, false, true)
   MAPS_PROPERTY("pUtiliserHauteurVehicule", false, false, true)
   MAPS_PROPERTY("pHauteurVehicules", 1.6, false, true)
   MAPS_PROPERTY("pProfondeurVehicules", 2.0, false, true)

    MAPS_PROPERTY_READ_ONLY("----- Filtrages : -----------",":")
    MAPS_PROPERTY("pPorteeMax", 350.0, false, true)
    MAPS_PROPERTY("pSupprimerValeursAberrantes", false, false, true)
    MAPS_PROPERTY_ENUM("pFiltrageSortie","Aucun|PasseBas|Median", 0, false,true)
    MAPS_PROPERTY("pFrequenceCoupure", 10.0, false, true)
MAPS_END_PROPERTIES_DEFINITION

MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicChangementRepereIR)
MAPS_END_ACTIONS_DEFINITION

MAPS_COMPONENT_DEFINITION(
    MAPSLivicChangementRepereIR,
    "LivicChangementRepereIR",
    "$Revision: 1.23 $",
    128,
    MAPS::Threaded,
    MAPS::Threaded,
    4,
    4,
    45,
    0)


                          
void MAPSLivicChangementRepereIR::Birth()
{
    xb_firstTime = true;
}

MAPSTimestamp MAPSLivicChangementRepereIR::mtsp_PistesIRSuivant()
{
    if( xo_pistesIRin ) StopReading(Input("iPistesIR"));
    MAPSIOElt *iEltPistesIR = StartReading(Input("iPistesIR"));
    xo_pistesIRin = &iEltPistesIR->RealObject();
    xi_nbPistesIR = MIN(iEltPistesIR->VectorSize(), MAX_CIBLES_IR);
    return iEltPistesIR->Timestamp();
}


void MAPSLivicChangementRepereIR::Core() 
{
    xi_nbPistesIR = 0;
    xo_pistesIRin = NULL;
    xtsp_timestampPistesIR = 0;



    // ------------------------------------------------------------------------------------------------
    // lecture des pistes IR
    if( IsConnected(Input("iPistesIR")) )
    {
        xtsp_timestampPistesIR = mtsp_PistesIRSuivant();
    }

    //bool xb_IdentificationStereo = false;

    //if (IsConnected(Input("iPistesIRCameraGauche")) && GetBoolProperty("pIdentificationStereo"))
    //{
    //    xb_IdentificationStereo = true;

    //    MAPSIOElt *iEltPistesIRGauche = StartReading(Input("iPistesIRCameraGauche"));
    //    xo_pistesIRGauchein = &iEltPistesIRGauche->RealObject();
    //    xi_nbPistesIRGauche = MIN(iEltPistesIRGauche->VectorSize(), MAX_CIBLES_IR);

    //}
    //else
    //    xb_IdentificationStereo = false;


    // lecture des pistes IR
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // lecture des pistes Stereo IR
    bool	xb_UtiliserIdentificationStereo = false;
    bool    xb_UtiliserMonoSiPasStereo      = false;
    if( IsConnected(Input("iPistesIRGauche")) && GetBoolProperty("pUtiliserIdentifiationStereo"))
    {
        if( xo_pistesIRinGauche ) StopReading(Input("iPistesIRGauche"));
        MAPSIOElt *iEltPistesIRGauche = StartReading(Input("iPistesIRGauche"));
        xo_pistesIRinGauche = &iEltPistesIRGauche->RealObject();
        xi_nbPistesIRGauche = MIN(iEltPistesIRGauche->VectorSize(), MAX_CIBLES_IR);
        xb_UtiliserIdentificationStereo = true;
        xb_UtiliserMonoSiPasStereo      = GetBoolProperty("pUtiliserMonoSiPasStereo");
    }
    // lecture des pistes IR
    // ------------------------------------------------------------------------------------------------


    //  -----------------------------------------------------------------------------------------------
    //  Recuperation de la hauteur des cameras et du tangage:
    //  -----------------------------------------------------
    if (DataAvailableInFIFO(Input("iTangageVehicule")))
    {
        MAPSIOElt *ioElt = StartReading(Input("iTangageVehicule"));
        xd_TangageVehicule = ioElt->Float();
        StopReading(Input("iTangageVehicule"));
    }
    else
        xd_TangageVehicule = 0;


    if (DataAvailableInFIFO(Input("iVariationHauteurVehicule")))
    {
        MAPSIOElt *ioElt = StartReading(Input("iVariationHauteurVehicule"));
        xd_VariationHauteurVehicule = ioElt->Float();
        StopReading(Input("iVariationHauteurVehicule"));
    }
    else
        xd_VariationHauteurVehicule = 0;

    //  -----------------------------------------------------------------------------------------------





    // ------------------------------------------------------------------------------------------------
    // le first time
    if (xb_firstTime)
    {
        xi_NbAnciennePistes  = 0;
        xi_NbPlusAnciennePistes  = 0;
        xb_firstTime=false;
    }
    // le first time
    // ------------------------------------------------------------------------------------------------





    //  -----------------------------------------------------------------------------------------------
    //  Ouverture des sorties
    MAPSIOElt *IOEltSortiePistesVehicule = StartWriting(Output("oPistesIRVehicule"));
    MAPSRealObject *xo_pistesIRout1 = &IOEltSortiePistesVehicule->RealObject();
    IOEltSortiePistesVehicule->VectorSize() = xi_nbPistesIR;
    IOEltSortiePistesVehicule->Timestamp() = xtsp_timestampPistesIR;

    MAPSIOElt *IOEltSortiePistesCamEntree = StartWriting(Output("oPistesIRCamEntree"));
    MAPSRealObject *xo_pistesIRout2 = &IOEltSortiePistesCamEntree->RealObject();
    IOEltSortiePistesCamEntree->VectorSize() = xi_nbPistesIR;
    IOEltSortiePistesCamEntree->Timestamp() = xtsp_timestampPistesIR;

    MAPSIOElt *IOEltSortiePistesCamSortie = StartWriting(Output("oPistesIRCamSortie"));
    MAPSRealObject *xo_pistesIRout3 = &IOEltSortiePistesCamSortie->RealObject();
    IOEltSortiePistesCamSortie->VectorSize() = xi_nbPistesIR;
    IOEltSortiePistesCamSortie->Timestamp() = xtsp_timestampPistesIR;

    MAPSIOElt *IOEltSortiePistesStereo = StartWriting(Output("oPistesIRStereo"));
    MAPSRealObject *xo_pistesIRout4 = &IOEltSortiePistesStereo->RealObject();
    IOEltSortiePistesStereo->VectorSize() = xi_nbPistesIR;
    IOEltSortiePistesStereo->Timestamp() = xtsp_timestampPistesIR;
    //  Ouverture des sorties
    //  -----------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    //  Recuperation des propriétés
    double  pHauteurLampeIRVehicule     = GetFloatProperty("pHauteurLampeIRVehicule");
    double  pHauteurLampeIRInfra        = GetFloatProperty("pHauteurLampeIRInfra");
    double  pDecalageCameraInX          = GetFloatProperty("pDecalageCameraInX");
    double  pDecalageCameraInY          = GetFloatProperty("pDecalageCameraInY") - xd_VariationHauteurVehicule;
    double  pDecalageCameraInZ          = GetFloatProperty("pDecalageCameraInZ");
    double  pAlphaUCameraIn              = GetFloatProperty("pAlphaUCameraIn");
    double  pAlphaVCameraIn              = GetFloatProperty("pAlphaVCameraIn");
    double  pTangageCameraIn            = GetFloatProperty("pTangageCameraIn") + xd_TangageVehicule;
    double  pLacetCameraIn              = GetFloatProperty("pLacetCameraIn");

    double  pDecalageStereoInX          = GetFloatProperty("pDecalageStereoInX");
    double  pDecalageStereoInY          = GetFloatProperty("pDecalageStereoInY") - xd_VariationHauteurVehicule;
    double  pDecalageStereoInZ          = GetFloatProperty("pDecalageStereoInZ");
    double  pAlphaUStereoIn             = GetFloatProperty("pAlphaUStereoIn");
    double  pAlphaVStereoIn             = GetFloatProperty("pAlphaVStereoIn");
    double  pBaseStereoIn               = GetFloatProperty("pBaseStereoIn");
    double  pTangageStereoIn            = GetFloatProperty("pTangageStereoIn") + xd_TangageVehicule;
    double  pLacetStereoIn              = GetFloatProperty("pLacetStereoIn");

    double  pDecalageCameraOutX         = GetFloatProperty("pDecalageCameraOutX");
    double  pDecalageCameraOutY         = GetFloatProperty("pDecalageCameraOutY") - xd_VariationHauteurVehicule;
    double  pDecalageCameraOutZ         = GetFloatProperty("pDecalageCameraOutZ");
    int     pResolutionHorizontaleIn    = (int)GetIntegerProperty("pResolutionHorizontaleIn");
    int     pResolutionVerticaleIn      = (int)GetIntegerProperty("pResolutionVerticaleIn");
    int     pResolutionHorizontaleStereoIn    = (int)GetIntegerProperty("pResolutionHorizontaleStereoIn");
    int     pResolutionVerticaleStereoIn      = (int)GetIntegerProperty("pResolutionVerticaleStereoIn");
    int     pResolutionHorizontaleOut   = (int)GetIntegerProperty("pResolutionHorizontaleOut");
    int     pResolutionVerticaleOut     = (int)GetIntegerProperty("pResolutionVerticaleOut");
    double  pAlphaCameraOut             = GetFloatProperty("pAlphaCameraOut");
    double  pTangageCameraOut           = GetFloatProperty("pTangageCameraOut") + xd_TangageVehicule;
    double  pBaseStereo                 = GetFloatProperty("pBaseStereo");

    int     VoIn                        = pResolutionVerticaleIn/2;
    int     UoIn                        = pResolutionHorizontaleIn/2;
    int     VoStereoIn                  = pResolutionVerticaleStereoIn/2;
    int     UoStereoIn                  = pResolutionHorizontaleStereoIn/2;
    int     VoOut                       = pResolutionVerticaleOut/2;
    int     UoOut                       = pResolutionHorizontaleOut/2;

	double		pLargeurVehicules			= GetFloatProperty("pLargeurVehicules");
	double		pHauteurVehicules			= GetFloatProperty("pHauteurVehicules");
	double		pProfondeurVehicules		= GetFloatProperty("pProfondeurVehicules");

    double  porteeMax                   = GetFloatProperty("pPorteeMax");
    MAPSEnumStruct pFiltrageSortie      = GetEnumProperty("pFiltrageSortie");
    double pFrequenceCoupure            = GetFloatProperty("pFrequenceCoupure");
    double  tempsBoucle                 = 40.0;
    // ------------------------------------------------------------------------------------------------


    // ------------------------------------------------------------------------------------------------
    // Changement de repere des pistes:
    double u,v,x,y,z,vh;
    double cosTIn   = cos(pTangageCameraIn*PI/180.0);
    double cosTOut  = cos(pTangageCameraOut*PI/180.0);
    double sinTIn   = sin(pTangageCameraIn*PI/180.0);
    double sinTOut  = sin(pTangageCameraOut*PI/180.0);
    double tanTIn   = sinTIn/cosTIn;

    double cosLacetIn = cos(pLacetCameraIn*PI/180.0);
    double sinLacetIn = sin(pLacetCameraIn*PI/180.0);

    double cosLacetStereoIn = cos(pLacetStereoIn*PI/180.0);
    double sinLacetStereoIn = sin(pLacetStereoIn*PI/180.0);

    for(int i=0; i<xi_nbPistesIR; i++)
    {
        bool    supprimerPiste = false;

        //  recopie des pistes sur les sorties:
        LivicMapsUtils::copieRealObject(&xo_pistesIRin[i], &xo_pistesIRout1[i]);
        LivicMapsUtils::copieRealObject(&xo_pistesIRin[i], &xo_pistesIRout2[i]);
        LivicMapsUtils::copieRealObject(&xo_pistesIRin[i], &xo_pistesIRout3[i]);
        LivicMapsUtils::copieRealObject(&xo_pistesIRin[i], &xo_pistesIRout4[i]);

		//	Recuperation de l'identifiant:
		long id = (long)xo_pistesIRin[i].id;


        //  recupération de la position dans l'image d'identification pour le calcul de localisation:
        u = xo_pistesIRin[i].x;
        v = xo_pistesIRin[i].y;
        
		if (typeIdentifiant(id) == TYPE_ID_INFRA)
			y = -pHauteurLampeIRInfra;
		else
			y = -pHauteurLampeIRVehicule;
       
        //  Localisation en utilisant la hauteur de la lampe:
        double ztemp = (y-pDecalageCameraInY) * ( pAlphaVCameraIn*cosTIn - (v-VoIn)*sinTIn ) / ( (v-VoIn)*cosTIn + pAlphaVCameraIn*sinTIn );
        double xtemp = (pAlphaVCameraIn/pAlphaUCameraIn)*((y-pDecalageCameraInY)*(u-UoIn)/((v-VoIn)*cosTIn + pAlphaVCameraIn*sinTIn ));

        z = pDecalageCameraInZ + ztemp*cosLacetIn + xtemp*sinLacetIn;   //  prise en compte du lacet.
        x = pDecalageCameraInX - ztemp*sinLacetIn + xtemp*cosLacetIn;
        vh = pAlphaVCameraIn * (-y)*z*( tanTIn*tanTIn+1 ) / ( (z*tanTIn-pDecalageCameraInY)*((z*tanTIn-pDecalageCameraInY)+y) );  

        //  Localisation par stereovision:
        if (xb_UtiliserIdentificationStereo)
        {
            for(int ig=0; ig<xi_nbPistesIRGauche; ig++)
            {
                if(xo_pistesIRinGauche[i].id == id)
                {
                    double ug = xo_pistesIRinGauche[i].x;
                    double vg = xo_pistesIRinGauche[i].y;
                    double vv = (v+vg)*0.5;
                    double disp = ug-u;

                    if (disp>EPSILON)
                    {
                        xtemp = pBaseStereoIn*(u-UoStereoIn)/disp;
                        ztemp = (pBaseStereoIn*pAlphaUStereoIn/pAlphaVStereoIn)*(pAlphaVStereoIn*cos(pTangageStereoIn*PIsur180) - (vv-VoStereoIn)*sin(pTangageStereoIn*PIsur180))/disp;

                        z = pDecalageStereoInZ + ztemp*cosLacetStereoIn + xtemp*sinLacetStereoIn;
                        x = pDecalageStereoInX - ztemp*sinLacetStereoIn + xtemp*cosLacetStereoIn;
                        y = pDecalageStereoInY + (pBaseStereoIn*pAlphaUStereoIn/pAlphaVStereoIn)*((vv-VoStereoIn)*cos(pTangageStereoIn*PIsur180) + pAlphaVStereoIn*sin(pTangageStereoIn*PIsur180))/disp;
                     }
                    else if (!xb_UtiliserMonoSiPasStereo)
                        supprimerPiste = true;
                }
                else if (!xb_UtiliserMonoSiPasStereo)
                    supprimerPiste = true;
            }
        }

        //  limitation de la portée à pPorteeMax
        if( (z<0) || (z>porteeMax) )
        {
            z = porteeMax;
            if ( GetBoolProperty("pSupprimerValeursAberrantes") )
                supprimerPiste = true;
        }

        //	Recopie de la taille des véhicules rn prenant en compte les incertitudes sur les dimensions en fonction de la distance:
		if (typeIdentifiant(id) == TYPE_ID_VEHICULE)
		{
			xo_pistesIRout1[i].vehicle.width		= pLargeurVehicules + z*0.05;
			xo_pistesIRout1[i].vehicle.height		= pHauteurVehicules;
			xo_pistesIRout1[i].vehicle.length		= pProfondeurVehicules + z*0.1;
		}

        //  rejet éventuel de pistes
        if (supprimerPiste)
        {
            xo_pistesIRout1[i].id = 0;
            xo_pistesIRout2[i].id = 0;
            xo_pistesIRout3[i].id = 0;
            xo_pistesIRout4[i].id = 0;
        }


        //  repere vehicule
        xo_pistesIRout1[i].x = x;
        xo_pistesIRout1[i].y = y;
        xo_pistesIRout1[i].z = z;

        xo_pistesIRout1[i].vehicle.dx = 0.0;
        xo_pistesIRout1[i].vehicle.dy = 0.0;
        xo_pistesIRout1[i].vehicle.dz = 0.0;

		if (!GetBoolProperty("pUtiliserHauteurVehicule"))
	        xo_pistesIRout1[i].vehicle.height = y; // hauteur du vehicule/lampe


        //  repere camera entree
        xo_pistesIRout2[i].x = u;
        xo_pistesIRout2[i].y = v;
        xo_pistesIRout2[i].z = 0.0;
        xo_pistesIRout2[i].vehicle.height = 0;//vh; // hauteur du vehicule/lampe

        //  repere camera sortie
        xo_pistesIRout3[i].x = UoOut + pAlphaCameraOut*(x-pDecalageCameraOutX) / ( (y-pDecalageCameraOutY)*sinTOut + (z-pDecalageCameraOutZ)*cosTOut );
        xo_pistesIRout3[i].y = ( (y-pDecalageCameraOutY)*(pAlphaCameraOut*cosTOut + VoOut*sinTOut) + (z-pDecalageCameraOutZ)*(VoOut*cosTOut - pAlphaCameraOut*sinTOut) ) / ( (y-pDecalageCameraOutY)*sinTOut + (z-pDecalageCameraOutZ)*cosTOut );
        xo_pistesIRout3[i].z = 0.0;

        //  repere capteur stereo
        xo_pistesIRout4[i].x = UoOut + pAlphaCameraOut*(x-pDecalageCameraOutX-pBaseStereo/2.0) / ( (y-pDecalageCameraOutY)*sinTOut + (z-pDecalageCameraOutZ)*cosTOut );
        xo_pistesIRout4[i].y = ( (y-pDecalageCameraOutY)*(pAlphaCameraOut*cosTOut + VoOut*sinTOut) + (z-pDecalageCameraOutZ)*(VoOut*cosTOut - pAlphaCameraOut*sinTOut) ) / ( (y-pDecalageCameraOutY)*sinTOut + (z-pDecalageCameraOutZ)*cosTOut );
        xo_pistesIRout4[i].z = (pAlphaCameraOut*pBaseStereo) / ( (y-pDecalageCameraOutY)*sinTOut + (z-pDecalageCameraOutZ)*cosTOut );



        // Filtrage eventuel des pistes:
        //  ---------------------------------
        switch (pFiltrageSortie.selectedEnum)
        {
            //  Pas de filtrage:
        case AUCUN_FILTRE:
            break;
            //  ----------------

            // Filtrage passe bas:
        case FILTRE_PASSEBAS:
            {
                //  recherche de la piste qui correspond au pas de temps précédent:
                bool xb_PisteConnue = false;
                double xInOld1,xInOld2,xInOld3,xInOld4, xOutOld1,xOutOld2,xOutOld3,xOutOld4, zInOld1,zInOld2,zInOld3,zInOld4, zOutOld1,zOutOld2,zOutOld3,zOutOld4, xNew1,xNew2,xNew3,xNew4, zNew1,zNew2,zNew3,zNew4;
                for (int noAnciennePiste = 0; noAnciennePiste <xi_NbAnciennePistes; ++noAnciennePiste )
                {
                    if (pi_IdAnciennesPistes[noAnciennePiste ] == xo_pistesIRout1[i].id)
                    {
                        xb_PisteConnue = true;
                        xInOld1 = pd_XAnciennesPistesEntree1[noAnciennePiste];
                        zInOld1 = pd_ZAnciennesPistesEntree1[noAnciennePiste];
                        xInOld2 = pd_XAnciennesPistesEntree2[noAnciennePiste];
                        zInOld2 = pd_ZAnciennesPistesEntree2[noAnciennePiste];
                        xInOld3 = pd_XAnciennesPistesEntree3[noAnciennePiste];
                        zInOld3 = pd_ZAnciennesPistesEntree3[noAnciennePiste];
                        xInOld4 = pd_XAnciennesPistesEntree4[noAnciennePiste];
                        zInOld4 = pd_ZAnciennesPistesEntree4[noAnciennePiste];


                        xOutOld1 = pd_XAnciennesPistesSortie1[noAnciennePiste];
                        zOutOld1 = pd_ZAnciennesPistesSortie1[noAnciennePiste];
                        xOutOld2 = pd_XAnciennesPistesSortie2[noAnciennePiste];
                        zOutOld2 = pd_ZAnciennesPistesSortie2[noAnciennePiste];
                        xOutOld3 = pd_XAnciennesPistesSortie3[noAnciennePiste];
                        zOutOld3 = pd_ZAnciennesPistesSortie3[noAnciennePiste];
                        xOutOld4 = pd_XAnciennesPistesSortie4[noAnciennePiste];
                        zOutOld4 = pd_ZAnciennesPistesSortie4[noAnciennePiste];
                        break;
                    }
                }

                if (xb_PisteConnue) //  si la piste était connue au temps précédent, on procède au filtrage:
                {
                    double Wac = tan(3.1415927*(tempsBoucle/1000.)*pFrequenceCoupure);
                    xNew1 = (Wac*(xo_pistesIRout1[i].x + xInOld1)-((Wac-1)*xOutOld1))/(Wac+1);
                    zNew1 = (Wac*(xo_pistesIRout1[i].z + zInOld1)-((Wac-1)*zOutOld1))/(Wac+1);
                    xNew2 = (Wac*(xo_pistesIRout2[i].x + xInOld2)-((Wac-1)*xOutOld2))/(Wac+1);
                    zNew2 = (Wac*(xo_pistesIRout2[i].z + zInOld2)-((Wac-1)*zOutOld2))/(Wac+1);
                    xNew3 = (Wac*(xo_pistesIRout3[i].x + xInOld3)-((Wac-1)*xOutOld3))/(Wac+1);
                    zNew3 = (Wac*(xo_pistesIRout3[i].z + zInOld3)-((Wac-1)*zOutOld3))/(Wac+1);
                    xNew4 = (Wac*(xo_pistesIRout4[i].x + xInOld4)-((Wac-1)*xOutOld4))/(Wac+1);
                    zNew4 = (Wac*(xo_pistesIRout4[i].z + zInOld4)-((Wac-1)*zOutOld4))/(Wac+1);

                    pd_SauvegardePistesFiltrees[4*i]    = xo_pistesIRout1[i].x;
                    pd_SauvegardePistesFiltrees[4*i+1]  = xNew1;
                    pd_SauvegardePistesFiltrees[4*i+2]  = xo_pistesIRout1[i].z;
                    pd_SauvegardePistesFiltrees[4*i+3]  = zNew1;
                    pd_SauvegardePistesFiltrees[4*i+4]    = xo_pistesIRout2[i].x;
                    pd_SauvegardePistesFiltrees[4*i+5]  = xNew2;
                    pd_SauvegardePistesFiltrees[4*i+6]  = xo_pistesIRout2[i].z;
                    pd_SauvegardePistesFiltrees[4*i+7]  = zNew2;
                    pd_SauvegardePistesFiltrees[4*i+8]    = xo_pistesIRout3[i].x;
                    pd_SauvegardePistesFiltrees[4*i+9]  = xNew3;
                    pd_SauvegardePistesFiltrees[4*i+10]  = xo_pistesIRout3[i].z;
                    pd_SauvegardePistesFiltrees[4*i+11]  = zNew3;
                    pd_SauvegardePistesFiltrees[4*i+12]    = xo_pistesIRout4[i].x;
                    pd_SauvegardePistesFiltrees[4*i+13]  = xNew4;
                    pd_SauvegardePistesFiltrees[4*i+14]  = xo_pistesIRout4[i].z;
                    pd_SauvegardePistesFiltrees[4*i+15]  = zNew4;
                    xo_pistesIRout1[i].x = xNew1;
                    xo_pistesIRout1[i].z = zNew1;
                    xo_pistesIRout2[i].x = xNew2;
                    xo_pistesIRout2[i].z = zNew2;
                    xo_pistesIRout3[i].x = xNew3;
                    xo_pistesIRout3[i].z = zNew3;
                    xo_pistesIRout4[i].x = xNew4;
                    xo_pistesIRout4[i].z = zNew4;
                }
                else
                {
                    pd_SauvegardePistesFiltrees[4*i]    = xo_pistesIRout1[i].x;
                    pd_SauvegardePistesFiltrees[4*i+1]  = xo_pistesIRout1[i].x;
                    pd_SauvegardePistesFiltrees[4*i+2]  = xo_pistesIRout1[i].z;
                    pd_SauvegardePistesFiltrees[4*i+3]  = xo_pistesIRout1[i].z;
                    pd_SauvegardePistesFiltrees[4*i+4]    = xo_pistesIRout2[i].x;
                    pd_SauvegardePistesFiltrees[4*i+5]  = xo_pistesIRout2[i].x;
                    pd_SauvegardePistesFiltrees[4*i+6]  = xo_pistesIRout2[i].z;
                    pd_SauvegardePistesFiltrees[4*i+7]  = xo_pistesIRout2[i].z;
                    pd_SauvegardePistesFiltrees[4*i+8]    = xo_pistesIRout3[i].x;
                    pd_SauvegardePistesFiltrees[4*i+9]  = xo_pistesIRout3[i].x;
                    pd_SauvegardePistesFiltrees[4*i+10]  = xo_pistesIRout3[i].z;
                    pd_SauvegardePistesFiltrees[4*i+11]  = xo_pistesIRout3[i].z;
                    pd_SauvegardePistesFiltrees[4*i+12]    = xo_pistesIRout4[i].x;
                    pd_SauvegardePistesFiltrees[4*i+13]  = xo_pistesIRout4[i].x;
                    pd_SauvegardePistesFiltrees[4*i+14]  = xo_pistesIRout4[i].z;
                    pd_SauvegardePistesFiltrees[4*i+15]  = xo_pistesIRout4[i].z;
                }

                break;
            }
            //  ---------------------------------------

            // Filtrage median 3:
        case FILTRE_MEDIAN:
            {
                //  recherche de la piste qui correspond au pas de temps précédent:
                bool xb_PisteConnue = false;
                double xInOld1,xInOld2,xInOld3,xInOld4, zInOld1,zInOld2,zInOld3,zInOld4;
                double xInPlusOld1,xInPlusOld2,xInPlusOld3,xInPlusOld4, zInPlusOld1,zInPlusOld2,zInPlusOld3,zInPlusOld4; 
                for (int noAnciennePiste = 0; noAnciennePiste <xi_NbAnciennePistes; ++noAnciennePiste )
                {
                    if (pi_IdAnciennesPistes[noAnciennePiste ] == xo_pistesIRout1[i].id)
                    {
                        xInOld1 = pd_XAnciennesPistesEntree1[noAnciennePiste];
                        zInOld1 = pd_ZAnciennesPistesEntree1[noAnciennePiste];
                        xInOld2 = pd_XAnciennesPistesEntree2[noAnciennePiste];
                        zInOld2 = pd_ZAnciennesPistesEntree2[noAnciennePiste];
                        xInOld3 = pd_XAnciennesPistesEntree3[noAnciennePiste];
                        zInOld3 = pd_ZAnciennesPistesEntree3[noAnciennePiste];
                        xInOld4 = pd_XAnciennesPistesEntree4[noAnciennePiste];
                        zInOld4 = pd_ZAnciennesPistesEntree4[noAnciennePiste];

                        for (int noPlusAnciennePiste = 0; noPlusAnciennePiste <xi_NbPlusAnciennePistes; ++noPlusAnciennePiste )
                        {
                            if (pi_IdPlusAnciennesPistes[noPlusAnciennePiste] == xo_pistesIRout1[i].id)
                            {
                                xb_PisteConnue = true;
                                xInPlusOld1 = pd_XPlusAnciennesPistesEntree1[noPlusAnciennePiste];
                                zInPlusOld1 = pd_ZPlusAnciennesPistesEntree1[noPlusAnciennePiste];
                                xInPlusOld2 = pd_XPlusAnciennesPistesEntree2[noPlusAnciennePiste];
                                zInPlusOld2 = pd_ZPlusAnciennesPistesEntree2[noPlusAnciennePiste];
                                xInPlusOld3 = pd_XPlusAnciennesPistesEntree3[noPlusAnciennePiste];
                                zInPlusOld3 = pd_ZPlusAnciennesPistesEntree3[noPlusAnciennePiste];
                                xInPlusOld4 = pd_XPlusAnciennesPistesEntree4[noPlusAnciennePiste];
                                zInPlusOld4 = pd_ZPlusAnciennesPistesEntree4[noPlusAnciennePiste];

                                break;
                            }
                        }
                        break;
                    }
                }

                pd_SauvegardePistesFiltrees[2*i]    = xo_pistesIRout1[i].x;
                pd_SauvegardePistesFiltrees[2*i+1]  = xo_pistesIRout1[i].z;
                pd_SauvegardePistesFiltrees[2*i+2]    = xo_pistesIRout2[i].x;
                pd_SauvegardePistesFiltrees[2*i+3]  = xo_pistesIRout2[i].z;
                pd_SauvegardePistesFiltrees[2*i+4]    = xo_pistesIRout3[i].x;
                pd_SauvegardePistesFiltrees[2*i+5]  = xo_pistesIRout3[i].z;
                pd_SauvegardePistesFiltrees[2*i+6]    = xo_pistesIRout4[i].x;
                pd_SauvegardePistesFiltrees[2*i+7]  = xo_pistesIRout4[i].z;

                if (xb_PisteConnue) //  si la piste était connue au temps précédent, on procède au filtrage:
                {
                    //  on recherche la valeur mediane:
                    double xmed1 = 0.0;
                    double zmed1 = 0.0;
                    double xmed2 = 0.0;
                    double zmed2 = 0.0;
                    double xmed3 = 0.0;
                    double zmed3 = 0.0;
                    double xmed4 = 0.0;
                    double zmed4 = 0.0;

                    if (zInPlusOld1>zInOld1)
                    {
                        if(xo_pistesIRout1[i].z < zInOld1)
                        {
                            xmed1 = xInOld1;
                            zmed1 = zInOld1;
                        }
                        else if (xo_pistesIRout1[i].z < zInPlusOld1)
                        {
                            xmed1 = xo_pistesIRout1[i].x;
                            zmed1 = xo_pistesIRout1[i].z;
                        }
                        else
                        {
                            xmed1 = xInPlusOld1;
                            zmed1 = zInPlusOld1;
                        }
                        if(xo_pistesIRout2[i].z < zInOld2)
                        {
                            xmed2 = xInOld1;
                            zmed2 = zInOld1;
                        }
                        else if (xo_pistesIRout2[i].z < zInPlusOld2)
                        {
                            xmed2 = xo_pistesIRout1[i].x;
                            zmed2 = xo_pistesIRout1[i].z;
                        }
                        else
                        {
                            xmed2 = xInPlusOld2;
                            zmed2 = zInPlusOld2;
                        }
                        if(xo_pistesIRout3[i].z < zInOld3)
                        {
                            xmed3 = xInOld3;
                            zmed3 = zInOld3;
                        }
                        else if (xo_pistesIRout3[i].z < zInPlusOld3)
                        {
                            xmed3 = xo_pistesIRout1[i].x;
                            zmed3 = xo_pistesIRout1[i].z;
                        }
                        else
                        {
                            xmed3 = xInPlusOld3;
                            zmed3 = zInPlusOld3;
                        }
                        if(xo_pistesIRout4[i].z < zInOld4)
                        {
                            xmed4 = xInOld4;
                            zmed4 = zInOld4;
                        }
                        else if (xo_pistesIRout4[i].z < zInPlusOld4)
                        {
                            xmed4 = xo_pistesIRout1[i].x;
                            zmed4 = xo_pistesIRout1[i].z;
                        }
                        else
                        {
                            xmed4 = xInPlusOld4;
                            zmed4 = zInPlusOld4;
                        }
                    }
                    else
                    {
                        if(xo_pistesIRout1[i].z < zInPlusOld1)
                        {
                            xmed1 = xInPlusOld1;
                            zmed1 = zInPlusOld1;
                        }
                        else if (xo_pistesIRout1[i].z < zInOld1)
                        {
                            xmed1 = xo_pistesIRout1[i].x;
                            zmed1 = xo_pistesIRout1[i].z;
                        }
                        else
                        {
                            xmed1 = xInOld1;
                            zmed1 = zInOld1;
                        }
                        if(xo_pistesIRout2[i].z < zInPlusOld2)
                        {
                            xmed2 = xInPlusOld2;
                            zmed2 = zInPlusOld2;
                        }
                        else if (xo_pistesIRout2[i].z < zInOld2)
                        {
                            xmed2 = xo_pistesIRout1[i].x;
                            zmed2 = xo_pistesIRout1[i].z;
                        }
                        else
                        {
                            xmed2 = xInOld2;
                            zmed2 = zInOld2;
                        }
                        if(xo_pistesIRout3[i].z < zInPlusOld3)
                        {
                            xmed3 = xInPlusOld3;
                            zmed3 = zInPlusOld3;
                        }
                        else if (xo_pistesIRout3[i].z < zInOld3)
                        {
                            xmed3 = xo_pistesIRout1[i].x;
                            zmed3 = xo_pistesIRout1[i].z;
                        }
                        else
                        {
                            xmed3 = xInOld3;
                            zmed3 = zInOld3;
                        }
                        if(xo_pistesIRout4[i].z < zInPlusOld4)
                        {
                            xmed4 = xInPlusOld4;
                            zmed4 = zInPlusOld4;
                        }
                        else if (xo_pistesIRout4[i].z < zInOld4)
                        {
                            xmed4 = xo_pistesIRout1[i].x;
                            zmed4 = xo_pistesIRout1[i].z;
                        }
                        else
                        {
                            xmed4 = xInOld4;
                            zmed4 = zInOld4;
                        }
                    }
                }

                break;
            }
            //  ---------------------------------------



            //  --------------------------
        default:
            break;
            //  --------------------------
        }

        // ------------------------------------------------------------------------------------------------


        //  S'il y avait filtrage, on recopie les valeurs en entrée et filtrées pour calculer le filtrage à l'instant suivant:
        if (pFiltrageSortie.selectedEnum == FILTRE_PASSEBAS)
        {
            for (i=0; i<xi_nbPistesIR; ++i)
            {
                pi_IdAnciennesPistes[i] = xo_pistesIRout1[i].id;
                pd_XAnciennesPistesEntree1[i] = pd_SauvegardePistesFiltrees[4*i];
                pd_XAnciennesPistesSortie1[i] = pd_SauvegardePistesFiltrees[4*i+1];
                pd_ZAnciennesPistesEntree1[i] = pd_SauvegardePistesFiltrees[4*i+2];
                pd_ZAnciennesPistesSortie1[i] = pd_SauvegardePistesFiltrees[4*i+3];
                pd_XAnciennesPistesEntree2[i] = pd_SauvegardePistesFiltrees[4*i+4];
                pd_XAnciennesPistesSortie2[i] = pd_SauvegardePistesFiltrees[4*i+5];
                pd_ZAnciennesPistesEntree2[i] = pd_SauvegardePistesFiltrees[4*i+6];
                pd_ZAnciennesPistesSortie2[i] = pd_SauvegardePistesFiltrees[4*i+7];
                pd_XAnciennesPistesEntree3[i] = pd_SauvegardePistesFiltrees[4*i+8];
                pd_XAnciennesPistesSortie3[i] = pd_SauvegardePistesFiltrees[4*i+9];
                pd_ZAnciennesPistesEntree3[i] = pd_SauvegardePistesFiltrees[4*i+10];
                pd_ZAnciennesPistesSortie3[i] = pd_SauvegardePistesFiltrees[4*i+11];
                pd_XAnciennesPistesEntree4[i] = pd_SauvegardePistesFiltrees[4*i+12];
                pd_XAnciennesPistesSortie4[i] = pd_SauvegardePistesFiltrees[4*i+13];
                pd_ZAnciennesPistesEntree4[i] = pd_SauvegardePistesFiltrees[4*i+14];
                pd_ZAnciennesPistesSortie4[i] = pd_SauvegardePistesFiltrees[4*i+15];
            }
            xi_NbAnciennePistes = xi_nbPistesIR;
        }

        else if (pFiltrageSortie.selectedEnum == FILTRE_MEDIAN)
        {
            for (i=0; i<xi_NbAnciennePistes; ++i)
            {
                pi_IdPlusAnciennesPistes[i] = pi_IdAnciennesPistes[i];
                pd_XPlusAnciennesPistesEntree1[i] = pd_XAnciennesPistesEntree1[i];
                pd_ZPlusAnciennesPistesEntree1[i] = pd_ZAnciennesPistesEntree1[i];
                pd_XPlusAnciennesPistesEntree2[i] = pd_XAnciennesPistesEntree2[i];
                pd_ZPlusAnciennesPistesEntree2[i] = pd_ZAnciennesPistesEntree2[i];
                pd_XPlusAnciennesPistesEntree3[i] = pd_XAnciennesPistesEntree3[i];
                pd_ZPlusAnciennesPistesEntree3[i] = pd_ZAnciennesPistesEntree3[i];
                pd_XPlusAnciennesPistesEntree4[i] = pd_XAnciennesPistesEntree4[i];
                pd_ZPlusAnciennesPistesEntree4[i] = pd_ZAnciennesPistesEntree4[i];
            }
            xi_NbPlusAnciennePistes = xi_NbAnciennePistes;

            for (i=0; i<xi_nbPistesIR; ++i)
            {
                pi_IdAnciennesPistes[i] = xo_pistesIRout1[i].id;
                pd_XAnciennesPistesEntree1[i] = pd_SauvegardePistesFiltrees[2*i];
                pd_ZAnciennesPistesEntree1[i] = pd_SauvegardePistesFiltrees[2*i+1];
                pd_XAnciennesPistesEntree2[i] = pd_SauvegardePistesFiltrees[2*i+2];
                pd_ZAnciennesPistesEntree2[i] = pd_SauvegardePistesFiltrees[2*i+3];
                pd_XAnciennesPistesEntree3[i] = pd_SauvegardePistesFiltrees[2*i+4];
                pd_ZAnciennesPistesEntree3[i] = pd_SauvegardePistesFiltrees[2*i+5];
                pd_XAnciennesPistesEntree4[i] = pd_SauvegardePistesFiltrees[2*i+6];
                pd_ZAnciennesPistesEntree4[i] = pd_SauvegardePistesFiltrees[2*i+7];
            }

            xi_NbAnciennePistes = xi_nbPistesIR;
        }

    }
    // ------------------------------------------------------------------------------------------------
    // Stopreadings et stopwritings
    if( IsConnected(Input("iPistesIR")) )	StopReading(Input("iPistesIR"));
	if (xb_UtiliserIdentificationStereo)	StopReading(Input("iPistesIRGauche"));
    StopWriting(IOEltSortiePistesVehicule);
    StopWriting(IOEltSortiePistesCamEntree);
    StopWriting(IOEltSortiePistesCamSortie);
    StopWriting(IOEltSortiePistesStereo);
    // Stopreadings et stopwritings
    // ------------------------------------------------------------------------------------------------



}

void MAPSLivicChangementRepereIR::Death()
{
}

