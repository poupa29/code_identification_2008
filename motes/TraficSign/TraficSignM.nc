

includes IntMsg;
includes message_trafic;
includes temps;

module TraficSignM
{
  provides interface StdControl;
  uses {
    interface Leds;
    interface LoggerRead;
    interface LoggerWrite;
    interface StdControl as SubControl;
    interface StdControl as SubControlModule;
    interface SendMsg as SendLog;
    interface ReceiveMsg as ReceiveVehicule;
    interface ReceiveMsg as MessageControl;
    interface TempsOutput;
  }
}

implementation
{
  //Gestion du log :
  MsgLog data;
  MsgLog *bufferPtr;
  char* ptr;
  unsigned long nb_enregistrement,nb_lecture;
  
  //gestion de véhicules identique
  int dernier_id[BUFFER_VEHICULE];
  int tab;
  
  //------------FONCTION DE BASE-------------------
  //Fonction de temporisation en ms
  void tempo (int ms)
  {int i;
    for(i=0;i<ms;i++)
       TOSH_uwait(1000); //1ms
  }
  
  //gestion des véhicules
  bool nouveau_vehicule (int id)
  { int i;
    for(i=0;i<BUFFER_VEHICULE;i++)
       if(dernier_id[i]==id)
       return 0;
    return 1;
  }
 
   void remplir_id_vehicule (int id)
  { 
   if(tab>BUFFER_VEHICULE)
      tab=0;
   dernier_id[tab]=id;
   tab++;
  }

  //Gestion du Temps
  Temps reset_time ()
  {Temps localset;
    localset.secondes=0;
	localset.minutes=0;
	localset.heures=0;
	localset.jours=0;
	localset.mois=0;
	localset.annees=0;
   return localset;
  }
  
  // A finir ...
  void send_time()
  {	
    TOS_Msg msg;
    Temps local;
    Temps *message = (Temps *)msg.data;
  
    local = call TempsOutput.get();
    
    local.secondes=message->secondes;
	local.minutes=message->minutes;
	local.heures=message->heures;
	local.jours=message->jours;
	local.mois=message->mois;
	local.annees=message->annees;
	
	//Envoi du message sur la radio
    call SendLog.send(TOS_BCAST_ADDR, sizeof(data), &msg);
   }
   
//-------------------------------------------------------------------------- 

/*************************************************************************/
  command result_t StdControl.init() {
	
	nb_enregistrement=0;
	nb_lecture=0;
	tab=0;
	bufferPtr = &(data);
	
    call Leds.init();
    call SubControl.init();
    call SubControlModule.init();
    return SUCCESS;
  }


  command result_t StdControl.start() {

    call SubControl.start();
    call SubControlModule.start();
    return SUCCESS;
  }

 command result_t StdControl.stop()
  {
    call SubControl.stop();
    call SubControlModule.stop();
    return SUCCESS;
  }
 
 //Reception et enregistrement des messages Véhicules
  event TOS_MsgPtr ReceiveVehicule.receive(TOS_MsgPtr m) { 
    MsgVehicule *message;
    Temps local;
    int id;
    
    message = (MsgVehicule *)m->data;
    id = message->id_vehicule; //extraction de l'id 
    
    
    //On enregitre uniquement les nouveaux vehicules :
    if(nouveau_vehicule(id))
	   {
	    data.vehicule.id_vehicule = id;
	    data.vehicule.average_speed = message->average_speed; //extraction de la vitesse
	    data.vehicule.type_vehicule = message->type_vehicule; //extraction de la vitesse
	    
	    local = call TempsOutput.get(); //recupere la date
	    data.time_min = local.minutes;
	    data.time_hour = local.heures;
	    data.time_day = local.jours;
	
		data.id_source = TOS_LOCAL_ADDRESS;
		
	    remplir_id_vehicule(id);
	    
		//Ecriture dans le log :
	    ptr = (char*)bufferPtr;
	    call LoggerWrite.append(ptr);
       }
  
  return m;
 }
 
  //Reception et gestion des messages de commande
  event TOS_MsgPtr MessageControl.receive(TOS_MsgPtr m) { 
  IntMsg *message; //Le message est de type entier 
  int code_msg;
  
  message = (IntMsg *)m->data; 
  code_msg = message->val; //extraction du code de commande
  //if(message->src==id_vehicule)
  switch(code_msg)
  {
  	case LIRE_LOG:
  	    call SubControlModule.stop(); //arret du module feu (flag=0) pendant l'envoi du log!
  		call LoggerRead.readNext(ptr); //lecture log
  		break;
  		
  	case RESET_LOG:
  		 nb_lecture = 0;
    	 nb_enregistrement=0;
    	 call LoggerRead.resetPointer();
    	 call LoggerWrite.resetPointer();
    	 break;
   
    case GET_TIME:
  		 send_time();
    	 break;	
    	 
    case RESET_TIME: //A faire !!!
    	 call SubControlModule.start();
  		 //call TempsOutput.set(reset_time());
    	 break;	
    	 
  	case SET_TIME:   //A faire !!!
  		//call TempsOutput.set(Temps localset);
  		 call SubControlModule.stop();
  		break;
  
  	default:
  		break;
  }
 
  return m;
 }
 
 //A la fin de chaque lecture : emmision de la trame
  event result_t LoggerRead.readDone(uint8_t * datalog, result_t status) {
    TOS_Msg msg;
    int i;
    
    nb_lecture++;
    
    //Remplissage du message radio
    for(i=0;i<sizeof(data);i++)
    	msg.data[i] =  datalog[i];
    	
	call Leds.yellowToggle();
	
	//Envoi du message radio
    call SendLog.send(TOS_BCAST_ADDR, sizeof(data), &msg);
    
    return SUCCESS;
  }


  event result_t SendLog.sendDone(TOS_MsgPtr pmsg, result_t status) {
    call Leds.greenToggle(); 
    
    
    tempo(20); //tempo sinon bug
    
    if(nb_lecture<nb_enregistrement)
    	call LoggerRead.readNext(ptr); //lecture de la ligne du log suivante
    	
    else
    	//Enregistrement terminé on reset la lecture
    	{nb_lecture = 0;
    	 call LoggerRead.resetPointer();
    	 call SubControlModule.start(); //on relance le module feu ou panneau
    	}
 
    return SUCCESS;
  }

  // event handler: logger write done event
  event result_t LoggerWrite.writeDone(result_t status) {
    
    nb_enregistrement++;
    //a enregistrer dans le log !!
    
    call Leds.redToggle(); 
    return SUCCESS;
  }
  

}
