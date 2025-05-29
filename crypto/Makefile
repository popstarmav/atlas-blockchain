CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -Wno-deprecated-declarations
AR = ar
ARFLAGS = rcs

SRCDIR = .
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
LIBRARY = libhblk_crypto.a

.PHONY: all clean

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -I$(SRCDIR) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(LIBRARY)
