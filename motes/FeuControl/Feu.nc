/*
 * Feu tricolore controlé
 * 
 */

/**
 * Blink is a basic application that toggles the leds on the mote
 * on every clock interrupt.  The clock interrupt is scheduled to
 * occur every second.  The initialization of the clock can be seen
 * in the Blink initialization function, StdControl.start().<p>
 *
 * @author tinyos-help@millennium.berkeley.edu
 **/
#define MSG_CONTROL 6
 
configuration Feu {
}
implementation {
  components Main, FeuM, TimerC, LedsC, IntToRfm, GenericComm as Comm;
  Main.StdControl -> FeuM.StdControl;
  Main.StdControl -> TimerC.StdControl;
  Main.StdControl -> IntToRfm.StdControl;

  
  FeuM.Timer -> TimerC.Timer[unique("Timer")];
  FeuM.Leds -> LedsC;
  FeuM.IntOutput -> IntToRfm;
  //RFM
  FeuM.MessageControl -> Comm.ReceiveMsg[MSG_CONTROL]; 
  FeuM.SubControl -> Comm;
  
}

