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

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>

#include "bmrtsg.h"
#include "libdrqueue.h"

char *bmrtsg_create (struct bmrtsgi *info) {
  /* This function creates the BMRT render script based on the information given */
  /* Returns a pointer to a string containing the path of the just created file */
  /* Returns NULL on failure and sets drerrno */

  static char filename[BUFFERLEN];
  char *p;   /* Scene filename without path */
  char scene[MAXCMDLEN];

  /* Check the parameters */
  if (!strlen(info->scene)) {
    drerrno = DRE_NOTCOMPLETE;
    return NULL;
  }

#ifdef __CYGWIN
  cygwin_conv_to_posix_path(info->scene, scene);
#else
  strncpy(scene,info->scene,MAXCMDLEN-1);
#endif

  p = strrchr(scene,'/');
  p = ( p ) ? p+1 : scene;
  snprintf(filename,BUFFERLEN-1,"%s/%s.%lX",info->scriptdir,p,(unsigned long int)time(NULL));

  // TODO: Unified path handling
  struct jobscript_info *ji = jobscript_new (JOBSCRIPT_PYTHON, filename);

  jobscript_write_heading (ji);
  jobscript_set_variable (ji,"SCENE",scene);
  jobscript_set_variable (ji,"DISP_STATS",&info->disp_stats);
  jobscript_set_variable (ji,"VERBOSE",&info->verbose);
  jobscript_set_variable (ji,"CUSTOM_BEEP",&info->custom_beep);

  if (info->custom_crop) {
    jobscript_set_variable (ji,"CUSTOM_CROP","yes");
    jobscript_set_variable_int (ji,"CROP_XMIN",info->xmin);
    jobscript_set_variable_int (ji,"CROP_XMAX",info->xmax);
    jobscript_set_variable_int (ji,"CROP_YMIN",info->ymin);
    jobscript_set_variable_int (ji,"CROP_YMAX",info->ymax);
  }
  
  if (info->custom_samples) {
  	jobscript_set_variable (ji,"CUSTOM_SAMPLES","yes");
  	jobscript_set_variable_int (ji,"XSAMPLES",info->xsamples);
  	jobscript_set_variable_int (ji,"YSAMPLES",info->ysamples);
  }
  
  if (info->custom_radiosity) {
  	jobscript_set_variable (ji,"CUSTOM_RADIOSITY","yes");
  	jobscript_set_variable_int (ji,"RADIOSITY_SAMPLES",info->radiosity_samples);
  }
  
  if (info->custom_raysamples) {
  	jobscript_set_variable (ji,"CUSTOM_RAYSAMPLES","yes");
  	jobscript_set_variable_int (ji,"RAYSAMPLES",info->raysamples);
  }

  jobscript_template_write (ji,"bmrt_sg.py");
  jobscript_close (ji);

  return filename;
}

char *bmrtsg_default_script_path (void) {
  static char buf[BUFFERLEN];
  char *p;

  if (!(p = getenv("DRQUEUE_TMP"))) {
    return ("/drqueue_tmp/not/set/");
  }

  if (p[strlen(p)-1] == '/')
    snprintf (buf,BUFFERLEN-1,"%s",p);
  else
    snprintf (buf,BUFFERLEN-1,"%s/",p);

  return buf;
}




