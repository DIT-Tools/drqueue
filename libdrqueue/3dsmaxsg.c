//
// Copyright (C) 2008 Andreas Schroeder
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

#include "3dsmaxsg.h"
#include "libdrqueue.h"

char *threedsmaxsg_create (struct threedsmaxsgi *info) {
  /* This function creates the luxrender render script based on the information given */
  /* Returns a pointer to a string containing the path of the just created file */
  /* Returns NULL on failure and sets drerrno */
  static char filename[BUFFERLEN];
  char *p;   /* Scene filename without path */

  
  /* FILE *f;
  /* FILE *etc_3dsmax_sg;  */  /* The 3dsmax script generator configuration file */
  /* int fd_etc_3dsmax_sg,fd_f; */
  /* static char filename[BUFFERLEN]; */
  /* char fn_etc_3dsmax_sg[BUFFERLEN]; */ /* File name pointing to DRQUEUE_ETC/3dsmax.sg */
  /* char buf[BUFFERLEN]; */
  /* int size; */
  /* char *p;  */ /* Scene filename without path */
  
  char scene[MAXCMDLEN];
  char image[MAXCMDLEN];

  /* Check the parameters */
  if (!strlen(info->scene)) {
    drerrno = DRE_NOTCOMPLETE;
    return NULL;
  }

#ifdef __CYGWIN
  cygwin_conv_to_posix_path(info->scene, scene);
  cygwin_conv_to_posix_path(info->image, image);
#else
  strncpy(scene,info->scene,MAXCMDLEN-1);
  strncpy(image,info->image,MAXCMDLEN-1);
#endif

  p = strrchr(scene,'/');
  p = ( p ) ? p+1 : scene;
  snprintf(filename,BUFFERLEN-1,"%s/%s.%lX",info->scriptdir,p,(unsigned long int)time(NULL));

  // TODO: Unified path handling
  struct jobscript_info *ji = jobscript_new (JOBSCRIPT_PYTHON,filename);

  jobscript_write_heading (ji);
  jobscript_set_variable (ji,"SCENE",info->scene);
  jobscript_set_variable (ji,"DRQUEUE_IMAGE",info->image);
  
  jobscript_template_write (ji,"3dsmax_sg.py");
  jobscript_close (ji);

  return filename;
}


char *threedsmaxsg_default_script_path (void) {
  static char buf[BUFFERLEN];
  char *p;

  if (!(p = getenv("DRQUEUE_TMP"))) {
    return ("/drqueue_tmp/not/set/report/bug/please/");
  }

  if (p[strlen(p)-1] == '/')
    snprintf (buf,BUFFERLEN-1,"%s",p);
  else
    snprintf (buf,BUFFERLEN-1,"%s/",p);

  return buf;
}
