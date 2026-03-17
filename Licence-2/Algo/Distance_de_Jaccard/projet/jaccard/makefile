hashtable_dir = ../hashtable/
holdall_dir = ../holdall/
op_dir = ../op/
avl_dir = ../avl/
CC = gcc
CFLAGS = -std=c2x \
  -Wall -Wconversion -Werror -Wextra -Wpedantic -Wwrite-strings \
  -O2 \
  -I$(hashtable_dir) -I$(holdall_dir) -I$(op_dir) -I$(avl_dir)\
  -DxWANT_HASHTABLE_EXT
vpath %.c $(hashtable_dir) $(holdall_dir) $(op_dir) $(avl_dir)
vpath %.h $(hashtable_dir) $(holdall_dir) $(op_dir) $(avl_dir)
objects = main.o hashtable.o holdall.o op.o bst.o
executable = jdis
makefile_indicator = .\#makefile\#

.PHONY: all clean

all: $(executable)

clean:
	$(RM) $(objects) $(executable)
	@$(RM) $(makefile_indicator)

$(executable): $(objects)
	$(CC) $(objects) -o $(executable)

main.o: main.c hashtable.h hashtable_ip.h holdall.h holdall_ip.h op.h op_ip.h bst.h bst_ip.h
hashtable.o: hashtable.c hashtable.h hashtable_ip.h
holdall.o: holdall.c holdall.h holdall_ip.h
op.o: op.c op.h
bst.o: bst.c bst.h bst_ip.h

include $(makefile_indicator)

$(makefile_indicator): makefile
	@touch $@
	@$(RM) $(objects) $(executable)
