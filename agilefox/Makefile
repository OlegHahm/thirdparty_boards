MODULE =$(BOARD)_base

.PHONY: $(BINDIR)agilefox_drivers.a

all: $(BINDIR)$(MODULE).a 
	$(MAKE) -C drivers

$(BINDIR)$(MODULE): $(OBJ)
	$(AR) rcs $(BINDIR)$(MODULE).a $(OBJ)

include $(RIOTBASE)/Makefile.base
