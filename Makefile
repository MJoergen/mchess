sources  = main.cc
sources += CBoard.cc
sources += CMove.cc
sources += ai.cc
sources += CMoveList.cc

program = mchess

TARGET = linux
#TARGET = windows

objects = $(sources:.cc=.o)
depends = $(sources:.cc=.d)

OPTIONS   = -W -Wall
OPTIONS  += -O3
OPTIONS  += -g  # debug info
#OPTIONS  += -pg # profiling

ifeq ($(TARGET),linux)
  CC = g++
else
  CC = i686-w64-mingw32-g++
  OPTIONS  += -static-libgcc -static-libstdc++
  program := $(program).exe
endif

$(program): $(objects) Makefile
	$(CC) -o $@ $(objects) $(OPTIONS)
	cp $@ $(HOME)/bin

%.d: %.cc Makefile
	set -e; $(CC) -M $(CPPFLAGS) $(DEFINES) $(INCLUDE_DIRS) $< \
		| sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' > $@; \
		[ -s $@ ] || rm -f $@

include $(depends)

%.o : %.cc Makefile
	$(CC) $(OPTIONS) $(DEFINES) $(INCLUDE_DIRS) -c $< -o $@

clean:
	-rm -f $(objects)
	-rm -f $(depends)
	-rm -f $(program)

