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


#ifndef SYSPSP_UI_GLOBALSETTINGSCOMPONENT_H_
#define SYSPSP_UI_GLOBALSETTINGSCOMPONENT_H_


#include "Graphics/ColourValue.h"

#include "AdjustDeadzoneScreen.h"

#include "DrawTextUtilities.h"
#include "Dialogs.h"
#include "UIComponent.h"
#include "UIContext.h"
#include "UIScreen.h"
#include "UISetting.h"
#include "PSPMenu.h"


#include "System/Thread.h"
#include "Core/FramerateLimiter.h"
#include "Interface/Preferences.h"
#include "System/IO.h"
#include "Utility/Translate.h"

#include "Input/InputManager.h"
extern u32 HAVE_DVE;
extern u32 PSP_TV_CABLE;
extern bool PSP_IS_SLIM;

namespace
{

	class CViewPortSetting : public CUISetting
	{
	public:
		CViewPortSetting( const char * name, const char * description )
			:	CUISetting( name, description )
		{
		}

		virtual	void	OnNext()				{ gGlobalPreferences.ViewportType = EViewportType( (gGlobalPreferences.ViewportType+1) % NUM_VIEWPORT_TYPES ); }
		virtual	void	OnPrevious()			{ gGlobalPreferences.ViewportType = EViewportType( (gGlobalPreferences.ViewportType + NUM_VIEWPORT_TYPES - 1) % NUM_VIEWPORT_TYPES ); }

		virtual const char *	GetSettingName() const
		{
			if ( gGlobalPreferences.TVEnable )
			{
				if ( gGlobalPreferences.TVType == TT_4_3 )
				{
					switch( gGlobalPreferences.ViewportType )
					{
					case VT_UNSCALED_4_3:	return "4:3 no overscan (640x448)";
					case VT_SCALED_4_3:		return "4:3 overscan (658x460)";
					case VT_FULLSCREEN:		return "16:9 stretch (720x460)";
					case VT_FULLSCREEN_HD:	return "16:9 (720x460)";
					}
					#ifdef DAEDALUS_DEBUG_CONSOLE
					DAEDALUS_ERROR( "Unhandled viewport type" );
					#endif
					return "?";
				}
				else
				{
					switch( gGlobalPreferences.ViewportType )
					{
					case VT_UNSCALED_4_3:	return "4:3 no overscan (528x448)";
					case VT_SCALED_4_3:		return "4:3 overscan (542x460)";
					case VT_FULLSCREEN:		return "16:9 stretch (720x460)";
					case VT_FULLSCREEN_HD:	return "16:9 (720x460)";
					}
						#ifdef DAEDALUS_DEBUG_CONSOLE
					DAEDALUS_ERROR( "Unhandled viewport type" );
					#endif
					return "?";
				}
			}
			else
			{
				switch( gGlobalPreferences.ViewportType )
				{
				case VT_UNSCALED_4_3:	return "4:3 unscaled (320x240)";
				case VT_SCALED_4_3:		return "4:3 scaled (362x272)";
				case VT_FULLSCREEN:		return "16:9 stretch (480x272)";
				case VT_FULLSCREEN_HD:	return "16:9 (480x272)";
				}
					#ifdef DAEDALUS_DEBUG_CONSOLE
				DAEDALUS_ERROR( "Unhandled viewport type" );
				#endif
				return "?";
			}
		}
	};

	class CTVTypeSetting : public CUISetting
	{
	public:
		CTVTypeSetting( const char * name, const char * description )
			:	CUISetting( name, description )
		{
		}

		virtual	void	OnNext()				{ gGlobalPreferences.TVType = ETVType( gGlobalPreferences.TVType ^ 1 ); }
		virtual	void	OnPrevious()			{ gGlobalPreferences.TVType = ETVType( gGlobalPreferences.TVType ^ 1 ); }

		virtual const char *	GetSettingName() const
		{
			switch( gGlobalPreferences.TVType )
			{
			case TT_4_3:			return "4:3 SDTV";
			case TT_WIDESCREEN:		return "16:9 HDTV";
			}
				#ifdef DAEDALUS_DEBUG_CONSOLE
			DAEDALUS_ERROR( "Unhandled TV type" );
			#endif
			return "?";
		}
	};

	class CLanguage : public CUISetting
	{
	public:
		CLanguage(  const char * name, const char * description )
			:	CUISetting( name, description )
		{
		}

		virtual	void		OnNext()		{ if (gGlobalPreferences.Language < Translate_Number() ) gGlobalPreferences.Language++; }
		virtual	void		OnPrevious()	{ if (gGlobalPreferences.Language > 0) gGlobalPreferences.Language--; }

		virtual	void			OnSelected()
		{
			Translate_Read( gGlobalPreferences.Language, "Languages/");
		}
		virtual const char *	GetSettingName() const
		{
			return Translate_Name( gGlobalPreferences.Language );
		}
	};

