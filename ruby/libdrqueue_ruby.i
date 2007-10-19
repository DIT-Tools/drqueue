// 
// Copyright (C) 2001,2002,2003,2004,2005,2006,2007 Jorge Daza Garcia-Blanes
// Copyright (C) 2007 Andreas Schroeder
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
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA	 02111-1307
// USA
// 
// 


%define DOCSTRING
"The drqueue module allows the access to the libdrqueue library responsible
of all major operations that can be applied remotely to drqueue master and
slaves. Also provides access to all data structures of DrQueue."
%enddef
%module (docstring=DOCSTRING) libdrqueue
%{
#include "libdrqueue.h"
%}


// Tell SWIG to keep track of mappings between C/C++ structs/classes
%trackobjects;


%include "typemaps.i"

%typemap(in,numinputs=0) struct computer **computer (struct computer *computer) {
	$1 = &computer;
}
%typemap(argout) struct computer **computer {
	if (result < 0) {
		rb_raise(rb_eIOError,drerrno_str());
		$result = (VALUE)NULL;
	} else {
		int i;
		VALUE l = rb_ary_new();
		struct computer *c = malloc (sizeof(struct computer)*result);
		if (!c) {
			rb_raise(rb_eNoMemError,"out of memory");
			return (VALUE)NULL;
		}
		struct computer *tc = c;
		memcpy (c,*$1,sizeof(struct computer)*result);
		for (i=0; i<result; i++) {
			VALUE o = SWIG_NewPointerObj((void*)(tc), SWIGTYPE_p_computer, 0);
			rb_ary_push(l,o);
			tc++;
		}
		free (*$1);
		$result = l;
	}
}


%typemap(in,numinputs=0) struct job **job (struct job *job) {
	$1 = &job;
}
%typemap(argout) struct job **job {
	if (result < 0) {
		rb_raise(rb_eIOError,drerrno_str());
		$result = (VALUE)NULL;
	} else {
		int i;
		VALUE l = rb_ary_new();
		struct job *j = malloc (sizeof(struct job)*result);
		if (!j) {
			rb_raise(rb_eNoMemError,"out of memory");
			return (VALUE)NULL;
		}
		struct job *tj = j;
		memcpy (j,*$1,sizeof(struct job)*result);
		for (i=0; i<result; i++) {
			VALUE o = SWIG_NewPointerObj((void*)(tj), SWIGTYPE_p_job, 0);
			rb_ary_push(l,o);
			tj++;
		}
		free(*$1);
		$result = l;
	}
}


%include "pointer.h"
%include "libdrqueue.h"
%include "computer.h"
%include "computer_info.h"
%include "computer_status.h"
%include "task.h"
%include "request.h"
%include "constants.h"
%include "job.h"
%include "envvars.h"
%include "common.h"
%include "computer_pool.h"

// for jobscript generators
%include "blendersg.h"
%include "mentalraysg.h"

// type mapppings
typedef unsigned int time_t;
typedef unsigned short int uint16_t;
typedef unsigned long int uint32_t;
typedef unsigned char uint8_t;


// these methods generate new objects
%newobject *::request_job_list;
%newobject *::request_computer_list;


