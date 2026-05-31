/******************************************************************************
   fonctions de base pour la réception de données sur un port
   série RS232.
******************************************************************************/
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
/*=============================================================================
  Définition des constantes
=============================================================================*/
#define RX_SIZE         4096    /* taille tampon d'entrée  */
#define TX_SIZE         4096    /* taille tampon de sortie */
#define MAX_WAIT_READ   0//5000    /* temps max d'attente pour lecture (en ms) */
/*=============================================================================
  Variables globales.
=============================================================================*/
/* Handle du port COM ouvert */
HANDLE g_hCOM = NULL;
/* Délais d'attente sur le port COM */
COMMTIMEOUTS g_cto =
{
    MAX_WAIT_READ,  /* ReadIntervalTimeOut          */
    0,              /* ReadTotalTimeOutMultiplier   */
    MAX_WAIT_READ,  /* ReadTotalTimeOutConstant     */
    0,              /* WriteTotalTimeOutMultiplier  */
    0               /* WriteTotalTimeOutConstant    */
};
/* Configuration du port COM */
DCB g_dcb =
{
    sizeof(DCB),        /* DCBlength            */
    57600,              /*Platform  Speed (baud)  :
										telos  115200  
										telosb  115200  
										tmote  115200  
										micaz  57600  
										mica2  57600  
										mica2dot  19200  
										eyes  115200  
										intelmote2  115200   */
    TRUE,               /* fBinary              */
    FALSE,              /* fParity              */
    FALSE,              /* fOutxCtsFlow         */
    FALSE,              /* fOutxDsrFlow         */
    DTR_CONTROL_ENABLE, /* fDtrControl          */
    FALSE,              /* fDsrSensitivity      */
    FALSE,              /* fTXContinueOnXoff    */
    FALSE,              /* fOutX                */
    FALSE,              /* fInX                 */
    FALSE,              /* fErrorChar           */
    FALSE,              /* fNull                */
    RTS_CONTROL_ENABLE, /* fRtsControl          */
    FALSE,              /* fAbortOnError        */
    0,                  /* fDummy2              */
    0,                  /* wReserved            */
    0x100,              /* XonLim               */
    0x100,              /* XoffLim              */
    8,                  /* ByteSize             */
    NOPARITY,           /* Parity               */
    ONESTOPBIT,         /* StopBits             */
    0x11,               /* XonChar              */
    0x13,               /* XoffChar             */
    '?',                /* ErrorChar            */
    0x1A,               /* EofChar              */
    0x10                /* EvtChar              */
};
/*=============================================================================
  Fonctions du module.
=============================================================================*/
BOOL OpenCOM    (int nId);
BOOL CloseCOM   ();
BOOL ReadCOM    (void* buffer, int nBytesToRead, DWORD* pBytesRead);
BOOL WriteCOM   (void* buffer, int nBytesToWrite, DWORD* pBytesWritten);
/******************************************************************************
  main : point d'entrée du programme.
******************************************************************************/
int main()
{
 /* variables locales */
    char buffer[20];                                                                    //voiture ID
    unsigned char packet[15]={0x7E, 0x42, 0xFF, 0xFF, 0x04, 0x7D, 0x5D, 0x04, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x7E};
    int nId, nChoice,clavier;
    unsigned int scr ,id_mote, val;
    bool flag;
    DWORD nBytesWritten, nBytesRead;
    /* demande du numéro du port COM */
    printf("Entrez le numero du port COM : ");
    scanf("%d", &nId);
    /* tentative d'ouverture */
    printf("Ouverture et configuration du port COM%d...\r\n", nId);
    if(!OpenCOM(nId)) return -1;
    printf("...OK\r\n");

    // 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14
    //7E 42 FF FF 04 7D 5D 04 C9 00 06 00 88 1D 7E
    printf("Id mote : ");
    scanf("%d", &id_mote);
    packet[10]=id_mote; 
    packet[11]=id_mote<<8;
    printf("READY !!\n");
    do
    {
  
        clavier = getch();
        flag=false;
        switch(clavier)
        {
            case ' ':
            case 'f':
                val=0;
                printf("Freinage") ;  
            break; 
            case 77: //-->
            case 'd':
                val=1;
                printf("Virage droite") ; 
            break; 
            case 75: //<--
            case 'g':
                val=2;
                printf("Virage gauche") ; 
            break;  
            case 'a':
                val=3;
                printf("Adherance") ; 
            break;  
            case 'o':
                val=4;
                printf("Obstacle") ; 
            break;  
            case 'l':
                val=5;
                printf("Love...") ; 
            break;  
            default:
                flag=true;        
        }
        
        if(flag==false)
          { //ecrit la valeur dans la trame
            packet[8]=val; 
            packet[9]=val<<8; 
            //Ecrit la trame sur le port
            if(WriteCOM(packet, 15, &nBytesWritten))
                printf("         OK !\r\n");
            else
                printf("         Erreur lors de l'envoi.\r\n");
          }     
       
    }while(clavier != 27); //Echap
    /* fermeture du port COM et retour */
    CloseCOM();
    return 0;
}

