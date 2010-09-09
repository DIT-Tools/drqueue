//
// Copyright (C) 2001,2002,2003,2004 Jorge Daza Garcia-Blanes
// Copyright (C) 2010 Andreas Schroeder
//
// This file is part of DrQueue
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

#ifndef _LIGHTWAVESG_H_
#define _LIGHTWAVESG_H_

#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push,1)

  struct lightwavesgi {  /* Maya Script Generator Info */
    char projectdir[BUFFERLEN];
    char configdir[BUFFERLEN];
    char scene[BUFFERLEN];
    char image[BUFFERLEN];
    char scriptdir[BUFFERLEN];
    char file_owner[BUFFERLEN];
    char camera[BUFFERLEN];
    int  res_x,res_y;  /* Resolution of the frame */
    char format[BUFFERLEN];
    int  mentalray;  // 1 if we should render with mr
  };

#pragma pack(pop)

  char *lightwavesg_create (struct lightwavesgi *info);

  char *lightwavesg_default_script_path (void);

#ifdef __cplusplus
}
#endif

#endif /* _LIGHTWAVESG_H_ */
