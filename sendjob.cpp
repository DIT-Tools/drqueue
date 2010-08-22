//
// Copyright (C) 2001,2002,2003,2004,2005,2006,2007 Jorge Daza Garcia-Blanes
//
// This file is part of DrQueue
//
// DrQueue is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// DrQueue is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
// USA
//
// To set up a maya job from a file and send it to the master
//

#include <fstream>
#include <iostream>
#include <string>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#if defined (__IRIX)
#include <sys/types.h>
#else
#include <stdint.h>
#endif

#include "sendjob.h"
#include "libdrqueue.h"

void usage (void);

int main (int argc,char *argv[]) {
  int opt;
  int toj = 0;
  char* frame_range;
  int fs, fe, sf;
  int nRet = 0;
  std::ifstream infile;
  
  
  if(network_initialize() != 0) {
    fprintf (stderr,"Could not initialize the network: %s\n", drerrno_str());
    nRet = 1;
    goto cleanup;
  }

  presentation();
  fs = 1;
  fe = 1;
  sf = 1;

  while ((opt = getopt (argc,argv,"df:hvt:")) != -1) {
    switch (opt) {
    case 'd':
      debug = true;
      continue;
    case 'v':
      show_version();
      exit (0);
    case 't':
      // DEBUG:
      if(debug) { std::cerr << "evaluating render type argument.\n"; }
      toj = str2toj (optarg);
      break;
    case 'f':
      // DEBUG:
      if(debug) { std::cerr << "evaluating frame argument.\n"; }
      // FIXME: make second and third frame number optional
      // count ':' and quit if not enough
      frame_range = (optarg);
      fs = atoi(strtok(frame_range,":"));
      fe = atoi(strtok(NULL,":"));
      sf = atoi(strtok(NULL,"\0"));
      break;
    case '?':
    case 'h':
      usage();
      nRet = 1;
      goto cleanup;
    }
  }

  if ((argc < 3) || (toj == TOJ_NONE)) {
    usage ();
    nRet = 1;
    goto cleanup;
  }
  
  // DEBUG:  
  if(debug) { std::cerr << "set_default_env().\n"; }
  set_default_env();

  if (!common_environment_check()) {
    std::cerr << "Error checking the environment: " << drerrno_str() << std::endl;
    nRet = 1;
    goto cleanup;
  }

  infile.open(argv[argc-1]);
  switch (toj) {
  case TOJ_GENERAL:
    if (RegisterGeneralJob (argv[argc-1], fs, fe, sf)) {
      std::cerr << "Error registering GENERAL job: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_MAYA:
    // DEBUG:  
    if(debug) { std::cerr << "entering RegisterMayaJobFromFile().\n"; }
  
    if (RegisterMayaJobFromFile (infile)) {
      std::cerr << "Error registering MAYA job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_MENTALRAY:
    if (RegisterMentalrayJobFromFile (infile)) {
      std::cerr << "Error registering MENTALRAY job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_BLENDER:
    if (RegisterBlenderJobFromFile (infile)) {
      std::cerr << "Error registering BLENDER job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_THREEDELIGHT:
    if (RegisterThreedelightJobFromFile (infile)) {
      std::cerr << "Error registering 3Delight job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_PIXIE:
    if (RegisterPixieJobFromFile (infile)) {
      std::cerr << "Error registering PIXIE job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_LIGHTWAVE:
    if (RegisterLightwaveJobFromFile (infile)) {
      std::cerr << "Error registering Lightwave job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_AFTEREFFECTS:
    if (RegisterAftereffectsJobFromFile (infile)) {
      std::cerr << "Error registering After Effects job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_SHAKE:
    if (RegisterShakeJobFromFile (infile)) {
      std::cerr << "Error registering Shake job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_AQSIS:
    if (RegisterAqsisJobFromFile (infile)) {
      std::cerr << "Error registering Aqsis job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_MANTRA:
    if (RegisterMantraJobFromFile (infile)) {
      std::cerr << "Error registering Mantra job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_TERRAGEN:
    if (RegisterTerragenJobFromFile (infile)) {
      std::cerr << "Error registering Terragen job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_NUKE:
    if (RegisterNukeJobFromFile (infile)) {
      std::cerr << "Error registering Nuke job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
    break;
  case TOJ_XSI:
    if (RegisterXSIJobFromFile (infile)) {
      std::cerr << "Error registering XSI job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
  case TOJ_LUXRENDER:
    if (RegisterLuxrenderJobFromFile (infile)) {
      std::cerr << "Error registering LUXRENDER job from file: " << argv[argc-1] << std::endl;
      nRet = 1;
      goto cleanup;
    }
  }

  std::cerr << "Job sent successfuly to the queue\n";

cleanup:
  network_shutdown();

  return nRet;
}

void presentation (void) {
  std::cout << "DrQueue - by Jorge Daza Garcia-Blanes\n\n";
}

void show_version () {
  std::cout << "sendjob - version 0.64.x\n\n";
}

void usage (void) {
  std::cerr << "About: sendjob generates a render job from a description file. See sendjob_maya_example.txt for an example.\n\n"; 
  std::cerr << "Usage: sendjob [-vdh] [-f frameStart[:frameEnd[:stepFrame]]] -t <type> <job_file>\n"
  << "Valid options:\n"
  << "\t-v version information\n"
  << "\t-d enable debug messages\n"
  << "\t-h prints this help\n"
  << "\t-f [frameStart[:frameEnd[:stepFrame]]]\n"
  << "\t-t [general|maya|blender|mentalray|aqsis|3delight|pixie|lightwave|terragen|nuke|aftereffects|shake|xsi|luxrender] type of job\n";
}


/*
    unlike the other Register functions, General does not parse the infile.  It will execute it instead.
*/
int RegisterGeneralJob (char* infile, int frameStart, int frameEnd, int frameStep) {
  // Job variables for the script generator
  struct job job;
  struct generalsgi generalSgi;

  uid_t uid;
  gid_t gid;
  char *scriptdir;
  std::string owner;
  char *pathToScript;
  char *tmpPath;

  job_init (&job);

  uid = getuid();
  gid = getgid();
  // scriptDir = get_current_dir_name();
  scriptdir = NULL;
  getcwd(scriptdir, BUFFERLEN);
  strncpy(generalSgi.script,infile,BUFFERLEN-1);
  // strncpy(generalSgi.scriptDir,scriptDir.c_str(),BUFFERLEN-1);
  //generalSgi.scriptdir = (char *)scriptdir;
  generalSgi.uid_owner = uid;
  generalSgi.gid_owner = gid;

  // Set where the tmp files will get stored
  if ((tmpPath = getenv("DRQUEUE_TMP"))) {
    snprintf(generalSgi.scriptdir,BUFFERLEN,"%s",tmpPath);
  } else {
    snprintf(generalSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));
  }

  // make the temporary executable file which drqueue will ultimately run.
  if (!(pathToScript = generalsg_create(&generalSgi))) {
    std::cerr << "Error creating general script file\n";
    return 1;
  }

  job.status = JOBSTATUS_WAITING;
  job.frame_info.ptr = NULL;
  job.envvars.variables.ptr = NULL;
  job.envvars.nvariables = 0;
  job.envvars.evshmid = -1;
  job.flags = 0;
  strncpy(job.name,infile,BUFFERLEN-1);
  owner = getlogin();
  strncpy(job.owner,owner.c_str(),BUFFERLEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  //strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  // strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.block_size = 1;
  job.priority = 500;

  job.koj = KOJ_GENERAL;


  // strncpy (job.koji.general.scriptDir,scriptDir,BUFFERLEN);
  strncpy(job.koji.general.scriptdir,scriptdir,PATH_MAX);
  // job.koji.general.scriptDir = scriptDir;

  // farm properties
  //   (I'll need to create a flag for setting all these)

  // job.limits.os_flags = OSF_ALL;
  // job.limits.os_flags = OSF_OSX;
  // job.limits.os_flags = OSF_FREEBSD;

  // Not exactly sure how to set this yet but This sets every platform except for windows (which sucks anyway :).
  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  /*
   fprintf(stderr,"DEBUG: ----\n");
   fprintf(stderr,"DEBUG: user_id: %d\n",uid);
   fprintf(stderr,"DEBUG: group_id: %d\n",gid);
   fprintf(stderr,"DEBUG: script: %s\n",generalSgi.script);
   fprintf(stderr,"DEBUG: scriptDir: %s\n",generalSgi.scriptDir);
   fprintf(stderr,"DEBUG: frame_start: %d\n",job.frame_start);
   fprintf(stderr,"DEBUG:   frame_end: %d\n",job.frame_end);
   fprintf(stderr,"DEBUG:  frame_step: %d\n",job.frame_step);
   fprintf(stderr,"DEBUG: ----\n");
  */

  if (!register_job(&job)) {
    std::cerr << "Error sending General job to the queue\n";
    return 1;
  }
  std::cerr << "General Job registered successfuly.\n";
  return 0;
}

int RegisterMayaJobFromFile (std::ifstream &infile) {
  
  // DEBUG:  
  if(debug) { std::cerr << "RegisterMayaJobFromFile().\n"; }
  
  // Job variables for the script generator
  struct job job;
  struct mayasgi mayaSgi;

  std::string owner;
  std::string jobName;
  std::string camera;
  int frameStart,frameEnd,frameStep, renderer;
  int resX,resY;
  std::string scenePath;
  std::string renderDir;
  std::string projectDir;
  std::string fileFormat;
  std::string image;
  char *pathToScript;

  job_init (&job);

  getline(infile,owner);
  getline(infile,jobName);
  getline(infile,camera);
  infile >> renderer;
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  infile >> resX;
  infile >> resY;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n
  getline(infile,renderDir);
  getline(infile,projectDir);
  getline(infile,fileFormat);
  getline(infile,image);

  mayaSgi.renderer = renderer;
  strncpy(mayaSgi.renderdir,renderDir.c_str(),BUFFERLEN-1);
  strncpy(mayaSgi.projectdir,projectDir.c_str(),BUFFERLEN-1);
  strncpy(mayaSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy(mayaSgi.image,image.c_str(),BUFFERLEN-1);
  snprintf(mayaSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));
  strncpy(mayaSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(mayaSgi.camera,camera.c_str(),BUFFERLEN-1);
  mayaSgi.res_x = resX;
  mayaSgi.res_y = resY;
  strncpy(mayaSgi.format,fileFormat.c_str(),BUFFERLEN-1);
  
  // DEBUG:
  if(debug) {
    std::cerr << "\nFetched job variables:\nrenderer: ";
    std::cerr << mayaSgi.renderer;
    std::cerr << "\nrenderdir: ";
    std::cerr << mayaSgi.renderdir;
    std::cerr << "\nprojectdir: ";
    std::cerr << mayaSgi.projectdir;
    std::cerr << "\nscene: ";
    std::cerr << mayaSgi.scene;
    std::cerr << "\nrenderdir: ";
    std::cerr << mayaSgi.image;
    std::cerr << "\nscriptdir: ";
    std::cerr << mayaSgi.scriptdir;
    std::cerr << "\nfile_owner: ";
    std::cerr << mayaSgi.file_owner;
    std::cerr << "\ncamera: ";
    std::cerr << mayaSgi.camera;
    std::cerr << "\nres_x: ";
    std::cerr << mayaSgi.res_x;
    std::cerr << "\nres_y: ";
    std::cerr << mayaSgi.res_y;
    std::cerr << "\nformat: ";
    std::cerr << mayaSgi.format;
    std::cerr << "\n\n";
  }
  
  if (!(pathToScript = mayasg_create(&mayaSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);

  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.frame_pad = 4;

  job.koj = KOJ_MAYA;
  strncpy (job.koji.maya.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.maya.renderdir,renderDir.c_str(),BUFFERLEN-1);
  strncpy (job.koji.maya.projectdir,projectDir.c_str(),BUFFERLEN-1);
  strncpy (job.koji.maya.image,"",BUFFERLEN-1);
  strncpy (job.koji.maya.viewcmd,"",BUFFERLEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterMentalrayJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct mentalraysgi mentalraySgi;

  std::string owner;
  std::string jobName;
  std::string camera;
  int frameStart,frameEnd,frameStep;
  int resX,resY;
  std::string scenePath;
  std::string renderDir;
  std::string fileFormat;
  std::string image;
  char *pathToScript;

  job_init(&job);

  getline(infile,owner);
  getline(infile,jobName);
  getline(infile,camera);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  infile >> resX;
  infile >> resY;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n
  getline(infile,renderDir);
  getline(infile,fileFormat);
  getline(infile,image);

  strncpy(mentalraySgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(mentalraySgi.camera,camera.c_str(),BUFFERLEN-1);
  mentalraySgi.res_x = resX;
  mentalraySgi.res_y = resY;
  strncpy(mentalraySgi.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy(mentalraySgi.renderdir,renderDir.c_str(),BUFFERLEN-1);
  strncpy(mentalraySgi.format,fileFormat.c_str(),BUFFERLEN-1);
  snprintf(mentalraySgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));
  strncpy(mentalraySgi.image,image.c_str(),BUFFERLEN-1);

  job.autoRequeue = 1;

  if (!(pathToScript = mentalraysg_create(&mentalraySgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_MENTALRAY;
  strncpy (job.koji.mentalray.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.mentalray.renderdir,renderDir.c_str(),BUFFERLEN-1);
  strncpy (job.koji.mentalray.image,"",BUFFERLEN-1);
  strncpy (job.koji.mentalray.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterBlenderJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct blendersgi blenderSgi;

  std::string jobName;
  int frameStart,frameEnd,frameStep,render_type;
  std::string scenePath;
  char *pathToScript;

  job_init(&job);

  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  infile >> render_type;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n

  strncpy(blenderSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  snprintf(blenderSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));
  blenderSgi.render_type = render_type;	
  job.autoRequeue = 1;

  if (!(pathToScript = blendersg_create(&blenderSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,getenv("USER"),MAXNAMELEN-1);
  strncpy (job.email,getenv("USER"),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.block_size = frameStep;
  job.priority = 500;

  job.koj = KOJ_BLENDER;
  strncpy (job.koji.blender.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.blender.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterLightwaveJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct lightwavesgi lightwaveSgi;

  std::string owner;
  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  std::string configDir;
  std::string projectDir;
  char *pathToScript;

  job_init(&job);

  getline(infile,owner);
  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n
  getline(infile,configDir);
  getline(infile,projectDir);

  //  strncpy(lightwaveSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(lightwaveSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy(lightwaveSgi.configdir,configDir.c_str(),BUFFERLEN-1);
  strncpy(lightwaveSgi.projectdir,projectDir.c_str(),BUFFERLEN-1);
  snprintf(lightwaveSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));

  job.autoRequeue = 1;

  if (!(pathToScript = lightwavesg_create(&lightwaveSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_LIGHTWAVE;
  strncpy (job.koji.lightwave.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.lightwave.configdir,configDir.c_str(),BUFFERLEN-1);
  strncpy (job.koji.lightwave.projectdir,projectDir.c_str(),BUFFERLEN-1);
  strncpy (job.koji.lightwave.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterTerragenJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct terragensgi terragenSgi;

  std::string owner;
  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  std::string worldfile;
  std::string terrainfile;
  char *pathToScript;

  job_init(&job);

  getline(infile,owner);
  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n
  getline(infile,worldfile);
  getline(infile,terrainfile);

  strncpy(terragenSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(terragenSgi.scriptfile,scenePath.c_str(),BUFFERLEN-1);
  strncpy(terragenSgi.worldfile,worldfile.c_str(),BUFFERLEN-1);
  strncpy(terragenSgi.terrainfile,terrainfile.c_str(),BUFFERLEN-1);
  snprintf(terragenSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));

  job.autoRequeue = 1;

  if (!(pathToScript = terragensg_create(&terragenSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_TERRAGEN;
  strncpy (job.koji.terragen.scriptfile,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.terragen.worldfile,worldfile.c_str(),BUFFERLEN-1);
  strncpy (job.koji.terragen.terrainfile,terrainfile.c_str(),BUFFERLEN-1);
  strncpy (job.koji.terragen.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterShakeJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct shakesgi shakeSgi;

  std::string owner;
  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  char *pathToScript;

  job_init(&job);

  getline(infile,owner);
  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n

  //  strncpy(shakeSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(shakeSgi.script,scenePath.c_str(),BUFFERLEN-1);
  snprintf(shakeSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));

  job.autoRequeue = 1;

  if (!(pathToScript = shakesg_create(&shakeSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_SHAKE;
  strncpy (job.koji.shake.script,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.shake.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterNukeJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct nukesgi nukeSgi;

  std::string owner;
  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  char *pathToScript;

  job_init(&job);

  getline(infile,owner);
  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n

  //  strncpy(nukeSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(nukeSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  snprintf(nukeSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));

  job.autoRequeue = 1;

  if (!(pathToScript = nukesg_create(&nukeSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_NUKE;
  strncpy (job.koji.nuke.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.nuke.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterThreedelightJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct threedelightsgi threedelightSgi;

  std::string owner;
  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  char *pathToScript;

  job_init(&job);

  getline(infile,owner);
  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n

  strncpy(threedelightSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(threedelightSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  snprintf(threedelightSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));

  job.autoRequeue = 1;

  if (!(pathToScript = threedelightsg_create(&threedelightSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_3DELIGHT;
  strncpy (job.koji.threedelight.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.threedelight.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterPixieJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct pixiesgi pixieSgi;

  std::string owner;
  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  char *pathToScript;

  job_init(&job);
  
  getline(infile,owner);
  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n

  //  strncpy(pixieSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(pixieSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  snprintf(pixieSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));
  job.autoRequeue = 1;

  if (!(pathToScript = pixiesg_create(&pixieSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_PIXIE;
  strncpy (job.koji.pixie.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.pixie.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterAftereffectsJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct aftereffectssgi aftereffectsSgi;

  std::string owner;
  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  std::string comp;
  char *pathToScript;

  job_init(&job);

  getline(infile,owner);
  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n
  getline(infile,comp);

  //  strncpy(aftereffectsSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(aftereffectsSgi.project,scenePath.c_str(),BUFFERLEN-1);
  strncpy(aftereffectsSgi.comp,comp.c_str(),BUFFERLEN-1);
  snprintf(aftereffectsSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));

  job.autoRequeue = 1;

  if (!(pathToScript = aftereffectssg_create(&aftereffectsSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_AFTEREFFECTS;
  strncpy (job.koji.aftereffects.project,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.aftereffects.comp,comp.c_str(),BUFFERLEN-1);
  strncpy (job.koji.aftereffects.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterAqsisJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct aqsissgi aqsisSgi;

  std::string owner;
  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  char *pathToScript;

  job_init(&job);

  getline(infile,owner);
  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n

  //  strncpy(aqsisSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(aqsisSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  snprintf(aqsisSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));

  job.autoRequeue = 1;

  if (!(pathToScript = aqsissg_create(&aqsisSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_AQSIS;
  strncpy (job.koji.aqsis.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.aqsis.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterMantraJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct mantrasgi mantraSgi;

  std::string owner;
  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  char *pathToScript;

  job_init(&job);

  getline(infile,owner);
  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n

  //  strncpy(aqsisSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  strncpy(mantraSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  snprintf(mantraSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));

  if (!(pathToScript = mantrasg_create(&mantraSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.priority = 500;

  job.koj = KOJ_MANTRA;
  strncpy (job.koji.mantra.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.mantra.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterXSIJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct xsisgi xsiSgi;

  job_init(&job);

  std::string owner;
  std::string jobName;
  std::string xsiDir;
  int priority, maxcpus;
  int frameStart,frameEnd,framePad,frameStep,blockSize;
  int resX,resY;
  int skipFrames;
  std::string scenePath;
  std::string pass;
  std::string renderDir;
  std::string image;
  std::string imageExt;
  char *pathToScript;
  int runScript;
  std::string scriptRun;

  getline(infile,owner);
  getline(infile,jobName);
  getline(infile,xsiDir); //the xsi version directory i.e. XSI_5.1
  infile >> priority;
  infile >> maxcpus;
  infile >> frameStart;
  infile >> frameEnd;
  infile >> framePad;
  infile >> frameStep;
  infile >> blockSize;
  infile >> resX;
  infile >> resY;
  infile >> skipFrames;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n
  getline(infile,pass);
  getline(infile,renderDir);
  getline(infile,image);
  getline(infile,imageExt);
  infile >> runScript;
  if (runScript) {
    getline(infile,scriptRun); //
    getline(infile,scriptRun); // Get two times because '>>' leaves the pointer before \n
  }

  strncpy(xsiSgi.file_owner,owner.c_str(),BUFFERLEN-1);
  xsiSgi.res_x = resX;
  xsiSgi.res_y = resY;
  xsiSgi.skipFrames = skipFrames;
  strncpy(xsiSgi.xsiDir,xsiDir.c_str(),BUFFERLEN-1);
  strncpy(xsiSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy(xsiSgi.xsipass,pass.c_str(),BUFFERLEN-1);
  strncpy(xsiSgi.renderdir,renderDir.c_str(),BUFFERLEN-1);
  snprintf(xsiSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));
  strncpy(xsiSgi.image,image.c_str(),BUFFERLEN-1);
  strncpy(xsiSgi.imageExt,imageExt.c_str(),BUFFERLEN-1);
  xsiSgi.runScript = runScript;
  strncpy(xsiSgi.scriptRun,scriptRun.c_str(),BUFFERLEN-1);

  if (!(pathToScript = xsisg_create(&xsiSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  //script file is created - now let's build the job and submit it.
  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,owner.c_str(),MAXNAMELEN-1);
  strncpy (job.email,owner.c_str(),MAXNAMELEN-1);

  job.autoRequeue = 1;
  job.status = JOBSTATUS_WAITING;
  job.frame_info.ptr = NULL;

  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_pad = framePad;
  job.frame_step = frameStep;
  job.block_size = blockSize;
  job.priority = priority;

  job.koj = KOJ_XSI;
  strncpy (job.koji.xsi.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.xsi.xsipass,pass.c_str(),BUFFERLEN-1);
  strncpy (job.koji.xsi.renderdir,renderDir.c_str(),BUFFERLEN-1);
  strncpy (job.koji.xsi.image,image.c_str(),BUFFERLEN-1);
  strncpy (job.koji.xsi.imageExt,imageExt.c_str(),BUFFERLEN-1);
  strncpy (job.koji.xsi.viewcmd,"$DRQUEUE_BIN/viewcmd/imf_batch",BUFFERLEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int RegisterLuxrenderJobFromFile (std::ifstream &infile) {
  // Job variables for the script generator
  struct job job;
  struct luxrendersgi luxrenderSgi;

  std::string jobName;
  int frameStart,frameEnd,frameStep;
  std::string scenePath;
  char *pathToScript;

  job_init(&job);

  getline(infile,jobName);
  infile >> frameStart;
  infile >> frameEnd;
  infile >> frameStep;
  getline(infile,scenePath); //
  getline(infile,scenePath); // Get two times because '>>' leaves the pointer before \n

  strncpy(luxrenderSgi.scene,scenePath.c_str(),BUFFERLEN-1);
  snprintf(luxrenderSgi.scriptdir,BUFFERLEN,"%s/tmp/",getenv("DRQUEUE_ROOT"));

  job.autoRequeue = 1;

  if (!(pathToScript = luxrendersg_create(&luxrenderSgi))) {
    std::cerr << "Error creating script file\n";
    return 1;
  }

  strncpy (job.name,jobName.c_str(),MAXNAMELEN-1);
  strncpy (job.cmd,pathToScript,MAXCMDLEN-1);
  strncpy (job.owner,getenv("USER"),MAXNAMELEN-1);
  strncpy (job.email,getenv("USER"),MAXNAMELEN-1);
  job.frame_start = frameStart;
  job.frame_end = frameEnd;
  job.frame_step = frameStep;
  job.block_size = frameStep;
  job.priority = 500;

  job.koj = KOJ_LUXRENDER;
  strncpy (job.koji.luxrender.scene,scenePath.c_str(),BUFFERLEN-1);
  strncpy (job.koji.luxrender.viewcmd,"",BUFFERLEN-1);

  job.limits.os_flags = OSF_ALL;
  job.limits.nmaxcpus = (uint16_t)-1;
  job.limits.nmaxcpuscomputer = (uint16_t)-1;
  job.limits.memory = 0;
  strncpy (job.limits.pool,"Default",MAXNAMELEN-1);

  if (!register_job(&job)) {
    std::cerr << "Error sending job to the queue\n";
    return 1;
  }

  return 0;
}

int str2toj (char *str) {
  int toj = TOJ_NONE;

  if (strstr(str,"general") != NULL) {
    toj = TOJ_GENERAL;
  } else if (strstr(str,"maya") != NULL) {
    toj = TOJ_MAYA;
  } else if (strstr(str,"mentalray") != NULL) {
    toj = TOJ_MENTALRAY;
  } else if (strstr(str,"blender") != NULL) {
    toj = TOJ_BLENDER;
  } else if (strstr(str,"3delight") != NULL) {
    toj = TOJ_THREEDELIGHT;
  } else if (strstr(str,"aqsis") != NULL) {
    toj = TOJ_AQSIS;
  } else if (strstr(str,"mantra") != NULL) {
    toj = TOJ_MANTRA;
  } else if (strstr(str,"lightwave") != NULL) {
    toj = TOJ_LIGHTWAVE;
  } else if (strstr(str,"pixie") != NULL) {
    toj = TOJ_PIXIE;
  } else if (strstr(str,"aftereffects") != NULL) {
    toj = TOJ_AFTEREFFECTS;
  } else if (strstr(str,"shake") != NULL) {
    toj = TOJ_SHAKE;
  } else if (strstr(str,"nuke") != NULL) {
    toj = TOJ_NUKE;
  } else if (strstr(str,"terragen") != NULL) {
    toj = TOJ_TERRAGEN;
  } else if (strstr(str,"xsi") != NULL) {
    toj = TOJ_XSI;
  } else if (strstr(str,"luxrender") != NULL) {
    toj = TOJ_LUXRENDER;
  }

  return toj;
}
