// -*- mode: c++ -*-

#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL.h>
#include "Vector.h"

class Input {
public:
  Input();
  void update();
  Vector getDirection();
  bool gotQuitEvent();

  /* Checks if key has been pressed since last update (only returns
     'true' once for each keypress) */
  bool pressed(int key);

  /* Checks if key has been released since last update (only returns
     'true' once for each keyrelease) */
  bool released(int key);

  /* Checks for mouse click, returns 'true' if left mouse button has
     been pressed. Sets the x and y parameters to the position of the
     click */
  bool mouseButtonDown(int &x, int &y);

  /* Checks for mouse release, returns 'true' if left mouse button has
     been released. Sets the x and y parameters to the position of the
     release */
  bool mouseButtonUp(int &x, int &y);

  /* Checks for mouse movement, returning 'true' if mouse moved since
     last check. Sets the x and y parameters to the position of the
     mouse */
  bool mouseMovement(int &x, int &y);

  static Input instance;

private:
  float dx, dy;
  bool quitEvent;
  bool mouseClicked;
  bool mouseReleased;
  bool mouseMoved;
  int mouseX, mouseY;
};

#endif
