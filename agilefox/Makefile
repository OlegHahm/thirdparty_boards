MODULE =$(BOARD)_base

.PHONY: $(BINDIR)agilefox_drivers.a

all: $(BINDIR)agilefox_base.a 
	$(MAKE) -C drivers

$(BINDIR)agilefox_base.a: $(OBJ)
	$(AR) rcs $(BINDIR)agilefox_base.a $(OBJ)

include $(RIOTBASE)/Makefile.base
