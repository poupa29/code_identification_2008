#include <stdio.h>
#include "MAPS_LivicDecodageInfrarouge.h"   // Includes the header of this component
#include "../commun/tarel/image.h"
#include "../commun/tarel/tools.h"


/*-------------------------------------------------------------------------------------------------
  mv_LitProprietesStatiques

  Utilisée par: Birth
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_LitProprietesStatiques()
{
    LDIRegion::modeCalibrage = GetBoolProperty("pModeCalibrage");
    pf_nbImagesParBit = GetFloatProperty("pNbImagesParBit");//(double)(FREQUENCE_ACQUISITION) / (2*GetFloatProperty("pFrequence"));
    pb_debug = GetBoolProperty("pDebug");
    pb_sivic = GetBoolProperty("pSivic");
    pb_bitParite = GetBoolProperty("pBitParite");
    pb_stats = GetBoolProperty("pStats");
    pf_rapportBinning = GetFloatProperty("pRapportBinningXsurY");
    xb_modeAllocStatique = GetBoolProperty("pModeAllocStatique");
    pb_decodageVitesse = GetBoolProperty("pDecodageVitesse");
    pi_seuil = (MAPSInteger)GetIntegerProperty("pSeuil");
    //PIDSeuil xd_PIDSeuil = pi_seuil;

    const char *s = GetStringProperty("pStartCode").Beginning();
    int i;
    for(i=0;i<strlen(s);i++)
    {
        if(s[i]=='1') 
            xo_startcode.enfile(1);
        else 
            xo_startcode.enfile(0);

    pi_tailleMessageCode = (MAPSInteger)GetIntegerProperty("pTailleMessageCode");
    pi_tailleMessageCode += strlen(s); // Ajout de la taille du start code
    if(pb_bitParite)
        pi_tailleMessageCode +=2; // Ajout du 0 obligatoire et du bit de parite
    }
}


/*-------------------------------------------------------------------------------------------------
  mv_LitProprietesDynamiques

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_LitProprietesDynamiques()
{
    MAPSFloat freqCamera = po_freqCamera->moyenne();
    MAPSFloat rapportResolutionY = ((MAPSFloat)xi_hauteur)/HAUTEUR_QUART_PAL;
    MAPSFloat rapportResolutionX = ((MAPSFloat)xi_largeur)/LARGEUR_QUART_PAL;

    pi_idPisteCalibrante = (MAPSInteger)GetIntegerProperty("pIdPisteCalibrante");
    pi_hauteurHorizon = (MAPSInteger)GetIntegerProperty("pHauteurHorizon");
    pf_coefTangageHorizon = (MAPSFloat)GetFloatProperty("pCoefTangageHorizon");
    pi_seuil = (MAPSInteger)GetIntegerProperty("pSeuil");
    //PIDSeuil pb_PIDSeuil = GetBoolProperty("pPIDSeuil");
    //PIDSeuil pi_PIDSeuilConsigne = GetIntegerProperty("pPIDSeuilConsigne");
    //PIDSeuil pd_PIDSeuil_Kp = GetFloatProperty("pPIDSeuilKp");
    //PIDSeuil pd_PIDSeuil_Ki = GetFloatProperty("pPIDSeuilKi");
    //PIDSeuil pd_PIDSeuil_Kd = GetFloatProperty("pPIDSeuilKd");
    pi_tailleDilatation = (MAPSInteger)(GetIntegerProperty("pTailleDilatation")*rapportResolutionY/2)*2+1;
    pf_voisinagex = GetFloatProperty("pVoisinageX")*rapportResolutionX/freqCamera;
    pf_voisinagey = GetFloatProperty("pVoisinageY")*rapportResolutionY/freqCamera;
    LDIRegion::confianceMax = (MAPSInteger)GetIntegerProperty("pConfianceMax");
    LDIRegion::penaliteConfianceNouvellePiste = (MAPSInteger)GetIntegerProperty("pPenaliteConfianceNouvellePiste");
    LDIRegion::activeAnciennete = GetBoolProperty("pActiveAnciennete");
    LDIRegion::coefAnciennete = (MAPSInteger)GetIntegerProperty("pCoefAnciennete");
    pe_formeSpot = GetEnumProperty("pFormeSpot");
    pf_rapportRectangle = GetFloatProperty("pRapportRectangle");
    pf_seuilRegionForme = GetFloatProperty("pSeuilRegionForme");
    pi_seuilRegionGrande = (MAPSInteger)(GetIntegerProperty("pSeuilRegionGrande")*rapportResolutionX*rapportResolutionY);
    pi_seuilRegionPonctuelle = (MAPSInteger)(GetIntegerProperty("pSeuilRegionPonctuelle")*rapportResolutionX*rapportResolutionY);
    pf_echelleAffichageVitesse = GetFloatProperty("pEchelleAffichageVitesse")*rapportResolutionY;
    pf_echelleAffichageAcceleration = GetFloatProperty("pEchelleAffichageAcceleration")*rapportResolutionY;
    pb_verbeux = GetBoolProperty("pVerbeux");
    pb_affichage = GetBoolProperty("pAffichage");
    pb_sortieToutesPisteIR = GetBoolProperty("pSortieToutesPisteIR");
    pb_sortieTempsExecution = GetBoolProperty("pSortieTempsExecution");
    pb_affichageDetaille = GetBoolProperty("pAffichageDetaille");
    pb_filtrageReflets = GetBoolProperty("pFiltrageReflets");
    //pi_SeuilTempsDetectionRegions = (MAPSInteger) (GetIntegerProperty("pSeuilTempsDetectionRegions"));
}


/*-------------------------------------------------------------------------------------------------
  mv_PIDSeuil

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
/*void MAPSLivicDecodageInfrarouge::mv_PIDSeuil(unsigned char* imgIn)
{
  if(IsConnected(Input("iAlerteCPU"))&& DataAvailableInFIFO(Input("iAlerteCPU")))
  {
	ReportError("Le PID de seuil ne peut s'exécuter si l'entrée AlertCPU est connectée");
	CommitSuicide();
  }

  double P, I, D, E, dE, dT;
  int pixelmoyen = 0;

  // on calcule combien il y a de pixels au dessus du seuil dans la moitie haute de l'image
  for(int ii=0 ; ii < xi_largeur ; ii++)
  {
	for(int jj=0 ; jj < xi_hauteur/2 ; jj++)
	{
      if( imgIn[ii+(xi_largeur+8)*jj]>xd_PIDSeuil ) pixelmoyen++;
    }
  }

  dT = (double)(xt_timestamp - xt_timestampPrec)/1000000;

  E = pixelmoyen - pi_PIDSeuilConsigne;
  dE = E - xd_E_PIDSeuilPrec;

  P = pd_PIDSeuil_Kp*E;

  if(dT!=0.0) D = (dE/dT)*pd_PIDSeuil_Kd;
  else D = 0.0;

  I = E*pd_PIDSeuil_Ki*dT+xd_I_PIDSeuilPrec;

  xd_PIDSeuil += P+D+I;

  MAPSIOElt *oEltE=StartWriting(Output("oE"));
  oEltE->Float()=E;
  StopWriting(oEltE);
  MAPSIOElt *oEltP=StartWriting(Output("oP"));
  oEltP->Float()=P;
  StopWriting(oEltP);
  MAPSIOElt *oEltSeuil=StartWriting(Output("oSeuil"));
  oEltSeuil->Float()=xd_PIDSeuil;
  StopWriting(oEltSeuil);
  
  pi_seuil = (int)xd_PIDSeuil;
  if( pi_seuil<GetIntegerProperty("pSeuil") ) pi_seuil=(MAPSInteger)GetIntegerProperty("pSeuil");
  if( pi_seuil>255 ) pi_seuil=255;

  xd_I_PIDSeuilPrec=I;
  xd_E_PIDSeuilPrec=E;
}*/


