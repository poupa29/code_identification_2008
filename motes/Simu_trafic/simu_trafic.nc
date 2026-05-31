/*
 * Copyright (c) 2002-2005 Intel Corporation
 * Copyright (c) 2000-2005 The Regents of the University of California
 * All rights reserved.
 * See license.txt file included with the distribution.
 *
 * $Id: simu_trafic.nc,v 1.1 2008/07/04 12:52:53 bergont Exp $
 */
includes message_trafic;

configuration simu_trafic {
}
implementation {
  components Main, simu_traficM, TimerC, LedsC, RandomLFSR, GenericComm as Comm;
  Main.StdControl -> simu_traficM.StdControl;
  Main.StdControl -> TimerC.StdControl;
  
  simu_traficM.Random -> RandomLFSR.Random;
  
  simu_traficM.Send -> Comm.SendMsg[MSG_VEHICULE];  //envoi des msg vehicule
  simu_traficM.SubControl -> Comm;
    
  simu_traficM.Timer -> TimerC.Timer[unique("Timer")];
  simu_traficM.Leds -> LedsC;
  
}

