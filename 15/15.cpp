#include <iostream>
#include <SDL2/SDL.h>
#include <map>

#include "../Intcomputer.h"

#include "Cats.h"

using namespace std;

bool quitEvent = false;

pipe input;

int direction;

struct point {
  int x, y;
};

struct visited {
  visited *parent;
  point p;
};

map<int,point> dir_map = {
  {0, {0, 0}},
  {1, {0, -1}},
  {2, {0, +1}},
  {3, {-1, 0}},
  {4, {+1, 0}},
};

void checkForInput() {
  SDL_Event event;

  while(SDL_PollEvent(&event)) {
    if(event.type == SDL_QUIT) {
      quitEvent = true;
    } else if(event.type == SDL_KEYDOWN && event.key.repeat == 0) {
      switch(event.key.keysym.sym) {
      case SDLK_UP:
	direction = 1;
	break;

      case SDLK_DOWN:
	direction = 2;
	break;

      case SDLK_LEFT:
	direction = 3;
	break;

      case SDLK_RIGHT:
	direction = 4;
	break;
      }
      input.write(direction);
    }
  }
}

int main() {
  pipe output;
  input.name = "input";
  output.name = "output";
  Intcomputer puter(input, output);
  puter.load_program_from_file("15.txt");

  constexpr int width = 50;
  constexpr int height = 30;
  Cats::Init(width*16, height*16);

  Cats::LoadTileset("tiles.json");
  Cats::SetupTileLayer(width, height, 16, 16);

  int lastFrameTime = SDL_GetTicks();

  // input.set_instrumentation(0);
  int x = width/2;
  int y = height/2;

  Cats::SetTile(x, y, "tiles", 4, 0);

  while(!quitEvent) {
    checkForInput();

    // run some iterations of the puter here
    // collect output, provide input when needed
    for(int i = 0;i < 10;i++) {
      puter.tick();
    }

    if(output.input_ready()) {
      int res = output.read();
      if(res == 0) {
	// Wall
	auto d = dir_map[direction];
	cout << "put wall tile at " << (x + d.x) << "," << (y + d.y) << endl;
	Cats::SetTile(x + d.x, y + d.y, "tiles", 1, 0);
      } else if(res == 1) {
	// Move to new spot, draw ball there, and draw white square
	// where ball was
	Cats::SetTile(x, y, "tiles", 0, 0);
	x += dir_map[direction].x;
	y += dir_map[direction].y;
	Cats::SetTile(x, y, "tiles", 4, 0);
      }
      cout << "res: " << res << endl;
    }

    float delta = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
    lastFrameTime = SDL_GetTicks();

    Cats::Redraw(delta);
  }

  return 0;
}
