//
// Copyright (C) 2001,2002,2003,2004,2005,2006 Jorge Daza Garcia-Blanes
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
// $Id$
//

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include "libdrqueue.h"
#include "config.h"
#include "list.h"
#include "constants.h"

void config_close (FILE *f_cfg);

void config_get_default_file (char *dst,char *tool,int dstlen) {
  snprintf (dst,dstlen,"%s/%s.%s",BASE_CONF_PATH,tool,BASE_CONF_EXT);
}

void config_parse_tool (char *tool) {
  char cfg_path[PATH_MAX];

  // Hardcoded
  config_get_default_file (cfg_path,tool,PATH_MAX);
  config_parse (cfg_path);
  if ( getenv ("DRQUEUE_ETC") ) {
    snprintf (cfg_path,PATH_MAX,"%s/%s.%s",getenv("DRQUEUE_ETC"),tool,BASE_CONF_EXT);
    config_parse (cfg_path);
  } else if ( getenv ("DRQUEUE_ROOT") ) {
    snprintf (cfg_path,PATH_MAX,"%s/etc/%s.%s",getenv("DRQUEUE_ROOT"),tool,BASE_CONF_EXT);
    config_parse (cfg_path);
  }
}

void config_parse (char *cfg) {
  FILE *f_conf;
  char buffer[BUFFERLEN];
  char *token;
  char renv[BUFFERLEN], *penv;

  if ((f_conf = fopen (cfg,"r")) == NULL) {
    fprintf (stderr,"Could not open config file: '%s'\n",cfg);
    return;
  }

  printf ("Parsing config at: %s\n",cfg);

  while ((fgets (buffer,BUFFERLEN-1,f_conf)) != NULL) {
    if (buffer[0] == '#') {
      continue;
    }
    token = strtok(buffer,"=\n\r");
    if (strcmp(token,"logs") == 0) {
      if ((token = strtok (NULL,"=\n\r")) != NULL) {
        fprintf (stderr,"Logs on: '%s'\n",token);
        snprintf (renv,BUFFERLEN,"DRQUEUE_LOGS=%s",token);
        if ((penv = (char*) malloc (strlen (renv)+1)) == NULL) {
          fprintf (stderr,"ERROR allocating memory for DRQUEUE_LOGS.\n");
          exit (1);
        }
        strncpy(penv,renv,strlen(renv)+1);
        if (putenv (penv) != 0) {
          fprintf (stderr,"ERROR seting the environment: '%s'\n",penv);
        }
      } else {
        fprintf (stderr,"Warning parsing config file. No value for logs. Using default.\n");
      }
    } else if (strcmp(token,"tmp") == 0) {
      if ((token = strtok (NULL,"=\n\r")) != NULL) {
        fprintf (stderr,"Tmp on: '%s'\n",token);
        snprintf (renv,BUFFERLEN,"DRQUEUE_TMP=%s",token);
        if ((penv = (char*) malloc (strlen(renv)+1)) == NULL) {
          fprintf (stderr,"ERROR allocating memory for DRQUEUE_TMP.\n");
          exit (1);
        }
        strncpy(penv,renv,strlen(renv)+1);
        if (putenv (penv) != 0) {
          fprintf (stderr,"ERROR seting the environment: '%s'\n",penv);
        }
      } else {
        fprintf (stderr,"Warning parsing config file. No value for tmp. Using default.\n");
      }
    } else if (strcmp(token,"bin") == 0) {
      if ((token = strtok (NULL,"=\n\r")) != NULL) {
        fprintf (stderr,"Bin on: '%s'\n",token);
        snprintf (renv,BUFFERLEN,"DRQUEUE_BIN=%s",token);
        if ((penv = (char*) malloc (strlen(renv)+1)) == NULL) {
          fprintf (stderr,"ERROR allocating memory for DRQUEUE_BIN.\n");
          exit (1);
        }
        strncpy(penv,renv,strlen(renv)+1);
        if (putenv (penv) != 0) {
          fprintf (stderr,"ERROR seting the environment: '%s'\n",penv);
        }
      } else {
        fprintf (stderr,"Warning parsing config file. No value for bin. Using default.\n");
      }
    } else if (strcmp(token,"etc") == 0) {
      if ((token = strtok (NULL,"=\n\r")) != NULL) {
        fprintf (stderr,"Etc on: '%s'\n",token);
        snprintf (renv,BUFFERLEN,"DRQUEUE_ETC=%s",token);
        if ((penv = (char*) malloc (strlen(renv)+1)) == NULL) {
          fprintf (stderr,"ERROR allocating memory for DRQUEUE_ETC.\n");
          exit (1);
        }
        strncpy(penv,renv,strlen(renv)+1);
        if (putenv (penv) != 0) {
          fprintf (stderr,"ERROR seting the environment: '%s'\n",penv);
        }
      } else {
        fprintf (stderr,"Warning parsing config file. No value for etc. Using default.\n");
      }
    } else if (strcmp(token,"db") == 0) {
      if ((token = strtok (NULL,"=\n\r")) != NULL) {
        fprintf (stderr,"Db on: '%s'\n",token);
        snprintf (renv,BUFFERLEN,"DRQUEUE_DB=%s",token);
        if ((penv = (char*) malloc (strlen(renv)+1)) == NULL) {
          fprintf (stderr,"ERROR allocating memory for DRQUEUE_DB.\n");
          exit (1);
        }
        strncpy(penv,renv,strlen(renv)+1);
        if (putenv (penv) != 0) {
          fprintf (stderr,"ERROR seting the environment: '%s'\n",penv);
        }
      } else {
        fprintf (stderr,"Warning parsing config file. No value for db. Using default.\n");
      }
    } else if (strcmp(token,"pool") == 0) {
      if ((token = strtok (NULL,"=\n\r")) != NULL) {
        fprintf (stderr,"Pools are: '%s'\n",token);
        snprintf (renv,BUFFERLEN,"DRQUEUE_POOL=%s",token);
        if ((penv = (char*) malloc (strlen(renv)+1)) == NULL) {
          fprintf (stderr,"ERROR allocating memory for DRQUEUE_POOL.\n");
          exit (1);
        }
        strncpy(penv,renv,strlen(renv)+1);
        if (putenv (penv) != 0) {
          fprintf (stderr,"ERROR seting the environment: '%s'\n",penv);
        }
      } else {
        fprintf (stderr,"Warning parsing config file. No value for pool. Using default.\n");
      }
    } else {
      fprintf (stderr,"ERROR parsing config file. Unknown token: '%s'\n",token);
      exit (1);
    }
  }
  
}

