
#=======================================================================
#=================== Settings ==========================================
#=======================================================================

KOKKOS_PATH = ${HOME}/Kokkos/kokkos
KOKKOSKERNELS_PATH = ${HOME}/Kokkos/kokkos-kernels
GTEST_PATH = ${KOKKOS_PATH}/tpls/gtest

KOKKOS_DEVICES=OpenMP
KOKKOS_CUDA_OPTIONS=enable_lambda

KOKKOSKERNELS_SCALARS="'double,complex<double>'"
KOKKOSKERNELS_OPTIONS=eti-only


ifneq (,$(findstring Cuda,$(KOKKOS_DEVICES)))
CXX = ${KOKKOS_PATH}/config/nvcc_wrapper
else
CXX = g++
endif

CXXFLAGS = -O3 -g
LINK = ${CXX}
LINKFLAGS =  

DEPFLAGS = -M

OBJ =
LIB = -lpthread


#=======================================================================
#=================== Include Kokkos and KokkosKernels ==================
#=======================================================================

#include $(KOKKOS_PATH)/Makefile.kokkos
include $(KOKKOSKERNELS_PATH)/Makefile.kokkos-kernels

#=======================================================================
#=================== Add common source files ===========================
#=======================================================================


TEST_TARGETS =
TARGETS =

#=======================================================================
#===================== SPARSE PERF =====================================
#=======================================================================
INC  = -I$(KOKKOSKERNELS_PATH)/perf_test/
BLASINC = -I$(KOKKOSKERNELS_PATH)/perf_test/blas
INC += $(BLASINC)
SPARSEINC = -I$(KOKKOSKERNELS_PATH)/perf_test/sparse
SPARSEINC += -I$(KOKKOSKERNELS_PATH)/perf_test/sparse/spmv
INC += $(SPARSEINC)
GRAPHINC = -I$(KOKKOSKERNELS_PATH)/perf_test/graph
INC += $(GRAPHINC)
COMMONINC = -I$(KOKKOSKERNELS_PATH)/perf_test/common
INC += $(COMMONINC)


TEST_HEADERS =  $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/*.hpp)
BLAS_HEADERS = $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/blas/*.hpp)
TEST_HEADERS += $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/blas/*.hpp)

SPARSE_HEADERS = $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/sparse/*.hpp)
SPARSE_HEADERS += $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/sparse/spmv/*.hpp)
TEST_HEADERS += $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/sparse/*.hpp)
TEST_HEADERS += $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/sparse/spmv/*.hpp)
GRAPH_HEADERS = $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/graph/*.hpp)
TEST_HEADERS += $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/graph/*.hpp)
COMMON_HEADERS = $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/common/*.hpp)
TEST_HEADERS += $(wildcard $(KOKKOSKERNELS_PATH)/perf_test/common/*.hpp)

#removing this for now, as those blas tests have teuchos dependency.
#EXECUTABLES = $(wildcard ${KOKKOSKERNELS_PATH}/perf_test/blas/*cpp) 

EXECUTABLES += $(wildcard ${KOKKOSKERNELS_PATH}/perf_test/sparse/*cpp)
EXECUTABLES += $(wildcard ${KOKKOSKERNELS_PATH}/perf_test/graph/*cpp) 
EXECUTABLES += $(wildcard ${KOKKOSKERNELS_PATH}/perf_test/common/*cpp) 

vpath %.cpp $(sort $(dir $(EXECUTABLES)))
OBJS = $(notdir $(EXECUTABLES:.cpp=.o))
TEST_TARGETS = $(notdir $(EXECUTABLES:.cpp=.exe))

#TEST_TARGETS = $(patsubst %.cpp, %.exe, $(EXECUTABLES))
#=======================================================================
#=================== Make Rules ========================================
#=======================================================================


clean: kokkos-clean kokkoskernels-clean
	rm -f *.o *.exe

default: $(TEST_TARGETS)

build: $(TEST_TARGETS)
	
%.exe:%.o $(KOKKOS_LINK_DEPENDS) $(KOKKOSKERNELS_LINK_DEPENDS) $(TEST_HEADERS)
	$(LINK) $(EXTRA_PATH) $< $(KOKKOSKERNELS_LIBS) $(KOKKOS_LIBS) $(LIB) $(KOKKOS_LDFLAGS) $(LDFLAGS) -o $@

%.o:%.cpp $(KOKKOS_CPP_DEPENDS) $(KOKKOSKERNELS_CPP_DEPENDS) $(TEST_HEADERS) 
	$(CXX) $(KOKKOS_CPPFLAGS) $(KOKKOSKERNELS_CPPFLAGS) $(KOKKOS_CXXFLAGS) $(INC) $(CXXFLAGS) $(EXTRA_INC) -I. -c $< -o $(notdir $@)

#depend:
#	makedepend -Y ${EXECUTABLES} $(TEST_HEADERS)
# DO NOT DELETE