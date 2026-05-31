

enum {
//Type de messages radio:
MSG_LOG = 7,
MSG_VEHICULE = 8,
MSG_CONTROL = 6,

//Nombres de voitures identique a stoquer en memoire
BUFFER_VEHICULE = 10,

//les messages de controle:
LIRE_LOG   = 1,
RESET_LOG  = 2,
GET_TIME   = 3,
RESET_TIME = 4,
SET_TIME   = 5,
};

//Message vehicule :
typedef struct MsgVehicule {
    uint16_t id_vehicule;
    uint8_t average_speed;  
    uint8_t type_vehicule; 
    
} MsgVehicule;

//Message log:
typedef struct MsgLog {
    MsgVehicule vehicule;
    uint8_t time_min;
    uint8_t time_hour;
    uint8_t time_day;
    //Temps time;
    uint16_t id_source;
    
} MsgLog;