FILE *
config_open (char *filename) {
  FILE *f;
  if ( (f = fopen (filename,"r")) != NULL ) {
    return f;
  }
  
  fprintf (stderr,"Could not open config file: %s\n",filename);
  perror ("config_open");

  return NULL;
}

void
config_close (FILE *f_cfg) {
  fclose (f_cfg);
}

int
config_line_ends_node (struct config_node *base, char *line) {
  if ( line[0]=='[' 
       && line[1]=='/'
       && strncmp (base->name,&line[2],strlen(base->name)) == 0 
       && line[strlen(base->name)+2]==']')
    return 1;
  return 0;
}

int config_end_node (struct config_node *base, FILE *file) {
  char line[BUFFERLEN];
  char *res;
  long pos = ftell (file);
  res = fgets (line,BUFFERLEN,file);
  if (( res != NULL) && config_line_ends_node (base,res)) {
    return 1;
  }
  if ( ! res ) {
    fprintf (stderr,"ERROR: End of file reached without having finished node: '%s'\n",base->name);
    return 1;
  }
  fseek (file,pos,SEEK_SET);
  return 0;
}

struct config_item *config_item_new (struct config_node *base, char *line) {
  char *name;
  char *value;
  char *sep;

  sep = strchr (line,'=');
  if ( !sep ) {
    return NULL;
  }
  *sep = '\0';
  value = sep+1;
  name = line;
  printf ("Name: '%s'\nValue: '%s'\n",name,value);
  return NULL;
}

void config_read_item (struct config_node *base, FILE *file) {
  char line[BUFFERLEN];
  char *res;
  res = fgets (line,BUFFERLEN,file);
  if ( res )
    config_item_new (base,res);
}

void config_add_node_items (struct config_node *base, FILE *file) {
  while ( ! config_end_node (base,file) ) {
    config_read_item (base,file);
  }
}

void config_add_node (struct config_node *base, FILE *file, char *line) {
  struct config_node *node = config_node_new (base,"Temp Name","No description",file);
  int len = (strlen(line)-3 > CFG_ITEM_NAME_LEN) ? CFG_ITEM_NAME_LEN : strlen(line)-3; 
  strncpy (node->name,&line[1],len);
  printf ("Adding node: '%s'\n",node->name);
  config_add_node_items (node,file);
  //config_add_node_children (node,file);
}

int config_find_nodes (struct config_node *base,FILE *file) {
  char line[BUFFERLEN];
  char *res;
  while ( (res = fgets (line,BUFFERLEN,file)) != NULL ) {
    if ((line[0]=='[') && line[strlen(line)-2]==']')
      config_add_node (base,file,line);
  }
  return 0;
}

struct config_node *
config_node_new (struct config_node *parent, char *name, char *desc, FILE *file) {
  struct config_node *config = (struct config_node *) malloc (sizeof (struct config_node));

  if (name) {
    snprintf(config->name,CFG_ITEM_NAME_LEN,"%s",name);
  } else {
    fprintf (stderr,"ERROR: Config Node needs a name\n");
    return NULL;
  }

  if (desc) {
    snprintf(config->value,CFG_ITEM_VALUE_LEN,"%s",desc);
  }

  config->item_list = list_new (NULL);
  config->node_list = list_new (NULL);
  config->filepos = ftell (file);
  config->parent = parent;
  if (parent) {
    list_add (parent->node_list,config);
  }

  return config;
}

struct config_node *
config_read (char *filename,char *tool) {

  // Fuction to be called for parsing "filename" config related to "tool"
  FILE *cfg_file = config_open (filename);

  if ( cfg_file == NULL ) {
    fprintf (stderr,"ERROR: Could not open file: '%s'\n",filename);
    return NULL;
  }

  struct config_node *config = config_node_new (NULL,tool,"Root config node",cfg_file);

  config_find_nodes (config,cfg_file);

  return config;
}
