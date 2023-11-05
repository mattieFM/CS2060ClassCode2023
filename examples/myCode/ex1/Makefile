CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk4)
LIBS = $(shell $(PKGCONFIG) --libs gtk4)
SRC = main.c



gcc $( pkg-config --cflags gtk4 ) -o example-0 main.c $( pkg-config --libs gtk4 )


