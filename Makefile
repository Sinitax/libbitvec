PREFIX ?= /usr/local
INCLUDEDIR ?= /include
LIBDIR ?= /lib

CFLAGS = -I include -Wno-prototype -Wunused-function -Wunused-variable

ifeq "$(LIBBITVEC_DEBUG)" "1"
CFLAGS += -g
endif

ifeq "$(LIBBITVEC_ASSERT)" "1"
CFLAGS += -DLIBBITVEC_ASSERT_ENABLE=1
endif

ifeq "$(LIBBITVEC_HANDLE_ERR)" "1"
CFLAGS += -DLIBBITVEC_HANDLE_ERRS=1
endif

all: build/libbitvec.so build/libbitvec.a build/test

clean:
	rm -rf build

build:
	mkdir build

build/libbitvec.a: src/bitvec.c include/bitvec.h | build
	$(CC) -o build/tmp.o src/bitvec.c $(CFLAGS) -r
	objcopy --keep-global-symbols=libbitvec.abi build/tmp.o build/fixed.o
	ar rcs $@ build/fixed.o

build/libbitvec.so: src/bitvec.c include/bitvec.h | build
	$(CC) -o $@ src/bitvec.c -fPIC $(CFLAGS) -shared -Wl,-version-script libbitvec.lds

build/test: src/test.c build/libbitvec.a
	$(CC) -o $@ $^ $(CFLAGS)

install:
	install -m644 include/bitvec.h -t "$(DESTDIR)$(PREFIX)$(INCLUDEDIR)"
	install -m755 build/libbitvec.so -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"
	install -m755 build/libbitvec.a -t "$(DESTDIR)$(PREFIX)$(LIBDIR)"

uninstall:
	rm -f "$(DESTDIR)$(PREFIX)$(INCLUDEDIR)/bitvec.h"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/libbitvec.so"
	rm -f "$(DESTDIR)$(PREFIX)$(LIBDIR)/libbitvec.a"

.PHONY: all clean install uninstall
