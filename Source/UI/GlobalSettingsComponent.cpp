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


#include "Base/Types.h"
#include "GlobalSettingsComponent.h"

#include <stdio.h>


CGlobalSettingsComponent::~CGlobalSettingsComponent() {}


CGlobalSettingsComponent *	CGlobalSettingsComponent::Create( CUIContext * p_context )
{
	return new CGlobalSettingsComponent( p_context );
}

CGlobalSettingsComponent::CGlobalSettingsComponent( CUIContext * p_context )
:	CUIComponent( p_context )
{

	mElements.Add( new CInfoSetting( "Display Info", "Whether to show additional info while the rom is running. Some modes are only available in DEBUG mode") );
	mElements.Add( new CViewPortSetting( "Viewport Size", "The size of the viewport on the PSP." ) );

#if DEADALUS_PSP
	if (HAVE_DVE && PSP_TV_CABLE > 0)
	{
		mElements.Add( new CBoolSetting( &gGlobalPreferences.TVEnable, "TV Output", "Whether to direct the video to the TV out.", "Yes", "No" ) );
		mElements.Add( new CBoolSetting( &gGlobalPreferences.TVLaced, "TV Interlaced", "Whether the TV needs interlaced output.", "Yes", "No" ) );
		mElements.Add( new CTVTypeSetting( "TV Type", "The aspect ratio of the TV." ) );
	}
	else
	{
		gGlobalPreferences.TVEnable = false;
	}
#endif
	mElements.Add( new CBoolSetting( &gGlobalPreferences.ForceLinearFilter,"Force Linear Filter", "Enable to force linear filter, this can improve the look of textures", "Yes", "No" ) );
	mElements.Add( new CBoolSetting( &gGlobalPreferences.RumblePak,"Controller add-on", "Enable either MemPak or RumblePak.", "RumblePak", "MemPak" ) );
	mElements.Add( new CAdjustDeadzoneSetting( mpContext, "Stick Deadzone", "Adjust the size of the deadzone applied to the PSP stick while playing. Press Start/X to edit." ) );

#if DEADALUS_PSP
	if (PSP_IS_SLIM) 
		mElements.Add( new CBoolSetting( &gGlobalPreferences.LargeROMBuffer, "ROM Buffering Mode", "File Cache, faster ROM boot but can stutter due to MS reads. ROM Buffer, no stutter but long boot time loading whole ROM into memory. Takes effect only @ ROM boot.", "File Cache", "ROM Buffer" ) );
#endif

#ifdef DAEDALUS_DEBUG_DISPLAYLIST
	mElements.Add( new CBoolSetting( &gGlobalPreferences.HighlightInexactBlendModes, "Highlight Inexact Blend Modes",	"Replace inexact blend modes with a placeholder texture.", "Yes", "No" ) );
	mElements.Add( new CBoolSetting( &gGlobalPreferences.CustomBlendModes, "Use Custom Blend Modes",	"Debugging tool to disable custom blendmodes.", "Yes", "No" ) );
#endif
	mElements.Add( new CBoolSetting( &gGlobalPreferences.BatteryWarning, "Low Battery Warning",	"Whether to allow Daedalus to notify when the battery is low.", "Yes", "No" ) );
	mElements.Add( new CColorSetting( "GUI Color", "Change GUI Color" ) );
	mElements.Add( new CLanguage( "Language", "Press X to load language" ) );
	mElements.Add( new CResetSetting( mpContext, "Reset Settings", "Will guide you to reset preferences to default, and hle cache files. Note : emulator will exit if resetting settings" ) );

}


void	CGlobalSettingsComponent::Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons )
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


void	CGlobalSettingsComponent::Render()
{
	mElements.Draw( mpContext, LIST_TEXT_LEFT, LIST_TEXT_WIDTH, AT_CENTRE, BELOW_MENU_MIN );

	CUIElement *	element( mElements.GetSelectedElement() );
	if( element != NULL )
	{
		const char *		p_description( element->GetDescription() );
		mpContext->DrawTextArea( DESCRIPTION_AREA_LEFT,
								 DESCRIPTION_AREA_TOP,
								 DESCRIPTION_AREA_RIGHT - DESCRIPTION_AREA_LEFT,
								 DESCRIPTION_AREA_BOTTOM - DESCRIPTION_AREA_TOP,
								 p_description,
								 DrawTextUtilities::TextWhite,
								 VA_BOTTOM );
	}

}
