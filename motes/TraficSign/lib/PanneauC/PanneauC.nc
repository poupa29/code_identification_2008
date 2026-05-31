/*
 * Copyright (c) 2002-2005 Intel Corporation
 * Copyright (c) 2000-2005 The Regents of the University of California
 * All rights reserved.
 * See license.txt file included with the distribution.
 *
 * $Id: PanneauC.nc,v 1.2 2008/07/10 08:29:20 bergont Exp $
 */

configuration PanneauC {
provides {
	    interface StdControl;
	    interface StdControl as ControlModule;
	  }
}
implementation {
  components  PanneauCM, TimerC, LedsC, IntToRfm, RandomLFSR;
  
  StdControl = PanneauCM.StdControl;
  ControlModule = PanneauCM.ControlModule;
  
  PanneauCM.SubControl -> PanneauCM.StdControl;
  PanneauCM.SubControl -> TimerC.StdControl;
  PanneauCM.SubControl -> IntToRfm.StdControl;
  
  PanneauCM.Random -> RandomLFSR.Random;
  
  PanneauCM.Timer -> TimerC.Timer[unique("Timer")];
  PanneauCM.Leds -> LedsC;
  PanneauCM.IntOutput -> IntToRfm;
  
}

