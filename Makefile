DIMS = 3
CPPFLAGS = -g -O0 -Wall -Wextra -std=c++0x -DDIMS=${DIMS}

all: gleam

clean:
	rm -f gleam

.PHONY: all clean
