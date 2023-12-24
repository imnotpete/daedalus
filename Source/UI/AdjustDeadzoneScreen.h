/*
Copyright (C) 2007 StrmnNrmn

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


#ifndef SYSPSP_UI_ADJUSTDEADZONESCREEN_H_
#define SYSPSP_UI_ADJUSTDEADZONESCREEN_H_

#include "UIContext.h"
#include "UIScreen.h"
#include "PSPMenu.h"

#include "Input/InputManager.h"
#include "Math/Math.h"	// VFPU Math
#include "Base/MathUtil.h"
#include "Math/Vector2.h"
#include "DrawTextUtilities.h"
#include "Interface/Preferences.h"
#include "Utility/Translate.h"


class CAdjustDeadzoneScreen : public CUIScreen
{
	public:

		CAdjustDeadzoneScreen( CUIContext * p_context );
		static CAdjustDeadzoneScreen* Create( CUIContext * p_context );

		~CAdjustDeadzoneScreen();

		 void				Run();
		 void				Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons );
		 void				Render();
		 bool				IsFinished() const									{ return mIsFinished; }

	private:
		void						DrawCircle( s32 x, s32 y, s32 r, c32 colour );
		void						DrawCrosshair( s32 x, s32 y, c32 colour );
		void						DrawStick( s32 x, s32 y, s32 r, const v2 & stick, f32 min_deadzone, f32 max_deadzone );

	private:
		bool						mIsFinished;
		v2							mPspStick;
		v2							mN64Stick;
		bool						mAdjustingMinDeadzone;
		f32							mStickMinDeadzone;
		f32							mStickMaxDeadzone;
};

#endif // SYSPSP_UI_ADJUSTDEADZONESCREEN_H_
