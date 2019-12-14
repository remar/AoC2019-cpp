SDL2CONFIG := sdl2-config
CATSDIR := ../../cats/cats-cpp/src
CPPFILES := $(wildcard *.cpp)
DEPFILES := $(patsubst %.cpp,%.d,$(CPPFILES))
CXXFLAGS += -I${CATSDIR} `$(SDL2CONFIG) --cflags` -std=c++11 -ggdb
OFILES := $(CPPFILES:.cpp=.o)


13.out: ${OFILES}
	$(CXX) -o $@ $^ -L${CATSDIR} -lcats `$(SDL2CONFIG) --libs` -lSDL2_image

-include $(DEPFILES)

%.o: %.cpp Makefile
	$(CXX) $(CXXFLAGS) -MMD -MP -c $< -o $@

clean:
	@rm -f *.o TheSpaceGame *.d
