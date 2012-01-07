POCO_BASE=../libs/poco

PROJECT_BASE = $(shell cd ..; pwd; cd afs_proxy)

POCO_ADD_INCLUDE += ./src ./includes ../libs/boinc ../libs/boinc/lib ../libs/boinc/api
POCO_ADD_LIBRARY += ../libs/boinc/* ../libs/boinc/lib ../libs/boinc/api ../libs/boinc/mac_build/build/Deployment

include $(POCO_BASE)/build/rules/global

objects = AtticRequestHandler Chunk Filehash HTTPUtils PointerCollection afs_proxy

target         = afs_proxy
target_libs    = PocoUtil PocoNet PocoXML PocoFoundation

SYSLIBS += -lboinc_api -lboinc

include $(POCO_BASE)/build/rules/exec


#LIBS = -L ../libs/poco/lib/Linux/x86_64 -lPocoFoundationd -lPocoDatad -lPocoXMLd -lPocoUtild -lPocoNetd -pthread -ldl -L../libs/boinc/* -L../libs/boinc/lib -L../libs//boinc/api -lboinc_api -lboinc
#DEBUG = -g
#CFLAGS = -Wall -Wall -Wno-sign-compare -static -g -D_DEBUG
#BUILDDIR = build/


#cppsources_afs = AtticRequestHandler Chunk Filehash HTTPUtils PointerCollection
#main_ex = afs_proxy


#all:	
#	-mkdir $(BUILDDIR)
#	@for cppfile in $(cppsources_afs); do \
#	echo Compiling $$cppfile; \
#	g++ $(CFLAGS) src/$$cppfile.cpp -o $(BUILDDIR)$$cppfile.o -c $(INCLUDES); \
#	done;	
#	g++ $(CFLAGS) src/afs_proxy.cpp $(BUILDDIR)*.o $(INCLUDES) -o afs_proxy $(LIBS)
#	
#clean:
#	-rm -rf $(BUILDDIR)

