

enum 
{
  BUFFERMAX = 50, //taille max du tableau
  S_MSG = 5, //Type de message 
};

typedef struct Special_Msg {
    uint16_t datal[BUFFERMAX];  //les données
    uint16_t moteid;            //ID du mote
  
} Special_Msg;
