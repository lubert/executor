#if !defined(_RSYS_NEXT_H_)
#define _RSYS_NEXT_H_

/*
 * Copyright 1991 by Abacus Research and Development, Inc.
 * All rights reserved.
 *
 * $Id: next.h 63 2004-12-24 18:19:43Z ctm $
 */

#include "rsys/smash.h"

/*
 * make sure there are no duplicate characters in INFO_START_STRING
 */

#define INFO_START_STRING "Run Paiot,-Svp."
                        /* 1234567890123456 */
      
typedef struct
{
  char info_start_string[sizeof(INFO_START_STRING)];
  LONGINT serialnumber;
  char name[81];
  char organization[81];
  char key[81];
  LONGINT inodenumber;
  LONGINT timechanged;
} infoblock;

extern infoblock ROMlib_info;

#define SETUPA5 LONGINT savea5; savea5 = EM_A5; EM_A5 = CL( (LONGINT) CurrentA5)

#define RESTOREA5 EM_A5 = savea5

#endif
