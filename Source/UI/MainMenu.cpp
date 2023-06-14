/*
Copyright (C) 2023 Wally / Z2442

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

/*
Ideally this will contain the base code to control the main menu.
Sub menus such as:
Emulator Preferences
Rom Preferences
Utilities such as Screenshot or Debug options

will all be parsed in separate files for consistency

A hidden debug options menu will not be in the release build to control things like

Game Hacks
Built in Optimisations
Control of the Dynamic Recompiler
Display List Debugger
Certain tweaks

These will be headen to prevent unnessary modification to the project as often
depremental to performance and stability.

Certain Platforms require additional features -
3DS - Push the Emulated game to the top screen
PSP - TV Out Capability


*/

#include <filesystem>
#include <vector>
#include <algorithm>

#include "third_party/imgui/imgui.h"

#include "Config/ConfigOptions.h"

bool toggle_fullscreen = false;
bool ShowFPSonmenu = false;

void MainMenu()
{
    std::filesystem::path RomDir = "Roms";
    std::vector<std::filesystem::path> RomList;
    for (const auto& dir_entry : std::filesystem::directory_iterator{RomDir})
    {
        RomList.push_back(dir_entry);
    }
    std::sort(RomList.begin(), RomList.end());
    std::vector<std::string> RomLabels;

    RomLabels.reserve(RomList.size());

    for (const auto& rom : RomList)
    {
        RomLabels.push_back(rom.string());
    }

    std::vector<const char*> RomLabelsCStr;
    RomLabelsCStr.reserve(RomLabels.size());
    for (const auto& label : RomLabels)
    {
        RomLabelsCStr.push_back(label.c_str());
    }

    ImGui::Begin("Roms");
    static int item_current = 1;
    if (ImGui::ListBox("Roms", &item_current, RomLabelsCStr.data(), static_cast<int>(RomLabelsCStr.size()), -1))
    {
        // Handle the selected item

            // If SDL Key is pressed, check index then Open Game
                // SDL Key should be a variable per platform
    }

    ImGui::SameLine();
    ImGui::End();
        extern EAudioPluginMode gAudioPluginEnabled;
        if (ImGui::BeginMainMenuBar())
        {
                if (ImGui::BeginMenu("Menu"))
                {
                if (ImGui::MenuItem("Settings"))   {
                        }

                ImGui::EndMenu();
            }

                if (ImGui::BeginMenu("Debug"))
                    {
                        // if (ImGui::MenuItem("Show FPS"))   {
                        // 	if(ShowFPSonmenu == false) {ShowFPSonmenu = true;}
                        // 	else if(ShowFPSonmenu == true) {ShowFPSonmenu = false;}
                        // }

                        ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Video Mode"))
                    {
                        if (ImGui::MenuItem("Fullscreen"))   {
                            if (toggle_fullscreen == false) {
                                // SDL_SetWindowFullscreen(gWindow, SDL_TRUE);
                                toggle_fullscreen = true;
                            }
                            else if (toggle_fullscreen == true) {
                                // SDL_SetWindowFullscreen(gWindow, SDL_FALSE);
                                toggle_fullscreen = false;
                            }
                        }

                        ImGui::EndMenu();
                }


                if (ImGui::BeginMenu("Audio mode")) {

                    if (ImGui::MenuItem("Disabled")) {
                        gAudioPluginEnabled = APM_DISABLED;
                    }

                    if (ImGui::MenuItem("Enabled Async")) {
                        gAudioPluginEnabled = APM_ENABLED_ASYNC;
                    }

                    if (ImGui::MenuItem("Enabled Sync")) {
                        gAudioPluginEnabled = APM_ENABLED_SYNC;
                    }

                    ImGui::EndMenu();

                }

        
                // if(ShowFPSonmenu == true){
                // // ImGui::Text("Current FPS %#.1f", gCurrentFramerate);
                // }

                ImGui::EndMainMenuBar();

                
        }


}