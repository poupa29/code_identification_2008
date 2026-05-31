

configuration FeuC {
provides {
	    interface StdControl;
		interface StdControl as ControlModule;
	  }
}
implementation {
  components FeuCM, TimerC, LedsC, IntToRfm;
  
  StdControl = FeuCM.StdControl;
  ControlModule = FeuCM.ControlModule;
  
  FeuCM.SubControl -> FeuCM.StdControl;
  FeuCM.SubControl -> TimerC.StdControl;
  FeuCM.SubControl -> IntToRfm.StdControl;

  FeuCM.Timer -> TimerC.Timer[unique("Timer")];
  FeuCM.Leds -> LedsC;
  FeuCM.IntOutput -> IntToRfm;
  
}

