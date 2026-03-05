#
# Copyright (c) 2015-2025, NVIDIA CORPORATION. All rights reserved.
#
# See LICENSE.txt for license information
#
include common.mk

# Set to 1 to enable MPI support (multi-process/multi-node)
MPI ?= 0
# e.g. Set to _mpi when using MPI=1
NAME_SUFFIX ?=
# Set to 1 to create and use libverifiable.so to reduce binary size
DSO ?= 0
ifeq ($(NCCL_TESTS_TOOLCHAIN),klx)
ENABLE_VERIFIABLE ?= 0
else
ENABLE_VERIFIABLE ?= 1
endif

.PHONY: build clean

BUILDDIR ?= ../build
ifneq ($(NCCL_HOME), "")
NVCUFLAGS += -I$(NCCL_HOME)/include/
NVLDFLAGS += -L$(NCCL_HOME)/lib
endif

ifeq ($(MPI), 1)
NVCUFLAGS += -DMPI_SUPPORT -I$(MPI_HOME)/include
NVLDFLAGS += -L$(MPI_HOME)/lib -L$(MPI_HOME)/lib64 -lmpi
endif
ifeq ($(MPI_IBM),1)
NVCUFLAGS += -DMPI_SUPPORT
NVLDFLAGS += -lmpi_ibm
endif
LIBRARIES += nccl
NVLDFLAGS += $(LIBRARIES:%=-l%)

DST_DIR := $(BUILDDIR)
SRC_FILES := $(wildcard *.cu)
OBJ_FILES := $(SRC_FILES:%.cu=${DST_DIR}/%.o)
BIN_FILES_LIST := all_reduce all_gather broadcast reduce_scatter reduce alltoall scatter gather sendrecv hypercube
BIN_FILES := $(BIN_FILES_LIST:%=${DST_DIR}/%_perf${NAME_SUFFIX})
BKCL_BIN_FILES_LIST := all_reduce all_gather broadcast reduce_scatter reduce
BKCL_BIN_FILES := $(BKCL_BIN_FILES_LIST:%=${DST_DIR}/%_perf${NAME_SUFFIX})
BKCL_PERF_TARGETS := $(BKCL_BIN_FILES_LIST:%=%_perf)

build: ${BIN_FILES}
# Dedicated targets for BKCL KLX flow.
$(BKCL_PERF_TARGETS): %_perf: ${DST_DIR}/%_perf${NAME_SUFFIX}
# BKCL shim supported perf binaries.
bkcl_perf: ${BKCL_BIN_FILES}

clean:
	rm -rf ${DST_DIR}

TEST_VERIFIABLE_SRCDIR := ../verifiable
TEST_VERIFIABLE_BUILDDIR := $(BUILDDIR)/verifiable
EXTRA_OBJS :=
ifeq ($(ENABLE_VERIFIABLE),1)
include ../verifiable/verifiable.mk
VERIFIABLE_DSO_LINK := -L$(TEST_VERIFIABLE_BUILDDIR) -lverifiable -Xlinker "--enable-new-dtags" -Xlinker "-rpath,\$$ORIGIN:\$$ORIGIN/verifiable"
else
VERIFIABLE_STUB_OBJ := ${DST_DIR}/verifiable_stub.o
EXTRA_OBJS += $(VERIFIABLE_STUB_OBJ)
TEST_VERIFIABLE_HDRS := $(TEST_VERIFIABLE_SRCDIR)/verifiable.h
TEST_VERIFIABLE_OBJS :=
TEST_VERIFIABLE_LIBS :=
VERIFIABLE_DSO_LINK :=
endif

.PRECIOUS: ${DST_DIR}/%.o

${DST_DIR}/%.o: %.cu common.h util.h $(TEST_VERIFIABLE_HDRS)
	@printf "Compiling  %-35s > %s\n" $< $@
	@mkdir -p ${DST_DIR}
	$(CUCOMPILE) -o $@ $(NVCUFLAGS) -c $<

${DST_DIR}/%$(NAME_SUFFIX).o: %.cu common.h util.h $(TEST_VERIFIABLE_HDRS)
	@printf "Compiling  %-35s > %s\n" $< $@
	@mkdir -p ${DST_DIR}
	$(CUCOMPILE) -o $@ $(NVCUFLAGS) -c $<

${DST_DIR}/timer.o: timer.cc timer.h
	@printf "Compiling  %-35s > %s\n" $< $@
	@mkdir -p ${DST_DIR}
	$(CXX) $(CXXFLAGS) -o $@ -c $<

ifeq ($(ENABLE_VERIFIABLE),0)
${DST_DIR}/verifiable_stub.o: verifiable_stub.cc $(TEST_VERIFIABLE_HDRS)
	@printf "Compiling  %-35s > %s\n" $< $@
	@mkdir -p ${DST_DIR}
	$(CXX) $(CXXFLAGS) $(NVCUFLAGS) -I$(TEST_VERIFIABLE_SRCDIR) -o $@ -c $<
endif

ifeq ($(DSO), 1)
${DST_DIR}/%_perf$(NAME_SUFFIX): ${DST_DIR}/%.o ${DST_DIR}/common$(NAME_SUFFIX).o ${DST_DIR}/util$(NAME_SUFFIX).o ${DST_DIR}/timer.o $(EXTRA_OBJS) $(TEST_VERIFIABLE_LIBS)
	@printf "Linking  %-35s > %s\n" $< $@
	@mkdir -p ${DST_DIR}
	$(CULINK) -o $@ $(NVCUFLAGS) $^ ${VERIFIABLE_DSO_LINK} ${NVLDFLAGS}
else
${DST_DIR}/%_perf$(NAME_SUFFIX):${DST_DIR}/%.o ${DST_DIR}/common$(NAME_SUFFIX).o ${DST_DIR}/util$(NAME_SUFFIX).o ${DST_DIR}/timer.o $(EXTRA_OBJS) $(TEST_VERIFIABLE_OBJS)
	@printf "Linking  %-35s > %s\n" $< $@
	@mkdir -p ${DST_DIR}
	$(CULINK) -o $@ $(NVCUFLAGS) $^ ${NVLDFLAGS}
endif

clean_intermediates:
	rm -f ${DST_DIR}/*.o $(TEST_VERIFIABLE_OBJS)