/*-------------------------------------------------------------------------------------------------
  mv_Seuillage

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_Seuillage(unsigned char* imgIn, unsigned char* imgOut, int seuil, int horizon)
{
    int ii, jj;
    for(ii=0 ; ii < xi_largeur ; ii++)
    {
        for(jj=horizon ; jj < xi_hauteur ; jj++)
        {
            if( imgIn[ii+(xi_largeur+8)*jj]>seuil )
                imgOut[ii+xi_largeur*jj] = imgIn[ii+(xi_largeur+8)*jj];
            else
                imgOut[ii+xi_largeur*jj] = 0;
        }
    }
}


/*-------------------------------------------------------------------------------------------------
  mv_EliminationsPointsBlancsSivic

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_EliminationsPointsBlancsSivic(unsigned char*img)
{
    int ii, jj;
    for(ii=0 ; ii < xi_largeur ; ii++)
    {
        for(jj=0 ; jj < xi_hauteur ; jj++)
        {
            if( img[ii+xi_largeur*jj] == 255 )
            {
            // si le point blancs est entouré de points non blancs (il est isolé)
            if( (img[(ii+1) +xi_largeur* (jj)]   < 255)&&
                (img[(ii-1) +xi_largeur* (jj)]   < 255)&&
                (img[(ii)   +xi_largeur* (jj+1)] < 255)&&
                (img[(ii)   +xi_largeur* (jj-1)] < 255)&&
                (img[(ii+1) +xi_largeur* (jj+1)] < 255)&&
                (img[(ii+1) +xi_largeur* (jj-1)] < 255)&&
                (img[(ii-1) +xi_largeur* (jj+1)] < 255)&&
                (img[(ii-1) +xi_largeur* (jj-1)] < 255)
              )
                {
                  // éteindre ce point
                  img[ii+xi_largeur*jj] = 0;
                }
            }
        }
    }
}


/*-------------------------------------------------------------------------------------------------
  mv_EliminationRegions

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_EliminationRegions(unsigned char **imgRegions1, LivicPilePointeur *pregions, int *nbRegions)
{
    int ii;
    *imgRegions1 = (unsigned char *)malloc(xi_largeur*xi_hauteur*sizeof(unsigned char));
    for(ii=0; ii<xi_largeur*xi_hauteur; ii++) (*imgRegions1)[ii] = 0;
    LivicPilePointeur aEffacer;

    for(int reg=0; reg < *nbRegions; reg++)
    {
        LivicPilePoint2D *region = (LivicPilePoint2D*)(*pregions)[reg];
        int __nbElements = region->nbElements();


        // ------------------------------------------------------------------------------------------------
        // 1. elimination des grandes regions
        //    (on tient compte de la dilatation en evaluant les points ajoutes
        //    pour une forme carree : evaluation arbitraire = tailleDilatation * cote * 4)
        int pixelsDilatation = (int)(pi_tailleDilatation * sqrt((double)pi_seuilRegionGrande) * 4.);
        if( __nbElements > (pi_seuilRegionGrande + pixelsDilatation) )
        {
            aEffacer.empile(region);
            if( pb_verbeux )
            {
                sprintf(xs_msg, "Elimination région %d trop grande (%d pixels)", reg+1, __nbElements);
                ReportWarning(xs_msg);
            }
            continue;
        }
        // 1. elimination des grandes regions
        // ------------------------------------------------------------------------------------------------

        
       
        // ------------------------------------------------------------------------------------------------
        // 2. elimination des regions non adaptées
        // on ne traite pas les toutes petites régions qui sont ponctuelles par leur taille

        //    on cherche les regions circulaires pleines
        double __barx, __bary;
        LvBarycentre_region(region, &__barx, &__bary);
        int __ibarx = (int)(__barx+.5);
        int __ibary = (int)(__bary+.5);
        // on cherche le plus petit cercle contenant la region et centre sur le barycentre, donc on calcule le
        // point le plus eloigne du barycentre (au carre pour eviter des racines)
       

        // Sélection région circulaire ou rectiligne
        switch (pe_formeSpot.selectedEnum)
        {
        case 0: //Régions circulaires
        {
            if( __nbElements > max(SEUIL_REGION_PONCTUELLE_MIN, (pi_seuilRegionPonctuelle + pi_tailleDilatation*pi_tailleDilatation)) )
            {
                int distancemax = 0;
                for(int pi=0; pi < __nbElements; pi++)
                {
                    int distance = ((*region)[pi][0]-__ibarx)*((*region)[pi][0]-__ibarx) + pf_rapportBinning*pf_rapportBinning*((*region)[pi][1]-__ibary)*((*region)[pi][1]-__ibary);
                    if( distance > distancemax ) distancemax = distance;
                }
                // on calcule la proportion de points allumes a l'interieur du cercle, c-a-d le pourcentage de remplissage
                // combien de points potentiels dans le disque (= surface du disque)?
                int nbPointsDisque = (int)(PI * distancemax/pf_rapportBinning);
                double tauxRemplissage = ((double)__nbElements) / ((double)nbPointsDisque);
                // si le taux de remplissage est inferieur a un seuil, le region est consideree comme non circulaire
                if( (tauxRemplissage < pf_seuilRegionForme)&&(__nbElements>20) )
                {
                    aEffacer.empile(region);
                    if( pb_verbeux )
                    {
                        sprintf(xs_msg, "Elimination circ. reg=%d, nb=%d, nbd=%d, tx=%f, dm=%d", reg+1, __nbElements, nbPointsDisque, tauxRemplissage, distancemax);
                        ReportWarning(xs_msg);
                    }
                    continue;
                }
            }
            break;
        } 
        case 1: //Régions rectilignes
        {
            int xmin = (*region)[0][0];
            int xmax = (*region)[0][0];
            int ymin = (*region)[0][1];
            int ymax = (*region)[0][1];

            for(int pi=1; pi < __nbElements; pi++)
            {
                if( (*region)[pi][0] > xmax ) xmax = (*region)[pi][0];
                if( (*region)[pi][0] < xmin ) xmin = (*region)[pi][0];
                if( (*region)[pi][1] > ymax ) ymax = (*region)[pi][1];
                if( (*region)[pi][1] < ymin ) ymin = (*region)[pi][1];
            }

            if ( (float)(max(1,ymax-ymin-2*pi_tailleDilatation)/max(1,xmax-xmin-2*pi_tailleDilatation))< (pf_rapportRectangle/pf_rapportBinning) )
            {
                aEffacer.empile(region);
                if( pb_verbeux )
                {
                    sprintf(xs_msg, "_Elimination rect. reg=%d, xmin=%d, xmax=%d, ymin=%d, ymax=%d, rapportRectangle=%f, nb=%d", reg+1, xmin, xmax, ymin, ymax, (float)((ymax-ymin)*pf_rapportBinning/(xmax-xmin)), __nbElements);
                    ReportWarning(xs_msg);
                }
                continue;
            }

            // on calcule la proportion de points allumes a l'interieur du rectangle, c-a-d le pourcentage de remplissage
            // combien de points potentiels dans le rectangle
            int nbPointsRectangle = (int)((xmax-xmin)*(ymax-ymin));
            double tauxRemplissage = ((double)__nbElements) / ((double)nbPointsRectangle);
            // si le taux de remplissage est inferieur a un seuil, le region est consideree comme non rectiligne
            if( (tauxRemplissage < pf_seuilRegionForme)&&(__nbElements>20) )
            {
                aEffacer.empile(region);
                if( pb_verbeux )
                {
                    sprintf(xs_msg, "Elimination rect. reg=%d, nb=%d, nbd=%d, tx=%f", reg+1, __nbElements, nbPointsRectangle, tauxRemplissage);
                    ReportWarning(xs_msg);
                }
                continue;
            }
            break;
        }
        }  
        // 2. elimination des regions non circulaires
        // ------------------------------------------------------------------------------------------------


        // ------------------------------------------------------------------------------------------------
        // 3. si la region n'a pas ete eliminee, on la copie dans le buffer
        for(int pi=0; pi < __nbElements; pi++)
        {
            (*imgRegions1)[(*region)[pi][0]+xi_largeur*(*region)[pi][1]] = reg+1;
        }
        // 3. si la region n'a pas ete eliminee, on la copie dans le buffer
        // ------------------------------------------------------------------------------------------------


    }


    // ------------------------------------------------------------------------------------------------
    // effacement effectif
    int aeff = aEffacer.nbElements();
    for(int ae=0; ae<aeff; ae++)
    {
        LivicPilePoint2D *region = (LivicPilePoint2D*)aEffacer[ae];
        (*pregions).efface(region);
        if( xt_timestamp > 59000000 )
        {
            int i=0;
        }
        if (LivicPilePoint2D::modeAllocStatique) LivicPilePoint2D::deletePile(region);
        else delete region;
    }
    aEffacer.vide();
    *nbRegions = (*pregions).nbElements();
    // effacement effectif
    // ------------------------------------------------------------------------------------------------


}


/*-------------------------------------------------------------------------------------------------
  mv_CalculFinalCalibrage

  Utilisée par: Death
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_CalculFinalCalibrage()
{
    if( !LDIRegion::modeCalibrage ) return;
    bool found = false;
    int nbPistes = po_pistes->nbElements();
    
    sprintf(xs_msg, "CALIBRAGE. Pistes calibrantes (sur %d) :", nbPistes);
    ReportWarning(xs_msg);
    mv_LOGDEBUG(xs_msg);
    
    
    // ------------------------------------------------------------------------------------------------
    // recherche des pistes calibrantes
    for(int pis=0; pis < nbPistes; pis++)
    {
        LDIRegion *piste = (LDIRegion*)((*po_pistes)[pis]);
        if( piste->effacee ) {
            continue;
        }
        if( piste->EstCalibrante() )
        {
            sprintf(xs_msg, "    piste %d : %.2f images/bits", piste->id, piste->calibrage->nbImagesParBit);
            if( piste->id == pi_idPisteCalibrante ) strcat(xs_msg, "  (piste choisie)");
            ReportWarning(xs_msg);
            mv_LOGDEBUG(xs_msg);
            found = true;
        }
    }
    // recherche des pistes calibrantes
    // ------------------------------------------------------------------------------------------------
    
    
    // ------------------------------------------------------------------------------------------------
    // aucune piste calibrante trouvée
    if( !found )
    {
        ReportError("  aucune");
        ReportError("ERREUR : Recommencez le calibrage avec une source calibrante correcte.");
    }
    // aucune piste calibrante trouvée
    // ------------------------------------------------------------------------------------------------
    
    
    // ------------------------------------------------------------------------------------------------
    // au moins une piste calibrante existe
    else
    {
        
        
        // ------------------------------------------------------------------------------------------------
        // on relit les proprietes dynamiques pour savoir la piste choisie par l'opérateur
        mv_LitProprietesDynamiques();
        // on relit les proprietes dynamiques pour savoir la piste choisie par l'opérateur
        // ------------------------------------------------------------------------------------------------
        
        
        // ------------------------------------------------------------------------------------------------
        // on reparcourt les pistes pour trouver celle choisie par l'opérateur dans pIdPisteCalibrante
        found = false;
		int pis=0;
        for(pis=0; pis < nbPistes; pis++)
        {
            LDIRegion *piste = (LDIRegion*)((*po_pistes)[pis]);
            if( piste->effacee ) {
                continue;
            }
            // piste trouvée.
            if( piste->id == pi_idPisteCalibrante )
            {
                if( !piste->EstCalibrante() )
                {
                    sprintf(xs_msg, "ERREUR : piste %d non calibrante. Verifiez la propriete pIdPisteCalibrante", pi_idPisteCalibrante);
                    ReportError(xs_msg);
                    break;
                }
                pf_nbImagesParBit = piste->calibrage->nbImagesParBit;
                DirectSetProperty("pNbImagesParBit", pf_nbImagesParBit);
                sprintf(xs_msg, "Piste choisie : %d, nbImagesParBit=%.2f", piste->id, pf_nbImagesParBit);
                ReportWarning(xs_msg);
                if( pf_nbImagesParBit <= 0 )
                {
                    ReportError("ERREUR : nbImagesParBit incorrect. Avez-vous attendu assez longtemps? Ou la source est-elle correcte?");
                    break;
                }
                ReportError("ATTENTION : La propriete pNbImagesParBit a ete modifiee. Fermez la fenetre de proprietes et enregistrez le diagramme");
                ReportError("pour prendre en compte la nouvelle valeur.");
                found = true;
                break;
            }
        }
        // on reparcourt les pistes pour trouver celle choisie par l'opérateur dans pIdPisteCalibrante
        // ------------------------------------------------------------------------------------------------
        
        
        if( pis==nbPistes )
        {
            sprintf(xs_msg, "ERREUR : piste %d inexistante. Verifiez la propriete pIdPisteCalibrante", pi_idPisteCalibrante);
            ReportError(xs_msg);
        }
    }
    // au moins une piste calibrante existe
    // ------------------------------------------------------------------------------------------------
    
    
}


/*-------------------------------------------------------------------------------------------------
mv_TraitementCiblesVisibles

  Utilisée par: Core
  -------------------------------------------------------------------------------------------------*/
  void MAPSLivicDecodageInfrarouge::mv_TraitementCiblesVisibles(int *nbPistes, LivicPilePointeur *pregions, int *nbRegions)
  {
    double barx, bary;
    int ibarx,ibary;
    double barxerr, baryerr;
    barxerr = pf_voisinagex; // en pixels
    baryerr = pf_voisinagey; // en pixels

      for(int reg=0; reg < *nbRegions; reg++)
      {            
          // ------------------------------------------------------------------------------------------------
          // 0. calcul du barycentre
          LivicPilePoint2D *region;
          (*pregions).depile((void**)&region);
          mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Region %d", reg));
          xi_logdbgi++;
          LvBarycentre_region(region, &barx, &bary);
          ibarx = (int)(barx+.5);
          ibary = (int)(bary+.5);
          mv_LOGDEBUG("Barycentre OK");
          // 0. calcul du barycentre
          // ------------------------------------------------------------------------------------------------
          
          
          // ------------------------------------------------------------------------------------------------
          // 1. recherche d'une piste correspondant à la région
          mv_LOGDEBUG("Recherche de la piste");
          xi_logdbgi++;
          LDIRegion *piste = NULL;
          //debutRecherchePiste
          piste = mo_RecherchePisteDansVoisinage(*nbPistes, barx, bary, barxerr, baryerr);
          // 1. recherche d'une piste correspondant à la région
          // ------------------------------------------------------------------------------------------------
          
          
          // ------------------------------------------------------------------------------------------------
          // 1. piste existante
          if( piste )
          {
              mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Piste %d trouvee", piste->id));
              
              
              // ------------------------------------------------------------------------------------------------
              // 1.1. On calcule son nouveau vecteur vitesse et acceleration
              double vx = ibarx - piste->bar.x;
              double vy = ibary - piste->bar.y;
              piste->acceleration.x = vx - piste->vitesse.x;
              piste->acceleration.y = vy - piste->vitesse.y;
              piste->vitesse.x = vx;
              piste->vitesse.y = vy;
              // 1.1. On calcule son nouveau vecteur vitesse et acceleration
              // ------------------------------------------------------------------------------------------------
              
              
              // ------------------------------------------------------------------------------------------------
              // 1.2. on met a jour son barycentre avec la nouvelle valeur et la nouvelle region
              if (LivicPilePoint2D::modeAllocStatique) LivicPilePoint2D::deletePile(piste->region);
              else delete piste->region;
              piste->region = region;
              piste->bar.x = barx;
              piste->bar.y = bary;
              // 1.2. on met a jour son barycentre avec la nouvelle valeur et la nouvelle region
              // ------------------------------------------------------------------------------------------------
              
              /*
              // ------------------------------------------------------------------------------------------------
              // 1.3. §§ ANCIEN CODE: SERVAIT A CALCULER LA FREQUENCE DU SIGNAL
              //      si nbImagesAbsente!=0, c'est que la piste vient de se rallumer :
              //      alors on calcule la nouvelle frequence instantanee et on l'ajoute
              //      a la frequence moyenne
              if( piste->nbImagesAbsente != 0 )
              {
                  piste->frequence_moy->enfile(
                  (double)(FREQUENCE_ACQUISITION) /
                  (double)(piste->nbImagesAbsente + piste->nbImagesPresente)
              );
              piste->nbImagesAbsente = 0;
              piste->nbImagesPresente = 0;
              // 1. calcul de la frequence moyenne et decision si c'est un signal IR ou pas
              double freqcalc = piste->frequence_moy->moyenne();
              if( freqcalc<0 ) freqcalc = 0.;
              double freqreel = pf_frequence * RAPPORT_FREQ_REELLE_FREQ_VUE;
              if( (freqcalc < (freqreel*1.10))&&(freqcalc > (freqreel*0.90)) )
                  piste->estSignalIR = true;
              else
                  piste->estSignalIR = false;
              }
              // 1.3. §§ ANCIEN CODE: SERVAIT A CALCULER LA FREQUENCE DU SIGNAL
              // ------------------------------------------------------------------------------------------------
              */
              
              // ------------------------------------------------------------------------------------------------
              // 1.3. fausse piste: lumière continue
              //      la piste est restee allumee pendant toute une periode de pi_tailleMessageCode, c'est une lumiere
              //      permanente. On la declare faussepiste (on continue a la tracker, mais on ignore son code).
              if( (!piste->faussePiste) || (piste->faussePiste == piste->nouvelle) )
              {
                  if( (piste->nbImagesPresente >= (pf_nbImagesParBit*pi_tailleMessageCode+1)) )
                    {
                        piste->SetFaussePiste(piste->continu);
                        piste->code->vide();
                        piste->identifiant->vide();
                        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Piste %d=continue", piste->id));
                    }
              }
              // 1.3. fausse piste: lumière continue
              // ------------------------------------------------------------------------------------------------


			  // ------------------------------------------------------------------------------------------------
              // 1.3.2 fausse piste: test de la durée (Réinitialisation ?)
			  if((piste->faussePiste != piste->piste) && (piste->faussePiste != piste->nouvelle))
              {
				  if((MAPS::CurrentTime() - (piste->timeout_fausse_piste)) >= (GetIntegerProperty("pTimoutFaussesPistes")*1000))
                    {
                        piste->effacee = true;
                    }
              }
              // 1.3.2 fausse piste: test de la durée (Réinitialisation ?)
              // ------------------------------------------------------------------------------------------------
              

              // ------------------------------------------------------------------------------------------------
              // 1.4. modeCalibrage : comptage des images absente et presente
              //      si nbImagesAbsente != 0, c'est que la piste vient de se rallumer
              //      si !piste nouvelle, c'est que ce n'est pas une nouvelle piste (il y a au moins un 1 et un 0)
              //      si la piste est calibrante,
              //      on stocke le nombre d'images absente et presente dans la file
              //      Si la piste n'a pas un signal de calibrage (presente ~= absente ie. signal 101010...),
              //      on la marque non calibrante
              if( LDIRegion::modeCalibrage && piste->EstCalibrante() )
              {
                  if( (piste->faussePiste != piste->nouvelle) && (piste->nbImagesAbsente != 0) )
                  {
                      // test si la piste est calibrante ou pas
                      if( (piste->nbImagesPresente < (piste->nbImagesAbsente-1)) ||
                          (piste->nbImagesPresente > (piste->nbImagesAbsente+1)) )
                      {
                          //piste->SetNonCalibrante();
                          if( pb_verbeux )
                          {
                            sprintf(xs_msg, "piste %d NON calibrante, IP=%d, IA=%d", piste->id, piste->nbImagesPresente, piste->nbImagesAbsente);
                            if( piste->id == pi_idPisteCalibrante ) strcat(xs_msg, "  (piste choisie)");
                            ReportWarning(xs_msg);
                          }
                      } else
                      {
                          // on signale (la première fois) qu'on a trouvé cette piste calibrante
                          if( piste->calibrage->nbImagesPresente->nbElements() == 0 )
                          {
                              sprintf(xs_msg, "piste %d calibrante, IP=%d, IA=%d", piste->id, piste->nbImagesPresente, piste->nbImagesAbsente);
                              if( piste->id == pi_idPisteCalibrante ) strcat(xs_msg, "  (piste choisie)");
                              ReportWarning(xs_msg);
                          }
                          piste->calibrage->nbImagesPresente->enfile(piste->nbImagesPresente);
                          piste->calibrage->nbImagesAbsente->enfile(piste->nbImagesAbsente);
                          int nbElementsCalibrage = piste->calibrage->nbImagesPresente->nbElements();
                          if( (nbElementsCalibrage>0) && ((nbElementsCalibrage % FENETRE_CALIBRAGE)==0) )
                          {
                              piste->calibrage->nbImagesParBit = (piste->calibrage->nbImagesPresente->moyenne() + piste->calibrage->nbImagesAbsente->moyenne())/2.;
                              sprintf(xs_msg, "Calibrage piste %d : IP=%f, IA=%f, images/bit=%.1f (moyenne sur %d bits)",
                                  piste->id,
                                  piste->calibrage->nbImagesPresente->moyenne(),
                                  piste->calibrage->nbImagesAbsente->moyenne(),
                                  piste->calibrage->nbImagesParBit,
                                  nbElementsCalibrage*2);
                              if( piste->id == pi_idPisteCalibrante ) strcat(xs_msg, "  (piste choisie)");
                              ReportWarning(xs_msg);
                              mv_LOGDEBUG(xs_msg);
                              piste->calibrage->nbImagesPresente->taille(nbElementsCalibrage + FENETRE_CALIBRAGE);
                              piste->calibrage->nbImagesAbsente->taille(nbElementsCalibrage + FENETRE_CALIBRAGE);
                          }
                      }
                  }
              }
              // 1.4. modeCalibrage : comptage des images absente et presente
              // ------------------------------------------------------------------------------------------------
              
              
              // ------------------------------------------------------------------------------------------------
              // 1.5. Calcul du code
              //      si nbImagesAbsente != 0, c'est que la piste vient de se rallumer :
              //      on a donc un 1 et un 0 sur un certain nombre d'images, ce qui donne
              //      une sequence. Le message n'est pas fini forcement. On a juste
              //      un nouveau front montant (1).
              //      si la somme des images est plus grande que le temps d'emission du message IR
              //      (pour une taille donnee de message), alors on doit extraire les 1 et 0
              //      car le message est fini.
              if( piste->nbImagesAbsente != 0 )
              {
                  //if( !(piste->faussePiste) || (piste->faussePiste == nouvelle) )
                  {
                      piste->bits1 = (double)(piste->nbImagesPresente) / pf_nbImagesParBit;
                      piste->bits0 = (double)(piste->nbImagesAbsente) / pf_nbImagesParBit;
                      piste->ibits1 = (int)(fmod(piste->bits1, 1)>0.5?ceil(piste->bits1):floor(piste->bits1));
                      piste->ibits0 = (int)(fmod(piste->bits0, 1)>0.5?ceil(piste->bits0):floor(piste->bits0));
                      int b;
                      if( pb_debug) sprintf(xs_msg, "piste %d: IP=%d, IA=%d, enfile=", piste->id, piste->nbImagesPresente, piste->nbImagesAbsente);
                      for(b=0; b<piste->ibits1; b++) {piste->code->enfile(1);if( pb_debug) strcat(xs_msg, "1");}
                      for(b=0; b<piste->ibits0; b++) {piste->code->enfile(0);if( pb_debug) strcat(xs_msg, "0");}
                      piste->bitsEnfiles=piste->ibits1+piste->ibits0;
                      piste->codeModifie = ((piste->ibits1 + piste->ibits0) > 0);
                      if( pb_debug )
                      {
                          char tmp[128];
                          char tmp2[512];
                          piste->code->enString(tmp);
                          sprintf(tmp2, " code=%s, id=", tmp);
                          piste->identifiant->enString(tmp);
                          strcat(tmp2, tmp);
                          strcat(xs_msg, tmp2);
                          ReportInfo(xs_msg);
                      }
                      mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Piste %d nouveaux bits OK", piste->id));
                  }
                  piste->nbImagesAbsente = 0;
                  piste->nbImagesPresente = 0;
                  // si la piste etait une nouvelle piste (cf. 2.), on a décodé ses deux premiers bits
                  // et on la marque à nouvelle = false pou que ses images de présence puissent être incrémentées
                  // cf. 1.6.
                  if( piste->faussePiste == piste->nouvelle ) piste->faussePiste = piste->piste;
              }
              // 1.5. Calcul du code
              // -----------------------------------------------------------------------------------------------
              
              
              // ------------------------------------------------------------------------------------------------
              // 1.6. la piste est visible : on incremente nbImagesPresente
              //      si elle n'est pas une nouvelle piste. En effet, une nouvelle piste est en cours de bit 1
              //      donc on laisse passer ce bit dont le décompte est incomplet
              //      cf. 1.5
              if( piste->faussePiste != piste->nouvelle )
              {
                  piste->nbImagesPresente++;
              } else
              {
                  mv_LOGDEBUG(sprintf(xs_logdbgmsg, "nouvelle", piste->nbImagesPresente, piste->nbImagesAbsente));
              }
              // 1.6. la piste est visible : on incremente nbImagesPresente
              // ------------------------------------------------------------------------------------------------
              
              
              piste->traitee = true;
              xi_logdbgi--;
        }
        // 1. piste existante
        // ------------------------------------------------------------------------------------------------
        
        
        // ------------------------------------------------------------------------------------------------
        // 2. la cible n'est pas une piste existante : on la rajoute
        //    ! elle est marquee nouvelle par le constructeur
        //    de facon à ne pas compter ses premières images de présence qui sont un 1 en cours (donc faux)
        //    cf. 1.5
        else
        {
            xi_logdbgi--;
            LDIRegion *piste = new LDIRegion();
            piste->code->taille(pi_tailleMessageCode);
            piste->region = region;
            piste->bar.x = barx;
            piste->bar.y = bary;
            piste->traitee = true;
            po_pistes->empile(piste);
            (*nbPistes)++;
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Nouvelle piste %d", piste->id));
        }
        // 2. la cible n'est pas une piste existante : on la rajoute
        // ------------------------------------------------------------------------------------------------
        
        
        xi_logdbgi--;
    }
    //printf("      fin detection region\n\n\n");
}


