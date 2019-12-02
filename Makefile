all: $(patsubst %.cpp, %.out, $(wildcard *.cpp))

%.out: %.cpp Makefile
	g++ $< -o $@ -ggdb -std=c++11

clean:
	@rm -f *~ *.out
