
includes IntMsg;
module FeuM {
  uses {
	interface Timer;
    interface Leds;
	interface IntOutput;
	interface StdControl as SubControl;
	interface ReceiveMsg as MessageControl;
  }
  provides {
    interface StdControl;
  }

}
implementation {

	#define PERIODE_RAFALE_MS 100
	#define RAPPORT_DUREE_SUR_RAFALE 10 

	#define DUREE_ROUGE 8
	#define DUREE_ORANGE 4
	#define DUREE_VERT 8
	
	#define CODE_MAPS_ROUGE 1
	#define CODE_MAPS_ORANGE 2 
	#define CODE_MAPS_VERT 3
	
	#define ADRESSE_LED_ROUGE 1
	#define ADRESSE_LED_ORANGE 4
	#define ADRESSE_LED_VERT 2
	
	uint8_t duree[3] = {DUREE_ROUGE,DUREE_VERT,DUREE_ORANGE}; //Rouge, Vert, Orange
	uint8_t  etat, count;
	


  command result_t StdControl.init()
  {
	count = 1;
	call SubControl.init();
	call Leds.init(); 
    return SUCCESS;
  }



  command result_t StdControl.start()
  {
    call SubControl.start();
    return call Timer.start(TIMER_REPEAT, PERIODE_RAFALE_MS);
  }


  command result_t StdControl.stop()
  {
    return call Timer.stop();
  }


  event result_t Timer.fired()
  {
	count++;
	if (count > (duree[etat])*RAPPORT_DUREE_SUR_RAFALE)
	{
		etat= (etat +1) % 3;
		count = 1;
	}
	
	switch(etat)
	{
	case 0:
		call Leds.set(ADRESSE_LED_ROUGE);
		call IntOutput.output(((PERIODE_RAFALE_MS*(DUREE_ROUGE+1)*RAPPORT_DUREE_SUR_RAFALE)-(PERIODE_RAFALE_MS*count))+CODE_MAPS_ROUGE);
		break;
	case 1:
		call Leds.set(ADRESSE_LED_VERT);
		call IntOutput.output(((PERIODE_RAFALE_MS*(DUREE_VERT+1)*RAPPORT_DUREE_SUR_RAFALE)-(PERIODE_RAFALE_MS*count))+CODE_MAPS_VERT);
		break;
	case 2:
		call Leds.set(ADRESSE_LED_ORANGE);
		call IntOutput.output(((PERIODE_RAFALE_MS*(DUREE_ORANGE+1)*RAPPORT_DUREE_SUR_RAFALE)-(PERIODE_RAFALE_MS*count))+CODE_MAPS_ORANGE);
		break;
	default: 
		call Leds.set(7);
		break;
	}
	
	
	return SUCCESS;
  }
  
  //Reception et gestion des messages de commande
  event TOS_MsgPtr MessageControl.receive(TOS_MsgPtr m) { 
  IntMsg *message; //Le message est de type entier 
  int code_msg;
  
  message = (IntMsg *)m->data; 
  code_msg = message->val; //extraction du code de commande
  //Fixe le feu au vert :
  etat=1;
  count = 1;
  
/*
  switch(code_msg)
  {
  	case 1:
  	   code...
  		break;
  		
  	default:
  		break;
  }
 */
 
  return m;
 }
  
  event result_t IntOutput.outputComplete(result_t success) 
  {
    if(success == 0) count --;
    return SUCCESS;
  }
  
}

