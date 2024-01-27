CC = gcc
FLAGS = -Wall -Werror

# List of C files
SOURCES = testCarnivoreBlocked.c \
          testCarnivoreOvertake.c \
          testVegetarianBlocked.c \
          testVegetarianOvertake.c \
          testMutualExclusionInKitchen.c \
          testSandwichBoth.c \
          testSandwichMeat.c \
          testSandwichTofu.c \
          testWaitingSwitch.c

# Generate corresponding target names (remove .c extension)
TARGETS = $(SOURCES:.c=)

# Default target to build all programs
all: $(TARGETS)

# Rule to build each program
%: %.c
	$(CC) $(FLAGS) -o $@ $<

# Clean up intermediate and executable files
clean:
	rm -f $(TARGETS)
