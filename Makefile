ALL=makecards

MORE=

CCOPTS=-I. -D_GNU_SOURCE -g -Wall -funsigned-char -IAXL -IQR -IReedsol -IImage -g
OPTS=-LAXL ${CCOPTS}

all: $(ALL)

AXL/axl.o: AXL/axl.c
	make -C AXL
QR/iec18004.o: QR/iec18004.c
	make -C QR
Reedsol/reedsol.o: Reedsol/reedsol.c
	make -C Reedsol
Image/image.o: Image/image.c
	make -C Image
1dbar/1dbar.o: 1dbar/1dbar.c
	make -C 1dbar 1dbar.o

clean:
	rm -f $(ALL) $(MORE)

update:
	git submodule update --init --recursive --remote
	git commit -a -m "Library update"

makecards: makecards.c Makefile AXL/axl.o QR/iec18004.o 1dbar/1dbar.o Image/image.o Reedsol/reedsol.o court.h
	-indent -pmt $< 
	cc -O -o $@ $< ${OPTS} -lpopt -DMAIN AXL/axl.o QR/iec18004.o 1dbar/1dbar.o Image/image.o Reedsol/reedsol.o -lcurl -lz

makecourt: makecourt.c Makefile AXL/axl.o
	-indent -pmt $< 
	cc -O -o $@ $< ${OPTS} -lpopt -DMAIN AXL/axl.o -lcurl

SVGFILES := $(wildcard ../svg/??.svg)
court.h: makecourt ${SVGFILES}
	./makecourt
