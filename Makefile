CC	= ../../bin/lcc -Wa-l -Wl-m -Wl-j
EMULATOR = VisualBoyAdvance
EMULATOR_FLAGS = -4 --ifb-motion-blur
BIN	= shmup.gb

all:	$(BIN)

run:	$(BIN)
	$(EMULATOR) $(EMULATOR_FLAGS) $(BIN)

make.bat: Makefile
	echo "REM Automatically generated from Makefile" > make.bat
	make -sn | sed y/\\//\\\\/ | grep -v make >> make.bat

%.o:	%.c
	$(CC) -c -o $@ $<

%.s:	%.c
	$(CC) -S -o $@ $<

%.o:	%.s
	$(CC) -c -o $@ $<

%.gb:	%.o
	$(CC) -o $@ $<

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm
