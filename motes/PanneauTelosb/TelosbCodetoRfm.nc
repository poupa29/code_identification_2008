/*
* Panneau intelligent
* 
* Envoie la valeur des capteurs sur la radio + la consigne de vitesse + le code meteo
* grace à DataToRfm, les 5 valeurs sont envoyées dans la trame comme suivant :
* consignevit --> codemeteo --> Humidity --> Temp --> TSR --> Id mote
* 
*/
configuration TelosbCodeToRfm { }
implementation
{
  components Main
           , TelosbCodeToRfmM
           , TimerC
           , DataToRfm //Permet d'envoyer un tableau de d'entier, source dans ./lib
           , LedsC
           , HumidityC //Temperature + humidité
           , HamamatsuC //capteur de lumière
           ;
           
  //Init
  Main.StdControl -> TimerC;
  Main.StdControl -> HamamatsuC;
  Main.StdControl -> TelosbCodeToRfmM;
  Main.StdControl -> DataToRfm.StdControl;
  
  //Composants: 
  TelosbCodeToRfmM.Timer -> TimerC.Timer[unique("Timer")];
  TelosbCodeToRfmM.Leds -> LedsC;
  TelosbCodeToRfmM.HumidityControl -> HumidityC;
  //ADC
  TelosbCodeToRfmM.Humidity -> HumidityC.Humidity;
  TelosbCodeToRfmM.Temperature -> HumidityC.Temperature;
  TelosbCodeToRfmM.TSR -> HamamatsuC.TSR;
  //ERROR
  TelosbCodeToRfmM.HumidityError -> HumidityC.HumidityError;
  TelosbCodeToRfmM.TemperatureError -> HumidityC.TemperatureError;  
  //RFM
  TelosbCodeToRfmM.Data -> DataToRfm;
}
