module PanneauCM {
  uses {
    interface StdControl as SubControl;
	interface Timer;
    interface Leds;
	interface IntOutput;
	interface Random;
  }
  provides {
    interface StdControl;
    interface StdControl as ControlModule;
  }

}
implementation {
	
	#define PERIODE_RAFALE_MS 100
  bool flag;

  command result_t StdControl.init()
  {
		call Leds.init(); 
		call Random.init();
    return SUCCESS;
  }



  command result_t StdControl.start()
  { flag=1;
    int var=call Random.rand();
    var = var & 0x000f;
    return call Timer.start(TIMER_REPEAT, PERIODE_RAFALE_MS + var);
  }


  command result_t StdControl.stop()
  {
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

	if(flag) call IntOutput.output(TOS_LOCAL_ADDRESS);
	
	call Leds.greenToggle();
	
	return SUCCESS;
  }
  
  event result_t IntOutput.outputComplete(result_t success) 
  {return SUCCESS;
  }

  
}

