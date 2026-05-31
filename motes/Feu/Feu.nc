/*
 * Copyright (c) 2002-2005 Intel Corporation
 * Copyright (c) 2000-2005 The Regents of the University of California
 * All rights reserved.
 * See license.txt file included with the distribution.
 *
 * $Id: Feu.nc,v 1.1 2008/03/06 17:22:57 vonarnim Exp $
 */

/**
 * Blink is a basic application that toggles the leds on the mote
 * on every clock interrupt.  The clock interrupt is scheduled to
 * occur every second.  The initialization of the clock can be seen
 * in the Blink initialization function, StdControl.start().<p>
 *
 * @author tinyos-help@millennium.berkeley.edu
 **/
configuration Feu {
}
implementation {
  components Main, FeuM, TimerC, LedsC, IntToRfm;
  Main.StdControl -> FeuM.StdControl;
  Main.StdControl -> TimerC.StdControl;
  Main.StdControl -> IntToRfm.StdControl;

  
  FeuM.Timer -> TimerC.Timer[unique("Timer")];
  FeuM.Leds -> LedsC;
  FeuM.IntOutput -> IntToRfm;
  
}