	class CAdjustDeadzoneSetting : public CUISetting
	{
	public:
		CAdjustDeadzoneSetting( CUIContext * p_context, const char * name, const char * description )
			:	CUISetting( name, description )
			,	mpContext( p_context )
		{
		}

		virtual	void			OnSelected()
		{
			CAdjustDeadzoneScreen *	adjust_deadzone =  CAdjustDeadzoneScreen::Create( mpContext );
			adjust_deadzone->Run();
			delete adjust_deadzone;
		}

		virtual const char *	GetSettingName() const
		{
			f32 min_deadzone = gGlobalPreferences.StickMinDeadzone;
			f32 max_deadzone = gGlobalPreferences.StickMaxDeadzone;

			static char buffer[ 10+10+1 ];
			snprintf( buffer, sizeof(buffer), "%d/%d", s32( 100.0f * min_deadzone ), s32( 100.0f * max_deadzone ) );
			return buffer;
		}

	private:
		CUIContext *			mpContext;
	};
	
// Not too sure we should be doing file management within the UI
	class CResetSetting : public CUISetting
	{
	public:
		CResetSetting(  CUIContext * p_context, const char * name, const char * description )
			:	CUISetting( name, description )
			,	mpContext( p_context )
		{
		}

		virtual	void			OnSelected()
		{
			if(gShowDialog.Render( mpContext,"Reset HLE cache?", false) )
			{
		
				if(gShowDialog.Render( mpContext,"Reset settings?", false) )
				{
					std::filesystem::remove("preferences.ini");
					std::filesystem::remove("rom.db");
					ThreadSleepMs(1000);	//safety wait for s

					gShowDialog.Render( mpContext,"Daedalus will exit now",true);

					sceKernelExitGame();
				}
			}
			if(gShowDialog.Render( mpContext,"Reset settings?", false) )
			{
				std::filesystem::remove("preferences.ini");
				std::filesystem::remove("rom.db");

				ThreadSleepMs(1000);	//safety wait for s

				gShowDialog.Render( mpContext,"Daedalus will exit now",true);

				sceKernelExitGame();
			}
		}

		virtual const char *	GetSettingName() const
		{
			return "Press X to Start";
		}

	private:
		CUIContext *			mpContext;
	};

	class CColorSetting : public CUISetting
	{
	public:
		CColorSetting(  const char * name, const char * description )
			:	CUISetting( name, description )
		{
		}

		virtual	void		OnNext()		{ gGlobalPreferences.GuiColor = EGuiColor( (gGlobalPreferences.GuiColor+1) % NUM_COLOR_TYPES ); }
		virtual	void		OnPrevious()	{ gGlobalPreferences.GuiColor = EGuiColor( (gGlobalPreferences.GuiColor + NUM_COLOR_TYPES - 1) % NUM_COLOR_TYPES ); }

		virtual const char *	GetSettingName() const
		{
			switch ( gGlobalPreferences.GuiColor )
			{
				case BLACK:		return "Black";
				case RED:		return "Red";
				case GREEN:		return "Green";
				case MAGENTA:	return "Magenta";
				case BLUE:		return "Blue";
				case TURQUOISE:	return "Turquoise";
				case ORANGE:	return "Orange";
				case PURPLE:	return "Purple";
				case GREY:		return "Grey";
			}
				#ifdef DAEDALUS_DEBUG_CONSOLE
			DAEDALUS_ERROR( "Unknown Color" );
			#endif
			return "?";
		}
	};

	class CInfoSetting : public CUISetting
	{
	public:
		CInfoSetting(  const char * name, const char * description )
			:	CUISetting( name, description )
		{
		}

		virtual	void		OnNext()		{ gGlobalPreferences.DisplayFramerate = !gGlobalPreferences.DisplayFramerate; }
		virtual	void		OnPrevious()	{ gGlobalPreferences.DisplayFramerate = !gGlobalPreferences.DisplayFramerate; }

		virtual const char *	GetSettingName() const
		{
			if ( gGlobalPreferences.DisplayFramerate )
				return "None";
			else
				return "None";
		}
	};

}


class CGlobalSettingsComponent : public CUIComponent
{
	public:

		CGlobalSettingsComponent( CUIContext * p_context );
		~CGlobalSettingsComponent();
	static CGlobalSettingsComponent *	Create( CUIContext * p_context );
		// CUIComponent
		virtual void				Update( float elapsed_time, const v2 & stick, u32 old_buttons, u32 new_buttons );
		virtual void				Render();

	private:
		CUIElementBag				mElements;
};

// class CGlobalSettingsComponent : public CUIComponent
// {
// 	public:
// 		CGlobalSettingsComponent( CUIContext * p_context );
// 		virtual ~CGlobalSettingsComponent();

// 		
// };

#endif // SYSPSP_UI_GLOBALSETTINGSCOMPONENT_H_
