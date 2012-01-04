all:
	cabal build
	cp dist/build/gleam/gleam .

clean:
	cabal clean
	rm gleam
