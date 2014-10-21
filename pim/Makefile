

#########################################
#
#target : all,lib env,install
#
##########################################
CXX = gcc
CXXFLAGS += -Wall -fPIC
CXXLDFLAGS += -lpthread

src = $(wildcard *.c)
obj = $(src:.c=.o)
progam = test-pim

all: t lib $(progam)

t:
	@echo $(src)
	@echo $(obj)

lib: $(obj)
	ar cr libpim.a libpim.o
	

$(src:.c=.o): %.o : %.c
	$(CXX) $(CXXFLAGS) $^ -c -o $@

$(progam): $(obj)
	$(CXX) $(CXXFLAGS) $(CXXLDFLAGS) $^ -o $@

clean:
	@rm $(progam) *.o *.a *.so -f
