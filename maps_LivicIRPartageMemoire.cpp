////////////////////////////////
// RTMaps SDK Component
////////////////////////////////

////////////////////////////////
// Purpose of this module :
////////////////////////////////

#include "maps_PartageMemoire.h"	// Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSPartageMemoire)
	MAPS_INPUT("iPistes",MAPS::FilterRealObjects,MAPS::FifoReader)
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSPartageMemoire)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSPartageMemoire)
    //MAPS_PROPERTY("pPeriode",20,false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSPartageMemoire)
    //MAPS_ACTION("aName",MAPSPartageMemoire::ActionName)
MAPS_END_ACTIONS_DEFINITION

// Use the macros to declare this component (PartageMemoire) behaviour
MAPS_COMPONENT_DEFINITION(MAPSPartageMemoire,"PartageMemoire","1.0",128,
			  MAPS::Threaded,MAPS::Threaded,
			  1, // Nb of inputs
			  0, // Nb of outputs
			  0, // Nb of properties
			  0) // Nb of actions

void MAPSPartageMemoire::Birth()
{
    // Reports this information to the RTMaps console
    ReportInfo("PartageMemoire: Passing through Birth() method");

	vectorSize = 0;
	Start=true;

	EtatSem=0;

	MP=NULL;
	MP=new MemoireP;

	if(MP==NULL)
	{
		ReportError("impossible de creerMP");
		Start=false;
		return;
	}
	if(MP->Initialise("testAClef",4*MAX_CIBLES_IR*sizeof(float)+1,true)!=0)
	{
		ReportError("Erreur initialise MP");
		ReportError(MP->GetLastErreur());
		Start=false;
		return;
	}
	/*if((iEltPistes=StartReading(Input("iPistes")))==NULL)
				ReportInfo("Erreur Input iPistes");
	EtatSem=MP->Write(&iEltPistes->RealObject());
	StopReading(Input("iPistes"));
	if(EtatSem!=0)
	{
		Start=false;
		ReportError("Erreur Write MP");
		ReportError(MP->GetLastErreur());
		return;
	}*/

}

void MAPSPartageMemoire::Core() 
{
    if(Start)
	{
		MAPSRealObject* realObject;
		if((iEltPistes=StartReading(Input("iPistes")))==NULL)
				ReportInfo("Erreur Input iPistes");
		//Alerte=(int)iEltAlerte->Integer();
		//StopReading(Input("iAlerte"));
		vectorSize = iEltPistes->VectorSize();
		realObject = &iEltPistes->RealObject();
		ObjectMP[0] = vectorSize;
		for(int i=1; i<vectorSize; i++)
		{
			ObjectMP[i]= realObject->id;
			ObjectMP[i+vectorSize] = realObject->x;
			ObjectMP[i+2*vectorSize] = realObject->y;
			ObjectMP[i+3*vectorSize] = realObject->z;
		}
		EtatSem=MP->Write((char*)(&ObjectMP));
		if(EtatSem!=0)
		{
			Start=false;
			ReportError("Erreur Write MP");
			ReportError(MP->GetLastErreur());
		}
		StopReading(Input("iPistes"));
	}
	else
	{
		Rest(2000*1000);
		ReportInfo("PartageMemoireHS");
	}
}

void MAPSPartageMemoire::Death()
{
    // Reports this information to the RTMaps console
    ReportInfo("PartageMemoire: Passing through Death() method");
	if(MP!=NULL)
		delete MP;
	MP=NULL;

}
