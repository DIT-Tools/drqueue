// 
// Copyright (C) 2001,2002,2003,2004 Jorge Daza Garcia-Blanes
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

#ifndef _PIXIESG_H_
#define _PIXIESG_H_

#ifdef __LINUX
#include <stdint.h>
#else
# ifdef __IRIX
#  include <sys/types.h>
# else
#  ifdef __OSX
#   include <stdint.h>
#  else
#   ifdef __FREEBSD
#    include <stdint.h>
#   else
#    error You need to define the OS, or OS defined not supported
#   endif
#  endif
# endif
#endif

#include "constants.h"

#ifdef __CPLUSPLUS
extern "C" {
#endif 

struct pixiesgi {		/* Pixie Script Generator Info */
  char renderdir[BUFFERLEN];
  char scene[BUFFERLEN];
  char scriptdir[BUFFERLEN];
/*
	char custom_crop;
	uint32_t xmin,xmax,ymin,ymax;
	char custom_samples;
	uint32_t xsamples,ysamples;
	char disp_stats;
	char verbose;
	char custom_beep;
	char custom_radiosity;
	uint32_t radiosity_samples;
	char custom_raysamples;
	uint32_t raysamples;
*/
};

char *pixiesg_create (struct pixiesgi *info);
char *pixiesg_default_script_path (void);

#ifdef __CPLUSPLUS
}
#endif 

#endif /* _PIXIESG_H_ */
