# --- Compilateur et flags ---
CC = gcc
CFLAGS = -std=c2x -D_XOPEN_SOURCE=700 -Wall -Wextra -Wconversion -Werror \
         -fstack-protector-all -fpie -pie -O2 -D_FORTIFY_SOURCE=2 \
         -MMD -MP -Iinclude -I../Commun

LDFLAGS = -lm

# --- Nom de l'exécutable ---
TARGET = build/serveur

# --- Sources ---
SRC_SRCS = $(wildcard src/*.c)
COMMON_SRCS = $(wildcard ../Commun/*.c)

# --- Objets : tous dans build/src ---
SRC_OBJS = $(SRC_SRCS:src/%.c=build/src/%.o)
COMMON_OBJS = $(patsubst ../Commun/%.c,build/src/%.o,$(COMMON_SRCS))
OBJS = $(SRC_OBJS) $(COMMON_OBJS)
DEPS = $(OBJS:.o=.d)

# --- Cible par défaut ---
all: $(TARGET)

# --- Compilation de l'exécutable ---
$(TARGET): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# --- Compilation des objets depuis src/ ---
build/src/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Compilation des objets depuis ../Commun/ ---
build/src/%.o: ../Commun/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# --- Inclure les dépendances automatiques ---
-include $(DEPS)

# --- Nettoyage ---
clean:
	rm -rf build/*

distclean: clean
	rm -f *~

.PHONY: all clean distclean