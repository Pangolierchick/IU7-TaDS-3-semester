CC = gcc-10
CF = -g -Wall -Wextra -pedantic -std=c99 -I ./include
OBJDIR = ./out
SRCDIR = ./src
DEPSDIR = ./include

DEPS = $(wildcard $(DEPSDIR)/*.h)
SRC = $(wildcard $(SRCDIR)/*.c)
TEMP = $(patsubst %.c, %.o, $(SRC))
OBJECTS = $(patsubst $(SRCDIR)%, $(OBJDIR)%, $(TEMP))

compile: objdir main


main: $(OBJECTS)
	$(CC) $(CF) $^ -o $@
	@echo "Build done."

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(DEPS)
	$(CC) $(CF) $< -c -o $@

objdir:
	@mkdir -p $(OBJDIR)

clean:
	rm -rf main $(OBJDIR)
	rm -rf ./tests_res
	rm -rf ./test_build
	@echo "Cleaned!"


test:
	mkdir -p tests_res
	./init_tests
	cppcheck --enable=all . 2> ./tests_res/cppcheck.log
	echo "Check cppcheck.log for cppcheck information"