/*-------------------------------------------------------------------------------------------------
  mo_RecherchePisteDansVoisinage
  
  Utilisee par: mv_TraitementCiblesVisibles
-------------------------------------------------------------------------------------------------*/
LDIRegion* MAPSLivicDecodageInfrarouge::mo_RecherchePisteDansVoisinage(int nbPistes, double barx, double bary, double barxerr, double baryerr)
{
    LDIRegion *piste = NULL;
    LDIRegion *pistetmp = NULL;
    // on parcourt les pistes, on retient toutes celles qui sont dans le voisinage de recherche
    // et on cherche parmi les candidates la plus proche
    double distancemin, distance=0.;
    // la distance min de départ est la plus grande possible (diagonale de l'image)
    distancemin = xi_largeur*xi_largeur+xi_hauteur*xi_hauteur;
    for(int pis=0; pis < nbPistes; pis++)
    {
        pistetmp = (LDIRegion*)((*po_pistes)[pis]);
        if( pistetmp->effacee ) {
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Piste %d effacee", pistetmp->id));
            continue;
        }
        MAPSFloat rapportResolutionX = ((MAPSFloat)xi_largeur)/LARGEUR_QUART_PAL;
        MAPSFloat rapportResolutionY = ((MAPSFloat)xi_hauteur)/HAUTEUR_QUART_PAL;

        // §§§ FAIRE DE LA PREVISION DE TRAJECTOIRE AVEC V ET A
        if(
            (pistetmp->bar.x < barx + ((double)D_TAILLE_FENETRE_PISTE*rapportResolutionX+barxerr*(pistetmp->nbImagesAbsente)))&&
            (pistetmp->bar.x > barx - ((double)D_TAILLE_FENETRE_PISTE*rapportResolutionX+barxerr*(pistetmp->nbImagesAbsente)))&&
            (pistetmp->bar.y < bary + ((double)D_TAILLE_FENETRE_PISTE*rapportResolutionY+baryerr*(pistetmp->nbImagesAbsente)))&&
            (pistetmp->bar.y > bary - ((double)D_TAILLE_FENETRE_PISTE*rapportResolutionY+baryerr*(pistetmp->nbImagesAbsente))) )
        {
            // piste retenue. On calcule sa distance au point
            distance = (pistetmp->bar.x-barx)*(pistetmp->bar.x-barx)+(pistetmp->bar.y-bary)*(pistetmp->bar.y-bary);
            if( distance<distancemin )
            {
                distancemin = distance;
                piste = pistetmp;
            }
        }
    }
    return piste;
}


