
//////////////////////////////////////////////
//Composant de couplage entre KSS VI et KSS SV
//////////////////////////////////////////////


#include "maps_LivicConfirmationSV_VI.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicConfirmationSV_VI)
	MAPS_INPUT("iPistesIR",MAPS::FilterRealObjects,MAPS::FifoReader)	
	MAPS_INPUT("iDataMote",MAPS::FilterInteger,MAPS::FifoReader)
	MAPS_INPUT("iIdMote",MAPS::FilterInteger,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicConfirmationSV_VI)
	MAPS_OUTPUT("oPistesIRSV",MAPS::RealObject,NULL,NULL,MAX_CIBLES_IR)
	MAPS_OUTPUT("oDataMote",MAPS::Integer,NULL,NULL,TAILLE_DATA_MOTE)
	MAPS_OUTPUT("oIdMote",MAPS::Integer,NULL,NULL,1)
	MAPS_OUTPUT("oIdMoteTableau",MAPS::Integer,NULL,NULL,TAMPON_MOTES)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicConfirmationSV_VI)
	MAPS_PROPERTY("pToleranceMillisecondes",250,false,true)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicConfirmationSV_VI)
    //MAPS_ACTION("aName",MAPSFiltreSV_VI::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (FiltreSV_VI) behaviour
MAPS_COMPONENT_DEFINITION(MAPSLivicConfirmationSV_VI,"LivicConfirmationSV_VI","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  3, // Nb of inputs
			  4, // Nb of outputs
			  1, // Nb of properties
			  0) // Nb of actions

void MAPSLivicConfirmationSV_VI::Birth()
{
	sv_id_recu = 0;
	//vide le tableau
	for(int i=0;i<TAMPON_MOTES;i++)
			sv_id_tab[i]=0;

	sv_timestamp = 0;
	debut_tolerance = (int)MAPS::CurrentTime();
}

void MAPSLivicConfirmationSV_VI::Core() 
{
	int i;
	tolerance = (int)GetIntegerProperty("pToleranceMillisecondes")*1000;
	MAPSIOElt *iEltSV=NULL;
	MAPSIOElt *iEltID=NULL;
	MAPSIOElt *iEltPistes_in=StartReading(Input("iPistesIR"));//Entrée du realobject

	MAPSRealObject *PistesIR_in = &iEltPistes_in->RealObject();

	//Lecture des données des motes
	if ((DataAvailableInFIFO(Input("iDataMote"))) && (DataAvailableInFIFO(Input("iIdMote"))))
	{
		iEltSV = StartReading(Input("iDataMote"));//Les données Int du mote
		for(i=0;i<iEltSV->VectorSize();i++)
			data[i] = iEltSV->Integer(i);

		iEltID = StartReading(Input("iIdMote"));//L'ID du mote
		sv_id_recu = iEltID->Integer();

		sv_timestamp = 	(int)iEltSV->Timestamp();

		StopReading(Input("iDataMote"));
		StopReading(Input("iIdMote"));
		
		
		//Recopie sur la sortie :
		MAPSIOElt* oEltSV = StartWriting(Output("oDataMote"));//Initialise l'écriture sur la sortie du composant
		oEltSV->VectorSize()=iEltSV->VectorSize();
		oEltSV->Timestamp() = sv_timestamp;
		for(i=0;i<iEltSV->VectorSize();i++)
			oEltSV->Integer(i) = data[i];    

		MAPSIOElt* oEltID = StartWriting(Output("oIdMote"));//Initialise l'écriture sur la sortie du composant	
		oEltID->Integer() = sv_id_recu;
	    oEltID->VectorSize()=1;
		
		StopWriting(oEltSV);
		StopWriting(oEltID);
		
		
		debut_tolerance = (int)MAPS::CurrentTime();
	}
	else   
		sv_id_recu = 0;
	
    remplir_tableau_id();

	MAPSIOElt* oEltPistes_out = StartWriting(Output("oPistesIRSV"));//Initialise l'écriture sur la sortie du composant
	MAPSRealObject *PistesIR_out = &oEltPistes_out->RealObject();
    oEltPistes_out->VectorSize() = iEltPistes_in->VectorSize();
    oEltPistes_out->Timestamp() = iEltPistes_in->Timestamp();
	
	for(int i=0; i<iEltPistes_in->VectorSize(); i++)//Pour chaque piste
    {
        LivicMapsUtils::copieRealObject(&(PistesIR_in[i]),&(PistesIR_out[i]));//Copie du realobject

	    if (presence_id(PistesIR_in[i].id))   //Si ID est confirmé
			PistesIR_out[i].sign.type = 1;    //Type du panneau = 1 
	    else
			PistesIR_out[i].sign.type = -1;   //Sinon type du panneau = -1
	}

	
	StopWriting(oEltPistes_out);
	StopReading(Input("iPistesIR"));
}

void MAPSLivicConfirmationSV_VI::Death()
{
}

//renvoie 'true' si l'id du mote est present
bool MAPSLivicConfirmationSV_VI::presence_id(unsigned int id)
{
	for(int i=0;i<TAMPON_MOTES;i++)
		if(sv_id_tab[i]==id)
			return true;
 return false;
}

void MAPSLivicConfirmationSV_VI::remplir_tableau_id(void)
{	
	int i;

	if(sv_id_recu != 0)
	{
		if(presence_id(sv_id_recu)) //l'id est deja present dans le tableau 
			 for(i=0;i<TAMPON_MOTES;i++) //recherche ou il se trouve dans le tableau
			 {	if(sv_id_tab[i] == sv_id_recu)
				   {sv_timestamp_tab[i] = MAPS::CurrentTime();
					break;
				   }
			 }
		else //Id n'est pas present dans le tableau
			 for(i=0;i<TAMPON_MOTES;i++) //recherche une place libre dans le tableau
				{	if(sv_id_tab[i] == 0)
					  { sv_id_tab[i] = sv_id_recu;
						sv_timestamp_tab[i] = MAPS::CurrentTime();
						break;
					  }
			    }
	}

	// Gestion des tolerances 
	for(i=0;i<TAMPON_MOTES;i++) 
		if((MAPS::CurrentTime() - sv_timestamp_tab[i])> tolerance)
			sv_id_tab[i] = 0 ; 

    //pour les enregistrements !!!
	MAPSIOElt* oEltSVtest = StartWriting(Output("oIdMoteTableau"));
		for(i=0;i<TAMPON_MOTES;i++)
			oEltSVtest->Integer(i) = sv_id_tab[i];  
	StopWriting(oEltSVtest);
}
