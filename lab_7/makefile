CC = clang
CF = -g3 -O0 -Wall -std=c99 -I ./inc

OBJDIR = ./out
SRCDIR = ./src
INCDIR = ./inc

DEPS = $(wildcard $(INCDIR)/*.h)
SRC = $(wildcard $(SRCDIR)/*.c)
TEMP = $(patsubst %.c, %.o, $(SRC))
OBJECTS = $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(TEMP))

all: objdir main

main: $(OBJECTS)
	$(CC) $(CF) $^ -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) $(CF) -c $< -o $@

objdir:
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)
	rm -rf ./main
	rm -rf *SYM
	rm -rf *.jpeg
	rm -rf graphviz.txt
	rm -rf *.log

draw_graph:
	dot -Tjpeg graphviz.txt -o graph.jpeg
	open graph.jpeg
