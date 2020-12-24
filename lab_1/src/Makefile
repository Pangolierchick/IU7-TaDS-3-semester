CC = gcc
CF = -Wall -Wextra -pedantic -std=c99
OBJDIR = ./obj
OBJECTS = $(OBJDIR)/myio.o $(OBJDIR)/main.o $(OBJDIR)/bigfloat.o $(OBJDIR)/error.o

compile: objdir main

main: $(OBJECTS)
	$(CC) $(CF) $(OBJECTS) -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CF) $< -c -o $@

objdir:
	mkdir -p ./obj

clean:
	rm -rf main $(OBJDIR)
