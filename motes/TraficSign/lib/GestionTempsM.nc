
includes temps;

module GestionTempsM {
  uses {
    interface StdControl as SubControl;
	interface Timer;
  }
  provides {
    interface TempsOutput;
    interface StdControl;
  }

}
implementation {
	
  #define PERIODE_SECONDE 1000
  Temps local;

  command result_t StdControl.init()
  {
    local.secondes=0;
	local.minutes=0;
	local.heures=0;
	local.jours=0;
	local.mois=0;
	local.annees=0;
    return SUCCESS;
  }

  command result_t StdControl.start()
  {
    return call Timer.start(TIMER_REPEAT, PERIODE_SECONDE);
  }


  command result_t StdControl.stop()
  {
    return call Timer.stop();
  }

  //Gestion du temps :
  event result_t Timer.fired()
  {
	local.secondes++;
	if(local.secondes>60)
		{local.minutes++;local.secondes=0;
		 if(local.minutes>60)
			{local.heures++;local.minutes=0;local.secondes=0;
			 if(local.heures>24)
			    {local.jours++;local.heures=0;local.minutes=0;local.secondes=0;
			     if(local.jours>30)
			        {local.mois++;local.jours=0;local.heures=0;local.minutes=0;local.secondes=0;
			         if(local.mois>24)
			          local.annees++;local.mois=0;local.jours=0;local.heures=0;local.minutes=0;local.secondes=0;
			        }
			    }
			}
		}
	
	return SUCCESS;
  }
  
  command Temps TempsOutput.get()
  {
   return local;
  }
  
  command result_t TempsOutput.set(Temps localset)
  {
    local = localset;
    return SUCCESS;
  }
  
  
}

