#include <iostream>
#include <SDL2/SDL.h>
#include <map>
#include <queue>
#include <list>
#include "Input.h"
#include "Robot.h"
#include "Cats.h"

using namespace std;

bool quitEvent = false;


int main() {
  Robot robot;

  constexpr int width = 200;
  constexpr int height = 120;
  constexpr int tileWidth = 4;
  constexpr int tileHeight = 4;
  Cats::Init(width*tileWidth, height*tileHeight, 1.5f);

  Cats::LoadTileset("tiles.json");
  Cats::SetupTileLayer(width, height, tileWidth, tileHeight);

  int lastFrameTime = SDL_GetTicks();

  int x = width/2;
  int y = height/2;

  Cats::SetTile(x, y, "tiles", 4, 0);

  robot.draw_surroundings(x, y);

  while(!Input::instance.gotQuitEvent()) {
    // checkForInput();
    Input::instance.update();

    if(Input::instance.pressed(SDLK_LEFT)) {
      if(robot.move(3) != 0) {
	x -= 1;
	robot.draw_surroundings(x, y);
      }
    } else if(Input::instance.pressed(SDLK_RIGHT)) {
      if(robot.move(4) != 0) {
	x += 1;
	robot.draw_surroundings(x, y);
      }
    } else if(Input::instance.pressed(SDLK_UP)) {
      if(robot.move(1) != 0) {
	y -= 1;
	robot.draw_surroundings(x, y);
      }
    } else if(Input::instance.pressed(SDLK_DOWN)) {
      if(robot.move(2) != 0) {
	y += 1;
	robot.draw_surroundings(x, y);
      }
    }

    float delta = (SDL_GetTicks() - lastFrameTime) / 1000.0f;
    lastFrameTime = SDL_GetTicks();

    Cats::Redraw(delta);
  }

  return 0;
}
