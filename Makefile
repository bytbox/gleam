CPPFLAGS = -g -O0 -Wall -Wextra -std=c++0x -DDIMS=3

all: gleam

clean:
	rm -f gleam

.PHONY: all clean
