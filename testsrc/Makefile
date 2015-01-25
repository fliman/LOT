
LOTDEBUG = -DLOT_DEBUG
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)
DEPS = $(SRCS:.cpp=.deps)
APPS = $(OBJS:%.o=%) 
OPT = -O0 -g ${LOTDEBUG}
INTEL=/cgg/intel/compiler_xe_2015_update1/
CXX=${INTEL}/bin/icpc
CXXFLAGS= -std=c++11 ${OPT} -I/RD_work/ywang/LOT/ -I/RD_work/ywang/blaze_cgg/ -I/RD_work/ywang/blaze_cgg/blaze
LDFLAGS = -L/RD_work/ywang/LOT 


#funtest:functortest.cpp
.PHONY : clean

all: ${APPS}

${APPS}:${OBJS}
	${CXX} $@.o -o $@ ${LDFLAGS}
#test:test.o 
#	${CXX} test.o -o test ${LDFLAGS}

%.o:%.cpp %.deps
	${CXX} ${CXXFLAGS} -c -o $@ $<

%.deps: %.cpp
	${CXX} ${CXXFLAGS} -c -M $< > $@

clean:
	echo ${APPS}; rm *.o *.a *~ ${APPS}
