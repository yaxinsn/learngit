# makefile written for gnu make
CXX		= g++ 
SRC		= ../src
CPPFLAGS	= -I$(SRC)
DEBUG		= -g
#OPTIMIZE	= -O2
GCCWARN		= -Wall -Wstrict-prototypes
CXXFLAGS	= $(DEBUG) $(GCCWARN) $(OPTIMIZE) $(INCLUDES) 

LIB		= ../libXmlRpc.a
//CXXLDFLAGS      = -l$(LIB)
LDLIBS          = $(SYSTEMLIBS) $(LIB)
# Add your system-dependent network libs here
# Solaris: -lsocket -lnsl
SYSTEMLIBS	=
src = $(wildcard *.cpp)
obj = $(src:.cpp=.o)


LDLIBS		= $(SYSTEMLIBS) $(LIB)


TESTS		= TEST MyClient #HelloClient HelloServer TestBase64Client TestBase64Server TestValues TestXmlValidator

all:		MyClient


TEST:
	echo " $(obj) "
#$(TESTS):	$(LIB)

#$(src:.cpp=.o): %.o : %.cpp
#	$(CXX) $(CXXFLAGS) $^  -o $@

MyClient:       $(obj)
	$(CXX) $(CXXFLAGS) $(CXXLDFLAGS)  $^ ../libXmlRpc.a -o $@

clean:
		rm -f *.o
		rm -f $(TESTS)
