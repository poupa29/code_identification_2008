#include "IR.h"


int typeIdentifiant(unsigned long _id)
{
    switch (_id)
    {
        case ID_PANNEAU_DEBUT_30:
            return TYPE_ID_INFRA;
        case ID_PANNEAU_FIN_30:
            return TYPE_ID_INFRA;
        case ID_PANNEAU_VITESSE_50:
            return TYPE_ID_INFRA;
        case ID_PANNEAU_FIN_VITESSE_50:
            return TYPE_ID_INFRA;
        case ID_PANNEAU_VIRAGE_GAUCHE:
            return TYPE_ID_INFRA;
        case ID_PANNEAU_CHICANE:
            return TYPE_ID_INFRA;
        case ID_PANNEAU_FEU:
            return TYPE_ID_INFRA;
        case ID_PANNEAU_VITESSE_90:
            return TYPE_ID_INFRA;
        case ID_STOP:
            return TYPE_ID_INFRA;
        case ID_DANGER:
            return TYPE_ID_INFRA;
        case ID_FEU:
            return TYPE_ID_INFRA;
        default:
            return TYPE_ID_VEHICULE;
    }
}