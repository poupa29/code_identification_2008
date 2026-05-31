#include "lampe.h"

/**************************************************************************************************
								INITIALISATION DU MICROCONTROLLEUR

//CLKREG = 8FH  > 0x01
//X2 = 4
  
**************************************************************************************************/
void init_microcontroleur(void)
{   
    CLKREG = 0x01;  // Mode X2 : 1 cycle machine = 6 coup d'horloges
    P1 = 0X03 ;  // P1 en entrée
    P3 = 0xFF;   // P3.7 en entrée, le reste du port en sortie
    EA = 0; // aucune interruptions
}


/**************************************************************************************************
								INITIALISATION DES TIMER

TIMER 0 : 5 kHz : PWM d'allumage des diodes ; Periode = 200µs   	Mode 2 : 8bits à rechargement 
TIMER 1 : Regle la periode d'émission des bits avec frequence(=FREQUENCE) (~10Hz)	Mode 1 : 16bits
**************************************************************************************************/
void init_timer(void)
{
	long periode_5k, periode_mot; 
    int init_5k, init;
    
	TMOD = 0x12 ;   // Timer 0 mode 2, Timer 1 mode 1
       
	//////////////////////
	// TIMER 0
	//
	periode_5k = (1000000 / 5000);
	init_5k = 65536 - (periode_5k / 2);
	TH0 = init_5k % 256;
	TR0 = 1; // lancement du timer
	
	//////////////////////
	// TIMER 1
	//
    xc_repetition = 1;
    periode_mot = (1000000 / xi_frequence);    // en µs
	while(1)
	{	
		if ( (periode_mot / xc_repetition) > 65535)
			xc_repetition++ ;
		else
			break;
	}
	
	init = 65535 - (periode_mot / xc_repetition);
    mem_TH1[0] = init/256;
	mem_TL1[0] = init%256;

    mem_TH1[1] = init-init*RAPPORT;
    mem_TH1[1] = mem_TH1[1] / 256;
	mem_TL1[1] = init-init*RAPPORT;
    mem_TL1[1] = mem_TL1[1] % 256;

    mem_TH1[2] = init+init*RAPPORT;
	mem_TH1[2] = mem_TH1[2] / 256;
    mem_TL1[2] = init+init*RAPPORT;
    mem_TL1[2] = mem_TL1[2] % 256;
}

/**************************************************************************************************
								RESET DU TIMER 1

Remet le timer 1 dans sont état initial et le redemarre  
type : ° 1 : reset normal : le timer est reset au valeur necessaire pour "frequence" 
       ° 2 : reset court : reset pour avoir une periode un peu plus courte
       ° 3 : reset long : ----------------------------------------- longue
**************************************************************************************************/
void reset_timer1(char i) 
{
    char indice = PERIODE_NORMALE;
    char i_precedent = i-1;
    char i_suivant = i+1;

	if( (!xb_calibrage)&&(xi_frequence>=FREQUENCE_SEUIL_RAPPORT) )
	{
	    if (i == 0)
	        i_precedent = xu_tailleSequence-1;
	    else if(i == xu_tailleSequence)
	        i_suivant = 0;
	
	    if((xs_sequence[i] == 1) && (xs_sequence[i_suivant] == 0))  // reset court
	        indice = PERIODE_COURTE;      
	    else if ((xs_sequence[i] == 0) && (xs_sequence[i_precedent] == 1))  // reset long
	        indice = PERIODE_LONGUE;
	}

	TR1 = 0;  // stop comptage
    TF1 = 0;  // overflow à 0 
	TH1 = mem_TH1[indice];
	TL1 = mem_TL1[indice];
	TR1 = 1;  // demarrage comptage
}

/**************************************************************************************************
								TEMPO

Permet de stopper l'execution du programme pendant une durée voulue  
**************************************************************************************************/
void tempo(int duree)
{
    int i;
    
    while(TF0 == 0);
    TF0 = 0;

    for(i=0; i<duree; i++)
    {
        while(TF0 == 0);  // Attente de 200µs
        TF0 = 0;
    } 
}

/**************************************************************************************************
								Gestion du 4017

reset4017 : permet de mettre le 4017 à son etat initiale (sur le 1er bit)  
inc4017 : permet d'incrementer le 4017 pour lire successivement tous les bits
**************************************************************************************************/
void reset4017(void)
{
    P1_2 = 0;
    tempo(1);
    P1_2 = 1;  
    tempo(1);
    P1_2 = 0;
}

void inc4017(void)
{
    P1_3 = 0;
    tempo(1);
    P1_3 = 1;
    tempo(1);
    P1_3 = 0;     
}