// JOB
%extend job {
	job ()
	{
		struct job *j;
		j = (struct job *)malloc (sizeof(struct job));
		if (!j) {
			rb_raise(rb_eNoMemError,"out of memory");
			return (VALUE)NULL;
		}
		job_init (j);
		return j;
	}

	~job ()
	{
		job_init(self);
		free (self);
	}	

	int environment_variable_add (char *name, char *value)
	{
		return envvars_variable_add (&self->envvars,name,value);
	}

	int environment_variable_delete (char *name)
	{
		return envvars_variable_delete (&self->envvars,name);
	}

	char *environment_variable_find (char *name)
	{
		struct envvar *variable;

		variable = envvars_variable_find (&self->envvars,name);
	
		if (!variable) {
			rb_raise(rb_eIndexError,"No such variable");
			return (VALUE)NULL;
		}

		return variable->value;
	}

	VALUE request_frame_list (int who)
	{
		VALUE l = rb_ary_new();
		int nframes = job_nframes(self);
		int i;
		if (nframes) {
			struct frame_info *fi = (struct frame_info *)malloc (sizeof(struct frame_info) * nframes);
			if (!fi) {
				rb_raise(rb_eNoMemError,"out of memory");
				return (VALUE)NULL;
			}
			if (!request_job_xferfi (self->id,fi,nframes,who)) {
				rb_raise(rb_eIOError,drerrno_str());
				return (VALUE)NULL;
			}
			for (i=0; i<nframes; i++) {
				VALUE o = SWIG_NewPointerObj((void*)(&fi[i]), SWIGTYPE_p_frame_info, 0);
				rb_ary_push(l,o);
			}
		}
		return l;
	}

	int job_frame_index_to_number (int index)
	{
		if ((index < 0) || (index >= job_nframes(self))) {
			rb_raise(rb_eIndexError,"frame index out of range");
			return -1;
		}

		return job_frame_index_to_number (self,index);
	}

	int request_stop (int who)
	{
		if (!request_job_stop (self->id,who)) {
			rb_raise(rb_eIOError,drerrno_str());
			return 0;
		}
		return 1;
	}

	int request_rerun (int who)
	{
		if (!request_job_rerun (self->id,who)) {
			rb_raise(rb_eIOError,drerrno_str());
			return 0;
		}
		return 1;
	}

	int request_hard_stop (int who)
	{
		if (!request_job_hstop (self->id,who)) {
			rb_raise(rb_eIOError,drerrno_str());
			return 0;
		}
		return 1;
	}

	int request_delete (int who)
	{
		if (!request_job_delete (self->id,who)) {
			rb_raise(rb_eIOError,drerrno_str());
			return 0;
		}
		return 1;
	}

	int request_continue (int who)
	{
		if (!request_job_continue (self->id,who)) {
			rb_raise(rb_eIOError,drerrno_str());
			return 0;
		}
		return 1;
	}

	int send_to_queue (void)
	{
		if (!register_job (self)) {
			rb_raise(rb_eIOError,drerrno_str());
			return 0;
		}
		return 1;
	}

	int update (int who)
	{
		if (!request_job_xfer(self->id,self,who)) {
			rb_raise(rb_eIOError,drerrno_str());
			return 0;
		}
		return 1;
	}
	
	
	/* Blender script file generation */
	char *blendersg (char *scene, char *scriptdir, int blender)
	{	
		struct blendersgi *blend = (struct blendersgi *)malloc (sizeof(struct blendersgi));
    	if (!blend) {
 	     	rb_raise(rb_eNoMemError,"out of memory");
    	 	return NULL;
   		}	
		
		char *outfile = (char *)malloc(sizeof(char *));
		if (!outfile) {
 	     	rb_raise(rb_eNoMemError,"out of memory");
    	 	return NULL;
   		}
		
		memset (blend,0,sizeof(struct blendersgi));
		
		strncpy(blend->scene, scene, BUFFERLEN-1);
		strncpy(blend->scriptdir, scriptdir, BUFFERLEN-1);
		blend->blender = blender;
		
  		outfile = blendersg_create(blend);
  		
		if (!outfile) {
			rb_raise(rb_eException,"Problem creating script file");
      		return NULL;
		}
		
		return outfile;
	}
	
	
	/* MentalRay script file generation */
	char *mentalraysg (char *scene, char *scriptdir, char *renderdir, char *image, char *file_owner, char *camera, int res_x, int res_y, char *format, int mentalray)
	{	
		struct mentalraysgi *ment = (struct mentalraysgi *)malloc (sizeof(struct mentalraysgi));
    	if (!ment) {
 	     	rb_raise(rb_eNoMemError,"out of memory");
    	 	return NULL;
   		}	
		
		char *outfile = (char *)malloc(sizeof(char *));
		if (!outfile) {
 	     	rb_raise(rb_eNoMemError,"out of memory");
    	 	return NULL;
   		}
		
		memset (ment,0,sizeof(struct mentalraysgi));
		
		strncpy(ment->renderdir, renderdir, BUFFERLEN-1);
		strncpy(ment->scene, scene, BUFFERLEN-1);
		strncpy(ment->image, image, BUFFERLEN-1);
		strncpy(ment->scriptdir, scriptdir, BUFFERLEN-1);
		strncpy(ment->file_owner, file_owner, BUFFERLEN-1);
		strncpy(ment->camera, camera, BUFFERLEN-1);
		ment->res_x = res_x;
		ment->res_y = res_y;
		strncpy(ment->format, format, BUFFERLEN-1);
		ment->mentalray = mentalray;
		
  		outfile = mentalraysg_create(ment);
  		
		if (!outfile) {
			rb_raise(rb_eException,"Problem creating script file");
      		return NULL;
		}
		
		return outfile;
	}
	
}



