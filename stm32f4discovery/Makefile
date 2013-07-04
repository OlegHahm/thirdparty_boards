SRC = $(wildcard *.c)
BINDIR = $(RIOTBOARD)/$(BOARD)/bin/
OBJ = $(SRC:%.c=$(BINDIR)%.o)## defines
DEP = $(SRC:%.c=$(BINDIR)%.d)

INCLUDES += -Iinclude/
INCLUDES += -I$(RIOTCPU)/cortex-m/CMSIS/Device/ST/STM32F4xx/include
INCLUDES += -I$(RIOTCPU)/cortex-m/CMSIS/include
INCLUDES += -I$(RIOTCPU)/stm32f407vgt6/include
INCLUDES += -I$(RIOTCPU)/cortex-m/STM32F4xx_StdPeriph_Driver/include

all: $(BINDIR)stm32f4discovery_base.a 
	$(MAKE) -C drivers

$(BINDIR)stm32f4discovery_base.a: $(OBJ)
	$(AR) rcs $(BINDIR)stm32f4discovery_base.a $(OBJ)

# pull in dependency info for *existing* .o files
-include $(OBJ:.o=.d)

# compile and generate dependency info
$(BINDIR)%.o: %.c
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(INCLUDES) $(BOARDINCLUDE) $(PROJECTINCLUDE) $(CPUINCLUDE) -c $*.c -o $(BINDIR)$*.o
	$(CC) $(CFLAGS) $(INCLUDES) $(BOARDINCLUDE) $(PROJECTINCLUDE) $(CPUINCLUDE) -MM $*.c > $(BINDIR)$*.d
	@printf "$(BINDIR)"|cat - $(BINDIR)$*.d > /tmp/riot_out && mv /tmp/riot_out $(BINDIR)$*.d

# remove compilation products
clean:
	${MAKE} -C drivers clean
	rm -f $(OBJ) $(DEP)

