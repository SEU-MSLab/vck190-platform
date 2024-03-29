# Targets
TARGETS:=

# Subdirectories
SUBDIRS = fpga
SUBDIRS_CLEAN = $(patsubst %,%.clean,$(SUBDIRS))

# Rules
.PHONY: all
all: $(SUBDIRS) $(TARGETS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	cd $@ && $(MAKE)

.PHONY: $(SUBDIRS_CLEAN)
$(SUBDIRS_CLEAN):
	cd $(@:.clean=) && $(MAKE) clean

.PHONY: clean
clean: $(SUBDIRS_CLEAN)
	-rm -rf $(TARGETS)

program:
	cd $(SUBDIRS) && $(MAKE) $@
	xsct -eval "connect ; fpga fpga/fpga.bit"
