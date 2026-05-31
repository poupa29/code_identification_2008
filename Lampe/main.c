            /***************************************************************************\
            *                                                                           *
            *                                   LIVIC                                   *
            *                                                                           *
            *                     Programme pour montage LAMPES IR                      *
            *                                                                           *
            *                                                                           *
            *  Juin 2006                                                 Amaury DOUENCE *
            \***************************************************************************/



#include "lampe.h"
#include "DEF.h"
/*
P1.0 -> Sortie
P1.1 -> Erreur
P1.2 -> Reset 4017
P1.3 -> Inc 4017
P1.4
P1.5 <- MOSI (ISP)
P1.6 -> MISO (ISP)
P1.7 <- SCK (ISP)

P3.0 <- StartCode0
P3.1 <- StartCode1
P3.2 <- Parite (INT0)
P3.3 <- Horloge0 (<~ INT1)
P3.4 <- Horloge1
P3.5 <- Horloge2 (~> PWM out)
P3.7 <- MotUtil
*/

void main(void)
{
    char z;

    init_microcontroleur();
   
    while(1)
    {
        erreur = 1;

        aquisition_proprietes();
        init_timer();
        creationSequence();
        
        erreur = 0;  
        erreur = verificationValiditeMot();
        
        P3 = 0xFF;
        while(1)
        {
            z = verificationProprietes();

            if(z != 1)
                {erreur = 1;
                 break;
                }   
            
            z = emissionSequence();
           /* if(z != 1)
                {erreur = 1;
                 //break;
                }*/
        }
    }
   
}