/*-------------------------------------------------------------------------------------------------
  mv_TraitementPistesInvisibles

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_TraitementPistesInvisibles(int nbPistes)
{
    for(int pis=0; pis < nbPistes; pis++)
    {
        LDIRegion *piste = (LDIRegion*)((*po_pistes)[pis]);
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Piste %d", piste->id));
        xi_logdbgi++;
        
        
        // ------------------------------------------------------------------------------------------------
        // 1. piste effacée: on saute
        if( piste->effacee ) {
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Piste %d effacee", piste->id));
            xi_logdbgi--;
            continue;
        }
        // 1. piste effacée: on saute
        // ------------------------------------------------------------------------------------------------
        
        
        // ------------------------------------------------------------------------------------------------
        // 3. on saute les po_pistes deja traitees en 1 et 2 et on les reinitialise pour le prochain Core()
        if( piste->traitee )
        {
            mv_LOGDEBUG("deja traitee");
            piste->traitee = false;
        }
        // 3. on saute les po_pistes deja traitees en 1 et 2 et on les reinitialise pour le prochain Core()
        // ------------------------------------------------------------------------------------------------
        
        
        else
        {
            // On tient une piste non traitee, donc invisible sur l'image courante
            // on predit sa nouvelle position avec la derniere vitesse mesuree
            //piste->bar.x += piste->vitesse.x;
            //piste->bar.y += piste->vitesse.y;
            
            
            // ------------------------------------------------------------------------------------------------
            // suppression des pistes trop longtemps absentes
            if( piste->nbImagesAbsente > (pf_nbImagesParBit*pi_tailleMessageCode+1) )
            {
                // La piste est disparue depuis trop longtemps :
                // on la supprime
                //po_pistes.efface(piste);
                piste->effacee = true;
                if( pb_verbeux )
                {
                    sprintf(xs_msg, "Piste %d effacee", piste->id);
                    ReportWarning(xs_msg);
                }
                mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Piste %d en effacement", piste->id));
            }
            // suppression des pistes trop longtemps absentes
            // ------------------------------------------------------------------------------------------------
            
            
            // ------------------------------------------------------------------------------------------------
            // on incremente nbImagesAbsente pour toutes les pistes absentes
            piste->nbImagesAbsente++;
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "ip=%d, ia=%d", piste->nbImagesPresente, piste->nbImagesAbsente));
            // on incremente nbImagesAbsente pour toutes les pistes absentes
            // ------------------------------------------------------------------------------------------------
            
            
        }
        xi_logdbgi--;
    }
}


/*-------------------------------------------------------------------------------------------------
  mv_DecodageCode

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_DecodageCode(int nbPistes)
{
    int i = 0;
	for(int pis=0; pis < nbPistes; pis++)
    {
        LDIRegion *piste = (LDIRegion*)((*po_pistes)[pis]);
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Piste %d", piste->id));
        xi_logdbgi++;
        bool codeOkPrec = piste->codeok;
        
        
        // ------------------------------------------------------------------------------------------------
        // 1. piste effacée: on saute
        if( piste->effacee ) {
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "effacee"));
            xi_logdbgi--;
            continue;
        }
        // 1. piste effacée: on saute
        // ------------------------------------------------------------------------------------------------
        
        
        // ------------------------------------------------------------------------------------------------
        // si le code de la piste a été modifié (nouveaux bits)
        if( piste->codeModifie )
        {
            piste->codeModifie = false;
            // decodage du message pour toutes les po_pistes
            // 1. Si la file de code est bien remplie, on regarde si les 3 premiers bits sont des start-bits 110
            // 2. Si oui, on decode le nombre constitue des suivants
            
            
            // ------------------------------------------------------------------------------------------------
            // si code bien rempli
            if( piste->code->nbElements() == pi_tailleMessageCode )
            {
    

                // ------------------------------------------------------------------------------------------------
                // copie du tableau de code
                short *tmp = piste->code->tableau();
                short *tabcode = (short*)malloc(pi_tailleMessageCode*sizeof(short));
                for(i=0; i<pi_tailleMessageCode; i++)
                {
                    tabcode[i] = tmp[i];
                }
                // copie du tableau de code
                // ------------------------------------------------------------------------------------------------
                
                
                // ------------------------------------------------------------------------------------------------
                // recherche des start-bits
                // A CORRIGER : faire une recherche circulaire : start peut-être à cheval entre le début et la fin!
                //              puis recopier le message dans un tableau avec le début en 0
                int start=0;
                bool found = false;
                int startcodesize = xo_startcode.nbElements();
                short* startcodetab = xo_startcode.tableau();

                while( (start<pi_tailleMessageCode) && (!found) )
                {
                    bool startcodeIsOk = true;
                    for(int i=0; i<startcodesize; i++)
                    {
                        startcodeIsOk =
                            startcodeIsOk &&
                            (tabcode[(start+i)%pi_tailleMessageCode] == startcodetab[i]);
                    }
                    if( startcodeIsOk ) found = true;
                    else start++;
                }
                // recherche des start-bits
                // ------------------------------------------------------------------------------------------------
                
                
                // ------------------------------------------------------------------------------------------------
                // si start-bits valides trouvés
                if( found )
                {
                    
                    
                    // ------------------------------------------------------------------------------------------------
                    // start-bits dans le message? -> fausse piste
                    // On regarde si le code de start interdit est present une autre fois dans la trame
                    // A CORRIGER : faire une recherche circulaire ou utiliser un tableau avec le début du msg à 0
                    bool elimine = false;
                    for(i=0; i<pi_tailleMessageCode-1; i++)
                    {
                        int j;
                        bool startcodeRepete = true;
                        for(j=0; j<startcodesize; j++)
                        {
                            startcodeRepete =
                                startcodeRepete &&
                                (tabcode[(start+1+i+j)%pi_tailleMessageCode] == startcodetab[j]);
                        }
                        
                        if(startcodeRepete )
                        {
                            // code de start detecte. On marque la piste et baisse la confiance.
                            int id = piste->identifiant->majorite();
                            if( piste->SetFaussePiste(piste->codeInterdit) )
                            {
                                if( pb_stats )
                                {
                                    xo_identifiantsStats.AjouteAnciennete(id);           
                                    if( codeOkPrec ) xo_identifiantsStats.AjouteFoisFaussePiste(id);
                                    xo_identifiantsStats.AjouteFaussePiste(id);
                                    xo_identifiantsStats.SetConfianceBasse(id, piste->confiance);
                                }
                                if( pb_verbeux ) ReportWarning(piste->PrintConfiance(xs_msg));
                                elimine = true;
                                mv_LOGDEBUG(sprintf(xs_logdbgmsg, "code interdit"));
                                break;
                            } else
                            {
                                if( pb_stats )
                                {
                                    xo_identifiantsStats.AjouteAnciennete(id);           
                                    xo_identifiantsStats.AjouteFoisRattrapageAnciennete(id);
                                    xo_identifiantsStats.AjouteCodeOk(id);
                                }
                                xi_nbPistesRattrapeesAnciennete++;
                                if( pb_verbeux )
                                {
                                    piste->PrintConfiance(xs_msg);
                                    strcat(xs_msg, " : rattrapee par l'anciennete");
                                    ReportWarning(xs_msg);
                                }
                                mv_LOGDEBUG(sprintf(xs_logdbgmsg, "rattrapee anciennete"));
                            }
                        }
                    }
                    // start-bits dans le message? -> fausse piste
                    // ------------------------------------------------------------------------------------------------
                    
                    
                    // ------------------------------------------------------------------------------------------------
                    // si piste non éliminée ci-dessus
                    if( !elimine )
                    {
                        
                        
                        // ------------------------------------------------------------------------------------------------
                        // decodage
                        short message = 0;
                        char s[128];
                        int imax = (pb_bitParite ? pi_tailleMessageCode-startcodesize-2 : pi_tailleMessageCode-startcodesize);
                        
                        if( pb_debug )
                        {
                            sprintf(xs_msg, "[" );
                            for(int i=0;i<pi_tailleMessageCode;i++)
                            {
                                sprintf(s, "%d,",tabcode[i]);
                                strcat(xs_msg, s);
                            }
                            sprintf(s, "start= %d", start);
                            strcat(xs_msg, s);
                            strcat(xs_msg, "] | ");
                        }
                        
                        
                        for(i=0; i<imax; i++)
                        {
                            // calcul du code (% = parcours circulaire du tableau)
                            message += tabcode[(start+startcodesize+i)%pi_tailleMessageCode] << (imax-1-i);
                            if( pb_debug )
                            {
                                sprintf(s, "[%d,%d,%d] ",
                                    i,
                                    tabcode[(start+startcodesize+i)%pi_tailleMessageCode],
                                    tabcode[(start+startcodesize+i)%pi_tailleMessageCode] << (imax-1-i));
                                strcat(xs_msg, s);
                            }
                        }
                        sprintf(s, "%d", message);
                        strcat(xs_msg, s);
                        mv_LOGDEBUG(xs_msg);
                        
                        bool codevalide = true;
                        // verifie le bit de parite
                        if( pb_bitParite )
                        {
                            if( tabcode[(start+pi_tailleMessageCode-2)%pi_tailleMessageCode] != 0)  // Bit obligatoirement à 0 : tabcode[(start+3+TAILLE_MSG_CODE-3-2)%pi_tailleMessageCode]
                                codevalide = false;
                            else
                            {
                                int calculparite = 0;
                                for(i=0; i<pi_tailleMessageCode; i++)
                                    calculparite += tabcode[(start+i)%pi_tailleMessageCode];
                                if(calculparite % 2 != 0) // nombre impaire de 1 dans le code
                                    codevalide = false;
                            }
                            
                        }
                        
                        if ( codevalide )
                        {                       
                            // le message est correct: on l'enfile, on marque codeok, et on augmente la confiance
                            piste->identifiant->enfile(message);
                            piste->SetCodeok();
                            int id = piste->identifiant->majorite();
                            if( piste->confiance < LDIRegion::confianceMax )
                            {
                                if( pb_verbeux ) ReportWarning(piste->PrintConfiance(xs_msg));
                                // la piste ne pouvait etre ok avant, donc pas de AjouteFoisFaussePiste
                                if( pb_stats )
                                {
                                    xo_identifiantsStats.AjouteAnciennete(id);
                                    xo_identifiantsStats.AjouteFaussePiste(id);
                                }
                            }
                            else
                            {
                                if( pb_stats )
                                {
                                    xo_identifiantsStats.AjouteAnciennete(id);
                                    xo_identifiantsStats.AjouteCodeOk(id);
                                }
                            }
                            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "code OK"));
                        }
                        else
                        {
                            int id = piste->identifiant->majorite();
                            if( piste->SetFaussePiste(piste->parite) )
                            {
                                if( pb_stats )
                                {
                                    xo_identifiantsStats.AjouteAnciennete(id);
                                    if( codeOkPrec ) xo_identifiantsStats.AjouteFoisFaussePiste(id);
                                    xo_identifiantsStats.AjouteFaussePiste(id);
                                    xo_identifiantsStats.SetConfianceBasse(id, piste->confiance);
                                }
                                mv_LOGDEBUG(sprintf(xs_logdbgmsg, "parite erronee"));
                            } else
                            {
                                if( pb_stats )
                                {
                                    xo_identifiantsStats.AjouteAnciennete(id);
                                    xo_identifiantsStats.AjouteFoisRattrapageAnciennete(id);
                                    xo_identifiantsStats.AjouteCodeOk(id);
                                }
                                if( pb_verbeux )
                                {
                                    piste->PrintConfiance(xs_msg);
                                    strcat(xs_msg, " : rattrapee par l'anciennete");
                                    ReportWarning(xs_msg);
                                }
                                xi_nbPistesRattrapeesAnciennete++;
                                mv_LOGDEBUG(sprintf(xs_logdbgmsg, "rattrapee anciennete"));
                            }
                        }
                        
                        // decodage
                        // ------------------------------------------------------------------------------------------------
                        
                        
                    }
                    // si piste non éliminée ci-dessus
                    // ------------------------------------------------------------------------------------------------
                    
                    
                }
                // si start-bits valides trouvés
                // ------------------------------------------------------------------------------------------------
        
        
                // ------------------------------------------------------------------------------------------------
                // pas de start-bits -> fausse piste
                else
                {
                    int id = piste->identifiant->majorite();
                    if( piste->SetFaussePiste(piste->noStart) )
                    {
                        if( pb_stats )
                        {
                            xo_identifiantsStats.AjouteAnciennete(id);
                            if( codeOkPrec ) xo_identifiantsStats.AjouteFoisFaussePiste(id);
                            xo_identifiantsStats.AjouteFaussePiste(id);
                            xo_identifiantsStats.SetConfianceBasse(id, piste->confiance);
                        }
                        if( pb_verbeux ) ReportWarning(piste->PrintConfiance(xs_msg));
                        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "code noStart"));
                    } else
                    {
                        if( pb_stats )
                        {
                            xo_identifiantsStats.AjouteAnciennete(id);
                            xo_identifiantsStats.AjouteFoisRattrapageAnciennete(id);
                            xo_identifiantsStats.AjouteCodeOk(id);
                        }
                        if( pb_verbeux )
                        {
                            piste->PrintConfiance(xs_msg);
                            strcat(xs_msg, " : rattrapee par l'anciennete");
                            ReportWarning(xs_msg);
                        }
                        xi_nbPistesRattrapeesAnciennete++;
                        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "rattrapee anciennete"));
                    }
                }
                // pas de start-bits -> fausse piste
                // ------------------------------------------------------------------------------------------------
  
        
                free(tabcode);
            }
            // si code bien rempli
            // ------------------------------------------------------------------------------------------------
        
        
            else mv_LOGDEBUG("Msgcode trop petit");
        }
        // si le code de la piste a été modifié (nouveaux bits)
        // ------------------------------------------------------------------------------------------------
        else mv_LOGDEBUG("Pas de nouveau bit");
        
        xi_logdbgi--;
    }
}


/*-------------------------------------------------------------------------------------------------
  mv_DecodageCodeVariable (ajout du paramètre vitesse)

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_DecodageCodeVariable(int nbPistes)
{
    for(int pis=0; pis < nbPistes; pis++)
    {
        LDIRegion *piste = (LDIRegion*)((*po_pistes)[pis]);
        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "Piste %d", piste->id));
        xi_logdbgi++;
        bool codeOkPrec = piste->codeok;
        //piste->vitesseRecu=-1;
        
        // ------------------------------------------------------------------------------------------------
        // 1. piste effacée: on saute
        if( piste->effacee ) {
            mv_LOGDEBUG(sprintf(xs_logdbgmsg, "effacee"));
            xi_logdbgi--;
            continue;
        }
        // 1. piste effacée: on saute
        // ------------------------------------------------------------------------------------------------
        
        
        // ------------------------------------------------------------------------------------------------
        // si le code de la piste a été modifié (nouveaux bits)
        if( piste->codeModifie )
        {
            piste->codeModifie = false;
            // decodage du message pour toutes les po_pistes
            // 1. Si la file de code est bien remplie, on regarde si les 3 premiers bits sont des start-bits 110
            // 2. Si oui, on decode le nombre constitue des suivants
            
            if( piste->code->nbElements() == pi_tailleMessageCode )
            {               
                int nbNouveauxBits = piste->bitsEnfiles;
                short *tmp = piste->code->tableau();
                int tailletmp = piste->code->nbElements();
                int *tabcode = /*(short*)*/ piste->codeVariable->tableau();
                int *tabcodeTemp = piste->codeVariableTemp->tableau(); //(short*)malloc(pi_tailleMessageCode*sizeof(short));
                int tailleCodeVariable = piste->codeVariable->nbElements();
                int taillecodeTemp = piste->codeVariableTemp->nbElements();
                int startcodesize = xo_startcode.nbElements();
                short* startcodetab = xo_startcode.tableau();
                //concordanceStartCode = 0;
                bool startCodeDebut = piste->startCodeDebut;
                bool startCodeFin = piste->startCodeFin;
                bool firstStart = true;
                int tailleIdentifiant;
                if(pb_bitParite)
                    tailleIdentifiant = pi_tailleMessageCode - startcodesize - 2;
                else
                    tailleIdentifiant = pi_tailleMessageCode - startcodesize;

                
                
                /*printf("code :");
                for( int j=0; j<tailletmp; j++)
                {
                    printf("%d,",tmp[j]);
                }
                printf("\n");*/
                

                // ------------------------------------------------------------------------------------------------
                // recherche du start code debutant une nouvelle sequence
                if(!startCodeDebut)
                {
                    int j;
                    for( j=0; j<=tailletmp-startcodesize; j++)
                    {                        
                        int concordanceStartCode = 0;
                        for(int i=0; i<startcodesize; i++)
                        {
                            if(tmp[i+j]==startcodetab[i])
                            {
                                concordanceStartCode++;
                            }
                        }
                        
                        //printf("%d",tmp[i]);
                        if(concordanceStartCode == startcodesize)
                        {
                            startCodeDebut = true;
                            if (pb_debug)
                            {
                                char* report;
                                report = (char*) malloc(100*sizeof(char));
                                sprintf(report,"StartCode de debut trouve, piste : %d",pis);                            
                                ReportInfo((const char*)report);
                                delete report;
                                //ReportInfo("StartCode de debut trouve");
                            }
                            //printf("\n");
                            break;
                        }
                    }
                    
                    // si un start code a ete trouve, on empile les bits suivants dans un tableau
                    if( startCodeDebut == true )
                    {
                        piste->codeVariable->vide();
                        //printf("recu");
                        for (j+=startcodesize; j<tailletmp;j++)
                        {
                            piste->codeVariable->empile(tmp[j]);
                           // printf("%d, ",tmp[j]);
                        }
                        firstStart = false;
                        tailleCodeVariable = piste->codeVariable->nbElements();
                        //printf("\n");
                    }
                }
                // recherche du start code debutant une nouvelle sequence
                // ------------------------------------------------------------------------------------------------
                

                
                if(startCodeDebut && (!startCodeFin)&& (firstStart))
                {
                    int i;
                    
                    for( i=0; (i<nbNouveauxBits)/*&&(tailleCodeVariable+i<TAILLE_VARIABLE_MAX)*/; i++)
                    {
                        /*if( (tmp[i+tailletmp-nbNouveauxBits]==startcodetab[i]) 
                            &&(tmp[i+tailletmp-nbNouveauxBits+1]==startcodetab[i+1]) 
                            &&(tmp[i+tailletmp-nbNouveauxBits+2]==startcodetab[i+2]) 
                            &&(tmp[i+tailletmp-nbNouveauxBits+3]==startcodetab[i+3]))
                        {
                        }
                        else*/ piste->codeVariable->empile(tmp[tailletmp-nbNouveauxBits+i]);
                        tailleCodeVariable = piste->codeVariable->nbElements();

                       // printf("%d, ",tmp[tailletmp-nbNouveauxBits+i]);
                    }
                    //printf("\n");
                    /*if(tailleCodeVariable+i>=TAILLE_VARIABLE_MAX)
                    {
                        char* report;
                        report = (char*) malloc(100*sizeof(char));
                        sprintf(report,"Trame trop longue, piste : %d",pis);                            
                        ReportInfo((const char*)report);
                        delete report;
                        startCodeDebut = false;
                    }*/

                    int j;
                    int emplacementStartCodeFin= tailleCodeVariable;
                    for( j=0; j<=tailleCodeVariable-startcodesize; j++)
                    {                        
                        int concordanceStartCode = 0;
                        for(i=0; i<startcodesize; i++)
                        {
                            if(tabcode[i+j]==((int)startcodetab[i]))
                            {
                                concordanceStartCode++;
                            }
                        }

                        /*char* report;
                        report = (char*) malloc(100*sizeof(char));
                        sprintf(report,"  %i",concordanceStartCode);
                        

                        ReportInfo((const char*)report);
                        delete report;*/

                        if(concordanceStartCode == startcodesize)
                        {
                            startCodeFin = true;
                            if (pb_debug)
                            {
                                char* report;
                                report = (char*) malloc(100*sizeof(char));
                                sprintf(report,"StartCode de fin trouve, piste : %d",pis);                            
                                ReportInfo((const char*)report);
                                delete report;
                            //ReportInfo("StartCode de fin trouve");
                            }
                            //printf("startcode de fin trouve \n");
                            //printf("%d bits a depiler",tailleCodeVariable-emplacementStartCodeFin);
                            emplacementStartCodeFin = j;
                            break;
                        }
                    }
                    if(startCodeFin == true)
                    {
                        int k;
                        int depile;

                        for(k=0; k<tailleCodeVariable-emplacementStartCodeFin;k++)
                        {
                            piste->codeVariable->depile(&depile);
                            if(((tailleCodeVariable-emplacementStartCodeFin)-k)>startcodesize)
                                piste->codeVariableTemp->empile(depile);
                            //tailleCodeVariable = piste->codeVariable->nbElements();
                        }
                        tailleCodeVariable = piste->codeVariable->nbElements();
                        taillecodeTemp = piste->codeVariableTemp->nbElements();
                        
                        //printf("traite");
                        /*for (k=0; k<tailleCodeVariable;k++)
                        {
                           
                            printf("%d, ",tabcode[k]);
                        }
                        
                        printf("\n");*/
                        /*if(((piste->codeVariable->nbElements()< (tailleIdentifiant+LNG_VITESSE+2))&& pb_bitParite)||
                            ((piste->codeVariable->nbElements()< (tailleIdentifiant+LNG_VITESSE))&& (!pb_bitParite)))
                        {
                            startCodeDebut = false;
                            startCodeFin = false;
                            piste->startCodeDebut = startCodeDebut;
                            piste->startCodeFin = startCodeFin;
                            if (pb_debug)
                            {
                                char* report;
                                report = (char*) malloc(100*sizeof(char));
                                sprintf(report,"Message trop petit : %d, piste : %d",piste->codeVariable->nbElements(),pis);
                                ReportInfo((const char*)report);
                                delete report;
                            }

                        }*/
                    }

                    if(tailleCodeVariable+i>=TAILLE_VARIABLE_MAX)
                    {
                        if(pb_debug)
                        {
                            char* report;
                            report = (char*) malloc(100*sizeof(char));
                            sprintf(report,"Trame trop longue, piste : %d",pis);                            
                            ReportInfo((const char*)report);
                            delete report;
                        }
                        piste->vitesseRecu=-1;
                        startCodeDebut = false;
                        startCodeFin = false;
                    }
                    
                        
                }

                
                //tailleCodeVariable = piste->codeVariable->nbElements();
                //printf("taille code variable : %d \n",tailleCodeVariable);

                
                /*if(tailleCodeVariable < (pi_tailleMessageCode+LNG_VITESSE))
                {
                    startCodeDebut = false;
                    startCodeFin = false;
                }*/
                

                piste->startCodeDebut = startCodeDebut;
                piste->startCodeFin = startCodeFin;
                /*char* report;
                        report = (char*) malloc(100*sizeof(char));
                        sprintf(report,"bip");
                        

                        ReportInfo((const char*)report);
                        delete report;*/
                // une sequence entiere est dans le codeVariable
                if( (startCodeDebut) && (startCodeFin) )
                {

                    // ------------------------------------------------------------------------------------------------
                    // Suppression du bit-stuffing
                    
                    int i,j;
	                char de_bit_stuffing = 0;
                    char *donnees_corrigees_reception;
                    int xo_longueurDonneesMax;
                    int tailledonneescorrigees =0;
                    donnees_corrigees_reception = (char*) malloc((tailleIdentifiant+LNG_VITESSE)*sizeof(char));
                    char *donnees_brutes;
                    donnees_brutes = (char*) malloc((tailleIdentifiant+LNG_VITESSE)*sizeof(char));
                    //char *donnees_corrigees;
                    if(pb_bitParite)
                    {
                        //donnees_corrigees = (char*) malloc((pi_tailleMessageCode+LNG_VITESSE+startcodesize+2)*sizeof(char));
                        xo_longueurDonneesMax = tailleIdentifiant+LNG_VITESSE/*+startcodesize*/+2;
                    }
                    else
                    {
                        //donnees_corrigees = (char*) malloc((pi_tailleMessageCode+LNG_VITESSE+startcodesize)*sizeof(char));
                        xo_longueurDonneesMax = tailleIdentifiant+LNG_VITESSE/*+startcodesize*/;
                    }
                    for (i = 0; i < tailleCodeVariable; i++)
		                donnees_corrigees_reception[i]=6;

                    
                    //char* report;
                    //            report = (char*) malloc(100*sizeof(char));
                    //            char* t;
                    //            t = (char*) malloc(2*sizeof(char));
                    //            sprintf(report, " recu[" );
                    //            for(i=0;i<tailleCodeVariable;i++)
                    //            {
                    //                sprintf(t, "%d,",tabcode[i]);
                    //                strcat(report, t);
                    //            }
                    //            /*sprintf(s, "start= %d", start);
                    //            strcat(xs_msg, s);*/
                    //            strcat(report, "] ");
                    //            ReportInfo(report);
                    //            
                    //            delete report;
                    //            delete t;

                    
                    tailledonneescorrigees=tailleCodeVariable;
                    //ON PARCOURT LA SEQUENCE RECUE
	                for (i = 0; i < tailleCodeVariable; i++)
	                {
		                //if ((i >= 0) && (i <= xo_longueurDonneesMax - startcodesize))
		                //{ //Si cas général
			                //Si on retrouve les 3 premiers bits du start code suivis de '0'
			                if (tabcode[i] == startcodetab[0] && tabcode[i+1] == startcodetab[1] && tabcode[i + 2] == startcodetab[2] && tabcode[i + 3] == 0)
			                {
				                for (j = 0; j < 4; j++)
				                {
					                if (j < 3)
                                    {
						                donnees_corrigees_reception[i - de_bit_stuffing + j] = tabcode[i + j];
                                        
                                    }
					                else if (j == 3)
					                {
						                //	donnees_corrigees_reception[i - de_bit_stuffing + j] = donnees_corrigees[i + j + 1];
						                de_bit_stuffing++;
					                }
				                }
				                i = i+3;
                                tailledonneescorrigees--;
			                }
			                //Sinon
			                else 
			                {
				                donnees_corrigees_reception[i - de_bit_stuffing] = tabcode[i];
                                //tailledonneescorrigees++;
			                }
		                //}
		                /*else
		                {
			                donnees_corrigees_reception[i - de_bit_stuffing] = tabcode[i];
                            tailledonneescorrigees++;
		                }*/
	                }
                    // Suppression du bit-stuffing
                    // ------------------------------------------------------------------------------------------------

//-------------------------------------
                    
                    if(tailledonneescorrigees == (xo_longueurDonneesMax))
                    
                    {
                        // Separation identifiant et donnees
                        int p;
                        char *identifiant_recu, *vitesse_recu;
                        identifiant_recu = (char*) malloc(tailleIdentifiant*sizeof(char));
                        vitesse_recu = (char*) malloc(LNG_VITESSE*sizeof(char));
	                    for (p=0; p < tailleIdentifiant; p++)
		                identifiant_recu[p]=donnees_corrigees_reception[p];
	                    for (p=0; p < LNG_VITESSE; p++)
		                vitesse_recu[p]=donnees_corrigees_reception[p+tailleIdentifiant];

                        /*if( pb_debug )
                            {*/
                                //char* report;
                                //report = (char*) malloc(100*sizeof(char));
                                //char* t;
                                //t = (char*) malloc(2*sizeof(char));
                                //sprintf(report, " identifiant[" );
                                //for(i=0;i<tailleIdentifiant;i++)
                                //{
                                //    sprintf(t, "%d,",identifiant_recu[i]);
                                //    strcat(report, t);
                                //}
                                ///*sprintf(s, "start= %d", start);
                                //strcat(xs_msg, s);*/
                                //strcat(report, "] ");
                                //ReportInfo(report);
                                /*delete report;
                                delete t;

                                char* report;
                                report = (char*) malloc(100*sizeof(char));
                                char* t;
                                t = (char*) malloc(2*sizeof(char));*/
                                //sprintf(report, " vitesse[" );
                                //for(i=0;i<LNG_VITESSE;i++)
                                //{
                                //    sprintf(t, "%d,",vitesse_recu[i]);
                                //    strcat(report, t);
                                //}
                                ///*sprintf(s, "start= %d", start);
                                //strcat(xs_msg, s);*/
                                //strcat(report, "] ");
                                //ReportInfo(report);
                                //delete report;
                                //delete t;
                                
                                

                           /* }*/                 
                            
                            // ------------------------------------------------------------------------------------------------
                            // decodage
                            short message = 0;
                            short vitesse = 0;

                            char s[128];
                            /*int imax = (pb_bitParite ? pi_tailleMessageCode-startcodesize-2 : pi_tailleMessageCode-startcodesize);
                            */
                            int imax = tailleIdentifiant;
                            //int imax = (pb_bitParite ? pi_tailleMessageCode-2 : pi_tailleMessageCode);


                            if( pb_debug )
                            {
                                sprintf(xs_msg, "[" );
                                for(i=0;i<tailleIdentifiant;i++)
                                {
                                    sprintf(s, "%d,",identifiant_recu[i]);
                                    strcat(xs_msg, s);
                                }
                                /*sprintf(s, "start= %d", start);
                                strcat(xs_msg, s);*/
                                strcat(xs_msg, "] | ");
                                
                                

                            }
                            
                            
                            for(i=0; i<imax; i++)
                            {
                                // calcul du code (% = parcours circulaire du tableau)
                                /*message += identifiant_recu[(startcodesize+i)%pi_tailleMessageCode] << (imax-1-i);*/
                                message += identifiant_recu[i] << (imax-1-i);
                                if( pb_debug )
                                {
                                    sprintf(s, "[%d,%d,%d] ",
                                        i,
                                        identifiant_recu[i],
                                        identifiant_recu[i] << (imax-1-i));
                                    strcat(xs_msg, s);
                                }
                            }
                            sprintf(s, "%d", message);
                            strcat(xs_msg, s);

                            for(i=0; i<LNG_VITESSE; i++)
                            {
                                // calcul du code (% = parcours circulaire du tableau)
                                /*message += identifiant_recu[(startcodesize+i)%pi_tailleMessageCode] << (imax-1-i);*/
                                vitesse += vitesse_recu[i] << (LNG_VITESSE-1-i);
                                
                            }
                            piste->vitesseRecu=vitesse;


                            if( pb_debug )
                            {
                                strcat(xs_msg, " vitesse : [");
                                for(i=0;i<LNG_VITESSE;i++)
                                {
                                    sprintf(s, "%d,",vitesse_recu[i]);
                                    strcat(xs_msg, s);
                                }
                                strcat(xs_msg, "]");
                            }
                            
                            mv_LOGDEBUG(xs_msg);
                            
    //----------------------------------------                       

                            delete identifiant_recu;
                            delete vitesse_recu;
                            bool codevalide = true;
                            // verifie le bit de parite
                            
                            if( pb_bitParite )
                            {
                                if( donnees_corrigees_reception[tailleIdentifiant+LNG_VITESSE] != 0)  // Bit obligatoirement à 0 : tabcode[(start+3+TAILLE_MSG_CODE-3-2)%pi_tailleMessageCode]
                                    codevalide = false;
                                else
                                {
                                    int calculparite = 0;
                                    for(i=0; i<tailleIdentifiant+LNG_VITESSE+2; i++)
                                        calculparite += donnees_corrigees_reception[i];
                                    for(i=0; i<startcodesize; i++)
                                        calculparite += startcodetab[i];
                                    if(calculparite % 2 != 0) // nombre impaire de 1 dans le code
                                        codevalide = false;
                                }
                                
                            }
                            
                            if ( codevalide )
                            {
                                // le message est correct: on l'enfile, on marque codeok, et on augmente la confiance
                                piste->identifiant->enfile(message);
                                piste->SetCodeok();
                                int id = piste->identifiant->majorite();
                                if( piste->confiance < LDIRegion::confianceMax )
                                {
                                    if( pb_verbeux ) ReportWarning(piste->PrintConfiance(xs_msg));
                                    // la piste ne pouvait etre ok avant, donc pas de AjouteFoisFaussePiste
                                    if( pb_stats )
                                    {
                                        xo_identifiantsStats.AjouteAnciennete(id);
                                        xo_identifiantsStats.AjouteFaussePiste(id);
                                    }
                                }
                                else
                                {
                                    if( pb_stats )
                                    {
                                        xo_identifiantsStats.AjouteAnciennete(id);
                                        xo_identifiantsStats.AjouteCodeOk(id);
                                    }
                                }
                                mv_LOGDEBUG(sprintf(xs_logdbgmsg, "code OK"));
                            }
                            else
                            {
                                piste->vitesseRecu=-1;
                                int id = piste->identifiant->majorite();
                                if( piste->SetFaussePiste(piste->parite) )
                                {
                                    if( pb_stats )
                                    {
                                        xo_identifiantsStats.AjouteAnciennete(id);
                                        if( codeOkPrec ) xo_identifiantsStats.AjouteFoisFaussePiste(id);
                                        xo_identifiantsStats.AjouteFaussePiste(id);
                                        xo_identifiantsStats.SetConfianceBasse(id, piste->confiance);
                                    }
                                    mv_LOGDEBUG(sprintf(xs_logdbgmsg, "parite erronee"));
                                } else
                                {
                                    if( pb_stats )
                                    {
                                        xo_identifiantsStats.AjouteAnciennete(id);
                                        xo_identifiantsStats.AjouteFoisRattrapageAnciennete(id);
                                        xo_identifiantsStats.AjouteCodeOk(id);
                                    }
                                    if( pb_verbeux )
                                    {
                                        piste->PrintConfiance(xs_msg);
                                        strcat(xs_msg, " : rattrapee par l'anciennete");
                                        ReportWarning(xs_msg);
                                    }
                                    xi_nbPistesRattrapeesAnciennete++;
                                    mv_LOGDEBUG(sprintf(xs_logdbgmsg, "rattrapee anciennete"));
                                }
                            }
                            
    //-------------------------
                            
                        }
                        
                    
                    // message trop petit ou trop grand pour contenir un identifiant et une vitesse
                    else
                        {
                            piste->vitesseRecu=-1;
                            startCodeDebut = false;
                            startCodeFin = false;
                            piste->startCodeDebut = startCodeDebut;
                            piste->startCodeFin = startCodeFin;
                            if (pb_debug)
                            {
                                char* report;
                                report = (char*) malloc(100*sizeof(char));
                                if(tailledonneescorrigees < (LNG_VITESSE+tailleIdentifiant))
                                    sprintf(report,"Message trop petit : %d, piste : %d",tailledonneescorrigees,pis);
                                else
                                    sprintf(report,"Message trop grand : %d, piste : %d",tailledonneescorrigees,pis);
                                ReportInfo((const char*)report);
                                delete report;
                            //ReportInfo("StartCode de fin trouve");
                            }
                        }

                        // decodage
                        // ------------------------------------------------------------------------------------------------
                        
               }         
                   // }
                    // si piste non éliminée ci-dessus
                    // ------------------------------------------------------------------------------------------------
                    
                    
             //   }
                // si start-bits valides trouvés
                // ------------------------------------------------------------------------------------------------
        
        
                // ------------------------------------------------------------------------------------------------
                // pas de start-bits -> fausse piste
                if(!startCodeDebut)
                {
                    piste->vitesseRecu=-1;
                    
                    int id = piste->identifiant->majorite();
                    if( piste->SetFaussePiste(piste->noStart) )
                    {
                        if( pb_stats )
                        {
                            xo_identifiantsStats.AjouteAnciennete(id);
                            if( codeOkPrec ) xo_identifiantsStats.AjouteFoisFaussePiste(id);
                            xo_identifiantsStats.AjouteFaussePiste(id);
                            xo_identifiantsStats.SetConfianceBasse(id, piste->confiance);
                        }
                        if( pb_verbeux ) ReportWarning(piste->PrintConfiance(xs_msg));
                        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "code noStart"));
                    } else
                    {
                        if( pb_stats )
                        {
                            xo_identifiantsStats.AjouteAnciennete(id);
                            xo_identifiantsStats.AjouteFoisRattrapageAnciennete(id);
                            xo_identifiantsStats.AjouteCodeOk(id);
                        }
                        if( pb_verbeux )
                        {
                            piste->PrintConfiance(xs_msg);
                            strcat(xs_msg, " : rattrapee par l'anciennete");
                            ReportWarning(xs_msg);
                        }
                        xi_nbPistesRattrapeesAnciennete++;
                        mv_LOGDEBUG(sprintf(xs_logdbgmsg, "rattrapee anciennete"));
                    }
                }
                // pas de start-bits -> fausse piste
                // ------------------------------------------------------------------------------------------------

                if( (startCodeDebut) && (startCodeFin) )
                {
                    //piste->startCodeDebut = true;
                    piste->startCodeFin = false;
                    piste->codeVariable->vide();
                    for(int i=0; i<taillecodeTemp; i++)
                    {
                        piste->codeVariable->empile(tabcodeTemp[i]);
                    }
                    piste->codeVariableTemp->vide();
                                    
                                    
                }
                //free(tabcode);
            }
            // si code bien rempli
            // ------------------------------------------------------------------------------------------------
        
        
            else mv_LOGDEBUG("Msgcode trop petit");
        }
        // si le code de la piste a été modifié (nouveaux bits)
        // ------------------------------------------------------------------------------------------------
        else mv_LOGDEBUG("Pas de nouveau bit");
        
        xi_logdbgi--;
    }

}
/*-------------------------------------------------------------------------------------------------
  mv_sortieVitesses

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/

//void MAPSLivicDecodageInfrarouge::mv_sortieVitesses(int nbPistes)
//{
//
//    if( nbPistes <= 0 ) return;
//    // récupération de toutes le pistes
//    LivicPilePointeur pistesSorties;
//    
//    for(int pis=0; pis < nbPistes; pis++)
//    {
//        LDIRegion *piste = (LDIRegion*)((*po_pistes)[pis]);
//        if( piste->effacee ) continue;
//        if( (piste->codeok|| pb_sortieToutesPisteIR) )
//        {
//            pistesSorties.empile(piste);
//        }
//    }
//    
//    // recopie sur la sortie RealObject
//    MAPSIOElt *oEltVitesses = StartWriting(Output("oVitesses"));
//    MAPSFloat *Vitesses = &oEltVitesses->Float();
//    oEltVitesses->VectorSize() = pistesSorties.nbElements();
//    oEltVitesses->Timestamp() = xt_timestamp;
//    int nbPistesSorties = pistesSorties.nbElements();
//
//    for(int i=0; i<nbPistesSorties; i++)
//    {
//        LDIRegion *piste = NULL;
//        pistesSorties.depile((void**)&piste);
//        Vitesses[i] = (float)piste->vitesseRecu;       
//    }
//    StopWriting(oEltVitesses);
//    
//    
//}

/*-------------------------------------------------------------------------------------------------
  mv_EffacementPistesEffacees

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_EffacementPistesEffacees(int *nbPistes)
{
    LivicPilePointeur* pistesGardees = new LivicPilePointeur(NBPISTES);
    pistesGardees->vide();

   
    for(int i=0; i<*nbPistes; i++)
    {
        LDIRegion *piste = (LDIRegion*)((*po_pistes)[i]);
        LivicPilePoint2D *piste2D = piste->region;
        if(!piste->effacee)
        {
            pistesGardees->empile(piste);
        }
        else delete piste;
    }
    po_pistes->vide();
    delete po_pistes;
    po_pistes=pistesGardees;
    *nbPistes=po_pistes->nbElements();
    //delete pistesGardees;

}

/*-------------------------------------------------------------------------------------------------
  mv_FiltrageReflets

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_FiltrageReflets(int nbPistes)
{
    if( nbPistes <= 0 ) return;
    // récupération de toutes le pistes
   // LivicPilePointeur pistesSorties;
    
    for(int pis=0; pis < nbPistes; pis++)
    {
        LDIRegion *piste = (LDIRegion*)((*po_pistes)[pis]);
		if( !piste->codeok ) continue;
        for(int pis2=0; pis2 < nbPistes; pis2++)
        {
            if (pis2 == pis) continue;
            LDIRegion *piste2 = (LDIRegion*)((*po_pistes)[pis2]);
			if( !piste2->codeok ) continue;
            if( piste->effacee || piste2->effacee) continue;
            if(  piste->identifiant->majorite() == piste2->identifiant->majorite() )
            {
				// elimination definitive de la piste
				if( piste->bar.y > piste2->bar.y ) {
					piste->confiance = 0;
					piste->SetFaussePiste(piste->reflet);
					sprintf(xs_msg, "reflet: %d", piste->id);
					ReportError(xs_msg);
				}
				if( piste2->bar.y > piste->bar.y ) {
					piste2->confiance = 0;
                    piste2->SetFaussePiste(piste2->reflet);
					sprintf(xs_msg, "reflet: %d", piste2->id);
					ReportError(xs_msg);
				} 
            }
        }
    }
}

/*-------------------------------------------------------------------------------------------------
  mv_SortiePistesIR

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_SortiePistesIR(int nbPistes)
{
    if( nbPistes <= 0 ) return;
    // récupération de toutes le pistes
    LivicPilePointeur pistesSorties;
    
    for(int pis=0; pis < nbPistes; pis++)
    {
        LDIRegion *piste = (LDIRegion*)((*po_pistes)[pis]);
        if( piste->effacee ) continue;
        if( piste->codeok || pb_sortieToutesPisteIR )
        {
            pistesSorties.empile(piste);
        }
    }
    
    // recopie sur la sortie RealObject
    MAPSIOElt *oEltPisteIR = StartWriting(Output("oPistesIR"));
    MAPSRealObject *pistesIR = &oEltPisteIR->RealObject();
    oEltPisteIR->VectorSize() = pistesSorties.nbElements();
    oEltPisteIR->Timestamp() = xt_timestamp;
    int nbPistesSorties = pistesSorties.nbElements();

    for(int i=0; i<nbPistesSorties; i++)
    {
        LDIRegion *piste = NULL;
        pistesSorties.depile((void**)&piste);
        pistesIR[i].x = piste->bar.x;
        pistesIR[i].y = piste->bar.y;
        pistesIR[i].id = piste->identifiant->majorite();
        pistesIR[i].misc1 = piste->id;
        pistesIR[i].misc2 = piste->faussePiste;
        pistesIR[i].vehicle.speed = piste->vitesseRecu;
    }
    StopWriting(oEltPisteIR);
}

/*-------------------------------------------------------------------------------------------------
  mv_RecopieImagesRegions

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_RecopieImagesRegions(
                                                          unsigned char *obR,
                                                          unsigned char *obR1,
                                                          unsigned char *imgRegions,
                                                          unsigned char *imgRegions1)
{
    
    
    // ------------------------------------------------------------------------------------------------
    // mise à 0 des images
    memset(obR, 0, (xi_largeur+8)*xi_hauteur*3);
    memset(obR1, 0, (xi_largeur+8)*xi_hauteur*3);
    // mise à 0 des images
    // ------------------------------------------------------------------------------------------------
    
    
    int pixelib, pixelob, pixelreg;
    for(int ii=0 ; ii < xi_largeur ; ii++)
    {
        pixelib = pixelreg = ii;
        for(int jj=0 ; jj < xi_hauteur ; jj++)
        {
            pixelob = 3*pixelib;
            // calcul des couleurs pour les régions (mode debug uniquement)
            obR[pixelob]   = (imgRegions[pixelreg]*COEF_COULEUR_REGION_R)%255;
            obR[pixelob+1] = (imgRegions[pixelreg]*COEF_COULEUR_REGION_V)%255;
            obR[pixelob+2] = (imgRegions[pixelreg]*COEF_COULEUR_REGION_B)%255;
            obR1[pixelob]  = (imgRegions1[pixelreg]*COEF_COULEUR_REGION_R)%255;
            obR1[pixelob+1]= (imgRegions1[pixelreg]*COEF_COULEUR_REGION_V)%255;
            obR1[pixelob+2]= (imgRegions1[pixelreg]*COEF_COULEUR_REGION_B)%255;
            /*obS[3*ii+3*(xi_largeur+8)*jj] = ib[ii+(xi_largeur+8)*jj];
            obS[3*ii+3*(xi_largeur+8)*jj+1] = ib[ii+(xi_largeur+8)*jj];
            obS[3*ii+3*(xi_largeur+8)*jj+2] = ib[ii+(xi_largeur+8)*jj];
            obR[3*ii+3*(xi_largeur+8)*jj] = (imgRegions[ii+xi_largeur*jj]*COEF_COULEUR_REGION_R)%255;
            obR[3*ii+3*(xi_largeur+8)*jj+1] = (imgRegions[ii+xi_largeur*jj]*COEF_COULEUR_REGION_V)%255;
            obR[3*ii+3*(xi_largeur+8)*jj+2] = (imgRegions[ii+xi_largeur*jj]*COEF_COULEUR_REGION_B)%255;*/
            pixelib += (xi_largeur+8);
            pixelreg += xi_largeur;
        }
    }
}


