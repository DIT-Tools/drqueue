//
// Copyright (C) 2001,2002,2003,2004,2005,2006 Jorge Daza Garcia-Blanes
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

#include "mayasg.h"
#include "libdrqueue.h"

#ifdef __CYGWIN
void cygwin_conv_to_posix_path(const char *path, char *posix_path);
#endif


char *mayasg_create (struct mayasgi *info) {
  /* This function creates the maya render script based on the information given */
  /* Returns a pointer to a string containing the path of the just created file */
  /* Returns NULL on failure and sets drerrno */
  static char filename[BUFFERLEN];
  char *p;   /* Scene filename without path */
  char scene[MAXCMDLEN];
  char renderdir[MAXCMDLEN];
  char projectdir[MAXCMDLEN];
  char precommand[MAXCMDLEN];
  char postcommand[MAXCMDLEN];

  /* Check the parameters */
  if ((!info->renderdir) || (!info->scene) || (!info->projectdir)
      || (!strlen(info->renderdir)) || (!strlen(info->scene)) || (!strlen(info->projectdir))) {
    drerrno = DRE_NOTCOMPLETE;
    return NULL;
  }

#ifdef __CYGWIN
  cygwin_conv_to_posix_path(info->scene, scene);
  cygwin_conv_to_posix_path(info->renderdir, renderdir);
  cygwin_conv_to_posix_path(info->projectdir, projectdir);
  cygwin_conv_to_posix_path(info->precommand, precommand);
  cygwin_conv_to_posix_path(info->postcommand, postcommand);
#else
  strncpy(scene,info->scene,MAXCMDLEN-1);
  strncpy(renderdir,info->renderdir,MAXCMDLEN-1);
  strncpy(projectdir,info->projectdir,MAXCMDLEN-1);
  strncpy(precommand,info->precommand,MAXCMDLEN-1);
  strncpy(postcommand,info->postcommand,MAXCMDLEN-1);
#endif

  p = strrchr(scene,'/');
  p = ( p ) ? p+1 : scene;
  snprintf(filename,BUFFERLEN-1,"%s/%s.%lX",info->scriptdir,p,(unsigned long int)time(NULL));

  // TODO: Unified path handling
  struct jobscript_info *ji = jobscript_new (JOBSCRIPT_TCSH,filename);

  jobscript_write_heading (ji);
  jobscript_set_variable (ji,"DRQUEUE_RD",renderdir);
  jobscript_set_variable (ji,"DRQUEUE_PD",projectdir);
  jobscript_set_variable (ji,"DRQUEUE_SCENE",scene);
  jobscript_set_variable (ji,"RF_OWNER",info->file_owner);
  if (strlen(info->format)) {
    jobscript_set_variable (ji,"FFORMAT",info->format);
  }
  if (info->res_x != -1) {
    jobscript_set_variable_int (ji,"RESX",info->res_x);
  }
  if (info->res_y != -1) {
    jobscript_set_variable_int (ji,"RESY",info->res_y);
  }
  if (strlen(info->camera)) {
    jobscript_set_variable (ji,"CAMERA",info->camera);
  }
  if (strlen(info->image)) {
    jobscript_set_variable (ji,"DRQUEUE_IMAGE",info->image);
  }
  if (info->renderer == 0) {
    jobscript_set_variable (ji,"RENDERER", "sw");
  }
  if (info->renderer == 1) {
    jobscript_set_variable (ji,"RENDERER", "mr");
  }
  if (info->renderer == 2) {
    jobscript_set_variable (ji,"RENDERER", "rman");
  }
  if (info->renderer == 3) {
    jobscript_set_variable (ji,"RENDERER", "file");
  }
  if (strlen(info->postcommand)) {
    jobscript_set_variable (ji,"DRQUEUE_POST",postcommand);
  }
  if (strlen(info->precommand)) {
    jobscript_set_variable (ji,"DRQUEUE_PRE",precommand);
  }

  jobscript_template_write (ji,"maya.sg");
  jobscript_close (ji);

  return filename;
}


char *mayasg_default_script_path (void) {
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



