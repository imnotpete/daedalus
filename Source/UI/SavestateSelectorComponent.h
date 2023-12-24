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


#ifndef SYSPSP_UI_SAVESTATESELECTORCOMPONENT_H_
#define SYSPSP_UI_SAVESTATESELECTORCOMPONENT_H_

#include <stdio.h>
#include <filesystem>
#include <chrono>
#include "UIComponent.h"
#include "Utility/Functor.h"

#include "Core/ROM.h"
#include "Interface/SaveState.h"
#include "Graphics/NativeTexture.h"
#include "Math/Vector2.h"
#include "DrawTextUtilities.h"
#include "UICommand.h"
#include "UIContext.h"
#include "UIElement.h"
#include "UIScreen.h"
#include "PSPMenu.h"
#include "SavestateSelectorComponent.h"
#include "System/IO.h"
#include "Utility/Stream.h"
#include "Utility/Translate.h"




class CSavestateSelectorComponent : public CUIComponent
{
	public:
			enum EAccessType
		{
			AT_SAVING,
			AT_LOADING,
		};


		CSavestateSelectorComponent( CUIContext * p_context, EAccessType accetype, CFunctor1< const char * > * on_slot_selected, const char *running_rom );
		~CSavestateSelectorComponent();

		// CUIScreen
		 void				Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons );
		 void				Render();
		 bool				IsFinished() const									{ return mIsFinished; }
	public:
	
		static CSavestateSelectorComponent *	Create( CUIContext * p_context, EAccessType access_type, CFunctor1< const char * > * on_savestate_selected, const char *running_rom );
		void LoadState();
		void SaveState();
		IO::Filename			current_slot_path;
		bool					isGameRunning;


	private:
		void				OnSlotSelected( u32 slot_idx );
		void				OnFolderSelected( u32 index );
		void				LoadFolders();
		void				LoadSlots();
		void				deleteSlot(u32 id_ss);
		bool					isDeletionAllowed;

	private:
		EAccessType				mAccessType;
		CFunctor1< const char * > *		mOnSlotSelected;

		u32					mSelectedSlot;
		bool					mIsFinished;
		bool	deleteButtonTriggered;

		CUIElementBag				mElements;
		std::vector<std::string> 		mElementTitle;
		bool					mSlotEmpty[ NUM_SAVESTATE_SLOTS ];
		IO::Filename			mPVFilename[ NUM_SAVESTATE_SLOTS ];
		s8						mPVExists[ NUM_SAVESTATE_SLOTS ];	//0=skip, 1=file exists, -1=show no preview
		std::shared_ptr<CNativeTexture>	mPreviewTexture;
		u32						mLastPreviewLoad;

};
#endif // SYSPSP_UI_SAVESTATESELECTORCOMPONENT_H_
