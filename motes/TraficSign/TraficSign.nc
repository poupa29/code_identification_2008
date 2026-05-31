
/*
* PANNEAU TRAFIC :
* 
* 
* 
*/
 
includes IntMsg;
includes message_trafic;
includes temps;
 
configuration TraficSign { }
implementation {											//LedsC
  components Main, TraficSignM, GenericComm as Comm, Logger, NoLeds as Led, GestionTemps ,FeuC; //,PanneauC;
  //Composant panneau ou feu
  //Main.StdControl -> PanneauC;
  TraficSignM.SubControl -> FeuC.StdControl;
  TraficSignM.SubControlModule -> FeuC.ControlModule; //controle du module
  
  //Main
  Main.StdControl -> TraficSignM;
  TraficSignM.Leds -> Led;
  //Gestion du temps
  TraficSignM.TempsOutput -> GestionTemps.TempsOutput;
  TraficSignM.SubControl -> GestionTemps.StdControl;
  //Logger
  TraficSignM.LoggerWrite -> Logger.LoggerWrite;
  TraficSignM.LoggerRead -> Logger.LoggerRead;
  TraficSignM.SubControl->Logger.StdControl;
  //Radio
  TraficSignM.SendLog -> Comm.SendMsg[MSG_LOG];                  //envoi du log
  TraficSignM.ReceiveVehicule -> Comm.ReceiveMsg[MSG_VEHICULE];  //reception des id voitures
  TraficSignM.MessageControl -> Comm.ReceiveMsg[MSG_CONTROL];    //reception de message de controle
  TraficSignM.SubControl -> Comm;

}