/* COMPUTER LIMITS */
%extend computer_limits {
	%exception get_pool {
		$action
		if ((!result) || (result == (void *)-1)) {
			rb_raise(rb_eIndexError,"Index out of range");
			return (VALUE)NULL;
		}
	}
	%newobject get_pool;
	struct pool *get_pool (int n)
	{
		struct pool *pool;
		
		if (n >= self->npools) {
			return (VALUE)NULL;
		} else if ( n < 0 ) {
			return (VALUE)NULL;
		}

		pool = (struct pool *)malloc (sizeof (struct pool));
		if (!pool) {
			rb_raise(rb_eNoMemError,"out of memory");
			return (VALUE)NULL;
		}
		
		if (self->npools) {
			if ((self->pool.ptr = (struct pool *) computer_pool_attach_shared_memory(self)) == (void*)-1) {
				return pool;
			}
		}
		memcpy(pool,&self->pool.ptr[n],sizeof(struct pool));

		computer_pool_detach_shared_memory (self);

		return pool;
	}

  void pool_add (char *computer_name, char *pool_name, int who)
  {
		if (!request_slave_limits_pool_add(computer_name,pool_name,who)) {
			rb_raise(rb_eIOError,drerrno_str());
    }
  }

  void pool_remove (char *computer_name, char *pool_name, int who)
  {
		if (!request_slave_limits_pool_remove(computer_name,pool_name,who)) {
			rb_raise(rb_eIOError,drerrno_str());
    }
  }

  void pool_list ()
  {
    computer_pool_list (self);
  }
}

/* COMPUTER STATUS */
%extend computer_status {
	%exception get_loadavg {
		$action
		if (result == (uint16_t)-1) {
			rb_raise(rb_eIndexError,"Index out of range");
			return (VALUE)NULL;
		}
	}
	uint16_t get_loadavg (int index)
	{
		if ((index < 0) || (index > 2)) {
			return -1;
		}

		return self->loadavg[index];
	}

	%exception get_task {
		$action
		if (!result) {
			rb_raise(rb_eIndexError,"Index out of range");
			return (VALUE)NULL;
		}
	}
	struct task *get_task (int index)
	{
		if ((index < 0) || (index >= MAXTASKS)) {
			return (VALUE)NULL;
		}
		return &self->task[index];
	}
}

// struct pool
%extend pool {
  pool (char *name)
  {
    struct pool *p;
    p = (struct pool *)malloc (sizeof(struct pool));
    if (!p) {
      rb_raise(rb_eNoMemError,"out of memory");
      return (VALUE)NULL;
    }
    memset (p,0,sizeof(struct pool));
    strncpy (p->name,name,MAXNAMELEN-1);
    return p;
  }

  ~pool ()
  {
    free (self);
  }
}


