////////////////////////////////
// RTMaps SDK Component header
////////////////////////////////

#ifndef _Maps_LivicCiblesAleatoires_H
#define _Maps_LivicCiblesAleatoires_H

#define ETEINT false
#define ALLUME true
#define TAILLESEQUENCEADD 6
#define TAILLESEQUENCEADDVITESSE 6

//#define FREQUENCE_CAMERA 514
//#define FREQUENCE_IMAGE 210
//#define LONGUEUR_TRAME 16
//#define NB_SPOTS 4
//#define WIDTH 240
//#define HEIGHT 160

// Includes maps sdk library header
#include "stdlib.h"
#include "time.h"
#include "maps.hpp"

//FILE* fic;


class Spot
{
public : 

    bool etat;
    MAPSTimestamp debut;
    MAPSTimestamp dureeEtatCourant;
    int pos_x, pos_y;
    int longueurTrame;
    int frequenceLampe;
    int sequenceSignalAdd[TAILLESEQUENCEADD];
    int sequenceSignalAddVitesse[TAILLESEQUENCEADDVITESSE];
    int positionSequence; 
    bool calibrage;
    


    Spot(int x, int y)
    {
        pos_x = x;
        pos_y = y;
        etat = ETEINT;
        //ChangeEtat();
        debut = MAPS::CurrentTime();
        dureeEtatCourant = 0;
        positionSequence = 0;
       /* sequenceSignalAdd[0] = 2;
        sequenceSignalAdd[1] = 1;
        sequenceSignalAdd[2] = 1;
        sequenceSignalAdd[3] = 8;
        sequenceSignalAdd[4] = 1;
        sequenceSignalAdd[5] = 3;*/
        sequenceSignalAdd[0] = 3;
        sequenceSignalAdd[1] = 3;
        sequenceSignalAdd[2] = 3;
        sequenceSignalAdd[3] = 3;
        sequenceSignalAdd[4] = 2;
        sequenceSignalAdd[5] = 2;
        //sequenceSignalAdd = {2,1,1,8,1,3};

        sequenceSignalAddVitesse[0] = 2;
        sequenceSignalAddVitesse[1] = 1;
        sequenceSignalAddVitesse[2] = 4;
        sequenceSignalAddVitesse[3] = 2;
        sequenceSignalAddVitesse[4] = 2;
        sequenceSignalAddVitesse[5] = 2;

    }

    ~Spot()
    {
    }

    void Spot::SetParam(int xi_longueurTrame, int xi_frequenceLampe, bool xb_calibrage)
    {
        longueurTrame = xi_longueurTrame;
        frequenceLampe = xi_frequenceLampe;
        calibrage = xb_calibrage;
    }

    void Spot::ChangeEtat()
    {
        MAPSTimestamp time = MAPS::CurrentTime();
        //srand( (unsigned)time( NULL ) );

        if(dureeEtatCourant <= (time-debut))
        {
            debut += dureeEtatCourant;
            double random = ((double)rand())/(double)RAND_MAX;
            dureeEtatCourant = (int)((longueurTrame-1)*random+1);
            dureeEtatCourant *= 1000000;
            dureeEtatCourant /= frequenceLampe;
            //debut = time - (dureeEtatCourant - (time-debut));
            etat = !etat;
            /*if(etat == ALLUME)
            {
                etat = ETEINT;
                debut = time;
            }*/
        }
        
    }

    void Spot::MAJSignal(bool xb_AddVitesse)
    {
        MAPSTimestamp time = MAPS::CurrentTime();
       // int sequenceSignalAdd[TAILLESEQUENCEADD] = {2, 1, 1, 8, 1, 3};

        if(dureeEtatCourant <= (time-debut))
        {
            if (calibrage)
            {
                //fprintf(fic,"temps lampe : %f \n",1./(float)dureeEtatCourant);
                debut += dureeEtatCourant;
                dureeEtatCourant = 5;
                dureeEtatCourant *= 1000000;
                dureeEtatCourant /= frequenceLampe;
                //debut = time - (dureeEtatCourant - (time-debut));
                etat = !etat;
                
            }
            else
            {
                debut += dureeEtatCourant;
                if(!xb_AddVitesse)
                {
                    dureeEtatCourant = sequenceSignalAdd[positionSequence];
                    dureeEtatCourant *= 1000000;
                    dureeEtatCourant /= frequenceLampe;
                    //debut = time - (dureeEtatCourant - (time-debut));
                    etat = !etat;
                    if (positionSequence < TAILLESEQUENCEADD-1)
                        positionSequence++;
                    else
                        positionSequence = 0;
                }
                else 
                {
                    dureeEtatCourant = sequenceSignalAddVitesse[positionSequence];
                    dureeEtatCourant *= 1000000;
                    dureeEtatCourant /= frequenceLampe;
                    //debut = time - (dureeEtatCourant - (time-debut));
                    etat = !etat;
                    if (positionSequence < TAILLESEQUENCEADDVITESSE-1)
                        positionSequence++;
                    else
                        positionSequence = 0;
                }
            /*if(etat == ALLUME)
            {
                etat = ETEINT;
                debut = time;
            }*/
            }
        }

    }

    int Spot::GetX()
    {
        return pos_x;
    }

    int Spot::GetY()
    {
        return pos_y;
    }


    bool Spot::SpotEtat()
    {
        return etat;
        /*if (etat == ALLUME)
            return 1;
        else
            return 0;*/
    }

};

// Declares a new MAPSComponent child class
class MAPSLivicCiblesAleatoires : public MAPSComponent 
{
	// Use standard header definition macro
	MAPS_COMPONENT_STANDARD_HEADER_CODE(MAPSLivicCiblesAleatoires)

public : 



private :
	MAPSInteger                 xi_frequenceCamera;
    MAPSInteger                 xi_frequenceLampe;
    MAPSInteger                 xi_longueurTrame;
    MAPSInteger                 xi_nbSpots;
    MAPSInteger                 xi_nbSpotsMax;
    MAPSInteger                 xi_width;
    MAPSInteger                 xi_height;
    bool                        xb_trueSignalAdd;
    bool                        xb_AddVitesse;
    bool                        xb_calibrage;

    MAPSTimestamp               xt_timestampDebut;
    MAPSTimestamp               xt_timestampFin;
    bool                        xb_firstTime;
//    Spot*                       po_spot;
    Spot**                      po_spot;
    //unsigned char*              pu_imgSortie;
    unsigned char*              pu_imgBytes;

};


#endif
