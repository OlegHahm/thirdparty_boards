MODULE =$(BOARD)_base

.PHONY: $(BINDIR)iot-lab_M3_drivers.a

all: $(BINDIR)iot-lab_M3_base.a 
	$(MAKE) -C drivers

$(BINDIR)iot-lab_M3_base.a: $(OBJ)
	$(AR) rcs $(BINDIR)iot-lab_M3_base.a $(OBJ)

include $(RIOTBASE)/Makefile.base