// COMPUTER
%extend computer {
	computer ()
	{
		struct computer *c;
		c = (struct computer *)malloc (sizeof(struct computer));
		if (!c) {
			rb_raise(rb_eNoMemError,"out of memory");
			return (VALUE)NULL;
		}
		computer_init(c);
		return c;
	}

	~computer ()
	{
		free (self);
	}

  VALUE list_pools (void)
  {
    VALUE l = rb_ary_new();
	int npools = self->limits.npools;

    if ((self->limits.pool.ptr = (struct pool *) computer_pool_attach_shared_memory(&self->limits)) == (void*)-1) {
    	rb_raise(rb_eException,drerrno_str());
    	return (VALUE)NULL;
	}

    int i;
    for (i=0;i<npools;i++) {
      struct pool *pool_i = (struct pool *)malloc (sizeof(struct pool));
			if (!pool_i) {
				rb_raise(rb_eNoMemError,"out of memory");
				return (VALUE)NULL;
			}
      memcpy (pool_i,&self->limits.pool.ptr[i],sizeof(struct pool));
	  VALUE o = SWIG_NewPointerObj((void*)(pool_i), SWIGTYPE_p_pool, 0);
	  rb_ary_push(l,o);
	  }

    computer_pool_detach_shared_memory (&self->limits);
	return l; 
  }

  VALUE set_pools (VALUE pool_list) {	
    int i;
    if (RARRAY(pool_list)->len >0) {	
      rb_raise(rb_eException,"Expecting a list");
      return (VALUE)NULL;
    }
    int npools = RARRAY(pool_list)->len;
    VALUE old_list = computer_list_pools(self);
    if (RARRAY(old_list)->len >0) {
      rb_raise(rb_eException,"Expecting a list");
      return (VALUE)NULL;
    }
    for (i=0;i<npools;i++) {
      VALUE pool_obj = rb_ary_entry(pool_list,i);
      struct pool *tpool = NULL;
      SWIG_ConvertPtr(pool_obj,(void **)&tpool, SWIGTYPE_p_pool, SWIG_POINTER_EXCEPTION | 0 );
      request_slave_limits_pool_add(self->hwinfo.name,tpool->name,CLIENT);
    }
    int onpools = RARRAY(old_list)->len;
    for (i=0;i<onpools;i++) {
      VALUE pool_obj = rb_ary_entry(old_list,i);
      struct pool *tpool = NULL;
      SWIG_ConvertPtr(pool_obj,(void **)&tpool, SWIGTYPE_p_pool, SWIG_POINTER_EXCEPTION | 0 );
      request_slave_limits_pool_remove(self->hwinfo.name,tpool->name,CLIENT);
    }
    VALUE last_list = computer_list_pools(self);
    return last_list;
  }

	void request_enable (int who)
	{
		if (!request_slave_limits_enabled_set (self->hwinfo.name,1,who)) {
			rb_raise(rb_eIOError,drerrno_str());
		}
	}

	void request_disable (int who)
	{
		if (!request_slave_limits_enabled_set (self->hwinfo.name,0,who)) {
			rb_raise(rb_eIOError,drerrno_str());
		}
	}

	void update (int who)
	{
		if (!request_comp_xfer(self->hwinfo.id,self,who)) {
			rb_raise(rb_eIOError,drerrno_str());
		}
	}

  void add_pool (char *pool_name, int who)
  {
    if (!request_slave_limits_pool_add(self->hwinfo.name,pool_name,who)) {
		  	rb_raise(rb_eIOError,drerrno_str());
    }
  }

  void remove_pool (char *pool_name, int who)
  {
    if (!request_slave_limits_pool_remove(self->hwinfo.name,pool_name,who)) {
		  	rb_raise(rb_eIOError,drerrno_str());
    }
  }
}
