
OPT = -O0 -g
INTEL=/cgg/intel/compiler_xe_2015_update1/
CXX=${INTEL}/bin/icpc
CXXFLAGS= -std=c++11 ${OPT} -I/RD_work/ywang/blaze_cgg/ -I/RD_work/ywang/blaze_cgg/blaze
LDFLAGS = -L/RD_work/ywang/LOT -llot


#funtest:functortest.cpp

test:test.o liblot.a
	${CXX} test.o -o test ${LDFLAGS}

test.o:test.cpp

liblot.a: CGMin.o
	ar cr liblot.a CGMin.o

CGMin.o : CGMin.cpp 

clean:
	rm *.o *.a *~
