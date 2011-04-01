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

#include <stdio.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include "libdrqueue.h"

#define STATUS_FINISHED 0
#define STATUS_ERROR 1
#define STATUS_REQUE 2
#define STATUS_NONE  3

void usage (void);

int main (int argc,char *argv[]) {

  int opt;
  uint32_t frame = -1;
  uint32_t ijob = -1;
  int status = STATUS_NONE;
  int nRet = 0;
  
  if(network_initialize() != 0) {
    fprintf (stderr,"Could not initialize the network: %s\n", drerrno_str());
    nRet = 1;
    goto cleanup;
  }

  while ((opt = getopt (argc,argv,"j:f:dervh")) != -1) {
    switch (opt) {
    case 'f':
      frame = atoi (optarg);
      break;
    case 'j':
      ijob = atoi (optarg);
      break;
    case 'r':
      status = STATUS_REQUE;
      break;
    case 'd':
      status = STATUS_FINISHED;
      break;
    case 'e':
      status = STATUS_ERROR;
      break;
    case 'v':
      show_version (argv);
      goto cleanup;
    case '?':
    case 'h':
      usage();
      nRet = 1;
      goto cleanup;
    }
  }

  if ((ijob == -1) || (status == STATUS_NONE)) {
    usage ();
    nRet = 1;
    goto cleanup;
  }

  set_default_env();

  if (!common_environment_check()) {
    fprintf (stderr,"Error checking the environment: %s\n",drerrno_str());
    nRet = 1;
    goto cleanup;
  }

  switch (status) {
  case STATUS_FINISHED:
    printf ("Setting frame finished: %i,%i\n",frame,ijob);
    if (! request_job_frame_finish (ijob,frame,CLIENT)) {
      fprintf (stderr,"ERROR: While trying to set finished: %s\n",drerrno_str());
      nRet = 1;
      goto cleanup;
    }
    printf ("Frame set finished successfully\n");
    break;
  case STATUS_ERROR:
    printf ("Setting frame to error not yet implemented: %i,%i\n",frame,ijob);
    break;
  case STATUS_REQUE:
    printf ("Requeueing frame: %i,%i\n",frame,ijob);
    if (! request_job_frame_waiting (ijob,frame,CLIENT)) {
      fprintf (stderr,"ERROR: While trying to requeue: %s\n",drerrno_str());
      nRet = 1;
      goto cleanup;
    }
    printf ("Frame requeued successfully\n");
    break;
  }

cleanup:
  network_shutdown();

  return nRet;
}

void usage (void) {
  fprintf (stderr,"Usage: ctask [-vh] -[d|r] -j <job_id> -f <frame>\n"
           "Valid options:\n"
           "\t-d will set to finished\n"
           "\t-r will requeue\n"
           "\t-j <job_id>\n"
           "\t-f <frame>\n"
           "\t-v print version\n"
           "\t-h print this help \n");
}
