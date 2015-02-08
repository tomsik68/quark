#include "input.h"
#include <SDL.h>

Input::Input()
{
    quitRequested = false;
}

void Input::update()
{
    while(SDL_PollEvent(&event)){
        switch(event.type){
        /* MOUSE */

        case SDL_MOUSEMOTION:
            mouseX = event.motion.x;
            mouseY = event.motion.y;
            break;
        case SDL_MOUSEBUTTONDOWN:
            mouseState[event.button.button] = true;
            break;
        case SDL_MOUSEBUTTONUP:
            mouseState[event.button.button] = false;
            break;

        /* KEYS */
        case SDL_KEYDOWN:
            keyState[event.key.keysym.sym] = true;
            break;
        case SDL_KEYUP:
            keyState[event.key.keysym.sym] = false;
            break;

        /* SPECIAL */
        case SDL_QUIT:
            quitRequested = true;
            break;
        }
    }
}

bool Input::isQuitRequested()
{
    return quitRequested;
}

bool Input::isKeyDown(SDL_Keycode key)
{
    return keyState[key];
}

bool Input::isMouseDown(Uint8 button)
{
    return mouseState[button];
}

Sint32 Input::getMouseX()
{
    return mouseX;
}

Sint32 Input::getMouseY()
{
    return mouseY;
}

Input::~Input()
{

}

