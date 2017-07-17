#pragma once

#include "imgui.h"

void drawIGSLogo( 
    const ImVec2& pos,
    const double time );

void aboutWindow(
    bool& visible,
    const char* title,
    const char* text,
    const ImVec2& size,
    double time );
