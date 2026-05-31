// lampe.h

#include <AT892051.H>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
 
#define TAILLE_MOT_UTIL 10
#define TAILLE_SEQUENCE_MAX 6+TAILLE_MOT_UTIL+2
#define erreur P1_1

// definition des differentes frequences d'horloges en Hz
//                    P1.3 .4 .5     
#define FREQUENCE0 12    // 000
#define FREQUENCE1 150   // 001
#define FREQUENCE2 210   // 010
#define FREQUENCE3 220   // 011
#define FREQUENCE4 230   // 100
#define FREQUENCE5 240   // 101
#define FREQUENCE6 250   // 110
#define FREQUENCE7 260   // 111
 
// definition des starts codes
//                               P1.1 P1.2  
extern char code START0[];
extern char code START1[];
extern char code START2[];
extern char code START3[];

// Rapport utilise pour le changement de rapport cyclique
//
#define FREQUENCE_SEUIL_RAPPORT 10000 // frequence a partir de laquelle on applique le rapport cyclique suivant: (la valeur a ete fixe pour qu il ne soit jamais applique)
#define RAPPORT 1/4
#define PERIODE_NORMALE 0   // duree normale d'un bit
#define PERIODE_COURTE 1    // duree raccourcie du dernier 1
#define PERIODE_LONGUE 2    // duree rallongee du premier 0

		// PROTOPYPES
// init.c
extern void init_microcontroleur(void);
extern void init_timer(void);
extern void reset_timer1(char);
extern void tempo(int duree);
extern void reset4017(void);
extern void inc4017(void);

// traitement.c
extern void aquisition_proprietes(void);
extern void lectureMotUtile(void);
extern void creationSequence(void);
extern void calculBitParite(void);
extern bit emissionSequence(void);
extern bit verificationProprietes(void);
extern bit verificationValiditeMot(void);
//extern bit verificationMotUtile(char);

      // VARIABLES GLOBALES
extern char xs_sequence[TAILLE_SEQUENCE_MAX];
extern unsigned char xu_tailleSequence;

extern char xs_motUtil[TAILLE_MOT_UTIL];
extern char xc_tamponP3;

extern char xs_startCode[10];
extern char xc_tailleStartCode;

extern char xc_repetition;
extern int xi_frequence;
extern int mem_TH1[3], mem_TL1[3];

extern bit xb_parite;
extern char xc_bitParite;

extern bit xb_calibrage;