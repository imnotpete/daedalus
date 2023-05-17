/*
Copyright (C) 2011 Salvy6735

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
#include "SysPosix/UI/Dialogs.h"
#include "SysPosix/UI/UIContext.h"

#include "Graphics/ColourValue.h"
#include "SysPSP/Graphics/DrawText.h"

#include <SDL2/SDL.h>



CDialog	gShowDialog;


CDialog::~CDialog() {}

bool CDialog::Render( CUIContext * p_context, const char* message, bool only_dialog )
{


	for(;;)
	{
		p_context->BeginRender();

		// Draw our box for this dialog
		p_context->DrawRect( 100, 116, 280, 54, c32::White );
		p_context->DrawRect( 102, 118, 276, 50, c32(128, 128, 128) ); // Magic Grey

		//Render our text for our dialog
		p_context->SetFontStyle( CUIContext::FS_HEADING );
		p_context->DrawTextAlign(0,480,AT_CENTRE,135,message,DrawTextUtilities::TextRed);
		p_context->SetFontStyle( CUIContext::FS_REGULAR );

		// Only show a dialog, do not give a choice
		if(only_dialog)
			p_context->DrawTextAlign(0,480,AT_CENTRE,158,"Press any button to continue",DrawTextUtilities::TextWhite);
		else
			p_context->DrawTextAlign(0,480,AT_CENTRE,158,"(X) Confirm       (O) Cancel",DrawTextUtilities::TextWhite);

		p_context->EndRender();

		if( SDL_CONTROLLER_BUTTON_A )
			return true;
		else if( SDL_CONTROLLER_BUTTON_B )
			return false;
	}
}
