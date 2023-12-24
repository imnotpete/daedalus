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


#ifndef SYSPSP_UI_PAUSEOPTIONSCOMPONENT_H_
#define SYSPSP_UI_PAUSEOPTIONSCOMPONENT_H_

#include "UIComponent.h"

#include "Core/CPU.h"
#include "Core/Dynamo.h"
#include "Core/ROM.h"
#include "Interface/SaveState.h"
#include "HLEGraphics/DisplayListDebugger.h"
#include "Graphics/ColourValue.h"
#include "Graphics/GraphicsContext.h"
#include "DrawTextUtilities.h"
#include "AdvancedOptionsScreen.h"
#include "SavestateSelectorComponent.h"
#include "CheatOptionsScreen.h"
#include "Dialogs.h"
#include "PauseOptionsComponent.h"
#include "PSPMenu.h"
#include "RomPreferencesScreen.h"
#include "UIContext.h"
#include "UIScreen.h"
#include "UISetting.h"
#include "UICommand.h"
#include "UISpacer.h"
#include "Utility/Functor.h"
#include "System/IO.h"


extern bool gTakeScreenshot;
extern bool gTakeScreenshotSS;

class CFunctor;

class CPauseOptionsComponent : public CUIComponent
{
	public:

		CPauseOptionsComponent( CUIContext * p_context, CFunctor * on_resume, CFunctor * on_reset );
		~CPauseOptionsComponent();
		static CPauseOptionsComponent *	Create( CUIContext * p_context, CFunctor * on_resume, CFunctor * on_reset );
		// CUIComponent
		 void				Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons );
		 void				Render();

	private:
				void				OnResume();
				void				OnReset();
				void				EditPreferences();
				void				AdvancedOptions();
				void				CheatOptions();
				void				SaveState();
				void				LoadState();
				void				TakeScreenshot();
#ifdef DAEDALUS_DIALOGS
				void				ExitConfirmation();
#endif
				void				OnSaveStateSlotSelected( const char * filename );
				void				OnLoadStateSlotSelected( const char * filename );

#ifdef DAEDALUS_DEBUG_DISPLAYLIST
				void				DebugDisplayList();
#endif
#ifdef DAEDALUS_KERNEL_MODE
				void				ProfileNextFrame();
#endif

	private:
		CFunctor *					mOnResume;
		CFunctor *					mOnReset;

		CUIElementBag				mElements;
};



#endif // SYSPSP_UI_PAUSEOPTIONSCOMPONENT_H_
