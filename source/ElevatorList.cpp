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

#include <SDL/SDL_opengl.h>
#include "Player.h"
#include "ElevatorList.h"
#include "Weapon.h"

#define D6_ELEV_TEXTURE         68

namespace Duel6
{
	static std::vector<Elevator> d6Elevators;

	void ELEV_Init()
	{
		glBindTexture(GL_TEXTURE_2D, d6WpnTextures[D6_ELEV_TEXTURE]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	void ELEV_Clear()
	{
		d6Elevators.clear();
	}

	void ELEV_Add(Elevator& elevator)
	{
		d6Elevators.push_back(elevator);
		d6Elevators.back().start();
	}

	void ELEV_MoveAll(Float32 elapsedTime)
	{
		for (Elevator& elevator : d6Elevators)
		{
			elevator.update(elapsedTime);
		}
	}

	void ELEV_DrawAll()
	{
		glBindTexture(GL_TEXTURE_2D, d6WpnTextures[D6_ELEV_TEXTURE]);
		glBegin(GL_QUADS);

		for (const Elevator& elevator : d6Elevators)
		{
			elevator.render();
		}

		glEnd();
	}

	void ELEV_CheckMan(Player& player)
	{
		Float32 x = player.getX() + 0.5f;
		Float32 y = player.getY();

		for (const Elevator& elevator : d6Elevators)
		{
			if (x < elevator.getX() || x > elevator.getX() + 1.0f ||
				y < elevator.getY() - 0.05f || y > elevator.getY() + 0.05f)
				continue;

			player.assignElevator(elevator);
			return;
		}
	}
}
