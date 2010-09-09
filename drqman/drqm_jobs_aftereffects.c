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

#ifdef HAVE_CONFIG_H
# include <lconfig.h>
#endif

#include "drqm_jobs.h"
#include "drqm_common.h"
#include "drqm_jobs_aftereffects.h"

static void dnj_koj_frame_aftereffects_project_search (GtkWidget *button, struct drqmj_koji_aftereffects *info);
static void dnj_koj_frame_aftereffects_project_set (GtkWidget *button, struct drqmj_koji_aftereffects *info);
static void dnj_koj_frame_aftereffects_script_search (GtkWidget *button, struct drqmj_koji_aftereffects *info);
static void dnj_koj_frame_aftereffects_script_set (GtkWidget *button, struct drqmj_koji_aftereffects *info);
static void dnj_koj_frame_aftereffects_bcreate_pressed (GtkWidget *button, struct drqmj_dnji *info);

GtkWidget *
dnj_koj_frame_aftereffects (struct drqm_jobs_info *info) {
  GtkWidget *frame;
  GtkWidget *vbox;
  GtkWidget *hbox,*hbox2;
  GtkWidget *label;
  GtkWidget *entry;
  GtkWidget *button;
  GtkWidget *bbox;
  GtkTooltips *tooltips;

  tooltips = TooltipsNew ();

  /* Frame */
  frame = gtk_frame_new ("After Effects job information");

  /* Main vbox */
  vbox = gtk_vbox_new (FALSE,2);
  gtk_container_add (GTK_CONTAINER(frame),vbox);

  /* Project file */
  hbox = gtk_hbox_new (TRUE,2);
  gtk_box_pack_start (GTK_BOX(vbox),hbox,FALSE,FALSE,2);
  label = gtk_label_new ("Project file:");
  gtk_box_pack_start (GTK_BOX(hbox),label,TRUE,TRUE,2);
  hbox2 = gtk_hbox_new (FALSE,0);
  gtk_box_pack_start (GTK_BOX(hbox),hbox2,TRUE,TRUE,0);
  entry = gtk_entry_new_with_max_length (BUFFERLEN-1);
  info->dnj.koji_aftereffects.eproject = entry;
  gtk_tooltips_set_tip(tooltips,entry,"File name of the After Effects project file that should be rendered",NULL);
  gtk_box_pack_start (GTK_BOX(hbox2),entry,TRUE,TRUE,2);
  button = gtk_button_new_with_label ("Search");
  gtk_tooltips_set_tip(tooltips,button,"File selector for the After Effects project file",NULL);
  gtk_box_pack_start (GTK_BOX(hbox2),button,FALSE,FALSE,2);
  g_signal_connect (G_OBJECT(button),"clicked",
                    G_CALLBACK(dnj_koj_frame_aftereffects_project_search),&info->dnj.koji_aftereffects);

  /* Name of composition */
  hbox = gtk_hbox_new (TRUE,2);
  gtk_box_pack_start (GTK_BOX(vbox),hbox,FALSE,FALSE,2);
  label = gtk_label_new ("Comp:");
  gtk_box_pack_start (GTK_BOX(hbox),label,FALSE,FALSE,2);
  entry = gtk_entry_new_with_max_length (BUFFERLEN-1);
  gtk_tooltips_set_tip(tooltips,entry,"Comp to be rendered",NULL);
  info->dnj.koji_aftereffects.ecomp = entry;
  gtk_box_pack_start (GTK_BOX(hbox),entry,TRUE,TRUE,2);

  /* View command */
  hbox = gtk_hbox_new (TRUE,2);
  gtk_box_pack_start (GTK_BOX(vbox),hbox,FALSE,FALSE,2);
  label = gtk_label_new ("View command:");
  gtk_box_pack_start (GTK_BOX(hbox),label,FALSE,FALSE,2);
  entry = gtk_entry_new_with_max_length (BUFFERLEN-1);
  gtk_tooltips_set_tip(tooltips,entry,"Command that will be executed when you select 'Watch image' "
                       "in the frames list (inside the detailed job view)",NULL);
  info->dnj.koji_aftereffects.eviewcmd = entry;
  gtk_entry_set_text(GTK_ENTRY(entry),KOJ_AFTEREFFECTS_DFLT_VIEWCMD);
  gtk_box_pack_start (GTK_BOX(hbox),entry,TRUE,TRUE,2);

  /* Script directory */
  hbox = gtk_hbox_new (TRUE,2);
  gtk_box_pack_start (GTK_BOX(vbox),hbox,FALSE,FALSE,2);
  label = gtk_label_new ("Script directory:");
  gtk_box_pack_start (GTK_BOX(hbox),label,TRUE,TRUE,2);
  hbox2 = gtk_hbox_new (FALSE,0);
  gtk_box_pack_start (GTK_BOX(hbox),hbox2,TRUE,TRUE,0);
  entry = gtk_entry_new_with_max_length (BUFFERLEN-1);
  gtk_tooltips_set_tip(tooltips,entry,"Directory in which, in case of using the automatic "
                       "script generator, the command script will be saved.",NULL);
  info->dnj.koji_aftereffects.escript = entry;
  gtk_entry_set_text (GTK_ENTRY(entry),aftereffectssg_default_script_path());
  gtk_box_pack_start (GTK_BOX(hbox2),entry,TRUE,TRUE,2);
  button = gtk_button_new_with_label ("Search");
  gtk_tooltips_set_tip(tooltips,button,"File selector for the script directory",NULL);
  gtk_box_pack_start (GTK_BOX(hbox2),button,FALSE,FALSE,2);
  g_signal_connect (G_OBJECT(button),"clicked",
                    G_CALLBACK(dnj_koj_frame_aftereffects_script_search),&info->dnj.koji_aftereffects);

  /* Buttons */
  /* Create script */
  bbox = gtk_hbutton_box_new ();
  gtk_box_pack_start (GTK_BOX(vbox),bbox,TRUE,TRUE,5);
  gtk_widget_show (bbox);
  button = gtk_button_new_with_label ("Create Script");
  gtk_tooltips_set_tip(tooltips,button,"Create automagically the script based on the given information",NULL);
  gtk_box_pack_start (GTK_BOX(bbox),button,TRUE,TRUE,2);
  switch (info->dnj.koj) {
  case KOJ_AFTEREFFECTS:
    g_signal_connect (G_OBJECT(button),"clicked",
                      G_CALLBACK(dnj_koj_frame_aftereffects_bcreate_pressed),&info->dnj);
    break;
  default:
    fprintf (stderr,"What ?!\n");
    break;
  }

  gtk_widget_show_all(frame);

  return frame;
}

