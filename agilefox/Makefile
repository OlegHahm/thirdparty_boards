SRC = $(wildcard *.c)
BINDIR = $(RIOTBOARD)/$(BOARD)/bin/
OBJ = $(SRC:%.c=$(BINDIR)%.o)## defines
DEP = $(SRC:%.c=$(BINDIR)%.d)

INCLUDES += -Iinclude/
INCLUDES += -I$(RIOTCPU)/STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/Include
INCLUDES += -I$(RIOTCPU)/STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x
INCLUDES += -I$(RIOTCPU)/STM32F10x_StdPeriph_Lib_V3.5.0/Libraries/STM32F10x_StdPeriph_Driver/inc
INCLUDES += -I$(RIOTCPU)/stm32f103rey6/include

INCLUDES += -I$(RIOTBASE)/drivers/at86rf231/include
INCLUDES += -I$(RIOTBASE)/sys/include
INCLUDES += -I$(RIOTBASE)/sys/net
INCLUDES += -I$(RIOTBASE)/core/include

all: $(BINDIR)agilefox_base.a 
	$(MAKE) -C drivers

$(BINDIR)agilefox_base.a: $(OBJ)
	$(AR) rcs $(BINDIR)agilefox_base.a $(OBJ)

# pull in dependency info for *existing* .o files
-include $(OBJ:.o=.d)

# compile and generate dependency info
$(BINDIR)%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $(BOARDINCLUDE) $(PROJECTINCLUDE) $(CPUINCLUDE) -c $*.c -o $(BINDIR)$*.o
	$(CC) $(CFLAGS) $(INCLUDES) $(BOARDINCLUDE) $(PROJECTINCLUDE) $(CPUINCLUDE) -MM $*.c > $(BINDIR)$*.d
	@printf "$(BINDIR)"|cat - $(BINDIR)$*.d > /tmp/riot_out && mv /tmp/riot_out $(BINDIR)$*.d

# remove compilation products
clean:
	${MAKE} -C drivers clean
	rm -f $(OBJ) $(DEP)