/******************************************************************************
  OpenCOM : ouverture et configuration du port COM.
  entrée : nId : Id du port COM à ouvrir.
  retour : vrai si l'opération a réussi, faux sinon.
******************************************************************************/
BOOL OpenCOM(int nId)
{
    /* variables locales */
    char szCOM[16];
    /* construction du nom du port, tentative d'ouverture */
    sprintf(szCOM, "COM%d", nId);
    g_hCOM = CreateFile(szCOM, GENERIC_READ|GENERIC_WRITE, 0, NULL,
                        OPEN_EXISTING, FILE_ATTRIBUTE_SYSTEM, NULL);
    if(g_hCOM == INVALID_HANDLE_VALUE)
    {
        printf("Erreur lors de l'ouverture du port COM%d", nId);
        return FALSE;
    }
    /* affectation taille des tampons d'émission et de réception */
    SetupComm(g_hCOM, RX_SIZE, TX_SIZE);
    /* configuration du port COM */
    if(!SetCommTimeouts(g_hCOM, &g_cto) || !SetCommState(g_hCOM, &g_dcb))
    {
        printf("Erreur lors de la configuration du port COM%d", nId);
        CloseHandle(g_hCOM);
        return FALSE;
    }
    /* on vide les tampons d'émission et de réception, mise à 1 DTR */
    PurgeComm(g_hCOM, PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_RXABORT);
    EscapeCommFunction(g_hCOM, SETDTR);
    return TRUE;
}
/******************************************************************************
  CloseCOM : fermeture du port COM.
  retour : vrai si l'opération a réussi, faux sinon.
******************************************************************************/
BOOL CloseCOM()
{
    /* fermeture du port COM */
    CloseHandle(g_hCOM);
    return TRUE;
}
/******************************************************************************
  ReadCOM : lecture de données sur le port COM.
  entrée : buffer       : buffer où mettre les données lues.
           nBytesToRead : nombre max d'octets à lire.
           pBytesRead   : variable qui va recevoir le nombre d'octets lus.
  retour : vrai si l'opération a réussi, faux sinon.
-------------------------------------------------------------------------------
  Remarques : - la constante MAX_WAIT_READ utilisée dans la structure
                COMMTIMEOUTS permet de limiter le temps d'attente si aucun
                caractères n'est présent dans le tampon d'entrée.
              - la fonction peut donc retourner vrai sans avoir lu de données.
******************************************************************************/
BOOL ReadCOM(void* buffer, int nBytesToRead, DWORD* pBytesRead)
{
    return ReadFile(g_hCOM, buffer, nBytesToRead, pBytesRead, NULL);
}
/******************************************************************************
  WriteCOM : envoi de données sur le port COM.
  entrée : buffer        : buffer avec les données à envoyer.
           nBytesToWrite : nombre d'octets à envoyer.
           pBytesWritten : variable qui va recevoir le nombre d'octets
                           envoyés.
  retour : vrai si l'opération a réussi, faux sinon.
******************************************************************************/
BOOL WriteCOM(void* buffer, int nBytesToWrite, DWORD* pBytesWritten)
{
    /* écriture sur le port */
    return WriteFile(g_hCOM, buffer, nBytesToWrite, pBytesWritten, NULL);
}