GtkWidget *
jdd_koj_aftereffects_widgets (struct drqm_jobs_info *info) {
  GtkWidget *table;
  GtkWidget *label;
  GtkAttachOptions options = (GtkAttachOptions)(GTK_EXPAND | GTK_SHRINK | GTK_FILL) ;
  char *labels[] = { "Project file:", info->jdd.job.koji.aftereffects.project,
                     "Comp:", info->jdd.job.koji.aftereffects.comp,
                     "View command:", info->jdd.job.koji.aftereffects.viewcmd,
                     NULL };
  char **cur;
  int r,c;   /* Rows and columns */

  table = gtk_table_new (4,2, FALSE);

  cur = labels;
  r = 0;
  while ( *cur ) {
    c = 0;   /* First column */
    label = gtk_label_new (*cur);
    gtk_misc_set_alignment (GTK_MISC(label), 0, .5);
    gtk_table_attach (GTK_TABLE(table),GTK_WIDGET(label), c, c+1, r, r+1, options, options, 1 , 1 );
    cur++;   /* Next label */
    c++;   /* New column */
    label = gtk_label_new (*cur);
    gtk_label_set_line_wrap (GTK_LABEL(label), TRUE);
    gtk_misc_set_alignment (GTK_MISC(label), 1, .5);
    gtk_table_attach (GTK_TABLE(table),GTK_WIDGET(label), c, c+1, r, r+1, options, options, 1 , 1 );
    cur++;
    r++;   /* New row */
  }

  return table;
}

