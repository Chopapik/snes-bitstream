PVSNESLIB_HOME ?= $(HOME)/snesdev/pvsneslib

export ROMNAME := snes
SRC = ./src

.PHONY: bitmaps all clean
all: bitmaps $(ROMNAME).sfc

# Font: PNG → .pic + .pal (GFXCONV from devkitsnes)
pvsneslibfont.pic: pvsneslibfont.png
	@echo convert font ... $(notdir $@)
	$(GFXCONV) -s 8 -o 16 -u 16 -p -e 0 -i $<

bitmaps: pvsneslibfont.pic

include $(PVSNESLIB_HOME)/devkitsnes/snes_rules

# clean runs targets from snes_rules (cleanBuildRes, cleanRom, cleanGfx)
# and removes generated font files
clean: cleanBuildRes cleanRom cleanGfx
	rm -f pvsneslibfont.pic pvsneslibfont.pal
