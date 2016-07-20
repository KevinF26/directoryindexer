COMPILER = gcc
CCFLAGS  = -Wall -pedantic
all:  index

index: libsl.a toklib.a
	$(COMPILER) $(CCFLAGS) -o index index.c libsl.a toklib.a -I.
toklib.a: modifiedtokenizer.c
	$(COMPILER) $(CCFLAGS) -o toklib.a -c modifiedtokenizer.c -I .
libsl.a: sorted-list.c sorted-list.h
	$(COMPILER) $(CCFLAGS) -o libsl.a -c sorted-list.c -I.
clean:
	rm -f libsl.a toklib.a index

