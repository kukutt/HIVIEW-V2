RM := rm
CP := cp
HOME := $(shell echo ${GSF_HOME})
CC := $(shell echo ${GSF_CC})
AR := $(shell echo ${GSF_AR})
CFLAGS := $(shell echo ${GSF_CFLAGS})
LDFLAGS := -shared -g $(shell echo ${GSF_LDFLAGS})
TARG := lib/libmpp.so

#============================================================= 

INCS := -I$(HOME) -Isrc -Iinc -Isrc/sdk/ -Isrc/sdk/include/
SRCS := $(shell ls src/*.c)
SRCS += src/sdk/samples/libimp-samples/sample-common.c
OBJS := $(patsubst %.c, %.o, $(SRCS))
LIBS += src/sdk/lib/glibc/libimp.so 
LIBS += src/sdk/lib/glibc/libalog.so
DEP_OBJS := OBJS
DEP_LIBS += 

$(TARG): $(OBJS)
	$(RM) lib/$(DEP_OBJS) -rf
	$(foreach a, $(DEP_LIBS), cd lib; $(AR) -t $(a) | sed 's/^/lib\//' >> $(DEP_OBJS); $(AR) x $(a); cd ..;)
	${CC} ${LDFLAGS} -o $@ ${OBJS} `cat lib/$(DEP_OBJS)` ${LIBS}
	rm -rf `cat lib/$(DEP_OBJS)`; rm -rf lib/$(DEP_OBJS);
	cp $(TARG) $(HOME)/lib/$(GSF_CPU_ARCH)/ -v
	cp src/sdk/lib/glibc/libimp.so $(HOME)/lib/$(GSF_CPU_ARCH)/ -v
	cp src/sdk/lib/glibc/libalog.so $(HOME)/lib/$(GSF_CPU_ARCH)/ -v
	cp src/sdk/lib/glibc/libaudioProcess.so $(HOME)/lib/$(GSF_CPU_ARCH)/ -v

.c.o:
	$(CC) $(CFLAGS) -c $< $(INCS) -o $@

.Phony: clean
clean:
	-rm $(TARG) $(OBJS) src/*.bak -rf
