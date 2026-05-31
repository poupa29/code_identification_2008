/*
* Sense to RFM : Module
*/

module TelosbCodeToRfmM
{
  provides interface StdControl;
  uses {
    interface Timer;
    interface Leds;
    //Liaison au main
    interface SplitControl as HumidityControl;
	//ADC
    interface ADC as Humidity;
    interface ADC as Temperature;
    interface ADC as TSR;
    //Connection vers RFM
    interface DataOutput as Data;
    //ADCError
    interface ADCError as HumidityError;
    interface ADCError as TemperatureError;
  }
}
implementation
{
  uint16_t Calcul_Etat();
  
  uint16_t Calcul_Consigne();
  
  //************ VARIABLE *******************

  enum {
    HUMIDITY,
    TEMPERATURE,
    TSRSENSOR,
  };
  
  #define TEMPO_MSG_SEND 150  //temporisation entre chaque envoie de message
  #define TEMPO_MAJ_CAPTEUR 2500  //Temporisation entre chaque MAJ des capteurs
  #define VITESSE_ROUTE 110  //La vitesse de la route ou est placé le panneau
  
  norace uint16_t humidity, temperature, tsr;
  norace int state,temp_capteur,tempo;
  norace bool flag;
  norace uint16_t code;
  //*****************************************
  
	//Initialisation :
  command result_t StdControl.init() {
    call Leds.init();
    call Leds.set(0);
    state = HUMIDITY;
    flag = 1;
    tempo = 0;
    temp_capteur = TEMPO_MAJ_CAPTEUR;
    //Allume le capteur
    call HumidityControl.init();

    return SUCCESS;
  }

  event result_t HumidityControl.initDone() {
    return SUCCESS;
  }

  command result_t StdControl.start() {
    call HumidityControl.start();
    return SUCCESS;
  }

  event result_t HumidityControl.startDone() {
    call HumidityError.enable();
    call TemperatureError.enable();
    call Timer.start( TIMER_REPEAT, TEMPO_MSG_SEND);
    return SUCCESS;
  }

  command result_t StdControl.stop() {
    call HumidityControl.stop();
    call Timer.stop();
    return SUCCESS;
  }

  event result_t HumidityControl.stopDone() {
    call HumidityError.disable();
    call TemperatureError.disable();
    return SUCCESS;
  }

  
  event result_t Timer.fired() {
  int datal[5];
  
    //Mise a jour des capteurs
    if(temp_capteur>TEMPO_MAJ_CAPTEUR)
	{	
		if(tempo>3)
		{
		   call Leds.yellowToggle();
		   
		   switch(state) //graphe des actions 
		   { 
		    case HUMIDITY:
		      call Humidity.getData();
		      break;
		    case TEMPERATURE:
		      call Temperature.getData();
		      break;
		    case TSRSENSOR:
		      call TSR.getData();
		       //Calcul du code      
		      code = Calcul_Etat();
		      temp_capteur=0;
		      call Leds.yellowOff();
				break;
			default:
			  call Leds.redToggle(); //Si bug ...
		    }  
		    tempo=0;
		 }   
		tempo++;
	 }    
	
	//ENVOIE LES VALEURS : 
	datal[0]=Calcul_Consigne(); //vitesse de consigne en KM/H
    datal[1]=code; //code meteo/alerte
    datal[2]=humidity; 
    datal[3]=temperature; 
    datal[4]=tsr; 
	call Data.output(sizeof(datal),datal);
	call Leds.greenToggle(); 
	    
	temp_capteur += TEMPO_MSG_SEND;
    return SUCCESS;
  }
  
 
  uint16_t Calcul_Consigne() {
 
  switch(code) {
  	case 1: // 1 --> il fait jour, tout est normal.
	    return VITESSE_ROUTE;
		break;
	case 2: // 2 --> il fait nuit.
	    return VITESSE_ROUTE - 10;
		break;
	case 3: // 3 --> il fait chaud
	    return VITESSE_ROUTE - 5;
		break;
	case 4: // 4 --> risque brouillard/pluie le jour
	    return VITESSE_ROUTE - 20;
		break;
	case 5: // 5 --> risque brouillard/pluie la nuit
	    return 50;
		break;
	case 6: // 6 --> risque de verglas le jour
	    return 40;
		break;
	case 7: // 7 --> risque de verglas la nuit
	    return 30;
		break;
	default:
		return VITESSE_ROUTE;
	}
  }
  
  
  uint16_t Calcul_Etat() {
  float temp,hum;
  //Etat:
  // 1 --> il fait jour, tout est normal.
  // 2 --> il fait nuit.
  // 3 --> il fait chaud
  // 4 --> risque brouillard/pluie le jour
  // 5 --> risque brouillard/pluie la nuit
  // 6 --> risque de verglas le jour
  // 7 --> risque de verglas la nuit
  
  //humidity, temperature, tsr;
  
  //Calcul :
  temp = -38.4 + 0.0098 * temperature; //en degrée c
  hum = -0.0000028*humidity*humidity + 0.0405*humidity-4; //en % 
  
   if(hum>70 && tsr<300 && temp < 3)
     return 7;
     
   if(hum>70 && temp < 3)
     return 6;
   
   if(hum>70 && tsr<300)
     return 5;  
     
   if(hum>70)
     return 4;    
    
   if(temp>32)
     return 3; 
 
   if(tsr<300)
	  return 2;
	
	return 1;  //tout est normal !
  }
  
  
  
  async event result_t Humidity.dataReady(uint16_t data) {
    humidity = data;
    state = TEMPERATURE; //passe a l'action suivante
    flag=1;
    return SUCCESS;
  }
	//Gestion erreur
  event result_t HumidityError.error(uint8_t token) {
    humidity = 0;
    state = TEMPERATURE;
    flag=1;
    return SUCCESS;
  }

  async event result_t Temperature.dataReady(uint16_t data) {
    temperature = data;
    state = TSRSENSOR;
    flag=1;
    return SUCCESS;
  }

  event result_t TemperatureError.error(uint8_t token) {
    temperature = 0;
    state = TSRSENSOR;
    flag=1;
    return SUCCESS;
  }

  async event result_t TSR.dataReady(uint16_t data) {
    tsr = data;
    state = HUMIDITY;
    flag=1;
    return SUCCESS;
  }

    event result_t Data.outputComplete(result_t success) {
    return SUCCESS;
  }

}
