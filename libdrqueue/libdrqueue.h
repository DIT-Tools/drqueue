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

#ifndef _LIBDRQUEUE_H_
#define _LIBDRQUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <errno.h>

#pragma pack(push,1)

extern int drerrno;

#include "pointer.h"
#include "slavedb.h"
#include "computer.h"
#include "job.h"
#include "task.h"
#include "logger.h"
#include "communications.h"
#include "request.h"
#include "drerrno.h"
#include "database.h"
#include "semaphores.h"
#include "common.h"
#include "envvars.h"
#include "jobscript.h"
#include "config.h"
#include "list.h"
#include "computer_pool.h"

/* Script generators */
#include "generalsg.h"
#include "mayasg.h"
#include "mentalraysg.h"
#include "blendersg.h"
#include "pixiesg.h"
#include "3delightsg.h"
#include "lightwavesg.h"
#include "aftereffectssg.h"
#include "shakesg.h"
#include "terragensg.h"
#include "nukesg.h"
#include "aqsissg.h"
#include "mantrasg.h"
#include "turtlesg.h"
#include "xsisg.h"
#include "cinema4dsg.h"
#include "luxrendersg.h"
#include "vraysg.h"
#include "3dsmaxsg.h"

extern int phantom[2];         /* FIXME: This should be local to the
				  slave */

#pragma pack(pop)

#ifdef __cplusplus
}
#endif

#endif /* _libdrqueue_h_ */
