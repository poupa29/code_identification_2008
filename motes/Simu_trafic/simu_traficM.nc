

includes message_trafic;

module simu_traficM {
  uses {
	interface Timer;
    interface Leds;
	interface Random;
	interface StdControl as SubControl;
	interface SendMsg as Send;
	
  }
  provides {
    interface StdControl;
  }

}
implementation {
	
	#define PERIODE_RAFALE_MS 200
	
	
  void send_msg(MsgVehicule data)
  {	
    TOS_Msg msg;
    MsgVehicule *message = (MsgVehicule *)msg.data;
  
    message->id_vehicule = data.id_vehicule;
    message->average_speed = data.average_speed;
    message->type_vehicule = data.type_vehicule;
	
	//Envoi du message sur la radio
    call Send.send(TOS_BCAST_ADDR, sizeof(data), &msg);
   }

  command result_t StdControl.init()
  {
		call Leds.init(); 
		call Random.init();
		call SubControl.init();
    return SUCCESS;
  }



  command result_t StdControl.start()
  {
    int var=call Random.rand();
    var = var & 0x000f;
    call SubControl.start();
    return call Timer.start(TIMER_REPEAT, PERIODE_RAFALE_MS + var);
  }


  command result_t StdControl.stop()
  {call SubControl.stop();
    return call Timer.stop();
  }


  event result_t Timer.fired()
  {
    MsgVehicule data;
    
    data.id_vehicule = call Random.rand()&0x0fff;
    data.average_speed = 90 + (call Random.rand() & 0x000f);
	data.type_vehicule = call Random.rand() & 0x000f;
	send_msg(data);

	call Leds.greenToggle();
	
	return SUCCESS;
  }
  
  
  event result_t Send.sendDone(TOS_MsgPtr pmsg, result_t status) {
   call Leds.yellowToggle();
    return SUCCESS;
  }
 
}

