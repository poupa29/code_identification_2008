
#include "../../general/utils/livicutils.h"

#define TAILLE_MSG 17
#define TAILLE_DATA_MOTE 15				 //Taille max des données du mote 
#define MSG_TAILLE 4096                   // taille des chaines de messages en octets

#define MAX_CIBLES_IR					256
#ifndef PI
#define PI								3.14159265358979
#endif //PI

#define ID_PANNEAU_DEBUT_30				252
#define ID_PANNEAU_FIN_30				251
#define ID_PANNEAU_VITESSE_90			12
#define ID_PANNEAU_VITESSE_50			1
#define ID_PANNEAU_FIN_VITESSE_50		9
#define ID_PANNEAU_VIRAGE_GAUCHE	    6
#define ID_PANNEAU_CHICANE			    255
#define ID_PANNEAU_FEU					254
#define ID_FEU							20
#define ID_STOP                         253
#define ID_DANGER				        7
#define ID_INFOS						6

#define MSG_TY_FREINAGE                 0
#define MSG_TY_VIRAGE_DROITE            1
#define MSG_TY_VIRAGE_GAUCHE            2
#define MSG_TY_ADHERENCE                3
#define MSG_TY_OBSTACLE                 4
#define MSG_TY_LOVE                     5
#define MSG_TY_FEU_ROUGE                6
#define MSG_TY_FEU_ORANGE               7
#define MSG_TY_FEU_VERT                 8
#define MSG_TY_INFO						9

#define TYPE_ID_INFRA                   1
#define TYPE_ID_VEHICULE                2

#define ID_VOITURE_MEGANE               8
#define ID_VOITURE_ZX                   10
#define ID_VOITURE_3                    4
#define ID_PICASSO                      2

//Constante des messages TinyOS :
#define AM_INTMSG	                	4
#define S_MSG		                	5
#define MSG_CONTROL             		6
#define MSG_LOG		                	7
#define MSG_VEHICULE                	8
#define MSG_STRING	                	9


int typeIdentifiant(unsigned long _id);