/*-------------------------------------------------------------------------------------------------
  mv_RecopieImageSortie

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_RecopieImageSortie(
                                                        unsigned char *obS,
                                                        unsigned char *ib)
{
    long taille = (xi_largeur+8)*xi_hauteur;
    for(int p=0; p<taille; p++)
    {
        memset(obS+3*p, ib[p], 3);
    }
}


/*-------------------------------------------------------------------------------------------------
  mv_AffichageImagesRegions

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_AffichageImagesRegions(unsigned char *obR, unsigned char *obR1)
{
    
    
    // ------------------------------------------------------------------------------------------------
    // affichage du numéro d'image
    sprintf(xs_msg, "%ld", (long)xt_timestamp);
    printT2I(
        obR,
        xs_msg,
        xi_largeur-60,
        10,
        WHITE,
        xi_largeur+8,
        xi_hauteur);
    printT2I(
        obR1,
        xs_msg,
        xi_largeur-60,
        10,
        WHITE,
        xi_largeur+8,
        xi_hauteur);
    // affichage du numéro d'image
    // ------------------------------------------------------------------------------------------------
    
    
    // ------------------------------------------------------------------------------------------------
    // légende des numéros et couleurs de région
    for(int nr=1; nr<(xi_hauteur/5); nr++)
    {
        for(int nri=0; nri<4; nri++)
        {
            TraceRGBLigne(
                obR,
                10, 5*nr+nri, 20, 5*nr+nri,
                (nr*COEF_COULEUR_REGION_R)%255,
                (nr*COEF_COULEUR_REGION_V)%255,
                (nr*COEF_COULEUR_REGION_B)%255,
                xi_largeur+8);
            TraceRGBLigne(
                obR1,
                10, 5*nr+nri, 20, 5*nr+nri,
                (nr*COEF_COULEUR_REGION_R)%255,
                (nr*COEF_COULEUR_REGION_V)%255,
                (nr*COEF_COULEUR_REGION_B)%255,
                xi_largeur+8);
        }
        if( nr<((xi_hauteur/5)/5) )
        {
            TraceRGBLigne(
                obR,
                21, 5*nr*5+4, 35, 5*nr*5+4,
                0, 255, 0,
                xi_largeur+8);
            TraceRGBLigne(
                obR1,
                21, 5*nr*5+4, 35, 5*nr*5+4,
                0, 255, 0,
                xi_largeur+8);
            sprintf(xs_msg, "%d", nr*5);
            printT2I(
                obR,
                xs_msg,
                22,
                5*nr*5-3,
                GREEN,
                xi_largeur+8,
                xi_hauteur);
            printT2I(
                obR1,
                xs_msg,
                22,
                5*nr*5-3,
                GREEN,
                xi_largeur+8,
                xi_hauteur);
        }
    }
    // légende des numéros et couleurs de région
    // ------------------------------------------------------------------------------------------------
    
    
}


/*-------------------------------------------------------------------------------------------------
  mv_AffichageImageSortie

  Utilisée par: Core
-------------------------------------------------------------------------------------------------*/
void MAPSLivicDecodageInfrarouge::mv_AffichageImageSortie(
                                                          unsigned char *obS,
                                                          int nbRegions,
                                                          int nbPistes)
{
    
     if(pb_affichageDetaille)
    {
        // ------------------------------------------------------------------------------------------------
        // affichage du numéro d'image
        sprintf(xs_msg, "%ld", (long)xt_timestamp);
        printT2I(
            obS,
            xs_msg,
            xi_largeur-60,
            10,
            WHITE,
            xi_largeur+8,
            xi_hauteur);
        // affichage du numéro d'image
        // ------------------------------------------------------------------------------------------------
        
       
        // ------------------------------------------------------------------------------------------------
        // affichage du nombre de po_pistes et du nombre d'images par bit
       
        sprintf(xs_msg, "Pistes=%d, Images par bit=%.2f", nbPistes, pf_nbImagesParBit);
        printT2I(obS, xs_msg, T2I_MARGE, T2I_MARGE, GREEN, xi_largeur+8, xi_hauteur);
        // affichage du nombre de po_pistes et du nombre d'images par bit
        // ------------------------------------------------------------------------------------------------
    }
    
    // ------------------------------------------------------------------------------------------------
    // affichage de la légende des couleurs en mode calibrage
    if( LDIRegion::modeCalibrage )
    {
        int legendeCouleur = 0;
        char *legendeCouleurTxt = "";
        printT2I(
            obS,
            "CALIBRAGE",
            xi_largeur+8 - 70,
            xi_hauteur - (T2I_INTERLIGNE*5) - T2I_MARGE - 5,
            WHITE,
            xi_largeur+8,
            xi_hauteur
            );
        for(int nr=0; nr<4; nr++)
        {
            switch (nr) {
            case 3: {legendeCouleur = GREEN; legendeCouleurTxt = "nouvelle"; break;}
            case 2: {legendeCouleur = CYAN; legendeCouleurTxt = "calib"; break;}
            case 1: {legendeCouleur = RED; legendeCouleurTxt = "non calib"; break;}
            case 0: {legendeCouleur = ORANGE; legendeCouleurTxt = "choisie"; break;}
            default: {legendeCouleur = WHITE; legendeCouleurTxt = "";}
            }
            printT2I(
                obS,
                legendeCouleurTxt,
                xi_largeur+8 - 70,
                xi_hauteur - (T2I_INTERLIGNE*nr) - T2I_MARGE - 5,
                legendeCouleur,
                xi_largeur+8,
                xi_hauteur
                );
            for(int nri=0; nri<4; nri++)
            {
                DrawRGBLine(
                    obS,
                    xi_largeur+8 - 10 - T2I_MARGE, xi_hauteur - (T2I_INTERLIGNE*nr+nri) - T2I_MARGE, xi_largeur+8 - T2I_MARGE, xi_hauteur - (T2I_INTERLIGNE*nr+nri) - T2I_MARGE,
                    legendeCouleur,
                    xi_largeur+8,
                    xi_hauteur);
            }
        }
    }
    // affichage de la légende des couleurs en mode calibrage
    // ------------------------------------------------------------------------------------------------
    
    
    // ------------------------------------------------------------------------------------------------
    // affichage de la légende des couleurs en mode normal
    else if(pb_affichageDetaille)
    {
        {
            int legendeCouleur = 0;
            char *legendeCouleurTxt = "";
            for(int nr=0; nr<7; nr++)
            {
                switch (nr) {
                    
                case 6: {legendeCouleur = GREEN; legendeCouleurTxt = "nouvelle"; break;}
                case 5: {legendeCouleur = CYAN; legendeCouleurTxt = "codeok"; break;}
                case 4: {legendeCouleur = YELLOW; legendeCouleurTxt = "fp:continu"; break;}
                case 3: {legendeCouleur = ORANGE; legendeCouleurTxt = "fp:interdit"; break;}
                case 2: {legendeCouleur = RED; legendeCouleurTxt = "fp:nostart"; break;}
                case 1: {legendeCouleur = PURPLE; legendeCouleurTxt = "fp:parite"; break;}
                case 0: {legendeCouleur = BLUE; legendeCouleurTxt = "fp:inconnu"; break;}
                default: {legendeCouleur = WHITE; legendeCouleurTxt = "";}
                }
                printT2I(
                    obS,
                    legendeCouleurTxt,
                    xi_largeur+8 - 70,
                    xi_hauteur - (T2I_INTERLIGNE*nr) - T2I_MARGE - 5,
                    legendeCouleur,
                    xi_largeur+8,
                    xi_hauteur
                    );
                for(int nri=0; nri<4; nri++)
                {
                    DrawRGBLine(
                        obS,
                        xi_largeur+8 - 10 - T2I_MARGE, xi_hauteur - (T2I_INTERLIGNE*nr+nri) - T2I_MARGE, xi_largeur+8 - T2I_MARGE, xi_hauteur - (T2I_INTERLIGNE*nr+nri) - T2I_MARGE,
                        legendeCouleur,
                        xi_largeur+8,
                        xi_hauteur);
                }
            }
        }
    }
    // affichage de la légende des couleurs en mode normal
    // ------------------------------------------------------------------------------------------------
    
	// ------------------------------------------------------------------------------------------------
    // affichage de la ligne d'horizon
	if( pi_hauteurHorizon>0 )
	{
        DrawRGBLine(
            obS,
            0, pi_hauteurHorizon, xi_largeur, pi_hauteurHorizon,
            YELLOW,
            xi_largeur+8,
            xi_hauteur);
	}
    // affichage de la ligne d'horizon
	// ------------------------------------------------------------------------------------------------

	// ------------------------------------------------------------------------------------------------
    // affichage de la valeur du seuil
	//PIDSeuil if( pb_PIDSeuil )
	//PIDSeuil {
	//PIDSeuil 	sprintf(xs_msg, "%d seuil=%d int=%d E=%d P=%d I=%d D=%d",
	//PIDSeuil 		pi_seuil,
	//PIDSeuil 		xi_aff_PID_Seuil,
	//PIDSeuil 		xi_aff_PID_intensiteMoyenne,
	//PIDSeuil 		(int)xd_E_PIDSeuilPrec,
	//PIDSeuil 		xi_aff_PID_P,
	//PIDSeuil 		xi_aff_PID_I,
	//PIDSeuil 		xi_aff_PID_D
	//PIDSeuil 		);
	//PIDSeuil 	printT2I(
	//PIDSeuil 		obS,
	//PIDSeuil 		xs_msg,
	//PIDSeuil 		5,
	//PIDSeuil 		xi_hauteur-10,
	//PIDSeuil 		GREEN,
	//PIDSeuil 		xi_largeur+8,
	//PIDSeuil 		xi_hauteur
	//PIDSeuil 		);
	//PIDSeuil }
    // affichage de la valeur du seuil
    // ------------------------------------------------------------------------------------------------
    
    // ------------------------------------------------------------------------------------------------
    // affichage des po_pistes
    int interligne = 0;
    for(int pis=0; pis < nbPistes; pis++)
    {
        int couleur = GREEN;
        LDIRegion *piste = (LDIRegion*)((*po_pistes)[pis]);
        if( piste->effacee ) {
            continue;
        }
        
        
        // ------------------------------------------------------------------------------------------------
        // choix de la couleur en mode normal selon le statut de la piste
        if( !LDIRegion::modeCalibrage )
        {
            if( piste->codeok ) couleur = CYAN;
            if( piste->faussePiste )
            {
                switch (piste->faussePiste) {
                case piste->nouvelle: couleur = GREEN; break;
                case piste->continu: couleur = YELLOW; break;
                case piste->codeInterdit: couleur = ORANGE; break;
                case piste->noStart: couleur = RED; break;
                case piste->parite: couleur = PURPLE; break;
                default: couleur = BLUE; break;
                }
            }
        }
        // choix de la couleur en mode normal selon le statut de la piste
        // ------------------------------------------------------------------------------------------------
        
        
        // ------------------------------------------------------------------------------------------------
        // choix de la couleur en mode calibrage selon le statut de la piste
        else
        {
            if( piste->faussePiste == piste->nouvelle ) couleur = GREEN;
            else
                if( piste->EstCalibrante() ) couleur = CYAN;
                else couleur = RED;
        }
        // choix de la couleur en mode calibrage selon le statut de la piste
        // ------------------------------------------------------------------------------------------------
        
        
        // ------------------------------------------------------------------------------------------------
        // affichage du marqueur de piste et de l'id
        // en mode normal on affiche l'id calculé et le numéro, en mode calibrage, le numéro de la piste
        // en mode calibrage, le numero de la piste choisie dans pIdPisteCalibrante est orange
        double voisinagex, voisinagey;
        voisinagex = pf_voisinagex;
        voisinagey = pf_voisinagey;
        if( (2*voisinagex*(piste->nbImagesAbsente) < xi_largeur) || (2*voisinagey*(piste->nbImagesAbsente) < xi_hauteur) )
        {
            DrawRGBSquare(
                obS,
                piste->ibarx() - (D_TAILLE_FENETRE_PISTE+(int)(voisinagex*(piste->nbImagesAbsente))),
                piste->ibary() - (D_TAILLE_FENETRE_PISTE+(int)(voisinagey*(piste->nbImagesAbsente))),
                piste->ibarx() + (D_TAILLE_FENETRE_PISTE+(int)(voisinagex*(piste->nbImagesAbsente))),
                piste->ibary() + (D_TAILLE_FENETRE_PISTE+(int)(voisinagey*(piste->nbImagesAbsente))),
                couleur,
                xi_largeur+8,
                xi_hauteur
                );
        }
        sprintf(xs_msg, "%d", piste->id);
        printT2I(
            obS,
            xs_msg,
            piste->ibarx()-3*D_TAILLE_FENETRE_PISTE,
            piste->ibary()-D_TAILLE_FENETRE_PISTE,
            couleur,
            xi_largeur+8,
            xi_hauteur
            );
        if(piste->vitesseRecu >= 0)
        {
            sprintf(xs_msg, "%d", piste->vitesseRecu);
            printT2I(
            obS,
            xs_msg,
            piste->ibarx()+3*D_TAILLE_FENETRE_PISTE,
            piste->ibary()-D_TAILLE_FENETRE_PISTE,
            couleur,
            xi_largeur+8,
            xi_hauteur
            );
        }
        if( (!LDIRegion::modeCalibrage)&&piste->codeok ) {
            sprintf(xs_msg, "%d", piste->identifiant->majorite());
            printT2I(
                obS,
                xs_msg,
                piste->ibarx()-D_TAILLE_FENETRE_PISTE,
                piste->ibary()-3*D_TAILLE_FENETRE_PISTE,
                (LDIRegion::modeCalibrage && (piste->id==pi_idPisteCalibrante))?ORANGE:couleur,
                xi_largeur+8,
                xi_hauteur
                );
        }
        // affichage du marqueur de piste et de l'id
        // ------------------------------------------------------------------------------------------------
        
        if(pb_affichageDetaille)
        {
            // ------------------------------------------------------------------------------------------------
            // affichage du vecteur vitesse de la piste
            double pEchelleV = pf_echelleAffichageVitesse;
            DrawRGBLine(
                obS,
                piste->ibarx(),
                piste->ibary(),
                (int)(piste->bar.x+pEchelleV*piste->vitesse.x),
                (int)(piste->bar.y+pEchelleV*piste->vitesse.y),
                couleur,
                xi_largeur+8,
                xi_hauteur
                );
            // affichage du vecteur vitesse de la piste
            // ------------------------------------------------------------------------------------------------
            
            /*
            // ------------------------------------------------------------------------------------------------
            // affichage du vecteur acceleration de la piste
            double pEchelleA = pf_echelleAffichageAcceleration;
            DrawRGBLine(
                obS,
                piste->bar.x,
                piste->bar.y,
                (int)(piste->bar.x+pEchelleA*piste->acceleration.x),
                (int)(piste->bar.y+pEchelleA*piste->acceleration.y),
                RED,
                xi_largeur+8,
                xi_hauteur
                );
            // affichage du vecteur acceleration de la piste
            // ------------------------------------------------------------------------------------------------
            */

            // ------------------------------------------------------------------------------------------------
            // affichage en legende du code et du message de la piste
            sprintf(xs_msg, "%d:IP=%d,IA=%d,bits1=%.1f,bits0=%.1f,ibits1=%d,ibits0=%d,code=",
                piste->id,
                piste->nbImagesPresente,
                piste->nbImagesAbsente,
                piste->bits1,
                piste->bits0,
                piste->ibits1,
                piste->ibits0
                );
            int nbBits = piste->code->nbElements();
            for(int b=0; b<nbBits; b++)
            {
                char sbit[5];
                sprintf(sbit, "%d", (*piste->code)[b]);
                strcat(xs_msg, sbit);
            }
            char smsg[32];
            sprintf(smsg, ",msg=%d", piste->identifiant->majorite());
            strcat(xs_msg, smsg);
            // y=+1 pour id=0, +1 car on commence a la deuxieme ligne
            printT2I(
                obS,
                xs_msg,
                T2I_MARGE,
                T2I_INTERLIGNE*(++interligne + 1),
                couleur,
                xi_largeur+8,
                xi_hauteur
                );
            // affichage en legende du code et du message de la piste
            // ------------------------------------------------------------------------------------------------
        }
        
  }
  // affichage des po_pistes
  // ------------------------------------------------------------------------------------------------
  
  
}
