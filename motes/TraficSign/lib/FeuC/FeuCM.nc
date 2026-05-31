
module FeuCM {
  uses {
    interface StdControl as SubControl;
	interface Timer;
    interface Leds;
	interface IntOutput;
  }
  provides {
    interface StdControl;
    interface StdControl as ControlModule;
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
	bool  flag;
	


  command result_t StdControl.init()
  {		
		count = 1;
		call Leds.init(); 
		//call SubControl.init();
		call SubControl.start();
    return SUCCESS;
  }

  command result_t StdControl.start()
  { flag=1;
    return call Timer.start(TIMER_REPEAT, PERIODE_RAFALE_MS);
  }

  command result_t StdControl.stop()
  { call SubControl.stop();
    return call Timer.stop();
  }
  
  //Controle externe du module
  command result_t ControlModule.init()
  {return SUCCESS;}
  
  command result_t ControlModule.start()
  {flag=1;
  return SUCCESS;}
  
  command result_t ControlModule.stop()
  {flag=0; //mise a 0 du flag d'envoi de message
   return SUCCESS;}
   

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
			 if(flag) call IntOutput.output(((PERIODE_RAFALE_MS*(DUREE_ROUGE+1)*RAPPORT_DUREE_SUR_RAFALE)-(PERIODE_RAFALE_MS*count))+CODE_MAPS_ROUGE);
			break;
		case 1:
			call Leds.set(ADRESSE_LED_VERT);
			 if(flag) call IntOutput.output(((PERIODE_RAFALE_MS*(DUREE_VERT+1)*RAPPORT_DUREE_SUR_RAFALE)-(PERIODE_RAFALE_MS*count))+CODE_MAPS_VERT);
			break;
		case 2:
			call Leds.set(ADRESSE_LED_ORANGE);
			 if(flag) call IntOutput.output(((PERIODE_RAFALE_MS*(DUREE_ORANGE+1)*RAPPORT_DUREE_SUR_RAFALE)-(PERIODE_RAFALE_MS*count))+CODE_MAPS_ORANGE);
			break;
		default: 
			call Leds.set(7);
			break;
		}
	
	
	return SUCCESS;
  }
  
  event result_t IntOutput.outputComplete(result_t success) 
  {
    if(success == 0) count --;
    return SUCCESS;
  }
  
}

