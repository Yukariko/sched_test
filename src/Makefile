
INCLUDEDIRS = \


LIBDIRS = \

LIBS = \

CPPFLAGS = -Wall -O2 -std=c++0x -g $(INCLUDEDIRS) -fPIE -c
LDFLAGS =  $(LIBDIRS) $(LIBS)

bin = libschedtest.so

t1 = process
t2 = sched_test
t3 = cpu

obj = $(t1).o $(t2).o $(t3).o

all: $(bin)
$(bin): $(obj)
		$(CXX) $(CPPFLAGS) $(obj) $(LDFLAGS) -o $@

clean:
		rm -f $(bin) *.o