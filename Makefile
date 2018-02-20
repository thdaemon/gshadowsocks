PREFIX :=
INSTALLDIR := /usr/bin
INSTALLDIR_LIB := /usr/lib/gshadowsocks
DATADIR := /usr/share/gshadowsocks

OBJS := main.o path.o srvwin.o

INSTALL_BINS := gssocks
INSTALL_BINS_LIB := proxy-helper

CROSS :=
CC := gcc
STRIP := strip

LDFLAGS := `pkg-config --libs gtk+-3.0 appindicator3-0.1`

ifeq "$(DEBUG)" "1"
	CFLAGS := -Wall -std=c99 -g -DDEBUG=1 `pkg-config --cflags gtk+-3.0 appindicator3-0.1` 
	STRIP := ""
else
	CFLAGS := -Wall -O2 -std=c99 `pkg-config --cflags gtk+-3.0 appindicator3-0.1`
endif

all : gssocks

gssocks : $(OBJS)
	@echo "  LD      $@"
	@$(CROSS)$(CC) -o $@ $(OBJS) $(LDFLAGS) -rdynamic

ifneq "$(DEBUG)" "1"
	@echo "  STRIP   $@"
	@$(CROSS)$(STRIP) -s $@
endif

%.o : %.c config.h
	@echo "  CC      $@"
	@$(CROSS)$(CC) -c -o $@ $(CFLAGS) $< -rdynamic

config.h :
	@echo "Please run ./mkconfig.sh to generate a config header." >&2
	@false

install : all
	mkdir -p $(PREFIX)$(INSTALLDIR)
	cp $(INSTALL_BINS) $(PREFIX)$(INSTALLDIR)/
	chmod a+x $(INSTALL_BINS)
	cp $(INSTALL_BINS) $(PREFIX)$(INSTALLDIR_LIB)/
	chmod a+x $(INSTALL_BINS_LIB)
	mkdir -p $(PREFIX)$(DATADIR)
	cp share/* $(PREFIX)$(DATADIR)

deb : all
	./pkgsrc/mkdeb.sh

defconfig :
	./mkconfig.sh

clean:
	rm -f *.o gssocks config.h *.so
	rm -rf build-package
