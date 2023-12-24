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


#include "Base/Types.h"
#include "PauseOptionsComponent.h"


CPauseOptionsComponent *	CPauseOptionsComponent::Create( CUIContext * p_context, CFunctor * on_resume, CFunctor * on_reset )
{
	return new CPauseOptionsComponent( p_context, on_resume, on_reset );
}


CPauseOptionsComponent::CPauseOptionsComponent( CUIContext * p_context, CFunctor * on_resume, CFunctor * on_reset )
:	CUIComponent( p_context )
,	mOnResume( on_resume )
,	mOnReset( on_reset )
{
	mElements.Add( new CUISpacer( 10 ) );
	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::EditPreferences ), "Edit Preferences", "Edit various preferences for this rom." ) );
	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::AdvancedOptions ), "Advanced Options", "Edit advanced options for this rom." ) );
	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::CheatOptions ), "Cheats", "Edit advanced options for this rom." ) );
	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::SaveState ), "Save State", "Save the current state." ) );
	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::LoadState ), "Load/Delete State", "Restore or delete a previously saved state." ) );
	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::TakeScreenshot ), "Take Screenshot", "Take a screenshot on resume." ) );

#ifdef DAEDALUS_DEBUG_DISPLAYLIST
		mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::DebugDisplayList ), "Debug Display List", "Debug display list on resume." ) );
#endif

#ifdef DAEDALUS_DEBUG_CONSOLE
		//mElements.Add( new CUICommandImpl( new CStaticFunctor( CPU_DumpFragmentCache ), "Dump Fragment Cache", "Dump the contents of the dynarec fragment cache to disk." ) );
	//	mElements.Add( new CUICommandImpl( new CStaticFunctor( CPU_ResetFragmentCache ), "Clear Fragment Cache", "Clear the contents of the dynarec fragment cache." ) );
	//	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::ProfileNextFrame ), "Profile Frame", "Profile the next frame on resume." ) );
#endif

	mElements.Add( new CUISpacer( 16 ) );
	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::OnResume ), "Resume Emulation", "Resume emulation." ) );

#ifdef DAEDALUS_DIALOGS
	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::ExitConfirmation ), "Return to Main Menu", "Return to the main menu." ) );
#else
	mElements.Add( new CUICommandImpl( new CMemberFunctor< CPauseOptionsComponent >( this, &CPauseOptionsComponent::OnReset ), "Return to Main Menu", "Return to the main menu." ) );
#endif
}

CPauseOptionsComponent::~CPauseOptionsComponent() {}


void	CPauseOptionsComponent::Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons )
{
	if(old_buttons != new_buttons)
	{
		if( new_buttons & PSP_CTRL_UP )
		{
			mElements.SelectPrevious();
		}
		if( new_buttons & PSP_CTRL_DOWN )
		{
			mElements.SelectNext();
		}

		CUIElement *	element( mElements.GetSelectedElement() );
		if( element != NULL )
		{
			if( new_buttons & PSP_CTRL_LEFT )
			{
				element->OnPrevious();
			}
			if( new_buttons & PSP_CTRL_RIGHT )
			{
				element->OnNext();
			}
			if( new_buttons & (PSP_CTRL_CROSS|PSP_CTRL_START) )
			{
				element->OnSelected();
			}
		}
	}
}


void	CPauseOptionsComponent::Render()
{

	mElements.Draw( mpContext, LIST_TEXT_LEFT, LIST_TEXT_WIDTH, AT_CENTRE, BELOW_MENU_MIN );

	auto element  = mElements.GetSelectedElement();
	if( element != NULL )
	{
		const char *		p_description =  element->GetDescription();

		mpContext->DrawTextArea( DESCRIPTION_AREA_LEFT,
								 DESCRIPTION_AREA_TOP,
								 DESCRIPTION_AREA_RIGHT - DESCRIPTION_AREA_LEFT,
								 DESCRIPTION_AREA_BOTTOM - DESCRIPTION_AREA_TOP,
								 p_description,
								 DrawTextUtilities::TextWhite,
								 VA_BOTTOM );
	}
}
#ifdef DAEDALUS_DIALOGS

void CPauseOptionsComponent::ExitConfirmation()
{
	if(gShowDialog.Render( mpContext,"Return to main menu?", false) )
	{
		(*mOnReset)();
	}
}
#endif

void CPauseOptionsComponent::OnResume() { (*mOnResume)(); }


void CPauseOptionsComponent::OnReset()
{
	(*mOnReset)();
}


void	CPauseOptionsComponent::EditPreferences()
{
	auto	edit_preferences =  CRomPreferencesScreen::Create( mpContext, g_ROM.mRomID );
	edit_preferences->Run();
	delete edit_preferences;
}


void	CPauseOptionsComponent::AdvancedOptions()
{
	auto advanced_options = CAdvancedOptionsScreen::Create( mpContext, g_ROM.mRomID );
	advanced_options->Run();
	delete advanced_options;
}

void	CPauseOptionsComponent::CheatOptions()
{
	auto cheat_options = CCheatOptionsScreen::Create( mpContext, g_ROM.mRomID );
	cheat_options->Run();
	delete cheat_options;
}


void	CPauseOptionsComponent::SaveState()
{
	auto component = CSavestateSelectorComponent::Create( mpContext, CSavestateSelectorComponent::AT_SAVING, new CMemberFunctor1< CPauseOptionsComponent, const char * >( this, &CPauseOptionsComponent::OnSaveStateSlotSelected ), g_ROM.settings.GameName.c_str() );
	auto screen =  CUIComponentScreen::Create( mpContext, component, SAVING_TITLE_TEXT );
	screen->Run();
	delete screen;
	(*mOnResume)();
}


void	CPauseOptionsComponent::LoadState()
{
	auto component = CSavestateSelectorComponent::Create( mpContext, CSavestateSelectorComponent::AT_LOADING, new CMemberFunctor1< CPauseOptionsComponent, const char * >( this, &CPauseOptionsComponent::OnLoadStateSlotSelected ), g_ROM.settings.GameName.c_str() );
	auto screen = CUIComponentScreen::Create( mpContext, component, LOADING_TITLE_TEXT );
	screen->Run();
	delete screen;
	(*mOnResume)();
}


void	CPauseOptionsComponent::OnSaveStateSlotSelected( const char * filename )
{
	std::filesystem::remove( filename ); // Ensure that we're re-creating the file
	CPU_RequestSaveState( filename );

	gTakeScreenshot = true;
	gTakeScreenshotSS = true;
}


void	CPauseOptionsComponent::OnLoadStateSlotSelected( const char * filename )
{
	CPU_RequestLoadState( filename );
}


void CPauseOptionsComponent::TakeScreenshot()
{
	gTakeScreenshot = true;
	(*mOnResume)();
}

#ifdef DAEDALUS_DEBUG_DISPLAYLIST

void CPauseOptionsComponent::DebugDisplayList()
{
	DLDebugger_RequestDebug();
	(*mOnResume)();
}
#endif
