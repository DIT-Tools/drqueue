//
// Copyright (C) 2001,2002,2003,2004,2005,2006 Jorge Daza Garcia-Blanes
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

#ifndef _FIXED_POINTER_H_
#define _FIXED_POINTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#pragma pack(push,1)

#define fptr_type(TYPE,NAME) union { TYPE *ptr; int64_t padding; } NAME

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif 
