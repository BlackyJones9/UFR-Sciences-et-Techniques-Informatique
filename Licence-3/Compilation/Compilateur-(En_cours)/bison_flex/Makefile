SHELL=/bin/sh
LEX=flex
YACC=bison
CC=gcc
CFLAGS=-g -std=c2x -pedantic -Wall -Werror -Wwrite-strings -I../include \
#-DVERBOSE
OPT=
LDFLAGS=
# --nounput: ne génère pas la fonction yyunput() inutile
# --DYY_NO_INPUT: ne prend pas en compte la fonction input() inutile
# -D_POSIX_SOURCE: déclare la fonction fileno()
LEXOPTS=-D_POSIX_SOURCE -DYY_NO_INPUT --nounput
YACCOPTS=
PROG=compil
DIR=../includes
OBJDIR=$(DIR)/src

# Récupère tous les fichiers .c dans ../includes/ et ses sous-répertoires
INCLUDE_SRC=$(wildcard $(DIR)/*.c) $(wildcard $(DIR)/hashtable/*.c) $(wildcard $(DIR)/holdall/*.c)
# Génère les noms des fichiers objets dans ../includes/src/
INCLUDE_OBJ=$(patsubst $(DIR)/%.c,$(OBJDIR)/%.o,$(INCLUDE_SRC))

$(PROG): lex.yy.o $(PROG).tab.o $(INCLUDE_OBJ)
	$(CC) $+ -o $@ $(LDFLAGS)

lex.yy.c: $(PROG).l $(PROG).tab.h
	$(LEX) $(LEXOPTS) $<

lex.yy.h: $(PROG).l
	$(LEX) $(LEXOPTS) --header-file=$@ $<

$(PROG).tab.c $(PROG).tab.h: $(PROG).y lex.yy.h
	$(YACC) $(YACCOPTS) $< -d -v --graph

# Règle pour les fichiers .o locaux (lex.yy.o, compil.tab.o)
%.o: %.c
	$(CC) $(CFLAGS) $(OPT) $< -c -o $@

# Règle pour les fichiers .o de ../includes/ -> ../includes/src/
# Crée automatiquement les sous-répertoires nécessaires
$(OBJDIR)/%.o: $(DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(OPT) $< -c -o $@

clean:
	touch $(PROG).output $(PROG).vcg
	rm -f $(PROG) *.o lex.yy.* $(PROG).tab.*
	rm -rf $(OBJDIR)