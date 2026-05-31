
includes special_msg;

module DataToRfmM 
{
  uses {
    interface StdControl as SubControl;
    interface SendMsg as Send;
  }
  provides {
    interface DataOutput;
    interface StdControl;
  }
}
implementation
{
  bool pending;
  TOS_Msg data;

  command result_t StdControl.init() {
    pending = FALSE;
    return call SubControl.init();
  }

  command result_t StdControl.start() 
  {
    return call SubControl.start();
  }


    command result_t StdControl.stop() 
  {
    return call SubControl.stop();
  }

  command result_t DataOutput.output(uint16_t length,uint16_t value[length/2])
  { int i;
    Special_Msg *message = (Special_Msg *)data.data;

    if (!pending) 
      {
	pending = TRUE;
	atomic {
	
    for(i=0;i<(length/2);i++)
	 {  message->datal[i] = value[i];
     }
     
     //id du mote
     message->datal[i] = TOS_LOCAL_ADDRESS; //bidouille pour que ca fonctionne !
     message->moteid = TOS_LOCAL_ADDRESS;
     
	}
	if (call Send.send(TOS_BCAST_ADDR, length + 2, &data))
	  return SUCCESS;

	pending = FALSE;
      }
    return FAIL;
  }

  event result_t Send.sendDone(TOS_MsgPtr msg, result_t success)
  {
    if (pending && msg == &data)
      {
	pending = FALSE;
	signal DataOutput.outputComplete(success);
      }
    return SUCCESS;
  }
}




