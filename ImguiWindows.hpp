#pragma once

#include "imgui.h"

void drawIGSLogo( 
    const ImVec2& pos,
    const double time );

	
/** Show an info about window with animated IGS logo. 
	Usage example:
	if( about_window_visible &&  
        aboutWindow( 
			ICON_FA_INFO_CIRCLE " About",
			"My fancy tools\n"
			"Abla bli blub ....\n",
			ImVec2( 400, 155 ),
			time ) )
			about_window_visible = false;
*/
bool aboutWindow(
    const char* title,
    const char* text,
    const ImVec2& size,
    double time );
