/*
 * Copyright (c) 2002-2005 Intel Corporation
 * Copyright (c) 2000-2005 The Regents of the University of California
 * All rights reserved.
 * See license.txt file included with the distribution.
 *
 * $Id: Panneau.nc,v 1.1 2008/03/06 17:22:58 vonarnim Exp $
 */

/**
 * Blink is a basic application that toggles the leds on the mote
 * on every clock interrupt.  The clock interrupt is scheduled to
 * occur every second.  The initialization of the clock can be seen
 * in the Blink initialization function, StdControl.start().<p>
 *
 * @author tinyos-help@millennium.berkeley.edu
 **/
configuration Panneau {
}
implementation {
  components Main, PanneauM, TimerC, LedsC, IntToRfm;
  Main.StdControl -> PanneauM.StdControl;
  Main.StdControl -> TimerC.StdControl;
  Main.StdControl -> IntToRfm.StdControl;

  
  PanneauM.Timer -> TimerC.Timer[unique("Timer")];
  PanneauM.Leds -> LedsC;
  PanneauM.IntOutput -> IntToRfm;
  
}

