#
# $Id$
#

ifeq (0,${MAKELEVEL})
whoami := $(shell whoami)
host-type := $(shell arch)
MAKE := ${MAKE} host-type=${host-type} whoami=${whoami}
endif

SRCS_LIBDRQUEUE := $(wildcard libdrqueue/*.c)
OBJS_LIBDRQUEUE := $(patsubst %.c,%.o,$(SRCS_LIBDRQUEUE))

ifeq ($(origin INSTROOT),undefined)
INSTROOT = /usr/local/drqueue
$(warning Using default installation directory $(INSTROOT))
endif

INSTUID ?= drqueue
INSTGID ?= drqueue 

DOTNETPATH ?= C:/WINDOWS/Microsoft.NET/Framework/v1.1.4322
NSISPATH ?= C:/Program\ Files/NSIS


#Figure out OS-specific Configuration parameters
ifeq ($(origin systype),undefined)
systype := $(shell bash -c "source ./bin/shlib; get_env_kernel")
machinetype := $(shell bash -c "source ./bin/shlib; get_env_machine")
endif

CPPFLAGS += -D_NO_COMPUTER_SEMAPHORES -D_NO_COMPUTER_POOL_SEMAPHORES -D_GNU_SOURCE -DCOMM_REPORT -I. -Ilibdrqueue
CFLAGS ?= -g -O0 -Wall $(ARCHFLAGS)
LDFLAGS ?= $(CFLAGS)
CXXFLAGS += $(CFLAGS) $(CPPFLAGS) -D__CPLUSPLUS 

ifeq ($(systype),Linux)
 CPPFLAGS += -D__LINUX
 ifeq ($(machinetype),i686)
  ARCHFLAGS += -march=i686 -m32
 else
  ifeq ($(machinetype),x86_64)
   CHOST ?= x86_64-pc-linux-gnu
   ARCHFLAGS += -march=x86-64 -m64 -pipe
  endif
 endif
else
 ifeq ($(systype),IRIX)
  CPPFLAGS += -D__IRIX
#  MAKE = /usr/freeware/bin/gmake
 else
  ifeq ($(systype),Darwin)
   CPPFLAGS += -D__OSX
   ifeq ($(machinetype),Power_Macintosh)
    ARCHFLAGS += -mpowerpc -mtune=powerpc
   endif
#   MAKE = make
  else 
   ifeq ($(systype),FreeBSD)
    CPPFLAGS += -D__FREEBSD
    MAKE = gmake
   else
    ifeq ($(systype),CYGWIN_NT-5.1)
     CPPFLAGS += -D__CYGWIN
     UIFLAGS += -e _mainCRTStartup -mwindows contrib/windows/Resources/drqueue.res 
    else	
     $(error Cannot make DrQueue -- systype "$(systype)" is unknown)
    endif
   endif
  endif
 endif
endif

ifneq ($(origin LIBWRAP),undefined)
 CFLAGS += -DLIBWRAP
 LDFLAGS += -lwrap
endif

#abstract make targets
.PHONY: default all install miniinstall irix_install linux_install doc tags clean testing_env drqman python.build python.install

BASE_C_TOOLS = slave master requeue jobfinfo blockhost ctask cjob jobinfo compinfo
BASE_CXX_TOOLS = sendjob
BASE_C_GUI = drqman
PYTHON_MODULE = python

all: base

drqman: drqman/drqman

base: $(BASE_C_TOOLS) $(BASE_CXX_TOOLS)

gui: $(BASE_GUI)

python.clean:
	cd python; python setup.py clean --all

python.build: python/drqueue.i $(SRCS_LIBDRQUEUE)
	cd python; python setup.py build_ext; python setup.py build

python.install: python.build
	cd python; python setup.py install

install: miniinstall $(systype)_install 

drqman/drqman: libdrqueue.a
	cd drqman && $(MAKE)

testing_env:
	mkdir tmp logs db
	chmod 777 tmp

IRIX_install:
	install -d -m 0777 $(INSTROOT)/tmp
	install -d -m 0777 $(INSTROOT)/logs
	install -d -m 0755 $(INSTROOT)/bin
	install -d -m 0755 $(INSTROOT)/etc
	install -d -m 0777 $(INSTROOT)/db
	install -d -m 0777 $(INSTROOT)/contrib
	cp -r ./bin/* $(INSTROOT)/bin/ || exit 0
	cp ./etc/* $(INSTROOT)/etc/ || exit 0
	cp ./contrib/* $(INSTROOT)/contrib/ || exit 0
	chmod -R 0755 $(INSTROOT)/bin/* || exit 0
	chmod 0755 $(INSTROOT)/contrib/* || exit 0
	chown -R $(INSTUID):$(INSTGID) $(INSTROOT)/bin/*
	chown $(INSTUID):$(INSTGID) $(INSTROOT)/contrib/*

Linux_install:
	install -d -m 0777 $(INSTROOT)/tmp
	install -d -m 0777 $(INSTROOT)/logs
	install -d -m 0755 $(INSTROOT)/bin
	install -d -m 0755 $(INSTROOT)/etc
	install -d -m 0777 $(INSTROOT)/db
	install -d -m 0777 $(INSTROOT)/contrib
	cp -r ./bin/* $(INSTROOT)/bin/ || exit 0
	cp ./etc/* $(INSTROOT)/etc/ || exit 0
	cp ./contrib/* $(INSTROOT)/contrib/ || exit 0
	chmod -R 0755 $(INSTROOT)/bin/* || exit 0
	chmod 0755 $(INSTROOT)/contrib/* || exit 0
	chown -R $(INSTUID):$(INSTGID) $(INSTROOT)/bin/*
	chown $(INSTUID):$(INSTGID) $(INSTROOT)/contrib/*

CYGWIN_NT-5.1_install:
	install -d -m 0777 $(INSTROOT)/tmp
	install -d -m 0777 $(INSTROOT)/logs
	install -d -m 0755 $(INSTROOT)/bin
	install -d -m 0755 $(INSTROOT)/etc
	install -d -m 0777 $(INSTROOT)/db
	install -d -m 0777 $(INSTROOT)/contrib
	install -d -m 0777 $(INSTROOT)/contrib/windows
	install -d -m 0777 $(INSTROOT)/contrib/windows/Installer
	cp -r ./bin/*.exe $(INSTROOT)/bin/ || exit 0
	cp /usr/sbin/cygserver $(INSTROOT)/bin || exit 0
	cp `which expr.exe` $(INSTROOT)/bin || exit 0
	cp `which tcsh.exe` $(INSTROOT)/bin || exit 0
	cp `which cygpath.exe` $(INSTROOT)/bin || exit 0
	cp ./etc/* $(INSTROOT)/etc/ || exit 0
	sh ./contrib/windows/build_services.sh $(PWD)/contrib/windows $(DOTNETPATH) 
	cp ./contrib/* $(INSTROOT)/contrib/ || exit 0
	cp ./contrib/windows/*.exe $(INSTROOT)/contrib/windows || exit 0
	cp ./contrib/windows/Installer/* $(INSTROOT)/contrib/windows/installer || exit 0
	cp COPYING $(INSTROOT)/
	chmod -R 0755 $(INSTROOT)/bin/* || exit 0
	chmod 0755 $(INSTROOT)/contrib/* || exit 0
	sh contrib/windows/install_dlls.sh $(INSTROOT)/bin
	$(NSISPATH)/makensis.exe `cygpath -w $(INSTROOT)/contrib/windows/Installer/installer.nsi`
	mv $(INSTROOT)/contrib/windows/Installer/Install.exe $(INSTROOT)/contrib/drqueue-setup.exe  

FreeBSD_install:
	install -d -m 0777 $(INSTROOT)/tmp
	install -d -m 0777 $(INSTROOT)/logs
	install -d -m 0755 $(INSTROOT)/bin
	install -d -m 0755 $(INSTROOT)/etc
	install -d -m 0777 $(INSTROOT)/db
	install -d -m 0777 $(INSTROOT)/contrib
	cp -r ./bin/* $(INSTROOT)/bin/ || exit 0
	cp ./etc/* $(INSTROOT)/etc/ || exit 0
	cp ./contrib/* $(INSTROOT)/contrib/ || exit 0
	chmod -R 0755 $(INSTROOT)/bin/* || exit 0
	chmod 0755 $(INSTROOT)/contrib/* || exit 0
	chown -R $(INSTUID):$(INSTGID) $(INSTROOT)/bin/*
	chown $(INSTUID):$(INSTGID) $(INSTROOT)/contrib/*

Darwin_install:
	install -d -m 0777 $(INSTROOT)/tmp
	install -d -m 0777 $(INSTROOT)/logs
	install -d -m 0755 $(INSTROOT)/bin
	install -d -m 0755 $(INSTROOT)/etc
	install -d -m 0777 $(INSTROOT)/db
	install -d -m 0777 $(INSTROOT)/contrib
	cp -r ./bin/* $(INSTROOT)/bin/ || exit 0
	cp ./etc/* $(INSTROOT)/etc/ || exit 0
	cp ./contrib/* $(INSTROOT)/contrib/ || exit 0
	chmod -R 0755 $(INSTROOT)/bin/* || exit 0
	chmod 0755 $(INSTROOT)/contrib/* || exit 0
	chown -R $(INSTUID):$(INSTGID) $(INSTROOT)/bin/*
	chown $(INSTUID):$(INSTGID) $(INSTROOT)/contrib/*

miniinstall: base
ifeq ($(systype),IRIX)
	install -root $(PWD) -d -m 0755 bin
	install -root $(PWD) -m 0755 -f /bin -src slave slave.$(systype).$(machinetype)
	install -root $(PWD) -m 0755 -f /bin -src master master.$(systype).$(machinetype)
	install -root $(PWD) -m 0755 -f /bin -src requeue requeue.$(systype).$(machinetype)
	install -root $(PWD) -m 0755 -f /bin -src jobfinfo jobfinfo.$(systype).$(machinetype)
	install -root $(PWD) -m 0755 -f /bin -src jobinfo jobinfo.$(systype).$(machinetype)
	install -root $(PWD) -m 0755 -f /bin -src blockhost blockhost.$(systype).$(machinetype)
	install -root $(PWD) -m 0755 -f /bin -src ctask ctask.$(systype).$(machinetype)
	install -root $(PWD) -m 0755 -f /bin -src cjob cjob.$(systype).$(machinetype)
	install -root $(PWD) -m 0755 -f /bin -src sendjob sendjob.$(systype).$(machinetype)
	test -x ./drqman/drqman && install -root $(PWD) -m 0755 -f /bin -src drqman/drqman drqman.$(systype).$(machinetype) || test 1
else
 ifeq ($(systype),CYGWIN_NT-5.1)
	install -d -m 0755 bin
	install -m 0755 -p ./slave.exe bin/slave.exe
	install -m 0755 -p ./master.exe bin/master.exe
	install -m 0755 -p ./requeue.exe bin/requeue.exe
	install -m 0755 -p ./jobfinfo.exe bin/jobfinfo.exe
	install -m 0755 -p ./jobinfo.exe bin/jobfinfo.exe
	install -m 0755 -p ./blockhost.exe bin/blockhost.exe
	install -m 0755 -p ./ctask.exe bin/ctask.exe
	install -m 0755 -p ./cjob.exe bin/cjob.exe
	install -m 0755 -p ./sendjob.exe bin/sendjob.exe
	test -x ./drqman/drqman.exe && install -m 0755 -p ./drqman/drqman.exe bin/drqman.exe || exit 0
else
	install -d -m 0755 bin
	install -m 0755 -p ./slave bin/slave.$(systype).$(machinetype)
	install -m 0755 -p ./master bin/master.$(systype).$(machinetype)
	install -m 0755 -p ./requeue bin/requeue.$(systype).$(machinetype)
	install -m 0755 -p ./jobfinfo bin/jobfinfo.$(systype).$(machinetype)
	install -m 0755 -p ./jobinfo bin/jobinfo.$(systype).$(machinetype)
	install -m 0755 -p ./blockhost bin/blockhost.$(systype).$(machinetype)
	install -m 0755 -p ./ctask bin/ctask.$(systype).$(machinetype)
	install -m 0755 -p ./cjob bin/cjob.$(systype).$(machinetype)
	install -m 0755 -p ./sendjob bin/sendjob.$(systype).$(machinetype)
	test -x ./drqman/drqman && install -m 0755 -p ./drqman/drqman bin/drqman.$(systype).$(machinetype) || exit 0
endif
endif

doc:
	text -d ./doc || mkdir ./doc
	cxref *.[ch] libdrqueue/*.[ch] drqman/*.[ch] $(CPPFLAGS) `pkg-config gtk+-2.0 --cflags` -all-comments -xref-all -index-all -html -O./doc

tags:
	etags *.[ch] libdrqueue/*.[ch] drqman/*.[ch]

clean:
	rm -fR *.o *.os *.exe *~ libdrqueue.a $(BASE_C_TOOLS) $(BASE_CXX_TOOLS) TAGS tmp/* logs/* db/* contrib/windows/*.exe \
        bin/*.$(systype).$(machinetype) $(OBJS_LIBDRQUEUE)
	rm -fR libdrqueue/*~
	rm -f libdrqueue/*.os
	$(MAKE) -C drqman clean

#actual object make targets
libdrqueue.h: $(wildcard libdrqueue/*.h)
libdrqueue.a: $(OBJS_LIBDRQUEUE) libdrqueue.h
	ar r $@ $(OBJS_LIBDRQUEUE)
	ranlib $@

ifeq ($(systype),CYGWIN_NT-5.1)
contrib/windows/Resources/drqueue.res: contrib/windows/Resources/drqueue.rc
	$(MAKE) -C contrib/windows/Resources
endif

%.c: %.h
%.cpp: %.h
%.o: %.c %.h constants.h
	$(CC) -c $(CFLAGS) $< -o $@

slave: slave.o libdrqueue.a
	$(CC) $(LDFLAGS) $^ -o $@
master: master.o libdrqueue.a
	$(CC) $(LDFLAGS) $^ -o $@
requeue: requeue.o libdrqueue.a
	$(CC) $(LDFLAGS) $^ -o $@
jobinfo: jobinfo.o libdrqueue.a
	$(CC) $(LDFLAGS) $^ -o $@
jobfinfo: jobfinfo.o libdrqueue.a
	$(CC) $(LDFLAGS) $^ -o $@
blockhost: blockhost.o libdrqueue.a
	$(CC) $(LDFLAGS) $^ -o $@
ctask: ctask.o libdrqueue.a
	$(CC) $(LDFLAGS) $^ -o $@
cjob: cjob.o libdrqueue.a
	$(CC) $(LDFLAGS) $^ -o $@
compinfo: compinfo.o libdrqueue.a
	$(CC) $(LDFLAGS) $^ -o $@

sendjob.o: sendjob.cpp sendjob.h
	$(CXX) -c $(CXXFLAGS) $<
sendjob: sendjob.o libdrqueue.a
	$(CXX) $^ $(LDFLAGS) -o $@ 


