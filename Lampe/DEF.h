// def.h

#include <AT892051.H>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "lampe.h"

      // VARIABLES GLOBALES
char xs_sequence[TAILLE_SEQUENCE_MAX];
unsigned char xu_tailleSequence = 0;

char xs_motUtil[TAILLE_MOT_UTIL];
char xc_tamponP3;

char xs_startCode[10];
char xc_tailleStartCode;

char xc_repetition;
int xi_frequence;
int mem_TH1[3], mem_TL1[3];

bit xb_parite;
char xc_bitParite;

bit xb_calibrage;

// definition des starts codes
// ATTENTION : ne pas oublier d'adapter la taille du tableau START en fonction du start code entre
//                                 P1.2  
char code START0[1]={-1};          // 0
char code START1[5]={1,1,0,1,-1};  // 1