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


#ifndef SYSPSP_UI_CHEATOPTIONSSCREEN_H_
#define SYSPSP_UI_CHEATOPTIONSSCREEN_H_

#include "PSPMenu.h"
#include "UIContext.h"
#include "UIScreen.h"
#include "UISetting.h"
#include "UISpacer.h"
#include "UICommand.h"

#include "Config/ConfigOptions.h"
#include "Core/Cheats.h"
#include "Core/ROM.h"
#include "Core/RomSettings.h"
#include "Graphics/ColourValue.h"
#include "Input/InputManager.h"
#include "DrawTextUtilities.h"
#include "System/IO.h"
#include "Interface/Preferences.h"
#include "Utility/Stream.h"



class CUIContext;
class RomID;


class CCheatOptionsScreen : public CUIScreen
{
	public:

		CCheatOptionsScreen( CUIContext * p_context, const RomID & rom_id );
		~CCheatOptionsScreen();
		static CCheatOptionsScreen *	Create( CUIContext * p_context, const RomID & rom_id );

		// CCheatOptionsScreen
		 void				Run();

		// CUIScreen
		 void				Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons );
		 void				Render();
		 bool				IsFinished() const									{ return mIsFinished; }

	private:
		void				OnConfirm();
		void				OnCancel();
		RomID						mRomID;
		std::string					mRomName;
		SRomPreferences				mRomPreferences;
		bool						mIsFinished;
		CUIElementBag				mElements;
};

#endif // SYSPSP_UI_CHEATOPTIONSSCREEN_H_
