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


#ifndef _VRAYSG_H_
#define _VRAYSG_H_

#ifdef __cplusplus
extern "C" {
#endif

#pragma pack(push,1)

  struct vraysgi {  /* vray Script Generator Info */
    char scene[BUFFERLEN];
    char scriptdir[BUFFERLEN];
  };

#pragma pack(pop)

  char *vraysg_create (struct vraysgi *info);
  char *vraysg_default_script_path (void);

#ifdef __cplusplus
}
#endif

#endif /* _LUXRENDERSG_H_ */
