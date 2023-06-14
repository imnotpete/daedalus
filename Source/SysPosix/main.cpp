/*
Copyright (C) 2012 StrmnNrmn

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

#include "BuildOptions.h"
#include "Base/Types.h"

#include "Core/CPU.h"
#include "Debug/DBGConsole.h"
#include "Interface/RomDB.h"
#include "System/SystemInit.h"
#include "Test/BatchTest.h"
#include "System/IO.h"
#include "Config/ConfigOptions.h"
#include "SysGL/GL.h"
#include "UI/MainMenu.h"

#include <SDL2/SDL.h>
#include "third_party/imgui/imgui.h"
#include "third_party/imgui/backends/imgui_impl_sdl2.h"
#include "third_party/imgui/backends/imgui_impl_opengl3.h"

#include <vector>
#include <filesystem>
#include <iostream>


#ifdef DAEDALUS_LINUX
#include <linux/limits.h>
#endif

	bool done = false;



int main(int argc, char **argv)
{

static u32 SCR_WIDTH = 640;
static u32 SCR_HEIGHT = 480;
	// Really should be initialised in another file
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		return false;
	}
    // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
#elif defined(__APPLE__)
    // GL 3.2 Core + GLSL 150
    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#endif
	// SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
	// SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
	// SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_Window *gWindow = SDL_CreateWindow( "DaedalusX64", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_GLContext gContext = SDL_GL_CreateContext( gWindow );
	SDL_GL_MakeCurrent(gWindow, gContext);
	SDL_GL_SetSwapInterval(1);

	 // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(gWindow, gContext);
    ImGui_ImplOpenGL3_Init(glsl_version);

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	
	//Start ImGui and Render MainMenu
	while (!done)
	{
		     SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(gWindow))
                done = true;
        }
	// Portability, need to pull in the correct IMPL for the platform
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();
	MainMenu();

	ImGui::Render();
	// This needs to be done a portable way 
	// Required otherwise drawing not possible

    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	SDL_GL_SwapWindow(gWindow);
	}
}

// 	int result = 0;

	
// 	if (!System_Init())
// 	{
// 		fprintf(stderr, "System_Init failed\n");
// 		return 1;
// 	}

// Probably should put all the argc stuff in another fole
// 	if (argc > 1)
// 	{
// 		bool 			batch_test = false;
// 		const char *	filename   = NULL;

// 		for (int i = 1; i < argc; ++i)
// 		{
// 			const char * arg = argv[i];
// 			if (*arg == '-')
// 			{
// 				++arg;
// 				if( strcmp( arg, "-batch" ) == 0 )
// 				{
// 					batch_test = true;
// 					break;
// 				}
// 				else if (strcmp( arg, "-roms" ) == 0 )
// 				{
// 					if (i+1 < argc)
// 					{
// 						const char *relative_path = argv[i+1];
// 						++i;

// 						char* dir = realpath(relative_path, nullptr);
// 						CRomDB::Get()->AddRomDirectory(dir);
// 						free(dir);
// 					}
// 				}
// 			}
// 			else
// 			{
// 				filename = arg;
// 			}
// 		}

// 		if (batch_test)
// 		{
// #ifdef DAEDALUS_BATCH_TEST_ENABLED
// 				BatchTestMain(argc, argv);
// #else
// 				fprintf(stderr, "BatchTest mode is not present in this build.\n");
// #endif
// 		}
// 		else if (filename)
// 		{
// 			System_Open( filename );

// 			//
// 			// Commit the preferences and roms databases before starting to run
// 			//
// 			CRomDB::Get()->Commit();
// 			//CPreferences::Get()->Commit();

// 			CPU_Run();
// 			System_Close();
// 		}
// 	}
// 	System_Finalize();
// 	return result;
// }

//FIXME: All this stuff needs tidying

void Dynarec_ClearedCPUStuffToDo() {}

void Dynarec_SetCPUStuffToDo() {}


extern "C" {
void _EnterDynaRec()
{
	DAEDALUS_ASSERT(false, "Unimplemented");
}
}
