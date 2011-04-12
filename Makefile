PG_INCLUDE=`pg_config --includedir-server`
PG_INSTALL=`pg_config --pkglibdir`

INCLUDE=-I$(PG_INCLUDE)
DEST_DIR=$(PG_INSTALL)
OPT_LEVEL=-O2
CFLAGS=$(OPT_LEVEL) -W -Wall -Wpointer-arith -Wno-unused-parameter -Wunused-function -Wunused-variable -Wunused-value -Werror
LIBS=-lkyotocabinet -lprotobuf -lprotobuf-c

.c.o:
	gcc -g -c $(INCLUDE) $(CFLAGS) -fpic c/entry.pb-c.c $<

.cc.o:
	gcc -g -c $(INCLUDE) $(CFLAGS) -fpic cpp/entry.pb.cc $<

all: libpg_kc.so

gen:
    protoc-c --c_out=c entry.proto
    protoc --cpp_out=cpp entry.proto	

clean:
	rm *.o
	rm *.so

libpg_kc.so: entry.proto pg_kc.o entry.pb-c.o
	gcc --library-path=/usr/local/lib --shared -o libpg_kc.so $(LIBS) -fpic entry.pb-c.o pg_kc.o

install: libpg_kc.so
	install -s libpg_kc.so $(DEST_DIR)