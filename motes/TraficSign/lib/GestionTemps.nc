

configuration GestionTemps {
	provides {
	    interface TempsOutput;
	    interface StdControl;
	  }
}
implementation {
  components GestionTempsM, TimerC;
  
  TempsOutput = GestionTempsM;
  StdControl = GestionTempsM;

  GestionTempsM.SubControl -> TimerC.StdControl;
  GestionTempsM.Timer -> TimerC.Timer[unique("Timer")]; 
}

