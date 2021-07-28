# Utilización:
#
# Para producir el ejecutable, escribe en la consola:
# make
#
# Para limpiar el área de trabajo (eliminar los .o y los .out), escribe en la consola:
# make clean
#
# Para ejecutar el programa a través de make, escribe en la consola:
# make run
#
# También puedes ejecutarlo como siempre:
# ./salida.out

CC = gcc
CFLAGS = -Wall -Wextra -std=c99
INC_DIR = -I.
LDFLAGS =
LIBS =
SOURCES = main.c Btree.c
DEPS = defs.h item.h Btree.h 
EXECUTABLE = salida.out
OBJECTS = $(SOURCES:.c=.o)

# Descomentar la siguiente línea (quitar el #) cuando se utilice este makefile para generar la información de depuración (GDB):
CFLAGS += -ggdb

CFLAGS += -DDBG_HELP=1

.PHONY = all clean run doc

all: $(EXECUTABLE)

%.o: %.c $(DEPS)
	@echo "\nCreando al objeto: " $@
	$(CC) $(CFLAGS) $(INC_DIR) -c $< -o $@

$(EXECUTABLE): $(OBJECTS)
	@echo Creando el ejecutable...
	$(CC) $(LDFLAGS) $(CFLAGS) $(INC_DIR) -o $@ $^

clean:
	@echo Limpiando el tiradero...
	rm -rvf *.o $(EXECUTABLE) *.*~ *~ *.un~

run: all
	./$(EXECUTABLE)

doc:
	@echo Generando la documentación...
	doxygen ./Doxyfile
