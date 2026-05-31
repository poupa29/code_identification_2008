
includes temps;

interface TempsOutput {

  //retourne le temps 
  
  command Temps get();
  //configure le temps
  
  command result_t set(Temps localset);


}
