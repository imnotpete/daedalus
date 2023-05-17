/*
Copyright (C) 2006 StrmnNrmn

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*/



#include "stdafx.h"


#include "Graphics/GraphicsContext.h"
#include "Math/Vector2.h"
#include "SysPosix/UI/UIContext.h"
#include "SysPosix/UI/UIScreen.h"
//#include "SysPosix/Utility/Buttons.h"
#include "Utility/Timer.h"

#include <SDL2/SDL.h>


CUIScreen::CUIScreen(CUIContext* p_context)
	: mpContext(p_context)
{
}

CUIScreen::~CUIScreen()
{
}

void CUIScreen::Run()
{
#ifdef DAEDALUS_ENABLE_ASSERTS
	DAEDALUS_ASSERT(mpContext != nullptr, "No context");
#endif
	const u32 SDL_BUTTONS_MASK(0xff); // Mask off extra SDL2 buttons
	const u32 PSP_BUTTONS_MASK(0xffff); // Mask off extra PSP buttons

	SDL_Event event;
	SDL_PumpEvents();
	u32 old_buttons = SDL_GetMouseState(nullptr, nullptr);
	old_buttons |= SDL_GetGlobalMouseState(nullptr, nullptr);

	CTimer timer;

	// Simple rom chooser
	while (!IsFinished())
	{
		f32 elapsed_time(timer.GetElapsedSeconds());

		SDL_PumpEvents();
		u32 new_buttons = SDL_GetMouseState(nullptr, nullptr);
		new_buttons |= SDL_GetGlobalMouseState(nullptr, nullptr);

		s32 stick_x(0);
		s32 stick_y(0);
		const u8* state = SDL_GetKeyboardState(nullptr);
		if (state[SDL_SCANCODE_A]) {
			stick_x = -128;
		}
		if (state[SDL_SCANCODE_D]) {
			stick_x = 128;
		}
		if (state[SDL_SCANCODE_W]) {
			stick_y = -128;
		}
		if (state[SDL_SCANCODE_S]) {
			stick_y = 128;
		}
		/*
		if (stick_x >= -STICK_DEADZONE && stick_x <= STICK_DEADZONE)
		{
			stick_x = 0;
		}
		if (stick_y >= -STICK_DEADZONE && stick_y <= STICK_DEADZONE)
		{
			stick_y = 0;
		}
		*/
		v2 stick;
		stick.x = static_cast<f32>(stick_x) / 128.0f;
		stick.y = static_cast<f32>(stick_y) / 128.0f;

		mpContext->Update(elapsed_time);

		Update(elapsed_time, stick, old_buttons, new_buttons);

		mpContext->BeginRender();

		Render();

		mpContext->EndRender();

		old_buttons = new_buttons;
	}

	//
	//	Wait until all buttons are release before continuing
	//  We do this to avoid any undesirable button input after returning to the emulation from pause menu.
	//
	SDL_PumpEvents();
	while ((SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTONS_MASK) != 0 || SDL_GetGlobalMouseState(nullptr, nullptr) != 0)
	{
		SDL_PumpEvents();
	}
}
