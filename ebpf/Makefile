.DEFAULT_GOAL := all

# Verbosity
Q		:= @

# Target directory
TARGET	:= $(realpath $(shell pwd))/build
ifneq ($(shell mkdir -p "$(TARGET)" && cd "$(TARGET)" && pwd), $(TARGET))
$(error Couldn't create directory "$(TARGET)")
endif

# Compilation & Link
CC		:= clang
CFLAGS		:= -O2 -g \
 		-I$(TARGET) \
		-Werror=pass-failed
LDFLAGS		:= -lbpf


BPF_CFLAGS	:= \
		-target bpf \
		$(CFLAGS) \
		-D__TARGET_ARCH_x86 \
		-funroll-loops \

OBJ_BPF		= $(TARGET)/test_probe.bpf
OBJ_RUN		= $(TARGET)/test_probe_runner
OBJ		= $(OBJ_BPF) $(OBJ_RUN)

DEP		= $(OBJ_BPF:.bpf=.d) $(addsuffix .d,$(OBJ_RUN))

$(TARGET)/test_probe.bpf: test_probe.c $(TARGET)/vmlinux.h
$(TARGET)/test_probe_runner: test_probe_runner.c $(TARGET)/test_probe.skel.h 

.PHONY:	all
all: $(OBJ)

.PHONY: clean
clean:
	@echo "  RM $(shell basename $(TARGET))"
	@$(RM) -rf $(TARGET)

.PHONY: re
re: clean
	@$(MAKE) --no-print-directory

-include $(DEP)
$(TARGET)/%.bpf:
	@echo "  CC $(shell basename $@)"
	@$(CC) $(BPF_CFLAGS) -MD -c $< -o $@

$(TARGET)/%_runner:
	@echo "  CC $(shell basename $@)"
	@$(CC) $(LDFLAGS) $(CFLAGS) -MD $< -o $@

$(TARGET)/vmlinux.h:
	@echo "  BTF DUMP $(shell basename $@)"
	@bpftool btf dump file /sys/kernel/btf/vmlinux format c > $@

$(TARGET)/%.skel.h: $(TARGET)/%.bpf
	@echo "  GEN SKELETON $(shell basename $@)"
	@bpftool gen skeleton $< > $@

.SECONDARY:
