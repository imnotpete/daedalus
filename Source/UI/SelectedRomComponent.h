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


#ifndef SYSPSP_UI_SELECTEDROMCOMPONENT_H_
#define SYSPSP_UI_SELECTEDROMCOMPONENT_H_


#include "UIComponent.h"
#include "Utility/Functor.h"

#include "Core/ROM.h"
#include "Graphics/ColourValue.h"
#include "Input/InputManager.h"
#include "DrawTextUtilities.h"
#include "AdvancedOptionsScreen.h"
#include "CheatOptionsScreen.h"
#include "PSPMenu.h"
#include "RomPreferencesScreen.h"
#include "SelectedRomComponent.h"
#include "UIContext.h"
#include "UIScreen.h"
#include "UISetting.h"
#include "UICommand.h"
#include "UISpacer.h"


class CSelectedRomComponent : public CUIComponent
{
	public:

		CSelectedRomComponent( CUIContext * p_context, CFunctor * on_start_emulation );
		~CSelectedRomComponent();
		static CSelectedRomComponent *	Create( CUIContext * p_context, CFunctor * on_start_emulation );

		 void				SetRomID( const RomID & rom_id )			{ mRomID = rom_id; }

		// CUIComponent
		 void				Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons );
		 void				Render();



	private:
		void						EditPreferences();
		void						AdvancedOptions();
		void						CheatOptions();
		void						StartEmulation();

	private:
		CFunctor *					OnStartEmulation;

		CUIElementBag				mElements;

		RomID						mRomID;
};


class RomID;

// class CSelectedRomComponent : public CUIComponent
// {
// 	public:
// 		CSelectedRomComponent( CUIContext * p_context );
// 		virtual ~CSelectedRomComponent();


// };

#endif // SYSPSP_UI_SELECTEDROMCOMPONENT_H_
