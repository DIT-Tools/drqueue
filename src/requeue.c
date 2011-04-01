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

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "libdrqueue.h"

void usage (void);

int main (int argc,char *argv[]) {
  int opt;
  int frame = -1;
  int ijob = -1;

  while ((opt = getopt (argc,argv,"j:f:vh")) != -1) {
    switch (opt) {
    case 'f':
      frame = atoi (optarg);
      break;
    case 'j':
      ijob = atoi (optarg);
      break;
    case 'v':
      show_version (argv);
      exit (0);
    case '?':
    case 'h':
      usage();
      exit (1);
    }
  }

  if ((frame == -1) || (ijob == -1)) {
    usage ();
    exit (1);
  }

  set_default_env();

  if (!common_environment_check()) {
    fprintf (stderr,"Error checking the environment: %s\n",drerrno_str());
    exit (1);
  }

  if (! request_job_frame_waiting ((uint32_t)ijob,(uint32_t)frame,CLIENT)) {
    fprintf (stderr,"ERROR: While trying to requeue: %s\n",drerrno_str());
    exit (1);
  }

  printf ("Frame requeued successfully\n");

  exit (0);
}

void usage (void) {
  fprintf (stderr,"Usage: requeue [-vh] -f <frame_number> -j <job_id>\n"
           "Valid options:\n"
           "\t-f <frame_number>\n"
           "\t-j <job_id>\n"
           "\t-v print version\n"
           "\t-h print this help\n");
}
