/*
 * Copyright (c) 2002-2005 Intel Corporation
 * Copyright (c) 2000-2005 The Regents of the University of California
 * All rights reserved.
 * See license.txt file included with the distribution.
 *
 * $Id: SingleTimer.nc,v 1.1 2008/07/10 08:29:23 bergont Exp $
 */

/*
 *
 * Authors:		Phil Levis
 * Date last modified:  7/30/03
 * Description:         This component provides a single timer, it is used in
 *                      the TinyOS tutorials to provide a Timer without
 *			requiring all of the mechanisms of parameterized
 *			interfaces.
 */

/**
 * @author Phil Levis
 */


configuration SingleTimer {
  provides interface Timer;
  provides interface StdControl;
}

implementation {
  components TimerC;
  
  Timer = TimerC.Timer[unique("Timer")];
  StdControl = TimerC;
}
