//
// Copyright (C) 2008,2010 Andreas Schroeder
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

#ifndef _3DSMAXSG_H_
#define _3DSMAXSG_H_

#include "constants.h"

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push,1)

  struct threedsmaxsgi {  /* 3dsmax Script Generator Info */
    char scene[BUFFERLEN];
    char scriptdir[BUFFERLEN];
    char image[BUFFERLEN];
  };

#pragma pack(pop)

  char *threedsmaxsg_create (struct threedsmaxsgi *info);
  char *threedsmaxsg_default_script_path (void);

#ifdef __cplusplus
}
#endif

#endif /* _3DSMAXSG_H_ */
