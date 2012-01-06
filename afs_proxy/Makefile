DEBUG = -g
CFLAGS = -Wall $(DEBUG)

INCLUDES = -I./includes -I../libs/boinc -I../libs/boinc/lib -I../libs/boinc/api -I../libs/poco/Foundation/include -I../libs/poco/Util/include -I../libs/poco/Net/include -I../libs/poco/XML/include

cppsources_afs = AtticRequestHandler Chunk Filehash HTTPUtils PointerCollection
main_ex = afs_proxy

all:
	@for cppfile in $(cppsources_afs); do \
	echo Compiling $$cppfile; \
	g++ $(CFLAGS) src/$$cppfile.cpp -o $$cppfile.o -c $(INCLUDES); \
	done;	
	g++ $(CFLAGS) src/afs_proxy.cpp *.o $(INCLUDES) -o ../afs_proxy_app -pthread -ldl -L../libs/boinc/* -L../libs/boinc/lib -L../libs/poco/lib/Linux/x86_64 -L../libs//boinc/api -lboinc_api -lboinc -lPocoXMLd -lPocoUtild -lPocoNetd -lPocoFoundationd -lPocoDatad
	
clean:
	rm *.o

