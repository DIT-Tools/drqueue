//
// Copyright (C) 2001,2002,2003,2004,2005 Jorge Daza Garcia-Blanes
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
// USA
//
// $Id$
//

#ifndef _ENVVARS_H_
#define _ENVVARS_H_

#include "constants.h"

#ifndef __IRIX
# include <stdint.h>
#endif

#include <sys/types.h>

// A single environment variable
struct envvar {
  char name[MAXNAMELEN];
  char value[MAXNAMELEN];
};

// The group of all environment variables
struct envvars {
  struct envvar *variables;
  uint16_t nvariables;
  int64_t evshmid;
  int64_t evsemid; // semaphore exclusive for envvars
};

int envvars_init (struct envvars *envvars);
int envvars_empty (struct envvars *envvars);
int envvars_variable_add (struct envvars *envvars, char *name, char *value);
int envvars_variable_delete (struct envvars *envvars, char *name);
int envvars_attach (struct envvars *envvars);
int envvars_detach (struct envvars *envvars);
struct envvar* envvars_variable_find (struct envvars *envvars, char *name);    // Returns "name"'s value or NULL if it does not exist
int64_t envvars_get_shared_memory (int size);
void envvars_dump_info (struct envvars *envvars);

#endif /* _ENVVARS_H_ */
