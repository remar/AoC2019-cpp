#include <iostream>
#include <SDL2/SDL.h>

#include "../Intcomputer.h"

#include "Cats.h"

using namespace std;

bool quitEvent = false;

void checkForInput() {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    if(event.type == SDL_QUIT) {
      quitEvent = true;
    }
  }
}

int main() {
  pipe input, output;
  input.name = "input";
  output.name = "output";
  Intcomputer puter(input, output);
  puter.load_program_from_file("13.txt");
  puter.write_value(0, 2);

  constexpr int width = 40;
  constexpr int height = 20;

  Cats::Init(width*16, height*16);

  Cats::LoadTileset("tiles.json");
  Cats::SetupTileLayer(width, height, 16, 16);

  for(int y = 0;y < height;y++) {
    for(int x = 0;x < width;x++) {
      Cats::SetTile(x, y, "tiles", 0, 0);
    }
  }

  int lastFrameTime = SDL_GetTicks();

  int ball_x, paddle_x;

  input.set_instrumentation(0);

  while(!quitEvent) {
    checkForInput();

    // run some iterations of the puter here
    // collect output, provide input when needed
    for(int i = 0;i < 10;i++) {
      puter.tick();
    }

    while(output.buffer_length() >= 3) {
      int x = output.read();
      int y = output.read();
      long long int val = output.read();
      if(x == -1 && y == 0) {
	cout << "SCORE: " << val << endl;
      } else {
	Cats::SetTile(x, y, "tiles", val, 0);
	if(val == 3) {
	  paddle_x = x;
	} else if(val == 4) {
	  ball_x = x;
	  input.set_instrumentation(paddle_x > ball_x ? -1 : (paddle_x < ball_x ? +1 : 0));
	}
      }
    }

    float delta = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
    lastFrameTime = SDL_GetTicks();

    Cats::Redraw(delta);
  }

  return 0;
}
