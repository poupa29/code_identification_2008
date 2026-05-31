
#include "MAPS_LivicMsgRadioSimu.h" // Includes the header of this component

// Use the macros to declare the inputs
MAPS_BEGIN_INPUTS_DEFINITION(MAPSLivicMsgRadioSimu)
    MAPS_INPUT("iSynchro",MAPS::FilterInteger,MAPS::FifoReader) 
MAPS_END_INPUTS_DEFINITION

// Use the macros to declare the outputs
MAPS_BEGIN_OUTPUTS_DEFINITION(MAPSLivicMsgRadioSimu)
    MAPS_OUTPUT("oMessage",MAPS::Stream8,NULL,NULL,TAILLE_MSG)
    MAPS_OUTPUT("oTY",MAPS::Stream8,NULL,NULL,1)
MAPS_END_OUTPUTS_DEFINITION

// Use the macros to declare the properties
MAPS_BEGIN_PROPERTIES_DEFINITION(MAPSLivicMsgRadioSimu)
    MAPS_PROPERTY("pModeFeuTricolore",false,false,false)
    MAPS_PROPERTY("pFrequenceRafaleFeuTri",2,false,false)
    MAPS_PROPERTY("pNombreDeMessages",0,false,false)
    MAPS_PROPERTY("pProprieteInteger",0,false,false)
    MAPS_PROPERTY("pProprieteString","",false,false)
MAPS_END_PROPERTIES_DEFINITION

// Use the macros to declare the actions
MAPS_BEGIN_ACTIONS_DEFINITION(MAPSLivicMsgRadioSimu)
MAPS_END_ACTIONS_DEFINITION

#define NB_PROPRIETES_VISIBLES 3

// Use the macros to declare this component (LivicMsgRadioSimu) behaviour
MAPS_COMPONENT_DEFINITION(
    MAPSLivicMsgRadioSimu,
    "LivicMsgRadioSimu",
    "$Revision: 1.5 $",
    128,
    MAPS::Threaded,
    MAPS::Threaded,
    1,
    2,
    NB_PROPRIETES_VISIBLES,
    0)

void MAPSLivicMsgRadioSimu::Dynamic()
{
    char sProperty[128];
    pNombreDeMessages = (int)GetIntegerProperty("pNombreDeMessages");
    for(int i=0 ; i < pNombreDeMessages ; i++)
    {
        sprintf(sProperty,"%s%d","pSynchroMsg",i);
        NewProperty(NB_PROPRIETES_VISIBLES,sProperty);
        sprintf(sProperty,"%s%d","pMsg",i);
        NewProperty(NB_PROPRIETES_VISIBLES+1,sProperty);
    }
}

void MAPSLivicMsgRadioSimu::Birth()
{
    char sProperty[128];
    pNombreDeMessages = (int)GetIntegerProperty("pNombreDeMessages");
    synchros = (int*)malloc(pNombreDeMessages*sizeof(int));
    messages = (char**)malloc(pNombreDeMessages*sizeof(char*));
    for(int i=0; i<pNombreDeMessages; i++) messages[i] = (char*)malloc((TAILLE_MSG+1)*sizeof(char));
    for(int i=0 ; i < pNombreDeMessages; i++)
    {
        sprintf(sProperty,"%s%d","pSynchroMsg",i);
        synchros[i] = (int)GetIntegerProperty(sProperty);
        sprintf(sProperty,"%s%d","pMsg",i);
        strcpy(messages[i], GetStringProperty(sProperty));
    }
	pa1 = 0;
	strcpy(messageFeuTri, "");
}

void MAPSLivicMsgRadioSimu::Core() 
{
    if( pNombreDeMessages == 0 ) return;
	char spa1[3+1];
	if( strlen(messageFeuTri)>0 )
	{
		MAPSTimestamp t = MAPS::CurrentTime();
		if( (t-tPrec)>(GetIntegerProperty("pFrequenceRafaleFeuTri")*1000000) )
		{
			tPrec = t;
			pa1 -= (int)GetIntegerProperty("pFrequenceRafaleFeuTri");
			pa1 = max(0, pa1);
			sprintf(spa1, "%03d", pa1);
			strcpy(messageFeuTri+10, spa1);
			MAPSIOElt *oEltMsg = StartWriting(Output("oMessage"));
			memcpy(oEltMsg->Stream8(), messageFeuTri, TAILLE_MSG+1);
			oEltMsg->VectorSize()=TAILLE_MSG+1;
			StopWriting(oEltMsg);
			ReportInfo(messageFeuTri);
			if( pa1==0 ) strcpy(messageFeuTri, "");
		}
	}

	if( DataAvailableInFIFO(Input("iSynchro")) )
	{
		MAPSIOElt *iEltSynchro = StartReading(Input("iSynchro"));
		int synchro = (int)(iEltSynchro->Integer());
		StopReading(Input("iSynchro"));
		int i;
		for(i=0; i<pNombreDeMessages; i++)
		{
			if( synchros[i] == synchro ) break;
		}
		if( i<pNombreDeMessages )
		{
			strcpy(messageFeuTri, "");
			pa1 = 0;
			char *msg = messages[i];
			MAPSIOElt *oEltMsg = StartWriting(Output("oMessage"));
			memcpy(oEltMsg->Stream8(), msg, TAILLE_MSG+1);
			oEltMsg->VectorSize()=TAILLE_MSG+1;
			StopWriting(oEltMsg);
			ReportInfo(msg);

			if( GetBoolProperty("pModeFeuTricolore") )
			{
				if( strlen(messageFeuTri)==0 )
				{
					strncpy(spa1, msg+10, 3);
					pa1 = atoi(spa1);
					if( pa1<=0 ) {
						strcpy(messageFeuTri, "");
					}
					else strcpy(messageFeuTri, msg);
					tPrec = MAPS::CurrentTime();
				}
			}
		}
	}
	Rest(100000);
}

void MAPSLivicMsgRadioSimu::Death()
{
    free(synchros);
    for(int i=0; i<pNombreDeMessages; i++) free(messages[i]);
    free(messages);
	pa1 = 0;
	strcpy(messageFeuTri, "");
}