#include "lampe.h"

/**************************************************************************************************
								ACQUISITION DES PROPRIETES
  
**************************************************************************************************/ 
void aquisition_proprietes(void)
{
    char z,taille ;
	
    ////////////////////////////////
    // Lecture des proprietes
    //
	P3 = 0xFF;
    xc_tamponP3 = P3;
    
    xc_tamponP3 = ~xc_tamponP3; // switchs en logique negative
	
    ////////////////////////////////
    // Bit de parite
	//
    if ((xc_tamponP3 & 0x04)== 0x04)     // 0000 0X00
        xb_parite = 1;
    else 
        xb_parite = 0; // 1 activée, 0 desactivée
    
	////////////////////////////////
    // start code
	//
    z = (xc_tamponP3 & 0x02);            // 0000 00X0
    taille=0;
           
    switch (z)   
    {
        case 0x00 :
            while(START0[taille]!=-1)
            {
                xs_startCode[taille] = START0[taille];
                taille++; 
            }break;
        case 0x02 : 
            while(START1[taille]!=-1)
            {
                xs_startCode[taille] = START1[taille];
                taille++ ;
            }break;
    }   
    xc_tailleStartCode = taille;

	////////////////////////////////
    // mode calibrage
	//
    z = (xc_tamponP3 & 0x01);            // 0000 000X
    xb_calibrage = (z==0?0:1);       

	////////////////////////////////
	// Horloge
	//
	z = (xc_tamponP3 & 0x38);   // 00XX X000

	switch(z) 
	{
		case 0x00 : xi_frequence = FREQUENCE0; break;
		case 0x20 : xi_frequence = FREQUENCE1; break;
		case 0x10 : xi_frequence = FREQUENCE2; break;
		case 0x30 : xi_frequence = FREQUENCE3; break;
		case 0x08 : xi_frequence = FREQUENCE4; break;
		case 0x28 : xi_frequence = FREQUENCE5; break;
		case 0x18 : xi_frequence = FREQUENCE6; break;
		case 0x38 : xi_frequence = FREQUENCE7; break;
	}
    xc_tamponP3 = ~xc_tamponP3;  //tampon P3 reprend la valeur de P3 en logique negative
}

/**************************************************************************************************
								Lecture du mot util
  
**************************************************************************************************/
void lectureMotUtile(void)
{
    char i;

    P3_7 = 1;

	reset4017();

    for(i=TAILLE_MOT_UTIL-1; i>=0; i--) 
    {
         xs_motUtil[i] = P3_7;
	     inc4017();
    } 
    
    P3_7 = 0;  // stop lecture sur P3.7 "pulled low"
}

/**************************************************************************************************
								Lecture du mot util

Creer la sequence qui va etre emise en fonctoin de tous les parametres precedement acquis  
**************************************************************************************************/
void creationSequence(void)
{
    char i,j;
    xu_tailleSequence = 0;

    if( xb_calibrage )
	{
		xu_tailleSequence = TAILLE_MOT_UTIL;
		j=xu_tailleSequence/2;
		for(i=0; i<j; i++)
		{
			xs_sequence[i] = 1;
			xs_sequence[i+j] = 0;
		}
		xc_tailleStartCode = 0;
		xb_parite = 0;
	}
	else
	{
	
	// Start code
	    for(i=0;i<xc_tailleStartCode;i++)
	    {
	        xs_sequence[i] = xs_startCode[i];
	    }
	    xu_tailleSequence += xc_tailleStartCode;    
	
	// Mot utile
	    lectureMotUtile();                                
	
	    for(i=0;i<TAILLE_MOT_UTIL;i++)
	    {
	        j = xc_tailleStartCode + i;
	        xs_sequence[j] = xs_motUtil[i];   
	    }
	    xu_tailleSequence += TAILLE_MOT_UTIL;
	
	// Bit de Parite
	    if(xb_parite)
	    {
	        calculBitParite();
	        xs_sequence[xu_tailleSequence] = 0;
	        xs_sequence[xu_tailleSequence+1] = xc_bitParite;
	        xu_tailleSequence += 2;
	    }
	}      
}

/**************************************************************************************************
								CALCUL DU BIT DE PARITE

Retour : valeur du bit de parité : 1 ou 0 
**************************************************************************************************/
void calculBitParite(void)
{
    int somme = 0;
    char i;
    
    // Calcul de la parite du start code ...
    for(i=0; i<xc_tailleStartCode; i++)
    {
        somme += xs_startCode[i];
    }
                         
    // ... puis du mot util
    for(i=0; i<TAILLE_MOT_UTIL; i++)
    {
        somme += xs_motUtil[i];
    }

    // Calcul de la parite
    if( somme % 2 == 0 )
        xc_bitParite = 0;
    else
        xc_bitParite = 1;
}

/**************************************************************************************************
								EMISSION DE LA SEQUENCE

Emet la sequence.
Vu le branchement electrique du circuit (montage inverseur) 
        => pour emettre un '1', il faut que le programme emette '0', et inversement
La fonction envoi le niveau haut avec la frequence de 5KHz  
**************************************************************************************************/
bit emissionSequence(void)
{
    char i, k;
    bit z=1;

    for(i=0; i<xu_tailleSequence; i++)
    {	
        //z = verificationMotUtile(i);

        for( k=0; k<xc_repetition; k++)
		{
            reset_timer1(i);

			if( xs_sequence[i] != 0)
			{
				while(TF1 == 0) 
				{
				    TF0 = 0;
					P1_0 = 0;
					while( TF0== 0);
                
					TF0 = 0;
					P1_0 = 1;
					while( TF0 == 0);
				}
			}
			else // sequence == 0
			{
				P1_0 = 1;
				while(TF1 == 0);
			}
		}
    }
    return z;
}

/**************************************************************************************************
								Verification proprietes

Verifie si les proprietes (barrette de 6 switchs) ont changés depuis l'acquisition initiale  
**************************************************************************************************/     
bit verificationProprietes(void)
{
    ////////////////////
    // Verification des proprietes
    //
    P3 = 0xff;
    if((xc_tamponP3&0x3F) != (P3&0x3F)) //0011 1111 lecture des prop uniquement
        return 0;
    
    return 1;
}

/**************************************************************************************************
								Verification validite mot

Verifie si le start code n'est pas repete 2 fois dans la sequence  
**************************************************************************************************/ 
bit verificationValiditeMot(void)
{
    char i,j ;
    bit startCodeRepete;

    if(xc_tailleStartCode == 0) // pas de verification si il n'y a pas de start code
        return 0;

    for(i=0; i<= xu_tailleSequence - 1; i++)
              {
                    startCodeRepete = 1;
                    for(j=0; j<xc_tailleStartCode; j++)
                    {
                        startCodeRepete =
                            startCodeRepete &&
                            (xs_sequence[1+i+j] == xs_startCode[j]);
                    }
                    if(startCodeRepete != 0)
                        break;
              }
    return startCodeRepete;
}


/**************************************************************************************************
								Verification mot utile

Verifie si le mot utile n'a pas changer depuis la derniere acquisition

            !!!!!  PROBLEME D'EXECUTION DE CETTE FONCTION AU 28/06/06  !!!!!
**************************************************************************************************/
/*bit verificationMotUtile(char i)           
{
    bit z=1;

    if((i>=xc_tailleStartCode) && (i<(xc_tailleStartCode + TAILLE_MOT_UTIL)))  // juste lors de l'emission du mot utile
    {
        if(xs_sequence[i] != P3_7)
            z = 0;
        inc4017();   
    }
    else
    {
        reset4017();
    }
    return z; 
}
*/