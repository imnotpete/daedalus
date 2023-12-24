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


#ifndef SYSPSP_UI_MAINMENUSCREEN_H_
#define SYSPSP_UI_MAINMENUSCREEN_H_

class CUIContext;

#include "Core/CPU.h"
#include "Core/ROM.h"
#include "Core/RomSettings.h"
#include "Interface/SaveState.h"
#include "Graphics/ColourValue.h"
#include "Graphics/GraphicsContext.h"
#include "Base/MathUtil.h"
#include "DrawTextUtilities.h"

#include "AboutComponent.h"
#include "GlobalSettingsComponent.h"

#include "PSPMenu.h"
#include "RomSelectorComponent.h"
#include "SelectedRomComponent.h"
#include "SavestateSelectorComponent.h"
#include "UIContext.h"
#include "UIScreen.h"
#include "SplashScreen.h"
#include "System/SystemInit.h"
#include "Interface/Preferences.h"


namespace
{

	enum EMenuOption
	{
		MO_GLOBAL_SETTINGS = 0,
		MO_ROMS,
		MO_SELECTED_ROM,
		MO_SAVESTATES,
		MO_ABOUT,
	};
	const s16 NUM_MENU_OPTIONS = MO_ABOUT + 1;

	const EMenuOption	MO_FIRST_OPTION = MO_GLOBAL_SETTINGS;
	const EMenuOption	MO_LAST_OPTION = MO_ABOUT;

	const char * const	gMenuOptionNames[ NUM_MENU_OPTIONS ] =
	{
		"Global Settings",
		"Roms",
		"Selected Rom",
		"Savestates",
		"About",
	};
}

class CMainMenuScreen :  public CUIScreen
{
	public:

		CMainMenuScreen( CUIContext * p_context );
		~CMainMenuScreen();
		static CMainMenuScreen *	Create( CUIContext * p_context );

		// CMainMenuScreen
		virtual void				Run();

		// CUIScreen
		virtual void				Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons );
		virtual void				Render();
		virtual bool				IsFinished() const									{ return mIsFinished; }


	private:
		static	EMenuOption			AsMenuOption( s32 option );

				EMenuOption			GetPreviousOption() const							{ return AsMenuOption( mCurrentOption - 1 ); }
				EMenuOption			GetCurrentOption() const							{ return AsMenuOption( mCurrentOption ); }
				EMenuOption			GetNextOption() const								{ return AsMenuOption( mCurrentOption + 1 ); }

				s32					GetPreviousValidOption() const;
				s32					GetNextValidOption() const;

				bool				IsOptionValid( EMenuOption option ) const;

				void				OnRomSelected( const char * rom_filename );
				void				OnSavestateSelected( const char * savestate_filename );
				void				OnStartEmulation();

	private:
		bool						mIsFinished;

		s32							mCurrentOption;
		f32							mCurrentDisplayOption;

		CUIComponent *				mOptionComponents[ NUM_MENU_OPTIONS ];
		CSelectedRomComponent *		mSelectedRomComponent;

		std::string					mRomFilename;
		RomID						mRomID;
};


void DisplayRomsAndChoose(bool show_splash);

#endif // SYSPSP_UI_MAINMENUSCREEN_H_
