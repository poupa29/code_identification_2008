module PanneauM {
  uses {
	interface Timer;
    interface Leds;
	interface IntOutput;
  }
  provides {
    interface StdControl;
  }

}
implementation {
	
	#define PERIODE_RAFALE_MS 100


  command result_t StdControl.init()
  {

		call Leds.init(); 
    return SUCCESS;
  }



  command result_t StdControl.start()
  {
    return call Timer.start(TIMER_REPEAT, PERIODE_RAFALE_MS);
  }


  command result_t StdControl.stop()
  {
    return call Timer.stop();
  }


  event result_t Timer.fired()
  {
	call IntOutput.output(TOS_LOCAL_ADDRESS);
	call Leds.greenToggle();
	
	return SUCCESS;
  }
  
  event result_t IntOutput.outputComplete(result_t success) 
  {
  }
  
}

