/*
* Copyright (c) 2006, Ondrej Danek (www.ondrej-danek.net)
* All rights reserved.
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Ondrej Danek nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
* LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
* OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <SDL2/SDL.h>
#include "Format.h"
#include "Input.h"

namespace Duel6 {
    void Input::setPressed(SDL_Keycode keyCode, bool pressed) {
        if (pressed) {
            pressedKeys.insert(keyCode);
        } else {
            pressedKeys.erase(keyCode);
        }
    }

    void Input::joyScan(Console &console) {
        joypads.clear();

        if (!SDL_WasInit(SDL_INIT_JOYSTICK)) {
            console.printLine("...Starting joypad sub-system");
            if (SDL_InitSubSystem(SDL_INIT_JOYSTICK)) {
                console.printLine("...Unable to initialize joypad sub-system");
                return;
            }
        }

        Int32 joysticks = SDL_NumJoysticks();
        console.printLine(Format("...Found {0} joypads") << joysticks);

        for (Int32 i = 0; i < joysticks; i++) {
            auto joypad = SDL_JoystickOpen(i);
            joypads.push_back(joypad);
            console.printLine(Format("... * {0}") << SDL_JoystickName(joypad));
        }
    }
}