static void
dnj_koj_frame_aftereffects_project_search (GtkWidget *button, struct drqmj_koji_aftereffects *info) {
  GtkWidget *dialog;
  char dir[BUFFERLEN];
  
  // fix compiler warning
  (void)button;

  dialog = gtk_file_selection_new ("Please select the project file");
  info->fsproject = dialog;

  if (strlen(gtk_entry_get_text(GTK_ENTRY(info->eproject)))) {
    strncpy (dir,gtk_entry_get_text(GTK_ENTRY(info->eproject)),BUFFERLEN-1);
    gtk_file_selection_set_filename (GTK_FILE_SELECTION(dialog),strcat(dir,"/"));
  }

  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION(dialog)->ok_button),
                      "clicked", GTK_SIGNAL_FUNC (dnj_koj_frame_aftereffects_project_set), info);
  gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION(dialog)->ok_button),
                             "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
                             (gpointer) dialog);
  gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION(dialog)->cancel_button),
                             "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
                             (gpointer) dialog);
  gtk_widget_show (dialog);
  gtk_window_set_modal (GTK_WINDOW(dialog),TRUE);
}

static void
dnj_koj_frame_aftereffects_project_set (GtkWidget *button, struct drqmj_koji_aftereffects *info) {
  char buf[BUFFERLEN];
  
  // fix compiler warning
  (void)button;

  strncpy(buf,gtk_file_selection_get_filename(GTK_FILE_SELECTION(info->fsproject)),BUFFERLEN-1);
  gtk_entry_set_text (GTK_ENTRY(info->eproject),buf);
}

static void
dnj_koj_frame_aftereffects_bcreate_pressed (GtkWidget *button, struct drqmj_dnji *info) {
  struct aftereffectssgi aftereffectssgi; /* Aftereffects script generator info */
  char *file;

  // fix compiler warning
  (void)button;

  strncpy (aftereffectssgi.project,gtk_entry_get_text(GTK_ENTRY(info->koji_aftereffects.eproject)),BUFFERLEN-1);
  strncpy (aftereffectssgi.comp,gtk_entry_get_text(GTK_ENTRY(info->koji_aftereffects.ecomp)),BUFFERLEN-1);
  strncpy (aftereffectssgi.scriptdir,gtk_entry_get_text(GTK_ENTRY(info->koji_aftereffects.escript)),BUFFERLEN-1);

  if ((file = aftereffectssg_create (&aftereffectssgi)) == NULL) {
    fprintf (stderr,"ERROR: %s\n",drerrno_str());
    return;
  } else {
    gtk_entry_set_text(GTK_ENTRY(info->ecmd),file);

  }
}

static void
dnj_koj_frame_aftereffects_script_search (GtkWidget *button, struct drqmj_koji_aftereffects *info) {
  GtkWidget *dialog;
  
  // fix compiler warning
  (void)button;

  dialog = gtk_file_selection_new ("Please select a script directory");
  info->fsscript = dialog;

  if (strlen(gtk_entry_get_text(GTK_ENTRY(info->escript)))) {
    gtk_file_selection_set_filename (GTK_FILE_SELECTION(dialog),gtk_entry_get_text(GTK_ENTRY(info->escript)));
  }

  gtk_signal_connect (GTK_OBJECT (GTK_FILE_SELECTION(dialog)->ok_button),
                      "clicked", GTK_SIGNAL_FUNC (dnj_koj_frame_aftereffects_script_set), info);
  gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION(dialog)->ok_button),
                             "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
                             (gpointer) dialog);
  gtk_signal_connect_object (GTK_OBJECT (GTK_FILE_SELECTION(dialog)->cancel_button),
                             "clicked", GTK_SIGNAL_FUNC (gtk_widget_destroy),
                             (gpointer) dialog);
  gtk_widget_show (dialog);
  gtk_window_set_modal (GTK_WINDOW(dialog),TRUE);

}

static void
dnj_koj_frame_aftereffects_script_set (GtkWidget *button, struct drqmj_koji_aftereffects *info) {
  char buf[BUFFERLEN];
  char *p;

  // fix compiler warning
  (void)button;

  strncpy(buf,gtk_file_selection_get_filename(GTK_FILE_SELECTION(info->fsscript)),BUFFERLEN-1);
  p = strrchr(buf,'/');
  if (p)
    *p = 0;
  gtk_entry_set_text (GTK_ENTRY(info->escript),buf);
}
