SRC = $(wildcard *.c)
BINDIR = ../bin/
OBJ = $(SRC:%.c=$(BINDIR)%.o)
DEP = $(SRC:%.c=$(BINDIR)%.d)

INCLUDES = -I../include 
INCLUDES += -I $(RIOTBASE)/drivers/cc110x
INCLUDES += -I $(RIOTBASE)/sys/include
INCLUDES += -I$(RIOTCPU)/$(CPU)/drivers/include
INCLUDES += -I$(RIOTCPU)/cortex-m/CMSIS/Device/ST/STM32F4xx/include
INCLUDES += -I$(RIOTCPU)/cortex-m/CMSIS/include
INCLUDES += -I$(RIOTCPU)/stm32f407vgt6/include
INCLUDES += -I$(RIOTCPU)/cortex-m/STM32F4xx_StdPeriph_Driver/include

$(BINDIR)stm32f4discovery_drivers.a: $(OBJ)
	$(AR) rcs $(BINDIR)stm32f4discovery_drivers.a $(OBJ)

# pull in dependency info for *existing* .o files
-include $(OBJ:.o=.d)

# compile and generate dependency info
$(BINDIR)%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(BOARDINCLUDE) $(PROJECTINCLUDE) $(CPUINCLUDE) -c $*.c -o $(BINDIR)$*.o
	$(CC) $(CFLAGS) $(INCLUDES) $(BOARDINCLUDE) $(PROJECTINCLUDE) $(CPUINCLUDE) -MM $*.c > $(BINDIR)$*.d
	@printf "$(BINDIR)" | cat - $(BINDIR)$*.d > /tmp/fw_out && mv /tmp/fw_out $(BINDIR)$*.d

# remove compilation products

clean:
	rm -f $(OBJ) $(DEP)



